#!/usr/bin/env bash
# Build and install OpenCASCADE (OCCT) 8.0.1 into a prefix.
# Shared by ci/Dockerfile (Arch container) and the macOS CI job so both
# build the same version developers use locally.
#
# Usage: ci/install-occt.sh [install-prefix]
#   Default prefix: /opt/occt
set -euo pipefail

OCCT_VERSION="8.0.1"
OCCT_TAG="V8_0_1"
OCCT_SHA256="0d6913eae4bcc09a3653ceced6dda1aec11c35a1513d4c06762c9b002092c68a"
PREFIX="${1:-/opt/occt}"
WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT

cd "$WORKDIR"
curl -fsSL "https://github.com/Open-Cascade-SAS/OCCT/archive/refs/tags/${OCCT_TAG}.tar.gz" -o occt.tar.gz
if command -v sha256sum >/dev/null 2>&1; then
    echo "${OCCT_SHA256}  occt.tar.gz" | sha256sum -c -
else
    echo "${OCCT_SHA256}  occt.tar.gz" | shasum -a 256 -c -
fi
tar xzf occt.tar.gz
OCCT_SRC="$(find . -maxdepth 1 -type d -name 'OCCT-*' | head -1)"

ARGS=(
    -G Ninja
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX="$PREFIX"
    -DCMAKE_POLICY_VERSION_MINIMUM=3.10
    -DBUILD_RELEASE_DISABLE_EXCEPTIONS=OFF
    -DBUILD_DOC_Overview=OFF
    -DINSTALL_DOC_Overview=OFF
    -DBUILD_MODULE_Draw=OFF
    -DUSE_FREEIMAGE=OFF
    -DUSE_FREETYPE=ON
    -DUSE_RAPIDJSON=ON
    -DUSE_TBB=ON
)

# On macOS the third-party libs come from Homebrew; pass explicit hints so
# OCCT's cmake doesn't have to guess /opt/homebrew.
if command -v brew >/dev/null 2>&1; then
    ARGS+=(
        -D3RDPARTY_FREETYPE_DIR="$(brew --prefix freetype)"
        -D3RDPARTY_RAPIDJSON_DIR="$(brew --prefix rapidjson)"
        -D3RDPARTY_RAPIDJSON_INCLUDE_DIR="$(brew --prefix rapidjson)/include"
        -D3RDPARTY_TBB_DIR="$(brew --prefix tbb)"
    )
fi

cmake -S "$OCCT_SRC" -B build "${ARGS[@]}"
cmake --build build --parallel
cmake --install build
