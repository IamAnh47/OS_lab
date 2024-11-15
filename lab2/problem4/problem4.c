#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "shared_memory.txt"
#define FILE_SIZE 100

int main() {
    int fd = open(FILE_NAME, O_RDWR | O_CREAT, 0666);
    ftruncate(fd, FILE_SIZE);

    char *mapped = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    strcpy(mapped, "Hello from process 1");

    munmap(mapped, FILE_SIZE);
    close(fd);

    return 0;
}
