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

#include "ConnectionPoolTest.hpp"

#include "oatpp/network/ConnectionPool.hpp"
#include "oatpp/async/Executor.hpp"
#include "oatpp/base/Log.hpp"

namespace avena { namespace test { namespace network {

namespace {

typedef avena::provider::Pool<
  avena::network::ConnectionProvider,
  avena::data::stream::IOStream,
  avena::network::ConnectionAcquisitionProxy
> ConnectionPool;

class StubStream : public avena::data::stream::IOStream, public avena::base::Countable {
public:

  v_io_size write(const void *buff, v_buff_size count, async::Action& actions) override {
    throw std::runtime_error("It's a stub!");
  }

  v_io_size read(void *buff, v_buff_size count, async::Action& action) override {
    throw std::runtime_error("It's a stub!");
  }

  void setOutputStreamIOMode(avena::data::stream::IOMode ioMode) override {
    throw std::runtime_error("It's a stub!");
  }

  avena::data::stream::IOMode getOutputStreamIOMode() override {
    throw std::runtime_error("It's a stub!");
  }

  avena::data::stream::Context& getOutputStreamContext() override {
    throw std::runtime_error("It's a stub!");
  }

  void setInputStreamIOMode(avena::data::stream::IOMode ioMode) override {
    throw std::runtime_error("It's a stub!");
  }

  avena::data::stream::IOMode getInputStreamIOMode() override {
    throw std::runtime_error("It's a stub!");
  }

  avena::data::stream::Context& getInputStreamContext() override {
    throw std::runtime_error("It's a stub!");
  }

};

class StubStreamProvider : public avena::network::ConnectionProvider {
private:

  class Invalidator : public avena::provider::Invalidator<avena::data::stream::IOStream> {
  public:
    void invalidate(const std::shared_ptr<avena::data::stream::IOStream>& connection) override {
      (void)connection;
      // DO Nothing.
    }
  };

private:
  std::shared_ptr<Invalidator> m_invalidator = std::make_shared<Invalidator>();
public:

  StubStreamProvider()
    : counter(0)
  {}

  std::atomic<v_int64> counter;

  avena::provider::ResourceHandle<avena::data::stream::IOStream> get() override {
    ++ counter;
    return avena::provider::ResourceHandle<avena::data::stream::IOStream>(
      std::make_shared<StubStream>(),
      m_invalidator
    );
  }

  avena::async::CoroutineStarterForResult<const avena::provider::ResourceHandle<avena::data::stream::IOStream>&> getAsync() override {

    class ConnectionCoroutine : public avena::async::CoroutineWithResult<ConnectionCoroutine, const avena::provider::ResourceHandle<avena::data::stream::IOStream>&> {
    private:
      std::shared_ptr<Invalidator> m_invalidator;
    public:

      ConnectionCoroutine(const std::shared_ptr<Invalidator>& invalidator)
        : m_invalidator(invalidator)
      {}

      Action act() override {
        return _return(avena::provider::ResourceHandle<avena::data::stream::IOStream>(
          std::make_shared<StubStream>(),
          m_invalidator
        ));
      }

    };

    ++ counter;
    return ConnectionCoroutine::startForResult(m_invalidator);

  }

  void stop() override {
    // DO NOTHING
  }

};

class ClientCoroutine : public avena::async::Coroutine<ClientCoroutine> {
private:
  std::shared_ptr<ConnectionPool> m_pool;
  avena::provider::ResourceHandle<avena::data::stream::IOStream> m_connection;
  v_int32 m_repeats;
  bool m_invalidate;
public:

  ClientCoroutine(const std::shared_ptr<ConnectionPool>& pool, bool invalidate)
    : m_pool(pool)
    , m_repeats(0)
    , m_invalidate(invalidate)
  {}

  Action act() override {
    return m_pool->getAsync().callbackTo(&ClientCoroutine::onConnection);
  }

  Action onConnection(const avena::provider::ResourceHandle<avena::data::stream::IOStream>& connection) {
    m_connection = connection;
    return yieldTo(&ClientCoroutine::useConnection);
  }

  Action useConnection() {
    if(m_repeats < 1) {
      m_repeats ++;
      return waitFor(std::chrono::milliseconds(100)).next(yieldTo(&ClientCoroutine::useConnection));
    }
    if(m_invalidate) {
      m_connection.invalidator->invalidate(m_connection.object);
    }
    return finish();
  }

};

void clientMethod(std::shared_ptr<ConnectionPool> pool, bool invalidate) {
  auto connection = pool->get();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  if(invalidate) {
    connection.invalidator->invalidate(connection.object);
  }
}

}

void ConnectionPoolTest::onRun() {

  avena::async::Executor executor(1, 1, 1);

  auto connectionProvider = std::make_shared<StubStreamProvider>();
  auto pool = ConnectionPool::createShared(connectionProvider, 10 /* maxConnections */, std::chrono::seconds(10) /* maxConnectionTTL */);

  std::list<std::thread> threads;

  for(v_int32 i = 0; i < 100; i ++ ) {

    threads.push_back(std::thread(clientMethod, pool, false));
    executor.execute<ClientCoroutine>(pool, false);

  }

  for(std::thread& thread : threads) {
    thread.join();
  }

  executor.waitTasksFinished();

  OATPP_LOGd(TAG, "connections_counter={}", connectionProvider->counter.load())
  OATPP_ASSERT(connectionProvider->counter <= 10)

  pool->stop();

  executor.stop();
  executor.join();

  /* wait pool cleanup task exit */
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

}

}}}
