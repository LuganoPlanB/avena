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

#include "ListTest.hpp"

#include "oatpp/Types.hpp"

namespace avena { namespace data { namespace  type {

void ListTest::onRun() {

  {
    OATPP_LOGi(TAG, "test default constructor...")
    avena::List<avena::String> list;

    OATPP_ASSERT(!list)
    OATPP_ASSERT(list == nullptr)

    OATPP_ASSERT(list.get() == nullptr)
    OATPP_ASSERT(list.getValueType()->classId.id == avena::data::type::__class::AbstractList::CLASS_ID.id)
    OATPP_ASSERT(list.getValueType()->params.size() == 1)
    OATPP_ASSERT(list.getValueType()->params.front() == avena::String::Class::getType())
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test empty ilist constructor...")
    avena::List<avena::String> list({});

    OATPP_ASSERT(list)
    OATPP_ASSERT(list != nullptr)
    OATPP_ASSERT(list->size() == 0)

    OATPP_ASSERT(list.get() != nullptr)
    OATPP_ASSERT(list.getValueType()->classId.id == avena::data::type::__class::AbstractList::CLASS_ID.id)
    OATPP_ASSERT(list.getValueType()->params.size() == 1)
    OATPP_ASSERT(list.getValueType()->params.front() == avena::String::Class::getType())
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test createShared()...")
    avena::List<avena::String> list = avena::List<avena::String>::createShared();

    OATPP_ASSERT(list)
    OATPP_ASSERT(list != nullptr)
    OATPP_ASSERT(list->size() == 0)

    OATPP_ASSERT(list.get() != nullptr)
    OATPP_ASSERT(list.getValueType()->classId.id == avena::data::type::__class::AbstractList::CLASS_ID.id)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test copy-assignment operator...")
    avena::List<avena::String> list1({});
    avena::List<avena::String> list2;

    list2 = list1;

    OATPP_ASSERT(list1)
    OATPP_ASSERT(list2)

    OATPP_ASSERT(list1->size() == 0)
    OATPP_ASSERT(list2->size() == 0)

    OATPP_ASSERT(list1.get() == list2.get())

    list2->push_back("a");

    OATPP_ASSERT(list1->size() == 1)
    OATPP_ASSERT(list2->size() == 1)

    list2 = {"b", "c"};

    OATPP_ASSERT(list1->size() == 1)
    OATPP_ASSERT(list2->size() == 2)

    OATPP_ASSERT(list2[0] == "b")
    OATPP_ASSERT(list2[1] == "c")
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test move-assignment operator...")
    avena::List<avena::String> list1({});
    avena::List<avena::String> list2;

    list2 = std::move(list1);

    OATPP_ASSERT(!list1)
    OATPP_ASSERT(list2)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test get element by index...")
    avena::List<avena::String> list = {"a", "b", "c"};

    OATPP_ASSERT(list)
    OATPP_ASSERT(list != nullptr)
    OATPP_ASSERT(list->size() == 3)

    OATPP_ASSERT(list[0] == "a")
    OATPP_ASSERT(list[1] == "b")
    OATPP_ASSERT(list[2] == "c")

    list[1] = "Hello!";

    OATPP_ASSERT(list->size() == 3)

    OATPP_ASSERT(list[0] == "a")
    OATPP_ASSERT(list[1] == "Hello!")
    OATPP_ASSERT(list[2] == "c")
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test polymorphicDispatcher...")
    avena::List<avena::String> list = {"a", "b", "c"};

    auto polymorphicDispatcher = static_cast<const avena::data::type::__class::Collection::PolymorphicDispatcher*>(
      list.getValueType()->polymorphicDispatcher
    );

    polymorphicDispatcher->addItem(list, avena::String("d"));

    OATPP_ASSERT(list->size() == 4)

    OATPP_ASSERT(list[0] == "a")
    OATPP_ASSERT(list[1] == "b")
    OATPP_ASSERT(list[2] == "c")
    OATPP_ASSERT(list[3] == "d")
    OATPP_LOGi(TAG, "OK")
  }

}

}}}
