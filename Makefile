CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -pthread

all: publisher subscriber

publisher: src/publisher.c src/eventbus.c
	$(CC) $(CFLAGS) -o publisher src/publisher.c src/eventbus.c $(LDFLAGS)

subscriber: src/subscriber.c src/eventbus.c
	$(CC) $(CFLAGS) -o subscriber src/subscriber.c src/eventbus.c $(LDFLAGS)

clean:
	rm -f publisher subscriber