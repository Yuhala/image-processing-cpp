#!/bin/bash
set -e
cd "$(dirname "$0")/.."
CXX=${CXX:-clang++}
if ! brew --prefix libomp >/dev/null 2>&1; then echo "Install OpenMP first:  brew install libomp"; exit 1; fi
OMP=$(brew --prefix libomp)

echo "Building baseline ..."
$CXX -O2 -std=c++11 -w baseline/main.cpp baseline/Image.cpp -o app_baseline

echo "Building OpenMP version ..."
$CXX -O2 -std=c++11 -w -Xpreprocessor -fopenmp -I"$OMP/include" -L"$OMP/lib" -lomp \
     parallel/main.cpp parallel/Image.cpp -o app_parallel
echo "done:  app_baseline  app_parallel"
