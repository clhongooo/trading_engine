#!/bin/bash
CURDIR=$(pwd)

#if [[ $1 == "a" ]]
#then
    cd $CURDIR/Debug && make clean && make -j6 all
#else
#    cd $CURDIR/Debug && make -j6 all
#fi
