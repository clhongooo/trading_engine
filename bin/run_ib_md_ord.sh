#!/bin/bash
LD_LIBRARY_PATH="../lib:$LD_LIBRARY_PATH"
CONF_FILE=../conf/ib/ib_md_ord.ini

###################################################
# create ib order mapping file if necessary
###################################################
IB_ORD_MAP_FILE=$(cat $CONF_FILE | grep OrderMappingFile | awk -F= '{print $2}')
[[ ! -e $IB_ORD_MAP_FILE ]] && touch $IB_ORD_MAP_FILE

if [[ $1 == "d" ]]
then
    gdb --args ./ib_md_ord $CONF_FILE
else
    ./ib_md_ord $CONF_FILE
fi
