#!/bin/bash

gcc test_gen.c -o testing.o
touch input.bin
./testing.o input.bin
mv input.bin ../../build

rm -rf *.o