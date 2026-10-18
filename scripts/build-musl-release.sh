#!/usr/bin/env bash
# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
set -Eeuo pipefail

if [[ $# != 3 ]]; then
  printf 'Usage: %s SOURCE_DIR VERSION OUTPUT_DIR\n' "$0" >&2
  exit 2
fi
source_dir="$(realpath "$1")"
version="$2"
output_dir="$(realpath -m "$3")"
[[ "$version" =~ ^(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)$ ]] || { printf 'Invalid release version\n' >&2; exit 2; }
: "${CC:?Set CC to the Dyne musl C compiler}"
: "${CXX:?Set CXX to the Dyne musl C++ compiler}"
[[ "$("$CC" -dumpmachine)" == x86_64-linux-musl ]] || { printf 'Expected x86_64-linux-musl compiler\n' >&2; exit 2; }
export CC CXX
toolchain_dir="$(realpath "$(dirname "$CC")/../..")"
ssl_root="$toolchain_dir/x86_64-linux-musl"
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
grep -q 'LIBRESSL_VERSION_TEXT.*"LibreSSL 4.1.0"' "$ssl_root/include/openssl/opensslv.h" || {
  printf 'Review the bundled LibreSSL license before using a new toolchain version\n' >&2
  exit 2
}
work_dir="$(mktemp -d "${TMPDIR:-/tmp}/avena-musl.XXXXXX")"
trap 'rm -rf -- "$work_dir"' EXIT
package="avena-$version-linux-x86_64-musl"
prefix="$work_dir/$package"
mkdir -p "$output_dir" "$prefix"

# Use only the TLS headers and static archives shipped by Dyne.
mkdir -p "$prefix/include" "$prefix/lib"
cp -a "$ssl_root/include/openssl" "$prefix/include/"
cp "$ssl_root/lib/libssl.a" "$ssl_root/lib/libcrypto.a" "$prefix/lib/"
common_cmake_args=(
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER="$CXX"
  -DCMAKE_FIND_ROOT_PATH="$prefix;$toolchain_dir/gcc-musl/x86_64-linux-musl"
  -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
  -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
  -DCMAKE_DISABLE_FIND_PACKAGE_PkgConfig=ON
  -DCMAKE_EXE_LINKER_FLAGS=-static -DCMAKE_PREFIX_PATH="$prefix"
  -DOPENSSL_USE_STATIC_LIBS=ON -DOPENSSL_ROOT_DIR="$prefix"
  -DOPENSSL_INCLUDE_DIR="$prefix/include"
  -DOPENSSL_SSL_LIBRARY="$prefix/lib/libssl.a"
  -DOPENSSL_CRYPTO_LIBRARY="$prefix/lib/libcrypto.a"
)

cmake -S "$source_dir" -B "$work_dir/build" -G Ninja \
  "${common_cmake_args[@]}" \
  -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_INSTALL_LIBDIR=lib \
  -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF -DOATPP_LINK_TEST_LIBRARY=OFF
cmake --build "$work_dir/build" --parallel 2
cmake --install "$work_dir/build"

cmake -S "$source_dir/tests/consumer" -B "$work_dir/consumer" -G Ninja \
  "${common_cmake_args[@]}"
cmake --build "$work_dir/consumer" --parallel 2
ctest --test-dir "$work_dir/consumer" --output-on-failure --no-tests=error
readelf -l "$work_dir/consumer/avena-consumer" > "$work_dir/program-headers"
readelf -d "$work_dir/consumer/avena-consumer" > "$work_dir/dynamic-section"
if grep -q INTERP "$work_dir/program-headers" || grep -q NEEDED "$work_dir/dynamic-section"; then
  printf 'Consumer executable has dynamic dependencies\n' >&2
  exit 1
fi
printf 'Verified static musl consumer: no INTERP or NEEDED entries\n'
install -Dm644 "$script_dir/../licenses/LibreSSL-COPYING" \
  "$prefix/share/avena/third-party/LibreSSL-COPYING"
{
  printf 'avena release: %s\nsource commit: %s\nTLS: LibreSSL 4.1.0 (from Dyne musl)\n' \
    "$version" "$(git -C "$source_dir" rev-parse HEAD)"
  "$CXX" --version
} > "$prefix/share/avena/BUILD-INFO.txt"
tar -czf "$output_dir/$package.tar.gz" -C "$work_dir" "$package"
(
  cd "$output_dir"
  sha256sum "$package.tar.gz" > "$package.tar.gz.sha256"
)
