#!/bin/bash

          BLMGCOMMONPATHS=/home/qy/Dropbox/dataENF/blmg/common_path.sh
         CTRTINIHKFUTBASE=contracts_hkfut_base.ini
           CTRTINISTKBASE=contracts_stk_base.ini
  CTRTINISTKBASEWITHCONID=contracts_stk_base_withConId.ini
                  CTRTINI=contracts.ini
                CONIDFILE=contracts_conId.ini

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
        cat $CTRTINISTKBASEWITHCONID | sed -e "s/__SYMBOL__/$sym/" | sed -e 's/__EXCHG__/SMART/' | sed -e 's/__CURRENCY__/USD/' | sed -e "s/__CONID__/$IDFROMCONIDFILE/" >> $CTRTINI
    else
        cat $CTRTINISTKBASE          | sed -e "s/__SYMBOL__/$sym/" | sed -e 's/__EXCHG__/SMART/' | sed -e 's/__CURRENCY__/USD/'                                          >> $CTRTINI
    fi
done

for sym in $SYMBOLLIST_HK
do
    cat $CTRTINISTKBASE | sed -e "s/__SYMBOL__/$sym/" | sed -e 's/__EXCHG__/SEHK/' | sed -e 's/__CURRENCY__/HKD/' >> $CTRTINI
done
