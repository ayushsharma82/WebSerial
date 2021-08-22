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

class WebSerialClass{

public:
    void begin(AsyncWebServer *server, const char* url = "/webserial");

    void msgCallback(RecvMsgHandler _recv);

    // Print

    void print(String m = "");

    void print(const char *m);

    void print(char *m);

    void print(int m);

    void print(uint8_t m);

    void print(uint16_t m);

    void print(uint32_t m);

    void print(double m);

    void print(float m);


    // Print with New Line

    void println(String m = "");

    void println(const char *m);

    void println(char *m);

    void println(int m);

    void println(uint8_t m);

    void println(uint16_t m);

    void println(uint32_t m);

    void println(float m);

    void println(double m);


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