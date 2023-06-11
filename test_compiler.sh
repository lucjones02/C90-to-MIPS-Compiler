#!/bin/bash

make clean
make bin/c_compiler -B

CHECKED=0
PASSED=0
COMPILED=0

rm -rf test_outputs
mkdir test_outputs

for i in compiler_tests/*; do
    FUNCTIONALITY=$(basename ${i})
    mkdir test_outputs/${FUNCTIONALITY}
    for TEST in ${i}/*_driver.c; do
        echo "----------------------------------------------------------------------------------------------"
        NAME=$(basename ${TEST} _driver.c)
        mkdir test_outputs/${FUNCTIONALITY}/${NAME}
        TEST_PROGRAM="test_outputs/${FUNCTIONALITY}/${NAME}/${NAME}"
        bin/c_compiler -S compiler_tests/${FUNCTIONALITY}/${NAME}.c -o ${TEST_PROGRAM}.s 2> /dev/null
        RES=$?
        if [[ RES -ne 0 ]]; then
            echo "Test ${NAME} did NOT COMPILE when testing ${FUNCTIONALITY}"
        else 
            COMPILED=$(( ${COMPILED}+1 ))
            mips-linux-gnu-gcc -mfp32 -o ${TEST_PROGRAM}.o -c ${TEST_PROGRAM}.s
            RES=$?
            if [[ RES -ne 0 ]]; then
                echo "Test ${NAME} did not convert from MIPS to binary when testing ${FUNCTIONALITY}"
            else 
                mips-linux-gnu-gcc -mfp32 -static -o ${TEST_PROGRAM} ${TEST_PROGRAM}.o ${TEST}
                RES=$?
                if [[ RES -ne 0 ]]; then
                    echo "Test ${NAME} did not link with driver program when testing ${FUNCTIONALITY}"
                else
                    qemu-mips ${TEST_PROGRAM}
                    RES=$?
                    if [[ RES -eq 0 ]]; then
                        echo "Test ${NAME} RAN SUCCESSFULLY when testing ${FUNCTIONALITY}"
                        PASSED=$(( ${PASSED}+1 ))
                    else 
                        echo "Test ${NAME} FAILED when testing ${FUNCTIONALITY}"
                    fi
                fi
            fi
        fi
        CHECKED=$(( ${CHECKED}+1 ))
    done
done

PERSONAL=0
PERSONAL_PASS=0
for i in compiler_tests/personal_tests/*_driver.c; do
    FUNCTIONALITY="personal_tests"
    NAME=$(basename ${i} _driver.c)
    TEST_PROGRAM="test_outputs/${FUNCTIONALITY}/${NAME}/${NAME}"
    qemu-mips ${TEST_PROGRAM}
    RES=$?
    if [[ RES -eq 0 ]]; then
        PERSONAL_PASS=$(( ${PERSONAL_PASS}+1 ))
    fi

    PERSONAL=$(( ${PERSONAL}+1 ))
done

echo "----------------------------------------------------------------------------------------------"
echo "The compiler compiled ${COMPILED} programs"
echo "The compiler passed ${PERSONAL_PASS} out of ${PERSONAL} added tests"
echo "The compiler passed $(expr ${PASSED} - ${PERSONAL_PASS}) out of $(expr ${CHECKED} - ${PERSONAL}) given tests"

make clean > /dev/null