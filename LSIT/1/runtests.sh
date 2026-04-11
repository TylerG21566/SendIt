#!/usr/bin/env bash
rm -rf build-tests/
set -euo pipefail
cmake -S . -B build_tests --preset default -DBUILD_TESTS=ON
cmake --build build_tests -j"$(nproc)"
cd build_tests && ctest --output-on-failure