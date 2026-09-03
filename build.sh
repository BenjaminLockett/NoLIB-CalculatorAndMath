#!/usr/bin/env sh
set -eu
cd "$(dirname "$0")"
cmake -S . -B build/cmake
cmake --build build/cmake --config Release
ctest --test-dir build/cmake -C Release --output-on-failure
