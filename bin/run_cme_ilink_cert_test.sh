#!/bin/bash
BIN=cme_ilink
SessionConfig=../conf/cme_ilink/W80004N.cfg
CmdListFile=../conf/cme_ilink/cert_test_cmd_list.txt
if [[ $1 == "d" ]]
then
    gdb --args $BIN -a W80004N -f $SessionConfig -c $CmdListFile
else
    $BIN -a W80004N -f $SessionConfig -c $CmdListFile
fi
