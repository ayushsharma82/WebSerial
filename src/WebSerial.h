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

#include "webserial_webpage.h"

typedef std::function<void(uint8_t *data, size_t len)> RecvMsgHandler;

// Uncomment to enable webserial debug mode
// #define WEBSERIAL_DEBUG 1

class WebSerialClass : public Print {

public:
    void begin(AsyncWebServer *server, const char* url = "/webserial");

    void msgCallback(RecvMsgHandler _recv);

    // Print

    size_t write(uint8_t);
    size_t write(const uint8_t* buffer, size_t size);

private:
    AsyncWebServer *_server;
    AsyncWebSocket *_ws;
    RecvMsgHandler _RecvFunc = NULL;
    
    #if defined(WEBSERIAL_DEBUG)
        void DEBUG_WEB_SERIAL(const char* message);
    #endif
};

extern WebSerialClass WebSerial;
#endif