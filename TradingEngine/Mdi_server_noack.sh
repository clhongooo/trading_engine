#!/bin/bash

# cd /mnt/d/nir/
#
# if [[ ! -z $1 ]]
# then
#     if [[ $1 == "b" ]]
#     then
#          #slowcat HSI_only_30sec_20130417_present 10000 100 | nc -l -p 21010
#          slowcat HSI_only_2006_201503 1000 100 | nc -l -p 21010
#          #cat HSI_only_2006_201503 | nc -l -p 21010
#          exit
#     fi
#     if [[ $1 == "n" ]]
#     then
#         slowcat Snapshot_20130205_20150116_sorted 10000 500 | nc -l -p 21010
#         exit
#     fi
# else
#     echo "Usage: arg 1 is data file"
# fi



MKTDATAFILE="/mnt/d/data/hkex/hkex_hsi_allinone.csv"
# MKTDATAFILE="/mnt/d/data/ibcaptured_usstk/20151028.csv"
slowcat $MKTDATAFILE 50 300 | nc -l -p 29010
