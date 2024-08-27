#include "eventbus.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void init_eventbus() {
    init_shared_memory();
}

void cleanup_eventbus() {
    destroy_shared_memory();
}

void publish_event(const char* topic, const char* data) {
    SharedMemory* shm = get_shared_memory();
    shm->publisher_count++;
    
    // Check if the topic already exists
    int topic_index = -1;
    for (int i = 0; i < shm->topic_count; i++) {
        if (strcmp(shm->topics[i], topic) == 0) {
            topic_index = i;
            break;
        }
    }
    
    // If the topic doesn't exist, add it
    if (topic_index == -1 && shm->topic_count < MAX_TOPICS) {
        strncpy(shm->topics[shm->topic_count], topic, MAX_TOPIC_LENGTH - 1);
        shm->topics[shm->topic_count][MAX_TOPIC_LENGTH - 1] = '\0';
        topic_index = shm->topic_count;
        shm->topic_count++;
    }
    
    if (topic_index != -1) {
        snprintf(shm->data, SHM_SIZE, "%s:%s", topic, data);
        shm->new_data = true;
        printf("Published event: %s\n", shm->data);
    } else {
        printf("Failed to publish event: too many topics\n");
    }
}

void subscribe_event(const char* topic, void (*callback)(const char*, const char*)) {
    SharedMemory* shm = get_shared_memory();
    shm->subscriber_count++;
    printf("Subscribed to topic: %s\n", topic);
    
    while (1) {
        if (shm->new_data) {
            char *colon = strchr(shm->data, ':');
            if (colon != NULL) {
                *colon = '\0';
                if (strcmp(shm->data, topic) == 0) {
                    callback(topic, colon + 1);
                }
                *colon = ':';
            }
            shm->new_data = false;
        }
        usleep(100000);  // Sleep for 100ms to reduce CPU usage
    }
}
#include "eventbus.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define SHM_KEY 1234
#define SEM_KEY 5678

typedef struct {
    char name[MAX_TOPIC_LENGTH];
    char publishers[MAX_PUBLISHERS][MAX_PUBLISHER_LENGTH];
    int publisher_count;
    char subscribers[MAX_SUBSCRIBERS][MAX_SUBSCRIBER_LENGTH];
    int subscriber_count;
    char data[MAX_DATA_LENGTH];
    bool new_data;
    pid_t last_publisher_pid;
} Topic;

typedef struct {
    Topic topics[MAX_TOPICS];
    int topic_count;
    pthread_mutex_t mutex;
} SharedMemory;

struct EventBus {
    int shmid;
    SharedMemory* shm;
    int semid;
};

static void sem_lock(int semid) {
    struct sembuf sb = {0, -1, SEM_UNDO};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop lock");
        exit(1);
    }
}

static void sem_unlock(int semid) {
    struct sembuf sb = {0, 1, SEM_UNDO};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop unlock");
        exit(1);
    }
}

EventBus* eventbus_create() {
    EventBus* bus = (EventBus*)malloc(sizeof(EventBus));
    if (!bus) {
        perror("malloc");
        return NULL;
    }

    // Create or get shared memory
    bus->shmid = shmget(SHM_KEY, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (bus->shmid == -1) {
        perror("shmget");
        free(bus);
        return NULL;
    }

    // Attach to shared memory
    bus->shm = (SharedMemory*)shmat(bus->shmid, NULL, 0);
    if (bus->shm == (void*)-1) {
        perror("shmat");
        free(bus);
        return NULL;
    }

    // Create or get semaphore
    bus->semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (bus->semid == -1) {
        perror("semget");
        shmdt(bus->shm);
        free(bus);
        return NULL;
    }

    // Initialize shared memory and mutex if we're the first process
    if (errno == ENOENT) {
        memset(bus->shm, 0, sizeof(SharedMemory));
        
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&bus->shm->mutex, &attr);
        pthread_mutexattr_destroy(&attr);

        // Initialize semaphore
        if (semctl(bus->semid, 0, SETVAL, 1) == -1) {
            perror("semctl");
            shmdt(bus->shm);
            free(bus);
            return NULL;
        }
    }

    return bus;
}

void eventbus_destroy(EventBus* bus) {
    if (bus) {
        if (bus->shm != (void*)-1) {
            shmdt(bus->shm);
        }
        free(bus);
    }
}

int eventbus_add_topic(EventBus* bus, const char* topic, const char* publisher) {
    if (!bus || !bus->shm) return -1;

    sem_lock(bus->semid);
    for (int i = 0; i < MAX_TOPICS; i++) {
        if (bus->shm->topics[i].name[0] == '\0') {
            strncpy(bus->shm->topics[i].name, topic, MAX_TOPIC_LENGTH - 1);
            bus->shm->topics[i].name[MAX_TOPIC_LENGTH - 1] = '\0';
            eventbus_add_publisher(bus, topic, publisher);
            bus->shm->topic_count++;
            sem_unlock(bus->semid);
            return 0;
        }
    }
    sem_unlock(bus->semid);
    return -1;
}

int eventbus_add_publisher(EventBus* bus, const char* topic, const char* publisher) {
    if (!bus || !bus->shm) return -1;

    sem_lock(bus->semid);
    for (int i = 0; i < MAX_TOPICS; i++) {
        if (strcmp(bus->shm->topics[i].name, topic) == 0) {
            for (int j = 0; j < MAX_PUBLISHERS; j++) {
                if (bus->shm->topics[i].publishers[j][0] == '\0') {
                    strncpy(bus->shm->topics[i].publishers[j], publisher, MAX_PUBLISHER_LENGTH - 1);
                    bus->shm->topics[i].publishers[j][MAX_PUBLISHER_LENGTH - 1] = '\0';
                    bus->shm->topics[i].publisher_count++;
                    sem_unlock(bus->semid);
                    return 0;
                }
            }
            sem_unlock(bus->semid);
            return -1;
        }
    }
    sem_unlock(bus->semid);
    return -1;
}

int eventbus_add_subscriber(EventBus* bus, const char* topic, const char* subscriber) {
    if (!bus || !bus->shm) return -1;

    sem_lock(bus->semid);
    for (int i = 0; i < MAX_TOPICS; i++) {
        if (strcmp(bus->shm->topics[i].name, topic) == 0) {
            for (int j = 0; j < MAX_SUBSCRIBERS; j++) {
                if (bus->shm->topics[i].subscribers[j][0] == '\0') {
                    strncpy(bus->shm->topics[i].subscribers[j], subscriber, MAX_SUBSCRIBER_LENGTH - 1);
                    bus->shm->topics[i].subscribers[j][MAX_SUBSCRIBER_LENGTH - 1] = '\0';
                    bus->shm->topics[i].subscriber_count++;
                    sem_unlock(bus->semid);
                    return 0;
                }
            }
            sem_unlock(bus->semid);
            return -1;
        }
    }
    sem_unlock(bus->semid);
    return -1;
}

int eventbus_publish(EventBus* bus, const char* topic, const char* data) {
    if (!bus || !bus->shm) return -1;

    sem_lock(bus->semid);
    for (int i = 0; i < MAX_TOPICS; i++) {
        if (strcmp(bus->shm->topics[i].name, topic) == 0) {
            strncpy(bus->shm->topics[i].data, data, MAX_DATA_LENGTH - 1);
            bus->shm->topics[i].data[MAX_DATA_LENGTH - 1] = '\0';
            bus->shm->topics[i].new_data = true;
            bus->shm->topics[i].last_publisher_pid = getpid();
            sem_unlock(bus->semid);
            return 0;
        }
    }
    sem_unlock(bus->semid);
    return -1;
}

int eventbus_subscribe(EventBus* bus, const char* topic, void (*callback)(const char*, const char*)) {
    if (!bus || !bus->shm) return -1;

    while (1) {
        sem_lock(bus->semid);
        for (int i = 0; i < MAX_TOPICS; i++) {
            if (strcmp(bus->shm->topics[i].name, topic) == 0) {
                if (bus->shm->topics[i].new_data) {
                    callback(topic, bus->shm->topics[i].data);
                    bus->shm->topics[i].new_data = false;
                }
                break;
            }
        }
        sem_unlock(bus->semid);
        usleep(100000);  // Sleep for 100ms to reduce CPU usage
    }
    return 0;
}

void eventbus_list_topics(EventBus* bus) {
    if (!bus || !bus->shm) return;

    sem_lock(bus->semid);
    printf("Topics:\n");
    for (int i = 0; i < MAX_TOPICS; i++) {
        if (bus->shm->topics[i].name[0] != '\0') {
            printf("- %s\n", bus->shm->topics[i].name);
            printf("  Publishers:\n");
            for (int j = 0; j < MAX_PUBLISHERS; j++) {
                if (bus->shm->topics[i].publishers[j][0] != '\0') {
                    printf("    - %s\n", bus->shm->topics[i].publishers[j]);
                }
            }
            printf("  Subscribers:\n");
            for (int j = 0; j < MAX_SUBSCRIBERS; j++) {
                if (bus->shm->topics[i].subscribers[j][0] != '\0') {
                    printf("    - %s\n", bus->shm->topics[i].subscribers[j]);
                }
            }
        }
    }
    sem_unlock(bus->semid);
}