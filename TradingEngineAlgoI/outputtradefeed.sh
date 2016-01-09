#!/bin/bash
TMPFILENAME=/mnt/d/nir/tmptrade.txt
#TRDFDLOCATION=/mnt/d/capi_protocol_server_rh63/data/log/capi_pro*
TRDFDLOCATION=/mnt/d/nir/nirvana.log

if [[ $1 == "b" ]]
then
    TMPFILENAME=/home/qy/dr/nirvana/results/S11A/$(date +'%Y%m%d_%H%M%S').csv
fi

cat $TRDFDLOCATION | egrep 'tradefeed' | sed -e 's/^.*Received: //' | sed -e 's/^.*Informational]: //' | tr ',' '\t' > $TMPFILENAME
#cat $TMPFILENAME | grep T1GBM > /mnt/w/tmp.txt
#cat $TMPFILENAME | grep T1 | grep GUMBELGAU > /mnt/w/tmp.txt
cat $TMPFILENAME > /mnt/w/tmp.txt
cat /mnt/w/tmp.txt
