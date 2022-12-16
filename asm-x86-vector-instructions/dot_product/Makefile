all: clean testing.o dot_product.o result.o
.PHONY: all

testing.o:
	gcc -c testing.c -o testing.o

dot_product.o:
	gcc -c dot_product.s -o dot_product.o

result.o:
	gcc -o result.o testing.o dot_product.o

clean:
	rm -rf testing.o
	rm -rf dot_product.o
	rm -rf result.o
