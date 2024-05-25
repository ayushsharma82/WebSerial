---
title: Callbacks
sidebar_label: Callbacks
sidebar_position: 5
---

WebSerial comes with a single callback called `onMessage`. It's responsible for notifying your firmware whenever it has received a command/message from user via webserial terminal.

If the callback is not registered, then the command received from terminal is simply discarded.

### Example Usage

```cpp
...
#include <WebSerial.h>

void setup() {
  ...

  /* Attach Message Callback */
  WebSerial.onMessage([&](uint8_t *data, size_t len) {
    Serial.printf("Received %lu bytes from WebSerial: ", len);
    Serial.write(data, len);
    Serial.println();
  });
}

void loop() {
  ...
}
```
