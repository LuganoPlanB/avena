// Copyright (C) 2026 Plan-B Foundation
// Modified C++ namespace from oatpp to avena.
//
// Created by Leonid  on 2019-03-25.
//

#ifndef oatpp_test_websocket_FullAsyncTest_hpp
#define oatpp_test_websocket_FullAsyncTest_hpp

#include "oatpp-test/UnitTest.hpp"

namespace avena { namespace test { namespace websocket {

class FullAsyncTest : public avena::test::UnitTest {
public:

  FullAsyncTest() : UnitTest("TEST[FullAsyncTest]") {}

  void onRun() override;

};

}}}


#endif // oatpp_test_websocket_FullAsyncTest_hpp
