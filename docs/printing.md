---
title: Printing Logs
sidebar_label: Printing Logs
sidebar_position: 5
---

# Printing Logs

The WebSerial library allows you to send logs and messages over a WebSocket connection, making it easy to debug and monitor your projects/devices via a web browser. This library inherits from the `Print` class of the Arduino core, so it uses methods similar to the `Serial` library.

:::tip Important Tip
**Don't** send large amounts of data in a single shot with WebSerial. (ie. more than 1KB)

WebSerial uses WebSockets as its underlying transport mechanism which is of low throughput (due to hardware limitation). Although there are built-in mechanisms in WebSerial library to ease this burden, it's advised to optimize the your firmware for a crash-free experience.
:::

## Methods

Here are a example few methods which can be used to print logs/messages to WebSerial terminal:

### `print()`

Prints data to the WebSerial port as human-readable ASCII text.

```cpp
WebSerial.print(data);
```

### `println()`

Prints data to the WebSerial terminal followed by a carriage return and newline.

```cpp
WebSerial.println(data);
```

### `printf()`

Prints formatted data to the WebSerial terminal.

```cpp
WebSerial.printf(format, args...);
```

### `write()`

Writes binary data to the WebSerial terminal.

:::note Note
WebSerial only accepts ASCII data with `write` opertaion, as it's sending data to a web browser there's no way to display other content types.
:::

```cpp
WebSerial.write(data);
WebSerial.write(buffer, size);
```
