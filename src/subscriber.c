#include "eventbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void event_callback(const char* topic, const char* data) {
    printf("Received event - Topic: %s, Data: %s\n", topic, data);
}

int main() {
    SharedMemory* shm = (SharedMemory*)create_shared_memory();
    
    char topic[MAX_TOPIC_LENGTH];
    printf("Enter topic to subscribe: ");
    fgets(topic, sizeof(topic), stdin);
    topic[strcspn(topic, "\n")] = 0;  // Remove newline
    
    subscribe_event(shm, topic, event_callback);
    
    // Note: The destroy_shared_memory(shm) call is removed because the subscribe_event function now runs indefinitely
    
    return 0;
}