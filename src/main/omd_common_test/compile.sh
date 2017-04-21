#!/bin/bash

CURDIR=$(pwd)

if [[ $# -eq 0 ]]
then
    cd ../omd_common/Debug
    make all
    cd $CURDIR/Debug
    rm -f omd_common_testing Main*
    make all
    exit
elif [[ $1 == "a" ]]
then
    cd Debug && make clean && make
    exit
elif [[ $1 == "aa" ]]
then
    cd ../omd_common/Debug
    make clean && make
    cd $CURDIR/Debug && make clean && make
    exit
fi

####################################################
## Compile
####################################################
cd $CURDIR/$TARGETDIR
rm -f "$TARGETCLS".o "$TARGETCLS".d
cd $CURDIR/Debug
rm -f omd_common_testing
make all
