.SILENT: all, build

all: build

build:
	nasm -f elf64 hash.s -o hash.o
	gcc main.c hash.o -o main
	rm -rf *.o
