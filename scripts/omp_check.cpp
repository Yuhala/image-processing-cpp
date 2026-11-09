// omp_check.cpp - confirms OpenMP is compiled in, linked, and actually using multiple cores.
//
// build (from the repo root):
//   clang++ -O2 -std=c++11 -Xpreprocessor -fopenmp -I$(brew --prefix libomp)/include \
//     -L$(brew --prefix libomp)/lib -lomp scripts/omp_check.cpp -o omp_check
// run:
//   ./omp_check
//
// What "working" looks like:
//   - "_OPENMP defined" with a version date (e.g. 201811 = OpenMP 5.0)
//   - num_procs = 10 on an M2 Pro
//   - one "hello from thread" line per thread
//   - the timed loop gets clearly faster as threads increase (about 5-8x at 10 threads)
#include <chrono>
#include <cmath>
#include <cstdio>
#include <vector>
#ifdef _OPENMP
#include <omp.h>
#endif

int main()
{
#ifndef _OPENMP
    printf("FAIL: _OPENMP is NOT defined - this was compiled without OpenMP, pragmas are ignored.\n");
    return 1;
#else
    printf("OK: _OPENMP defined, version %d\n", _OPENMP);
    printf("omp_get_num_procs()   = %d   (cores OpenMP can see)\n", omp_get_num_procs());
    printf("omp_get_max_threads() = %d   (threads a parallel region will use; set by OMP_NUM_THREADS)\n",
           omp_get_max_threads());

#pragma omp parallel
    {
#pragma omp critical
        printf("  hello from thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
    }

    // A compute-heavy loop with no shared writes: should scale well with threads.
    const int n = 20000000;
    std::vector<double> out(n);
    int counts[] = {1, 2, 4, 6, 8, 10};
    double t1 = 0;
    printf("\nthreads   time(ms)   speedup\n");
    for (int t : counts)
    {
        omp_set_num_threads(t);
        auto start = std::chrono::steady_clock::now();
#pragma omp parallel for schedule(static)
        for (int i = 0; i < n; i++)
            out[i] = std::sin(i * 0.001) * std::cos(i * 0.002) + std::sqrt((double)i);
        double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
        if (t == 1) t1 = ms;
        printf("%7d   %8.1f   %6.2fx\n", t, ms, t1 / ms);
    }
    printf("(checksum %.3f - ignore; stops the compiler deleting the loop)\n", out[n / 2]);
    return 0;
#endif
}