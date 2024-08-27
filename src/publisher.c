#include "eventbus.h"
#include <stdio.h>

int main() {
    SharedMemory* shm = (SharedMemory*)create_shared_memory();
    publish_event(shm, "topic1", "data1");
    destroy_shared_memory(shm);
    return 0;
}