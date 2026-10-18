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
 *                         Benedikt-Alexander Mokroß <bam@icognize.de>
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

#ifndef oatpp_test_web_app_BearerAuthorizationController_hpp
#define oatpp_test_web_app_BearerAuthorizationController_hpp

#include "./DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/utils/Conversion.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"

#include <sstream>

namespace avena { namespace test { namespace web { namespace app {

class BearerAuthorizationObject : public avena::web::server::handler::AuthorizationObject {
public:
  avena::String user;
  avena::String password;
  avena::String token;
};

class MyBearerAuthorizationHandler : public avena::web::server::handler::BearerAuthorizationHandler {
public:

  MyBearerAuthorizationHandler()
    : avena::web::server::handler::BearerAuthorizationHandler("custom-bearer-realm")
  {}

  std::shared_ptr<AuthorizationObject> authorize(const avena::String& token) override {

    if(token == "4e99e8c12de7e01535248d2bac85e732") {
      auto obj = std::make_shared<BearerAuthorizationObject>();
      obj->user = "foo";
      obj->password = "bar";
      obj->token = token;
      return obj;
    }

    return nullptr;
  }

};

class BearerAuthorizationController : public avena::web::server::api::ApiController {
private:
  static constexpr const char* TAG = "test::web::app::BearerAuthorizationController";

private:
  std::shared_ptr<AuthorizationHandler> m_authHandler = std::make_shared<MyBearerAuthorizationHandler>();
public:

  BearerAuthorizationController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : avena::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<BearerAuthorizationController> createShared(const std::shared_ptr<ObjectMapper>& objectMapper = OATPP_GET_COMPONENT(std::shared_ptr<ObjectMapper>)){
    return std::make_shared<BearerAuthorizationController>(objectMapper);
  }

#include OATPP_CODEGEN_BEGIN(ApiController)

  ENDPOINT("GET", "bearer-authorization", authorization,
           AUTHORIZATION(std::shared_ptr<BearerAuthorizationObject>, authorizatioBearer, m_authHandler)) {
    auto dto = TestDto::createShared();
    dto->testValue = authorizatioBearer->user + ":" + authorizatioBearer->password;
    if(dto->testValue == "foo:bar" && authorizatioBearer->token == "4e99e8c12de7e01535248d2bac85e732") {
      return createDtoResponse(Status::CODE_200, dto);
    } else {
      return createDtoResponse(Status::CODE_401, dto);
    }
  }

#include OATPP_CODEGEN_END(ApiController)

};

}}}}

#endif /* oatpp_test_web_app_Controller_hpp */
