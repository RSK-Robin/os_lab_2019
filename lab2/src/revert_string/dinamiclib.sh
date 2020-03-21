#!/bin/bash
gcc -fPIC -c revert_string.c -o revert_string.o
gcc -shared revert_string.o -o librevert.so
gcc main.c -L. -lrevert -o dinamic_reverse
export LD_LIBRARY_PATH=$(/projects/os_lab_2019/lab2/src/revert_string)
rm *.o
