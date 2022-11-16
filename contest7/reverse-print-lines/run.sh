#!/bin/bash

make

echo
echo ===================================================
echo

./result.o
rm -rf reverse-print-lines.o
rm -rf syscall.o
rm -rf result.o
