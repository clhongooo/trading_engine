#!/bin/bash

          BLMGCOMMONPATHS=/home/qy/Dropbox/dataENF/blmg/common_path.sh
         CTRTINIHKFUTBASE=contracts_fut_base.ini
           CTRTINISTKBASE=contracts_stk_base.ini
  CTRTINISTKBASEWITHCONID=contracts_stk_base_withConId.ini
           CTRTINICURBASE=contracts_cur_base.ini
           CTRTINIIDXBASE=contracts_idx_base.ini
                  CTRTINI=contracts.ini
                CONIDFILE=contracts_conId.ini
                  TMPFILE=/tmp/create_ctrt.tmp
                 TMPFILE2=/tmp/create_ctrt2.tmp

##################################################
cat $CTRTINIHKFUTBASE > $CTRTINI

##################################################
# source $BLMGCOMMONPATHS

# for sym in $SYMBOLLIST_USSTK_1 $SYMBOLLIST_USSTK_2 $SYMBOLLIST_USSTK_3 $SYMBOLLIST_USSTK_4
for sym in SPY AAPL GDX
do
    IDFROMCONIDFILE=$(cat $CONIDFILE | grep "^$sym\>" | awk '{print $2}')

    if [[ -n $IDFROMCONIDFILE ]]
    then
        cat $CTRTINISTKBASEWITHCONID > $TMPFILE
    else
        cat $CTRTINISTKBASE          > $TMPFILE
    fi

    cat $TMPFILE | sed -e "s/__SYMBOL__/$sym/" | sed -e 's/__EXCHG__/SMART/' | sed -e 's/__CURRENCY__/USD/' > $TMPFILE2

    if [[ -n $IDFROMCONIDFILE ]]
    then
        cat $TMPFILE2 | sed -e "s/__CONID__/$IDFROMCONIDFILE/" >> $CTRTINI
    else
        cat $TMPFILE2                                          >> $CTRTINI
    fi

    echo >> $CTRTINI
done

for sym in 2628 2800
do
    cat $CTRTINISTKBASE | sed -e "s/__SYMBOL__/$sym/" | sed -e 's/__EXCHG__/SEHK/' | sed -e 's/__CURRENCY__/HKD/' >> $CTRTINI
    echo >> $CTRTINI
done

for sym in HSI
do
    cat $CTRTINIIDXBASE | sed -e "s/__SYMBOL__/$sym/" | sed -e 's/__EXCHG__/HKFE/' | sed -e 's/__CURRENCY__/HKD/' >> $CTRTINI
    echo >> $CTRTINI
done

for sym in EUR.USD
do
    cat $CTRTINICURBASE | sed -e "s/__SYMBOL__/$sym/" | sed -e 's/__EXCHG__/IDEALPRO/' | sed -e 's/__CURRENCY__/USD/' >> $CTRTINI
    echo >> $CTRTINI
done

rm -f $TMPFILE $TMPFILE2
