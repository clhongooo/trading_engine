#!/bin/bash

CORE_MINUS_1=$(cat /proc/cpuinfo | grep processor | tail -n 1 | awk '{print $3}')
BUILD_FOLDER=target
BIN_FOLDER=bin
if [[ $1 == *"c"* && -d $BUILD_FOLDER ]]
then
    rm -rf $BUILD_FOLDER
fi
[[ ! -d $BUILD_FOLDER ]] && mkdir $BUILD_FOLDER
[[ ! -d $BIN_FOLDER ]] && mkdir $BIN_FOLDER
cd $BUILD_FOLDER

if [[ $1 == *"d"* ]]
then
    cmake -DCMAKE_BUILD_TYPE=Debug ../src/main/
elif [[ $1 == *"r"* ]]
then
    cmake -DCMAKE_BUILD_TYPE=Release ../src/main/
fi
MAKE_VERBOSITY=""
if [[ $@ == *"v"* ]]
then
    MAKE_VERBOSITY="VERBOSE=1"
fi
make -j $CORE_MINUS_1 $MAKE_VERBOSITY
find . -type f -executable | grep -v CMakeFiles | xargs -I{} cp {} ../$BIN_FOLDER
