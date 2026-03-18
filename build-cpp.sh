#!/bin/bash
set -e

cd "$(dirname "$0")/cpp"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

if [ -n "$1" ]; then
    cmake --build build --target "$1"
else
    cmake --build build
fi
