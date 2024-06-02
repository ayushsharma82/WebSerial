---
title: Installation
sidebar_label: Installation
sidebar_position: 2
---

This document explains the installation procedure for getting started with WebSerial.

### Dependencies

*WebSerial depends on the following libraries present in your libraries folder. Please stricly install the compatible versions of these dependencies only! Using any other version might break WebSerial or may cause WebSerial to work partially.*

:::important Note on Dependencies
As of v2, WebSerial has officially switched to using fork of ESPAsyncWebServer and its dependencies from [@mathieucarbou](https://github.com/mathieucarbou). This fork of ESPAsyncWebServer is being maintained regularly and contains many bug fixes along with arduino-esp32 core v3 support.

It's suggested to migrate all your projects to using this fork as the original me-no-dev/ESPAsyncWebServer repo is not being maintained  *since years*.
:::

#### For ESP8266

- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino) - **latest**
- (mathieucarbou) [esphome-ESPAsyncTCP](https://github.com/mathieucarbou/esphome-ESPAsyncTCP#v2.0.0) - **v2.0.0**
- (mathieucarbou) [ESPAsyncWebServer](https://github.com/mathieucarbou/ESPAsyncWebServer#v2.10.4) - **v2.10.4**

#### For ESP32

- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32) @ **latest**
- (mathieucarbou) [AsyncTCP](https://github.com/mathieucarbou/AsyncTCP#v3.1.4) @ **v3.1.4**
- (mathieucarbou) [ESPAsyncWebServer](https://github.com/mathieucarbou/ESPAsyncWebServer#v2.10.4) - **v2.10.4**

### Installing WebSerial

#### 1. Directly Through Arduino IDE - Library Manager

Go to Sketch > Include Library > Library Manager > Search for "WebSerial" > Install

#### 2. Manual Install

##### For Windows

- Download the [Repository](https://github.com/ayushsharma82/WebSerial/archive/master.zip)
- Extract the .zip in `Documents > Arduino > Libraries > {Place "WebSerial" folder Here}`

##### For Linux

- Download the [Repository](https://github.com/ayushsharma82/WebSerial/archive/master.zip)
- Extract the .zip in `Sketchbook > Libraries > {Place "WebSerial" folder Here}`

#### 3. Import through Arduino IDE

- Download the [Repository](https://github.com/ayushsharma82/WebSerial/archive/master.zip)
- Go to `Sketch > Include Library > Add .zip Library > Select the Downloaded .zip File.`
