// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.
//
// Created by Leonid  on 2019-03-25.
//

#include "FullTest.hpp"

#include "app/Controller.hpp"
#include "app/WebSocketListener.hpp"

#include "oatpp-websocket/Connector.hpp"
#include "oatpp-websocket/ConnectionHandler.hpp"

#include "oatpp/web/server/HttpConnectionHandler.hpp"

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
    return avena::web::server::HttpConnectionHandler::createShared(router);
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
  OATPP_CREATE_COMPONENT(std::shared_ptr<avena::websocket::ConnectionHandler>, websocketConnectionHandler)([] {
    auto connectionHandler = avena::websocket::ConnectionHandler::createShared();
    connectionHandler->setSocketInstanceListener(std::make_shared<app::WebSocketInstanceListener>());
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

class ClientWebSocketListener : public avena::websocket::WebSocket::Listener {
private:
  static constexpr const char *const TAG = "ClientWebSocketListener";
private:
  avena::data::stream::BufferOutputStream m_messageBuffer;
public:

  void onPing(const WebSocket &socket, const avena::String &message) override {
    OATPP_LOGd(TAG, "Ping frame received. Sending Pong back.");
    socket.sendPong("");
  }

  void onPong(const WebSocket &socket, const avena::String &message) override {
    OATPP_LOGd(TAG, "Pong frame received. Do nothing.");
    // DO NOTHING
  }

  void onClose(const WebSocket &socket, v_uint16 code, const avena::String &message) override {
    OATPP_LOGd(TAG, "Close frame received. Code={}, Message='{}'", code, message);
  }

  /**
   * Called when "text" or "binary" frame received.
   * When all data of message is read, readMessage is called again with size == 0 to
   * indicate end of the message
   */
  void readMessage(const WebSocket &socket, v_uint8 opcode, p_char8 data, avena::v_io_size size) override {
    if (size == 0) {
      auto wholeMessage = m_messageBuffer.toString();
      OATPP_LOGd(TAG, "Message='{}'", wholeMessage);
      socket.sendOneFrameText("Hello from oatpp! Your message was: " + wholeMessage);
      m_messageBuffer.setCurrentPosition(0);
    } else if (size > 0) {
      m_messageBuffer.writeSimple(data, size);
    }
  };

};

}

void FullTest::onRun() {

  TestComponent component;

  avena::test::web::ClientServerTestRunner runner;

  runner.addController(app::Controller::createShared());

  runner.run([] {


    OATPP_COMPONENT(std::shared_ptr<avena::websocket::Connector>, connector);

    auto connection = connector->connect("ws");
    avena::websocket::WebSocket socket(connection, true);

    socket.setListener(std::make_shared<ClientWebSocketListener>());

    //std::this_thread::sleep_for(std::chrono::minutes(10));

  }, std::chrono::minutes(10));

}

}}}