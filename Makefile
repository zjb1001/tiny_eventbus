CC = gcc
CFLAGS = -Wall -Wextra

all: publisher subscriber

publisher: src/publisher.c src/eventbus.c src/shm_manager.c
	$(CC) $(CFLAGS) -o publisher src/publisher.c src/eventbus.c src/shm_manager.c

subscriber: src/subscriber.c src/eventbus.c src/shm_manager.c
	$(CC) $(CFLAGS) -o subscriber src/subscriber.c src/eventbus.c src/shm_manager.c

clean:
	rm -f publisher subscriber