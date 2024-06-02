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

typedef std::function<void(uint8_t *data, size_t len)> WSLMessageHandler;

class WebSerialClass : public Print {
  public:
    void begin(AsyncWebServer *server, const char* url = "/webserial");
    inline void setAuthentication(const char* username, const char* password) { setAuthentication(String(username), String(password)); }
    void setAuthentication(const String& username, const String& password);
    void onMessage(WSLMessageHandler recv);
    size_t write(uint8_t);
    size_t write(uint8_t* buffer, size_t size);
    void loop();

  private:
    // Global Buffer
    bool _buffer_mutex = false;
    size_t _buffer_offset = 0;
    uint8_t _buffer[WSL_BUFFER_SIZE];

    // Print buffer
    bool _print_buffer_mutex = false;
    size_t _print_buffer_offset = 0;
    uint8_t _print_buffer[WSL_PRINT_BUFFER_SIZE];
    unsigned long _last_print_buffer_write_time = 0;
    unsigned long _last_print_buffer_flush_time = 0;

    // Server
    AsyncWebServer *_server;
    AsyncWebSocket *_ws;
    WSLMessageHandler _recv = nullptr;
    unsigned long _last_cleanup_time = 0;
    bool _authenticate = false;
    String _username;
    String _password;

    // Print
    void _wait_for_global_mutex();
    void _wait_for_print_mutex();
    bool _has_enough_space(size_t size);
    size_t _start_row();
    size_t _write_row(uint8_t *data, size_t len);
    size_t _write_row_packet(uint64_t reserved1, uint8_t reserved2, const uint8_t *payload, const size_t payload_size);
    size_t _end_row();
    void _flush_print_buffer();
    void _flush_global_buffer();
};

extern WebSerialClass WebSerial;
#endif