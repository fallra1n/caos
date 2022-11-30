all: clean testing.o source.o result.o
.PHONY: all

testing.o:
	gcc -c testing.c -o testing.o

source.o:
	gcc -c normalize-path.c -o source.o

result.o:
	gcc testing.o source.o -o result.o

clean:
	rm -rf *.o
