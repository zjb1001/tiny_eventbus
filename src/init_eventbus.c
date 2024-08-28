#include "eventbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("Caught signal %d. Cleaning up...\n", signum);
    eventbus_cleanup();
    exit(signum);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <bus_name>\n", argv[0]);
        return 1;
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (!eventbus_init(argv[1])) {
        fprintf(stderr, "Failed to initialize event bus\n");
        return 1;
    }

    printf("Event bus '%s' initialized. Press Ctrl+C to exit.\n", argv[1]);

    // Keep the process running
    while(1) {
        sleep(1);
    }

    return 0;
}