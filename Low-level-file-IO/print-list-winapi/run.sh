#!/bin/bash

make

echo
echo ========================OUTPUT===========================
echo

touch input.bin
./testing.o input.bin
WINEDEBUG=-all wine a.exe input.bin

rm -rf *.o
rm -rf *.exe
rm -rf input.bin