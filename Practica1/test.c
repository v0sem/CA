#include <stdio.h>
#include <x86intrin.h>

#define ARRAY_SIZE 2048

static double a[ARRAY_SIZE], b[ARRAY_SIZE], c;

int main(int argc, char *argv[]) {
    int t, i;

    double m = 1.0001;

    /* Vectorized initialization of A and B arrays */
    __m256d index = _mm256_setr_pd(0, 1, 2, 3);
    __m256d increment = _mm256_set_pd(4, 4, 4, 4);
    __m256d a_values, b_values;
    for (i = 0; i < ARRAY_SIZE; i += 4) {
        /*a[i] = i + 1*/
        a_values = _mm256_add_pd(index, increment);
        /*b[i] = i*/
        b_values = index;
        _mm256_storeu_pd(&a[i], a_values);
        _mm256_storeu_pd(&b[i], b_values);
        index = _mm256_add_pd(index, increment); // Increment index
    }

    /* Perform an operation a number of times */
    for (t = 0; t < 1000000; t++) {
        __m256d mm = _mm256_set1_pd(m);
        __m256d sum = _mm256_setzero_pd();
        for (i = 0; i < ARRAY_SIZE; i += 4) {
            __m256d va = _mm256_loadu_pd(&a[i]);
            __m256d vb = _mm256_loadu_pd(&b[i]);
            __m256d tmp = _mm256_fmadd_pd(mm, va, vb);
            sum = _mm256_add_pd(tmp, sum);
        }
        sum = _mm256_hadd_pd(sum, sum);
        sum = _mm256_hadd_pd(sum, sum);
        c += _mm256_cvtsd_f64(sum);
    }

    printf("%f\n", c);
    return 0;
}
