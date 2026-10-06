CC = gcc
CFLAGS = -Wall -Wextra -O3 -I.

SRCS := $(wildcard *.c)
BINS := $(SRCS:.c=)

.PHONY: all clean

all: clean $(BINS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BINS)
