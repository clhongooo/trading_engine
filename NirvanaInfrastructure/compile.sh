#!/bin/bash

MAKE_JOB=$(expr $(nproc) - 1)

cd Debug
rm -f *.a

if [[ $1 == "c" ||  $2 == "c" ]]
then
    make clean
    make -j$MAKE_JOB all
else
    make -j$MAKE_JOB all
fi

