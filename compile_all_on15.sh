#!/bin/bash
LSDIRO="/home/qy/Dropbox/nirvana/workspaceCN/loglibrary/Debug \
/home/qy/Dropbox/nirvana/workspaceCN/atu_tcp_library/Debug \
/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommon/Debug \
/home/qy/Dropbox/nirvana/workspaceCN/NirvanaInfrastructure/Debug \
/home/qy/Dropbox/nirvana/workspaceCN/TradingEngine/Debug"

for DIRO in $LSDIRO
do
    cd $DIRO
    make clean
    make
done
