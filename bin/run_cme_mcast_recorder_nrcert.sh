#!/bin/bash
# CONFIG=../conf/mcast_recorder/cme_mdp/ncert.csv
CONFIG=../conf/mcast_recorder/cme_mdp/ncert_snapshot.csv
rm -f cme_mdp_data/*
if [[ $1 == "d" ]]
then
    gdb --args mcast_recorder $CONFIG cme_mdp_data 3
else
    mcast_recorder $CONFIG cme_mdp_data 3
fi
