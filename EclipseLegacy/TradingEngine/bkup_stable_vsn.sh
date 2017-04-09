#!/bin/bash

TMPFOLDER=/tmp/nir_bkupstbvsn

rm -rf $TMPFOLDER
mkdir $TMPFOLDER

cd ..
cp -r NirvanaCommon/ $TMPFOLDER
cp -r TradingEngine/ $TMPFOLDER

cd $TMPFOLDER/NirvanaCommon/
rm -rf Debug

cd $TMPFOLDER/TradingEngine/
mv Debug/TradingEngine .
rm -rf Debug/*
mv TradingEngine Debug/

#############################

tar zcf /home/qy/Dropbox/nirvana/stableVersions/NirCmnTrdgEng_$(date +'%Y%m%d_%H%M%S').tgz $TMPFOLDER
rm -rf $TMPFOLDER
