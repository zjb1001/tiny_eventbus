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