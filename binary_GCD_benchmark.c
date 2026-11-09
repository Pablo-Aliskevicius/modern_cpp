#include <stdio.h>
#include <time.h>
#include <stdint.h>

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

/*
gcc -O3 binary_GCD_benchmark.c -o ./out/gcd_benchmark
./out/gcd_benchmark

*/
// 1. Standard Euclidean Algorithm (Iterative)
uint64_t euclidean_gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 2. Binary GCD Algorithm (Stein's Algorithm)
uint64_t binary_gcd(uint64_t a, uint64_t b) {
    if (a == 0) return b;
    if (b == 0) return a;
    
    int shift;
#if __has_builtin(__builtin_ctzll) || defined(__GNUC__)
    /* Use __builtin_ctzll */
    shift = __builtin_ctzll(a | b);
    a >>= shift;
    b >>= shift;

    a >>= __builtin_ctzll(a);
#else
    /* Fallback for compilers without __builtin_ctzll */
    for (shift = 0; ((a | b) & 1) == 0; ++shift) {
        a >>= 1;
        b >>= 1;
    }

    while ((a & 1) == 0) {
        a >>= 1;
    }

#endif
    
    do {
#if __has_builtin(__builtin_ctzll) || defined(__GNUC__)
        b >>= __builtin_ctzll(b);
#else
        while ((b & 1) == 0) {
            b >>= 1;
        }
#endif

        if (a > b) {
            uint64_t temp = a;
            a = b;
            b = temp;
        }

        b = b - a; // Both are odd, so b - a is even
    } while (b != 0);

    return a << shift;
}

#define ITERATIONS 10000000

int main() {
    // Generate 100 large pseudo-random number pairs for testing
    uint64_t test_pairs[100][2];
    uint64_t seed = 987654321ULL;
    
    for (int i = 0; i < 100; i++) {
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;
        test_pairs[i][0] = seed;
        
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;
        test_pairs[i][1] = seed;
        
        // Avoid zeros
        if (test_pairs[i][0] == 0) test_pairs[i][0] = 54321;
        if (test_pairs[i][1] == 0) test_pairs[i][1] = 12345;
    }

    clock_t start, end;
    double cpu_time_used;
    volatile uint64_t dummy = 0; // Prevents compiler optimization dead-code elimination

    // --- Benchmark: Euclidean GCD ---
    start = clock();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int idx = iter % 100;
        dummy += euclidean_gcd(test_pairs[idx][0], test_pairs[idx][1]);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Euclidean Algorithm Time:  %.4f seconds\n", cpu_time_used);

    // --- Benchmark: Binary GCD ---
    start = clock();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int idx = iter % 100;
        dummy += binary_gcd(test_pairs[idx][0], test_pairs[idx][1]);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Binary GCD (Stein's) Time: %.4f seconds\n", cpu_time_used);

    // Reference check to use dummy value
    if (dummy == 0) printf("Optimization guard check.\n");

    return 0;
}

/*
10-year old Dell, with Linux and gcc:

Euclidean Algorithm Time:  5.0048 seconds
Binary GCD (Stein's) Time: 0.8906 seconds

*/