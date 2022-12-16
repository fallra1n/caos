#!/bin/bash

gcc file-list-summ-size.c -o res.o

touch a.txt
touch b.txt

echo aaaa > a.txt
echo bbbb > b.txt

echo
echo ========================OUTPUT===========================
echo

./res.o

rm -rf *.o
rm -rf *.txt