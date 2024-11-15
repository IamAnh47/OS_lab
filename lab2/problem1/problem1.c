#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SHARED_MEM_NAME "/movie_ratings"
#define MAX_MOVIES 2000
#define FILE1 "movie-100k_1.txt"
#define FILE2 "movie-100k_2.txt"

typedef struct {
    int movie_count[MAX_MOVIES];
    double movie_sum[MAX_MOVIES];
    int process_done[2];
} SharedData;

void process_file(const char *filename, SharedData *shared_data, int process_id) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int userID, movieID, rating;
        long timestamp;
        if (sscanf(line, "%d\t%d\t%d\t%ld", &userID, &movieID, &rating, &timestamp) == 4) {
            shared_data->movie_sum[movieID] += rating;
            shared_data->movie_count[movieID]++;
        }
    }

    fclose(file);
    shared_data->process_done[process_id] = 1; // Mark this process as done
}

int main() {
    int fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, sizeof(SharedData)); // Set the size of shared memory
    SharedData *shared_data = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // Initialize shared data
    memset(shared_data, 0, sizeof(SharedData));

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // First child process
        process_file(FILE1, shared_data, 0);
        exit(EXIT_SUCCESS);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Second child process
        process_file(FILE2, shared_data, 1);
        exit(EXIT_SUCCESS);
    }

    // Parent process waits for children
    wait(NULL);
    wait(NULL);

    // Calculate and print average ratings
    printf("MovieID\tAverageRating\n");
    for (int i = 0; i < MAX_MOVIES; i++) {
        if (shared_data->movie_count[i] > 0) {
            double average = shared_data->movie_sum[i] / shared_data->movie_count[i];
            printf("%d\t%.2f\n", i, average);
        }
    }

    // Clean up
    munmap(shared_data, sizeof(SharedData));
    shm_unlink(SHARED_MEM_NAME);

    return 0;
}
