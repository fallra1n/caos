all: clean testing.o arrays-64.o result.o
.PHONY: all

testing.o:
	gcc -c testing.c -o testing.o

arrays-64.o:
	gcc -c arrays-64.s -o arrays-64.o

result.o:
	gcc -o result.o testing.o arrays-64.o

clean:
	rm -rf testing.o
	rm -rf arrays-64.o
	rm -rf result.o
