CC = gcc
CFLAGS = -Wall -Wextra

all: publisher subscriber

publisher: src/publisher.c src/eventbus.c src/shared_memory.c
    $(CC) $(CFLAGS) -o publisher src/publisher.c src/eventbus.c src/shared_memory.c

subscriber: src/subscriber.c src/eventbus.c src/shared_memory.c
    $(CC) $(CFLAGS) -o subscriber src/subscriber.c src/eventbus.c src/shared_memory.c

clean:
    rm -f publisher subscriber