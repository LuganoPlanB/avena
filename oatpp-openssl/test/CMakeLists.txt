# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
# Modified for the avena unified build.

find_package(Threads)

add_executable(oatpp-openssl-tests
        oatpp-openssl/tests.cpp
        oatpp-openssl/FullTest.cpp
        oatpp-openssl/FullTest.hpp
        oatpp-openssl/FullAsyncTest.cpp
        oatpp-openssl/FullAsyncTest.hpp
        oatpp-openssl/FullAsyncClientTest.cpp
        oatpp-openssl/FullAsyncClientTest.hpp
        oatpp-openssl/app/Controller.hpp
        oatpp-openssl/app/AsyncController.hpp
        oatpp-openssl/app/Client.hpp
        oatpp-openssl/app/DTOs.hpp
)

#################################################################
## Generate local test fixtures: the imported snapshot did not contain them.

find_program(AVENA_OPENSSL_EXECUTABLE NAMES openssl REQUIRED)
set(cert_dir "${CMAKE_CURRENT_BINARY_DIR}/cert")
add_custom_command(
        OUTPUT "${cert_dir}/test_key.pem" "${cert_dir}/test_cert.crt"
               "${cert_dir}/test_dhparams.pem"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${cert_dir}"
        COMMAND "${AVENA_OPENSSL_EXECUTABLE}" req -x509 -newkey rsa:2048
                -nodes -sha256 -days 3650
                -config "${CMAKE_CURRENT_SOURCE_DIR}/certificate.cnf"
                -keyout "${cert_dir}/test_key.pem" -out "${cert_dir}/test_cert.crt"
        COMMAND "${AVENA_OPENSSL_EXECUTABLE}" genpkey -genparam -algorithm DH
                -pkeyopt group:ffdhe2048 -out "${cert_dir}/test_dhparams.pem"
        COMMENT "Generating test-only TLS fixtures"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/certificate.cnf"
        VERBATIM
)
add_custom_target(avena-tls-fixtures DEPENDS
        "${cert_dir}/test_key.pem" "${cert_dir}/test_cert.crt"
        "${cert_dir}/test_dhparams.pem")
add_dependencies(oatpp-openssl-tests avena-tls-fixtures)
target_compile_definitions(oatpp-openssl-tests PRIVATE
        CERT_PEM_PATH="${cert_dir}/test_key.pem"
        CERT_CRT_PATH="${cert_dir}/test_cert.crt"
        CERT_DH_PARAMS_PATH="${cert_dir}/test_dhparams.pem"
)

#################################################################

set_target_properties(oatpp-openssl-tests PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(oatpp-openssl-tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)
    add_dependencies(oatpp-openssl-tests ${LIB_OATPP_EXTERNAL})
endif()

add_dependencies(oatpp-openssl-tests ${OATPP_THIS_MODULE_NAME})

target_link_oatpp(oatpp-openssl-tests)

target_link_libraries(oatpp-openssl-tests
        PRIVATE ${OATPP_THIS_MODULE_NAME}
        PRIVATE Threads::Threads
)

add_test(oatpp-openssl-tests oatpp-openssl-tests)
set_tests_properties(oatpp-openssl-tests PROPERTIES TIMEOUT 900 RUN_SERIAL TRUE)
