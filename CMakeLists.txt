# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(avena VERSION 1.4.0 LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Use an out-of-source build: cmake -S . -B build")
endif()

include(CTest)
include(GNUInstallDirs)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(OATPP_INSTALL "Install libraries, headers and CMake packages" ON)
option(OATPP_LINK_TEST_LIBRARY "Build the oatpp test support library" ON)
# The root BUILD_TESTING switch controls all three inherited test suites.
set(OATPP_BUILD_TESTS ${BUILD_TESTING})
if(BUILD_TESTING AND NOT OATPP_LINK_TEST_LIBRARY)
    message(FATAL_ERROR "BUILD_TESTING requires OATPP_LINK_TEST_LIBRARY=ON")
endif()

add_subdirectory(oatpp)
add_library(oatpp::oatpp ALIAS oatpp)
if(TARGET oatpp-test)
    add_library(oatpp::oatpp-test ALIAS oatpp-test)
endif()
add_subdirectory(oatpp-websocket)
add_library(oatpp::oatpp-websocket ALIAS oatpp-websocket)
add_subdirectory(oatpp-openssl)
add_library(oatpp::oatpp-openssl ALIAS oatpp-openssl)

if(BUILD_TESTING)
    add_executable(avena-smoke tests/consumer/main.cpp)
    target_compile_features(avena-smoke PRIVATE cxx_std_17)
    target_link_libraries(avena-smoke PRIVATE
        oatpp::oatpp-openssl oatpp::oatpp-websocket)
    add_test(NAME avena-smoke COMMAND avena-smoke)
    set_tests_properties(avena-smoke PROPERTIES TIMEOUT 30)
    add_executable(avena-tls-error-queue tests/tls-error-queue.cpp)
    target_compile_features(avena-tls-error-queue PRIVATE cxx_std_17)
    target_link_libraries(avena-tls-error-queue PRIVATE oatpp::oatpp-openssl)
    target_compile_definitions(avena-tls-error-queue PRIVATE
        "$<TARGET_PROPERTY:oatpp-openssl-tests,COMPILE_DEFINITIONS>")
    add_dependencies(avena-tls-error-queue avena-tls-fixtures)
    add_test(NAME avena-tls-error-queue COMMAND avena-tls-error-queue)
    set_tests_properties(avena-tls-error-queue PROPERTIES TIMEOUT 30)
endif()

if(OATPP_INSTALL)
    install(FILES LICENSE NOTICE README.md PROVENANCE.md
        DESTINATION ${CMAKE_INSTALL_DATADIR}/avena)
endif()
