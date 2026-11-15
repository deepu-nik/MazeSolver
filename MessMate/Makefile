CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -O2
RAYLIB_CFLAGS ?= $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS ?= $(shell pkg-config --libs raylib 2>/dev/null)
all: messmate
messmate: main.c scheduler.c scheduler.h
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) main.c scheduler.c -o messmate $(RAYLIB_LIBS)
test: tests/test_scheduler.c scheduler.c scheduler.h
	$(CC) $(CFLAGS) tests/test_scheduler.c scheduler.c -o test_scheduler
	./test_scheduler
clean:
	rm -f messmate test_scheduler
