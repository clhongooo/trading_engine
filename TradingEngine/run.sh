#!/bin/bash

rm -f core* ./Debug/core*

ARG1=$1
ARG2=$2


if [[ $# -eq 0 ]]
then
    # echo "Usage: arg 1 = instance number"
    # echo "       arg 2 = d/dbg if debug mode; val if valgrind"
    # exit

    echo "Fucking Menu:"
    echo "0 ) us"
    echo "1 ) spyes"
    echo "2 ) hk"
    echo "3 ) r9"
    echo "4 ) r7"
    echo "5 ) b1hkf"
    echo
    echo "6 ) zsun_b2spy_pprbkt_nmdi_noti"
    echo "7 ) zsun_b2spy_bkt_nmdi_noti"
    echo
    echo "8 ) zsun_b2ussht_bkt_nmdi_noti"
    echo "9 ) zsun_b2hk_bkt_nmdi_noti"
    echo
    echo "11) zsun_nir1_bkt_nmdi_noti.ini"
    echo
    echo "12) b2bkt"
    echo "13) cash_b2hsi_bkt_nmdi_noti"
    echo "15) scenb2spy"
    echo "16) scenb2hyg"
    echo "17) testrollfwd"
    echo
    echo -n "Choice> "

    read line
    if   [[ $line -eq 0  ]]; then ARG1="us"                          
    elif [[ $line -eq 1  ]]; then ARG1="spyes"                       
    elif [[ $line -eq 2  ]]; then ARG1="hk"                          
    elif [[ $line -eq 3  ]]; then ARG1="r9"                          
    elif [[ $line -eq 4  ]]; then ARG1="r7"                          
    elif [[ $line -eq 5  ]]; then ARG1="b1hkf"                       
    elif [[ $line -eq 6  ]]; then ARG1="zsun_b2spy_pprbkt_nmdi_noti" 
    elif [[ $line -eq 7  ]]; then ARG1="zsun_b2spy_bkt_nmdi_noti"    
    elif [[ $line -eq 8  ]]; then ARG1="zsun_b2ussht_bkt_nmdi_noti"    
    elif [[ $line -eq 9  ]]; then ARG1="zsun_b2hk_bkt_nmdi_noti"     
    elif [[ $line -eq 11 ]]; then ARG1="zsun_nir1_bkt_nmdi_noti"     
    elif [[ $line -eq 12 ]]; then ARG1="b2bkt"                       
    elif [[ $line -eq 13 ]]; then ARG1="cash_b2hsi_bkt_nmdi_noti"    
    elif [[ $line -eq 15 ]]; then ARG1="scenb2spy"                   
    elif [[ $line -eq 16 ]]; then ARG1="scenb2hyg"                   
    elif [[ $line -eq 17 ]]; then ARG1="testrollfwd"                 
    fi

fi





if [[ $ARG2 == "d" ||  $ARG2 == "dbg" ]]
then
    RUNCMD="gdb --args ./Debug/TradingEngine"
elif [[ $ARG2 == "v" || $ARG2 == "val" ]]
then
    # RUNCMD="valgrind -v --tool=memcheck --leak-check=full --show-leak-kinds=all ./Debug/TradingEngine"
    RUNCMD="valgrind -v --tool=memcheck ./Debug/TradingEngine"
    # RUNCMD="valgrind --tool=callgrind ./Debug/TradingEngine"
else
    RUNCMD="./Debug/TradingEngine"
fi


#--------------------------------------------------
if [[ $ARG1 == "us" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_us.ini"
    exit
fi
if [[ $ARG1 == "spyes" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_spyes.ini"
    exit
fi

if [[ $ARG1 == "hk" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_hk.ini"
    exit
fi
if [[ $ARG1 == "r9" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_r9.ini"
    exit
fi
if [[ $ARG1 == "r7" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_r7.ini"
    exit
fi
if [[ $ARG1 == "b1hkf" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_b1hkf.ini"
    exit
fi

if [[ $ARG1 == "zsun_b2spy_pprbkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_b2spy_pprbkt_nmdi_noti.ini"
    exit
fi
if [[ $ARG1 == "zsun_b2spy_bkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_b2spy_bkt_nmdi_noti.ini"
    exit
fi
if [[ $ARG1 == "zsun_b2ussht_bkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_b2ussht_bkt_nmdi_noti.ini"
    exit
fi
if [[ $ARG1 == "zsun_b2hk_bkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_b2hk_bkt_nmdi_noti.ini"
    exit
fi
if [[ $ARG1 == "zsun_nir1_bkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_nir1_bkt_nmdi_noti.ini"
    exit
fi
if [[ $ARG1 == "b2bkt" ]]
then
    SYM=$ARG2
    cat /home/qy/Dropbox/nirvana/conf/zsun_b2_bkt_nmdi_noti_base.ini | sed -e "s/__SYMBOL__/$SYM/g" > /home/qy/Dropbox/nirvana/conf/zsun_b2_bkt_nmdi_noti.ini
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/zsun_b2_bkt_nmdi_noti.ini"
    exit
fi
if [[ $ARG1 == "cash_b2hsi_bkt_nmdi_noti" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/cash_b2hsi_bkt_nmdi_noti.ini"
    exit
fi
if [[ $ARG1 == "scenb2spy" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/scentest_b2_spy.ini"
    exit
fi
if [[ $ARG1 == "scenb2hyg" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/scentest_b2_hyg.ini"
    exit
fi


if [[ $ARG1 == "testrollfwd" ]]
then
    $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana_testrollfwd.ini"
    exit
fi





#  gdb --args ./Debug/TradingEngine "/home/qy/Dropbox/nirvana/conf/nirvana"$ARG1".ini"
   $RUNCMD "/home/qy/Dropbox/nirvana/conf/nirvana"$ARG1".ini"

