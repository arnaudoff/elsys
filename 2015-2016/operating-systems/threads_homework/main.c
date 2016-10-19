#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define THREAD_COUNT 4

typedef struct {
    int start;
    int end;
} range_t;

void *calculate_range(void* range) {
    range_t* curr_range = (range_t*)range;

    void* result = (void*)1;
    for (int i = curr_range->start; i < curr_range->end; i++) {
        double a = cos(i) * cos(i) + sin(i) * sin(i);
        if (a > 1.0005 || a < 0.9995) {
            result = (void*)0;
        }
    }

    free(curr_range);
    return result;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int arg_start = 0;
    for (int i = 0; i < THREAD_COUNT; i++) {
        range_t *curr_range = (range_t*)malloc(sizeof(range_t));
        curr_range->start = arg_start;
        curr_range->end = arg_start + 25000000;

        int res = pthread_create(&threads[i], NULL, calculate_range, curr_range);

        if (res != 0) {
            perror("Could not spawn new thread");
            exit(-1);
        }

        arg_start = curr_range->end;
    }

    long final_result = 1;
    for (int i = 0; i < THREAD_COUNT; i++) {
        void *thread_result;
        int res = pthread_join(threads[i], (void **)&thread_result);
        if (res != 0) {
            perror("Could not spawn thread");
            exit(-1);
        }

        final_result <<= (long)thread_result;
    }

    if (final_result & (1 << 4)) {
        printf("OK!\n");
    } else {
        printf("Not OK!\n");
    }

    return 0;
}
