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

#include "StringTest.hpp"

#include "oatpp/Types.hpp"

#include <functional>

namespace avena { namespace data { namespace  type {

void StringTest::onRun() {

  {
    avena::String s = "hello"; // check hash function exists
    std::hash<avena::String>{}(s);
  }

  {
    OATPP_LOGi(TAG, "test default constructor")
    avena::String s;
    OATPP_ASSERT(!s)
    OATPP_ASSERT(s == nullptr)
    OATPP_ASSERT(s == static_cast<const char*>(nullptr))
    OATPP_ASSERT(s.getValueType() == avena::String::Class::getType())
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test nullptr constructor")
    avena::String s(nullptr);
    OATPP_ASSERT(!s)
    OATPP_ASSERT(s == nullptr)
    OATPP_ASSERT(s.getValueType() == avena::String::Class::getType())
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test const char* constructor")
    avena::String s("abc\0xyz");
    OATPP_ASSERT(s)
    OATPP_ASSERT(s != nullptr)
    OATPP_ASSERT(s->size() == 3)
    OATPP_ASSERT(s == "abc")
    OATPP_ASSERT(s == "abc\0xyz")
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test std::string constructor")
    std::string a("abc\0xyz", 7);
    avena::String s(a);
    OATPP_ASSERT(s)
    OATPP_ASSERT(s != nullptr)
    OATPP_ASSERT(s->size() == 7)
    OATPP_ASSERT(s != "abc")
    OATPP_ASSERT(s != "abc\0xyz")
    OATPP_ASSERT(s == std::string("abc\0xyz", 7))
    OATPP_ASSERT(s == a)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test std::string move constructor")
    std::string a("abc\0xyz", 7);
    avena::String s(std::move(a));
    OATPP_ASSERT(s)
    OATPP_ASSERT(s != nullptr)
    OATPP_ASSERT(s->size() == 7)
    OATPP_ASSERT(s != "abc")
    OATPP_ASSERT(s != "abc\0xyz")
    OATPP_ASSERT(s == std::string("abc\0xyz", 7))
    OATPP_ASSERT(a == "")
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test const char* assign operator")
    avena::String s;
    s = "abc\0xyz";
    OATPP_ASSERT(s)
    OATPP_ASSERT(s != nullptr)
    OATPP_ASSERT(s->size() == 3)
    OATPP_ASSERT(s == "abc")
    OATPP_ASSERT(s == "abc\0xyz")
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test std::string assign operator")
    avena::String s;
    std::string a = std::string("abc\0xyz", 7);
    s = a;
    OATPP_ASSERT(s)
    OATPP_ASSERT(s != nullptr)
    OATPP_ASSERT(s->size() == 7)
    OATPP_ASSERT(s != "abc")
    OATPP_ASSERT(s != "abc\0xyz")
    OATPP_ASSERT(s == std::string("abc\0xyz", 7))
    OATPP_ASSERT(s == a)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test std::string move assign operator")
    avena::String s;
    std::string a = std::string("abc\0xyz", 7);
    s = std::move(a);
    OATPP_ASSERT(s)
    OATPP_ASSERT(s != nullptr)
    OATPP_ASSERT(s->size() == 7)
    OATPP_ASSERT(s != "abc")
    OATPP_ASSERT(s != "abc\0xyz")
    OATPP_ASSERT(s == std::string("abc\0xyz", 7))
    OATPP_ASSERT(a == "")
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test const char* implicit constructor")
    avena::String s = "";
    OATPP_ASSERT(s)
    OATPP_ASSERT(s != nullptr)
    OATPP_ASSERT(s->size() == 0)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test '==', '!=' operators")
    avena::String s1 = "a";
    avena::String s2;

    OATPP_ASSERT(s1 != s2)
    OATPP_ASSERT(s2 != s1)

    OATPP_ASSERT(s1 == "a")
    OATPP_ASSERT(s1 != "aa")
    OATPP_ASSERT(s1 != "")

    s2 = "aa";

    OATPP_ASSERT(s1 != s2)
    OATPP_ASSERT(s2 != s1)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test copy-asssign operator")
    avena::String s1 = "s1";
    avena::String s2;

    s2 = s1;

    OATPP_ASSERT(s1 == s2)
    OATPP_ASSERT(s1.get() == s2.get())

    s1 = "s2";

    OATPP_ASSERT(s1 != s2)
    OATPP_ASSERT(s2 != s1)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test const char* assign operator")
    avena::String s1 = "s1";
    avena::String s2(s1);

    OATPP_ASSERT(s1 == s2)
    OATPP_ASSERT(s1.get() == s2.get())

    s1 = "s2";

    OATPP_ASSERT(s1 != s2)
    OATPP_ASSERT(s2 != s1)
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test move assign operator")
    avena::String s1 = "s1";
    avena::String s2;

    s2 = std::move(s1);

    OATPP_ASSERT(s1 == nullptr)
    OATPP_ASSERT(s2 != nullptr)
    OATPP_ASSERT(s2 == "s1")

    OATPP_ASSERT(s1 != s2)
    OATPP_ASSERT(s1.get() != s2.get())
    OATPP_LOGi(TAG, "OK")
  }

  {
    OATPP_LOGi(TAG, "test compareCI_ASCII methods 1")

    avena::String s1 = "hello";

    {
      avena::String s2;
      OATPP_ASSERT(!s1.equalsCI_ASCII(s2))
    }

    {
      const char* s2 = nullptr;
      OATPP_ASSERT(!s1.equalsCI_ASCII(s2))
    }

  }

  {
    OATPP_LOGi(TAG, "test compareCI_ASCII methods 2")

    avena::String s1;

    {
      avena::String s2 = "hello";
      OATPP_ASSERT(!s1.equalsCI_ASCII(s2))
    }

    {
      std::string s2 = "hello";
      OATPP_ASSERT(!s1.equalsCI_ASCII(s2))
    }

    {
      const char* s2 = "hello";
      OATPP_ASSERT(!s1.equalsCI_ASCII(s2))
    }

    {
      avena::String s2;
      OATPP_ASSERT(s1.equalsCI_ASCII(s2))
    }

    {
      const char* s2 = nullptr;
      OATPP_ASSERT(s1.equalsCI_ASCII(s2))
    }

    {
      OATPP_ASSERT(s1.equalsCI_ASCII(nullptr))
    }

    {

      bool exceptionThrown = false;

      try {
        std::string s2 = s1;
      } catch (const std::runtime_error&) {
        exceptionThrown = true;
      }

      OATPP_ASSERT(exceptionThrown)

    }

  }

  {
    OATPP_LOGi(TAG, "test compareCI_ASCII methods 3")

    avena::String s1 = "hello";

    {
      avena::String s2 = "HELLO";
      OATPP_ASSERT(s1.equalsCI_ASCII(s2))
    }

    {
      std::string s2 = "HELLO";
      OATPP_ASSERT(s1.equalsCI_ASCII(s2))
    }

    {
      const char* s2 = "HELLO";
      OATPP_ASSERT(s1.equalsCI_ASCII(s2))
    }

    {
      OATPP_ASSERT(s1.equalsCI_ASCII("HELLO"))
    }

  }

  {
    OATPP_LOGi(TAG, "check default value")
    avena::String s0;
    avena::String s1 = "hello";
    OATPP_ASSERT(s0.getValue("def") == "def")
    OATPP_ASSERT(s1.getValue("def") == "hello")
  }

}

}}}
