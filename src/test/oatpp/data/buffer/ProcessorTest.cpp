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

#include "ProcessorTest.hpp"

#include "oatpp/data/stream/BufferStream.hpp"
#include "oatpp/data/buffer/Processor.hpp"

#include "oatpp/async/Executor.hpp"
#include "oatpp/base/Log.hpp"

namespace avena { namespace data { namespace buffer {

namespace {

typedef avena::data::buffer::Processor Processor;

class BaseProcessor : public avena::data::buffer::Processor {
private:
  v_int32 m_bufferSize;
  avena::data::stream::BufferOutputStream m_buffer;
public:

  BaseProcessor(v_int32 bufferSize)
    : m_bufferSize(bufferSize)
  {}

  avena::v_io_size suggestInputStreamReadSize() override {
    return m_bufferSize;
  }

  virtual void process(p_char8 data, v_buff_size size) = 0;

  v_int32 iterate(avena::data::buffer::InlineReadData& dataIn, avena::data::buffer::InlineReadData& dataOut) override {

    if(dataOut.bytesLeft > 0) {
      return Error::FLUSH_DATA_OUT;
    }

    if(dataIn.currBufferPtr != nullptr) {

      while (dataIn.bytesLeft > 0 && m_buffer.getCurrentPosition() < m_bufferSize) {
        m_buffer.writeSimple(dataIn.currBufferPtr, 1);
        dataIn.inc(1);
      }

      if(m_buffer.getCurrentPosition() < m_bufferSize) {
        return Error::PROVIDE_DATA_IN;
      }

      dataOut.set(m_buffer.getData(), m_buffer.getCurrentPosition());
      process(m_buffer.getData(), m_buffer.getCurrentPosition());
      m_buffer.setCurrentPosition(0);
      return Error::FLUSH_DATA_OUT;

    } else if(m_buffer.getCurrentPosition() > 0) {
      dataOut.set(m_buffer.getData(), m_buffer.getCurrentPosition());
      process(m_buffer.getData(), m_buffer.getCurrentPosition());
      m_buffer.setCurrentPosition(0);
      return Error::FLUSH_DATA_OUT;
    }

    dataOut.set(nullptr, 0);
    return Error::FINISHED;

  }

};

class ProcessorToUpper : public BaseProcessor {
public:

  ProcessorToUpper(v_int32 bufferSize) : BaseProcessor(bufferSize) {}

  void process(p_char8 data, v_buff_size size) override {
    utils::String::upperCase_ASCII(data, size);
  }

};

class ProcessorToLower : public BaseProcessor {
public:

  ProcessorToLower(v_int32 bufferSize) : BaseProcessor(bufferSize) {}

  void process(p_char8 data, v_buff_size size) override {
    utils::String::lowerCase_ASCII(data, size);
  }

};

class ProcessorChangeLetter : public BaseProcessor {
private:
  v_char8 m_fromChar;
  v_char8 m_toChar;
public:

  ProcessorChangeLetter(v_char8 fromChar, v_char8 toChar, v_int32 bufferSize)
    : BaseProcessor(bufferSize)
    , m_fromChar(fromChar)
    , m_toChar(toChar)
  {}

  void process(p_char8 data, v_buff_size size) override {
    for(v_buff_size i = 0; i < size; i ++) {

      if(data[i] == m_fromChar) {
        data[i] = m_toChar;
      }

    }
  }

};

class TestCoroutine : public avena::async::Coroutine<TestCoroutine> {
public:
  static std::atomic<v_int64> COUNTER;
private:
  avena::String m_data;
  avena::String m_etalon;
  std::shared_ptr<avena::data::stream::ReadCallback> m_readCallback;
  std::shared_ptr<avena::data::stream::BufferOutputStream> m_writeCallback;
  std::shared_ptr<avena::data::buffer::Processor> m_processor;
  std::shared_ptr<avena::data::buffer::IOBuffer> m_buffer;
public:

  TestCoroutine(const avena::String &mData,
                const avena::String &mEtalon,
                const std::shared_ptr<avena::data::stream::ReadCallback> &mReadCallback,
                const std::shared_ptr<avena::data::stream::BufferOutputStream> &mWriteCallback,
                const std::shared_ptr<Processor> &mProcessor,
                const std::shared_ptr<avena::data::buffer::IOBuffer> &mBuffer)
    : m_data(mData)
    , m_etalon(mEtalon)
    , m_readCallback(mReadCallback)
    , m_writeCallback(mWriteCallback)
    , m_processor(mProcessor)
    , m_buffer(mBuffer)
  {
    COUNTER ++;
  }

  ~TestCoroutine() override {
    COUNTER --;
  }

  Action act() override {
    return avena::data::stream::transferAsync(m_readCallback, m_writeCallback, 0, m_buffer, m_processor)
      .next(yieldTo(&TestCoroutine::compare));
  }

  Action compare() {
    auto result = m_writeCallback->toString();
    OATPP_ASSERT(m_etalon == result)
    return finish();
  }

};

std::atomic<v_int64> TestCoroutine::COUNTER(0);

avena::String runTestCase(const avena::String& data, v_int32 p1N, v_int32 p2N, v_int32 p3N, v_int32 bufferSize) {

  avena::data::stream::BufferInputStream inStream(data);
  avena::data::stream::BufferOutputStream outStream;

  avena::data::buffer::ProcessingPipeline processor({
    avena::base::ObjectHandle<Processor>(std::make_shared<ProcessorToUpper>(p1N)),
    avena::base::ObjectHandle<Processor>(std::make_shared<ProcessorChangeLetter>('A', '-', p2N)),
    avena::base::ObjectHandle<Processor>(std::make_shared<ProcessorToLower>(p3N)),
  });

  std::unique_ptr<v_char8[]> buffer(new v_char8[static_cast<unsigned long>(bufferSize)]);
  avena::data::stream::transfer(&inStream, &outStream, 0, buffer.get(), bufferSize, &processor);

  return outStream.toString();

}

}

void ProcessorTest::onRun() {

  avena::String data = ">aaaaa0123456789bbbbb<";
  avena::String etalon = ">-----0123456789bbbbb<";

  for(v_int32 p1N = 1; p1N < 11; p1N ++) {
    for(v_int32 p2N = 1; p2N < 11; p2N ++) {
      for (v_int32 p3N = 1; p3N < 11; p3N++) {
        for (v_int32 buffSize = 1; buffSize < 11; buffSize++) {

          auto result = runTestCase(data, p1N, p2N, p3N, buffSize);

          if (result != etalon) {
            OATPP_LOGd(TAG, "error[{}, {}, {}, b={}] result='{}'", p1N, p2N, p3N, buffSize, result->data())
          }
          OATPP_ASSERT(result == etalon)

        }
      }
    }
  }

  {

    v_char8 buffer[1024];

    for(v_int32 i = 1; i < 30; i++) {

      avena::data::stream::BufferInputStream inStream(data);
      avena::data::stream::BufferOutputStream outStream;

      auto progress = avena::data::stream::transfer(&inStream, &outStream, 0, buffer, i);

      auto result = outStream.toString();

      OATPP_ASSERT(result == data)
      OATPP_ASSERT(inStream.getCurrentPosition() == progress)

    }

  }

  avena::async::Executor executor;

  {

    for(v_int32 p1N = 1; p1N < 11; p1N ++) {
      for(v_int32 p2N = 1; p2N < 11; p2N ++) {
        for (v_int32 p3N = 1; p3N < 11; p3N++) {

          auto inStream = std::make_shared<avena::data::stream::BufferInputStream>(data);
          auto outStream = std::make_shared<avena::data::stream::BufferOutputStream>();

          auto processor = std::shared_ptr<avena::data::buffer::ProcessingPipeline>(new avena::data::buffer::ProcessingPipeline({
            avena::base::ObjectHandle<Processor>(std::make_shared<ProcessorToUpper>(p1N)),
            avena::base::ObjectHandle<Processor>(std::make_shared<ProcessorChangeLetter>('A', '-', p2N)),
            avena::base::ObjectHandle<Processor>(std::make_shared<ProcessorToLower>(p3N)),
          }));

          auto buffer = std::make_shared<avena::data::buffer::IOBuffer>();

          executor.execute<TestCoroutine>(data, etalon, inStream, outStream, processor, buffer);

        }
      }
    }

    while(TestCoroutine::COUNTER > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

  }

  executor.waitTasksFinished();
  executor.stop();
  executor.join();

}

}}}
