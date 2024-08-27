/* event_bus.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include "eventbus.h"

static SharedMemory* shm_ptr = NULL;
static sem_t* sem = NULL;

int init_event_bus() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return -1;
    }

    if (ftruncate(shm_fd, sizeof(SharedMemory)) == -1) {
        perror("ftruncate");
        return -1;
    }

    shm_ptr = mmap(0, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    memset(shm_ptr, 0, sizeof(SharedMemory));

    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }

    return 0;
}

int register_publisher(const char* name) {
    sem_wait(sem);

    int id = -1;
    if (shm_ptr->publisher_count < MAX_PUBLISHERS) {
        id = shm_ptr->publisher_count;
        shm_ptr->publishers[id].id = id;
        strncpy(shm_ptr->publishers[id].name, name, MAX_TOPIC_NAME - 1);
        shm_ptr->publisher_count++;
    }

    sem_post(sem);

    return id;
}

int publish(int publisher_id, const char* topic_name, const char* message) {
    sem_wait(sem);

    int result = -1;
    for (int i = 0; i < MAX_TOPICS; i++) {
        if (shm_ptr->topics[i].name[0] == '\0' || strcmp(shm_ptr->topics[i].name, topic_name) == 0) {
            strncpy(shm_ptr->topics[i].name, topic_name, MAX_TOPIC_NAME - 1);
            strncpy(shm_ptr->topics[i].message, message, MAX_MESSAGE_SIZE - 1);
            shm_ptr->topics[i].valid = 1;
            result = 0;
            break;
        }
    }

    sem_post(sem);

    return result;
}

int subscribe(const char* topic_name) {
    sem_wait(sem);

    int result = -1;
    for (int i = 0; i < MAX_TOPICS; i++) {
        if (strcmp(shm_ptr->topics[i].name, topic_name) == 0) {
            result = i;
            break;
        }
    }

    sem_post(sem);

    return result;
}

int main() {
    if (init_event_bus() != 0) {
        fprintf(stderr, "Failed to initialize event bus\n");
        return 1;
    }

    printf("Event Bus is running. Press Ctrl+C to exit.\n");
    while(1) {
        sleep(1);
    }

    return 0;
}