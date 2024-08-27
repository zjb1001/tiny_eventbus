#include "eventbus.h"
#include <stdio.h>

int main() {
    SharedMemory* shm = (SharedMemory*)create_shared_memory();
    subscribe_event(shm, "topic1");
    destroy_shared_memory(shm);
    return 0;
}