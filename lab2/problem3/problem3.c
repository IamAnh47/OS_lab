#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <unistd.h>
#include <fcntl.h>

#define QUEUE_A_TO_B "/queue_a_to_b"
#define QUEUE_B_TO_A "/queue_b_to_a"
#define MAX_MSG_SIZE 256
#define MAX_MESSAGES 10

void *thread_a_func(void *arg) {
    mqd_t mq_send_handle = mq_open(QUEUE_A_TO_B, O_WRONLY);
    if (mq_send_handle == -1) {
        perror("Thread A: mq_open for sending failed");
        pthread_exit(NULL);
    }

    mqd_t mq_receive_handle = mq_open(QUEUE_B_TO_A, O_RDONLY);
    if (mq_receive_handle == -1) {
        perror("Thread A: mq_open for receiving failed");
        pthread_exit(NULL);
    }

    char send_buffer[MAX_MSG_SIZE];
    char recv_buffer[MAX_MSG_SIZE];

    for (int i = 0; i < 5; i++) {
        snprintf(send_buffer, MAX_MSG_SIZE, "Message %d from A", i + 1);
        if (mq_send(mq_send_handle, send_buffer, strlen(send_buffer) + 1, 0) == -1) {
            perror("Thread A: mq_send failed");
        } else {
            printf("Thread A: Sent message: %s\n", send_buffer);
        }

        if (mq_receive(mq_receive_handle, recv_buffer, MAX_MSG_SIZE, NULL) == -1) {
            perror("Thread A: mq_receive failed");
        } else {
            printf("Thread A: Received message: %s\n", recv_buffer);
        }
    }

    mq_close(mq_send_handle);
    mq_close(mq_receive_handle);
    pthread_exit(NULL);
}

void *thread_b_func(void *arg) {
    mqd_t mq_receive_handle = mq_open(QUEUE_A_TO_B, O_RDONLY);
    if (mq_receive_handle == -1) {
        perror("Thread B: mq_open for receiving failed");
        pthread_exit(NULL);
    }

    mqd_t mq_send_handle = mq_open(QUEUE_B_TO_A, O_WRONLY);
    if (mq_send_handle == -1) {
        perror("Thread B: mq_open for sending failed");
        pthread_exit(NULL);
    }

    char send_buffer[MAX_MSG_SIZE];
    char recv_buffer[MAX_MSG_SIZE];

    for (int i = 0; i < 5; i++) {
        if (mq_receive(mq_receive_handle, recv_buffer, MAX_MSG_SIZE, NULL) == -1) {
            perror("Thread B: mq_receive failed");
        } else {
            printf("Thread B: Received message: %s\n", recv_buffer);
        }

        snprintf(send_buffer, MAX_MSG_SIZE, "Message %d from B", i + 1);
        if (mq_send(mq_send_handle, send_buffer, strlen(send_buffer) + 1, 0) == -1) {
            perror("Thread B: mq_send failed");
        } else {
            printf("Thread B: Sent message: %s\n", send_buffer);
        }
    }

    mq_close(mq_send_handle);
    mq_close(mq_receive_handle);
    pthread_exit(NULL);
}


int main() {
    // Thiết lập thuộc tính hàng đợi
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = MAX_MESSAGES,
        .mq_msgsize = MAX_MSG_SIZE,
        .mq_curmsgs = 0,
    };

    // Tạo hàng đợi
    mqd_t mq1 = mq_open(QUEUE_A_TO_B, O_CREAT | O_RDWR, 0666, &attr);
    mqd_t mq2 = mq_open(QUEUE_B_TO_A, O_CREAT | O_RDWR, 0666, &attr);

    if (mq1 == -1 || mq2 == -1) {
        perror("Main: mq_open failed");
        exit(EXIT_FAILURE);
    }

    // Tạo hai luồng
    pthread_t thread_a, thread_b;
    pthread_create(&thread_a, NULL, thread_a_func, NULL);
    pthread_create(&thread_b, NULL, thread_b_func, NULL);

    // Chờ các luồng kết thúc
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    // Xóa hàng đợi
    mq_close(mq1);
    mq_close(mq2);
    mq_unlink(QUEUE_A_TO_B);
    mq_unlink(QUEUE_B_TO_A);

    return 0;
}
