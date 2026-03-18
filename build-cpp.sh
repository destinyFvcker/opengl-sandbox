#!/bin/bash
set -e

cd "$(dirname "$0")/cpp"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
