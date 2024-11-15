#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define SHM_KEY 12345
#define MAX_MOVIES 1682

typedef struct {
    int movie_id;
    double total_rating;
    int count;
} MovieRating;

void calculate_average(const char *filename, MovieRating *shared_data) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    int user_id, movie_id, rating, timestamp;
    while (fscanf(file, "%d %d %d %d", &user_id, &movie_id, &rating, &timestamp) != EOF) {
        shared_data[movie_id].movie_id = movie_id;
        shared_data[movie_id].total_rating += rating;
        shared_data[movie_id].count += 1;
    }

    fclose(file);
}

int main() {
    int shm_id = shmget(SHM_KEY, sizeof(MovieRating) * MAX_MOVIES, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("Failed to create shared memory");
        exit(1);
    }

    MovieRating *shared_data = (MovieRating *) shmat(shm_id, NULL, 0);
    memset(shared_data, 0, sizeof(MovieRating) * MAX_MOVIES);

    if (fork() == 0) {
        calculate_average("movie-100k_1.txt", shared_data);
        exit(0);
    }

    if (fork() == 0) {
        calculate_average("movie-100k_2.txt", shared_data);
        exit(0);
    }


    wait(NULL);
    wait(NULL);


    for (int i = 1; i < MAX_MOVIES; i++) {
        if (shared_data[i].count > 0) {
            double avg_rating = shared_data[i].total_rating / shared_data[i].count;
            printf("Movie ID %d: Average Rating = %.2f\n", shared_data[i].movie_id, avg_rating);
        }
    }

    shmdt(shared_data);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
