/* event_bus.h */
#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#define MAX_TOPICS 20
#define MAX_PUBLISHERS 10
#define MAX_SUBSCRIBERS 10
#define MAX_TOPIC_NAME 32
#define MAX_MESSAGE_SIZE 256
#define SHM_NAME "/eventbus_shm"
#define SEM_NAME "/eventbus_sem"

typedef struct {
    char name[MAX_TOPIC_NAME];
    char message[MAX_MESSAGE_SIZE];
    int valid;
} Topic;

typedef struct {
    int id;
    char name[MAX_TOPIC_NAME];
} Publisher;

typedef struct {
    Topic topics[MAX_TOPICS];
    Publisher publishers[MAX_PUBLISHERS];
    int publisher_count;
} SharedMemory;

int register_publisher(const char* name);
int publish(int publisher_id, const char* topic_name, const char* message);
int subscribe(const char* topic_name);

#endif