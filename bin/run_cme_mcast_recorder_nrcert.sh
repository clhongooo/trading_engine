#!/bin/bash
rm -f cme_mdp_data/*
mcast_recorder ../conf/mcast_recorder/cme_mdp/ncert.csv cme_mdp_data 3
