.SILENT: all, build

all: build

build:
	nasm -f elf64 sum.s -o sum.o
	gcc main.c sum.o -o main
	rm -rf *.o
