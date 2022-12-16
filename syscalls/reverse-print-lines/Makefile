all: syscall.o reverse-print-lines.o result.o
.PHONY: all

reverse-print-lines.o:
	gcc -c -nostdlib -fno-stack-protector reverse-print-lines.S -o reverse-print-lines.o

syscall.o:
	gcc -c -nostdlib -fno-stack-protector syscall.S -o syscall.o

result.o:
	gcc reverse-print-lines.o syscall.o -nostdlib -fno-stack-protector -o result.o
