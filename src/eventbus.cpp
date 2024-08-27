#include "eventbus.h"
#include <string>
#include <iostream>
#include <unistd.h>

void EventBus::init_eventbus() {
    shm = static_cast<SharedMemory*>(create_shared_memory());
}

void EventBus::cleanup_eventbus() {
    destroy_shared_memory(shm);
}

void EventBus::publish_event(const std::string& topic, const std::string& data) {
    shm->publisher_count++;
    
    // Check if the topic already exists
    int topic_index = -1;
    for (int i = 0; i < shm->topic_count; i++) {
        if (std::string(shm->topics[i]) == topic) {
            topic_index = i;
            break;
        }
    }
    
    // If the topic doesn't exist, add it
    if (topic_index == -1 && shm->topic_count < MAX_TOPICS) {
        strncpy(shm->topics[shm->topic_count], topic.c_str(), MAX_TOPIC_LENGTH - 1);
        shm->topics[shm->topic_count][MAX_TOPIC_LENGTH - 1] = '\0';
        topic_index = shm->topic_count;
        shm->topic_count++;
    }
    
    if (topic_index != -1) {
        snprintf(shm->data, SHM_SIZE, "%s:%s", topic.c_str(), data.c_str());
        shm->new_data = true;
        std::cout << "Published event: " << shm->data << std::endl;
        
        // Notify subscribers
        if (subscribers.find(topic) != subscribers.end()) {
            for (auto& callback : subscribers[topic]) {
                callback(topic, data);
            }
        }
    } else {
        std::cout << "Failed to publish event: too many topics" << std::endl;
    }
}

void EventBus::subscribe_event(const std::string& topic, void (*callback)(const std::string&, const std::string&)) {
    shm->subscriber_count++;
    std::cout << "Subscribed to topic: " << topic << std::endl;
    
    subscribers[topic].insert(callback);
}