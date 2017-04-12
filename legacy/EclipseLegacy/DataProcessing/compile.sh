#!/bin/bash
CURDIR=$(pwd)

MAKE_JOB=$(expr $(nproc) - 1)
rm -f core*
cd $CURDIR/Debug && make clean && make -j$MAKE_JOB all
