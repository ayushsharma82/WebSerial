/*
 * This example streams lines to the browser at a high rate (20 lines/s on ESP32, 2 lines/s on ESP8266).
 *
 * WebSerial v3 adapts automatically: when the link is idle each line is sent immediately, and when
 * lines arrive faster than the network can carry them they are batched into larger messages.
 * There is no separate high-performance mode any more; any Print function (print, printf, write)
 * is fine.
 *
 * If lines are produced faster than the link can sustain, the oldest unsent lines are dropped and
 * the terminal shows "N lines dropped". Build with -D WSL_OVERFLOW_POLICY=WSL_OVERFLOW_BLOCK to make
 * the writer wait for space instead (like Serial does).
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

static const char* dict = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz1234567890";
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
#ifdef ESP8266
  if (millis() - last > 500) {
#else
  if (millis() - last > 50) {
#endif
    count++;

    long r = random(10, 250) + 15;
    String buffer;
    buffer.reserve(r);
    buffer += count;
    while (buffer.length() < 10) {
      buffer += " ";
    }
    buffer += "";
    for (int i = 0; i < r; i++) {
      buffer += dict[random(0, 62)];
    }

    WebSerial.println(buffer);

    last = millis();
  }

  // Housekeeping (required on RP2040/RP2350, harmless elsewhere)
  WebSerial.loop();
}
