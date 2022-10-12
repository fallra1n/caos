all: clean testing.o globalvars-64.o result.o
.PHONY: all


testing.o:
	aarch64-linux-gnu-gcc -c testing.c -o testing.o

globalvars-64.o:
	aarch64-linux-gnu-gcc -c globalvars-64.s -o globalvars-64.o

result.o:
	aarch64-linux-gnu-gcc -o result.o testing.o globalvars-64.o

clean:
	rm -rf testing.o
	rm -rf globalvars-64.o
	rm -rf result.o
