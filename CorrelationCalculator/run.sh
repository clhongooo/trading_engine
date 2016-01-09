#!/bin/bash

rm -f core* ./Debug/core*

./Debug/CorrelationCalculator /home/qy/Dropbox/dataENF/yahData/hkstkhsihhi_allthruhist /home/qy/Dropbox/dataENF/yahData/data/ /home/qy/Dropbox/nirvana/conf/hk_correl_matrix/  20000101 20150225 /home/qy/Dropbox/nirvana/conf/HKFE_Calendar_full.csv 44

