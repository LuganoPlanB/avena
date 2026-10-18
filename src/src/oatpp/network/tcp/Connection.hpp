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

#ifndef oatpp_network_tcp_Connection_hpp
#define oatpp_network_tcp_Connection_hpp

#include "oatpp/data/stream/Stream.hpp"

namespace avena { namespace network { namespace tcp {

/**
 * TCP Connection implementation. Extends &id:avena::base::Countable; and &id:avena::data::stream::IOStream;.
 */
class Connection : public avena::base::Countable, public avena::data::stream::IOStream {
private:
  static avena::data::stream::DefaultInitializedContext DEFAULT_CONTEXT;
private:
  v_io_handle m_handle;
  data::stream::IOMode m_mode;
private:
  void setStreamIOMode(avena::data::stream::IOMode ioMode);
public:
  /**
   * Constructor.
   * @param handle - file descriptor (socket handle). See &id:avena::v_io_handle;.
   */
  Connection(v_io_handle handle);
public:

  /**
   * Virtual Destructor (See &id:avena::base::Countable;).
   * Close socket handle.
   */
  ~Connection() override;

  /**
   * Implementation of &id:avena::data::stream::IOStream::write;.
   * @param buff - buffer containing data to write.
   * @param count - bytes count you want to write.
   * @param action - async specific action. If action is NOT &id:avena::async::Action::TYPE_NONE;, then
   * caller MUST return this action on coroutine iteration.
   * @return - actual amount of bytes written. See &id:avena::v_io_size;.
   */
  v_io_size write(const void *buff, v_buff_size count, async::Action& action) override;

  /**
   * Implementation of &id:avena::data::stream::IOStream::read;.
   * @param buff - buffer to read data to.
   * @param count - buffer size.
   * @param action - async specific action. If action is NOT &id:avena::async::Action::TYPE_NONE;, then
   * caller MUST return this action on coroutine iteration.
   * @return - actual amount of bytes read. See &id:avena::v_io_size;.
   */
  v_io_size read(void *buff, v_buff_size count, async::Action& action) override;

  /**
   * Set OutputStream I/O mode.
   * @param ioMode
   */
  void setOutputStreamIOMode(avena::data::stream::IOMode ioMode) override;

  /**
   * Set OutputStream I/O mode.
   * @return
   */
  avena::data::stream::IOMode getOutputStreamIOMode() override;

  /**
   * Get output stream context.
   * @return - &id:avena::data::stream::Context;.
   */
  avena::data::stream::Context& getOutputStreamContext() override;

  /**
   * Set InputStream I/O mode.
   * @param ioMode
   */
  void setInputStreamIOMode(avena::data::stream::IOMode ioMode) override;

  /**
   * Get InputStream I/O mode.
   * @return
   */
  avena::data::stream::IOMode getInputStreamIOMode() override;

  /**
   * Get input stream context. <br>
   * @return - &id:avena::data::stream::Context;.
   */
  avena::data::stream::Context& getInputStreamContext() override;

  /**
   * Close socket handle.
   */
  void close();

  /**
   * Get socket handle.
   * @return - socket handle. &id:avena::v_io_handle;.
   */
  v_io_handle getHandle(){
    return m_handle;
  }
  
};
  
}}}

#endif /* oatpp_network_tcp_Connection_hpp */
