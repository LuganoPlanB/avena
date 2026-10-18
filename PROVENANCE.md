<!-- Copyright (C) 2026 Plan-B Foundation -->
<!-- SPDX-License-Identifier: Apache-2.0 -->
# Source provenance

Imported from `conspire/vendor/oatpp*` in
`https://github.com/dyne/conspire`, commit
`973cb4888d4f80042a4ef9ecdf3d6269d8132a2f`.
The initial avena commit preserves all 564 tracked snapshot files byte for byte.
The upstream revisions below are recorded by Conspire's `vendor/README.md`;
they identify the snapshot bases, not a claim that Conspire made no changes.

| Directory | Upstream repository | Snapshot revision |
| --- | --- | --- |
| `src` (imported as `oatpp`) | https://github.com/oatpp/oatpp | `f83d648fd82dc222ef88aabbafb68efbd7d7bf50` |
| `src/websocket` (imported as `oatpp-websocket`) | https://github.com/oatpp/oatpp-websocket | `e5b67adfd3105627ef700ac49308565c93c491f9` |
| `src/openssl` (imported as `oatpp-openssl`) | https://github.com/oatpp/oatpp-openssl | `32c6ff8b59406470dbdff6dd65a21b671052abad` |

Each component's original `LICENSE` and per-file notices are preserved.
The root README, AGENTS.md and `.github/workflows/ci.yml` govern avena.
Obsolete upstream READMEs, historical release notes and community contribution
pages were removed; useful TLS examples are maintained in the root README.
Component CI files still describe upstream.
Git history starts at the snapshot import; upstream Git histories are not
included. Consult the repositories above for earlier history.

Initial avena maintenance adds the root build, scopes CMake package metadata,
connects extensions to the local core, gives test executables unique names,
and exports extension dependencies for installed consumers. The snapshots
omit the TLS certificate fixtures referenced by their tests; avena generates
those in the build directory for `localhost` and `virtualhost`.

Running the TLS suite exposed errors leaking between connections on an
executor thread. `src/openssl/src/oatpp-openssl/Connection.cpp` now clears
the OpenSSL error queue before TLS operations, as required by
[SSL_get_error](https://docs.openssl.org/1.1.1/man3/SSL_get_error/).
`tests/tls-error-queue.cpp` reproduces incorrect read/write failure classification
with an injected unrelated error. The async TLS test also retains a separate
failure flag so subsequent successes cannot erase an error and stall the test.
The subsequent avena packaging migration renames installed headers to
`<avena/...>`, merges the core,
extensions and TLS archives into `libavena.a`, and exports only the
`avena::avena` CMake target. Public headers are generated from the original
sources with include paths rewritten and both upstream and Plan-B notices.
The C++ namespace migration then renames definitions, qualified references,
codegen and test helpers from `oatpp` to `avena`. It intentionally breaks the
upstream C++ ABI and adds no compatibility namespace alias. Original copyright
and license notices are preserved; modified sources carry Plan-B's 2026 notice.
The upstream component CMake files are retained as implementation history;
only the root build defines the supported installation layout. Consult the
upstream repositories and imported revisions above for historical API notes;
current usage examples use `avena::` and live in the root README and tests.
