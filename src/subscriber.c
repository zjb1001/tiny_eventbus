#include "eventbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void event_callback(const char* topic, const char* data) {
    printf("Received event - Topic: %s, Data: %s\n", topic, data);
}

int main() {
    init_eventbus();
    
    char topic[MAX_TOPIC_LENGTH];
    printf("Enter topic to subscribe: ");
    fgets(topic, sizeof(topic), stdin);
    topic[strcspn(topic, "\n")] = 0;  // Remove newline
    
    subscribe_event(topic, event_callback);
    
    // Note: cleanup_eventbus() is not called here because subscribe_event runs indefinitely
    
    return 0;
}