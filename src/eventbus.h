#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "shm_manager.h"

void publish_event(const char* topic, const char* data);
void subscribe_event(const char* topic, void (*callback)(const char*, const char*));
void init_eventbus();
void cleanup_eventbus();

#endif // EVENTBUS_H