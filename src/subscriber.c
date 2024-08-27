/* subscriber.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include "eventbus.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <topic_name>\n", argv[0]);
        exit(1);
    }

    int topic_id = subscribe(argv[1]);
    if (topic_id == -1) {
        fprintf(stderr, "Failed to subscribe to topic\n");
        exit(1);
    }

    printf("Subscribed to topic: %s\n", argv[1]);

    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    SharedMemory* shm_ptr = mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    sem_t* sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    while (1) {
        sem_wait(sem);

        if (shm_ptr->topics[topic_id].valid) {
            printf("Received message on topic %s: %s\n", argv[1], shm_ptr->topics[topic_id].message);
            shm_ptr->topics[topic_id].valid = 0;
        }

        sem_post(sem);

        sleep(1);
    }

    return 0;
}