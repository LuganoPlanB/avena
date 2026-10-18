# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
function(avena_prepare_headers)
    set(output "${CMAKE_CURRENT_BINARY_DIR}/include")
    set(mappings "src/src/oatpp|avena"
        "src/websocket/src/oatpp-websocket|avena/websocket"
        "src/openssl/src/oatpp-openssl|avena/openssl")
    if(AVENA_BUILD_TEST_SUPPORT)
        list(APPEND mappings "src/src/oatpp-test|avena/test")
    else()
        file(REMOVE_RECURSE "${output}/avena/test")
    endif()
    foreach(mapping IN LISTS mappings)
        string(REPLACE "|" ";" mapping "${mapping}")
        list(GET mapping 0 source)
        list(GET mapping 1 destination)
        file(GLOB_RECURSE headers CONFIGURE_DEPENDS RELATIVE "${PROJECT_SOURCE_DIR}/${source}"
            "${PROJECT_SOURCE_DIR}/${source}/*.hpp")
        foreach(header IN LISTS headers)
            set(input "${PROJECT_SOURCE_DIR}/${source}/${header}")
            set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${input}")
            file(READ "${input}" content)
            # Includes quoted codegen macro paths and retains upstream notices.
            foreach(delimiter IN ITEMS "\"" "<")
                string(REPLACE "${delimiter}oatpp/" "${delimiter}avena/" content "${content}")
                string(REPLACE "${delimiter}oatpp-websocket/" "${delimiter}avena/websocket/" content "${content}")
                string(REPLACE "${delimiter}oatpp-openssl/" "${delimiter}avena/openssl/" content "${content}")
                string(REPLACE "${delimiter}oatpp-test/" "${delimiter}avena/test/" content "${content}")
            endforeach()
            if(NOT content MATCHES "Copyright \\(C\\) 2026 Plan-B Foundation")
                string(PREPEND content "// Copyright (C) 2026 Plan-B Foundation\n")
            endif()
            file(GENERATE OUTPUT "${output}/${destination}/${header}" CONTENT
                "// Modified public include paths for avena.\n${content}")
        endforeach()
    endforeach()
    file(COPY "${OPENSSL_INCLUDE_DIR}/openssl" DESTINATION "${output}/avena/third_party")
    # Multiarch OpenSSL keeps generated configuration headers in a second path.
    find_path(AVENA_TLS_CONFIG_INCLUDE NAMES openssl/opensslconf.h
        HINTS "${OPENSSL_INCLUDE_DIR}" "${OPENSSL_INCLUDE_DIR}/${CMAKE_LIBRARY_ARCHITECTURE}")
    if(AVENA_TLS_CONFIG_INCLUDE AND NOT AVENA_TLS_CONFIG_INCLUDE STREQUAL OPENSSL_INCLUDE_DIR)
        file(COPY "${AVENA_TLS_CONFIG_INCLUDE}/openssl/" DESTINATION "${output}/avena/third_party/openssl")
    endif()
    set(AVENA_PUBLIC_INCLUDE_DIR "${output}" PARENT_SCOPE)
endfunction()
