// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.
/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_websocket_Connector_hpp
#define oatpp_websocket_Connector_hpp

#include "oatpp/web/client/HttpRequestExecutor.hpp"

namespace avena { namespace websocket {

/**
 * Helper class to do client websocket connect.
 */
class Connector {
public:
  /**
   * Convenience typedef for &id:avena::data::stream::IOStream;.
   */
  typedef avena::data::stream::IOStream Connection;

  /**
   * Convenience typedef for &id:avena::async::Action;.
   */
  typedef avena::async::Action Action;

  /*
   * Convenience typedef for &id:avena::async::CoroutineStarterForResult;.
   */
  template <typename ...Args>
  using
  CoroutineStarterForResult = typename avena::async::CoroutineStarterForResult<Args...>;

  /**
   * Convenience typedef for &id:avena::web::protocol::http::Headers;.
   */
  typedef avena::web::protocol::http::Headers Headers;

private:
  std::shared_ptr<avena::network::ClientConnectionProvider> m_connectionProvider;
  avena::web::client::HttpRequestExecutor m_requestExecutor;
public:

  /**
   * Constructor.
   * @param connectionProvider - &id:avena::network::ClientConnectionProvider;.
   */
  Connector(const std::shared_ptr<avena::network::ClientConnectionProvider>& connectionProvider);
public:

  /**
   * Create shared Connector.
   * @param connectionProvider - &id:avena::network::ClientConnectionProvider;.
   * @return - `std::shared_ptr` to Connector.
   */
  static std::shared_ptr<Connector> createShared(const std::shared_ptr<avena::network::ClientConnectionProvider>& connectionProvider) {
    return std::make_shared<Connector>(connectionProvider);
  }

  /**
   * Connect to server, do websocket-handshake and return Connection which can be then passed to WebSocket constructor. <br>
   * **(Blocking call)**
   * @param path - path to websocket endpoint.
   * @param headers - websocket connect request headers.
   * @return - &l:Connector::Connection;.
   */
  provider::ResourceHandle<Connection> connect(const avena::String& path, const Headers& headers = {});

  /**
   * Connect to server, do websocket-handshake and return Connection which can be then passed to WebSocket constructor. <br>
   * **(Async call)**
   * @param path - path to websocket endpoint.
   * @param headers - websocket connect request headers.
   * @return - &id:avena::async::Action;.
   */
  CoroutineStarterForResult<const provider::ResourceHandle<Connection>&> connectAsync(const avena::String& path, const Headers& headers = {});
  
};
  
}}

#endif /* oatpp_websocket_Connector_hpp */
