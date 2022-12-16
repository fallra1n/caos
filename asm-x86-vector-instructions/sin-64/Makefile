all: clean testing.o sin-64.o result.o
.PHONY: all

testing.o:
	gcc -c testing.c -o testing.o

sin-64.o:
	gcc -c sin-64.s -o sin-64.o

result.o:
	gcc -o result.o testing.o sin-64.o

clean:
	rm -rf testing.o
	rm -rf sin-64.o
	rm -rf result.o
