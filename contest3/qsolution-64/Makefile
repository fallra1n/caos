all: clean testing.o eqsolution-64.o result.o
.PHONY: all

SHELL:=/bin/bash
.ONESHELL:

testing.o:
	aarch64-linux-gnu-gcc -c testing.c -o testing.o

eqsolution-64.o:
	aarch64-linux-gnu-gcc -c eqsolution-64.s -o eqsolution-64.o

result.o:
	aarch64-linux-gnu-gcc -o result.o testing.o eqsolution-64.o

clean:
	rm -rf testing.o
	rm -rf eqsolution-64.o
	rm -rf result.o
