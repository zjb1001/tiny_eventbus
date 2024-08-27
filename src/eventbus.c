#include "eventbus.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void publish_event(SharedMemory* shm, const char* topic, const char* data) {
    shm->publisher_count++;
    
    // Check if the topic already exists
    int topic_index = -1;
    for (int i = 0; i < shm->topic_count; i++) {
        if (strcmp(shm->topics[i], topic) == 0) {
            topic_index = i;
            break;
        }
    }
    
    // If the topic doesn't exist, add it
    if (topic_index == -1 && shm->topic_count < MAX_TOPICS) {
        strncpy(shm->topics[shm->topic_count], topic, MAX_TOPIC_LENGTH - 1);
        shm->topics[shm->topic_count][MAX_TOPIC_LENGTH - 1] = '\0';
        topic_index = shm->topic_count;
        shm->topic_count++;
    }
    
    if (topic_index != -1) {
        snprintf(shm->data, SHM_SIZE, "%s:%s", topic, data);
        shm->new_data = true;
        printf("Published event: %s\n", shm->data);
    } else {
        printf("Failed to publish event: too many topics\n");
    }
}

void subscribe_event(SharedMemory* shm, const char* topic, void (*callback)(const char*, const char*)) {
    shm->subscriber_count++;
    printf("Subscribed to topic: %s\n", topic);
    
    while (1) {
        if (shm->new_data) {
            char *colon = strchr(shm->data, ':');
            if (colon != NULL) {
                *colon = '\0';
                if (strcmp(shm->data, topic) == 0) {
                    callback(topic, colon + 1);
                }
                *colon = ':';
            }
            shm->new_data = false;
        }
        usleep(100000);  // Sleep for 100ms to reduce CPU usage
    }
}