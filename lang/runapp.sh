
rm -rf build/
set -euo pipefail
cmake -S . -B build -DBUILD_TESTS=OFF
cmake --build build -j"$(nproc)"
./build/lsit_app "$@"