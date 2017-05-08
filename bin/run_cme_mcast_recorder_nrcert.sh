#!/bin/bash
rm -f cme_mdp_data/*
if [[ $1 == "d" ]]
then
    gdb --args mcast_recorder ../conf/mcast_recorder/cme_mdp/ncert.csv cme_mdp_data 3
else
    mcast_recorder ../conf/mcast_recorder/cme_mdp/ncert.csv cme_mdp_data 3
fi
