#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int start;
    int end;
    unsigned long long sum;
} SumRange;

void *calculate_sum(void *arg) {
    SumRange *range = (SumRange *)arg;
    range->sum = 0;
    for (int i = range->start; i <= range->end; i++) {
        range->sum += i;
    }
    return NULL;
}

unsigned long long sum_multi_thread(int n, int num_threads) {
    pthread_t threads[num_threads];
    SumRange ranges[num_threads];
    int range_size = n / num_threads;
    unsigned long long total_sum = 0;

    for (int i = 0; i < num_threads; i++) {
        ranges[i].start = i * range_size + 1;
        ranges[i].end = (i == num_threads - 1) ? n : (i + 1) * range_size;
        pthread_create(&threads[i], NULL, calculate_sum, &ranges[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_sum += ranges[i].sum;
    }

    return total_sum;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_threads> <n>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int n = atoi(argv[2]);
    unsigned long long result = sum_multi_thread(n, num_threads);
    printf("Sum is: %llu\n", result);
    return 0;
}
