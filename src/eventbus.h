#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <stdbool.h>

#define MAX_TOPICS          10
#define MAX_SUBSCRIBERS     10
#define MAX_MESSAGE_SIZE    256
#define MAX_TOPIC_NAME      32
#define MAX_BUS_NAME        32

typedef struct {
    int subscriber_id;
    bool active;
} Subscriber;

typedef struct {
    char name[MAX_TOPIC_NAME];
    char message[MAX_MESSAGE_SIZE];
    Subscriber subscribers[MAX_SUBSCRIBERS];
    int subscriber_count;
} Topic;

typedef struct {
    char name[MAX_BUS_NAME];
    Topic topics[MAX_TOPICS];
    int topic_count;
} EventBus;

// Function prototypes
bool eventbus_init(const char* bus_name);
bool eventbus_connect(const char* bus_name);
void eventbus_disconnect();
int eventbus_create_topic(const char* topic_name);
int eventbus_subscribe_to_topic(const char* topic_name, int subscriber_id);
int eventbus_publish_message(const char* topic_name, const char* message);
char* eventbus_read_message(const char* topic_name);
void eventbus_cleanup();

#endif // EVENTBUS_H