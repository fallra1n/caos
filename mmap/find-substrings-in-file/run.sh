#!/bin/bash

gcc find-substrings-in-file.c -o res.o

touch a.txt
echo aaa cbb cc aaafff aafff baaaafff > a.txt

./res.o a.txt aaa

rm -rf res.o
rm -rf a.txt