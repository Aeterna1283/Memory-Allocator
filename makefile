CFLAGS=-O2 -Wall -std=c2x

.PHONY: clean

all: clean alloc

alloc: alloc.o heap.o
	gcc $(CFLAGS) $^ -o $@

alloc.o: alloc.c alloc.h
	gcc $(CFLAGS) -c $<

heap.o: heap.asm
	nasm -f elf64 $^

clean:
	rm -f *.o alloc
