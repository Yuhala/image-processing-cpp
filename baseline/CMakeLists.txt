cmake_minimum_required(VERSION 3.16)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -O2 -g: optimised, but keeps debug info for profiling
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# help finding Homebrew's libomp
if(APPLE)
  execute_process(
    COMMAND brew --prefix libomp
    OUTPUT_VARIABLE LIBOMP_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )
  if(NOT LIBOMP_PREFIX)
    set(LIBOMP_PREFIX /opt/homebrew/opt/libomp)  # fallback
  endif()
  message(STATUS "Using libomp at: ${LIBOMP_PREFIX}")

  set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
  set(OpenMP_CXX_LIB_NAMES "omp")
  set(OpenMP_omp_LIBRARY "${LIBOMP_PREFIX}/lib/libomp.dylib")
  set(OpenMP_CXX_INCLUDE_DIR "${LIBOMP_PREFIX}/include")
endif()

find_package(OpenMP REQUIRED)

add_executable(baseline main.cpp Image.cpp)

if(APPLE)
  target_include_directories(baseline PRIVATE "${LIBOMP_PREFIX}/include")
endif()

target_link_libraries(baseline PRIVATE OpenMP::OpenMP_CXX)