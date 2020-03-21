#!/bin/bash
gcc -o librevert.so -shared -fPIC revert_string.c
export LD_LIBRARY_PATH=$(pwd)
gcc main.c -L. -lrevert -o dinamic_reverse
