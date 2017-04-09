#!/bin/bash

if [[ $# -eq 0 ]]
then
    echo "Usage: arg 1 = [1/2/3] = instance number"
    exit
fi

if [[ ! -f /mnt/d/data/hkex/hkex_hsi_1m_allinone.csv ]]
then
    echo "Data file doesn't exist."
    exit
fi

#----------------------------------------------------------------------------------------------------
CURDIR=$(pwd)

#----------------------------------------------------------------------------------------------------
# Parameters
#----------------------------------------------------------------------------------------------------

INSTANCENUM=$1
if [[ $INSTANCENUM -eq 1 ]]
then
    SUBDIR=nir1
    CONFIGFILE=nirvana1.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    "40,5,1,100,0"
    "50,5,1,100,0"
    "60,5,1,100,0"
    "70,5,1,100,0"
    "80,5,1,100,0"
    "90,5,1,100,0"
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 2 ]]
then
    SUBDIR=nir2
    CONFIGFILE=nirvana2.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    "150,5,1,100,0"
    "160,5,1,100,0"
    "170,5,1,100,0"
    "180,5,1,100,0"
    "190,5,1,100,0"
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 3 ]]
then
    SUBDIR=nir3
    CONFIGFILE=nirvana3.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    "100,5,1,100,0"
    "110,5,1,100,0"
    "120,5,1,100,0"
    "130,5,1,100,0"
    "140,5,1,100,0"
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 4 ]]
then
    SUBDIR=nir4
    CONFIGFILE=nirvana4.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    "10,5,1,100,0"
    "20,5,1,100,0"
    "30,5,1,100,0"
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 5 ]]
then
    SUBDIR=nir5
    CONFIGFILE=nirvana5.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    "100,5,1,100,0"
    "200,5,1,100,0"
    "300,5,1,100,0"
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 6 ]]
then
    SUBDIR=nir6
    CONFIGFILE=nirvana6.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 7 ]]
then
    SUBDIR=nir7
    CONFIGFILE=nirvana7.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 8 ]]
then
    SUBDIR=nir8
    CONFIGFILE=nirvana8.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
fi

TERMINALDATE=20150630
CONFIGFILEPATH=/home/qy/Dropbox/nirvana/conf/
LOGFILEPATH=/mnt/d/nir/
SETOFFILESTOCOPY1="$LOGFILEPATH/$SUBDIR/nirvana_mtm_holdings.log $LOGFILEPATH/$SUBDIR/nirvana_mtm.log $LOGFILEPATH/$SUBDIR/nirvana_r9.log"
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
        cat $CONFIGFILEPATH/nirvana_base_r9.ini | sed -e "s/___OPTIMIZATIONSYMBOL___/$sym/" | sed -e "s/___SUBDIR___/$SUBDIR/" | sed -e "s/___PARAMVECTORS___/${PARAMVECTORS[$pv]}/"  > $CONFIGFILEPATH/$CONFIGFILE

        sleep 1

        cd $CURDIR
        ./Debug/TradingEngine "/home/qy/Dropbox/nirvana/conf/nirvana"$1".ini" &

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
                RESULTSFOLDER=$BKTRESULTSFOLDER/"r9_"$sym"_"$(date +'%Y%m%d_%H%M')"_"$INSTANCENUM
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
