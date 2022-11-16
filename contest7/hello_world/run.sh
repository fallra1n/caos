#!/bin/bash

make

echo
echo ===============================================
echo

./result.o
rm -rf copy_stream.o
rm -rf syscall.o
rm -rf result.o
