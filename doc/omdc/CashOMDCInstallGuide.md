# CASH OMD-C Installation Guide

## Install libraries
The libraries are:

1.   boost\_1\_53\_0.tar.gz
1.   cppunit-1.12.1.tar.gz
1.   libjson\_7.6.1.zip
1.   pantheios-1.0.1-beta214.zip
1.   stlsoft-1.9.117-hdrs.zip

## Copy the binaries to Red Hat 5.8

Expand the provided compressed file to the following director structure:

-   production/
-   production/bin
-   production/conf
-   production/data
-   production/data/out
-   production/data/log
-   production/data/err
-   production/data/canneddata

## Change Config.ini

1.   LogPath
1.   RTS information: Username, Password, IP etc . (Just ignore if not planning to use RTS)


## Start / Stop scripts

Start OMD-C with the script "startOMDC\_prdn.sh"

Stop OMD-C with the script "stopOMDC.sh"






<!---
-------------------
## Copy workspaceC to RedHat

## hkex sim nyx
1.   Copy stuff in /opt/nyx/xdp/release/bin/
1.   mkdir/opt/nyx/xdp/release/data/binary_testing/ 
1.   Change /opt/nyx/xdp/release/config/xdp-rts-client-sim.xml

## cpp project makefile
change paths (for workspaceC and pantheios gcc46) in all makefile's inside omdc/Debug



runSTest.sh: Change Data path and Log path, Change RUN_TEST flags, Change PLAYBACK_SPEED
--->
