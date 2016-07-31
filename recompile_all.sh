#!/bin/bash
LSDIRO="/home/$(whoami)/workspaceCN/loglibrary/Debug \
/home/$(whoami)/workspaceCN/atu_tcp_library/Debug \
/home/$(whoami)/workspaceCN/NirvanaCommon/Debug \
/home/$(whoami)/workspaceCN/NirvanaInfrastructure/Debug \
/home/$(whoami)/workspaceCN/TradingEngine/Debug"

for DIRO in $LSDIRO
do
    cd $DIRO
    make clean
    make -j6 all
done

cp /home/$(whoami)/workspaceCN/TradingEngine/Debug/TradingEngine /tmp/TradingEngine
chmod 777 /tmp/TradingEngine
