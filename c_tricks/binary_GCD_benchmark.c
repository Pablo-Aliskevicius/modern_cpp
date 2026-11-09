#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if !(__has_builtin(__builtin_ctzll) || defined(__GNUC__)) && defined(_MSC_VER) && defined(_M_X64)
#include <intrin.h>
#endif

/*
Linux:
gcc -O3 binary_GCD_benchmark.c -o ./out/gcd_benchmark
./out/gcd_benchmark

Windows:
cl /O2 binary_GCD_benchmark.c /Fe:out/gcd_benchmark.exe
out\gcd_benchmark.exe

*/
static inline unsigned trailing_zeroes(uint64_t value) {
#if __has_builtin(__builtin_ctzll) || defined(__GNUC__)
    return (unsigned)__builtin_ctzll(value);
#elif defined(_MSC_VER) && defined(_M_X64)
    unsigned long index;
    _BitScanForward64(&index, value);
    return (unsigned)index;
#else
    unsigned count = 0;
    while ((value & 1) == 0) {
        value >>= 1;
        ++count;
    }
    return count;
#endif
}

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
    
    unsigned shift = trailing_zeroes(a | b);
    a >>= shift;
    b >>= shift;
    a >>= trailing_zeroes(a);
    
    do {
        b >>= trailing_zeroes(b);

        if (a > b) {
            uint64_t temp = a;
            a = b;
            b = temp;
        }

        b = b - a; // Both are odd, so b - a is even
    } while (b != 0);

    return a << shift;
}

#define RANDOM_TEST_PAIR_COUNT 100
#define TEST_PAIR_COUNT (RANDOM_TEST_PAIR_COUNT + 2)
#define ITERATIONS 10000000

int main() {
    // Generate 100 large pseudo-random number pairs
    uint64_t test_pairs[TEST_PAIR_COUNT][2];
    uint64_t seed = 987654321ULL;
    
    for (int i = 0; i < RANDOM_TEST_PAIR_COUNT; i++) {
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

    test_pairs[100][0] = UINT64_C(1) << 8;
    test_pairs[100][1] = UINT64_C(1) << 4;
    test_pairs[101][0] = UINT64_C(1) << 60;
    test_pairs[101][1] = UINT64_C(1) << 3;

    // --- Correctness test ---
    for (int i = 0; i < TEST_PAIR_COUNT; i++) {
        uint64_t a = test_pairs[i][0];
        uint64_t b = test_pairs[i][1];
        uint64_t euclidean_result = euclidean_gcd(a, b);
        uint64_t binary_result = binary_gcd(a, b);

        if (euclidean_result != binary_result) {
            fprintf(stderr,
                    "Correctness test failed for pair %d (%" PRIu64 ", %" PRIu64
                    "): Euclidean GCD = %" PRIu64 ", Binary GCD = %" PRIu64 "\n",
                    i, a, b, euclidean_result, binary_result);
            return 1;
        }
    }
    printf("Correctness test passed for all %d test pairs.\n", TEST_PAIR_COUNT);

    clock_t start, end;
    double cpu_time_used;
    volatile uint64_t dummy = 0; // Prevents compiler optimization dead-code elimination

    // --- Benchmark: Euclidean GCD ---
    start = clock();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int idx = iter % RANDOM_TEST_PAIR_COUNT;
        dummy += euclidean_gcd(test_pairs[idx][0], test_pairs[idx][1]);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Euclidean Algorithm Time:  %.4f seconds\n", cpu_time_used);

    // --- Benchmark: Binary GCD ---
    start = clock();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        int idx = iter % RANDOM_TEST_PAIR_COUNT;
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

Slightly younger HP laptop, with Windows and MSVC:
Euclidean Algorithm Time:  1.6430 seconds
Binary GCD (Stein's) Time: 1.0470 seconds

*/