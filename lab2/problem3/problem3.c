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

// Hàm gửi tin nhắn từ Thread A đến Thread B
void *thread_a_func(void *arg) {
    mqd_t mq_send = mq_open(QUEUE_A_TO_B, O_WRONLY);
    mqd_t mq_receive = mq_open(QUEUE_B_TO_A, O_RDONLY);

    if (mq_send == -1 || mq_receive == -1) {
        perror("Thread A: mq_open failed");
        pthread_exit(NULL);
    }

    char send_buffer[MAX_MSG_SIZE];
    char recv_buffer[MAX_MSG_SIZE];

    for (int i = 0; i < 5; i++) {
        snprintf(send_buffer, MAX_MSG_SIZE, "Message %d from A", i + 1);
        mq_send(mq_send, send_buffer, strlen(send_buffer) + 1, 0);

        printf("Thread A sent: %s\n", send_buffer);

        mq_receive(mq_receive, recv_buffer, MAX_MSG_SIZE, NULL);
        printf("Thread A received: %s\n", recv_buffer);
    }

    mq_close(mq_send);
    mq_close(mq_receive);
    pthread_exit(NULL);
}

// Hàm gửi tin nhắn từ Thread B đến Thread A
void *thread_b_func(void *arg) {
    mqd_t mq_receive = mq_open(QUEUE_A_TO_B, O_RDONLY);
    mqd_t mq_send = mq_open(QUEUE_B_TO_A, O_WRONLY);

    if (mq_send == -1 || mq_receive == -1) {
        perror("Thread B: mq_open failed");
        pthread_exit(NULL);
    }

    char send_buffer[MAX_MSG_SIZE];
    char recv_buffer[MAX_MSG_SIZE];

    for (int i = 0; i < 5; i++) {
        mq_receive(mq_receive, recv_buffer, MAX_MSG_SIZE, NULL);
        printf("Thread B received: %s\n", recv_buffer);

        snprintf(send_buffer, MAX_MSG_SIZE, "Reply %d from B", i + 1);
        mq_send(mq_send, send_buffer, strlen(send_buffer) + 1, 0);

        printf("Thread B sent: %s\n", send_buffer);
    }

    mq_close(mq_receive);
    mq_close(mq_send);
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
