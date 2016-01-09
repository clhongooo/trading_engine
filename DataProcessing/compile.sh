#!/bin/bash
CURDIR=$(pwd)

rm -f core*
cd $CURDIR/Debug && make clean && make -j6 all
