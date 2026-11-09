#include <stdio.h>
#include <math.h>
#include <time.h>

#define ITERATIONS 100000000

/*
Fast Inverse Square Root (Quake III Algorithm)
==============================================
This legendary hack computes $1/\sqrt{x}$ 
using a bit-level manipulation of IEEE 754 floating-point representation 
combined with a single Newton-Raphson step, avoiding slow floating-point 
division and square root instructions.

Output:
-------
Standard 1.0 / sqrtf Time:  0.1990 seconds
Quake III InvSqrt Time:     0.2070 seconds

Legendary hacks are known to compiler writers, so they may optimize code 
in ways that are not obvious. 

Intrinsics:
-----------

There’s no standard C function that combines reciprocal and square root. On x86, there
are _approximate_ reciprocal-square-root intrinsics:

```c
#include <immintrin.h>

__m128 x = _mm_set_ss(value);
float result = _mm_cvtss_f32(_mm_rsqrt_ss(x));
```

_mm_rsqrt_ss is available with SSE and supported by both GCC and MSVC, but it returns
an approximation with lower precision than 1.0f / sqrtf(value). For a vector of four 
floats, use _mm_rsqrt_ps.

If you need the standard result’s accuracy and special-case behavior, keep 
`1.0f / sqrtf(x)`. With options such as GCC’s -ffast-math, the compiler may use an 
approximate reciprocal-square-root instruction where permitted.

*/

// Naive standard library inverse square root
float naive_inv_sqrt(float x) {
    return 1.0f / sqrtf(x);
}

// Quake III Fast Inverse Square Root
float quake_inv_sqrt(float x) {
    float xhalf = 0.5f * x;
    int i = *(int*)&x;              // Cast float bits to integer
    i = 0x5f3759df - (i >> 1);      // Magic number bit shift
    x = *(float*)&i;                // Cast back to float
    x = x * (1.5f - xhalf * x * x); // Newton-Raphson iteration
    return x;
}

int main() {
    float val = 2.5f;
    volatile float dummy = 0;
    clock_t start, end;

    start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        dummy += naive_inv_sqrt(val + (float)(i % 10));
    }
    end = clock();
    printf("Standard 1.0 / sqrtf Time:  %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        dummy += quake_inv_sqrt(val + (float)(i % 10));
    }
    end = clock();
    printf("Quake III InvSqrt Time:     %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    if (dummy == 0.0f) printf("");
    return 0;
}