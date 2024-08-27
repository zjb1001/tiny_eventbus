#include "shared_memory.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

void* create_shared_memory() {
    int shmid = shmget(SHM_KEY, sizeof(SharedMemory), 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    void* shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    return shm_ptr;
}

void destroy_shared_memory(void* shm_ptr) {
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(1);
    }
    int shmid = shmget(SHM_KEY, sizeof(SharedMemory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
}