// Copyright (C) 2026 Plan-B Foundation
// SPDX-License-Identifier: Apache-2.0
#include "oatpp/Environment.hpp"
#include "oatpp-openssl/Config.hpp"
#include "oatpp-websocket/Handshaker.hpp"

int main() {
  oatpp::Environment::init();
  bool valid = false;
  {
    auto config = oatpp::openssl::Config::createDefaultClientConfigShared();
    oatpp::websocket::Handshaker::Headers headers;
    oatpp::websocket::Handshaker::clientsideHandshake(headers);
    valid = config != nullptr && headers.get("Upgrade") == "websocket";
  }
  const auto remaining = oatpp::Environment::getObjectsCount();
  oatpp::Environment::destroy();
  return valid && remaining == 0 ? 0 : 1;
}
