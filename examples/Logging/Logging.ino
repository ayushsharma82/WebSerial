/*
 * This example shows how to use WebSerial to stream logging data to the browser.
 *
 * Lines are buffered and batched automatically. Lines printed before a browser connects
 * (e.g. during boot) are replayed to it as history.
 */
#include <Arduino.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
  #include <WiFi.h>
  #include <RPAsyncTCP.h>
#endif

#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <WString.h>
#include <WebSerial.h>

AsyncWebServer server(80);

static uint32_t last = millis();
static uint32_t count = 0;

void setup() {
  Serial.begin(115200);

  WiFi.softAP("WSLDemo");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP().toString());

  WebSerial.onMessage([](const String& msg) { Serial.println(msg); });
  WebSerial.begin(&server);

  server.onNotFound([](AsyncWebServerRequest* request) { request->redirect("/webserial"); });
  server.begin();
}

void loop() {
  if (millis() - last > 1000) {
    count++;

    WebSerial.print(F("IP address: "));
    WebSerial.println(WiFi.softAPIP());
    WebSerial.printf("Uptime: %lums\n", millis());
    #if defined(ESP8266)
      WebSerial.printf("Free heap: %" PRIu32 "\n", ESP.getFreeHeap());
    #elif defined(ESP32)
      WebSerial.printf("Free heap: %" PRIu32 "\n", ESP.getFreeHeap());
    #elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
      WebSerial.printf("Free heap: %" PRIu32 "\n", rp2040.getFreeHeap());
    #endif

    last = millis();
  }

  // Housekeeping (required on RP2040/RP2350, harmless elsewhere)
  WebSerial.loop();
}
