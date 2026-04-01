.SILENT: test
.PHONY: test build

SRC := 	main.c \
		utils/main.c

INCLUDE := 	\
			-Iutils

TARGET := build/howo-not-to-simple-keygen
CC := gcc
CC_FLAGS := \
    -O3 \
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
    -fstack-reuse=all \
    -flto \
    -Wl,--strip-all \
    -Wl,--gc-sections \
    -static \
    -fPIE \
    -pie \
    -fstack-protector-all

all: build

build:
	mkdir -p build
	$(CC) $(SRC) -o $(TARGET) $(INCLUDE) $(CC_FLAGS)

TMP_TARGET := /tmp/build_ntsk/ntsk

test:
	mkdir -p /tmp/build_ntsk
	$(CC) $(SRC) -o $(TMP_TARGET) $(INCLUDE) $(CC_FLAGS)
	$(TMP_TARGET)
	rm -rf $(TMP_TARGET)

POC_TARGET := ./build/poc
POC_SRC := 	poc.c \
		utils/main.c

poc-build:
	mkdir -p build
	$(CC) $(POC_SRC) -o $(POC_TARGET) $(INCLUDE) $(CC_FLAGS)