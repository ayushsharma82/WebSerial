// WebSerial core configuration. Override any value with -D in build flags.
#ifndef WSL_CONFIG_H
#define WSL_CONFIG_H

// Ring buffer holding unsent lines and history replayed to new clients
#ifndef WSL_BUFFER_SIZE
  #if defined(ESP8266)
    #define WSL_BUFFER_SIZE         2048
  #elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
    #define WSL_BUFFER_SIZE         4096
  #else
    #define WSL_BUFFER_SIZE         8192
  #endif
#endif

// Longest line kept as one record; longer lines are split and flagged CONT
#ifndef WSL_MAX_LINE
  #define WSL_MAX_LINE              256
#endif

// Largest WebSocket message the sender builds
#ifndef WSL_MAX_FRAME
  #if defined(ESP8266)
    #define WSL_MAX_FRAME           2048
  #else
    #define WSL_MAX_FRAME           4096
  #endif
#endif

// Unacknowledged LOGS frames allowed per client (flow-control window)
#ifndef WSL_WINDOW
  #define WSL_WINDOW                2
#endif

// Concurrent writers (tasks/cores) whose partial lines are kept apart
#ifndef WSL_WRITER_SLOTS
  #if defined(ESP32)
    #define WSL_WRITER_SLOTS        4
  #else
    #define WSL_WRITER_SLOTS        2
  #endif
#endif

// Max simultaneously tracked browser clients
#ifndef WSL_MAX_CLIENTS
  #define WSL_MAX_CLIENTS           4
#endif

// A partial line (no '\n' yet) idle this long is sent as a fragment
#ifndef WSL_PARTIAL_FLUSH_MS
  #define WSL_PARTIAL_FLUSH_MS      20
#endif

// Close a client that has not acknowledged a frame for this long. Generous on purpose: browsers
// slow down background tabs, and TCP itself already detects connections that are really dead.
#ifndef WSL_ACK_TIMEOUT_MS
  #define WSL_ACK_TIMEOUT_MS        30000
#endif

// BLOCK policy: a client that has not acknowledged for this long (e.g. a background tab) stops holding
// writers back; it misses lines instead (reported as dropped), so one slow tab cannot slow the firmware.
#ifndef WSL_STALL_MS
  #define WSL_STALL_MS              1000
#endif

// Longest command accepted from the browser
#ifndef WSL_MAX_INPUT
  #define WSL_MAX_INPUT             512
#endif

// Overflow policy when the ring is full and a connected client has not received the oldest lines
#define WSL_OVERFLOW_DROP           0   // drop the oldest unsent lines, report "N lines dropped"
#define WSL_OVERFLOW_BLOCK          1   // writer waits for space (like Serial), up to WSL_BLOCK_TIMEOUT_MS
#ifndef WSL_OVERFLOW_POLICY
  #define WSL_OVERFLOW_POLICY       WSL_OVERFLOW_DROP
#endif
#ifndef WSL_BLOCK_TIMEOUT_MS
  #define WSL_BLOCK_TIMEOUT_MS      1000
#endif

#if WSL_MAX_LINE > 65535
  #error "WSL_MAX_LINE must fit in 16 bits"
#endif
#if WSL_BUFFER_SIZE < 2 * (WSL_MAX_LINE + 8)
  #error "WSL_BUFFER_SIZE must hold at least two maximum-length lines"
#endif
// DROPPED frame (10) + LOGS header (6) + start (4) + record header (8) + longest line
#if WSL_MAX_FRAME < WSL_MAX_LINE + 28
  #error "WSL_MAX_FRAME must hold at least one maximum-length line"
#endif
#if WSL_WRITER_SLOTS < 1 || WSL_WRITER_SLOTS > 15
  #error "WSL_WRITER_SLOTS must be 1-15 (the writer id is 4 bits in each record)"
#endif
#if WSL_WINDOW < 1
  #error "WSL_WINDOW must be at least 1"
#endif

#endif // WSL_CONFIG_H
