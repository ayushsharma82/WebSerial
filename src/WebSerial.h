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
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
  #include "WiFi.h"
  #include "RPAsyncTCP.h"
  #include "ESPAsyncWebServer.h"
  #if defined(TARGET_RP2040) || defined(PICO_RP2040)
    #define HARDWARE "RP2040"
  #elif defined(TARGET_RP2350) || defined(PICO_RP2350)
    #define HARDWARE "RP2350"
  #endif
#endif

#ifndef WSL_MAX_WS_CLIENTS
#define WSL_MAX_WS_CLIENTS DEFAULT_MAX_WS_CLIENTS
#endif

// Buffer sizes, overflow policy and timeouts: see core/wsl_config.h (set them with -D build flags).
// WSL_HIGH_PERF and the v2 flush-timing macros no longer exist; buffering adapts automatically.

typedef std::function<void(uint8_t *data, size_t len)> WSLMessageHandler;
typedef std::function<void(const String& msg)> WSLStringMessageHandler;
typedef std::function<void(uint32_t clientId)> WSLClientHandler;

class WebSerialClass : public Print {
  public:
    WebSerialClass();

    // The terminal page and its WebSocket are both served at `url`
    void begin(AsyncWebServer *server, const char* url = "/webserial");
    inline void setAuthentication(const char* username, const char* password) { setAuthentication(String(username), String(password)); }
    void setAuthentication(const String& username, const String& password);
    void onMessage(WSLMessageHandler recv);
    void onMessage(WSLStringMessageHandler recv);
    void onConnect(WSLClientHandler callback);
    void onDisconnect(WSLClientHandler callback);
    size_t getConnectionCount();

    // Safe to call from any task or core (not from interrupts). Lines written before begin()
    // or while no browser is connected are kept and replayed when a browser connects.
    size_t write(uint8_t) override;
    size_t write(const uint8_t* buffer, size_t size) override;
    using Print::write;

    // Housekeeping: flushes partial lines (print() without a newline), closes unresponsive
    // browsers and retries sending. ESP32 and ESP8266 run it in the background; on RP2040/RP2350
    // call it from loop(). Calling it anywhere is always harmless.
    void loop();

    // Deprecated in v3: buffering is automatic, this has no effect.
    __attribute__((deprecated("WebSerial v3 buffers automatically; remove setBuffer()")))
    void setBuffer(size_t initialCapacity);

    // Deprecated in v3: use WebSerial.write(data, len), which is already a single copy.
    __attribute__((deprecated("use WebSerial.write(data, len)")))
    AsyncWebSocketMessageBuffer* makeBuffer(size_t size = 0);
    __attribute__((deprecated("use WebSerial.write(data, len)")))
    void send(AsyncWebSocketMessageBuffer* buffer);

  private:

    void _onEvent(AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);
    void _kick();
    void _pump();
    bool _canBlock();

    // Server
    AsyncWebServer *_server = nullptr;
    AsyncWebSocket *_ws = nullptr;
    WSLMessageHandler _recv = nullptr;
    WSLStringMessageHandler _recvString = nullptr;
    WSLClientHandler _onConnect = nullptr;
    WSLClientHandler _onDisconnect = nullptr;
    bool _authenticate = false;
    String _username;
    String _password;
    String _url;
    String _deviceId;

    // The log stream itself lives in WebSerial.cpp so this layout never depends on WSL_* sizes
    bool _pumping = false;
    bool _pumpAgain = false;
    unsigned long _lastCleanup = 0;
};

extern WebSerialClass WebSerial;
#endif
