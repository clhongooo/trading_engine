#!/bin/bash

cd Debug
rm -f *.a

if [[ $1 == "c" ||  $2 == "c" ]]
then
    make clean
    make -j2 all
else
    make -j3 all
fi

