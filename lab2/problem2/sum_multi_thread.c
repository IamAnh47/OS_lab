#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    unsigned long long start;
    unsigned long long end;
    unsigned long long partial_sum;
} ThreadData;

// Hàm do từng luồng thực hiện để tính tổng [start, end]
void *calculate_partial_sum(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->partial_sum = 0;
    for (unsigned long long i = data->start; i <= data->end; i++) {
        data->partial_sum += i;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <numThreads> <n>\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);
    unsigned long long n = atoll(argv[2]);

    if (numThreads <= 0 || n <= 0) {
        printf("Both numThreads and n must be positive.\n");
        return 1;
    }

    pthread_t threads[numThreads];
    ThreadData thread_data[numThreads];

    unsigned long long range = n / numThreads;
    unsigned long long remaining = n % numThreads;

    // Chia đoạn và tạo các luồng
    unsigned long long start = 1;
    for (int i = 0; i < numThreads; i++) {
        thread_data[i].start = start;
        thread_data[i].end = start + range - 1;

        // Chia phần dư cho các luồng đầu tiên
        if (remaining > 0) {
            thread_data[i].end += 1;
            remaining--;
        }

        start = thread_data[i].end + 1;

        if (pthread_create(&threads[i], NULL, calculate_partial_sum, &thread_data[i]) != 0) {
            perror("Error creating thread");
            return 1;
        }
    }

    // Đợi các luồng hoàn thành
    unsigned long long total_sum = 0;
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].partial_sum;
    }

    printf("Sum of numbers from 1 to %llu is: %llu\n", n, total_sum);
    return 0;
}
