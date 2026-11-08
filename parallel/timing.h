#ifndef TIMING_H
#define TIMING_H
#include <chrono>
#include <cstdio>
#include <cstdlib>
#ifdef _OPENMP
#include <omp.h>
#endif

inline double now_ms()
{
    return std::chrono::duration<double, std::milli>(
               std::chrono::steady_clock::now().time_since_epoch()).count();
}

static double g_read_ms = 0.0; // time spent inside readImage

// Prints one line to stderr only when IMG_TIMING is set. 
inline void report_timing(double t0, double t1, double t2)
{
    if (!getenv("IMG_TIMING")) return;
    int threads = 1;
#ifdef _OPENMP
    threads = omp_get_max_threads();
#endif
    double op = t1 - t0; // read + compute
    fprintf(stderr, "TIMING threads=%d read_ms=%.3f compute_ms=%.3f write_ms=%.3f total_ms=%.3f\n",
            threads, g_read_ms, op - g_read_ms, t2 - t1, t2 - t0);
}
#endif