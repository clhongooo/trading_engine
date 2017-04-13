#!/bin/bash

CURDIR=$(pwd)

if [[ $(ifconfig | grep 192.168.91.8\    | wc -l) -gt 0 ]]; then source changeMkflPath918; fi
if [[ $(ifconfig | grep 192.168.91.91    | wc -l) -gt 0 ]]; then source changeMkflPath9191; fi

if [[ $# -eq 0 ]]
then
    cd Debug
    make all
    exit
elif [[ $1 == "a" ]]
then
    cd Debug
    make clean
    make all
    exit
fi
