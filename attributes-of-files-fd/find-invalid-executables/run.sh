#!/bin/bash

gcc find-invalid-executables.c -o res.o

touch a.txt
touch b.txt

chmod +x a.txt
chmod +x b.txt

echo
echo ========================OUTPUT===========================
echo

./res.o

rm -rf *.o
rm -rf *.txt