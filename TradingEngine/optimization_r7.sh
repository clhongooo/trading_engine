#!/bin/bash

if [[ $# -eq 0 ]]
then
    echo "Usage: arg 1 = [1/2/3] = instance number"
    exit
fi

if [[ ! -f /tmp/hkex_hsi_1m_allinone.csv ]]
then
    echo "/tmp/hkex_hsi_1m_allinone.csv doesn't exist."
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
    "50,5,0.03,1,200"
    "50,5,0.07,1,200"
    "50,5,0.11,1,200"
    "50,5,0.15,1,200"
    "50,5,0.19,1,200"
    "50,5,0.23,1,200"
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 2 ]]
then
    SUBDIR=nir2
    CONFIGFILE=nirvana2.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    "100,5,0.03,1,200"
    "100,5,0.07,1,200"
    "100,5,0.11,1,200"
    "100,5,0.15,1,200"
    "100,5,0.19,1,200"
    "100,5,0.23,1,200"
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 3 ]]
then
    SUBDIR=nir3
    CONFIGFILE=nirvana3.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    "150,5,0.03,1,200"
    "150,5,0.07,1,200"
    "150,5,0.11,1,200"
    "150,5,0.15,1,200"
    "150,5,0.19,1,200"
    "150,5,0.23,1,200"
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 4 ]]
then
    SUBDIR=nir4
    CONFIGFILE=nirvana4.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
    )
    NOOFPARAMVEC=${#PARAMVECTORS[@]}
elif [[ $INSTANCENUM -eq 5 ]]
then
    SUBDIR=nir5
    CONFIGFILE=nirvana5.ini
    SYMBOLLIST="HSIc1"
    PARAMVECTORS=(
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
SETOFFILESTOCOPY1="$LOGFILEPATH/$SUBDIR/nirvana_mtm_holdings.log $LOGFILEPATH/$SUBDIR/nirvana_mtm.log $LOGFILEPATH/$SUBDIR/nirvana_r7.log"
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
        cat $CONFIGFILEPATH/nirvana_base_r7.ini | sed -e "s/___OPTIMIZATIONSYMBOL___/$sym/" | sed -e "s/___SUBDIR___/$SUBDIR/" | sed -e "s/___PARAMVECTORS___/${PARAMVECTORS[$pv]}/"  > $CONFIGFILEPATH/$CONFIGFILE

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
                RESULTSFOLDER=$BKTRESULTSFOLDER/"r7_"$sym"_"$(date +'%Y%m%d_%H%M')"_"$INSTANCENUM
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
