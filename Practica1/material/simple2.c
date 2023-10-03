#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

#define ARRAY_SIZE 2048
#define NUMBER_OF_TRIALS 100000

/*
 * Statically allocate our arrays.  Compilers can
 * align them correctly.
 */
static double a[ARRAY_SIZE], b[ARRAY_SIZE], c;

int main(int argc, char *argv[]) {
    int i,t,time, time_micro;
    
    struct timeval tv;
    double m = 1.0001;

    /* Populate A and B arrays */
    for (i=0; i < ARRAY_SIZE; i++) {
        b[i] = i;
        a[i] = i+1;
    }

    gettimeofday(&tv,NULL);
    time = tv.tv_sec;
    time_micro = tv.tv_usec;
    /* Perform an operation a number of times */
    for (t=0; t < NUMBER_OF_TRIALS; t++) {
        for (i=0; i < ARRAY_SIZE; i++) {
            c += m*a[i] + b[i];
        }
    }
    gettimeofday(&tv,NULL);
    printf("%ld.%ld\n", tv.tv_sec - time, tv.tv_usec - time_micro);
    return 0;
}
