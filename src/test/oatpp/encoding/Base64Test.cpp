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

#include "Base64Test.hpp"

#include "oatpp/encoding/Base64.hpp"
#include "oatpp/base/Log.hpp"

namespace avena { namespace test { namespace encoding {
  
void Base64Test::onRun() {

  avena::String message = "oat++ web framework";
  avena::String messageEncoded = "b2F0Kysgd2ViIGZyYW1ld29yaw==";
  
  {
    avena::String encoded = avena::encoding::Base64::encode(message);
    OATPP_LOGv(TAG, "encoded='{}'", encoded->c_str())
    OATPP_ASSERT(encoded == messageEncoded)
    avena::String decoded = avena::encoding::Base64::decode(encoded);
    OATPP_ASSERT(message == decoded)
  }
  
  {
    avena::String encoded = avena::encoding::Base64::encode(message, avena::encoding::Base64::ALPHABET_BASE64_URL_SAFE);
    OATPP_LOGv(TAG, "encoded='{}'", encoded->c_str())
    avena::String decoded = avena::encoding::Base64::decode(encoded, avena::encoding::Base64::ALPHABET_BASE64_URL_SAFE_AUXILIARY_CHARS);
    OATPP_ASSERT(message == decoded)
  }

}
  
}}}
