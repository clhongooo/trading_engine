#!/bin/bash
# BIN=cme_mdp
# CMEInstrumentsFile=../conf/cme_mdp/CMEInstruments.cfg
# if [[ $1 == "d" ]]
# then
#     gdb --args $BIN 'multicast://224.0.25.126:16901|10.249.71.131' $CMEInstrumentsFile CNHH7 CNHM7
# else
#     $BIN 'multicast://224.0.28.123:16657|10.249.71.131' $CMEInstrumentsFile CNHH7 CNHM7
# fi
BIN=omd_mdp
CONFIG=../conf/cme_mdp/Config_MDP_nrcert.ini
if [[ $1 == "d" ]]
then
    gdb --args $BIN $CONFIG
elif [[ $1 == "v" ]]
then
    valgrind -v --tool=memcheck $BIN $CONFIG
else
    $BIN $CONFIG
fi
