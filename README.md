<!-- Copyright (C) 2026 Plan-B Foundation -->
<!-- SPDX-License-Identifier: Apache-2.0 -->
# avena

avena is Plan-B Foundation's fork of the oatpp C++ web framework, combining
the core, WebSocket and OpenSSL libraries in one repository and one CMake build.
It starts from Conspire's vendored 1.4.0 API snapshots; see [PROVENANCE.md](PROVENANCE.md).
Existing `oatpp` namespaces, headers, library names and CMake package names
are retained for compatibility.

## Build and test

Requires CMake 3.20+, a C++17 compiler, OpenSSL development headers/libraries
(1.1 or later), and Ninja or Make. Tests also need the `openssl` executable.
On Debian/Ubuntu the packages are `cmake ninja-build g++ libssl-dev openssl`.

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
cmake --install build --prefix "$PWD/install"
```

The root build needs no source downloads or preinstalled oatpp. A quick
TLS-configuration/WebSocket-handshake smoke test, a TLS error-queue regression,
and all three inherited
CTest suites run by default, including HTTP, WebSocket and TLS
integration tests. They use loopback sockets, run serially, and have 15-minute
timeouts. A single CTest entry can contain many upstream unit cases. CMake
generates test-only TLS certificates and DH parameters in the build directory;
these are neither committed nor installed.

Use `-DBUILD_SHARED_LIBS=ON` for shared libraries. For a production-only build,
use `-DBUILD_TESTING=OFF -DOATPP_LINK_TEST_LIBRARY=OFF`. Set
`-DOATPP_INSTALL=OFF` to omit installation rules. If available, ccache can be
enabled with `-DCMAKE_CXX_COMPILER_LAUNCHER=ccache`.

## Use the installed packages

```cmake
find_package(oatpp 1.4.0 REQUIRED)
find_package(oatpp-websocket 1.4.0 REQUIRED)
find_package(oatpp-openssl 1.4.0 REQUIRED)
target_link_libraries(my_app PRIVATE
    oatpp::oatpp oatpp::oatpp-websocket oatpp::oatpp-openssl)
```

Pass the installation prefix in `CMAKE_PREFIX_PATH`. The same targets are
available when embedding avena with `add_subdirectory` (set BUILD_TESTING=OFF
unless the parent wants the inherited suites). Validate installation with:

```sh
cmake -S tests/consumer -B build/consumer -G Ninja \
  -DCMAKE_PREFIX_PATH="$PWD/install"
cmake --build build/consumer
ctest --test-dir build/consumer --output-on-failure
```

GitHub Actions builds Debug/static and Release/shared variants, runs the
inherited tests, and builds/runs an installed consumer. A separate production
build checks that libraries work without test support. This is build and
functional test coverage; no line-coverage percentage is claimed.

## License and maintenance

Apache License 2.0, with original licenses and copyright notices preserved.
Plan-B changes carry `Copyright (C) 2026 Plan-B Foundation`.
See [LICENSE](LICENSE), [NOTICE](NOTICE), and [AGENTS.md](AGENTS.md).
