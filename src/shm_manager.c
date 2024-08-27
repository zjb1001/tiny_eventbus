#include "shm_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

static SharedMemory* shm = NULL;
static int shmid = -1;

void init_shared_memory() {
    if (shm != NULL) {
        return;  // Already initialized
    }

    shmid = shmget(SHM_KEY, sizeof(SharedMemory), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    shm = (SharedMemory*)shmat(shmid, NULL, 0);
    if (shm == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Initialize the shared memory
    shm->publisher_count = 0;
    shm->subscriber_count = 0;
    shm->topic_count = 0;
    shm->new_data = false;
}

SharedMemory* get_shared_memory() {
    if (shm == NULL) {
        init_shared_memory();
    }
    return shm;
}

void destroy_shared_memory() {
    if (shm != NULL) {
        if (shmdt(shm) == -1) {
            perror("shmdt failed");
        }
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl failed");
        }
        shm = NULL;
        shmid = -1;
    }
}