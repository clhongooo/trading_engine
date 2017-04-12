#!/bin/bash

source kill_simulation_capi.sh
python simulation_capi_mdi.py &
sleep 1

if [[ $1 == *"d"* ]]
then
    gdb --args $HOME/TradingEngineBuild/TradingEngine systest1_config.ini
else
    $HOME/TradingEngineBuild/TradingEngine systest1_config.ini
fi

sleep 1
source kill_simulation_capi.sh
