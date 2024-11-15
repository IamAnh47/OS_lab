#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <unistd.h>

#define MSG_KEY_1 12345  
#define MSG_KEY_2 54321  

#define MSG_SIZE 100


struct message {
    long msg_type;
    char msg_text[MSG_SIZE];
};


void* thread1_func(void* arg) {
    int msgid_send = msgget(MSG_KEY_1, 0666 | IPC_CREAT);
    int msgid_recv = msgget(MSG_KEY_2, 0666 | IPC_CREAT);
    
    struct message msg;
    msg.msg_type = 1;
    strcpy(msg.msg_text, "Hello from Thread 1");

    printf("Thread 1: Sending message to Thread 2\n");
    msgsnd(msgid_send, &msg, sizeof(msg), 0);

    msgrcv(msgid_recv, &msg, sizeof(msg), 1, 0);
    printf("Thread 1: Received message from Thread 2: %s\n", msg.msg_text);

    msgctl(msgid_send, IPC_RMID, NULL);
    msgctl(msgid_recv, IPC_RMID, NULL);

    return NULL;
}

void* thread2_func(void* arg) {
    int msgid_recv = msgget(MSG_KEY_1, 0666 | IPC_CREAT);
    int msgid_send = msgget(MSG_KEY_2, 0666 | IPC_CREAT);
    
    struct message msg;

    msgrcv(msgid_recv, &msg, sizeof(msg), 1, 0);
    printf("Thread 2: Received message from Thread 1: %s\n", msg.msg_text);

    msg.msg_type = 1;
    strcpy(msg.msg_text, "Hello from Thread 2");
    printf("Thread 2: Sending reply to Thread 1\n");
    msgsnd(msgid_send, &msg, sizeof(msg), 0);

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread1_func, NULL);
    pthread_create(&thread2, NULL, thread2_func, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
