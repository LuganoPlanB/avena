# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
# Modified for the avena unified build.


add_executable(oatpp-websocket-tests
        oatpp-websocket/tests.cpp
        oatpp-websocket/app/AsyncController.hpp
        oatpp-websocket/app/AsyncWebSocketListener.hpp
        oatpp-websocket/app/Controller.hpp
        oatpp-websocket/app/WebSocketListener.hpp
        oatpp-websocket/FullAsyncTest.cpp
        oatpp-websocket/FullAsyncTest.hpp
        oatpp-websocket/FullTest.cpp
        oatpp-websocket/FullTest.hpp
)

set_target_properties(oatpp-websocket-tests PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(oatpp-websocket-tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)
    add_dependencies(oatpp-websocket-tests ${LIB_OATPP_EXTERNAL})
endif()

add_dependencies(oatpp-websocket-tests ${OATPP_THIS_MODULE_NAME})

target_link_oatpp(oatpp-websocket-tests)

target_link_libraries(oatpp-websocket-tests
        PRIVATE ${OATPP_THIS_MODULE_NAME}
)

## TODO link dependencies here (if some)

add_test(oatpp-websocket-tests oatpp-websocket-tests)
set_tests_properties(oatpp-websocket-tests PROPERTIES TIMEOUT 900 RUN_SERIAL TRUE)
