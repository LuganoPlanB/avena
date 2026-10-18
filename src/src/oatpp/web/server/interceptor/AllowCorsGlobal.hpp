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

#ifndef oatpp_web_server_interceptor_AllowCorsGlobal_hpp
#define oatpp_web_server_interceptor_AllowCorsGlobal_hpp

#include "oatpp/web/server/interceptor/ResponseInterceptor.hpp"
#include "oatpp/web/server/interceptor/RequestInterceptor.hpp"

namespace avena { namespace web { namespace server { namespace interceptor {

class AllowOptionsGlobal : public RequestInterceptor {
public:
  std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request) override;
};

class AllowCorsGlobal : public ResponseInterceptor {
private:
  avena::String m_origin;
  avena::String m_methods;
  avena::String m_headers;
  avena::String m_maxAge;
public:

  AllowCorsGlobal(const avena::String &origin = "*",
                  const avena::String &methods = "GET, POST, OPTIONS",
                  const avena::String &headers = "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization",
                  const avena::String &maxAge = "1728000");

  std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request,
                                              const std::shared_ptr<OutgoingResponse>& response) override;

};

}}}}

#endif // oatpp_web_server_interceptor_AllowCorsGlobal_hpp
