#!/bin/bash

YEAR=$(date +'%Y')
LOGFILE=( "/home/atu/omdc/OMD.log" )
OUTFILE="/tmp/chkans"

rm -f $OUTFILE

for j in 1 2 3 4
do

    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:288,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":288,               >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:8114,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":8114,              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:8115,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":8115,              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:8116,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":8116,              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:8117,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":8117,              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:8118,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":8118,              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:8119,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":8119,              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:8120,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":8120,              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:8121,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":8121,              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:11280,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":11280,             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:34,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":34,                >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Odd Lot: Code:66,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'Type\":33,|Type\":34,' | grep Code\":66,                >> $OUTFILE

    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:288,"    >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":288,"                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:8114,"   >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":8114,"                          >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:8115,"   >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":8115,"                          >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:8116,"   >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":8116,"                          >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:8117,"   >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":8117,"                          >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:8118,"   >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":8118,"                          >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:8119,"   >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":8119,"                          >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:8120,"   >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":8120,"                          >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:8121,"   >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":8121,"                          >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:11280,"  >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":11280,"                         >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:34,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":34,"                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Order Book: Code:66,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | egrep 'BID|ASK' | grep "Code\":66,"                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:10: ETS"             >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":10, | grep ETS                                     >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:10: GEM"             >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":10, | grep GEM                                     >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:10: MAIN"            >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":10, | grep MAIN                                    >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:10: NASD"            >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":10, | grep NASD                                    >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:10089,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":10089,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:11001,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":11001,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:11002,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":11002,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:11112,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":11112,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:13005,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":13005,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:14196,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":14196,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:16252,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":16252,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:199,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":199,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:609,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":609,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:611,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":611,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:23,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":23,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:24,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":24,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:2529,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":2529,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:4336,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":4336,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:4125,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":4125,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:25,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":25,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:26,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":26,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:288,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":288,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:11280,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":11280,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:34,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":34,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:4010,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":4010,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:4061,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":4061,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:40,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":40,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:22,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":22,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:4314,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":4314,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:4363,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":4363,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:6024,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":6024,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:9346,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":9346,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:64854,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":64854,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:66,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":66,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:67,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":67,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:68043,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":68043,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:74,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":74,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:75,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":75,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:781,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":781,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:78,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":78,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:79,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":79,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:99995,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":99995,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:11: Code:99996,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":11, | grep Code\":99996,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:13: Code:1017,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":13, | grep Code\":1017,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:13: Code:19773,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":13, | grep Code\":19773,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:13: Code:8145,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":13, | grep Code\":8145,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:13: Code:9001,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":13, | grep Code\":9001,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:14: Code:CAD"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":14, | grep Code\":\"CAD                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:14: Code:CNY"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":14, | grep Code\":\"CNY                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:14: Code:EUR"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":14, | grep Code\":\"EUR                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:14: Code:GBP"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":14, | grep Code\":\"GBP                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:14: Code:HKD"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":14, | grep Code\":\"HKD                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:14: Code:JPY"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":14, | grep Code\":\"JPY                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:14: Code:SGD"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":14, | grep Code\":\"SGD                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:14: Code:USD"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":14, | grep Code\":\"USD                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:20: Code:ETS"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":20, | grep Code\":\"ETS                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:20: Code:GEM"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":20, | grep Code\":\"GEM                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:20: Code:MAIN"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":20, | grep Code\":\"MAIN                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:20: Code:NASD"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":20, | grep Code\":\"NASD                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:11896,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":11896,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:1197,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":1197,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:1,"         >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":1,                               >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:22,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":22,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:2,"         >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":2,                               >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:609,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":609,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:611,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":611,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:9346,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":9346,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:9346,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":9346,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:64428,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":64428,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:9202,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":9202,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:4336,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":4336,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:21: Code:9362,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":21, | grep Code\":9362,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:22: NewsID:13,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":22, | grep NewsID\":\"013\",                       >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:22: NewsID:6,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":22, | grep NewsID\":\"006\",                       >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:23,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":23,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:24,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":24,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:25,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":25,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:26,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":26,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:288,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":288,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:11280,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":11280,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:11112,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":11112,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:13005,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":13005,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:34,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":34,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:40,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":40,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:4125,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":4125,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:6005,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":6005,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:6007,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":6007,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:6010,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":6010,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:6024,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":6024,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:66,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":66,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:67,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":67,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:74,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":74,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:75,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":75,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:78,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":78,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:40: Code:79,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":40, | grep Code\":79,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:41: Code:4005,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":41, | grep Code\":4005,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:41: Code:4007,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":41, | grep Code\":4007,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:41: Code:4010,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":41, | grep Code\":4010,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:41: Code:4024,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":41, | grep Code\":4024,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:41: Code:6001,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":41, | grep Code\":6001,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:44: Code:2598,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":44, | grep Code\":2598,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:44: Code:4176,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":44, | grep Code\":4176,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:11112,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":11112,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:11112,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":11112,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:11112,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":11112,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:13005,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":13005,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:13005,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":13005,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:13005,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":13005,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:8114,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":8114,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:8114,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":8114,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:8114,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":8114,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:8115,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":8115,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:8115,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":8115,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:8115,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":8115,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:8116,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":8116,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:8116,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":8116,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:8116,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":8116,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:8117,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":8117,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:8117,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":8117,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:8117,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":8117,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:8118,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":8118,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:8118,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":8118,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:8118,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":8118,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:8119,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":8119,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:8119,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":8119,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:8119,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":8119,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:8120,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":8120,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:8120,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":8120,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:8120,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":8120,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:8121,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":8121,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:8121,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":8121,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:8121,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":8121,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:4125,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":4125,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:4125,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":4125,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:4125,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":4125,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:19,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":19,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:19,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":19,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:19,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":19,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:50: Code:42,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":50, | grep Code\":42,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:51: Code:42,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":51, | grep Code\":42,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:52: Code:42,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":52, | grep Code\":42,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:34,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":34,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:66,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":66,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:288,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":288,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:8114,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":8114,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:8115,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":8115,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:8116,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":8116,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:8117,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":8117,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:8118,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":8118,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:8119,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":8119,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:8120,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":8120,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:8121,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":8121,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:54: Code:11280,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":54, | grep Code\":11280,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:11112,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":11112,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:13005,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":13005,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:4125,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":4125,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:288,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":288,                             >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:11280,"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":11280,                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:23,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":23,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:24,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":24,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:25,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":25,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:26,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":26,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:34,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":34,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:4005,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":4005,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:4007,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":4007,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:4008,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":4008,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:4010,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":4010,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:4024,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":4024,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:40,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":40,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:66,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":66,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:67,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":67,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:74,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":74,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:75,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":75,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:78,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":78,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:60: Code:79,"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":60, | grep Code\":79,                              >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:61: Code:ETS"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":61, | grep Code\":\"ETS                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:61: Code:GEMHKD"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":61, | grep Code\":\"GEM | grep Code\":\"HKD        >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:61: Code:GEM"        >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":61, | grep Code\":\"GEM                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:61: Code:MAINHKD"    >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":61, | grep Code\":\"MAIN | grep Code\":\"HKD       >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:61: Code:MAIN"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":61, | grep Code\":\"MAIN                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:61: Code:MAINUSD"    >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":61, | grep Code\":\"MAIN | grep Code\":\"USD       >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:61: Code:NASDJPY"    >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":61, | grep Code\":\"NASD | grep Code\":\"JPY       >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:61: Code:NASD"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":61, | grep Code\":\"NASD                           >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:62: Code:4001,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":62, | grep Code\":4001,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:62: Code:4005,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":62, | grep Code\":4005,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:62: Code:4007,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":62, | grep Code\":4007,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:62: Code:4010,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":62, | grep Code\":4010,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:62: Code:4024,"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":62, | grep Code\":4024,                            >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:70: Code:100,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":70, | grep Code\":\"0000100                        >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:70: Code:CSI300"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":70, | grep CSI300                                  >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:70: Code:SPHKG"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":70, | grep SPHKG                                   >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:71: Code:100,"       >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":71, | grep Code\":\"0000100                        >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:71: Code:CSI300"     >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":71, | grep CSI300                                  >> $OUTFILE
    echo  "Batch 5.$j"  >> $OUTFILE;      echo "Type:71: Code:SPHKG"      >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep Type\":71, | grep SPHKG                                   >> $OUTFILE


    echo  "Batch 5.$j"  >> $OUTFILE;      echo "TerminalSession"          >> $OUTFILE;      cat "${LOGFILE[$j]}" | grep TerminalSession                                           >> $OUTFILE


done
