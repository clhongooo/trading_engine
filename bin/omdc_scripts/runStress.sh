#!/bin/bash

CURRENTFOLDER=$(pwd)

####################################################
## Prompts us for the sudo pwd immediately, don't waste time
####################################################
sudo date

LOGFILEDIR="/mnt/hgfs/nyx/"
LOGFILE="OMD.log"
DATAFILEDIR="/mnt/hgfs/nyx/"

           PLAYBACK_SPEED=200
          PLAYBACK_SPEED2=200
            RUN_TEST_5_1a=0
            RUN_TEST_5_1b=1
            RUN_TEST_5_2a=0
            RUN_TEST_5_2b=1
            RUN_TEST_5_3a=0
            RUN_TEST_5_3b=1
            RUN_TEST_5_4a=0
            RUN_TEST_5_4b=1
           
YEAR=$(date +'%Y')



small_cleanup()
{
    echo "runSTest: small_cleanup"
    source /opt/nyx/xdp/release/profile.wombat
    echo "runSTest: stopMC"
    source /opt/nyx/xdp/release/bin/stopMC
    echo rs | nc localhost 8000
    sleep 5
}

large_cleanup()
{
    echo "runSTest: large_cleanup"
    source /opt/nyx/xdp/release/profile.wombat
    echo "runSTest: stopRTS"
    source /opt/nyx/xdp/release/bin/stopRTS
    echo "runSTest: kill omdc!!!"
    killall omdc
    pkill -f "omdc"
    echo "runSTest: kill RTSTestingServer!!!"
    killall RTSTestingServer
    pkill -f "RTSTestingServer"
    echo "runSTest: stopMC"
    source /opt/nyx/xdp/release/bin/stopMC
    sleep 5
}


turn_on_rts_testing_server()
{
    echo "runSTest: RTSTestingServer"
    cd ../RTSTestingServer/
    ./Debug/RTSTestingServer &
}



setup_linux_env()
{
    ##--------------------------------------------------
    ## Init Linux environment
    ##--------------------------------------------------
    sudo sh -c 'echo 2 > /proc/sys/net/ipv4/conf/lo/rp_filter'
    # round 10 sec tcp timeout
    sudo sh -c 'echo 4 > /proc/sys/net/ipv4/tcp_syn_retries'
    sudo /sbin/ifconfig lo multicast
    sudo /sbin/ifconfig eth0 -multicast                           2> /dev/null
    sudo /sbin/ifconfig eth1 -multicast                           2> /dev/null
    sudo /sbin/ifconfig eth2 -multicast                           2> /dev/null
    sudo /sbin/ifconfig eth3 -multicast                           2> /dev/null
    sudo /sbin/ifconfig eth4 -multicast                           2> /dev/null
    sudo /sbin/ifconfig eth5 -multicast                           2> /dev/null
    sudo /sbin/ifconfig eth6 -multicast                           2> /dev/null
    sudo /sbin/ifconfig eth7 -multicast                           2> /dev/null
    sudo /sbin/ifconfig eth8 -multicast                           2> /dev/null
    sudo /sbin/ifconfig eth9 -multicast                           2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth0      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth1      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth2      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth3      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth4      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth5      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth6      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth7      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth8      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev eth9      2> /dev/null
    sudo route del -net 224.0.0.0 netmask 240.0.0.0 dev lo        2> /dev/null
    sudo route add -net 224.0.0.0 netmask 240.0.0.0 dev lo        2> /dev/null

}




##==================================================


setup_linux_env

large_cleanup

cd $CURRENTFOLDER
echo "runSTest: start omdc"
./Debug/omdc &
sleep 7

while [ 1 ]
do

if [[ $RUN_TEST_5_2b -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.2b
    ##--------------------------------------------------
    
    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/Testing/OMDMcastCaptureReplay/bin/

    FILE_LOCATION_1=$DATAFILEDIR/Data-20131217/
    FILE_LOCATION_2=$DATAFILEDIR/Data-20131218/
    TAG_1=20131217-083000
    TAG_2=20131218-083000

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-1a-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-1b-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-501a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-501b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51001   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-2a-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51001   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-2b-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51001   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-502a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51001   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-502b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51002   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-3a-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51002   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-3b-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51002   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-503a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51002   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-503b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51003   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-4a-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51003   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-4b-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51003   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-504a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51003   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-504b-"$TAG_2"  lo   > /dev/null  &
    sleep 2

    ####################################################

    sleep 3600

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    small_cleanup

fi



if [[ $RUN_TEST_5_1b -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.1b
    ##--------------------------------------------------

    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/Testing/OMDMcastCaptureReplay/bin/

    FILE_LOCATION_1=$DATAFILEDIR/Data-20131217/
    FILE_LOCATION_2=$DATAFILEDIR/Data-20131218/
    TAG_1=20131217-083000
    TAG_2=20131218-083000

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.22    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-10a-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.22  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-10b-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.32    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-510a-"$TAG_2"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.32  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_2"/MCR-Capture-510b-"$TAG_2"  lo   > /dev/null &
    sleep 2

    ####################################################

    sleep 3600

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    small_cleanup

fi


if [[ $RUN_TEST_5_3a -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.3a
    ##--------------------------------------------------
    
    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/Testing/OMDMcastCaptureReplay/bin/

    FILE_LOCATION_1=$DATAFILEDIR/Data-20131217/
    FILE_LOCATION_2=$DATAFILEDIR/Data-20131218/
    TAG_1=20131217-083000
    TAG_2=20131218-083000

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.25    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-41a-"$TAG_1"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.25  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-41b-"$TAG_1"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.35    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-541a-"$TAG_1"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.35  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-541b-"$TAG_1"  lo   > /dev/null &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.26    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-42a-"$TAG_1"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.26  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-42b-"$TAG_1"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.36    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-542a-"$TAG_1"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.36  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-542b-"$TAG_1"  lo   > /dev/null &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.27    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-43a-"$TAG_1"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.27  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-43b-"$TAG_1"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.37    51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-543a-"$TAG_1"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.37  51000   $PLAYBACK_SPEED   1    9999999  "$FILE_LOCATION_1"/MCR-Capture-543b-"$TAG_1"  lo   > /dev/null &
    sleep 2

    ####################################################

    sleep 3600

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    small_cleanup

fi

if [[ $RUN_TEST_5_3b -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.3b
    ##--------------------------------------------------
    
    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/Testing/OMDMcastCaptureReplay/bin/

    FILE_LOCATION_1=$DATAFILEDIR/Data-20131217/
    FILE_LOCATION_2=$DATAFILEDIR/Data-20131218/
    TAG_1=20131217-083000
    TAG_2=20131218-083000

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.25    51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-41a-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.25  51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-41b-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.35    51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-541a-"$TAG_2"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.35  51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-541b-"$TAG_2"  lo   > /dev/null &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.26    51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-42a-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.26  51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-42b-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.36    51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-542a-"$TAG_2"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.36  51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-542b-"$TAG_2"  lo   > /dev/null &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.27    51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-43a-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.27  51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-43b-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.37    51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-543a-"$TAG_2"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.37  51000   $PLAYBACK_SPEED   1  9999999  "$FILE_LOCATION_2"/MCR-Capture-543b-"$TAG_2"  lo   > /dev/null &
    sleep 2

    ####################################################

    sleep 3600

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    small_cleanup

fi

if [[ $RUN_TEST_5_4b -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.4b
    ##--------------------------------------------------
    
    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/Testing/OMDMcastCaptureReplay/bin/

    FILE_LOCATION_1=$DATAFILEDIR/Data-20131217/
    FILE_LOCATION_2=$DATAFILEDIR/Data-20131218/
    TAG_1=20131217-083000
    TAG_2=20131218-083000

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51000   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-70a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51000   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-70b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51000   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-570a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51000   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-570b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51001   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-71a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51001   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-71b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51001   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-571a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51001   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-571b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51002   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-72a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51002   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-72b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51002   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-572a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51002   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-572b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51003   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-73a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51003   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-73b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51003   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-573a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51003   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-573b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51004   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-74a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51004   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-74b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51004   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-574a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51004   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-574b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51005   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-75a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51005   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-75b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51005   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-575a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51005   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-575b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51006   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-76a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51006   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-76b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51006   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-576a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51006   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-576b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51007   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-77a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51007   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-77b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51007   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-577a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51007   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-577b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51008   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-78a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51008   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-78b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51008   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-578a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51008   $PLAYBACK_SPEED2  1   9999999  "$FILE_LOCATION_2"/MCR-Capture-578b-"$TAG_2"  lo   > /dev/null  &
    sleep 2

    ####################################################

    sleep 3600

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    small_cleanup

fi


done
