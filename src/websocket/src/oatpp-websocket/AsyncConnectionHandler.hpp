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

#ifndef oatpp_websocket_AsyncConnectionHandler_hpp
#define oatpp_websocket_AsyncConnectionHandler_hpp

#include "./AsyncWebSocket.hpp"

#include "oatpp/async/Executor.hpp"
#include "oatpp/network/ConnectionHandler.hpp"

namespace avena { namespace websocket {

/**
 * Asynchronous websocket connection handler.
 * Extends &id:avena::base::Countable;, &id:avena::network::ConnectionHandler;.
 */
class AsyncConnectionHandler : public base::Countable, public network::ConnectionHandler {
public:

  /**
   * Listener for new websocket instances.
   */
  class SocketInstanceListener {
  public:
    /**
     * Convenience typedef for &id:avena::websocket::AsyncWebSocket;.
     */
    typedef avena::websocket::AsyncWebSocket AsyncWebSocket;

    /**
     * Convenience typedef for &id:avena::network::ConnectionHandler::ParameterMap;.
     */
    typedef avena::network::ConnectionHandler::ParameterMap ParameterMap;
  public:

    /**
     * Default virtual destructor.
     */
    virtual ~SocketInstanceListener() = default;

    /**
     * Called when socket is created. <br>
     * **This method should not block**.
     * @param socket - &id:avena::websocket::AsyncWebSocket;.
     * @param params - `std::shared_ptr` to const &id:avena::network::ConnectionHandler::ParameterMap;.
     */
    virtual void onAfterCreate_NonBlocking(const std::shared_ptr<AsyncWebSocket>& socket, const std::shared_ptr<const ParameterMap>& params) = 0;

    /**
     * Called before socket instance is destroyed. <br>
     * **This method should not block**.
     * @param socket - &id:avena::websocket::AsyncWebSocket;.
     */
    virtual void onBeforeDestroy_NonBlocking(const std::shared_ptr<AsyncWebSocket>& socket) = 0;
  };
  
private:
  std::shared_ptr<avena::async::Executor> m_executor;
  std::shared_ptr<SocketInstanceListener> m_listener;
public:

  /**
   * Constructor. With threadCount.
   * Will create &id:avena::async::Executor;.
   * @param threadCount - number of threads for &id:avena::async::Executor;.
   */
  AsyncConnectionHandler(v_int32 threadCount = avena::async::Executor::VALUE_SUGGESTED);

  /**
   * Constructor. With &id:avena::async::Executor;.
   * @param executor - &id:avena::async::Executor;.
   */
  AsyncConnectionHandler(const std::shared_ptr<avena::async::Executor>& executor);

public:

  /**
   * Create shared AsyncConnectionHandler with threadCount.
   * @param threadCount - number of threads for &id:avena::async::Executor;.
   * @return - `std::shared_ptr` to AsyncConnectionHandler.
   */
  static std::shared_ptr<AsyncConnectionHandler> createShared(v_int32 threadCount = avena::async::Executor::VALUE_SUGGESTED);

  /**
   * Create shared AsyncConnectionHandler with &id:avena::async::Executor;.
   * @param executor - &id:avena::async::Executor;.
   * @return - `std::shared_ptr` to AsyncConnectionHandler.
   */
  static std::shared_ptr<AsyncConnectionHandler> createShared(const std::shared_ptr<avena::async::Executor>& executor);

  /**
   * Set &l:AsyncConnectionHandler::SocketInstanceListener;.
   * @param listener - &l:AsyncConnectionHandler::SocketInstanceListener;.
   */
  void setSocketInstanceListener(const std::shared_ptr<SocketInstanceListener>& listener);

  /**
   * Implementation of &id:avena::network::ConnectionHandler::handleConnection;.
   * @param connection - &id:avena::data::stream::IOStream;.
   */
  void handleConnection(const provider::ResourceHandle<IOStream>& connection, const std::shared_ptr<const ParameterMap>& params) override;

  /**
   * Will call &id:avena::async::Executor::stop;.
   */
  void stop() override;
  
};
  
}}

#endif /* oatpp_websocket_AsyncConnectionHandler_hpp */
