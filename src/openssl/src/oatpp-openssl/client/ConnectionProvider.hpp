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

#ifndef oatpp_openssl_client_ConnectionProvider_hpp
#define oatpp_openssl_client_ConnectionProvider_hpp

#include "oatpp-openssl/Config.hpp"

#include "oatpp/network/Address.hpp"
#include "oatpp/network/ConnectionProvider.hpp"

namespace avena { namespace openssl { namespace client {

/**
 * Openssl client connection provider.
 * Extends &id:avena::base::Countable;, &id:avena::network::ClientConnectionProvider;.
 */
class ConnectionProvider : public avena::network::ClientConnectionProvider {
private:

  class ConnectionInvalidator : public provider::Invalidator<data::stream::IOStream> {
  public:
    void invalidate(const std::shared_ptr<data::stream::IOStream>& connection) override;
  };

private:
  std::shared_ptr<ConnectionInvalidator> m_connectionInvalidator;
  std::shared_ptr<Config> m_config;
  std::shared_ptr<avena::network::ClientConnectionProvider> m_streamProvider;
private:
  void initSSLClient();
private:
  SSL_CTX* m_ctx;
public:
  /**
   * Constructor.
   * @param config - &id:avena::openssl::Config;.
   * @param streamProvider - provider of underlying transport stream. &id:avena::network::ClientConnectionProvider;.
   */
  ConnectionProvider(const std::shared_ptr<Config>& config, const std::shared_ptr<avena::network::ClientConnectionProvider>& streamProvider);
public:

  /**
   * Create shared ConnectionProvider.
   * @param config - &id:avena::openssl::Config;.
   * @param streamProvider - provider of underlying transport stream. &id:avena::network::ClientConnectionProvider;.
   * @return - `std::shared_ptr` to ConnectionProvider.
   */
  static std::shared_ptr<ConnectionProvider> createShared(const std::shared_ptr<Config>& config,
                                                          const std::shared_ptr<avena::network::ClientConnectionProvider>& streamProvider);

  /**
   * Create shared ConnectionProvider.
   * @param config - &id:avena::openssl::Config;.
   * @param address - &id:avena::network::Address;.
   * @param useExtendedConnections - set `true` to use &l:ConnectionProvider::ExtendedConnection;.
   * `false` to use &id:avena::network::tcp::Connection;.
   * @return - `std::shared_ptr` to ConnectionProvider.
   */
  static std::shared_ptr<ConnectionProvider> createShared(const std::shared_ptr<Config>& config,
                                                          const network::Address& address);

  /**
   * Virtual destructor.
   */
  ~ConnectionProvider();

  /**
   * Close all handles.
   */
  void stop() override;

  /**
   * Get connection.
   * @return - `std::shared_ptr` to &id:avena::data::stream::IOStream;.
   */
  provider::ResourceHandle<data::stream::IOStream> get() override;

  /**
   * Get connection in asynchronous manner.
   * @return - &id:avena::async::CoroutineStarterForResult;.
   */
  avena::async::CoroutineStarterForResult<const provider::ResourceHandle<data::stream::IOStream>&> getAsync() override;

};
  
}}}

#endif /* oatpp_openssl_client_ConnectionProvider_hpp */
