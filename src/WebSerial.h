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

#define BUFFER_SIZE 500

#include "webserial_webpage.h"

typedef std::function<void(uint8_t *data, size_t len)> RecvMsgHandler;


class WebSerialClass{

public:
    void begin(AsyncWebServer *server, const char* url = "/webserial"){
        _server = server;
        _ws = new AsyncWebSocket("/webserialws");

        _server->on(url, HTTP_GET, [](AsyncWebServerRequest *request){
            // Send Webpage
            AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", WEBSERIAL_HTML, WEBSERIAL_HTML_SIZE);
            response->addHeader("Content-Encoding","gzip");
            request->send(response);        
        });

        _ws->onEvent([&](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) -> void {
            if(type == WS_EVT_CONNECT){
                #if defined(DEBUG)
                    DEBUG_WEB_SERIAL("Client connection received");
                #endif
            } else if(type == WS_EVT_DISCONNECT){
                #if defined(DEBUG)
                    DEBUG_WEB_SERIAL("Client disconnected");
                #endif
            } else if(type == WS_EVT_DATA){
                #if defined(DEBUG)
                    DEBUG_WEB_SERIAL("Received Websocket Data");
                #endif
                if(_RecvFunc != NULL){
                    _RecvFunc(data, len);
                }
            }
        });

        _server->addHandler(_ws);

        #if defined(DEBUG)
            DEBUG_WEB_SERIAL("Attached AsyncWebServer along with Websockets");
        #endif
    }

    void msgCallback(RecvMsgHandler _recv){
        _RecvFunc = _recv;
    }

    // Print

    void print(String m = ""){
        _ws->textAll(m);
    }

    void print(const char *m){
        _ws->textAll(m);
    }

    void print(char *m){
        _ws->textAll(m);
    }

    void print(int m){
        _ws->textAll(String(m));
    }

    void print(uint8_t m){
        _ws->textAll(String(m));
    }

    void print(uint16_t m){
        _ws->textAll(String(m));
    }

    void print(uint32_t m){
        _ws->textAll(String(m));
    }

    void print(double m){
        _ws->textAll(String(m));
    }

    void print(float m){
        _ws->textAll(String(m));
    }


    // Print with New Line

    void println(String m = ""){
        _ws->textAll(m+"\n");        
    }

    void println(const char *m){
        _ws->textAll(String(m)+"\n");
    }

    void println(char *m){
        _ws->textAll(String(m)+"\n");
    }

    void println(int m){
        _ws->textAll(String(m)+"\n");
    }

    void println(uint8_t m){
        _ws->textAll(String(m)+"\n");
    }

    void println(uint16_t m){
        _ws->textAll(String(m)+"\n");
    }

    void println(uint32_t m){
        _ws->textAll(String(m)+"\n");
    }

    void println(float m){
        _ws->textAll(String(m)+"\n");
    }

    void println(double m){
        _ws->textAll(String(m)+"\n");
    }


private:
    AsyncWebServer *_server;
    AsyncWebSocket *_ws;
    RecvMsgHandler _RecvFunc = NULL;
    
    #if defined(DEBUG)
        void DEBUG_WEB_SERIAL(const char* message){
            Serial.println("[WebSerial] "+message);
        }
    #endif
};

WebSerialClass WebSerial;
#endif