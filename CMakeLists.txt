# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(avena VERSION 1.4.0 LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Use an out-of-source build: cmake -S . -B build")
endif()
include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
option(BUILD_SHARED_LIBS "Build libavena as a shared library" OFF)
option(AVENA_INSTALL "Install the avena library, headers and CMake package" ON)
option(AVENA_BUILD_TEST_SUPPORT "Include test support in libavena" ${BUILD_TESTING})
if(BUILD_TESTING AND NOT AVENA_BUILD_TEST_SUPPORT)
    message(FATAL_ERROR "BUILD_TESTING requires AVENA_BUILD_TEST_SUPPORT=ON")
endif()
if(NOT BUILD_SHARED_LIBS)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
    set(ZLIB_USE_STATIC_LIBS TRUE)
endif()

# Compile the inherited modules as objects, with no separate archive or install.
find_package(OpenSSL 1.1 REQUIRED)
set(OATPP_INSTALL OFF)
set(OATPP_BUILD_TESTS ${BUILD_TESTING})
set(OATPP_LINK_TEST_LIBRARY ${AVENA_BUILD_TEST_SUPPORT})
set(AVENA_COMPONENT_LIBRARY_TYPE OBJECT)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(src)
add_library(oatpp::oatpp ALIAS oatpp)
if(TARGET oatpp-test)
    add_library(oatpp::oatpp-test ALIAS oatpp-test)
endif()
add_subdirectory(src/websocket)
add_subdirectory(src/openssl)
find_package(Threads REQUIRED)

add_library(avena
    $<TARGET_OBJECTS:oatpp>
    $<TARGET_OBJECTS:oatpp-websocket>
    $<TARGET_OBJECTS:oatpp-openssl>)
add_library(avena::avena ALIAS avena)
if(AVENA_BUILD_TEST_SUPPORT)
    target_sources(avena PRIVATE $<TARGET_OBJECTS:oatpp-test>)
endif()
target_compile_features(avena PUBLIC cxx_std_17)
target_link_libraries(avena PUBLIC Threads::Threads)
if(WIN32)
    target_link_libraries(avena PUBLIC wsock32 ws2_32)
elseif(OATPP_LINK_ATOMIC AND NOT APPLE AND NOT CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    target_link_libraries(avena PUBLIC atomic)
endif()
set_target_properties(avena PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)

include(cmake/avena-headers.cmake)
avena_prepare_headers()
target_include_directories(avena PUBLIC
    $<BUILD_INTERFACE:${AVENA_PUBLIC_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${AVENA_PUBLIC_INCLUDE_DIR}/avena/third_party>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/avena/third_party>)
if(BUILD_SHARED_LIBS)
    target_link_libraries(avena PUBLIC OpenSSL::SSL OpenSSL::Crypto)
else()
    include(cmake/avena-static.cmake)
    avena_bundle_tls(avena)
endif()

if(BUILD_TESTING)
    add_executable(avena-smoke tests/consumer/main.cpp)
    target_link_libraries(avena-smoke PRIVATE avena::avena)
    add_test(NAME avena-smoke COMMAND avena-smoke)
    set_tests_properties(avena-smoke PROPERTIES TIMEOUT 30)
    add_executable(avena-tls-error-queue tests/tls-error-queue.cpp)
    target_link_libraries(avena-tls-error-queue PRIVATE avena::avena)
    target_compile_definitions(avena-tls-error-queue PRIVATE
        "$<TARGET_PROPERTY:oatpp-openssl-tests,COMPILE_DEFINITIONS>")
    add_dependencies(avena-tls-error-queue avena-tls-fixtures)
    add_test(NAME avena-tls-error-queue COMMAND avena-tls-error-queue)
    set_tests_properties(avena-tls-error-queue PROPERTIES TIMEOUT 30)
endif()

if(AVENA_INSTALL)
    install(TARGETS avena EXPORT avenaTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(DIRECTORY "${AVENA_PUBLIC_INCLUDE_DIR}/avena"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(EXPORT avenaTargets NAMESPACE avena::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/avena)
    configure_package_config_file(cmake/avenaConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/avenaConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/avena)
    write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/avenaConfigVersion.cmake"
        VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/avenaConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/avenaConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/avena)
    install(FILES LICENSE NOTICE README.md PROVENANCE.md
        DESTINATION ${CMAKE_INSTALL_DATADIR}/avena)
endif()
