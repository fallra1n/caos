all: clean testing.o reverse-print-chars-64.o result.o
.PHONY: all

testing.o:
	aarch64-linux-gnu-gcc -c testing.c -o testing.o

reverse-print-chars-64.o:
	aarch64-linux-gnu-gcc -c reverse-print-chars-64.s -o reverse-print-chars-64.o

result.o:
	aarch64-linux-gnu-gcc -o result.o testing.o reverse-print-chars-64.o

clean:
	rm -rf testing.o
	rm -rf reverse-print-chars-64.o
	rm -rf result.o
