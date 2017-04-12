#!/bin/bash

CORE_MINUS_1=$(cat /proc/cpuinfo | grep processor | tail -n 1 | awk '{print $3}')


ls | grep -v compile.sh | xargs rm -rf
cmake ../src/main/
make -j $CORE_MINUS_1
