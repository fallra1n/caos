all: testing.o print-list-winapi.o
.PHONY: all

testing.o:
	gcc test_gen.c -o testing.o

print-list-winapi.o:
	i686-w64-mingw32-gcc print-list-winapi.c
