# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
execute_process(COMMAND "${nm}" --defined-only --extern-only --demangle "${library}"
    OUTPUT_VARIABLE symbols ERROR_VARIABLE diagnostics RESULT_VARIABLE result)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Unable to inspect namespace symbols: ${diagnostics}")
endif()
if(symbols MATCHES "(^|[^A-Za-z0-9_])oatpp::")
    message(FATAL_ERROR "The library still defines oatpp:: symbols")
endif()
foreach(symbol IN ITEMS "avena::Environment::init" "avena::websocket::Handshaker::clientsideHandshake"
        "avena::openssl::Config::createDefaultClientConfigShared")
    string(FIND "${symbols}" "${symbol}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "Missing expected namespace symbol: ${symbol}")
    endif()
endforeach()
message(STATUS "Verified avena namespace symbols across core, WebSocket and TLS")
