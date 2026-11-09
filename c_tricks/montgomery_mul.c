
/*
Montgomery Multiplication:
==========================
Montgomery multiplication replaces expensive division by a modulus N with 
bit shifts and multiplications, making it foundational for cryptographic 
algorithms like RSA.

The implementation below avoids 128-bit integers and instead uses two 32-bit 
iterations to perform the reduction, enabling compiling with MSVC in Windows.

Output:
-------

Correctness test passed for 500 input pairs.
Naive Mod Mul Time:         0.0130 seconds (checksum=3088883340587340390)
Montgomery Mod Mul Time:    0.1520 seconds (checksum=3088883340587340390)

Over-smart is bad. The naive implementation is faster than the Montgomery one.

Why is the Montgomery path slower than the naive one?
-----------------------------------------------------

This benchmark times one standalone modular multiplication, and our 
Montgomery wrapper converts both operands into Montgomery form and 
converts the result back on every iteration.

That means each iteration does roughly four Montgomery reductions 
and three manually split 64×64 multiplications. Montgomery arithmetic 
is designed to avoid those conversions: convert values once, then do 
many multiplications in Montgomery form, converting the final result 
back only once.

Meanwhile, this particular naive operation is relatively cheap: its 
product fits in uint64_t, and the modulus is constant, so an optimizing 
compiler may replace % modulus with a faster sequence of operations. 
The naive method can therefore win for these inputs.

For a more representative Montgomery benchmark, reuse a Montgomery-encoded 
operand (or perform a chain of operations in Montgomery form), and include 
conversions only when they’re actually needed by the workload. The current 
result indicates that the end-to-end Montgomery path is slower for this 
single-operation case; it doesn’t show that Montgomery multiplication is 
always slower. The checksum match confirms both timed paths return equivalent 
results.

*/

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

#define ITERATIONS 5000000
#define CORRECTNESS_TEST_COUNT 500

typedef struct {
    uint64_t n;
    uint32_t n0_inv; // -N^(-1) mod 2^32
    uint64_t r2;     // R^2 mod N
} MontgomeryContext;

// Fast 32-bit modular inverse loop
uint32_t mod_inverse32(uint32_t n) {
    uint32_t inv = 1;
    for (int i = 0; i < 5; i++) {
        inv = inv * (2 - n * inv);
    }
    return inv;
}

void montgomery_init(MontgomeryContext *ctx, uint64_t n) {
    ctx->n = n;
    ctx->n0_inv = -mod_inverse32((uint32_t)n);

    // Naive fallback computation of R^2 mod N using purely 64-bit safe loops
    uint64_t r2 = 1;
    for (int i = 0; i < 128; i++) {
        r2 <<= 1;
        if (r2 >= n) r2 -= n;
    }
    ctx->r2 = r2;
}

// 64-bit safe Montgomery Reduction using two 32-bit iterations
// Eliminates any __uint128_t dependency.
uint64_t montgomery_reduce(const MontgomeryContext *ctx, uint64_t T_low, uint64_t T_high) {
    uint32_t t[4];
    t[0] = (uint32_t)T_low;
    t[1] = (uint32_t)(T_low >> 32);
    t[2] = (uint32_t)T_high;
    t[3] = (uint32_t)(T_high >> 32);

    uint32_t n32[2];
    n32[0] = (uint32_t)ctx->n;
    n32[1] = (uint32_t)(ctx->n >> 32);

    // Iteratively clear out lower 32-bit components via base 2^32 REDC
    for (int i = 0; i < 2; i++) {
        uint32_t m = t[i] * ctx->n0_inv;
        uint64_t carry = 0;

        for (int j = 0; j < 2; j++) {
            uint64_t prod = (uint64_t)m * n32[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)prod;
            carry = prod >> 32;
        }

        // Ripple carry upward through remaining upper buffer blocks
        int k = i + 2;
        while (carry > 0 && k < 4) {
            uint64_t sum = (uint64_t)t[k] + carry;
            t[k] = (uint32_t)sum;
            carry = sum >> 32;
            k++;
        }
    }

    // Reconstruct the 64-bit upper half result
    uint64_t u = ((uint64_t)t[3] << 32) | t[2];

    if (u >= ctx->n) {
        u -= ctx->n;
    }
    return u;
}

// Multiplies two 64-bit numbers into split components without 128-bit values
void mul64_split(uint64_t a, uint64_t b, uint64_t *res_low, uint64_t *res_high) {
    uint64_t a_low  = a & 0xFFFFFFFF;
    uint64_t a_high = a >> 32;
    uint64_t b_low  = b & 0xFFFFFFFF;
    uint64_t b_high = b >> 32;

    uint64_t p0 = a_low * b_low;
    uint64_t p1 = a_low * b_high;
    uint64_t p2 = a_high * b_low;
    uint64_t p3 = a_high * b_high;

    uint64_t middle = p1 + p2;
    // Check overflow bounds on the middle additions
    uint64_t middle_carry = (middle < p1) ? 1ULL << 32 : 0;

    uint64_t low = p0 + (middle << 32);
    uint64_t low_carry = (low < p0) ? 1 : 0;

    *res_low = low;
    *res_high = p3 + (middle >> 32) + middle_carry + low_carry;
}

uint64_t to_montgomery(const MontgomeryContext *ctx, uint64_t x) {
    uint64_t low, high;
    mul64_split(x, ctx->r2, &low, &high);
    return montgomery_reduce(ctx, low, high);
}

uint64_t from_montgomery(const MontgomeryContext *ctx, uint64_t x_bar) {
    return montgomery_reduce(ctx, x_bar, 0);
}

uint64_t montgomery_mul(const MontgomeryContext *ctx, uint64_t a_bar, uint64_t b_bar) {
    uint64_t low, high;
    mul64_split(a_bar, b_bar, &low, &high);
    return montgomery_reduce(ctx, low, high);
}

uint64_t montgomery_mod_mul(const MontgomeryContext *ctx, uint64_t a, uint64_t b) {
    uint64_t a_bar = to_montgomery(ctx, a);
    uint64_t b_bar = to_montgomery(ctx, b);
    return from_montgomery(ctx, montgomery_mul(ctx, a_bar, b_bar));
}

// These test and benchmark inputs are bounded so their product fits in uint64_t.
uint64_t naive_mod_mul(uint64_t a, uint64_t b, uint64_t n) {
    return (a * b) % n;
}

int correctness_test(const MontgomeryContext *ctx, uint64_t modulus,
                     uint64_t a, uint64_t b) {
    for (uint64_t i = 0; i < CORRECTNESS_TEST_COUNT; i++) {
        uint64_t test_a = a + i * UINT64_C(1000000);
        uint64_t test_b = b + i;
        uint64_t expected = naive_mod_mul(test_a, test_b, modulus);
        uint64_t actual = montgomery_mod_mul(ctx, test_a, test_b);

        if (expected != actual) {
            fprintf(stderr,
                    "Correctness test failed at iteration %" PRIu64
                    " (a=%" PRIu64 ", b=%" PRIu64 "): naive=%" PRIu64
                    ", Montgomery=%" PRIu64 "\n",
                    i, test_a, test_b, expected, actual);
            return 0;
        }
    }

    printf("Correctness test passed for %d input pairs.\n",
           CORRECTNESS_TEST_COUNT);
    return 1;
}

void benchmark_naive(uint64_t modulus, uint64_t a, uint64_t b) {
    volatile uint64_t checksum = 0;
    clock_t start = clock();
    for (uint64_t i = 0; i < ITERATIONS; i++) {
        checksum += naive_mod_mul(a + i, b, modulus);
    }
    clock_t end = clock();
    printf("Naive Mod Mul Time:         %.4f seconds (checksum=%" PRIu64 ")\n",
           (double)(end - start) / CLOCKS_PER_SEC, (uint64_t)checksum);
}

void benchmark_montgomery(const MontgomeryContext *ctx, uint64_t a, uint64_t b) {
    volatile uint64_t checksum = 0;
    clock_t start = clock();
    for (uint64_t i = 0; i < ITERATIONS; i++) {
        checksum += montgomery_mod_mul(ctx, a + i, b);
    }
    clock_t end = clock();
    printf("Montgomery Mod Mul Time:    %.4f seconds (checksum=%" PRIu64 ")\n",
           (double)(end - start) / CLOCKS_PER_SEC, (uint64_t)checksum);
}

int main(void) {
    uint64_t modulus = UINT64_C(1234567891011); // Odd modulus
    uint64_t a = UINT64_C(9876543210);
    uint64_t b = UINT64_C(1122334455);

    MontgomeryContext ctx;
    montgomery_init(&ctx, modulus);

    if (!correctness_test(&ctx, modulus, a, b)) {
        return 1;
    }

    benchmark_naive(modulus, a, b);
    benchmark_montgomery(&ctx, a, b);
    return 0;
}
