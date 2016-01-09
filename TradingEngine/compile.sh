#!/bin/bash
CURDIR=$(pwd)

if [[ $1 == "aa" ]]
then
    cd $CURDIR/../NirvanaCommon/Debug && rm -f *.a
    if [[ $2 == "c" ]]
    then
        make clean
        make -j2 all
    else
        make -j2 all
    fi
fi

if [[ $1 == "aa" || $1 == "a" ]]
then
    cd $CURDIR/../NirvanaInfrastructure/Debug && rm -f *.a
    if [[ $2 == "c" ]]
    then
        make clean
        make -j2 all
    else
        make -j2 all
    fi
fi

cd $CURDIR/Debug && rm -f TradingEngine && make clean && make -j6 all
