#include "WebSerial.h"


void WebSerialClass::begin(AsyncWebServer *server, const char* url){
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

    #if defined(WEBSERIAL_DEBUG)
        DEBUG_WEB_SERIAL("Attached AsyncWebServer along with Websockets");
    #endif
}

void WebSerialClass::msgCallback(RecvMsgHandler _recv){
    _RecvFunc = _recv;
}

// Print
void WebSerialClass::print(String m){
    _ws->textAll(m);
}

void WebSerialClass::print(const char *m){
    _ws->textAll(m);
}

void WebSerialClass::print(char *m){
    _ws->textAll(m);
}

void WebSerialClass::print(int m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(uint8_t m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(uint16_t m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(uint32_t m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(double m){
    _ws->textAll(String(m));
}

void WebSerialClass::print(float m){
    _ws->textAll(String(m));
}


// Print with New Line

void WebSerialClass::println(String m){
    _ws->textAll(m+"\n");        
}

void WebSerialClass::println(const char *m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(char *m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(int m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(uint8_t m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(uint16_t m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(uint32_t m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(float m){
    _ws->textAll(String(m)+"\n");
}

void WebSerialClass::println(double m){
    _ws->textAll(String(m)+"\n");
}

#if defined(WEBSERIAL_DEBUG)
    void WebSerialClass::DEBUG_WEB_SERIAL(const char* message){
        Serial.println("[WebSerial] "+message);
    }
#endif

WebSerialClass WebSerial;