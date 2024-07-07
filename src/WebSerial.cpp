#include "WebSerial.h"

#include "wslp.h"

// DO NOT change magic bytes
#define WSL_MAGIC_BYTE_1              0xAB
#define WSL_MAGIC_BYTE_2              0xCD
#define WSL_LOG_PACKET_HEADER_SIZE    14
#define WSL_CALC_LOG_PACKET_SIZE(len) (WSL_LOG_PACKET_HEADER_SIZE + len)

typedef enum {
  WSL_WRITE_ROW = 0x01,
  WSL_MESSAGE = 0x02,
  WSL_PING = 0x03,
  WSL_PONG = 0x04,
} WSLPacketType;

static const uint8_t WSL_PONG_MSG[] = {WSL_MAGIC_BYTE_1, WSL_MAGIC_BYTE_2, WSLPacketType::WSL_PONG};
static const size_t WSL_PONG_MSG_LEN = sizeof(WSL_PONG_MSG) / sizeof(WSL_PONG_MSG[0]);

static const uint8_t WSL_HEAD[] = {
  WSL_MAGIC_BYTE_1,             // Magic Bytes
  WSL_MAGIC_BYTE_2,             // Magic Bytes
  WSLPacketType::WSL_WRITE_ROW, // Packet Type (1 byte)
  0x00,                         // Reserved
  0x00,                         // Reserved
  0x00,                         // Reserved
  0x00,                         // Reserved
  0x00,                         // Padding
  0x00,                         // Padding
  0x00,                         // Padding
  0x00,                         // Padding
  0x00                          // Reserved
};
static const size_t WSL_HEAD_LEN = sizeof(WSL_HEAD) / sizeof(WSL_HEAD[0]);

static const size_t WSL_MSG_SIZE_LEN = sizeof(uint16_t);

void WebSerialClass::setAuthentication(const String& username, const String& password){
  _username = username;
  _password = password;
  _authenticate = !_username.isEmpty() && !_password.isEmpty();
  if (_ws != nullptr) {
    _ws->setAuthentication(_username.c_str(), _password.c_str());
  }
}

void WebSerialClass::begin(AsyncWebServer *server, const char* url) {
  _server = server;
  _ws = new AsyncWebSocket("/wserial");

  if (_authenticate) {
    _ws->setAuthentication(_username.c_str(), _password.c_str());
  }

  // Webpage Handler
  _server->on(url, HTTP_GET, [&](AsyncWebServerRequest *request){
    if(_authenticate){
      if(!request->authenticate(_username.c_str(), _password.c_str()))
        return request->requestAuthentication();
    }
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", WEBSERIAL_HTML, sizeof(WEBSERIAL_HTML));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);        
  });

  // WS Handler
  _ws->onEvent([&](__unused AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, __unused void * arg, uint8_t *data, __unused size_t len) -> void {
    // if(type == WS_EVT_CONNECT){
    // } else if(type == WS_EVT_DISCONNECT){
    // } else if(type == WS_EVT_DATA){
    if (type == WS_EVT_CONNECT) {
      client->setCloseClientOnQueueFull(false);
      return;
    }
    if(type == WS_EVT_DATA){
      // Detect magic bytes
      if (data[0] == WSL_MAGIC_BYTE_1 && data[1] == WSL_MAGIC_BYTE_2) {
        if (data[2] == WSLPacketType::WSL_MESSAGE) {
          // Parse message size (uint16_t)
          size_t message_size = (data[4] << 8) | data[3];
          // Issue callback
          if(_recv != nullptr){
            _recv(data + 5, message_size);
          }
        } else if (data[2] == WSLPacketType::WSL_PING) {
          // Send pong
          client->binary(WSL_PONG_MSG, WSL_PONG_MSG_LEN);
        }
      }
    }
  });

  // Attach AsyncWebServer with Websockets
  _server->addHandler(_ws);
}

// onMessage Callback Handler
void WebSerialClass::onMessage(WSLMessageHandler recv) {
  _recv = recv;
}

void WebSerialClass::onMessage(WSLStringMessageHandler callback) {
  _recvString = callback;
  _recv = [&](uint8_t *data, size_t len) {
    if(data && len) {
      String msg;
      msg.reserve(len);
      msg.concat((char*)data, len);
      _recvString(msg);
    }
  };
}

// Print func
size_t WebSerialClass::write(uint8_t m) {
  if (!_ws)
    return 0;

#ifdef WSL_HIGH_PERF
  // We do not support non-buffered write on webserial for the HIGH_PERF version
  // we fail with a stack trace allowing the user to change the code to use write(const uint8_t* buffer, size_t size) instead
  if(!_initialBufferCapacity) {
#ifdef ESP8266
    ets_printf("Non-buffered write is not supported. Please use write(const uint8_t* buffer, size_t size) instead.");
#else
    log_e("Non-buffered write is not supported. Please use write(const uint8_t* buffer, size_t size) instead.");
#endif
    assert(false);
    return 0;
  }
#endif // WSL_HIGH_PERF

  write(&m, 1);
  return(1);
}

// Println / Printf / Write func
size_t WebSerialClass::write(const uint8_t* buffer, size_t size) {
  if (!_ws || size == 0)
    return 0;

#ifdef WSL_HIGH_PERF
  // No buffer, send directly (i.e. use case for log streaming)
  if (!_initialBufferCapacity) {
    size = buffer[size - 1] == '\n' ? size - 1 : size;
    _send(buffer, size);
    return size;
  }

  // fill the buffer while sending data for each EOL
  size_t start = 0, end = 0;
  while (end < size) {
    if (buffer[end] == '\n') {
      if (end > start) {
        _buffer.concat(reinterpret_cast<const char*>(buffer + start), end - start);
      }
      _send(reinterpret_cast<const uint8_t*>(_buffer.c_str()), _buffer.length());
      start = end + 1;
    }
    end++;
  }
  if (end > start) {
    _buffer.concat(reinterpret_cast<const char*>(buffer + start), end - start);
  }
  return size;
#else
  loop();

  if (_print_buffer_offset + size > WSL_PRINT_BUFFER_SIZE) {
    // Flush print buffer if full
    _flush_print_buffer();
  }
    
  memcpy(_print_buffer + _print_buffer_offset, buffer, size);
  _print_buffer_offset += size;
  _last_print_buffer_write_time = micros();
  return(size);
#endif
}

#ifdef WSL_HIGH_PERF
void WebSerialClass::_send(const uint8_t* buffer, size_t size) {
  if (_ws && size > 0) {
    _ws->cleanupClients(WSL_MAX_WS_CLIENTS);
    if (_ws->count()) {
      if (size > UINT16_MAX)
        size = UINT16_MAX;
      AsyncWebSocketMessageBuffer* wsbuffer = _ws->makeBuffer(WSL_HEAD_LEN + WSL_MSG_SIZE_LEN + size);
      _write_row_packet(wsbuffer->get(), buffer, size);
      _ws->binaryAll(wsbuffer);
    }
  }

  // if buffer grew too much, free it, otherwise clear it
  if (_initialBufferCapacity) {
    if (_buffer.length() > _initialBufferCapacity) {
      setBuffer(_initialBufferCapacity);
    } else {
      _buffer.clear();
    }
  }
}

#else // WSL_HIGH_PERF

bool WebSerialClass::_has_enough_space(size_t size) {
  // Check if total packet size exceeds buffer limit
  return (_buffer_offset + WSL_CALC_LOG_PACKET_SIZE(size) > WSL_BUFFER_SIZE);
}

size_t WebSerialClass::_write_row(uint8_t *data, size_t len) {
  // Split the logData into multiple packets
  size_t remaining_size = len;
  uint8_t* current_ptr = data;

  while (remaining_size > 0) {
    size_t packet_size = (remaining_size > WSL_MAX_ROW_PACKET_PAYLOAD_SIZE) ? WSL_MAX_ROW_PACKET_PAYLOAD_SIZE : remaining_size;

    // Clear if buffer is full
    if (!_has_enough_space(packet_size)) {
      _flush_global_buffer();
    }

    // Write Packet to Buffer
    _buffer_offset += _write_row_packet(_buffer, current_ptr, packet_size);

    // Set remaining size
    remaining_size -= packet_size;
    current_ptr += packet_size;
  }

  return len;
}

void WebSerialClass::_flush_print_buffer() {
  if (_print_buffer_offset > 0) {
    if (_buffer_offset + _print_buffer_offset > WSL_BUFFER_SIZE) {
      // Flush global buffer to websocket
      _flush_global_buffer();
    }

    // Flush print to global buffer and create a packet
    _write_row(_print_buffer, _print_buffer_offset);
    _print_buffer_offset = 0;
    _last_print_buffer_flush_time = millis();
  }
}

void WebSerialClass::_flush_global_buffer() {
  if (_buffer_offset > 0) {
    // Flush buffer to websocket
    _ws->binaryAll(_buffer, _buffer_offset);
    // Reset buffer offset
    _buffer_offset = 0;
  }
}
#endif // WSL_HIGH_PERF

void WebSerialClass::loop() {
#ifndef WSL_HIGH_PERF
  if ((unsigned long)(millis() - _last_cleanup_time) > WSL_CLEANUP_TIME_MS) {
    _last_cleanup_time = millis();
    _ws->cleanupClients(WSL_MAX_WS_CLIENTS);
  }

  // If FLUSH_TIME ms has been passed since last packet time, flush logs
  if (_last_print_buffer_write_time != 0) {
    if ((unsigned long)(micros() - _last_print_buffer_write_time) > WSL_PRINT_FLUSH_TIME_US) {
      _flush_print_buffer();
    }
  }

  // If FLUSH_TIME ms has been passed since last flush time, flush logs
  if (_last_print_buffer_flush_time != 0) {
    if ((unsigned long)(millis() - _last_print_buffer_flush_time) > WSL_GLOBAL_FLUSH_TIME_MS) {
      _flush_global_buffer();
    }
  }
#endif // WSL_HIGH_PERF
}

void WebSerialClass::setBuffer(size_t initialCapacity) {
#ifdef WSL_HIGH_PERF
  assert(initialCapacity <= UINT16_MAX);
  _initialBufferCapacity = initialCapacity;
  _buffer = String();
  _buffer.reserve(initialCapacity);
#endif
}

size_t WebSerialClass::_write_row_packet(uint8_t* dest, const uint8_t *payload, size_t payload_size) {
  // sanity check to ensure the payload size is within the hard limit
  if(payload_size > UINT16_MAX)
    payload_size = UINT16_MAX;
  // Write header
  memmove(dest, WSL_HEAD, WSL_HEAD_LEN);
  // Message Length (2 bytes)
  memset(dest + WSL_HEAD_LEN, static_cast<uint16_t>(payload_size), WSL_MSG_SIZE_LEN);
  // Set Message
  memmove(dest + WSL_HEAD_LEN + WSL_MSG_SIZE_LEN, payload, payload_size);
  // Return total packet size
  return WSL_HEAD_LEN + WSL_MSG_SIZE_LEN + payload_size;
}

WebSerialClass WebSerial;