#include <stdio.h>
#include <stdint.h>
#include <time.h>

#if (defined(_M_X64) && defined(_MSC_VER)) || \
    (defined(__x86_64__) && (defined(__GNUC__) || defined(__clang__)))
#define HAS_POPCNT_INTRINSIC 1
#include <nmmintrin.h>
#if defined(_MSC_VER)
#include <intrin.h>
#endif
#else
#define HAS_POPCNT_INTRINSIC 0
#endif


/*
Brian Kernighan’s Algorithm for Popcount
========================================

Instead of checking every single bit in a 64-bit integer, Brian Kernighan's 
trick (x &= (x - 1)) clears the lowest set bit instantly, running only as 
many iterations as there are set bits.

There is an intrinsic that does this, but this is a good exercise 
in bit manipulation and benchmarking:
The hardware instruction is POPCNT.

GCC: 
    __builtin_popcountll(x) counts set bits in an unsigned long long. 
    GCC emits the instruction when the target supports it; otherwise it may 
    use a software implementation.

Intel compilers (ICC/ICX): 
    The x86 intrinsic is _mm_popcnt_u64(x), declared in <nmmintrin.h>. 
    GCC also supports this intrinsic on x86.


Output:
------- 

Correctness test passed for 500 values, 3 implementations including POPCNT.
Naive Popcount Time:        1.9310 seconds
Brian Kernighan Time:       0.7980 seconds
POPCNT Time:                0.0370 seconds

*/

#define ITERATIONS 50000000

// Naive loop checking all 64 bits
int naive_popcount(uint64_t x) {
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if ((x >> i) & 1) count++;
    }
    return count;
}

// Brian Kernighan's Algorithm
int brian_kernighan_popcount(uint64_t x) {
    int count = 0;
    while (x) {
        x &= (x - 1); // Clears lowest set bit
        count++;
    }
    return count;
}

#if HAS_POPCNT_INTRINSIC
#if defined(__GNUC__) || defined(__clang__)
__attribute__((target("popcnt")))
#endif
static int intrinsic_popcount(uint64_t x) {
    return (int)_mm_popcnt_u64(x);
}

static int popcnt_supported(void) {
#if defined(_MSC_VER)
    int cpu_info[4];
    __cpuid(cpu_info, 1);
    return (cpu_info[2] & (1 << 23)) != 0;
#else
    __builtin_cpu_init();
    return __builtin_cpu_supports("popcnt");
#endif
}
#endif

int main() {
    uint64_t val = 0x0F0F0F0F0F0F0F0FULL; // 32 bits set
#if HAS_POPCNT_INTRINSIC
    int has_popcnt = popcnt_supported();
#endif

    // --- Correctness test ---
    for (uint64_t i = 0; i < 500; i++) {
        uint64_t test_value = val + i;
        int naive_result = naive_popcount(test_value);
        int kernighan_result = brian_kernighan_popcount(test_value);

        if (naive_result != kernighan_result) {
            fprintf(stderr,
                    "Correctness test failed at iteration %llu: naive = %d, Brian Kernighan = %d\n",
                    (unsigned long long)i, naive_result, kernighan_result);
            return 1;
        }
#if HAS_POPCNT_INTRINSIC
        if (has_popcnt) {
            int intrinsic_result = intrinsic_popcount(test_value);
            if (naive_result != intrinsic_result) {
                fprintf(stderr,
                        "Correctness test failed at iteration %llu: naive = %d, POPCNT = %d\n",
                        (unsigned long long)i, naive_result, intrinsic_result);
                return 1;
            }
        }
#endif
    }
#if HAS_POPCNT_INTRINSIC
    if (has_popcnt) {
        printf("Correctness test passed for 500 values, 3 implementations including POPCNT.\n");
    } else {
        printf("Correctness test passed for 500 values.\n");
        printf("POPCNT is not supported by this CPU; skipping its test and benchmark.\n");
    }
#endif

    volatile uint64_t dummy = 0;
    clock_t start, end;

    start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        dummy += naive_popcount(val + i);
    }
    end = clock();
    printf("Naive Popcount Time:        %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        dummy += brian_kernighan_popcount(val + i);
    }
    end = clock();
    printf("Brian Kernighan Time:       %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

#if HAS_POPCNT_INTRINSIC
    if (has_popcnt) {
        start = clock();
        for (int i = 0; i < ITERATIONS; i++) {
            dummy += intrinsic_popcount(val + i);
        }
        end = clock();
        printf("POPCNT Time:                %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    }
#endif

    if (dummy == 0) printf("");
    return 0;
}