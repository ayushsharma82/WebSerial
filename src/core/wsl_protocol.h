// WebSerial protocol v3 encoding/decoding (see docs/protocol-v3.md). No Arduino dependencies.
#ifndef WSL_PROTOCOL_H
#define WSL_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace wsl {

static const uint8_t MAGIC_1 = 0xAB;
static const uint8_t MAGIC_2 = 0xCD;
static const uint8_t PROTOCOL_VERSION = 3;
static const size_t FRAME_HEADER_SIZE = 6;
static const size_t RECORD_HEADER_SIZE = 8;

enum FrameType : uint8_t {
  FRAME_WELCOME = 0x01,
  FRAME_LOGS = 0x02,
  FRAME_DROPPED = 0x03,
  FRAME_ACK = 0x04,
  FRAME_INPUT = 0x05,
  FRAME_PING = 0x06,
  FRAME_PONG = 0x07,
};

enum RecordFlags : uint8_t {
  RECORD_CONT = 0x01,    // fragment; continued by the next record from the same writer
                         // bits 1-3: reserved for extensions
};
// Bits 4-7 of a record's flags: the writer (1-15; 0 = unknown) whose line a CONT fragment belongs to
static const uint8_t RECORD_WRITER_SHIFT = 4;
static const uint8_t RECORD_WRITER_MASK = 0xF0;

// WELCOME TLV tags. 0x05, 0x06 and 0x10-0x1F are reserved for extensions (Welcome::extra).
enum WelcomeTag : uint8_t {
  TAG_PROTOCOL = 0x01,
  TAG_EDITION = 0x02,
  TAG_HARDWARE = 0x03,
  TAG_DEVICE_ID = 0x04,
  TAG_DEVICE_MS = 0x07,
  TAG_HISTORY_END = 0x08,
  TAG_MAX_LINE = 0x09,
};

inline void put_u16(uint8_t* p, uint16_t v) { p[0] = uint8_t(v); p[1] = uint8_t(v >> 8); }
inline void put_u32(uint8_t* p, uint32_t v) { p[0] = uint8_t(v); p[1] = uint8_t(v >> 8); p[2] = uint8_t(v >> 16); p[3] = uint8_t(v >> 24); }
inline uint16_t get_u16(const uint8_t* p) { return uint16_t(p[0] | (p[1] << 8)); }
inline uint32_t get_u32(const uint8_t* p) { return uint32_t(p[0]) | (uint32_t(p[1]) << 8) | (uint32_t(p[2]) << 16) | (uint32_t(p[3]) << 24); }

// Writes the 6-byte frame header at dst
void write_frame_header(uint8_t* dst, uint8_t type, uint16_t body_len);

struct Welcome {
  uint8_t edition = 0;              // 0 = Lite; other values identify other editions
  const char* hardware = nullptr;
  const char* device_id = nullptr;
  uint32_t device_ms = 0;
  uint32_t history_end = 0;
  uint16_t max_line = 0;
  const uint8_t* extra = nullptr;   // extension TLV entries appended verbatim (reserved tags only)
  size_t extra_len = 0;
};

// TLV helpers for building WELCOME extension entries
void append_tlv(std::vector<uint8_t>& out, uint8_t tag, const uint8_t* value, size_t len);
void append_tlv_str(std::vector<uint8_t>& out, uint8_t tag, const char* s);  // omitted when null or empty
void append_tlv_u8(std::vector<uint8_t>& out, uint8_t tag, uint8_t v);
void append_tlv_u16(std::vector<uint8_t>& out, uint8_t tag, uint16_t v);
void append_tlv_u32(std::vector<uint8_t>& out, uint8_t tag, uint32_t v);

// Appends a complete WELCOME frame to out
void append_welcome(std::vector<uint8_t>& out, const Welcome& w);

// Appends a DROPPED frame to out
void append_dropped(std::vector<uint8_t>& out, uint32_t count);

// Writes a PONG frame (14 bytes) into dst and returns its size
size_t write_pong(uint8_t* dst, uint32_t seq, uint32_t device_ms);
static const size_t PONG_FRAME_SIZE = FRAME_HEADER_SIZE + 8;

// Callbacks for frames received from the browser
class InboundHandler {
 public:
  virtual ~InboundHandler() {}
  virtual void on_ack(uint32_t end) = 0;
  virtual void on_input(const uint8_t* data, size_t len) = 0;
  virtual void on_ping(uint32_t seq) = 0;
};

// Parses every frame in one WebSocket message. Stops at the first malformed frame,
// skips unknown types, and never reads past len. Returns the number of frames handled.
size_t parse_inbound(const uint8_t* data, size_t len, InboundHandler& handler, size_t max_input = 512);

}  // namespace wsl

#endif // WSL_PROTOCOL_H
