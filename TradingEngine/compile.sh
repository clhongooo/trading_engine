#!/bin/bash
CURDIR=$(pwd)

MAKE_JOB=$(expr $(nproc) - 1)

if [[ $1 == "aa" ]]
then
    cd $CURDIR/../NirvanaCommon/Debug && rm -f *.a
    if [[ $2 == "c" ]]
    then
        make clean
        make -j$MAKE_JOB all
    else
        make -j$MAKE_JOB all
    fi
fi

if [[ $1 == "aa" || $1 == "a" ]]
then
    cd $CURDIR/../NirvanaInfrastructure/Debug && rm -f *.a
    if [[ $2 == "c" ]]
    then
        make clean
        make -j$MAKE_JOB all
    else
        make -j$MAKE_JOB all
    fi
fi

cd $CURDIR/Debug && rm -f TradingEngine && make clean && make -j$MAKE_JOB all
