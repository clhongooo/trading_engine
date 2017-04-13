#!/bin/bash
CURDIR=$(pwd)

cd $CURDIR/Debug
make clean
make
cd ..
