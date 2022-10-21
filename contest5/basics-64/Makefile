all: clean testing.o basics-64.o result.o
.PHONY: all

testing.o:
	gcc -fPIE -c testing.c -o testing.o

basics-64.o:
	gcc  -fPIE -c basics-64.s -o basics-64.o

result.o:
	gcc -fPIE -o result.o testing.o basics-64.o

clean:
	rm -rf testing.o
	rm -rf basics-64.o
	rm -rf result.o
