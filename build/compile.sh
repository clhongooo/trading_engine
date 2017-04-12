#!/bin/bash

CORE_MINUS_1=$(cat /proc/cpuinfo | grep processor | tail -n 1 | awk '{print $3}')

if [[ $1 == "c" ]]
then
    ls | grep -v compile.sh | xargs rm -rf
fi
cmake ../src/main/
make -j $CORE_MINUS_1 VERBOSE=1
