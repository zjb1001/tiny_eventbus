#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdint.h>
#include <stdbool.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024
#define MAX_TOPICS 10
#define MAX_TOPIC_LENGTH 50

typedef struct {
    int publisher_count;
    int subscriber_count;
    char topics[MAX_TOPICS][MAX_TOPIC_LENGTH];
    int topic_count;
    char data[SHM_SIZE];
    bool new_data;
} SharedMemory;

void* create_shared_memory();
void destroy_shared_memory(void* shm_ptr);

#endif // SHARED_MEMORY_H