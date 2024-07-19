/*
__        __   _    ____            _       _ 
\ \      / /__| |__/ ___|  ___ _ __(_) __ _| |
 \ \ /\ / / _ \ '_ \___ \ / _ \ '__| |/ _` | |
  \ V  V /  __/ |_) |__) |  __/ |  | | (_| | |
   \_/\_/ \___|_.__/____/ \___|_|  |_|\__,_|_|
                                              

A remote terminal for wireless microcontrollers!

Checkout Pro version at: https://webserial.pro

-----

Author: Ayush Sharma (ayush@softt.io)
License: AGPL-3.0 (https://www.gnu.org/licenses/agpl-3.0.html)
*/

#ifndef WebSerial_h
#define WebSerial_h

#include "Arduino.h"
#include "stdlib_noniso.h"
#include <functional>

#if defined(ESP8266)
    #define HARDWARE "ESP8266"
    #include "ESP8266WiFi.h"
    #include "ESPAsyncTCP.h"
    #include "ESPAsyncWebServer.h"
#elif defined(ESP32)
    #define HARDWARE "ESP32"
    #include "WiFi.h"
    #include "AsyncTCP.h"
    #include "ESPAsyncWebServer.h"
#endif

#ifndef WSL_MAX_WS_CLIENTS
#define WSL_MAX_WS_CLIENTS DEFAULT_MAX_WS_CLIENTS
#endif

// High performance mode:
// - Low memory footprint (no stack allocation, no global buffer by default)
// - Low latency (messages sent immediately to the WebSocket queue)
// - High throughput (up to 20 messages per second, no locking mechanism)
// Activation with: -D WSL_HIGH_PERFORMANCE
// Also recommended to tweak AsyncTCP and ESPAsyncWebServer settings, for example:
//  -D CONFIG_ASYNC_TCP_QUEUE_SIZE=128  // AsyncTCP queue size
//  -D CONFIG_ASYNC_TCP_RUNNING_CORE=1  // core for the async_task
//  -D WS_MAX_QUEUED_MESSAGES=128       // WS message queue size
#ifndef WSL_HIGH_PERF
  // Global buffer ( buffers all packets )
  #ifndef WSL_BUFFER_SIZE
  #define WSL_BUFFER_SIZE                       2048
  #endif
  #ifndef WSL_PRINT_BUFFER_SIZE
  #define WSL_PRINT_BUFFER_SIZE                 1024
  #endif
  #ifndef WSL_MAX_ROW_PACKET_PAYLOAD_SIZE
  #define WSL_MAX_ROW_PACKET_PAYLOAD_SIZE       512
  #endif

  #ifndef WSL_PRINT_FLUSH_TIME_US
  #define WSL_PRINT_FLUSH_TIME_US               100
  #endif
  #ifndef WSL_GLOBAL_FLUSH_TIME_MS
  #define WSL_GLOBAL_FLUSH_TIME_MS              100
  #endif
  #ifndef WSL_CLEANUP_TIME_MS
  #define WSL_CLEANUP_TIME_MS                   5000
  #endif

  #if WSL_BUFFER_SIZE < 512
    #error "WSL_BUFFER_SIZE must be >= 512 bytes"
  #endif

  #if WSL_BUFFER_SIZE < WSL_PRINT_BUFFER_SIZE
    #error "WSL_BUFFER_SIZE must be >= WSL_PRINT_BUFFER_SIZE"
  #endif

  #if WSL_PRINT_FLUSH_TIME_US < 1
    #error "WSL_PRINT_FLUSH_TIME_US must be greater than 1us"
  #endif

  #if WSL_GLOBAL_FLUSH_TIME_MS < 50
    #error "WSL_GLOBAL_FLUSH_TIME_MS must be greater than 50ms"
  #endif
#endif // WSL_HIGH_PERFORMANCE

typedef std::function<void(uint8_t *data, size_t len)> WSLMessageHandler;
typedef std::function<void(const String& msg)> WSLStringMessageHandler;

class WebSerialClass : public Print {
  public:
    void begin(AsyncWebServer *server, const char* url = "/webserial");
    inline void setAuthentication(const char* username, const char* password) { setAuthentication(String(username), String(password)); }
    void setAuthentication(const String& username, const String& password);
    void onMessage(WSLMessageHandler recv);
    void onMessage(WSLStringMessageHandler recv);
    size_t write(uint8_t) override;
    size_t write(const uint8_t* buffer, size_t size) override;
    
    // Only valid if WSL_HIGH_PERF is not activated (which is the default)
    // Housekeeping for WebSerial internals.
    // Calling this loop has no effect if WSL_HIGH_PERF is activated
    void loop();
    
    // Only valid if WSL_HIGH_PERF is activated
    // A buffer (shared across cores) can be initialised with an initial capacity to be able to use any Print functions event those that are not buffered and would
    // create a performance impact for WS calls. The goal of this buffer is to be used with lines ending with '\n', like log messages.
    // The buffer size will eventually grow until a '\n' is found, then the message will be sent to the WS clients and a new buffer will be created.
    // Set initialCapacity to 0 to disable buffering.
    // Must be called before begin(): calling it after will erase the buffer and its content will be lost.
    // The buffer is not enabled by default.
    void setBuffer(size_t initialCapacity);

  private:
    // Server
    AsyncWebServer *_server;
    AsyncWebSocket *_ws;
    WSLMessageHandler _recv = nullptr;
    WSLStringMessageHandler _recvString = nullptr;
    bool _authenticate = false;
    String _username;
    String _password;

#ifdef WSL_HIGH_PERF
    size_t _initialBufferCapacity = 0;
    String _buffer;
    void _send(const uint8_t* buffer, size_t size);
#else
    unsigned long _last_cleanup_time = 0;

    // Global Buffer
    size_t _buffer_offset = 0;
    uint8_t _buffer[WSL_BUFFER_SIZE];

    // Print buffer
    size_t _print_buffer_offset = 0;
    uint8_t _print_buffer[WSL_PRINT_BUFFER_SIZE];
    unsigned long _last_print_buffer_write_time = 0;
    unsigned long _last_print_buffer_flush_time = 0;

    // Print
    bool _has_enough_space(size_t size);
    size_t _start_row();
    size_t _write_row(uint8_t *data, size_t len);
    size_t _end_row();
    void _flush_print_buffer();
    void _flush_global_buffer();
#endif

    static size_t _write_row_packet(uint8_t* dest, const uint8_t *payload, size_t payload_size);
};

extern WebSerialClass WebSerial;
#endif