#!/bin/bash
rm -f core*
#./Debug/DataProcessing 1 /home/qy/d/data/ebroker_data/data1/20130417.csv /tmp/DataProcTestOut HSIJ3 && view /tmp/DataProcTestOut
#./Debug/DataProcessing 1 /home/qy/d/data/ebroker_data/data1/20130417.csv /tmp/DataProcTestOut HHIJ3 && view /tmp/DataProcTestOut

# ./Debug/DataProcessing 3 /home/qy/dr/nirvana/copula/stockdata/00144.csv /home/qy/dr/nirvana/copula/stockdata/00144_1m.csv D
# ./Debug/DataProcessing 3 /home/qy/dr/nirvana/copula/stockdata/00322.csv /home/qy/dr/nirvana/copula/stockdata/00322_1m.csv D

# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HSIc1_ebkr.csv /home/qy/Rtest/HSIc1_ebkr_2.csv 2
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HSIc1_ebkr.csv /home/qy/Rtest/HSIc1_ebkr_5.csv 5
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HSIc1_ebkr.csv /home/qy/Rtest/HSIc1_ebkr_15.csv 15
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HSIc1_ebkr.csv /home/qy/Rtest/HSIc1_ebkr_30.csv 30
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HSIc1_ebkr.csv /home/qy/Rtest/HSIc1_ebkr_60.csv 60
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HSIc1_ebkr.csv /home/qy/Rtest/HSIc1_ebkr_120.csv 120
#
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HHIc1_ebkr.csv /home/qy/Rtest/HHIc1_ebkr_2.csv 2
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HHIc1_ebkr.csv /home/qy/Rtest/HHIc1_ebkr_5.csv 5
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HHIc1_ebkr.csv /home/qy/Rtest/HHIc1_ebkr_15.csv 15
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HHIc1_ebkr.csv /home/qy/Rtest/HHIc1_ebkr_30.csv 30
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HHIc1_ebkr.csv /home/qy/Rtest/HHIc1_ebkr_60.csv 60
# /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 4 /home/qy/Rtest/HHIc1_ebkr.csv /home/qy/Rtest/HHIc1_ebkr_120.csv 120


# ./Debug/DataProcessing 333 /home/qy/Dropbox/dataENF/assembledData/HSIFutures/combineAll/ /home/qy/dr/dataENF/assembledData/HSIFutures/HSIFC.txt 20000102 20140701 HI
# ./Debug/DataProcessing 333 /home/qy/Dropbox/dataENF/assembledData/HHIFutures/combineAll/ /home/qy/dr/dataENF/assembledData/HHIFutures/HHIFC.txt 20040102 20140701 HC



#./Debug/DataProcessing 234 /tmp/20130417.csv /tmp/20130418.csv /tmp/DataProcTestOut && view /tmp/DataProcTestOut


# ./Debug/DataProcessing 235 /tmp/20131118.csv /tmp/DataProcTestOut 10 && view /tmp/DataProcTestOut
# ./Debug/DataProcessing 235 /mnt/d/data/ebroker_data/fut_opt/All_20130205_20150116 /mnt/d/data/ebroker_data/fut_opt/Snapshot_20130205_20150116 10
# ./Debug/DataProcessing 235 /mnt/d/nir/HSI_only_tick_20130417_present /mnt/d/nir/HSI_only_1min_20130417_present 60

# # for INTERVAL in 10 30 60
# for INTERVAL in 60
# do
#    SRC_DIR="/mnt/d/data/ebroker_data/hkfut4/"
#    TGT_DIR="/mnt/d/data/ebroker_data/hkfut4_snap"$INTERVAL"sec/"
#    rm -f $TGT_DIR/*
#    cd $SRC_DIR
#    for i in *csv
#    do
#        /home/qy/workspaceCN/DataProcessing/Debug/DataProcessing 235 $SRC_DIR/$i $TGT_DIR/$i $INTERVAL
#    done
# done

# cd /home/qy/Dropbox/dataENF/yahData/data/
# for i in *
# do
#     /home/qy/Dropbox/nirvana/workspaceCN/DataProcessing/Debug/DataProcessing 236 /home/qy/Dropbox/dataENF/yahData/data/"$i" /tmp/"$i".txt 0"$(echo $i | sed -e 's/.HK.csv//')" 093000 110000 140000 160000 DOHLCV
# done


# cd /home/qy/dr/nirvana/conf/suppdaybardata/
# /home/qy/Dropbox/nirvana/workspaceCN/DataProcessing/Debug/DataProcessing 236 /home/qy/dr/nirvana/conf/suppdaybardata/QQQ.csv /tmp/QQQ.txt QQQ 220000 010000 021500 040000 DxOHLC
# /home/qy/Dropbox/nirvana/workspaceCN/DataProcessing/Debug/DataProcessing 236 /home/qy/dr/nirvana/conf/suppdaybardata/SPY.csv /tmp/SPY.txt SPY 220000 010000 021500 040000 DxOHLC


./Debug/DataProcessing 237 /mnt/d/data/hkex/HSI_M1_ohlc.csv /mnt/d/data/hkex/HSI_M1_mdi.csv HSI f 0 1 5 t 6
