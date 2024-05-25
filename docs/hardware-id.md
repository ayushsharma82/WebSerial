---
title: Hardware ID
sidebar_label: Hardware ID (Pro)
sidebar_position: 10
---

:::danger Pro Feature
This is an exclusive feature of WebSerial Pro. Check it out [here](https://webserial.pro).
:::

<br/>

<img src="/v2/img/hardware-id.png" alt="Hardware ID" width="400px" className="card-preview" />

<br/>

## Introduction

The Hardware ID feature is shown on the WebSerial Pro portal for easy identification of multiple devices in your network. This ID should be a unique string that helps you easily identify the specific hardware device among others in your network.

### Example Usage:

```cpp
...
#include <WebSerialPro.h>

void setup() {
  ...

  // Set the Hardware ID
  WebSerial.setID("my_device_001");
}

void loop() {
  ...
}
```

In the above example, the `WebSerial.setID("my_device_001");` function call assigns the Hardware ID "my_device_001" to the device.


## Best Practices

To make the most of the Hardware ID feature, consider the following best practices:

- Choose a Hardware ID that is unique to each device to avoid conflicts.

- Avoid using special characters or spaces in the Hardware ID, as these might cause issues when used in network communication or file naming.

- Document and keep track of the Hardware IDs for all your devices to ensure proper management.

- Ensure that the Hardware ID is set during the device's initialization phase, such as in the `setup()` function, to guarantee that it is properly configured before any network or communication activities.
