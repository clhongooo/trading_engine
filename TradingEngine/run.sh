#!/bin/bash

rm -f core* ./Debug/core*

if [[ $# -eq 0 ]]
then
    echo "Usage: arg 1 = instance number"
    echo "       arg 2 = d/dbg if debug mode; val if valgrind"
    exit
fi



if [[ $2 == "d" ||  $2 == "dbg" ]]
then
    RUNCMD="gdb --args ./Debug/TradingEngine"
elif [[ $2 == "v" ]]
then
    RUNCMD="valgrind -v --tool=memcheck --leak-check=full --show-leak-kinds=all ./Debug/TradingEngine"
else
    RUNCMD="./Debug/TradingEngine"
fi


#--------------------------------------------------
if [[ $1 == "us" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_us.ini"
    exit
fi
if [[ $1 == "spyes" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_spyes.ini"
    exit
fi

if [[ $1 == "hk" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_hk.ini"
    exit
fi
if [[ $1 == "r9" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_r9.ini"
    exit
fi
if [[ $1 == "r7" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_r7.ini"
    exit
fi
if [[ $1 == "b1hkf" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_b1hkf.ini"
    exit
fi

if [[ $1 == "zsun_b2spy_pprbkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_b2spy_pprbkt_nmdi_noti.ini"
    exit
fi
if [[ $1 == "zsun_b2spy_bkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_b2spy_bkt_nmdi_noti.ini"
    exit
fi
if [[ $1 == "zsun_b2hk_bkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_b2hk_bkt_nmdi_noti.ini"
    exit
fi
if [[ $1 == "cash_b26p_pprbkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/cash_b26p_pprbkt_nmdi_noti.ini"
    exit
fi
if [[ $1 == "cash_b2spy_pprbkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/cash_b2spy_pprbkt_nmdi_noti.ini"
    exit
fi
if [[ $1 == "b2bkt" ]]
then
    SYM=$2
    cat /home/qy/Dropbox/nirvana/conf/zsun_b2_bkt_nmdi_noti_base.ini | sed -e "s/__SYMBOL__/$SYM/g" > /home/qy/Dropbox/nirvana/conf/zsun_b2_bkt_nmdi_noti.ini
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_b2_bkt_nmdi_noti.ini"
    exit
fi
if [[ $1 == "cash_b2hsi_bkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/cash_b2hsi_bkt_nmdi_noti.ini"
    exit
fi
if [[ $1 == "b26p" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/scentest_b2_6pdt.ini"
    exit
fi
if [[ $1 == "scenb2spy" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/scentest_b2_spy.ini"
    exit
fi
if [[ $1 == "scenb2hyg" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/scentest_b2_hyg.ini"
    exit
fi


if [[ $1 == "testrollfwd" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_testrollfwd.ini"
    exit
fi





#  gdb --args ./Debug/TradingEngine "/home/qy/Dropbox/nirvana/conf/nirvana"$1".ini"
   $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana"$1".ini"
#  valgrind ./Debug/TradingEngine "/home/qy/Dropbox/nirvana/conf/nirvana"$1".ini"
#  valgrind --tool=callgrind ./Debug/TradingEngine "/home/qy/Dropbox/nirvana/conf/nirvana"$1".ini"



