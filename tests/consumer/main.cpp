// Copyright (C) 2026 Plan-B Foundation
// SPDX-License-Identifier: Apache-2.0
#include <avena/Environment.hpp>
#include <avena/openssl/Config.hpp>
#include <avena/websocket/Handshaker.hpp>
#include <avena/json/ObjectMapper.hpp>
#include <avena/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)
class Payload : public oatpp::DTO {
  DTO_INIT(Payload, DTO)
  DTO_FIELD(String, message);
};
#include OATPP_CODEGEN_END(DTO)

int main() {
  oatpp::Environment::init();
  bool valid = false;
  {
    auto config = oatpp::openssl::Config::createDefaultClientConfigShared();
    oatpp::websocket::Handshaker::Headers headers;
    oatpp::websocket::Handshaker::clientsideHandshake(headers);
    auto payload = Payload::createShared();
    payload->message = "avena";
    auto mapper = std::make_shared<oatpp::json::ObjectMapper>();
    auto context = SSL_CTX_new(TLS_client_method());
    valid = config != nullptr && headers.get("Upgrade") == "websocket" &&
            mapper->writeToString(payload) == "{\"message\":\"avena\"}" && context != nullptr;
    SSL_CTX_free(context);
  }
  const auto remaining = oatpp::Environment::getObjectsCount();
  oatpp::Environment::destroy();
  return valid && remaining == 0 ? 0 : 1;
}
