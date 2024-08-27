/* publisher.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "eventbus.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <publisher_name>\n", argv[0]);
        exit(1);
    }

    int publisher_id = register_publisher(argv[1]);
    if (publisher_id == -1) {
        fprintf(stderr, "Failed to register publisher\n");
        exit(1);
    }

    printf("Registered as publisher with ID: %d\n", publisher_id);

    char topic_name[MAX_TOPIC_NAME];
    char message[MAX_MESSAGE_SIZE];

    while (1) {
        printf("Enter topic name (or 'quit' to exit): ");
        fgets(topic_name, sizeof(topic_name), stdin);
        topic_name[strcspn(topic_name, "\n")] = 0;

        if (strcmp(topic_name, "quit") == 0) {
            break;
        }

        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;

        if (publish(publisher_id, topic_name, message) == 0) {
            printf("Message published successfully\n");
        } else {
            printf("Failed to publish message\n");
        }
    }

    return 0;
}