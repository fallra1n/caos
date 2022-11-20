#!/bin/bash

make

echo
echo ========================OUTPUT===========================
echo

touch input.bin
touch input_copy.bin
./testing.o input.bin
cp input.bin input_copy.bin
./sort-file-contents.o input.bin

rm -rf *.o
rm -rf *.bin
rm -rf [0-9]*