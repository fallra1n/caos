#!/bin/bash

make

echo
echo ========================OUTPUT===========================
echo

touch input.bin
./testing.o input.bin
./print-list-using-mmap.o input.bin

rm -rf *.o
rm -rf input.bin