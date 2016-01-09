#!/bin/bash

rm -f core* ./Debug/core*

if [[ $# -eq 0 ]]
then
    echo "Usage: arg 1 = instance number"
    exit
fi


./mdi_server_ack.py $1 &
sleep 2

./run.sh $1
