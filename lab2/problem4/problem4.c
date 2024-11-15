#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

#define FILE_NAME "shared_memory.dat"
#define FILE_SIZE 1024 // 1KB shared memory

void writer_process() {
    // Mở hoặc tạo file
    int fd = open(FILE_NAME, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Mở rộng file tới kích thước yêu cầu
    if (ftruncate(fd, FILE_SIZE) == -1) {
        perror("Failed to set file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Map file vào vùng nhớ
    char *mapped = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Failed to mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd); 

    // Ghi dữ liệu vào vùng nhớ
    const char *message = "Hello from writer process!";
    snprintf(mapped, FILE_SIZE, "%s", message);
    printf("Writer: Wrote message to shared memory: %s\n", message);

    sleep(5);

    munmap(mapped, FILE_SIZE);
}

void reader_process() {

    int fd = open(FILE_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Map file vào vùng nhớ
    char *mapped = mmap(NULL, FILE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Failed to mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd); 

    printf("Reader: Read message from shared memory: %s\n", mapped);

    // Unmap vùng nhớ
    munmap(mapped, FILE_SIZE);
}

int main() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        sleep(1); 
        reader_process();
    } else {
        writer_process();
        wait(NULL); 
    }

    unlink(FILE_NAME);

    return 0;
}
