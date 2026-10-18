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

#include "AnyTest.hpp"

#include "oatpp/data/type/Any.hpp"

#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/macro/codegen.hpp"

namespace avena { namespace data { namespace  type {

namespace {

#include OATPP_CODEGEN_BEGIN(DTO)

class Dto1 : public avena::DTO {
  DTO_INIT(Dto1, DTO)
};

class Dto2 : public avena::DTO {
  DTO_INIT(Dto2, DTO)
};

class Test : public avena::DTO {

  DTO_INIT(Test, DTO)

  DTO_FIELD(avena::Any, any);

};

#include OATPP_CODEGEN_END(DTO)

}

void AnyTest::onRun() {

  {
    OATPP_LOGi(TAG, "Test default constructor...")
    avena::Any any;
    OATPP_ASSERT(!any)
    OATPP_ASSERT(any.getValueType() == avena::data::type::__class::Any::getType())
    OATPP_ASSERT(any.getStoredType() == nullptr)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "Test nullptr constructor...")
    avena::Any any(nullptr);
    OATPP_ASSERT(!any)
    OATPP_ASSERT(any.getValueType() == avena::data::type::__class::Any::getType())
    OATPP_ASSERT(any.getStoredType() == nullptr)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "Test retrieve()...")
    avena::Any any(avena::String("Hello Any!"));
    OATPP_ASSERT(any)
    OATPP_ASSERT(any.getValueType() == avena::data::type::__class::Any::getType())
    OATPP_ASSERT(any.getStoredType() == avena::data::type::__class::String::getType())
    auto str = any.retrieve<avena::String>();
    OATPP_ASSERT(str == "Hello Any!")
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "Test store()...")
    avena::Any any(avena::Int32(32));

    OATPP_ASSERT(any)
    OATPP_ASSERT(any.getValueType() == avena::data::type::__class::Any::getType())
    OATPP_ASSERT(any.getStoredType() == avena::data::type::__class::Int32::getType())

    any.store(avena::String("Hello Any!"));

    OATPP_ASSERT(any)
    OATPP_ASSERT(any.getValueType() == avena::data::type::__class::Any::getType())
    OATPP_ASSERT(any.getStoredType() == avena::data::type::__class::String::getType())

    auto str = any.retrieve<avena::String>();
    OATPP_ASSERT(str == "Hello Any!")
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "Test retrieve() class check...")
    avena::Any any(Dto1::createShared());
    OATPP_ASSERT(any)
    OATPP_ASSERT(any.getValueType() == avena::data::type::__class::Any::getType())
    OATPP_ASSERT(any.getStoredType() == Object<Dto1>::Class::getType())

    bool wasError = false;

    try {
      auto obj = any.retrieve<avena::Object<Dto2>>(); // wrong object
    } catch (std::runtime_error&) {
      wasError = true;
    }

    OATPP_ASSERT(wasError)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "Test copy-assign operator...")
    avena::Any any1(avena::String("Hello!"));
    avena::Any any2;

    any2 = any1;

    OATPP_ASSERT(any1)
    OATPP_ASSERT(any2)

    OATPP_ASSERT(any1.getValueType() == avena::data::type::__class::Any::getType())
    OATPP_ASSERT(any2.getValueType() == avena::data::type::__class::Any::getType())

    OATPP_ASSERT(any1.getStoredType() == avena::data::type::__class::String::getType())
    OATPP_ASSERT(any2.getStoredType() == avena::data::type::__class::String::getType())

    OATPP_ASSERT(any1 == any2)
    OATPP_ASSERT(any1.getPtr().get() != any2.getPtr().get())

    auto str1 = any1.retrieve<avena::String>();
    auto str2 = any2.retrieve<avena::String>();

    OATPP_ASSERT(str1 == str2)
    OATPP_ASSERT(str1.get() == str2.get() && str1 == "Hello!")
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "Test move-assign operator...")
    avena::Any any1(avena::String("Hello!"));
    avena::Any any2;

    any2 = std::move(any1);

    OATPP_ASSERT(!any1)
    OATPP_ASSERT(any2)

    OATPP_ASSERT(any1.getValueType() == avena::data::type::__class::Any::getType())
    OATPP_ASSERT(any2.getValueType() == avena::data::type::__class::Any::getType())

    OATPP_ASSERT(any1.getStoredType() == nullptr)
    OATPP_ASSERT(any2.getStoredType() == avena::data::type::__class::String::getType())

    OATPP_ASSERT(any1 != any2)
    OATPP_ASSERT(any1.getPtr().get() != any2.getPtr().get())

    auto str1 = any1.retrieve<avena::String>();
    auto str2 = any2.retrieve<avena::String>();

    OATPP_ASSERT(str1 != str2)
    OATPP_ASSERT(str2 == "Hello!")
    OATPP_LOGi(TAG, "OK")
  }

}

}}}
