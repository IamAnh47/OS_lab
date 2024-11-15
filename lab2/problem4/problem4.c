#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define FILE_NAME "shared_file.txt"
#define FILE_SIZE 1024 

int main() {
    int fd = open(FILE_NAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, FILE_SIZE) == -1) {
        perror("Error setting file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char *mapped_data = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd); 

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        const char *message = "Hello from child process!\n";
        snprintf(mapped_data, FILE_SIZE, "%s", message);
        printf("Child: Wrote to shared memory: %s", message);
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);

        printf("Parent: Read from shared memory: %s", mapped_data);

        if (munmap(mapped_data, FILE_SIZE) == -1) {
            perror("Error unmapping memory");
            exit(EXIT_FAILURE);
        }

        if (remove(FILE_NAME) == -1) {
            perror("Error removing file");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
