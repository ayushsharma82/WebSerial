---
title: Installation
sidebar_label: Installation
sidebar_position: 2
---

This document explains the installation procedure for getting started with WebSerial.

### Dependencies

*WebSerial depends on the following libraries present in your libraries folder. Please stricly install the compatible versions of these dependencies only! Using any other version might break WebSerial or may cause WebSerial to work partially.*

#### For ESP8266

- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino) @ **latest**
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) @ **latest**
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) @ **latest**

#### For ESP32

- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32) @ **latest**
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP) @ **latest**
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) @ **latest**

:::tip Alternative Dependencies

Although WebSerial supports the **official** dependencies, If you are using WebSerial to send a lot of packets, It's advised to switch to the following **fork of dependencies** for a crash-free experience. These forks are drop-in replacements which contain bugfixes and offer much better performance.

#### For ESP8266

- [esphome/ESPAsyncTCP](https://github.com/esphome/ESPAsyncTCP) @ **latest**
- [mathieucarbou/ESPAsyncWebServer](https://github.com/mathieucarbou/ESPAsyncWebServer) @ **2.5.1**

#### For ESP32

- [esphome/AsyncTCP](https://github.com/esphome/AsyncTCP) @ **latest**
- [mathieucarbou/ESPAsyncWebServer](https://github.com/mathieucarbou/ESPAsyncWebServer) @ **2.5.1**

*Please make sure to uninstall the 'original' dependencies from your libraries folder if you have installed any of these forks. If previous dependencies remain installed, the compiler will throw an error.*
:::

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
