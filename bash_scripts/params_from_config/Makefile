all: clean program.exe
.PHONY: all

SHELL:=/bin/bash
.ONESHELL:

lines = ''
lines = $(shell sed -ne 's/^[1-9a-zA-Z_]*=/:/p' compile.conf)

program.exe:
	gcc -D'Lines="$(lines)"' solution.c -o program.exe

clean:
	rm -rf program.exe
