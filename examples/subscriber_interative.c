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

    int subscriber_id = getpid();
    char bus[MAX_BUS_NAME];        strcpy(bus, argv[1]);
    char topic[MAX_TOPIC_NAME];  

    if (!eventbus_connect(bus)) {
        fprintf(stderr, "Failed to connect to event bus '%s'\n", bus);
        return 1;
    }

    while (1) {
        printf("Enter a topic (or 'quit' to exit): ");
        fgets(topic, sizeof(topic), stdin);
        topic[strcspn(topic, "\n")] = 0;  // Remove newline

        if (strcmp(topic, "quit") == 0) {
            break;
        }

        if (eventbus_subscribe_to_topic(topic) != 0) {
            fprintf(stderr, "Failed to subscribe to topic %s\n", topic);
            eventbus_disconnect();
            return 1;
        }

        printf("Subscriber %d connected to bus '%s' and subscribed to topic %s\n", subscriber_id, bus, topic);

        char* message = eventbus_read_message(topic);
        if (message && strlen(message) > 0) {
            printf("Subscriber %d received: %s\n", subscriber_id, message);
        }
        sleep(1);
    }

    eventbus_disconnect();
    return 0;
}