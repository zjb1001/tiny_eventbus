#include "eventbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    SharedMemory* shm = (SharedMemory*)create_shared_memory();
    
    char topic[MAX_TOPIC_LENGTH];
    char data[SHM_SIZE];
    
    while (1) {
        printf("Enter topic (or 'quit' to exit): ");
        fgets(topic, sizeof(topic), stdin);
        topic[strcspn(topic, "\n")] = 0;  // Remove newline
        
        if (strcmp(topic, "quit") == 0) {
            break;
        }
        
        printf("Enter data: ");
        fgets(data, sizeof(data), stdin);
        data[strcspn(data, "\n")] = 0;  // Remove newline
        
        publish_event(shm, topic, data);
        
        sleep(1);  // Wait for 1 second before next publication
    }
    
    destroy_shared_memory(shm);
    return 0;
}