.SILENT: all, build

all: build

build:
	nasm -f elf64 -o main.o main.s
	ld -o main main.o
	rm -rf main.o
