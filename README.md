<!-- Copyright (C) 2026 Plan-B Foundation -->
<!-- SPDX-License-Identifier: Apache-2.0 -->
# avena

avena is Plan-B Foundation's fork of the oatpp C++ web framework, combining
the core, WebSocket and OpenSSL libraries in one repository and one CMake build.
It starts from Conspire's vendored 1.4.0 API snapshots; see [PROVENANCE.md](PROVENANCE.md).
The core lives under `src/`, TLS under `src/openssl/`, and WebSocket under
`src/websocket/`. The public library and C++ namespace are `avena`. This
deliberately breaks the old `oatpp` C++ ABI; update namespace references to
`avena::` and rebuild consumers.

## Build and test

Requires CMake 3.20+, Ninja or Make, and a C++17 compiler. The musl build uses
the prebuilt Dyne compiler and LibreSSL under `/opt/dyne`. Tests also need the
host `openssl` executable for certificate generation. No toolchain source is
built. On Debian/Ubuntu the host tools are `cmake ninja-build openssl`.

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE="$PWD/cmake/dyne-musl.cmake"
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

The static build creates one `libavena.a`, containing core, WebSocket, TLS,
and the complete static TLS dependency closure. Compiler and platform runtime
libraries remain the compiler's responsibility. No `liboat*`, `libssl.a` or
`libcrypto.a` is installed separately. Test support is included only when
`AVENA_BUILD_TEST_SUPPORT=ON` (defaults to BUILD_TESTING).

For a native shared build, omit the musl toolchain and use
`-DBUILD_SHARED_LIBS=ON`; this creates `libavena.so` and uses the platform's
OpenSSL development package. A native static build requires all static TLS
dependencies to be available. For a production-only build, use
`-DBUILD_TESTING=OFF -DAVENA_BUILD_TEST_SUPPORT=OFF`. Set
`-DAVENA_INSTALL=OFF` to omit installation rules. If available, ccache can be
enabled with `-DCMAKE_CXX_COMPILER_LAUNCHER=ccache`.

## Use the installed package

```cmake
find_package(avena 1.4.0 CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE avena::avena)
```

Headers use `<avena/Environment.hpp>`, `<avena/websocket/Handshaker.hpp>` and
`<avena/openssl/Config.hpp>`. The install layout is:

```text
include/avena/                 public headers and bundled TLS headers
lib/libavena.a                all static library code
lib/cmake/avena/               avenaConfig.cmake and avenaTargets.cmake
share/avena/                   licenses, notices and build information
```

The corresponding C++ names are `avena::Environment`,
`avena::websocket::Handshaker` and `avena::openssl::Config`. DTO/codegen macros
retain their existing names (for example `OATPP_CODEGEN_BEGIN`) and generate
`avena::` references.

Pass the installation prefix in `CMAKE_PREFIX_PATH`. The same target is
available when embedding avena with `add_subdirectory` (set BUILD_TESTING=OFF
unless the parent wants the inherited suites). Validate installation with:

```sh
cmake -S tests/consumer -B build/consumer -G Ninja \
  -DCMAKE_PREFIX_PATH="$PWD/install" \
  -DCMAKE_TOOLCHAIN_FILE="$PWD/cmake/dyne-musl.cmake"
cmake --build build/consumer
ctest --test-dir build/consumer --output-on-failure
```

Use a clean installation prefix when migrating from the old layout; CMake
installation does not remove files belonging to older packages. Without CMake,
the corresponding musl compile/link command is:

```sh
/opt/dyne/gcc-musl/bin/x86_64-linux-musl-g++ -std=c++17 \
  -Iinstall/include -Iinstall/include/avena/third_party \
  tests/consumer/main.cpp -Linstall/lib -lavena -pthread -ldl -latomic \
  -static -o consumer
./consumer
```

The consumer includes only public `<avena/...>` headers, exercises DTO code
generation and JSON, creates WebSocket headers and a TLS context, and links
only `avena::avena`. Static consumers do not link separate TLS archives.
The layout check rejects legacy install names and extra static archives.

GitHub Actions builds musl Debug/static and native Release/shared variants,
runs the inherited tests, and builds/runs an installed consumer. A production
build checks that libraries work without test support. This is build and
functional test coverage; no line-coverage percentage is claimed.

## Releases

The initial `1.4.0` tag identifies commit `2946e9e`, before release automation
was added. Tags use stable `MAJOR.MINOR.PATCH` names without a `v` prefix.
That historical release retains its original multi-library layout. The new
single-library layout applies to revisions after the packaging migration;
use matching historical tooling when rebuilding older tags.

Run the **Release** workflow on `main` to calculate the next version with
[`ietf-tools/semver-action`](https://github.com/ietf-tools/semver-action).
Conventional `feat:` commits increase the minor version, `fix:` commits increase
the patch version, and breaking changes increase the major version. Other
changes fall back to a patch release. The workflow builds and validates the
package before creating the new tag and GitHub release. To build an existing
tag, supply its name in the workflow's optional `tag` input. Pushing a new tag
that contains the workflow also starts a release build.

[`dyne/musl-action`](https://github.com/dyne/musl-action) supplies the x86-64
musl compiler and its bundled LibreSSL 4.1.0 headers and static libraries.
`scripts/build-musl-release.sh` uses only this Dyne toolchain for target code;
it builds all three avena libraries and runs a fully static installed consumer.
CMake dependency lookup is restricted to the package and musl compiler roots.
Release archives contain `<avena/...>` headers, one `libavena.a` (including
LibreSSL), the avena CMake package, licenses and build information, plus a SHA-256
checksum. They target x86-64
Linux with musl; the regular CI suites continue to test the native builds.
The installed avena CMake package identifies the 1.4.0 API line.

To reproduce a release after installing the Dyne toolchain:

```sh
CC=/opt/dyne/gcc-musl/bin/x86_64-linux-musl-gcc \
CXX=/opt/dyne/gcc-musl/bin/x86_64-linux-musl-g++ \
bash scripts/build-musl-release.sh "$PWD" 1.4.0 "$PWD/build/release"
```

## License and maintenance

Apache License 2.0, with original licenses and copyright notices preserved.
Plan-B changes carry `Copyright (C) 2026 Plan-B Foundation`.
See [LICENSE](LICENSE), [NOTICE](NOTICE), and [AGENTS.md](AGENTS.md).
