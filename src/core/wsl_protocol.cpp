#include "wsl_protocol.h"

#include <string.h>

namespace wsl {

void write_frame_header(uint8_t* dst, uint8_t type, uint16_t body_len) {
  dst[0] = MAGIC_1;
  dst[1] = MAGIC_2;
  dst[2] = PROTOCOL_VERSION;
  dst[3] = type;
  put_u16(dst + 4, body_len);
}

void append_tlv(std::vector<uint8_t>& out, uint8_t tag, const uint8_t* value, size_t len) {
  if (len > 255) len = 255;
  out.push_back(tag);
  out.push_back(uint8_t(len));
  out.insert(out.end(), value, value + len);
}

void append_tlv_str(std::vector<uint8_t>& out, uint8_t tag, const char* s) {
  if (s && *s) append_tlv(out, tag, reinterpret_cast<const uint8_t*>(s), strlen(s));
}

void append_tlv_u8(std::vector<uint8_t>& out, uint8_t tag, uint8_t v) { append_tlv(out, tag, &v, 1); }
void append_tlv_u16(std::vector<uint8_t>& out, uint8_t tag, uint16_t v) { uint8_t b[2]; put_u16(b, v); append_tlv(out, tag, b, 2); }
void append_tlv_u32(std::vector<uint8_t>& out, uint8_t tag, uint32_t v) { uint8_t b[4]; put_u32(b, v); append_tlv(out, tag, b, 4); }

void append_welcome(std::vector<uint8_t>& out, const Welcome& w) {
  size_t start = out.size();
  out.resize(start + FRAME_HEADER_SIZE);
  append_tlv_u8(out, TAG_PROTOCOL, PROTOCOL_VERSION);
  append_tlv_u8(out, TAG_EDITION, w.edition);
  append_tlv_str(out, TAG_HARDWARE, w.hardware);
  append_tlv_str(out, TAG_DEVICE_ID, w.device_id);
  append_tlv_u32(out, TAG_DEVICE_MS, w.device_ms);
  append_tlv_u32(out, TAG_HISTORY_END, w.history_end);
  append_tlv_u16(out, TAG_MAX_LINE, w.max_line);
  if (w.extra && w.extra_len) out.insert(out.end(), w.extra, w.extra + w.extra_len);
  write_frame_header(&out[start], FRAME_WELCOME, uint16_t(out.size() - start - FRAME_HEADER_SIZE));
}

void append_dropped(std::vector<uint8_t>& out, uint32_t count) {
  size_t start = out.size();
  out.resize(start + FRAME_HEADER_SIZE + 4);
  write_frame_header(&out[start], FRAME_DROPPED, 4);
  put_u32(&out[start + FRAME_HEADER_SIZE], count);
}

size_t write_pong(uint8_t* dst, uint32_t seq, uint32_t device_ms) {
  write_frame_header(dst, FRAME_PONG, 8);
  put_u32(dst + FRAME_HEADER_SIZE, seq);
  put_u32(dst + FRAME_HEADER_SIZE + 4, device_ms);
  return PONG_FRAME_SIZE;
}

size_t parse_inbound(const uint8_t* data, size_t len, InboundHandler& handler, size_t max_input) {
  size_t handled = 0;
  size_t i = 0;
  while (data && len - i >= FRAME_HEADER_SIZE) {
    const uint8_t* f = data + i;
    if (f[0] != MAGIC_1 || f[1] != MAGIC_2 || f[2] != PROTOCOL_VERSION) break;
    size_t body_len = get_u16(f + 4);
    if (body_len > len - i - FRAME_HEADER_SIZE) break;
    const uint8_t* body = f + FRAME_HEADER_SIZE;
    switch (f[3]) {
      case FRAME_ACK:
        if (body_len >= 4) { handler.on_ack(get_u32(body)); handled++; }
        break;
      case FRAME_INPUT:
        handler.on_input(body, body_len < max_input ? body_len : max_input);
        handled++;
        break;
      case FRAME_PING:
        if (body_len >= 4) { handler.on_ping(get_u32(body)); handled++; }
        break;
      default:
        break;  // unknown or device->browser type: skip
    }
    i += FRAME_HEADER_SIZE + body_len;
  }
  return handled;
}

}  // namespace wsl
