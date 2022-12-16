#!/bin/bash

touch input.txt

echo asadbek 18 years old > input.txt

gcc read-filter-write.c -o result.o
./result.o input.txt output1.txt output2.txt

echo DIGITS
cat output1.txt
echo

echo ===================================

echo NODIGITS
cat output2.txt
echo

rm -rf *.txt
rm -rf *.o