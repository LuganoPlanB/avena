// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.
//
// Created by Leonid  on 2019-03-25.
//

#ifndef oatpp_test_websocket_FullTest_hpp
#define oatpp_test_websocket_FullTest_hpp

#include "oatpp-test/UnitTest.hpp"

namespace avena { namespace test { namespace websocket {

class FullTest : public avena::test::UnitTest {
public:

  FullTest() : UnitTest("TEST[FullTest]") {}

  void onRun() override;

};

}}}


#endif // oatpp_test_websocket_FullAsyncTest_hpp
