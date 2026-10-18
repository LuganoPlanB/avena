// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.
//
// Created by Leonid  on 2019-03-25.
//

#ifndef oatpp_test_websocket_app_AsyncWebSocketListener_hpp
#define oatpp_test_websocket_app_AsyncWebSocketListener_hpp

#include "oatpp-websocket/AsyncConnectionHandler.hpp"
#include "oatpp-websocket/AsyncWebSocket.hpp"

namespace avena { namespace test { namespace websocket { namespace app {

class AsyncWebSocketListener : public avena::websocket::AsyncWebSocket::Listener {
private:
  avena::data::stream::BufferOutputStream m_messageBuffer;
public:

  CoroutineStarter onPing(const std::shared_ptr<AsyncWebSocket>& socket, const avena::String& message) override {
    return socket->sendPongAsync(message);
  }

  CoroutineStarter onPong(const std::shared_ptr<AsyncWebSocket>& socket, const avena::String& message) override {
    return nullptr;
  }

  CoroutineStarter onClose(const std::shared_ptr<AsyncWebSocket>& socket, v_uint16 code, const avena::String& message) override {
    return socket->sendCloseAsync();
  }

  CoroutineStarter readMessage(const std::shared_ptr<AsyncWebSocket>& socket, v_uint8 opcode, p_char8 data, avena::v_io_size size) override {
    if(size == 0) {
      avena::String wholeMessage = m_messageBuffer.toString();
      m_messageBuffer.setCurrentPosition(0);
      return socket->sendOneFrameTextAsync("Hello from oatpp!: " + wholeMessage);
    } else if(size > 0) {
      m_messageBuffer.writeSimple(data, size);
    }
    return nullptr;
  }

};

class ServerSenderCoroutine : public avena::async::Coroutine<ServerSenderCoroutine> {
private:
  std::shared_ptr<avena::websocket::AsyncWebSocket> m_socket;
public:

  ServerSenderCoroutine(const std::shared_ptr<avena::websocket::AsyncWebSocket>& socket)
    : m_socket(socket)
  {}

  Action act() override {
    return m_socket->sendOneFrameTextAsync("hello!").next(yieldTo(&ServerSenderCoroutine::act));
  }

};

class AsyncWebSocketInstanceListener : public avena::websocket::AsyncConnectionHandler::SocketInstanceListener {
private:
  static constexpr const char* const TAG = "AsyncWebSocketInstanceListener";
  static std::atomic<v_int32>& getAtom() {
    static std::atomic<v_int32> atom(0);
    return atom;
  }
public:

  /**
   *  Called when socket is created
   */
  void onAfterCreate_NonBlocking(const std::shared_ptr<AsyncWebSocket>& socket, const std::shared_ptr<const ParameterMap>& params) override {
    getAtom() ++;

    OATPP_ASSERT(params);
    auto testParam = params->find("p1");
    OATPP_ASSERT(testParam != params->end());
    OATPP_ASSERT(testParam->second == "v1");

    socket->setListener(std::make_shared<AsyncWebSocketListener>());
  }

  /**
   *  Called before socket instance is destroyed.
   */
  void onBeforeDestroy_NonBlocking(const std::shared_ptr<AsyncWebSocket>& socket) override {
    //OATPP_LOGD(TAG, "Closing connection");
    getAtom() --;
  }

};

}}}}


#endif //oatpp_test_websocket_app_AsyncWebSocketListener_hpp
