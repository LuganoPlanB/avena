// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.

#include "FullTest.hpp"
#include "FullAsyncTest.hpp"
#include "FullAsyncClientTest.hpp"

#include "oatpp/Environment.hpp"

#include <iostream>
#include <csignal>

namespace {

void runTests() {

  /* ignore SIGPIPE */
  #if !(defined(WIN32) || defined(_WIN32))
    std::signal(SIGPIPE, SIG_IGN);
  #endif

  {

    avena::test::openssl::FullTest test_virtual(0, 100, false);
    test_virtual.run();

    avena::test::openssl::FullTest test_port(8443, 10, false);
    test_port.run();

  }

  {

    avena::test::openssl::FullTest test_virtual(0, 100, true);
    test_virtual.run();

    avena::test::openssl::FullTest test_port(8443, 10, true);
    test_port.run();

  }

  {

    avena::test::openssl::FullAsyncTest test_virtual(0, 100);
    test_virtual.run();

    avena::test::openssl::FullAsyncTest test_port(8443, 10);
    test_port.run();

  }

  {

    avena::test::openssl::FullAsyncClientTest test_virtual(0, 10);
    test_virtual.run(20); // - run this test 20 times.

    avena::test::openssl::FullAsyncClientTest test_port(8443, 10);
    test_port.run(1);

  }

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
