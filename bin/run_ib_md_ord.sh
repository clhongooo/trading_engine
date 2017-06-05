#!/bin/bash
LD_LIBRARY_PATH="../lib:$LD_LIBRARY_PATH"

if [[ $1 == "d" ]]
then
    gdb --args ./ib_md_ord ../conf/ib/ib_md_ord.ini
else
    ./ib_md_ord ../conf/ib/ib_md_ord.ini
fi
