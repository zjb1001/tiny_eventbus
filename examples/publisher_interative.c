#include "../src/eventbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <bus_name>\n", argv[0]);
        return 1;
    }

    if (!eventbus_connect(argv[1])) {
        fprintf(stderr, "Failed to connect to event bus '%s'\n", argv[1]);
        return 1;
    }

    int topic_id = eventbus_create_topic("hello");
    if (topic_id < 0) {
        fprintf(stderr, "Failed to create topic\n");
        eventbus_disconnect();
        return 1;
    }

    printf("Connected to bus '%s' and created topic 'hello'\n", argv[1]);

    char message[MAX_MESSAGE_SIZE];
    while (1) {
        printf("Enter a message (or 'quit' to exit): ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;  // Remove newline

        if (strcmp(message, "quit") == 0) {
            break;
        }

        if (eventbus_publish_message("hello", message) == 0) {
            printf("Published: %s\n", message);
        } else {
            fprintf(stderr, "Failed to publish message\n");
        }
    }

    eventbus_disconnect();
    return 0;
}