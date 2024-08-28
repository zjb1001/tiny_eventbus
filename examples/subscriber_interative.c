#include "../src/eventbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <bus_name> <subscriber_id>\n", argv[0]);
        return 1;
    }

    int subscriber_id = atoi(argv[2]);

    if (!eventbus_connect(argv[1])) {
        fprintf(stderr, "Failed to connect to event bus '%s'\n", argv[1]);
        return 1;
    }

    if (eventbus_subscribe_to_topic("hello", subscriber_id) != 0) {
        fprintf(stderr, "Failed to subscribe to topic 'hello'\n");
        eventbus_disconnect();
        return 1;
    }

    printf("Subscriber %d connected to bus '%s' and subscribed to topic 'hello'\n", subscriber_id, argv[1]);
    printf("Listening for messages... (Press Ctrl+C to exit)\n");

    while (1) {
        char* message = eventbus_read_message("hello");
        if (message && strlen(message) > 0) {
            printf("Subscriber %d received: %s\n", subscriber_id, message);
        }
        sleep(1);
    }

    eventbus_disconnect();
    return 0;
}