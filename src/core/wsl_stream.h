// WebSerial log stream: per-writer line assembly, a fixed-size ring of complete line records,
// and per-client cursors with flow control. Not thread-safe: the caller serialises access.
// No Arduino dependencies, so it is unit-tested natively (test/test_core).
#ifndef WSL_STREAM_H
#define WSL_STREAM_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

#include "wsl_config.h"
#include "wsl_protocol.h"

namespace wsl {

class LogStream {
 public:
  LogStream(uint8_t* storage, size_t capacity);

  // ---- Producer side

  // Appends bytes from one writer (task/core id, non-zero). Complete lines become records;
  // partial lines wait in the writer's slot so lines from different writers never interleave.
  // Returns true if at least one record was committed.
  bool write(uintptr_t owner, uint32_t now, const uint8_t* data, size_t len, uint8_t level = 0);

  // Commits partial lines idle for at least idle_ms as CONT fragments.
  bool flush_idle(uint32_t now, uint32_t idle_ms);
  bool has_partial() const;

  // True if `bytes` more can be committed without overwriting lines a connected client
  // has not received yet (used by the BLOCK overflow policy). With stall_ms > 0, clients whose
  // oldest unacknowledged frame is at least that old are ignored.
  bool has_space_for(size_t bytes, uint32_t now = 0, uint32_t stall_ms = 0) const;

  // ---- Clients

  // Registers a client. Its cursor starts at the oldest retained record, so it receives history.
  // history_end receives the stream offset where live lines begin. Returns false if no slot is free.
  bool add_client(uint32_t id, uint32_t now, uint32_t* history_end);
  void remove_client(uint32_t id);
  void on_ack(uint32_t id, uint32_t end, uint32_t now);
  size_t client_count() const;

  // Writes the ids of clients whose oldest unacknowledged frame is older than timeout_ms.
  size_t stale_clients(uint32_t now, uint32_t timeout_ms, uint32_t* ids, size_t max_ids) const;

  // ---- Sender

  // True if next_message() would produce something (cheap check before allocating a buffer)
  bool sendable() const;

  // Builds the next message for a client with window credit and pending data (or pending drops).
  // All clients at the same stream position get the same bytes; their ids are written to ids.
  // Returns the number of recipients, 0 when there is nothing to send.
  size_t next_message(uint32_t now, std::vector<uint8_t>& out, uint32_t* ids, size_t max_ids,
                      size_t max_frame = WSL_MAX_FRAME);

  // ---- Introspection

  uint64_t head() const { return _head; }
  uint64_t tail() const { return _tail; }
  uint64_t lines_dropped() const { return _dropped_total; }  // records evicted before some client received them
  size_t capacity() const { return _cap; }

 private:
  struct Slot {
    uintptr_t owner = 0;
    uint32_t since = 0;
    uint16_t len = 0;
    uint8_t level = 0;
    bool open = false;  // a CONT fragment was sent and the line is not ended yet
    uint8_t data[WSL_MAX_LINE];
  };

  struct Client {
    bool active = false;
    uint32_t id = 0;
    uint64_t cursor = 0;
    uint32_t dropped = 0;
    uint8_t inflight = 0;
    uint64_t pending_end[WSL_WINDOW];
    uint32_t sent_at[WSL_WINDOW];
  };

  void commit(uint32_t ms, uint8_t flags, uint8_t level, const uint8_t* data, size_t len);
  void emit(Slot& slot, uint32_t ms, uint8_t flags);
  uint8_t writer_of(const Slot& slot) const;
  Slot* slot_for(uintptr_t owner, uint32_t now);
  Client* find_client(uint32_t id);
  void put(uint64_t at, const uint8_t* src, size_t len);
  void get(uint64_t at, uint8_t* dst, size_t len) const;
  uint16_t len_at(uint64_t at) const;

  uint8_t* _buf;
  size_t _cap;
  uint64_t _tail = 0;  // oldest retained record
  uint64_t _head = 0;  // next write position
  uint64_t _dropped_total = 0;
  size_t _rr = 0;      // round-robin start for fairness between clients
  Slot _slots[WSL_WRITER_SLOTS];
  Client _clients[WSL_MAX_CLIENTS];
};

}  // namespace wsl

#endif // WSL_STREAM_H
