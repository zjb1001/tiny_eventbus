#include "eventbus.h"
#include <iostream>
#include <string>

void event_callback(const std::string& topic, const std::string& data) {
    std::cout << "Received event - Topic: " << topic << ", Data: " << data << std::endl;
}

int main() {
    EventBus::getInstance().init_eventbus();
    
    std::string topic;
    std::cout << "Enter topic to subscribe: ";
    std::getline(std::cin, topic);
    
    EventBus::getInstance().subscribe_event(topic, event_callback);
    
    // Note: cleanup_eventbus() is not called here because subscribe_event runs indefinitely
    
    return 0;
}