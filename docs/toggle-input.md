---
title: Toggle Input
sidebar_label: Toggle Input (Pro)
sidebar_position: 5
---

:::danger Pro Feature
This is an exclusive feature of WebSerial Pro. Check it out [here](https://webserial.pro).
:::

The toggle input feature in WebSerial Pro allows users to easily enable or disable the command input bar within the webserial interface. It is particularly useful when users want to use WebSerial solely for monitoring purposes, such as when they do not want commands sent to their firmware or when it is integrated into a product for monitoring only.

The input bar can be re-enabled whenever command input is required, making it a versatile tool for various use cases.

## Disable Input

<br/>

<img src="/v2/img/input-bar-disabled.png" alt="Export Logs Button" width="1200px" />

<br/>
<br/>

By default, the command input bar is enabled in WebSerial. To disable it, you can simply add this line of code in your firmware:

```cpp
WebSerial.disableInput();
```

## Enable Input

<br/>

<img src="/v2/img/input-bar.png" alt="Export Logs Button" width="1200px" />

<br/>
<br/>

Similarly, You can re-enable command input bar using the following line of code:

```cpp
WebSerial.enableInput();
```

<br/>

### Example Usage

```cpp
...
#include <WebSerialPro.h>

void setup() {
  ...

  // Disables WebSerial Command Input Bar
  WebSerial.disableInput();
}

void loop() {
  ...
}
```
