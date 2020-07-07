#!/bin/bash

set -euo pipefail

pio lib -g install "ESP Async WebServer@1.2.3" "AsyncTCP@1.1.1" "ESPAsyncTCP@1.2.2"

pio ci --lib=. --board esp32dev "examples/ESP32_Demo"
pio ci --lib=. --board esp12e "examples/ESP8266_Demo"
