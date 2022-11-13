all: clean syscall.o hello_world.o result.o
.PHONY: all

hello_world.o:
	gcc -c -nostdlib hello_world.c -o hello_world.o

syscall.o:
	gcc -c -nostdlib syscall.S -o syscall.o

result.o:
	gcc hello_world.o syscall.o -nostdlib -o result.o

clean:
	rm -rf hello_world.o
	rm -rf syscall.o
	rm -rf result.o
