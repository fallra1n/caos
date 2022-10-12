all: clean testing.o format_io-64 result.o
.PHONY: all


testing.o:
	aarch64-linux-gnu-gcc -c testing.c -o testing.o

format_io-64:
	aarch64-linux-gnu-gcc -c format_io-64.s -o format_io-64.o

result.o:
	aarch64-linux-gnu-gcc -o result.o testing.o format_io-64.o

clean:
	rm -rf testing.o
	rm -rf format_io-64.o
	rm -rf result.o
