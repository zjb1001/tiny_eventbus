#ifndef SHM_MANAGER_H
#define SHM_MANAGER_H

#include "shared_memory.h"

SharedMemory* get_shared_memory();
void init_shared_memory();
void destroy_shared_memory();

#endif // SHM_MANAGER_H