// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.
//
// Created by Leonid  on 2019-03-25.
//

#include "FullAsyncTest.hpp"

#include "app/AsyncController.hpp"
#include "app/AsyncWebSocketListener.hpp"

#include "oatpp-websocket/Connector.hpp"
#include "oatpp-websocket/AsyncConnectionHandler.hpp"

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/Interface.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"

#include "oatpp/macro/component.hpp"

namespace avena { namespace test { namespace websocket {

namespace {

//#define OATPP_TEST_USE_PORT 8000

class TestComponent {
public:

  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::async::Executor>, httpServerExecutor)("http-server-exec", [] {
    return std::make_shared<avena::async::Executor>(4, 2);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::async::Executor>, wsServerExecutor)("ws-server-exec", [] {
    return std::make_shared<avena::async::Executor>(4, 2);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::async::Executor>, wsClientExecutor)("ws-client-exec", [] {
    return std::make_shared<avena::async::Executor>(4, 2);
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::network::virtual_::Interface>, virtualInterface)([] {
    return avena::network::virtual_::Interface::obtainShared("virtualhost");
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::network::ServerConnectionProvider>, serverConnectionProvider)([] {
#ifdef OATPP_TEST_USE_PORT
    return avena::network::server::SimpleTCPConnectionProvider::createShared(OATPP_TEST_USE_PORT);
#else
    OATPP_COMPONENT(std::shared_ptr<avena::network::virtual_::Interface>, interface);
    return avena::network::virtual_::server::ConnectionProvider::createShared(interface);
#endif
  }());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::web::server::HttpRouter>, httpRouter)([] {
    return avena::web::server::HttpRouter::createShared();
  }());

  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<avena::web::server::HttpRouter>, router); // get Router component
    OATPP_COMPONENT(std::shared_ptr<avena::async::Executor>, executor, "http-server-exec");
    return avena::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
  }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    auto objectMapper = std::make_shared<avena::json::ObjectMapper>();
    objectMapper->deserializerConfig().mapper.allowUnknownFields = false;
    return objectMapper;
  }());

  /**
   *  Create websocket connection handler
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::websocket::AsyncConnectionHandler>, websocketConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<avena::async::Executor>, executor, "ws-server-exec");
    auto connectionHandler = avena::websocket::AsyncConnectionHandler::createShared(executor);
    connectionHandler->setSocketInstanceListener(std::make_shared<app::AsyncWebSocketInstanceListener>());
    return connectionHandler;
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::network::ClientConnectionProvider>, clientConnectionProvider)([this] {
#ifdef OATPP_TEST_USE_PORT
    return avena::network::client::SimpleTCPConnectionProvider::createShared("127.0.0.1", OATPP_TEST_USE_PORT);
#else
    OATPP_COMPONENT(std::shared_ptr<avena::network::virtual_::Interface>, interface);
    return avena::network::virtual_::client::ConnectionProvider::createShared(interface);
#endif
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::websocket::Connector>, connector)([] {
    OATPP_COMPONENT(std::shared_ptr<avena::network::ClientConnectionProvider>, clientConnectionProvider);
    return avena::websocket::Connector::createShared(clientConnectionProvider);
  }());

};

class ClientSocketListener : public avena::websocket::AsyncWebSocket::Listener{
private:
  v_int64 m_lastTick = 0;
  v_int32 m_messageCounter = 0;
  bool m_printLog;
  avena::data::stream::BufferOutputStream m_messageBuffer;
public:

  ClientSocketListener(bool printLog)
    : m_printLog(printLog)
  {}

  CoroutineStarter onPing(const std::shared_ptr<AsyncWebSocket>& socket, const avena::String& message) override {
    return socket->sendPongAsync(message);
  }

  CoroutineStarter onPong(const std::shared_ptr<AsyncWebSocket>& socket, const avena::String& message) override {
    return nullptr;
  }

  CoroutineStarter onClose(const std::shared_ptr<AsyncWebSocket>& socket, v_uint16 code, const avena::String& message) override {
    return nullptr;
  }

  CoroutineStarter readMessage(const std::shared_ptr<AsyncWebSocket>& socket, v_uint8 opcode, p_char8 data, avena::v_io_size size) override {
    if(size == 0) {
      m_messageCounter ++;
      auto wholeMessage = m_messageBuffer.toString();
      m_messageBuffer.setCurrentPosition(0);
      if(m_printLog) {
        auto tick = avena::Environment::getMicroTickCount();
        OATPP_LOGd("client", "sid={}, received {}, latency={}, messageCount={}",
                   reinterpret_cast<v_uint64>(socket.get()),
                   wholeMessage, tick - m_lastTick,
                   m_messageCounter);
        m_lastTick = tick;
      }
    } else if(size > 0) {
      m_messageBuffer.writeSimple(data, size);
    }
    return nullptr;
  }

};

class ClientSenderCoroutine : public avena::async::Coroutine<ClientSenderCoroutine> {
private:
  std::shared_ptr<avena::websocket::AsyncWebSocket> m_socket;
  v_int32 m_messagesPerClient;
  v_int32 m_messagesCount;
public:

  ClientSenderCoroutine(const std::shared_ptr<avena::websocket::AsyncWebSocket>& socket, v_int32 messagesPerClient)
    : m_socket(socket)
    , m_messagesPerClient(messagesPerClient)
    , m_messagesCount(0)
  {}

  Action act() override {
    if(m_messagesCount < m_messagesPerClient) {
      m_messagesCount ++;
      return m_socket->sendOneFrameTextAsync("hello!").next(yieldTo(&ClientSenderCoroutine::act));
    }
    return m_socket->sendCloseAsync().next(finish());
  }

};

class ClientCoroutine : public avena::async::Coroutine<ClientCoroutine> {
public:
  static std::atomic<v_int32> CONNECTIONS;
  static std::atomic<v_int32> FINISHED_COUNTER;
private:
  OATPP_COMPONENT(std::shared_ptr<avena::async::Executor>, executor, "ws-client-exec");
  OATPP_COMPONENT(std::shared_ptr<avena::websocket::Connector>, connector);
  std::shared_ptr<avena::websocket::AsyncWebSocket> socket;
  bool m_printLog;
  v_int32 m_messagesPerClient;
public:

  ClientCoroutine(bool printLog, v_int32 messagesPerClient)
    : m_printLog(printLog)
    , m_messagesPerClient(messagesPerClient)
  {}

  Action act() override {
    return connector->connectAsync("ws").callbackTo(&ClientCoroutine::onConnected);
  }

  Action onConnected(const avena::provider::ResourceHandle<avena::data::stream::IOStream>& connection) {
    ++ CONNECTIONS;
    socket = avena::websocket::AsyncWebSocket::createShared(connection, true);
    socket->setListener(std::make_shared<ClientSocketListener>(m_printLog));
    executor->execute<ClientSenderCoroutine>(socket, m_messagesPerClient);
    return socket->listenAsync().next(yieldTo(&ClientCoroutine::onFinishListen));
  }

  Action onFinishListen() {
    ++ FINISHED_COUNTER;
    OATPP_LOGd("Client", "Finished count={}", FINISHED_COUNTER.load());
    return finish();
  }

  Action handleError(Error* error) override {
    if(error) {
      OATPP_LOGd("Client", "Error. !!!---!!!---!!!---!!!---!!!---!!!---!!!---!!!---!!!---!!!---!!!---!!!---!!! {}", error->what());
    }
    return error;
  }

};

std::atomic<v_int32> ClientCoroutine::CONNECTIONS(0);
std::atomic<v_int32> ClientCoroutine::FINISHED_COUNTER(0);

}

void FullAsyncTest::onRun() {


  TestComponent component;

  OATPP_COMPONENT(std::shared_ptr<avena::async::Executor>, httpServerExecutor, "http-server-exec");
  OATPP_COMPONENT(std::shared_ptr<avena::async::Executor>, wsServerExecutor, "ws-server-exec");
  OATPP_COMPONENT(std::shared_ptr<avena::async::Executor>, wsClientExecutor, "ws-client-exec");

  avena::test::web::ClientServerTestRunner runner;

  runner.addController(app::AsyncController::createShared());

  runner.run([] {

    OATPP_COMPONENT(std::shared_ptr<avena::async::Executor>, executor, "ws-client-exec");

    /////////////////////////////////////////////////////////////////////////////////////
    // Create clients

    v_int32 clients = 1000;
    v_int32 messagesPerClient = 100;

    for(v_int32 i = 0; i < clients; i ++) {
      executor->execute<ClientCoroutine>(false, messagesPerClient);
    }

    clients ++;
    executor->execute<ClientCoroutine>(true, messagesPerClient);

    /////////////////////////////////////////////////////////////////////////////////////

    OATPP_LOGd("AAA", "waiting...");

    while(true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      if(ClientCoroutine::FINISHED_COUNTER == clients) {
        break;
      }
    }

  }, std::chrono::minutes(10));

  httpServerExecutor->waitTasksFinished();
  wsServerExecutor->waitTasksFinished();
  wsClientExecutor->waitTasksFinished();

  httpServerExecutor->stop();
  wsServerExecutor->stop();
  wsClientExecutor->stop();

  httpServerExecutor->join();
  wsServerExecutor->join();
  wsClientExecutor->join();

}

}}}