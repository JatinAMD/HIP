/*
Copyright (c) 2015-2019 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/* HIT_START
 * BUILD: %t %s ../test_common.cpp
 * TEST: %t
 * HIT_END
 */

#include <hip/hip_runtime.h>
#include <type_traits>
#include <random>
#include "test_common.h"

template <typename T, typename M>
inline constexpr int count() {
    return sizeof(T) / sizeof(M);
}

inline int getRandomNumber(int min = INT_MIN, int max = INT_MAX) {
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<std::mt19937::result_type> gen(min, max);
    return gen(rng);
}

inline float getRandomFloat() { return float(getRandomNumber() / getRandomNumber()); }

template <typename T, typename B>
void fillMatrix(T* a, int size) {
    for (int i = 0; i < size; i++) {
        T t;
        t.x = getRandomFloat();
        if constexpr (count<T, B>() >= 2) t.y = getRandomFloat();
        if constexpr (count<T, B>() >= 3) t.z = getRandomFloat();
        if constexpr (count<T, B>() >= 4) t.w = getRandomFloat();

        a[i] = t;
    }
}

// Test operations
template <typename T, typename B>
void testOperations(T* a, T* b, int size) {
    a.x += b.x;
    a.x++;
    b.x++;
    if constexpr (count<T, B>() >= 2) {
        a.y = b.x;
        a.x = b.y;
    }
    if constexpr (count<T, B>() >= 3) {
        b.x /= a.x;
        a.x *= b.z;
        a.y--;
        b.y = -a.x;
    }
    if constexpr (count<T, B>() >= 4) {
        b.w = a.x;
        a.w += (-b.y);
    }
}

// a[i][j] += (a[i][j] * b[j][i]);
template <typename T>
void matAcc(T* a, T* b, int size) {
    for (int i = 0; i < size; i++) {
        a[i] += (a[i] * b[i]);
    }
}

template <typename T>
void dcopy(T* a, T* b, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = b[i];
    }
}

template <typename T>
bool isEqual(T* a, T* b, int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

template <typename T>
__global__ void gMatAcc(T* a, T* b, int size) {
    int i = threadIdx.x;

    if (i >= size) return;
    a[i] += (a[i] * b[i]);
}

// Main function that tests type
// T = what you want to test
// A = pack of 4 i.e. float4 int4
// B = pack of 3 i.e. float3 int3
// C = pack of 2 i.e. float2 int2
// D = pack of 1 i.e. float1 int1
template <typename T, typename A, typename B, typename C, typename D>
void testType(int msize) {
    T *fa, *fb, *fc;
    fa = new T[msize];
    fb = new T[msize];
    fc = new T[msize];

    T *d_fa, *d_fb;

    constexpr int c = count<T, D>();

    if (c <= 0 || c >= 4) failed("Invalid Size\n");

    fillMatrix<T,D>(fa, msize);
    deepCopy(fb, fa, msize);
    testOperations<T,D>(fa, fb, msize);

    hipMalloc(&d_fa, sizeof(T) * msize);
    hipMalloc(&d_fb, sizeof(T) * msize);

    hipMemcpy(d_fa, fa, sizeof(T) * msize, hipMemcpyHostToDevice);
    hipMemcpy(d_fb, fb, sizeof(T) * msize, hipMemcpyHostToDevice);

    matAcc<T>(fa, fb, msize);

    hipLaunchKernelGGL(gMatAcc, 1, msize, 0, 0, d_fa, d_fb, msize);

    hipMemcpy(fc, d_fa, sizeof(T) * msize, hipMemcpyDeviceToHost);

    bool pass = true;
    if (!isEqual<T>(fa, fc, msize)) {
        pass = false;
    }

    delete[] fa;
    delete[] fb;
    delete[] fc;
    hipFree(d_fa);
    hipFree(d_fb);

    if (!pass) {
        failed("Failed");
    }
}

int main() {
    const int msize = 100;
    // Floats
    testType<double2, double4, double3, double2, double1>(msize);
    
    passed();
}
