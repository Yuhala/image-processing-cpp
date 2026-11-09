#!/bin/bash
# build.sh - quick build of all three versions without CMake.
#   app_baseline  original sequential code
#   app_bestseq   optimised sequential code
#   app_parallel  OpenMP version
# Works on macOS (Apple clang + Homebrew libomp) and Linux (g++ or clang++ with OpenMP).
set -e
cd "$(dirname "$0")/.."

FLAGS="-O2 -std=c++17 -w"

if [ "$(uname)" = "Darwin" ]; then
  CXX=${CXX:-clang++}
  if ! command -v brew >/dev/null 2>&1 || ! brew --prefix libomp >/dev/null 2>&1; then
    echo "OpenMP runtime not found. Install it with:  brew install libomp"
    exit 1
  fi
  OMP=$(brew --prefix libomp)
  OMPFLAGS="-Xpreprocessor -fopenmp -I$OMP/include -L$OMP/lib -lomp"
else
  CXX=${CXX:-g++}
  OMPFLAGS="-fopenmp"
fi

echo "Using $CXX"
echo "Building baseline ..."
$CXX $FLAGS baseline/main.cpp baseline/Image.cpp -o app_baseline

echo "Building best sequential ..."
$CXX $FLAGS bestseq/main.cpp bestseq/Image.cpp -o app_bestseq

echo "Building OpenMP version ..."
$CXX $FLAGS $OMPFLAGS parallel/main.cpp parallel/Image.cpp -o app_parallel

echo "done:  app_baseline  app_bestseq  app_parallel"
