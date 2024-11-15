#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SHARED_MEM_NAME "/" // Ten bo nho chia se
#define MAX_MOVIES 2000 
#define FILE1 "ratings_part1.txt" 
#define FILE2 "ratings_part2.txt" 


typedef struct {
    int movie_count[MAX_MOVIES]; 
    double movie_sum[MAX_MOVIES]; 
    int process_done[2]; 
} SharedData;

// Ham xu ly file
void process_file(const char *filename, SharedData *shared_data, int process_id) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Khong the mo file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    // Doc tung dong trong file
    while (fgets(line, sizeof(line), file)) {
        int userID, movieID, rating;
        long timestamp;
        // Doc du lieu theo dinh dang trong file
        // Cong diem danh gia
        // Tang so luong danh gia
        if (sscanf(line, "%d\t%d\t%d\t%ld", &userID, &movieID, &rating, &timestamp) == 4) {
            shared_data->movie_sum[movieID] += rating; 
            shared_data->movie_count[movieID]++;       
        }
    }

    fclose(file);
    shared_data->process_done[process_id] = 1; // Danh dau process da hoan thanh
}

int main() {
    // Tao bo nho chia se
    int fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open that bai");
        exit(EXIT_FAILURE);
    }

    // Dat kich thuoc cho bo nho chia se
    ftruncate(fd, sizeof(SharedData));
    SharedData *shared_data = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap that bai");
        exit(EXIT_FAILURE);
    }

    // Khoi tao du lieu chia se
    memset(shared_data, 0, sizeof(SharedData));

    // Tao process con thu nhat
    pid_t pid1 = fork();
    if (pid1 == 0) {
        process_file(FILE1, shared_data, 0); 
        exit(EXIT_SUCCESS);
    }

    // Tao process con thu hai
    pid_t pid2 = fork();
    if (pid2 == 0) {
        process_file(FILE2, shared_data, 1); 
        exit(EXIT_SUCCESS);
    }

    // Process cha cho hai process con ket thuc
    wait(NULL);
    wait(NULL);

    // Tinh trung binh danh gia va in ket qua
    printf("MovieID\tAverageRating\n");
    for (int i = 0; i < MAX_MOVIES; i++) {
        if (shared_data->movie_count[i] > 0) {
            double average = shared_data->movie_sum[i] / shared_data->movie_count[i];
            printf("%d\t%.2f\n", i, average);
        }
    }

    // Giai phong tai nguyen
    munmap(shared_data, sizeof(SharedData));
    shm_unlink(SHARED_MEM_NAME);

    return 0;
}
