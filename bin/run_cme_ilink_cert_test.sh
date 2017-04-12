#!/bin/bash
BIN=cme_ilink
SessionConfig=../src/main/cme_ilink/W80004N.cfg
CmdListFile=../src/main/cme_ilink/cert_test_cmd_list.txt
$BIN -a W80004N -f $SessionConfig -c $CmdListFile
