#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "shm_manager.h"
#include <map>
#include <set>
#include <string>

class EventBus {
public:
    static EventBus& getInstance() {
        static EventBus instance;
        return instance;
    }

    void publish_event(const std::string& topic, const std::string& data);
    void subscribe_event(const std::string& topic, void (*callback)(const std::string&, const std::string&));
    void init_eventbus();
    void cleanup_eventbus();

private:
    EventBus() {}
    ~EventBus() {}
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    std::map<std::string, std::set<void (*)(const std::string&, const std::string&)>> subscribers;
    SharedMemory* shm;
};

#endif // EVENTBUS_H
#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <stdbool.h>
#include <sys/types.h>

#define MAX_TOPICS 20
#define MAX_PUBLISHERS 10
#define MAX_SUBSCRIBERS 10
#define MAX_TOPIC_LENGTH 50
#define MAX_PUBLISHER_LENGTH 50
#define MAX_SUBSCRIBER_LENGTH 50
#define MAX_DATA_LENGTH 1024

typedef struct EventBus EventBus;

EventBus* eventbus_create();
void eventbus_destroy(EventBus* bus);

int eventbus_add_topic(EventBus* bus, const char* topic, const char* publisher);
int eventbus_add_publisher(EventBus* bus, const char* topic, const char* publisher);
int eventbus_add_subscriber(EventBus* bus, const char* topic, const char* subscriber);
int eventbus_publish(EventBus* bus, const char* topic, const char* data);
int eventbus_subscribe(EventBus* bus, const char* topic, void (*callback)(const char*, const char*));
void eventbus_list_topics(EventBus* bus);

#endif // EVENTBUS_H