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

#include "InMemoryDataTest.hpp"

#include "oatpp/data/resource/InMemoryData.hpp"

namespace avena { namespace data { namespace resource {

void InMemoryDataTest::onRun() {

  {
    avena::data::resource::InMemoryData data;

    OATPP_ASSERT(data.getKnownSize() == 0)
    OATPP_ASSERT(data.getInMemoryData() == nullptr)
    OATPP_ASSERT(data.getLocation() == nullptr)
  }

  {
    avena::String testData = "Hello World";
    avena::data::resource::InMemoryData data;

    {
      auto s = data.openOutputStream();
      s->writeExactSizeDataSimple(testData->data(), static_cast<v_buff_size>(testData->size()));
    }

    OATPP_ASSERT(data.getKnownSize() == static_cast<v_int64>(testData->size()))
    OATPP_ASSERT(data.getInMemoryData() == testData)
    OATPP_ASSERT(data.getLocation() == nullptr)
  }

  {
    avena::String testData1 = "Hello";
    avena::String testData2 = "World";
    avena::data::resource::InMemoryData data("data=");

    {
      auto s1 = data.openOutputStream();
      s1->writeExactSizeDataSimple(testData1->data(), static_cast<v_buff_size>(testData1->size()));

      auto s2 = data.openOutputStream();
      s2->writeExactSizeDataSimple(testData2->data(), static_cast<v_buff_size>(testData2->size()));

      s1.reset();
      OATPP_ASSERT(data.getInMemoryData() == "data=" + testData1)
    }

    OATPP_ASSERT(data.getInMemoryData() == "data=" + testData2)
  }

  {
    avena::String testData = "Hello";
    avena::data::resource::InMemoryData data("data=");

    auto is = data.openInputStream();

    {
      auto s1 = data.openOutputStream();
      s1->writeExactSizeDataSimple(testData->data(), static_cast<v_buff_size>(testData->size()));
    }

    avena::data::stream::BufferOutputStream s;
    char buffer[100];
    avena::data::stream::transfer(is, &s, 0, buffer, 100);

    OATPP_ASSERT(s.toString() == "data=")

  }

}

}}}
