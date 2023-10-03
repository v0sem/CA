#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <x86intrin.h>

#define ARRAY_SIZE 2048
#define NUMBER_OF_TRIALS 1000000

/*
 * Statically allocate our arrays.  Compilers can
 * align them correctly.
 */
static double a[ARRAY_SIZE], b[ARRAY_SIZE], c;

int main(int argc, char *argv[]) {
    int i,t,j, time, time_micro;
    struct timeval tv;

    double m = 1.0001;

    /* Vectorized initialization of A and B arrays */
    __m256d index = {0.0,1.0,2.0,3.0};
    __m256d a_buffer = {1.0,1.0,1.0,1.0};
    __m256d increment = {4.0,4.0,4.0,4.0};
    __m256d a_values, b_values;
    for (i = 0; i < ARRAY_SIZE; i += 4) {
        /*a[i] = i + 1*/
        a_values = _mm256_add_pd(index, a_buffer);
        /*b[i] = i*/
        b_values = index;

        /*Store the values in our arrays in groups of 4*/
        _mm256_storeu_pd(&a[i], a_values);
        _mm256_storeu_pd(&b[i], b_values);

        /*Increment index*/
        index = _mm256_add_pd(index, increment);
    }

    gettimeofday(&tv, NULL);
    time = tv.tv_sec;
    time_micro = tv.tv_usec;
    /* Perform an operation a number of times */
    for (t=0; t < NUMBER_OF_TRIALS; t++) {
        __m256d mm = {1.0001, 1.0001, 1.0001, 1.0001};
        __m256d sum = {0.0, 0.0, 0.0, 0.0};
        
        for (i=0; i < ARRAY_SIZE; i+=4) {
            
            __m256d va = _mm256_load_pd(&a[i]);
            __m256d vb = _mm256_load_pd(&b[i]);
            // Compute m*a+b
            __m256d tmp = _mm256_fmadd_pd(mm, va, vb);
            // Accumulate results
            sum = _mm256_add_pd (tmp, sum);
            // Get sum[2], sum[3]
            __m128d xmm = _mm256_extractf128_pd (sum, 1);
            // Extend to 256 bits: sum[2], sum[3], 0, 0
            __m256d ymm = _mm256_castpd128_pd256(xmm);
            // Perform sum[0]+sum[1], sum[2]+sum[3], sum[2]+sum[3], 0+0
            sum = _mm256_hadd_pd (sum, ymm); 

            c = sum[0];
        }
    }
    
    gettimeofday(&tv, NULL);
    printf("%ld.%ld\n", tv.tv_sec - time, tv.tv_usec - time_micro);
    printf("%f\n", c);
    return 0;
}
