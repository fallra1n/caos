#!/bin/bash

make

echo
echo ==================-SUCCESSFULL COMPILATION-==================
echo

./result.o
rm -rf copy_stream.o
rm -rf syscall.o
rm -rf result.o
