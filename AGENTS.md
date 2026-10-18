<!-- Copyright (C) 2026 Plan-B Foundation -->
<!-- SPDX-License-Identifier: Apache-2.0 -->
# Maintaining avena

avena is Plan-B Foundation's Apache-2.0 fork of oatpp core, WebSocket and
OpenSSL. Read README.md and PROVENANCE.md before changing the build or imports.

- Keep upstream licenses, copyright headers and attribution intact. Add
  `Copyright (C) 2026 Plan-B Foundation` to every source, build, test or
  documentation file modified by the foundation, using the file's comment
  syntax. Mark modifications and use SPDX-License-Identifier: Apache-2.0 for
  new files. Extend the year range for later-year contributions; retain 2026.
  Do not rewrite untouched upstream files solely to claim copyright.
- Use the root CMake entry point. Keep builds out of source. Never commit
  generated files, local credentials, build trees or install prefixes.
- Preserve `oatpp` C++ namespaces, include paths, library names and installed
  CMake targets unless an explicitly planned compatibility break requires it.
- Keep module metadata directory-scoped. Extensions must use the core target
  already in the build and must not download another copy of oatpp.
- Make focused changes, with a regression test for changed behavior. Preserve
  test assertions; do not disable failing tests to make CI pass. TLS fixtures
  are generated under the build directory and are never production credentials.
- Validate the root configure/build/CTest sequence and installed consumer in
  README.md. Build static and shared variants for build-system changes; also
  test BUILD_TESTING=OFF and OATPP_LINK_TEST_LIBRARY=OFF. Report exact failures
  and any checks not run. The integration suites bind loopback ports and must
  run serially with bounded timeouts.
- CI starts with compilation, runs the inherited tests, and checks an installed
  consumer. Keep new checks reproducible locally. Do not describe these checks
  as measured line coverage without collecting a coverage report.
- Record upstream refreshes in PROVENANCE.md, including repository, revision,
  local patches and validation. Keep import and maintenance commits separate.
