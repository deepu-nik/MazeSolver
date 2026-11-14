CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -O2
RAYLIB_CFLAGS ?= $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS ?= $(shell pkg-config --libs raylib 2>/dev/null)
all: tripwise
tripwise: main.c optimizer.c optimizer.h
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) main.c optimizer.c -o tripwise $(RAYLIB_LIBS)
test: tests/test_optimizer.c optimizer.c optimizer.h
	$(CC) $(CFLAGS) tests/test_optimizer.c optimizer.c -o test_optimizer
	./test_optimizer
clean:
	rm -f tripwise test_optimizer
