#!/bin/bash
if [[ $1 == "d" ]]
then
    gdb --args ./cme-md 'multicast://224.0.25.126:16901|10.249.71.131' ./CmeInstruments.cfg CNHH7 CNHM7
else
    # ./cme-md 'multicast://224.0.25.126:16901|10.249.71.131' ./CmeInstruments.cfg CNHH7 CNHM7
    ./cme-md 'multicast://224.0.28.125:16644|10.249.71.131' ./CmeInstruments.cfg CNHH7 CNHM7
fi
