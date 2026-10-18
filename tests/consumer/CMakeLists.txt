# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(avena-consumer LANGUAGES CXX)
enable_testing()
# Resolve extension dependencies transitively, without a prior core lookup.
find_package(oatpp-openssl 1.4.0 REQUIRED)
find_package(oatpp-websocket 1.4.0 REQUIRED)
add_executable(avena-consumer main.cpp)
target_compile_features(avena-consumer PRIVATE cxx_std_17)
target_link_libraries(avena-consumer PRIVATE
    oatpp::oatpp-openssl oatpp::oatpp-websocket)
add_test(NAME installed-consumer COMMAND avena-consumer)
set_tests_properties(installed-consumer PROPERTIES TIMEOUT 30)
