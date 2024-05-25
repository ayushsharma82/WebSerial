---
title: Authentication
sidebar_label: Authentication
sidebar_position: 5
---

## Introduction

The authentication feature in WebSerial enhances the security of your Internet of Things (IoT) devices by requiring authentication before allowing anybody to access the logs of your device. This feature ensures that only authorized individuals or systems can access and modify your devices, adding an extra layer of protection to your IoT ecosystem.

## Setting Authentication

You can set up authentication for your device using the `WebSerial.setAuthentication("username", "password");` function. This function requires two parameters: a username and a password. These credentials act as a gatekeeper, preventing unauthorized access to the webserial interface.

### Example Usage

```cpp
#include <WebSerial.h>

void setup() {
  ...

  // Set Authentication Credentials
  WebSerial.setAuthentication("myUsername", "myPassword");
}

void loop() {
  ...
}
```

<!-- ## Clearing Authentication

Authentication once set can also be cleared by calling:
```cpp
WebSerial.clearAuthentication();
``` -->

## Use Cases

The Authentication feature is crucial in various scenarios:

1. **Security**: It safeguards your IoT devices against unauthorized access, reducing the risk of malicious actors getting access to important information in your logs.

2. **Access Control**: By setting unique credentials for each device, you can control access to individual devices, ensuring that only authorized personnel or systems can modify them.

3. **Compliance**: Some security and compliance standards require authentication mechanisms for IoT devices. WebSerial's authentication feature helps you meet these requirements.

## Best Practices

To ensure the effectiveness of the authentication feature, consider these best practices:

- Choose strong and unique usernames and passwords for each device.

- Avoid using default or easily guessable credentials to enhance security.

- Regularly update and rotate the authentication credentials to maintain security.
