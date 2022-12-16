all: clean testing.o intro-64.o result.o
.PHONY: all

testing.o:
	gcc -c testing.c -o testing.o

intro-64.o:
	gcc -c -msse intro-64.s -o intro-64.o

result.o:
	gcc -o result.o testing.o intro-64.o

clean:
	rm -rf testing.o
	rm -rf intro-64.o
	rm -rf result.o
