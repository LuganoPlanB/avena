# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
# Modified for the avena unified build.

function(target_link_oatpp target)
    if(TARGET oatpp::oatpp)
        target_link_libraries(${target} PUBLIC oatpp::oatpp)
        set(test_library oatpp::oatpp-test)
    else()
        target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${OATPP_DIR_SRC}>)
        target_link_libraries(${target} PUBLIC oatpp)
        set(test_library oatpp-test)
    endif()
    get_target_property(target_type ${target} TYPE)
    if(target_type STREQUAL "EXECUTABLE" AND OATPP_BUILD_TESTS)
        target_link_libraries(${target} PRIVATE ${test_library})
    endif()
endfunction()
