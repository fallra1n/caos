all: clean testing.o char-filter-digits-64 result.o
.PHONY: all

testing.o:
	aarch64-linux-gnu-gcc -c testing.c -o testing.o

char-filter-digits-64:
	aarch64-linux-gnu-gcc -c char-filter-digits-64.s -o char-filter-digits-64.o

result.o:
	aarch64-linux-gnu-gcc -o result.o testing.o char-filter-digits-64.o

clean:
	rm -rf testing.o
	rm -rf char-filter-digits-64.o
	rm -rf result.o
