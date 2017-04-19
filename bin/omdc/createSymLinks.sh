#!/bin/bash
ln -s ../../../McastRecorder/Debug/McastRecorder ./mcast_recorder_main
ln -s ../../../omd_common/Debug/libomd_common.a .
ln -s ../../../omd_mdi/Debug/libomd_mdi.so .
ln -s ../../Debug/omdc ./omdc_main
ln -s ../../Config-Prdn-SP-PriceOnly-HeavyLog.ini ../conf/
ln -s ../../Config-Prdn-SP-PriceOnly-LiteLog.ini ../conf/
