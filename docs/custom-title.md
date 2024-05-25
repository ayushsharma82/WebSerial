---
title: Custom Title
sidebar_label: Custom Title (Pro)
sidebar_position: 12
---

:::danger Pro Feature
This is an exclusive feature of WebSerial Pro. Check it out [here](https://webserial.pro).
:::

<br/>

<img src="/v3/img/custom-title.png" alt="Custom Title" width="400px" />

<br/>


## Overview 

The `setTitle()` function is a method provided by the WebSerial Pro library. This title is displayed at the top of the webserial interface/webpage, providing a user-friendly and customizable interface for managing firmware updates.

## Syntax

```cpp
void setTitle(const char* title);
```

## Example

```cpp
#include <WebSerial.h>

void setup() {
  ...

  // Set the title of your webserial terminal
  WebSerial.setTitle("Remote Terminal - XYZ Inc");
}

void loop() {
  ...
}
```

## Usage

1. Include the WebSerial library in your Arduino sketch.
2. In the `setup()` function of your sketch, initialize the WebSerial library using `WebSerial.begin()`.
3. Use the `setTitle()` function to set a meaningful title for your webserial interface. This title will be displayed at the top of the webserial interface/webpage when users access it.
4. Continue with the rest of your sketch's setup and loop functions.
