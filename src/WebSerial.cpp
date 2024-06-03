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
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", WEBSERIAL_HTML, sizeof(WEBSERIAL_HTML));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);        
  });

  // WS Handler
  _ws->onEvent([&](__unused AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, __unused void * arg, uint8_t *data, __unused size_t len) -> void {
    // if(type == WS_EVT_CONNECT){
    // } else if(type == WS_EVT_DISCONNECT){
    // } else if(type == WS_EVT_DATA){
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
          uint8_t pong[] = {WSL_MAGIC_BYTE_1, WSL_MAGIC_BYTE_2, WSLPacketType::WSL_PONG};
          client->binary(pong, sizeof(pong) / sizeof(pong[0]));
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

// Print func
size_t WebSerialClass::write(uint8_t m) {
  write(&m, 1);
  return(1);
}

// Println / Printf / Write func
size_t WebSerialClass::write(uint8_t* buffer, size_t size) {
  loop();
  _wait_for_print_mutex();
  _print_buffer_mutex = true;

  if (_print_buffer_offset + size > WSL_PRINT_BUFFER_SIZE) {
    // Flush print buffer if full
    _flush_print_buffer();
  }
    
  memcpy(_print_buffer + _print_buffer_offset, buffer, size);
  _print_buffer_offset += size;

  _print_buffer_mutex = false;
  _last_print_buffer_write_time = micros();
  return(size);
}

void WebSerialClass::_wait_for_global_mutex() {
  // Wait for mutex to be released
  if (_buffer_mutex) {
    while (_buffer_mutex) {
      delayMicroseconds(10);
    }
  }
}

void WebSerialClass::_wait_for_print_mutex() {
  // Wait for mutex to be released
  if (_print_buffer_mutex) {
    while (_print_buffer_mutex) {
      delayMicroseconds(10);
    }
  }
}

bool WebSerialClass::_has_enough_space(size_t size) {
  // Check if total packet size exceeds buffer limit
  return (_buffer_offset + WSL_CALC_LOG_PACKET_SIZE(size) > WSL_BUFFER_SIZE);
}

size_t WebSerialClass::_write_row_packet(__unused uint64_t reserved1, __unused uint8_t reserved2, const uint8_t *payload, const size_t payload_size) {
  size_t header_size = 0;

  // Write Magic Bytes
  _buffer[_buffer_offset + header_size++] = WSL_MAGIC_BYTE_1;
  _buffer[_buffer_offset + header_size++] = WSL_MAGIC_BYTE_2;

  // Packet Type (1 byte)
  _buffer[_buffer_offset + header_size++] = WSLPacketType::WSL_WRITE_ROW;

  // Reserved (8 bytes)
  _buffer[_buffer_offset + header_size++] = 0x00;
  _buffer[_buffer_offset + header_size++] = 0x00;
  _buffer[_buffer_offset + header_size++] = 0x00;
  _buffer[_buffer_offset + header_size++] = 0x00;
  _buffer[_buffer_offset + header_size++] = 0x00;
  _buffer[_buffer_offset + header_size++] = 0x00;
  _buffer[_buffer_offset + header_size++] = 0x00;
  _buffer[_buffer_offset + header_size++] = 0x00;

  // Reserved (1 byte)
  _buffer[_buffer_offset + header_size++] = 0x00;

  // Message Length (2 bytes)
  memset(_buffer + _buffer_offset + header_size, (uint16_t)payload_size, sizeof((uint16_t)payload_size));
  header_size += sizeof((uint16_t)payload_size);

  // Set Message
  memcpy(_buffer + _buffer_offset + header_size, payload, payload_size);

  // Return total packet size
  return header_size + payload_size;
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

    // Wait for mutex to be released
    _wait_for_global_mutex();

    // Lock Mutex
    _buffer_mutex = true;

    // Write Packet to Buffer
    _buffer_offset += _write_row_packet(0, 0, current_ptr, packet_size);

    // Unlock Mutex
    _buffer_mutex = false;

    // Set remaining size
    remaining_size -= packet_size;
    current_ptr += packet_size;
  }

  return len;
}

void WebSerialClass::_flush_print_buffer() {
  _wait_for_print_mutex();
  if (_print_buffer_mutex == false && _print_buffer_offset > 0) {
    _print_buffer_mutex = true;

    if (_buffer_offset + _print_buffer_offset > WSL_BUFFER_SIZE) {
      // Flush global buffer to websocket
      _flush_global_buffer();
    }

    // Flush print to global buffer and create a packet
    _write_row(_print_buffer, _print_buffer_offset);
    _print_buffer_offset = 0;
    
    _print_buffer_mutex = false;
    _last_print_buffer_flush_time = millis();
  }
}

void WebSerialClass::_flush_global_buffer() {
  _wait_for_global_mutex();
  if (_buffer_mutex == false && _buffer_offset > 0) {
    _buffer_mutex = true;

    // Flush buffer to websocket
    _ws->binaryAll(_buffer, _buffer_offset);
    // Reset buffer offset
    _buffer_offset = 0;
    
    _buffer_mutex = false;
  }
}

void WebSerialClass::loop() {
  if ((unsigned long)(millis() - _last_cleanup_time) > WSL_CLEANUP_TIME_MS) {
    _last_cleanup_time = millis();
    _ws->cleanupClients();
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
}

WebSerialClass WebSerial;