CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lrt -pthread

SRCDIR = src
EXMDIR = examples

all: init_eventbus publisher_interative subscriber_interative publisher_command subscriber_command

init_eventbus: $(SRCDIR)/init_eventbus.c $(SRCDIR)/eventbus.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

publisher_interative: $(EXMDIR)/publisher_interative.c $(SRCDIR)/eventbus.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

subscriber_interative: $(EXMDIR)/subscriber_interative.c $(SRCDIR)/eventbus.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

publisher_command: $(EXMDIR)/publisher_command.c $(SRCDIR)/eventbus.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

subscriber_command: $(EXMDIR)/subscriber_command.c $(SRCDIR)/eventbus.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f init_eventbus publisher_interative subscriber_interative publisher_command subscriber_command