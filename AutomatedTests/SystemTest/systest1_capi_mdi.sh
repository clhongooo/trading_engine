#!/bin/bash

source kill_simulation_capi.sh
python simulation_capi_mdi.py &
sleep 1

if [[ $1 == *"d"* ]]
then
    gdb --args ../../TradingEngine/Debug/TradingEngine systest1_config.ini
else
    ../../TradingEngine/Debug/TradingEngine systest1_config.ini
fi

source kill_simulation_capi.sh
