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

#ifndef oatpp_network_ConnectionPool_hpp
#define oatpp_network_ConnectionPool_hpp

#include "ConnectionProvider.hpp"
#include "oatpp/provider/Pool.hpp"

namespace avena { namespace network {

/**
 * Wrapper over &id:avena::data::stream::IOStream;.
 * Will acquire connection from the pool on initialization and will return connection to the pool on destruction.
 */
struct ConnectionAcquisitionProxy : public provider::AcquisitionProxy<data::stream::IOStream, ConnectionAcquisitionProxy> {

  ConnectionAcquisitionProxy(const provider::ResourceHandle<data::stream::IOStream>& resource,
                             const std::shared_ptr<PoolInstance>& pool)
    : provider::AcquisitionProxy<data::stream::IOStream, ConnectionAcquisitionProxy>(resource, pool)
  {}

  v_io_size write(const void *buff, v_buff_size count, async::Action& action) override;
  v_io_size read(void *buff, v_buff_size count, async::Action& action) override;

  void setOutputStreamIOMode(avena::data::stream::IOMode ioMode) override;
  avena::data::stream::IOMode getOutputStreamIOMode() override;

  void setInputStreamIOMode(avena::data::stream::IOMode ioMode) override;
  avena::data::stream::IOMode getInputStreamIOMode() override;

  avena::data::stream::Context& getOutputStreamContext() override;
  avena::data::stream::Context& getInputStreamContext() override;

};

typedef avena::provider::Pool<
  avena::network::ClientConnectionProvider,
  avena::data::stream::IOStream,
  avena::network::ConnectionAcquisitionProxy
> ClientConnectionPool;

typedef avena::provider::Pool<
  avena::network::ServerConnectionProvider,
  avena::data::stream::IOStream,
  avena::network::ConnectionAcquisitionProxy
> ServerConnectionPool;


}}

#endif // oatpp_network_ConnectionPool_hpp
