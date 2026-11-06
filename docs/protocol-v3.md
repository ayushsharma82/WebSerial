# WebSerial Protocol v3

Binary protocol between the WebSerial firmware ("device") and the browser UI ("browser") over a single WebSocket. Golden test vectors live in [`test/vectors/protocol-v3.json`](../test/vectors/protocol-v3.json) and are checked by both the firmware tests and the UI tests.

v3 is a clean break from v2: there is no fallback, and a v3 UI shows "incompatible firmware" if it does not receive a `WELCOME` within 1 s of connecting.

## Conventions

- All integers are **little-endian**.
- Strings are UTF-8, not NUL-terminated, and length-prefixed.
- Stream offsets are byte positions in the device's log stream, sent as the low 32 bits. Compare them with wrap-around arithmetic (`(int32_t)(a - b)`).
- Device time is `millis()` since boot, `u32`. The browser maps it to wall-clock time using `WELCOME.device_ms` (and refines it with `PONG`).

## Framing

Every WebSocket message is **binary** and contains one or more frames back to back:

```
offset size  field
0      1     magic1   0xAB
1      1     magic2   0xCD
2      1     version  0x03
3      1     type
4      2     length   (u16, body length in bytes)
6      n     body
```

Receivers must:
- stop parsing a message at the first frame whose magic or version is wrong, or whose `length` runs past the end of the message;
- skip frames of an unknown `type`, using `length`, for forward compatibility.

## Frame types

| Type | Name | Direction | Body |
|---|---|---|---|
| `0x01` | `WELCOME` | device → browser | TLV list (see below) |
| `0x02` | `LOGS` | device → browser | `start u32` then records |
| `0x03` | `DROPPED` | device → browser | `count u32` |
| `0x04` | `ACK` | browser → device | `end u32` |
| `0x05` | `INPUT` | browser → device | raw command bytes |
| `0x06` | `PING` | browser → device | `seq u32` |
| `0x07` | `PONG` | device → browser | `seq u32`, `device_ms u32` |
| `0x10`–`0x1F` | reserved | — | extensions; receivers that don't know them skip them |

### `WELCOME` (sent once, right after the WebSocket opens)

The body is a list of TLV entries: `tag u8 | len u8 | value[len]`. Receivers ignore unknown tags.

| Tag | Name | Value |
|---|---|---|
| `0x01` | protocol | `u8` = 3 |
| `0x02` | edition | `u8`: 0 = WebSerial (this library); other values identify other editions |
| `0x03` | hardware | string, e.g. `ESP32`, `ESP8266`, `RP2040`, `RP2350` |
| `0x04` | device_id | string, e.g. `ESP32-A1B2C3` |
| `0x07` | device_ms | `u32`, device time when `WELCOME` was built |
| `0x08` | history_end | `u32` stream offset; records starting before it were logged **before this browser connected** |
| `0x09` | max_line | `u16`, longest record payload the device emits |

Tags `0x05`, `0x06` and `0x10`–`0x1F` are reserved for extensions; receivers ignore tags they don't know.

### `LOGS`

```
start u32        stream offset of the first record in this frame
records…         repeated until the end of the body:
  len   u16      payload length
  flags u8       bit0 CONT  : fragment; the next record from the same writer continues this line
                 bits 1-3   : reserved for extensions (0)
                 bits 4-7   : writer id 1-15 (0 = unknown). Lines built by different tasks/cores
                              have different writers; join a CONT fragment only with a later
                              record of the same writer (other writers' lines may come between)
  level u8       0 none, 1 error, 2 warn, 3 info, 4 debug, 5 verbose
  ms    u32      device time when the line was committed
  payload[len]   line text without the trailing "\r\n" / "\n"
```

Each record occupies `8 + len` bytes of stream, so the offset of record *k* is `start` plus the sizes of the records before it. The frame's **end offset** is `start + (body length − 4)`.

**Flow control:** after processing a `LOGS` frame, the browser must send `ACK(end)`.
- The device keeps at most `WSL_WINDOW` (default 2) unacknowledged `LOGS` frames per client, and batches whatever accumulates meanwhile into the next frame. That is what makes batching adapt to the link.
- A client that does not acknowledge for `WSL_ACK_TIMEOUT_MS` (default 30 s) is closed. It reconnects and catches up from history. With the BLOCK overflow policy, a client that has not acknowledged for `WSL_STALL_MS` (default 1 s), such as a background tab, no longer holds writers back and gets a `DROPPED` count instead.
- After reconnecting to the same boot of the device, the browser skips replayed records it already has (offsets before the end of the last frame it processed) and shows a notice if records were missed in between.
- Each client has its own position in the stream, so a slow tab never stalls others.

### `DROPPED`

Lines this client lost because the device's ring buffer overflowed before they could be sent. It's emitted immediately before the next `LOGS` frame for that client. The UI shows "N lines dropped" at that point in the log.

### `ACK`

`end` is the end offset of the last `LOGS` frame the browser has processed. It acknowledges that frame and all earlier ones.

### `INPUT`

The command text typed by the user. The device passes exactly these bytes to `onMessage()`, with no terminator added. Maximum length is `WSL_MAX_INPUT` (default 512); longer frames are truncated.

### `PING` / `PONG`

The browser sends `PING(seq)` every 3 s. The device answers `PONG(seq, device_ms)`, which gives the browser round-trip time and keeps the clock mapping fresh. No `PONG` for 10 s means the browser treats the connection as stale and reconnects.

## Connection sequence

```
browser                                  device
   │──── WebSocket upgrade ───────────────►│
   │◄─── WELCOME (history_end = H) ────────│   client cursor = oldest retained record
   │◄─── LOGS (start = tail, history…) ────│   records before H are history
   │──── ACK(end) ────────────────────────►│
   │◄─── LOGS … ───────────────────────────│   live lines, ≤ 2 frames in flight
   │──── PING(seq) ───────────────────────►│
   │◄─── PONG(seq, device_ms) ─────────────│
   │──── INPUT("reboot") ─────────────────►│   → onMessage()
```
