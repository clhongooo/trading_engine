#!/bin/bash

CURDIR=$(pwd)

while [ 1 ]
do
    date >> $CURDIR/monitorOmdc.log
    ps aux | head -1 >> $CURDIR/monitorOmdc.log
    ps aux | grep omdc | grep -v "grep" >> $CURDIR/monitorOmdc.log
    ls -lh /home/atu/omdc/OMD*.log >> $CURDIR/monitorOmdc.log

    if [[ $(ps aux | grep omdc | grep -v "grep" | wc -l) -eq 0 ]]
    then
        echo "========================================================="  >> $CURDIR/monitorOmdc.log
        echo "================ omdc has stopped!?!====================="  >> $CURDIR/monitorOmdc.log
        echo "========================================================="  >> $CURDIR/monitorOmdc.log
    else
        echo "========================================================="  >> $CURDIR/monitorOmdc.log
        echo "================ omdc is running. ======================="  >> $CURDIR/monitorOmdc.log
        echo "========================================================="  >> $CURDIR/monitorOmdc.log
    fi

    sleep 30
done
