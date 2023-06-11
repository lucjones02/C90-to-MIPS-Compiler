#!/bin/bash

rm -rf test.o test.s test

git pull
make clean
make bin/c_compiler -B

bin/c_compiler -S test_program.c -o test.s
mips-linux-gnu-gcc -mfp32 -o test.o -c test.s
mips-linux-gnu-gcc -mfp32 -static -o test test.o test_program_driver.c
qemu-mips test
echo $?