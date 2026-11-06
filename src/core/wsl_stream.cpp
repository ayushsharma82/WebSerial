#include "wsl_stream.h"

#include <string.h>

namespace wsl {

LogStream::LogStream(uint8_t* storage, size_t capacity) : _buf(storage), _cap(capacity) {}

// ---------------- Ring storage

void LogStream::put(uint64_t at, const uint8_t* src, size_t len) {
  size_t i = size_t(at % _cap);
  size_t first = len < _cap - i ? len : _cap - i;
  memcpy(_buf + i, src, first);
  memcpy(_buf, src + first, len - first);
}

void LogStream::get(uint64_t at, uint8_t* dst, size_t len) const {
  size_t i = size_t(at % _cap);
  size_t first = len < _cap - i ? len : _cap - i;
  memcpy(dst, _buf + i, first);
  memcpy(dst + first, _buf, len - first);
}

uint16_t LogStream::len_at(uint64_t at) const {
  return uint16_t(_buf[at % _cap] | (_buf[(at + 1) % _cap] << 8));
}

void LogStream::commit(uint32_t ms, uint8_t flags, uint8_t level, const uint8_t* data, size_t len) {
  if (len > WSL_MAX_LINE) len = WSL_MAX_LINE;
  size_t need = RECORD_HEADER_SIZE + len;
  // Evict whole records from the tail. Sent history goes silently; a client that had not
  // received an evicted record gets its cursor moved past it and a dropped count.
  while (_cap - size_t(_head - _tail) < need) {
    uint64_t next_tail = _tail + RECORD_HEADER_SIZE + len_at(_tail);
    bool cut = false;
    for (Client& c : _clients) {
      if (c.active && c.cursor < next_tail) {
        c.cursor = next_tail;
        c.dropped++;
        cut = true;
      }
    }
    if (cut) _dropped_total++;
    _tail = next_tail;
  }
  uint8_t header[RECORD_HEADER_SIZE];
  put_u16(header, uint16_t(len));
  header[2] = flags;
  header[3] = level;
  put_u32(header + 4, ms);
  put(_head, header, RECORD_HEADER_SIZE);
  put(_head + RECORD_HEADER_SIZE, data, len);
  _head += need;
}

bool LogStream::has_space_for(size_t bytes, uint32_t now, uint32_t stall_ms) const {
  uint64_t oldest_needed = _head;
  for (const Client& c : _clients) {
    if (!c.active) continue;
    // A stalled client (e.g. a background tab) does not hold writers back; it will get a dropped count
    if (stall_ms && c.inflight > 0 && int32_t(now - c.sent_at[0]) >= int32_t(stall_ms)) continue;
    if (c.cursor < oldest_needed) oldest_needed = c.cursor;
  }
  return _cap - size_t(_head - oldest_needed) >= bytes;
}

// ---------------- Line assembly

uint8_t LogStream::writer_of(const Slot& slot) const {
  return uint8_t(&slot - _slots) + 1;
}

// Commits the slot's bytes. A CONT fragment keeps the line open (and the slot reserved for its
// writer) until that writer ends the line; a final record closes it.
void LogStream::emit(Slot& slot, uint32_t ms, uint8_t flags) {
  if (!(flags & RECORD_CONT) && slot.len && slot.data[slot.len - 1] == '\r') slot.len--;
  commit(ms, uint8_t(flags | (writer_of(slot) << RECORD_WRITER_SHIFT)), slot.level, slot.data, slot.len);
  slot.len = 0;
  slot.open = (flags & RECORD_CONT) != 0;
}

LogStream::Slot* LogStream::slot_for(uintptr_t owner, uint32_t now) {
  for (Slot& s : _slots) if (s.owner == owner) return &s;
  for (Slot& s : _slots) if (s.owner == 0) { s.owner = owner; return &s; }
  // More concurrent writers than slots: close the oldest writer's line and reuse its slot
  Slot* oldest = &_slots[0];
  for (Slot& s : _slots) if (int32_t(s.since - oldest->since) < 0) oldest = &s;
  if (oldest->len || oldest->open) emit(*oldest, now, 0);
  oldest->owner = owner;
  return oldest;
}

bool LogStream::write(uintptr_t owner, uint32_t now, const uint8_t* data, size_t len, uint8_t level) {
  if (owner == 0) owner = 1;
  Slot* s = slot_for(owner, now);
  const uint8_t writer = uint8_t(writer_of(*s) << RECORD_WRITER_SHIFT);
  bool committed = false;
  for (size_t i = 0; i < len;) {
    const uint8_t* nl = static_cast<const uint8_t*>(memchr(data + i, '\n', len - i));
    size_t seg = nl ? size_t(nl - (data + i)) : len - i;
    if (s->len == 0 && !s->open && nl && seg <= WSL_MAX_LINE) {
      // Fast path: a whole line in one call (printf, println of a String) goes straight to the ring
      size_t line = (seg && data[i + seg - 1] == '\r') ? seg - 1 : seg;
      commit(now, writer, level, data + i, line);
      committed = true;
      i += seg + 1;
      continue;
    }
    size_t room = WSL_MAX_LINE - s->len;
    size_t take = seg < room ? seg : room;
    if (s->len == 0) {
      s->since = now;
      if (!s->open) s->level = level;
    }
    memcpy(s->data + s->len, data + i, take);
    s->len = uint16_t(s->len + take);
    i += take;
    if (s->len == WSL_MAX_LINE && (!nl || take < seg)) {
      emit(*s, now, RECORD_CONT);
      committed = true;
    } else if (nl && i < len && data[i] == '\n') {
      emit(*s, now, 0);
      committed = true;
      i++;
    }
  }
  if (s->len == 0 && !s->open) s->owner = 0;
  return committed;
}

bool LogStream::flush_idle(uint32_t now, uint32_t idle_ms) {
  bool committed = false;
  for (Slot& s : _slots) {
    // Signed age: a write stamped by another task slightly after `now` is not old
    if (s.len && int32_t(now - s.since) >= int32_t(idle_ms)) {
      emit(s, now, RECORD_CONT);  // the slot stays with its writer until the line is ended
      committed = true;
    }
  }
  return committed;
}

bool LogStream::has_partial() const {
  for (const Slot& s : _slots) if (s.len) return true;
  return false;
}

// ---------------- Clients

LogStream::Client* LogStream::find_client(uint32_t id) {
  for (Client& c : _clients) if (c.active && c.id == id) return &c;
  return nullptr;
}

bool LogStream::add_client(uint32_t id, uint32_t now, uint32_t* history_end) {
  (void)now;
  Client* c = find_client(id);
  if (!c) {
    for (Client& f : _clients) if (!f.active) { c = &f; break; }
    if (!c) return false;
  }
  c->active = true;
  c->id = id;
  c->cursor = _tail;
  c->dropped = 0;
  c->inflight = 0;
  if (history_end) *history_end = uint32_t(_head);
  return true;
}

void LogStream::remove_client(uint32_t id) {
  Client* c = find_client(id);
  if (c) c->active = false;
}

void LogStream::on_ack(uint32_t id, uint32_t end, uint32_t now) {
  (void)now;
  Client* c = find_client(id);
  if (!c) return;
  // ACK(end) acknowledges every frame ending at or before end
  while (c->inflight > 0 && int32_t(uint32_t(c->pending_end[0]) - end) <= 0) {
    for (uint8_t k = 1; k < c->inflight; k++) {
      c->pending_end[k - 1] = c->pending_end[k];
      c->sent_at[k - 1] = c->sent_at[k];
    }
    c->inflight--;
  }
}

size_t LogStream::client_count() const {
  size_t n = 0;
  for (const Client& c : _clients) if (c.active) n++;
  return n;
}

size_t LogStream::stale_clients(uint32_t now, uint32_t timeout_ms, uint32_t* ids, size_t max_ids) const {
  size_t n = 0;
  for (const Client& c : _clients) {
    // Signed age: a frame sent by another task slightly after `now` must not look ~49 days old
    if (c.active && c.inflight > 0 && int32_t(now - c.sent_at[0]) >= int32_t(timeout_ms) && n < max_ids) ids[n++] = c.id;
  }
  return n;
}

// ---------------- Sender

bool LogStream::sendable() const {
  for (const Client& c : _clients) {
    if (!c.active) continue;
    if (c.cursor < _head && c.inflight < WSL_WINDOW) return true;
    if (c.dropped > 0 && c.cursor >= _head) return true;
  }
  return false;
}

size_t LogStream::next_message(uint32_t now, std::vector<uint8_t>& out, uint32_t* ids, size_t max_ids, size_t max_frame) {
  out.clear();
  for (size_t k = 0; k < WSL_MAX_CLIENTS; k++) {
    size_t idx = (_rr + k) % WSL_MAX_CLIENTS;
    Client& c = _clients[idx];
    if (!c.active) continue;
    bool can_log = c.cursor < _head && c.inflight < WSL_WINDOW;
    bool drop_only = c.dropped > 0 && c.cursor >= _head;
    if (!can_log && !drop_only) continue;

    const uint64_t start = c.cursor;
    const uint32_t dropped = c.dropped;
    if (dropped) append_dropped(out, dropped);

    uint64_t end = start;
    if (can_log) {
      size_t frame = out.size();
      out.resize(frame + FRAME_HEADER_SIZE + 4);
      size_t budget = max_frame > out.size() ? max_frame - out.size() : 0;
      while (end < _head) {
        size_t size = RECORD_HEADER_SIZE + len_at(end);
        if (size > budget && end > start) break;  // the first record always fits (see wsl_config.h)
        size_t at = out.size();
        out.resize(at + size);
        get(end, &out[at], size);
        budget = size < budget ? budget - size : 0;
        end += size;
      }
      write_frame_header(&out[frame], FRAME_LOGS, uint16_t(out.size() - frame - FRAME_HEADER_SIZE));
      put_u32(&out[frame + FRAME_HEADER_SIZE], uint32_t(start));
    }

    // Every client at the same position (and with credit, if logs are included) shares this message
    size_t n = 0;
    for (Client& d : _clients) {
      if (!d.active || d.cursor != start || d.dropped != dropped) continue;
      if (end > start && d.inflight >= WSL_WINDOW) continue;
      if (n < max_ids) ids[n++] = d.id;
      d.cursor = end;
      d.dropped = 0;
      if (end > start) {
        d.pending_end[d.inflight] = end;
        d.sent_at[d.inflight] = now;
        d.inflight++;
      }
    }
    _rr = (idx + 1) % WSL_MAX_CLIENTS;
    return n;
  }
  return 0;
}

}  // namespace wsl
