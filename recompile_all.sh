#!/bin/bash
LSDIRO="/home/qy/workspaceCN/loglibrary/Debug \
/home/qy/workspaceCN/atu_tcp_library/Debug \
/home/qy/workspaceCN/NirvanaCommon/Debug \
/home/qy/workspaceCN/NirvanaInfrastructure/Debug \
/home/qy/workspaceCN/TradingEngine/Debug"

for DIRO in $LSDIRO
do
    cd $DIRO
    make clean
    make -j6 all
done

cp /home/qy/workspaceCN/TradingEngine/Debug/TradingEngine /tmp/TradingEngine
chmod 777 /tmp/TradingEngine
