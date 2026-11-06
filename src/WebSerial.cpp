#include "WebSerial.h"

#include "wslp.h"
#include "core/wsl_config.h"
#include "core/wsl_stream.h"

#include <memory>
#include <vector>

#if defined(ESP32)
  #include <mutex>
#elif defined(ESP8266)
  extern "C" {
    #include "user_interface.h"
  }
  #include <coredecls.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
  #define WSL_RP2XXX
  #include <pico/mutex.h>
  #include <pico/unique_id.h>
  extern volatile int __inLWIP;   // arduino-pico: > 0 while inside lwIP (incl. network callbacks)
#endif

// ---------------------------------------------------------------- Platform layer
//
// One lock protects the log stream. Producers (any task/core) only copy into the ring under it;
// the network work happens outside it:
//   ESP32   - a small background task sends, woken by write()/ACKs and every WSL_PARTIAL_FLUSH_MS
//   ESP8266 - sends synchronously (loop and network callbacks never preempt each other) plus an os_timer
//   RP2040  - sends synchronously from core 0 outside interrupts; loop() does the housekeeping

#if defined(ESP32)
  static std::recursive_mutex wsl_mutex;
  struct WSLLock {
    WSLLock() { wsl_mutex.lock(); }
    ~WSLLock() { wsl_mutex.unlock(); }
  };
  static TaskHandle_t wsl_task = nullptr;
#elif defined(WSL_RP2XXX)
  auto_init_recursive_mutex(wsl_mutex);
  struct WSLLock {
    WSLLock() { recursive_mutex_enter_blocking(&wsl_mutex); }
    ~WSLLock() { recursive_mutex_exit(&wsl_mutex); }
  };
#else
  struct WSLLock {};
  static ETSTimer wsl_timer;
#endif

// Ring buffer + per-client cursors (one WebSerial instance per firmware), guarded by WSLLock
static uint8_t wsl_storage[WSL_BUFFER_SIZE];
static wsl::LogStream wsl_stream(wsl_storage, sizeof(wsl_storage));

// Identifies the current writer so partial lines from different tasks/cores are kept apart
static uintptr_t wsl_writer_id() {
#if defined(ESP32)
  return reinterpret_cast<uintptr_t>(xTaskGetCurrentTaskHandle());
#elif defined(ESP8266)
  return can_yield() ? 1 : 2;   // loop context vs. system/network context
#elif defined(WSL_RP2XXX)
  return 1 + get_core_num() + (__get_current_exception() ? 2 : 0);
#else
  return 1;
#endif
}

static String wsl_device_id() {
#if defined(ESP32)
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  String id = String(chipId, HEX);
#elif defined(ESP8266)
  String id = String(ESP.getChipId(), HEX);
#elif defined(WSL_RP2XXX)
  char raw[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
  pico_get_unique_board_id_string(raw, sizeof(raw));
  String id = raw;
#else
  String id = "";
#endif
  id.toUpperCase();
  return String(HARDWARE) + "-" + id;
}

// ---------------------------------------------------------------- Inbound frames

class WSLInbound : public wsl::InboundHandler {
  public:
    WSLInbound(const WSLMessageHandler& recv, AsyncWebSocketClient* client) : _recv(recv), _client(client) {}

    void on_ack(uint32_t end) override {
      WSLLock lock;
      wsl_stream.on_ack(_client->id(), end, millis());
      acked = true;
    }

    void on_input(const uint8_t* data, size_t len) override {
      if (_recv != nullptr && len > 0) {
        _recv(const_cast<uint8_t*>(data), len);
      }
    }

    void on_ping(uint32_t seq) override {
      uint8_t pong[wsl::PONG_FRAME_SIZE];
      wsl::write_pong(pong, seq, millis());
      _client->binary(pong, sizeof(pong));
    }

    bool acked = false;

  private:
    const WSLMessageHandler& _recv;
    AsyncWebSocketClient* _client;
};

// ---------------------------------------------------------------- Setup

WebSerialClass::WebSerialClass() {}

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

  // Page and WebSocket share one URL (the WebSocket handler only matches upgrade requests)
  _url = url;
  if (!_url.startsWith("/")) {
    _url = "/" + _url;
  }
  while (_url.length() > 1 && _url.endsWith("/")) {
    _url.remove(_url.length() - 1);
  }
  _deviceId = wsl_device_id();

  _ws = new AsyncWebSocket(_url);

  if (_authenticate) {
    _ws->setAuthentication(_username.c_str(), _password.c_str());
  }

  _ws->onEvent([this](__unused AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) -> void {
    _onEvent(client, type, arg, data, len);
  });

  // Attach AsyncWebServer with Websockets
  _server->addHandler(_ws);

  // Webpage Handler
  _server->on(_url.c_str(), HTTP_GET, [&](AsyncWebServerRequest *request){
    if(_authenticate){
      if(!request->authenticate(_username.c_str(), _password.c_str()))
        return request->requestAuthentication();
    }
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", WEBSERIAL_HTML, sizeof(WEBSERIAL_HTML));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

#if defined(ESP32)
  if (wsl_task == nullptr) {
    xTaskCreate([](void* self) {
      for (;;) {
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(WSL_PARTIAL_FLUSH_MS));
        static_cast<WebSerialClass*>(self)->loop();
      }
    }, "webserial", 4096, this, tskIDLE_PRIORITY + 2, &wsl_task);
  }
#elif defined(ESP8266)
  os_timer_disarm(&wsl_timer);
  os_timer_setfn(&wsl_timer, [](void* self) { static_cast<WebSerialClass*>(self)->loop(); }, this);
  os_timer_arm(&wsl_timer, WSL_PARTIAL_FLUSH_MS, true);
#endif
}

void WebSerialClass::_onEvent(AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    client->setCloseClientOnQueueFull(false);

    // WELCOME goes out before the client is registered, so no LOGS frame can overtake it
    uint32_t historyEnd;
    {
      WSLLock lock;
      historyEnd = uint32_t(wsl_stream.head());
    }
    wsl::Welcome info;
    info.edition = 0;
    info.hardware = HARDWARE;
    info.device_id = _deviceId.c_str();
    info.device_ms = millis();
    info.history_end = historyEnd;
    info.max_line = WSL_MAX_LINE;
    std::vector<uint8_t> welcome;
    wsl::append_welcome(welcome, info);
    client->binary(welcome.data(), welcome.size());

    bool added;
    {
      WSLLock lock;
      added = wsl_stream.add_client(client->id(), millis(), nullptr);
    }
    if (!added) {
      client->close();
      return;
    }
    if (_onConnect) {
      _onConnect(client->id());
    }
    _kick();
  } else if (type == WS_EVT_DISCONNECT) {
    {
      WSLLock lock;
      wsl_stream.remove_client(client->id());
    }
    if (_onDisconnect) {
      _onDisconnect(client->id());
    }
  } else if (type == WS_EVT_DATA) {
    // Only accept complete, unfragmented binary frames; the parser never reads past len
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info == nullptr || !info->final || info->index != 0 || info->len != len || info->opcode != WS_BINARY) {
      return;
    }
    WSLInbound inbound(_recv, client);
    wsl::parse_inbound(data, len, inbound, WSL_MAX_INPUT);
    if (inbound.acked) {
      _kick();
    }
  }
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

void WebSerialClass::onConnect(WSLClientHandler callback) {
  _onConnect = callback;
}

void WebSerialClass::onDisconnect(WSLClientHandler callback) {
  _onDisconnect = callback;
}

size_t WebSerialClass::getConnectionCount() {
  return _ws == nullptr ? 0 : _ws->count();
}

// ---------------------------------------------------------------- Writing

size_t WebSerialClass::write(uint8_t m) {
  return write(&m, 1);
}

size_t WebSerialClass::write(const uint8_t* buffer, size_t size) {
  if (buffer == nullptr || size == 0)
    return 0;
  // Printing from another file's global constructor can run before the stream is constructed
  if (wsl_stream.capacity() == 0)
    return size;

#if WSL_OVERFLOW_POLICY == WSL_OVERFLOW_BLOCK
  if (_canBlock()) {
    // Room needed: the bytes plus one record header per line, capped at what the ring can ever free
    size_t lines = 1;
    for (const uint8_t* p = buffer; (p = (const uint8_t*)memchr(p, '\n', size - size_t(p - buffer))) != nullptr; p++) lines++;
    size_t need = size + lines * wsl::RECORD_HEADER_SIZE;
    if (need > wsl_stream.capacity()) need = wsl_stream.capacity();
    unsigned long start = millis();
    for (;;) {
      {
        WSLLock lock;
        if (wsl_stream.has_space_for(need, millis(), WSL_STALL_MS)) break;
      }
      if ((unsigned long)(millis() - start) >= WSL_BLOCK_TIMEOUT_MS) break;
      _kick();
      delay(1);
    }
  }
#endif

  bool committed;
  {
    WSLLock lock;
    committed = wsl_stream.write(wsl_writer_id(), millis(), buffer, size);
  }
  if (committed) {
    _kick();
  }
  return size;
}

bool WebSerialClass::_canBlock() {
#if defined(ESP32)
  return !xPortInIsrContext() && xTaskGetCurrentTaskHandle() != wsl_task && strcmp(pcTaskGetName(nullptr), "async_tcp") != 0;
#elif defined(ESP8266)
  return can_yield();
#elif defined(WSL_RP2XXX)
  return !__get_current_exception() && __inLWIP == 0;
#else
  return false;
#endif
}

// ---------------------------------------------------------------- Sending

void WebSerialClass::_kick() {
  if (_ws == nullptr)
    return;
#if defined(ESP32)
  if (wsl_task != nullptr) {
    xTaskNotifyGive(wsl_task);
  }
#elif defined(WSL_RP2XXX)
  // ESPAsyncWebServer is not multicore-safe on RP2040: only core 0 touches it, never from an interrupt
  if (get_core_num() == 0 && !__get_current_exception()) {
    _pump();
  }
#else
  _pump();
#endif
}

void WebSerialClass::_pump() {
  if (_ws == nullptr)
    return;
  {
    WSLLock lock;
    if (_pumping) {
      // Another context is sending: it will pick up the new data
      _pumpAgain = true;
      return;
    }
    _pumping = true;
  }
  uint32_t ids[WSL_MAX_CLIENTS];
  for (;;) {
    bool more;
    {
      WSLLock lock;
      more = wsl_stream.sendable();
      if (!more) {
        if (!_pumpAgain) {
          _pumping = false;
          return;
        }
        _pumpAgain = false;
        continue;
      }
    }
    AsyncWebSocketSharedBuffer message = std::make_shared<std::vector<uint8_t>>();
    message->reserve(WSL_MAX_FRAME);
    size_t recipients;
    {
      WSLLock lock;
      recipients = wsl_stream.next_message(millis(), *message, ids, WSL_MAX_CLIENTS, WSL_MAX_FRAME);
    }
    if (recipients == 0)
      continue;
    if (message->size() < message->capacity() / 2) {
      message->shrink_to_fit();
    }
    // One shared buffer for every browser at the same position; the library looks clients up by id under its own lock
    for (size_t i = 0; i < recipients; i++) {
      _ws->binary(ids[i], message);
    }
  }
}

void WebSerialClass::loop() {
  if (_ws == nullptr)
    return;
#if defined(WSL_RP2XXX)
  if (get_core_num() != 0 || __get_current_exception())
    return;
#endif
  uint32_t stale[WSL_MAX_CLIENTS];
  size_t staleCount;
  uint32_t now;
  {
    WSLLock lock;
    // Read the clock under the lock: another task may stamp a send between millis() and the lock
    now = millis();
    wsl_stream.flush_idle(now, WSL_PARTIAL_FLUSH_MS);
    staleCount = wsl_stream.stale_clients(now, WSL_ACK_TIMEOUT_MS, stale, WSL_MAX_CLIENTS);
    for (size_t i = 0; i < staleCount; i++) {
      wsl_stream.remove_client(stale[i]);
    }
  }
  // Browsers that stopped acknowledging are closed; they reconnect and catch up from history
  for (size_t i = 0; i < staleCount; i++) {
#if defined(ESP32)
    log_w("[WebSerial] closing client %u: no acknowledgement for %u ms", stale[i], WSL_ACK_TIMEOUT_MS);
#endif
    _ws->close(stale[i]);
  }
  if ((unsigned long)(now - _lastCleanup) > 1000) {
    _lastCleanup = now;
    _ws->cleanupClients(WSL_MAX_CLIENTS);
  }
  _pump();
}

// ---------------------------------------------------------------- Deprecated v2 API

void WebSerialClass::setBuffer(size_t initialCapacity) {
  (void)initialCapacity;
}

AsyncWebSocketMessageBuffer* WebSerialClass::makeBuffer(size_t size) {
  return _ws == nullptr ? nullptr : _ws->makeBuffer(size);
}

void WebSerialClass::send(AsyncWebSocketMessageBuffer* buffer) {
  if (buffer == nullptr)
    return;
  // v2 sent each buffer as one line
  write(buffer->get(), buffer->length());
  if (buffer->length() == 0 || buffer->get()[buffer->length() - 1] != '\n') {
    write('\n');
  }
  delete buffer;
}

WebSerialClass WebSerial;
