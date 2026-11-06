// Native unit tests for the WebSerial core (protocol + log stream).
// Run with: pio test -e native
#include <unity.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

#include "../../src/core/wsl_protocol.cpp"
#include "../../src/core/wsl_stream.cpp"

using namespace wsl;

// ---------------------------------------------------------------- helpers

static std::string g_vectors;

static void load_vectors() {
  if (!g_vectors.empty()) return;
  FILE* f = fopen("test/vectors/protocol-v3.json", "rb");
  TEST_ASSERT_NOT_NULL_MESSAGE(f, "run from the library root so test/vectors/protocol-v3.json is found");
  char buf[4096];
  size_t n;
  while ((n = fread(buf, 1, sizeof buf, f)) > 0) g_vectors.append(buf, n);
  fclose(f);
}

// Returns the "hex" of the vector named `name` (first match after its name)
static std::string vector_hex(const char* name, int* valid_frames = nullptr) {
  load_vectors();
  std::string key = std::string("\"name\": \"") + name + "\"";
  size_t at = g_vectors.find(key);
  TEST_ASSERT_TRUE_MESSAGE(at != std::string::npos, name);
  size_t h = g_vectors.find("\"hex\": \"", at) + 8;
  std::string hex = g_vectors.substr(h, g_vectors.find('"', h) - h);
  if (valid_frames) {
    size_t v = g_vectors.find("\"valid_frames\": ", h);
    *valid_frames = atoi(g_vectors.c_str() + v + 16);
  }
  return hex;
}

static std::vector<uint8_t> from_hex(const std::string& hex) {
  std::vector<uint8_t> out;
  for (size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(uint8_t(strtoul(hex.substr(i, 2).c_str(), nullptr, 16)));
  return out;
}

static std::string to_hex(const std::vector<uint8_t>& v) {
  static const char* d = "0123456789abcdef";
  std::string s;
  for (uint8_t b : v) { s += d[b >> 4]; s += d[b & 15]; }
  return s;
}

struct Record { uint8_t flags, level; uint32_t ms; std::string text; };
struct Decoded {
  std::vector<Record> records;
  uint32_t dropped = 0;
  uint32_t logs_frames = 0;
  uint32_t start = 0, end = 0;  // of the last LOGS frame
};

// Strict device->browser decoder used to check what the sender produces
static Decoded decode(const std::vector<uint8_t>& m) {
  Decoded d;
  size_t i = 0;
  while (i < m.size()) {
    TEST_ASSERT_TRUE(m.size() - i >= FRAME_HEADER_SIZE);
    TEST_ASSERT_EQUAL_HEX8(MAGIC_1, m[i]);
    TEST_ASSERT_EQUAL_HEX8(MAGIC_2, m[i + 1]);
    TEST_ASSERT_EQUAL_UINT8(PROTOCOL_VERSION, m[i + 2]);
    uint8_t type = m[i + 3];
    size_t len = get_u16(&m[i + 4]);
    TEST_ASSERT_TRUE(i + FRAME_HEADER_SIZE + len <= m.size());
    const uint8_t* b = &m[i + FRAME_HEADER_SIZE];
    if (type == FRAME_DROPPED) {
      TEST_ASSERT_EQUAL(4, len);
      d.dropped += get_u32(b);
    } else if (type == FRAME_LOGS) {
      d.logs_frames++;
      d.start = get_u32(b);
      size_t j = 4;
      while (j < len) {
        TEST_ASSERT_TRUE(len - j >= RECORD_HEADER_SIZE);
        Record r;
        size_t n = get_u16(b + j);
        r.flags = b[j + 2];
        r.level = b[j + 3];
        r.ms = get_u32(b + j + 4);
        TEST_ASSERT_TRUE(j + RECORD_HEADER_SIZE + n <= len);
        r.text.assign(reinterpret_cast<const char*>(b + j + RECORD_HEADER_SIZE), n);
        d.records.push_back(r);
        j += RECORD_HEADER_SIZE + n;
      }
      d.end = d.start + uint32_t(len - 4);
    }
    i += FRAME_HEADER_SIZE + len;
  }
  return d;
}

static void W(LogStream& s, uintptr_t owner, uint32_t now, const char* text, uint8_t level = 0) {
  s.write(owner, now, reinterpret_cast<const uint8_t*>(text), strlen(text), level);
}

// Drains everything currently sendable, acknowledging each LOGS frame for all recipients
static Decoded drain(LogStream& s, uint32_t now = 0) {
  Decoded all;
  std::vector<uint8_t> out;
  uint32_t ids[WSL_MAX_CLIENTS];
  for (int guard = 0; guard < 10000; guard++) {
    size_t n = s.next_message(now, out, ids, WSL_MAX_CLIENTS);
    if (!n) break;
    Decoded d = decode(out);
    all.dropped += d.dropped;
    all.records.insert(all.records.end(), d.records.begin(), d.records.end());
    if (d.logs_frames) {
      all.logs_frames += d.logs_frames;
      all.start = d.start;
      all.end = d.end;
      for (size_t k = 0; k < n; k++) s.on_ack(ids[k], d.end, now);
    }
  }
  return all;
}

// ---------------------------------------------------------------- protocol vectors

void test_welcome_vectors() {
  Welcome lite;
  lite.edition = 0;
  lite.hardware = "ESP32";
  lite.device_id = "ESP32-A1B2C3";
  lite.device_ms = 123456;
  lite.history_end = 0x100;
  lite.max_line = 256;
  std::vector<uint8_t> out;
  append_welcome(out, lite);
  TEST_ASSERT_EQUAL_STRING(vector_hex("welcome_lite").c_str(), to_hex(out).c_str());

  // Extension entries are appended verbatim through Welcome::extra
  std::vector<uint8_t> extra;
  append_tlv_str(extra, 0x05, "ignored");
  append_tlv_u16(extra, 0x10, 7);
  append_tlv_str(extra, 0x05, "");  // empty strings are omitted
  lite.extra = extra.data();
  lite.extra_len = extra.size();
  out.clear();
  append_welcome(out, lite);
  TEST_ASSERT_EQUAL_STRING(vector_hex("welcome_with_extension_tags").c_str(), to_hex(out).c_str());
}

void test_small_frame_vectors() {
  std::vector<uint8_t> out;
  append_dropped(out, 5);
  TEST_ASSERT_EQUAL_STRING(vector_hex("dropped").c_str(), to_hex(out).c_str());

  uint8_t pong[PONG_FRAME_SIZE];
  write_pong(pong, 7, 5000);
  TEST_ASSERT_EQUAL_STRING(vector_hex("pong").c_str(), to_hex(std::vector<uint8_t>(pong, pong + PONG_FRAME_SIZE)).c_str());

  // dropped_then_logs starts with the DROPPED(3) frame
  out.clear();
  append_dropped(out, 3);
  TEST_ASSERT_EQUAL_STRING(vector_hex("dropped_then_logs").substr(0, out.size() * 2).c_str(), to_hex(out).c_str());
}

void test_logs_vector_from_stream() {
  static uint8_t storage[1024];
  LogStream s(storage, sizeof storage);
  uint32_t hist;
  TEST_ASSERT_TRUE(s.add_client(1, 0, &hist));
  W(s, 1, 0, "12345678\n");  // 16-byte record so the vector's frame starts at offset 16
  drain(s);
  W(s, 1, 1000, "hello\n");
  W(s, 1, 1001, "\n");
  W(s, 1, 1001, "Connecting...");
  s.flush_idle(1002, 1);
  W(s, 2, 1500, "WiFi up\r\n", 3);
  std::vector<uint8_t> out;
  uint32_t ids[WSL_MAX_CLIENTS];
  TEST_ASSERT_EQUAL(1, s.next_message(0, out, ids, WSL_MAX_CLIENTS));
  TEST_ASSERT_EQUAL_STRING(vector_hex("logs_records").c_str(), to_hex(out).c_str());
}

struct CountingHandler : InboundHandler {
  std::vector<uint32_t> acks, pings;
  std::vector<std::string> inputs;
  void on_ack(uint32_t end) override { acks.push_back(end); }
  void on_input(const uint8_t* d, size_t n) override { inputs.push_back(std::string(reinterpret_cast<const char*>(d), n)); }
  void on_ping(uint32_t seq) override { pings.push_back(seq); }
};

void test_inbound_vectors() {
  CountingHandler h;
  std::vector<uint8_t> m = from_hex(vector_hex("ack"));
  TEST_ASSERT_EQUAL(1, parse_inbound(m.data(), m.size(), h));
  m = from_hex(vector_hex("input"));
  TEST_ASSERT_EQUAL(1, parse_inbound(m.data(), m.size(), h));
  m = from_hex(vector_hex("input_utf8"));
  TEST_ASSERT_EQUAL(1, parse_inbound(m.data(), m.size(), h));
  m = from_hex(vector_hex("ping"));
  TEST_ASSERT_EQUAL(1, parse_inbound(m.data(), m.size(), h));
  TEST_ASSERT_EQUAL(1, h.acks.size());
  TEST_ASSERT_EQUAL_UINT32(42, h.acks[0]);
  TEST_ASSERT_EQUAL_STRING("reboot", h.inputs[0].c_str());
  TEST_ASSERT_EQUAL_STRING("temp=21.5\xc2\xb0" "C", h.inputs[1].c_str());
  TEST_ASSERT_EQUAL_UINT32(7, h.pings[0]);

  // Input longer than max_input is truncated, never over-read
  m = from_hex(vector_hex("input"));
  CountingHandler t;
  parse_inbound(m.data(), m.size(), t, 3);
  TEST_ASSERT_EQUAL_STRING("reb", t.inputs[0].c_str());
}

void test_malformed_vectors() {
  const char* names[] = {"empty", "bad_magic", "v2_version", "truncated_header", "length_past_end",
                         "unknown_type_skipped", "valid_then_garbage", "short_ack_body"};
  for (const char* n : names) {
    int expected = -1;
    std::vector<uint8_t> m = from_hex(vector_hex(n, &expected));
    CountingHandler h;
    TEST_ASSERT_EQUAL_MESSAGE(expected, parse_inbound(m.data(), m.size(), h), n);
  }
  CountingHandler h;
  TEST_ASSERT_EQUAL(0, parse_inbound(nullptr, 0, h));
}

void test_inbound_fuzz_never_overreads() {
  // Random bytes in exactly-sized heap buffers: AddressSanitizer catches any over-read
  srand(1234);
  for (int iter = 0; iter < 20000; iter++) {
    size_t n = size_t(rand() % 64);
    uint8_t* buf = static_cast<uint8_t*>(malloc(n ? n : 1));
    for (size_t i = 0; i < n; i++) buf[i] = uint8_t(rand());
    if (n >= 3 && rand() % 2) { buf[0] = MAGIC_1; buf[1] = MAGIC_2; buf[2] = PROTOCOL_VERSION; }
    CountingHandler h;
    parse_inbound(buf, n, h);
    for (auto& s : h.inputs) TEST_ASSERT_TRUE(s.size() <= n);
    free(buf);
  }
}

// ---------------------------------------------------------------- line assembly

void test_lines_and_partials() {
  static uint8_t storage[4096];
  LogStream s(storage, sizeof storage);
  uint32_t hist;
  s.add_client(9, 0, &hist);
  W(s, 1, 1, "hello\r\n");
  W(s, 1, 1, "a");
  W(s, 1, 1, "b");
  W(s, 1, 1, "c\n");
  W(s, 1, 1, "A1 ");          // writer 1 starts a line
  W(s, 2, 1, "B1\n");         // writer 2 finishes a whole line meanwhile
  W(s, 1, 1, "A2\r\n");       // writer 1 completes its line: no interleaving
  W(s, 3, 5, "Connecting...");
  TEST_ASSERT_TRUE(s.has_partial());
  TEST_ASSERT_FALSE(s.flush_idle(10, 20));  // not idle long enough
  TEST_ASSERT_TRUE(s.flush_idle(25, 20));
  TEST_ASSERT_FALSE(s.has_partial());
  Decoded d = drain(s);
  const char* want[] = {"hello", "abc", "B1", "A1 A2", "Connecting..."};
  TEST_ASSERT_EQUAL(5, d.records.size());
  for (int i = 0; i < 5; i++) TEST_ASSERT_EQUAL_STRING(want[i], d.records[i].text.c_str());
  TEST_ASSERT_EQUAL_HEX8(RECORD_CONT, d.records[4].flags & 0x0F);
  TEST_ASSERT_EQUAL_UINT32(25, d.records[4].ms);
}

void test_long_line_split() {
  static uint8_t storage[4096];
  LogStream s(storage, sizeof storage);
  uint32_t hist;
  s.add_client(1, 0, &hist);
  std::string line(WSL_MAX_LINE * 2 + 88, 'x');
  line += "\n";
  W(s, 1, 0, line.c_str());
  Decoded d = drain(s);
  TEST_ASSERT_EQUAL(3, d.records.size());
  TEST_ASSERT_EQUAL(WSL_MAX_LINE, d.records[0].text.size());
  TEST_ASSERT_EQUAL_HEX8(RECORD_CONT, d.records[0].flags & 0x0F);
  TEST_ASSERT_EQUAL_HEX8(RECORD_CONT, d.records[1].flags & 0x0F);
  TEST_ASSERT_EQUAL(88, d.records[2].text.size());
  TEST_ASSERT_EQUAL_HEX8(0, d.records[2].flags & 0x0F);
}

void test_more_writers_than_slots() {
  static uint8_t storage[4096];
  LogStream s(storage, sizeof storage);
  uint32_t hist;
  s.add_client(1, 0, &hist);
  for (uintptr_t w = 1; w <= WSL_WRITER_SLOTS + 1; w++) W(s, w, uint32_t(w), "part");
  Decoded d = drain(s);
  TEST_ASSERT_EQUAL(1, d.records.size());  // the oldest writer's line was closed to free its slot
  TEST_ASSERT_EQUAL_HEX8(0, d.records[0].flags & 0x0F);
  TEST_ASSERT_EQUAL(1, d.records[0].flags >> RECORD_WRITER_SHIFT);
}

void test_fragments_carry_writer() {
  // Found on hardware: a partial line flushed after idle must only be joined with its own writer's next record
  static uint8_t storage[4096];
  LogStream s(storage, sizeof storage);
  uint32_t hist;
  s.add_client(1, 0, &hist);
  W(s, 7, 0, "Connecting to sensor...");   // network task, no newline yet
  s.flush_idle(30, 20);                     // shown after 20 ms as a fragment
  W(s, 8, 31, "seq=1 tick\n");               // loop task, complete line
  W(s, 9, 32, "B ");                         // third writer must not reuse writer 7's open slot
  W(s, 7, 40, " done\n");                     // network task ends its line
  W(s, 9, 41, "line\n");
  Decoded d = drain(s);
  TEST_ASSERT_EQUAL(4, d.records.size());
  const Record& a = d.records[0];
  TEST_ASSERT_EQUAL_STRING("Connecting to sensor...", a.text.c_str());
  TEST_ASSERT_EQUAL_HEX8(RECORD_CONT, a.flags & 0x0F);
  uint8_t wa = a.flags >> RECORD_WRITER_SHIFT, wb = d.records[1].flags >> RECORD_WRITER_SHIFT;
  TEST_ASSERT_TRUE(wa >= 1 && wb >= 1 && wa != wb);
  TEST_ASSERT_EQUAL_STRING("seq=1 tick", d.records[1].text.c_str());
  TEST_ASSERT_EQUAL_STRING(" done", d.records[2].text.c_str());
  TEST_ASSERT_EQUAL(wa, d.records[2].flags >> RECORD_WRITER_SHIFT);   // same writer: UI joins these two
  TEST_ASSERT_EQUAL_HEX8(0, d.records[2].flags & 0x0F);
  TEST_ASSERT_EQUAL_STRING("B line", d.records[3].text.c_str());
  TEST_ASSERT_TRUE((d.records[3].flags >> RECORD_WRITER_SHIFT) != wa);
}

// ---------------------------------------------------------------- history, overflow, flow control

void test_history_replay() {
  static uint8_t storage[1024];
  LogStream s(storage, sizeof storage);
  W(s, 1, 0, "boot\n");     // logged before anyone connected
  W(s, 1, 0, "wifi ok\n");
  uint32_t hist = 0;
  TEST_ASSERT_TRUE(s.add_client(5, 0, &hist));
  TEST_ASSERT_EQUAL_UINT32(uint32_t(s.head()), hist);
  W(s, 1, 0, "live\n");
  Decoded d = drain(s);
  TEST_ASSERT_EQUAL(3, d.records.size());
  TEST_ASSERT_EQUAL_STRING("boot", d.records[0].text.c_str());
  TEST_ASSERT_EQUAL_STRING("live", d.records[2].text.c_str());
  // Without clients the ring just keeps the newest history; nothing counts as dropped
  s.remove_client(5);
  for (int i = 0; i < 500; i++) W(s, 1, 0, "filler line\n");
  TEST_ASSERT_EQUAL_UINT64(0, s.lines_dropped());
}

void test_overflow_drops_are_reported() {
  static uint8_t storage[(WSL_MAX_LINE + 8) * 2];
  LogStream s(storage, sizeof storage);
  uint32_t hist;
  s.add_client(1, 0, &hist);
  for (int i = 0; i < 200; i++) {
    char line[16];
    snprintf(line, sizeof line, "line%03d\n", i);
    W(s, 1, 0, line);
  }
  Decoded d = drain(s);
  TEST_ASSERT_EQUAL_STRING("line199", d.records.back().text.c_str());
  TEST_ASSERT_EQUAL(200, d.records.size() + d.dropped);
  TEST_ASSERT_TRUE(d.dropped > 0);
}

void test_window_and_acks() {
  static uint8_t storage[4096];
  LogStream s(storage, sizeof storage);
  uint32_t hist, ids[WSL_MAX_CLIENTS];
  s.add_client(1, 0, &hist);
  std::vector<uint8_t> out;
  std::vector<uint32_t> ends;
  for (int i = 0; i < WSL_WINDOW; i++) {
    W(s, 1, 0, "x\n");
    TEST_ASSERT_EQUAL(1, s.next_message(0, out, ids, WSL_MAX_CLIENTS));
    ends.push_back(decode(out).end);
  }
  W(s, 1, 0, "batched 1\n");
  W(s, 1, 0, "batched 2\n");
  TEST_ASSERT_FALSE(s.sendable());
  TEST_ASSERT_EQUAL(0, s.next_message(0, out, ids, WSL_MAX_CLIENTS));  // window full
  s.on_ack(1, ends[0], 0);
  TEST_ASSERT_TRUE(s.sendable());
  TEST_ASSERT_EQUAL(1, s.next_message(0, out, ids, WSL_MAX_CLIENTS));
  TEST_ASSERT_EQUAL(2, decode(out).records.size());                      // both lines in one frame

  // Stale detection: oldest unacknowledged frame older than the timeout
  uint32_t stale[WSL_MAX_CLIENTS];
  TEST_ASSERT_EQUAL(0, s.stale_clients(100, 5000, stale, WSL_MAX_CLIENTS));
  TEST_ASSERT_EQUAL(1, s.stale_clients(6000, 5000, stale, WSL_MAX_CLIENTS));
  // A cumulative ACK clears everything
  s.on_ack(1, decode(out).end, 0);
  TEST_ASSERT_EQUAL(0, s.stale_clients(6000, 5000, stale, WSL_MAX_CLIENTS));
}

void test_times_from_the_future_are_not_stale() {
  // Found on hardware: loop() read millis() before taking the lock while another task sent a frame,
  // so sent_at was later than "now" and the unsigned age wrapped to ~49 days: healthy clients were closed.
  static uint8_t storage[4096];
  LogStream s(storage, sizeof storage);
  uint32_t hist, ids[WSL_MAX_CLIENTS], stale[WSL_MAX_CLIENTS];
  s.add_client(1, 0, &hist);
  W(s, 1, 1000, "x\n");
  std::vector<uint8_t> out;
  TEST_ASSERT_EQUAL(1, s.next_message(1000, out, ids, WSL_MAX_CLIENTS));  // sent at t=1000
  TEST_ASSERT_EQUAL(0, s.stale_clients(999, 5000, stale, WSL_MAX_CLIENTS)); // "now" read just before
  TEST_ASSERT_EQUAL(0, s.stale_clients(5999, 5000, stale, WSL_MAX_CLIENTS));
  TEST_ASSERT_EQUAL(1, s.stale_clients(6000, 5000, stale, WSL_MAX_CLIENTS));
  // Same for partial lines: written at t=1000, checked with an older "now"
  W(s, 2, 1000, "partial");
  TEST_ASSERT_FALSE(s.flush_idle(999, 20));
  TEST_ASSERT_TRUE(s.has_partial());
}

void test_clients_share_and_diverge() {
  static uint8_t storage[4096];
  LogStream s(storage, sizeof storage);
  uint32_t hist, ids[WSL_MAX_CLIENTS];
  s.add_client(1, 0, &hist);
  s.add_client(2, 0, &hist);
  W(s, 1, 0, "same for both\n");
  std::vector<uint8_t> out;
  TEST_ASSERT_EQUAL(2, s.next_message(0, out, ids, WSL_MAX_CLIENTS));  // one shared buffer, two recipients
  s.on_ack(1, decode(out).end, 0);  // client 2 never acknowledges (slow tab)
  size_t to_client_1 = 0;
  for (int i = 0; i < WSL_WINDOW + 3; i++) {
    W(s, 1, 0, "more\n");
    while (size_t n = s.next_message(0, out, ids, WSL_MAX_CLIENTS)) {
      for (size_t k = 0; k < n; k++) {
        if (ids[k] == 1) { s.on_ack(1, decode(out).end, 0); to_client_1 += decode(out).records.size(); }
      }
    }
  }
  TEST_ASSERT_EQUAL(WSL_WINDOW + 3, to_client_1);   // client 1 kept streaming despite client 2
  s.remove_client(2);
  TEST_ASSERT_EQUAL(1, s.client_count());
}

void test_has_space_for_block_policy() {
  static uint8_t storage[(WSL_MAX_LINE + 8) * 2];
  LogStream s(storage, sizeof storage);
  TEST_ASSERT_TRUE(s.has_space_for(sizeof storage));  // no clients: history may always be overwritten
  uint32_t hist;
  s.add_client(1, 0, &hist);
  size_t unsent = 0;
  while (unsent + 18 <= sizeof storage - 40) { W(s, 1, 0, "0123456789\n"); unsent += 18; }
  TEST_ASSERT_FALSE(s.has_space_for(41 + (sizeof storage - 40 - unsent)));
  drain(s);
  TEST_ASSERT_TRUE(s.has_space_for(sizeof storage));
}

void test_stalled_client_does_not_block_writers() {
  // Found on hardware: a background tab acknowledging slowly held BLOCK writers back
  static uint8_t storage[(WSL_MAX_LINE + 8) * 2];
  LogStream s(storage, sizeof storage);
  uint32_t hist, ids[WSL_MAX_CLIENTS];
  s.add_client(1, 0, &hist);
  std::vector<uint8_t> out;
  W(s, 1, 100, "first\n");
  TEST_ASSERT_EQUAL(1, s.next_message(100, out, ids, WSL_MAX_CLIENTS));  // in flight since t=100, never acked
  while (s.has_space_for(18)) W(s, 1, 100, "0123456789\n");               // fill the ring with unsent lines
  TEST_ASSERT_FALSE(s.has_space_for(18, 600, 1000));   // stalled for 500 ms: still waits
  TEST_ASSERT_TRUE(s.has_space_for(18, 1100, 1000));   // stalled for 1 s: no longer holds writers back
  TEST_ASSERT_FALSE(s.has_space_for(18, 99, 1000));    // clock read before the send: not stalled
}

void test_offsets_wrap_u32() {
  // ~4.8 MB through a small ring: offsets pass 2^32 / 1000 times over, ACKs keep matching
  static uint8_t storage[(WSL_MAX_LINE + 8) * 2];
  LogStream s(storage, sizeof storage);
  uint32_t hist, ids[WSL_MAX_CLIENTS];
  s.add_client(1, 0, &hist);
  std::vector<uint8_t> out;
  for (int i = 0; i < 300000; i++) {
    W(s, 1, 0, "0123456789\n");
    while (s.next_message(0, out, ids, WSL_MAX_CLIENTS)) s.on_ack(1, decode(out).end, 0);
  }
  TEST_ASSERT_EQUAL_UINT64(0, s.lines_dropped());
}

void test_stream_fuzz_accounting() {
  // Random writers, sizes, acks and flushes: every committed line is delivered intact or counted dropped
  static uint8_t storage[(WSL_MAX_LINE + 8) * 3];
  LogStream s(storage, sizeof storage);
  uint32_t hist, ids[WSL_MAX_CLIENTS];
  s.add_client(1, 0, &hist);
  srand(99);
  size_t committed_lines = 0, delivered = 0, dropped = 0;
  std::vector<uint8_t> out;
  uint32_t last_end = 0;
  bool unacked = false;
  for (int iter = 0; iter < 200000; iter++) {
    int r = rand() % 10;
    if (r < 6) {
      std::string line = "L" + std::to_string(iter) + ":" + std::string(size_t(rand() % 40), 'z') + "\n";
      W(s, uintptr_t(1 + rand() % 3), uint32_t(iter), line.c_str());
      committed_lines++;
    } else if (r < 8) {
      if (s.next_message(uint32_t(iter), out, ids, WSL_MAX_CLIENTS)) {
        Decoded d = decode(out);
        for (auto& rec : d.records) TEST_ASSERT_EQUAL('L', rec.text[0]);
        delivered += d.records.size();
        dropped += d.dropped;
        if (d.logs_frames) { last_end = d.end; unacked = true; }
      }
    } else if (unacked) {
      s.on_ack(1, last_end, uint32_t(iter));  // cumulative
      unacked = false;
    }
  }
  if (unacked) s.on_ack(1, last_end, 0);
  Decoded d = drain(s);
  delivered += d.records.size();
  dropped += d.dropped;
  TEST_ASSERT_EQUAL(committed_lines, delivered + dropped);
  TEST_ASSERT_TRUE(dropped > 0);   // the small ring must have overflowed
}

void setUp() {}
void tearDown() {}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_welcome_vectors);
  RUN_TEST(test_small_frame_vectors);
  RUN_TEST(test_logs_vector_from_stream);
  RUN_TEST(test_inbound_vectors);
  RUN_TEST(test_malformed_vectors);
  RUN_TEST(test_inbound_fuzz_never_overreads);
  RUN_TEST(test_lines_and_partials);
  RUN_TEST(test_long_line_split);
  RUN_TEST(test_more_writers_than_slots);
  RUN_TEST(test_fragments_carry_writer);
  RUN_TEST(test_history_replay);
  RUN_TEST(test_overflow_drops_are_reported);
  RUN_TEST(test_window_and_acks);
  RUN_TEST(test_times_from_the_future_are_not_stale);
  RUN_TEST(test_clients_share_and_diverge);
  RUN_TEST(test_has_space_for_block_policy);
  RUN_TEST(test_stalled_client_does_not_block_writers);
  RUN_TEST(test_offsets_wrap_u32);
  RUN_TEST(test_stream_fuzz_accounting);
  return UNITY_END();
}
