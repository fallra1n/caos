#!/bin/bash

gcc manage-symlinks.c -o result.o

echo =========CREATE REGULAR FILE=========================

touch a.txt
echo aaaaaaaaaaa > a.txt

echo =========CREATE SYMBOLIC LINK========================

ln -s a.txt b.txt

./result.o

rm -rf *.txt
rm -rf *.o