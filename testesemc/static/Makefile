.SILENT: test build
.PHONY: test build

SRC := "main"
TARGET := build/static-tests
CC := gcc
CC_FLAGS := \
	-c \
    -O3 \
    -flto \
    -fvisibility=hidden \
    -fno-ident \
    -fmerge-all-constants \
    -fno-asynchronous-unwind-tables \
    -fno-unwind-tables \
    -funroll-loops \
    -finline-functions \
    -finline-small-functions \
    -fomit-frame-pointer \
    -fno-builtin \
    -fno-plt \
    -fno-exceptions \
    -fdata-sections \
    -ffunction-sections \
    -fstack-reuse=all 

LD_FLAGS := \
    -flto \
    -Wl,--strip-all \
    -Wl,--gc-sections \
	-static

all: build

build:
	mkdir -p build
	$(CC) $(SRC).c -o $(SRC).o $(CC_FLAGS)
	$(CC) $(SRC).o -o $(TARGET) $(LD_FLAGS)
	rm -rf $(SRC).o

$TMP_TARGET := /tmp/build_ntsk/ntsk

test:
	mkdir -p /tmp/build_ntsk
	$(CC) $(SRC).c -o $(SRC).o $(CC_FLAGS)
	$(CC) $(SRC).o -o $(TMP_TARGET) $(LD_FLAGS)
	$(TMP_TARGET)
	rm -rf $(SRC).o $(TMP_TARGET) /tmp/build_ntsk
