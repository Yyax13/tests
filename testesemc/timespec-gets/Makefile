CC := gcc
CC_FLAGS := -O3
I_FLAGS := -I.

SRCS := $(wildcard *.c)
BINS := $(patsubst %.c,%,$(SRCS))

.DEFAULT_GOAL := all

.PHONY: all
all: $(BINS)

%: %.c
	$(CC) $(CC_FLAGS) $(I_FLAGS) $< -o $@
