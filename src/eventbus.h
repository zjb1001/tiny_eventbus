#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "shared_memory.h"

void publish_event(SharedMemory* shm, const char* topic, const char* data);
void subscribe_event(SharedMemory* shm, const char* topic);

#endif // EVENTBUS_H