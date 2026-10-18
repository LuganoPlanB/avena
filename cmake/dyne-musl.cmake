# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
# Use prebuilt Dyne binaries and libraries; never build the toolchain sources.
set(CMAKE_C_COMPILER /opt/dyne/gcc-musl/bin/x86_64-linux-musl-gcc)
set(CMAKE_CXX_COMPILER /opt/dyne/gcc-musl/bin/x86_64-linux-musl-g++)
set(CMAKE_EXE_LINKER_FLAGS_INIT -static)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_PREFIX_PATH}
    /opt/dyne/x86_64-linux-musl /opt/dyne/gcc-musl/x86_64-linux-musl)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_DISABLE_FIND_PACKAGE_PkgConfig ON)
set(OPENSSL_ROOT_DIR /opt/dyne/x86_64-linux-musl)
set(OPENSSL_USE_STATIC_LIBS ON)
