// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.

#include "oatpp-test/UnitTest.hpp"

#include "FullAsyncTest.hpp"
#include "FullTest.hpp"

#include "oatpp/concurrency/SpinLock.hpp"
#include "oatpp/Environment.hpp"

#include "oatpp-websocket/Handshaker.hpp"

#include <iostream>

namespace {

class Test : public avena::test::UnitTest {
public:
  Test() : avena::test::UnitTest("MyTag")
  {}

  void onRun() override {

    avena::websocket::Handshaker::Headers headers;
    avena::websocket::Handshaker::clientsideHandshake(headers);

  }
};

void runTests() {
  //OATPP_RUN_TEST(avena::test::websocket::FullTest);
  OATPP_RUN_TEST(avena::test::websocket::FullAsyncTest);
}

}

int main() {

  avena::Environment::init();

  runTests();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << avena::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << avena::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(avena::Environment::getObjectsCount() == 0);

  avena::Environment::destroy();

  return 0;
}
