#include "eventbus.h"
#include <iostream>
#include <string>
#include <unistd.h>

int main() {
    EventBus::getInstance().init_eventbus();
    
    std::string topic;
    std::string data;
    
    while (1) {
        std::cout << "Enter topic (or 'quit' to exit): ";
        std::getline(std::cin, topic);
        
        if (topic == "quit") {
            break;
        }
        
        std::cout << "Enter data: ";
        std::getline(std::cin, data);
        
        EventBus::getInstance().publish_event(topic, data);
        
        sleep(1);  // Wait for 1 second before next publication
    }
    
    EventBus::getInstance().cleanup_eventbus();
    return 0;
}