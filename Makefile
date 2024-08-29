CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lrt -pthread

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++14

SRCDIR = src
EXMDIR = examples
TESTDIR = test

all: init_eventbus publisher_interative subscriber_interative publisher_command subscriber_command test_eventbus

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

test_eventbus: $(TESTDIR)/test_eventbus.cpp $(SRCDIR)/eventbus.c $(SRCDIR)/eventbus.h
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lgtest -lgtest_main -lpthread

run_test: test_eventbus
	./test_eventbus  ||  true
	

clean:
	rm -f init_eventbus publisher_interative subscriber_interative publisher_command subscriber_command test_eventbus