#!/bin/bash

set -euo pipefail

pio lib -g install "ESP Async WebServer@1.2.3"

pio ci --lib=. --board esp32dev "examples/ESP32_Async_Demo"
pio ci --lib=. --board esp12e "examples/ESP8266_Async_Demo"
