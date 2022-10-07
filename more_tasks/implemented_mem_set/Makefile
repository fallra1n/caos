all: clean testing.o eqsolution-64.o result.o
.PHONY: all

SHELL:=/bin/bash
.ONESHELL:

testing.o:
	aarch64-linux-gnu-gcc -c testing.c -o testing.o

eqsolution-64.o:
	aarch64-linux-gnu-gcc -c implemented_mem_set.s -o implemented_mem_set.o

result.o:
	aarch64-linux-gnu-gcc -o result.o testing.o implemented_mem_set.o

clean:
	rm -rf testing.o
	rm -rf implemented_mem_set.o
	rm -rf result.o
