#!/bin/bash

CURDIR=$(pwd)

if [[ $(ifconfig | grep 192.168.91.8\    | wc -l) -gt 0 ]]; then source changeMkflPath918; cat Config-Readiness-SP-918.ini > Config.ini; fi
if [[ $(ifconfig | grep 192.168.91.95    | wc -l) -gt 0 ]]; then source changeMkflPath9195; cat Config-Readiness-SP-9195.ini > Config.ini; fi
if [[ $(ifconfig | grep 192.168.91.132   | wc -l) -gt 0 ]]; then cat Config-Readiness-SP-132.ini > Config.ini; fi

if [[ $# -eq 0 ]]
then
    cd ../omd_common/Debug
    rm -f libomd_common.a    ### DataProcessing/RefreshProcessor* DataProcessing/PreProcessor*
    make all
    cd $CURDIR/Debug
    rm -f omdc
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
elif [[ $1 == "t" ]]
then
    TARGETDIR="Debug/Testing/UnitTest"
    TARGETCLS="Debug/Testing/UnitTest/TestRTSResponse"
elif [[ $1 == "term" ]]
then
    TARGETDIR="Debug"
    TARGETCLS="Terminal"
fi

####################################################
## Compile
####################################################
cd $CURDIR/$TARGETDIR
rm -f "$TARGETCLS".o "$TARGETCLS".d
cd $CURDIR/Debug
rm -f omdc
make all
