#include "eventbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>

#define SHM_KEY 1234
#define SEM_PREFIX "/eventbus_sem_"

static EventBus* bus = NULL;
static int shmid = -1;
static sem_t* sem = NULL;
static char current_bus_name[MAX_BUS_NAME] = {0};

static key_t get_key(const char* bus_name) {
    char temp_file[256];
    snprintf(temp_file, sizeof(temp_file), "/tmp/%s", bus_name);

	// Check if the file exists, if not create it
    int fd = open(temp_file, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    close(fd);
	
    return ftok(temp_file, SHM_KEY);
}

static int get_shared_memory(const char* bus_name) {
    key_t key = get_key(bus_name);
    if (key == -1) {
        perror("ftok failed");
        return -1;
    }
    
    int shmid = shmget(key, sizeof(EventBus), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return -1;
    }
    return shmid;
}

static EventBus* attach_event_bus(int shmid) {
    EventBus* bus = (EventBus*)shmat(shmid, NULL, 0);
    if (bus == (void*)-1) {
        perror("shmat failed");
        return NULL;
    }
    return bus;
}

static void detach_event_bus() {
    if (bus && shmdt(bus) == -1) {
        perror("shmdt failed");
    }
    bus = NULL;
}

bool eventbus_init(const char* bus_name) {
    key_t key = get_key(bus_name);
    if (key == -1) {
        perror("ftok failed");
        return false;
    }
    
    shmid = shmget(key, sizeof(EventBus), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return false;
    }
    
    bus = attach_event_bus(shmid);
    if (!bus) {
        return false;
    }
    
    memset(bus, 0, sizeof(EventBus));
    strncpy(bus->name, bus_name, MAX_BUS_NAME - 1);
    bus->name[MAX_BUS_NAME - 1] = '\0';
    
    char sem_name[64];
    snprintf(sem_name, sizeof(sem_name), "%s%s", SEM_PREFIX, bus_name);
    sem = sem_open(sem_name, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        detach_event_bus();
        return false;
    }
    
    strncpy(current_bus_name, bus_name, MAX_BUS_NAME - 1);
    current_bus_name[MAX_BUS_NAME - 1] = '\0';
    
    return true;
}

bool eventbus_connect(const char* bus_name) {
    shmid = get_shared_memory(bus_name);
    if (shmid == -1) {
        return false;
    }
    
    bus = attach_event_bus(shmid);
    if (!bus) {
        return false;
    }
    
    char sem_name[64];
    snprintf(sem_name, sizeof(sem_name), "%s%s", SEM_PREFIX, bus_name);
    sem = sem_open(sem_name, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        detach_event_bus();
        return false;
    }
    
    strncpy(current_bus_name, bus_name, MAX_BUS_NAME - 1);
    current_bus_name[MAX_BUS_NAME - 1] = '\0';
    
    return true;
}

void eventbus_disconnect() {
    detach_event_bus();
    if (sem != SEM_FAILED) {
        sem_close(sem);
    }
    memset(current_bus_name, 0, sizeof(current_bus_name));
}

int eventbus_create_topic(const char* topic_name) {
    if (!bus || sem_wait(sem) == -1) {
        return -1;
    }
    
    int result = -1;
    if (bus->topic_count < MAX_TOPICS) {
        for (int i = 0; i < bus->topic_count; i++) {
            if (strcmp(bus->topics[i].name, topic_name) == 0) {
                result = i;
                break;
            }
        }
        if (result == -1) {
            strncpy(bus->topics[bus->topic_count].name, topic_name, MAX_TOPIC_NAME - 1);
            bus->topics[bus->topic_count].name[MAX_TOPIC_NAME - 1] = '\0';
            bus->topics[bus->topic_count].subscriber_count = 0;
            result = bus->topic_count++;
        }
    }
    
    sem_post(sem);
    return result;
}

int eventbus_subscribe_to_topic(const char* topic_name, int subscriber_id) {
    if (!bus || sem_wait(sem) == -1) {
        return -1;
    }
    
    int result = -1;
    for (int i = 0; i < bus->topic_count; i++) {
        if (strcmp(bus->topics[i].name, topic_name) == 0) {
            if (bus->topics[i].subscriber_count < MAX_SUBSCRIBERS) {
                bus->topics[i].subscribers[bus->topics[i].subscriber_count].subscriber_id = subscriber_id;
                bus->topics[i].subscribers[bus->topics[i].subscriber_count].active = true;
                bus->topics[i].subscriber_count++;
                result = 0;
            }
            break;
        }
    }
    
    sem_post(sem);
    return result;
}

int eventbus_publish_message(const char* topic_name, const char* message) {
    if (!bus || sem_wait(sem) == -1) {
        return -1;
    }
    
    int result = -1;
    for (int i = 0; i < bus->topic_count; i++) {
        if (strcmp(bus->topics[i].name, topic_name) == 0) {
            time_t now = time(NULL);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
            snprintf(bus->topics[i].message, MAX_MESSAGE_SIZE, "%s [%s]", message, time_str);
            result = 0;
            break;
        }
    }
    
    sem_post(sem);
    return result;
}

char* eventbus_read_message(const char* topic_name) {
    if (!bus || sem_wait(sem) == -1) {
        return NULL;
    }
    
    char* result = NULL;
    for (int i = 0; i < bus->topic_count; i++) {
        if (strcmp(bus->topics[i].name, topic_name) == 0) {
            result = bus->topics[i].message;
            break;
        }
    }
    
    sem_post(sem);
    return result;
}

void eventbus_cleanup() {
    eventbus_disconnect();
    char sem_name[64];
    snprintf(sem_name, sizeof(sem_name), "%s%s", SEM_PREFIX, current_bus_name);
    sem_unlink(sem_name);
}