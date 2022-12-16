all: clean syscall.o copy_stream.o result.o
.PHONY: all

copy_stream.o:
	gcc -c -nostdlib -fno-stack-protector copy_stream.S -o copy_stream.o

syscall.o:
	gcc -c -nostdlib -fno-stack-protector syscall.S -o syscall.o

result.o:
	gcc copy_stream.o syscall.o -nostdlib -fno-stack-protector -o result.o

clean:
	rm -rf copy_stream.o
	rm -rf syscall.o
	rm -rf result.o
