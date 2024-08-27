#include "eventbus.h"
#include <string.h>
#include <stdio.h>

void publish_event(SharedMemory* shm, const char* topic, const char* data) {
    shm->publisher_count++;
    snprintf(shm->data, SHM_SIZE, "%s:%s", topic, data);
    printf("Published event: %s\n", shm->data);
}

void subscribe_event(SharedMemory* shm, const char* topic) {
    shm->subscriber_count++;
    printf("Subscribed to topic: %s\n", topic);
    printf("Received data: %s\n", shm->data);
}