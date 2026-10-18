// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.
//
//  AsyncController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef oatpp_test_websocket_app_websoAsyncController_hpp
#define oatpp_test_websocket_app_websoAsyncController_hpp

#include "oatpp-websocket/Handshaker.hpp"
#include "oatpp-websocket/AsyncConnectionHandler.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/base/Log.hpp"

namespace avena { namespace test {namespace websocket { namespace app {

class AsyncController : public avena::web::server::api::ApiController {
private:
  typedef AsyncController __ControllerType;
private:
  OATPP_COMPONENT(std::shared_ptr<avena::websocket::AsyncConnectionHandler>, websocketConnectionHandler);
protected:
  AsyncController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : avena::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<AsyncController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)){
    return std::shared_ptr<AsyncController>(new AsyncController(objectMapper));
  }

  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)

  ENDPOINT_ASYNC("GET", "ws", WS) {

    static std::atomic<v_int32>& getAtom() {
      static std::atomic<v_int32> atom(0);
      return atom;
    }

    ENDPOINT_ASYNC_INIT(WS)

    Action act() override {
      getAtom() ++;
      auto params = std::make_shared<avena::network::ConnectionHandler::ParameterMap>();
      (*params)["p1"] = "v1";
      auto response = avena::websocket::Handshaker::serversideHandshake(request->getHeaders(), controller->websocketConnectionHandler);
      response->setConnectionUpgradeParameters(params);
      return _return(response);
    }

  };

  ENDPOINT_ASYNC("GET", "*", ALL) {

  ENDPOINT_ASYNC_INIT(ALL)

    Action act() override {
      avena::String path = request->getPathTail();
      if(path) {
        OATPP_LOGd("Controller", "path='{}'", path);
      } else {
        OATPP_LOGd("Controller", "path='{}'", "nullptr");
      }
      return _return(controller->createResponse(Status::CODE_400, "wtf"));
    }

  };

  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)

};

}}}}

#endif /* oatpp_test_websocket_app_websoAsyncController_hpp */
