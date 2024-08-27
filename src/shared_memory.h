#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdint.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

typedef struct {
    int publisher_count;
    int subscriber_count;
    char data[SHM_SIZE];
} SharedMemory;

void* create_shared_memory();
void destroy_shared_memory(void* shm_ptr);

#endif // SHARED_MEMORY_H