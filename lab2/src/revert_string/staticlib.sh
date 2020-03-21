#!/bin/bash
gcc -c revert_string.c -o revert_string
ar rcs libstatic_revert.a revert_string
gcc main.c -L. -lstatic_revert -o static_main