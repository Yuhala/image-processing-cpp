#include <chrono>
#include <cstdio>
#include <omp.h>

int main()
{
    const int reps = 10000;
    volatile int sink = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int r = 0; r < reps; r++)
    {
#pragma omp parallel for schedule(static)
        for (int i = 0; i < omp_get_num_threads(); i++)
            sink = i; // trivial body: measures the cost of the region itself
    }
    auto t1 = std::chrono::steady_clock::now();
    double us = std::chrono::duration<double, std::micro>(t1 - t0).count() / reps;
    printf("threads=%d  overhead per parallel region = %.2f us\n", omp_get_max_threads(), us);
}