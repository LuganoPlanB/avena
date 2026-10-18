# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(avena-consumer LANGUAGES CXX)
enable_testing()
find_package(avena 1.4.0 CONFIG REQUIRED)
add_executable(avena-consumer main.cpp)
target_compile_features(avena-consumer PRIVATE cxx_std_17)
target_link_libraries(avena-consumer PRIVATE avena::avena)
add_test(NAME installed-consumer COMMAND avena-consumer)
set_tests_properties(installed-consumer PROPERTIES TIMEOUT 30)
