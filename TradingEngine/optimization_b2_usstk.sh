#!/bin/bash

if [[ $# -eq 0 ]]
then
    echo "Usage: arg 1 = [1/2/3] = instance number"
    exit
fi

if [[ ! -f /mnt/d/data/nirvana/usequities_adj_allinone.csv ]]
then
    echo "Data file doesn't exist."
    exit
fi

#----------------------------------------------------------------------------------------------------
CURDIR=$(pwd)

#----------------------------------------------------------------------------------------------------
# Parameters
#----------------------------------------------------------------------------------------------------

PARAMVECTORS=(
"1000000,1,0.035,1,0.15,0.05,0.05,504,756,504,756,-3,5,0.2,-3,5,0.2,-5,5,0.5,-5,5,0.5,2,1"
)

NOOFPARAMVEC=${#PARAMVECTORS[@]}

INSTANCENUM=$1
if [[ $INSTANCENUM -eq 1 ]]
then
    SUBDIR=nir1
    CONFIGFILE=nirvana1.ini
    #SYMBOLLIST="SPY VXX EEM GDX XLF IWM EWJ QQQ FXI USO EFA XIV EWZ XLE UVXY XLU VWO RSX GDXJ TLT XOP IYR XLI XLK JNK EWT XLV XLP EZU OIH HYG EWG SLV UNG GLD AMLP DXJ XLY EPI VGK DIA HEDJ VEA KRE XLB DBEF VNQ SMH ASHR OIL IVV EWH LQD ITB XHB IAU EWU IEMG FEZ UUP VTI XME EWY BKLN EWI INDA EWW BND XRT EWA EWC KBE AGG PFF AMJ DBC TBT TIP IBB IYH MDY IHE IYC IWV ITA IYK EMB IYJ"
    SYMBOLLIST="IBB IYH MDY IHE IYC IWV ITA IYK EMB IYJ"
elif [[ $INSTANCENUM -eq 2 ]]
then
    SUBDIR=nir2
    CONFIGFILE=nirvana2.ini
    SYMBOLLIST="EWZ EZU FEZ FXI GDX GDXJ GLD HEDJ HYG IAU IEMG INDA ITB IVV IWM IYR JNK KBE KRE LQD OIH OIL PFF XLY XRT"
elif [[ $INSTANCENUM -eq 3 ]]
then
    SUBDIR=nir3
    CONFIGFILE=nirvana3.ini
    SYMBOLLIST="IYR"
elif [[ $INSTANCENUM -eq 4 ]]
then
    SUBDIR=nir4
    CONFIGFILE=nirvana4.ini
    SYMBOLLIST="UUP"
elif [[ $INSTANCENUM -eq 5 ]]
then
    SUBDIR=nir5
    CONFIGFILE=nirvana5.ini
    SYMBOLLIST="GDXJ UNG JNK SMH"
elif [[ $INSTANCENUM -eq 6 ]]
then
    SUBDIR=nir6
    CONFIGFILE=nirvana6.ini
    SYMBOLLIST="EWZ"
fi

TERMINALDATE=20151217
CONFIGFILEPATH=/home/qy/Dropbox/nirvana/conf/
LOGFILEPATH=/mnt/d/nir/
SETOFFILESTOCOPY1="$LOGFILEPATH/$SUBDIR/nirvana_mtm_holdings.log $LOGFILEPATH/$SUBDIR/nirvana_mtm.log $LOGFILEPATH/$SUBDIR/nirvana_b2*"
SETOFFILESTOCOPY2="$CONFIGFILEPATH/$CONFIGFILE"
BKTRESULTSFOLDER="/home/qy/Dropbox/nirvana/bkt_results/"

#----------------------------------------------------------------------------------------------------


if [[ ! -d $BKTRESULTSFOLDER ]]
then
    mkdir -p $BKTRESULTSFOLDER
fi

ps ux | grep TradingEngine | grep $CONFIGFILE | awk '{print $2}' | xargs kill > /dev/null 2>&1


for pv in $(seq 0 $(expr $NOOFPARAMVEC - 1))
do

    for sym in $SYMBOLLIST
    do
        cat $CONFIGFILEPATH/nirvana_base_b2usstk.ini | sed -e "s/___OPTIMIZATIONSYMBOL___/$sym/" | sed -e "s/___SUBDIR___/$SUBDIR/" | sed -e "s/___PARAMVECTORS___/${PARAMVECTORS[$pv]}/"  > $CONFIGFILEPATH/$CONFIGFILE

        sleep 1

        cd $CURDIR
        ./Debug/TradingEngine "/home/qy/Dropbox/nirvana/conf/nirvana"$INSTANCENUM".ini" &

        for j in $SETOFFILESTOCOPY1
        do
            echo > $j
        done

        while [ 1 ]
        do
            if [[ $(cat $LOGFILEPATH/$SUBDIR/nirvana.log | grep $TERMINALDATE | wc -l) -gt 0 ]]
            then
                ps ux | grep TradingEngine | grep $CONFIGFILE | awk '{print $2}' | xargs kill > /dev/null 2>&1
                sleep 2
                ps ux | grep TradingEngine | grep $CONFIGFILE | awk '{print $2}' | xargs kill > /dev/null 2>&1
                sleep 2
                RESULTSFOLDER=$BKTRESULTSFOLDER/"b2usstk_"$sym"_"$(date +'%Y%m%d_%H%M')"_"$INSTANCENUM
                mkdir $RESULTSFOLDER
                cp -r $SETOFFILESTOCOPY1 $SETOFFILESTOCOPY2 $RESULTSFOLDER
                cat $LOGFILEPATH/$SUBDIR/nirvana.log | grep MTMLog > $RESULTSFOLDER/nirvana_mtm.log
                gzip $RESULTSFOLDER/*
                gunzip $RESULTSFOLDER/*mtm*

                sleep 2
                break
            fi
            sleep 1
        done

    done

done
