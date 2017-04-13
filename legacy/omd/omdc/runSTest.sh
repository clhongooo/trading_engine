#!/bin/bash

CURRENTFOLDER=$(pwd)

####################################################
## Prompts us for the sudo pwd immediately, don't waste time
####################################################
sudo date

##--------------------------------------------------
## 1      RTS
## 1.1    missing seq no block x 1,                config: max seq no range 1000
## 1.2    missing seq no blocks x 2,               config: max seq no range 1000
## 1.3    missing seq no blocks x 2,               config: max seq no range 50
## 1.4    missing the first 20 seq no,             config: max seq no range 1000
## 1.5    missing seq no block x 1 for 2 channels, config: max seq no range 1000
## 1.10   switching RTS servers
## 2      Line Arbitration
## 2.1    missing odd pkt in Chnl A, missing even in Chnl B
## 5.     OMD-C Readiness Test
## 5.1    Case 5,1,2,3,4 Channel 10
## 5.2    Case 5,1,2,3,4 Channel 1,2,3,4
## 5.3    Case 5,1,2,3,4 Channel 41,42,43
## 5.4    Case 5,1,2,3,4 Channel 70,71,72,73,74,75,76,77,78
## 8      Terminal
## 8.1    RESET
## 8.2.a  REFRESH all channels
## 8.2.b  REFRESH only a few channels
## 8.3    STOP
## 8.4    SHUTDOWN
## 9      Memory leakage test
## 9.1    Without any multicast
##--------------------------------------------------

LOGFILEDIR="/mnt/hgfs/nyx/"
LOGFILE="OMD.log"
DATAFILEDIR="/mnt/hgfs/nyx/"

# LOGFILEDIR="/mnt/d/log/"
# LOGFILE="OMD.log"
# DATAFILEDIR="/mnt/d/omd/"

                 INIT_RTS=0

             RUN_TEST_1_1=0
             RUN_TEST_1_2=0
             RUN_TEST_1_3=0
             RUN_TEST_1_4=0
             RUN_TEST_1_5=0
            RUN_TEST_1_10=0

             RUN_TEST_2_1=0

           PLAYBACK_SPEED=1
          PLAYBACK_SPEED2=1
  TURN_ON_RTS_TESTING_SVR=0
            RUN_TEST_5_1a=1
            RUN_TEST_5_1b=0
            RUN_TEST_5_2b=0
            RUN_TEST_5_3b=0
            RUN_TEST_5_4b=0
    CONSOLIDATE_LOG_FILES=0

             RUN_TEST_8_1=0
           RUN_TEST_8_2_a=0
           RUN_TEST_8_2_b=0
             RUN_TEST_8_3=0
             RUN_TEST_8_4=0

             RUN_TEST_9_1=0



##--------------------------------------------------
## Define Readiness Test session timeslots
##--------------------------------------------------
#Session 5
READINESS_START_TIME_SESS_5=1
READINESS_END_TIME_SESS_5=11700
#Session 1
READINESS_START_TIME_SESS_1=11700
READINESS_END_TIME_SESS_1=21000
#Session 2
READINESS_START_TIME_SESS_2=21000
READINESS_END_TIME_SESS_2=25800
#Session 3
READINESS_START_TIME_SESS_3=25800
READINESS_END_TIME_SESS_3=30000
#Session 4_1
READINESS_START_TIME_SESS_41=30000
READINESS_END_TIME_SESS_41=32400
#Session 4_2
READINESS_START_TIME_SESS_42=32400
READINESS_END_TIME_SESS_42=34200
#READINESS_END_TIME_SESS_42=37800


YEAR=$(date +'%Y')


check_log()
{
    ##--------------------------------------------------
    ## Check log file
    ##--------------------------------------------------
    bTestResult=0
    if [[ $(cat $LOGFILEDIR/$LOGFILE | grep RealTimeP | grep Processor | grep "Channel ID: $1" | grep "Seq No" | grep \ "$2"$ | wc -l) -gt 0 ||
          $(cat $LOGFILEDIR/$LOGFILE | grep RealTimeP | grep Processor | grep "Channel ID: $1" | grep "Seq No" | grep \ "$3"$ | wc -l) -gt 0 ]]
    then
        bTestResult=1
    fi

    ## ##--------------------------------------------------
    ## ## The jamming up of Logs prevent this test from working sometimes
    ## ##--------------------------------------------------
    ## failCount=0
    ## for i in $(seq 1 990)
    ## do
    ##     if [[ $(cat $LOGFILEDIR/$LOGFILE | grep RealTimeP | grep Processor | grep Seq | grep \ "$i"$ | wc -l) -eq 0 ]]
    ##     then
    ##          failCount=$(expr $failCount + 1)
    ##     fi
    ## done
    ## if [[ $failCount -gt 50 ]]
    ## then
    ##     bTestResult=0
    ## fi

    ##--------------------------------------------------
    ## Output result
    ##--------------------------------------------------
    echo "              ===================================="
    if [[ $bTestResult -eq 0 ]]
    then
        echo "              |    ScenarioTest: Failed          |"
    else
        echo "              |    ScenarioTest: Passed          |"
    fi
    echo "             ====================================="
}


check_log_rts_switch()
{
    bTestResult=1
    if [[ $(cat $LOGFILEDIR/$LOGFILE | grep Error          | grep 127 | grep 9394 | wc -l) -eq 0 ]]; then bTestResult=0 ; fi
    if [[ $(cat $LOGFILEDIR/$LOGFILE | grep Error          | grep 127 | grep 9395 | wc -l) -eq 0 ]]; then bTestResult=0 ; fi
    if [[ $(cat $LOGFILEDIR/$LOGFILE | grep Error          | grep 127 | grep 9396 | wc -l) -eq 0 ]]; then bTestResult=0 ; fi
    if [[ $(cat $LOGFILEDIR/$LOGFILE | grep Error          | grep 127 | grep 9397 | wc -l) -eq 0 ]]; then bTestResult=0 ; fi
    if [[ $(cat $LOGFILEDIR/$LOGFILE | grep "Connected to" | grep 127 | grep 6969 | wc -l) -ne 1 ]]; then bTestResult=0 ; fi

    ##--------------------------------------------------
    ## Output result
    ##--------------------------------------------------
    echo "              ===================================="
    if [[ $bTestResult -eq 0 ]]
    then
        echo "              |    ScenarioTest: Failed          |"
    else
        echo "              |    ScenarioTest: Passed          |"
    fi
    echo "             ====================================="
}

##--------------------------------------------------
## Clean up before the next test
##--------------------------------------------------
small_cleanup()
{
    echo "runSTest: small_cleanup"
    echo "runSTest: kill omdc!!!"
    killall omdc
    pkill -f "omdc"
    echo "runSTest: stopMC"
    source /opt/nyx/xdp/release/bin/stopMC
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


prep_data()
{
    if [[ ! -d /opt/nyx/xdp/release/data/binary_testing ]]
    then
        mkdir /opt/nyx/xdp/release/data/binary_testing
    fi

    cd /opt/nyx/xdp/release/bin
    source /opt/nyx/xdp/release/bin/truncPacketInCannedData.wrap
    source /opt/nyx/xdp/release/bin/removePacketInCannedData.wrap
    cd $CURRENTFOLDER
}




##==================================================


prep_data

setup_linux_env

large_cleanup


if [[ $INIT_RTS -eq 1 ]]
then
    ##--------------------------------------------------
    ## Feed full multicast data to RTS server to warm it up
    ##--------------------------------------------------
    echo "runSTest: startUTestRTS"
    source /opt/nyx/xdp/release/bin/startUTestRTS
    echo "runSTest: startSTestMC1"
    source /opt/nyx/xdp/release/bin/startSTestMC1
    echo "runSTest: stopMC"
    source /opt/nyx/xdp/release/bin/stopMC
fi



if [[ $RUN_TEST_1_1 -eq 1 ]]
then
     ##--------------------------------------------------
     ## Scenario Test 1.1
     ##--------------------------------------------------
     cd $CURRENTFOLDER
     echo "runSTest: start omdc"
     ./Debug/omdc STest 1.1 &
     sleep 10

     echo "runSTest: startSTestMC2"
     source /opt/nyx/xdp/release/bin/startSTestMC2

     sleep 90
     check_log 20 989 990
     small_cleanup

fi


if [[ $RUN_TEST_1_2 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 1.2
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 1.2 &
    sleep 10

    echo "runSTest: startSTestMC3"
    source /opt/nyx/xdp/release/bin/startSTestMC3

    sleep 90
    check_log 20 989 990
    small_cleanup
fi


if [[ $RUN_TEST_1_3 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 1.3
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 1.3 &
    sleep 10

    echo "runSTest: startSTestMC3"
    source /opt/nyx/xdp/release/bin/startSTestMC3

    sleep 90
    check_log 20 989 990
    small_cleanup
fi


if [[ $RUN_TEST_1_4 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 1.4
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 1.4 &
    sleep 10

    echo "runSTest: startSTestMC4"
    source /opt/nyx/xdp/release/bin/startSTestMC4

    sleep 90
    check_log 20 989 990
    small_cleanup
fi


if [[ $RUN_TEST_1_5 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 1.5
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 1.5 &
    sleep 10

    echo "runSTest: startSTestMC5"
    source /opt/nyx/xdp/release/bin/startSTestMC5

    sleep 90
    check_log 20 989 990
    check_log 21 989 990
    small_cleanup

fi

if [[ $RUN_TEST_1_10 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 1.10
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 1.10 &
    sleep 5

    # Reuse startSTestMC2
    echo "runSTest: startSTestMC2"
    source /opt/nyx/xdp/release/bin/startSTestMC2

    sleep 5

    check_log_rts_switch

    small_cleanup
fi

if [[ $RUN_TEST_2_1 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 2.1
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 2.1 &
    sleep 10

    echo "runSTest: startSTestMC6"
    source /opt/nyx/xdp/release/bin/startSTestMC6

    sleep 15
    check_log 41 29989 29990
    small_cleanup

fi

if [[ $TURN_ON_RTS_TESTING_SVR -eq 1 ]]
then
    turn_on_rts_testing_server
fi

if [[ $RUN_TEST_5_1a -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.1a
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc &
    sleep 10

    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/../omd_common_testing/Testing/OMDMcastCaptureReplay/bin/

    INTENDSTARTTIME=$(expr 1000 \* $(expr 300 + $(date +'%s')))
    echo -n "Time now           : "
    date
    INTENDSTARTTIMESTR=$(date --date="@$(expr $INTENDSTARTTIME \/ 1000)")
    echo    "Intended start time: $INTENDSTARTTIMESTR"

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.23    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_20    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.23  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_20    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.33    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_20    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.33  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_20    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.23    51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_21    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.23  51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_21    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.33    51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_21    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.33  51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_21    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.23    51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_22    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.23  51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_22    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.33    51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_22    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.33  51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_22    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.23    51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_23    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.23  51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_23    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.33    51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_23    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.33  51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_23    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.23    51004   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_24    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.23  51004   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_24    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.33    51004   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_24    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.33  51004   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_24    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.23    51005   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_25    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.23  51005   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_25    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.33    51005   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_25    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.33  51005   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_25    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.23    51006   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_26    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.23  51006   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_26    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.33    51006   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_26    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.33  51006   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_26    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.23    51007   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_27    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.23  51007   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_27    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.33    51007   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_27    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.33  51007   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_27    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.23    51008   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_28    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.23  51008   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_28    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.33    51008   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_28    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.33  51008   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_28    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_01    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_01    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_01    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_01    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_02    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_02    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_02    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_02    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_03    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_03    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_03    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_03    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_04    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_04    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_04    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_04    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_70    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_70    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_70    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_70    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_71    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_71    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_71    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51001   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_71    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_72    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_72    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_72    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51002   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_72    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_73    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_73    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_73    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51003   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_73    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51004   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_74    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51004   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_74    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51004   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_74    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51004   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_74    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51005   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_75    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51005   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_75    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51005   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_75    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51005   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_75    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51006   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_76    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51006   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_76    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51006   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_76    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51006   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_76    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51007   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_77    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51007   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_77    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51007   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_77    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51007   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_77    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51008   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_78    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51008   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_78    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51008   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_78    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51008   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_78    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.25    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_41    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.25  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_41    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.35    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_41    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.35  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_41    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.26    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_42    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.26  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_42    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.36    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_42    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.36  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_42    lo   > /dev/null  &
    sleep 5; echo -n "."
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.27    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_A_43    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.27  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RT_B_43    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.37    51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_A_43    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.37  51000   $INTENDSTARTTIME   1  1020  30120   $DATAFILEDIR/Data918-20140707/cusfmt-20140707-083854-RF_B_43    lo   > /dev/null  &
    sleep 5; echo -n "."
    echo "Finished starting McastCaptureReplay programs!"

    # further sleep make a total of 5 min
    sleep 175

    ####################################################
    # 1020 = 17 min
    # 083854 = 518
    # 08:45 = 525
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 43 + $(expr 10 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 44 + $(expr 10 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 45 + $(expr 10 \* 60)) - 525) \* 60) ; echo rs   | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 57 + $(expr 11 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 58 + $(expr 11 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 59 + $(expr 11 \* 60)) - 525) \* 60) ; echo rs   | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 42 + $(expr 14 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 43 + $(expr 14 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 44 + $(expr 14 \* 60)) - 525) \* 60) ; echo rs   | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 57 + $(expr 15 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 58 + $(expr 15 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 59 + $(expr 15 \* 60)) - 525) \* 60) ; echo rs   | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 12 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 13 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 14 + $(expr 17 \* 60)) - 525) \* 60) ; echo rs   | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 30 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 31 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 32 + $(expr 17 \* 60)) - 525) \* 60) ; echo rs   | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 15 + $(expr 17 \* 60)) - 525) \* 60) ; echo log5 | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 16 + $(expr 17 \* 60)) - 525) \* 60) ; echo log5 | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 17 + $(expr 17 \* 60)) - 525) \* 60) ; echo ct   | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 18 + $(expr 17 \* 60)) - 525) \* 60) ; echo ct   | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 19 + $(expr 17 \* 60)) - 525) \* 60) ; echo ct   | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 20 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 21 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 22 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 23 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 24 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 25 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 26 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 27 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 28 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 29 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 30 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 31 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 32 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 33 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 34 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 40 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 41 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 42 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 43 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 44 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 45 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 46 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 47 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 48 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 49 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 50 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 51 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 52 + $(expr 17 \* 60)) - 525) \* 60) ; echo lsn  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 53 + $(expr 17 \* 60)) - 525) \* 60) ; echo cbd  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"; sleep $(expr $(expr $(expr 54 + $(expr 17 \* 60)) - 525) \* 60) ; echo rfc  | nc localhost 8000 &  #| nc -w 1 localhost 8000 &
    sleep 15; pkill -f "nc localhost 8000"

    sleep 300

    ####################################################

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    mv $LOGFILEDIR/$LOGFILE $LOGFILEDIR/OMD-5.1a-$(date +'%Y%m%d-%H%M').log

    small_cleanup

fi

if [[ $RUN_TEST_5_2b -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.2b
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 5.2 &
    sleep 7

    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/Testing/OMDMcastCaptureReplay/bin/

    INTENDSTARTTIME=$(expr 1000 \* $(expr 30 + $(date +'%s')))

    FILE_LOCATION_1=$DATAFILEDIR/Data-20131217/
    FILE_LOCATION_2=$DATAFILEDIR/Data-20131218/
    TAG_1=20131217-083000
    TAG_2=20131218-083000

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-1a-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-1b-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-501a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-501b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51001   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-2a-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51001   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-2b-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51001   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-502a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51001   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-502b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51002   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-3a-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51002   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-3b-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51002   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-503a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51002   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-503b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.21    51003   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-4a-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.21  51003   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-4b-"$TAG_2"    lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.31    51003   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-504a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.31  51003   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-504b-"$TAG_2"  lo   > /dev/null  &
    sleep 2

    ####################################################

    sleep $(expr $(expr $READINESS_END_TIME_SESS_5 - $READINESS_START_TIME_SESS_5) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.2-sess5-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_1 - $READINESS_START_TIME_SESS_1) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.2-sess1-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_2 - $READINESS_START_TIME_SESS_2) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.2-sess2-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_3 - $READINESS_START_TIME_SESS_3) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.2-sess3-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_41 - $READINESS_START_TIME_SESS_41) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.2-sess41-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_42 - $READINESS_START_TIME_SESS_42) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.2-sess42-$(date +'%Y%m%d-%H%M').log

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    mv $LOGFILEDIR/$LOGFILE $LOGFILEDIR/OMD-5.2-$(date +'%Y%m%d-%H%M').log
    small_cleanup

fi


if [[ $RUN_TEST_5_1b -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.1b
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 5.1 &
    sleep 5

    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/Testing/OMDMcastCaptureReplay/bin/

    INTENDSTARTTIME=$(expr 1000 \* $(expr 30 + $(date +'%s')))

    FILE_LOCATION_1=$DATAFILEDIR/Data-20131217/
    FILE_LOCATION_2=$DATAFILEDIR/Data-20131218/
    TAG_1=20131217-083000
    TAG_2=20131218-083000

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.22    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-10a-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.22  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-10b-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.32    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-510a-"$TAG_2"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.32  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-510b-"$TAG_2"  lo   > /dev/null &
    sleep 2

    ####################################################

    sleep $(expr $(expr $READINESS_END_TIME_SESS_5 - $READINESS_START_TIME_SESS_5) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.1-sess5-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_1 - $READINESS_START_TIME_SESS_1) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.1-sess1-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_2 - $READINESS_START_TIME_SESS_2) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.1-sess2-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_3 - $READINESS_START_TIME_SESS_3) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.1-sess3-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_41 - $READINESS_START_TIME_SESS_41) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.1-sess41-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_42 - $READINESS_START_TIME_SESS_42) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.1-sess42-$(date +'%Y%m%d-%H%M').log

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    mv $LOGFILEDIR/$LOGFILE $LOGFILEDIR/OMD-5.1-$(date +'%Y%m%d-%H%M').log

    small_cleanup

fi


if [[ $RUN_TEST_5_3b -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.3b
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 5.3 &
    sleep 5

    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/Testing/OMDMcastCaptureReplay/bin/

    INTENDSTARTTIME=$(expr 1000 \* $(expr 30 + $(date +'%s')))

    FILE_LOCATION_1=$DATAFILEDIR/Data-20131217/
    FILE_LOCATION_2=$DATAFILEDIR/Data-20131218/
    TAG_1=20131217-083000
    TAG_2=20131218-083000

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.25    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-41a-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.25  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-41b-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.35    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-541a-"$TAG_2"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.35  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-541b-"$TAG_2"  lo   > /dev/null &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.26    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-42a-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.26  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-42b-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.36    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-542a-"$TAG_2"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.36  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-542b-"$TAG_2"  lo   > /dev/null &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.27    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-43a-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.27  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-43b-"$TAG_2"   lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.37    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-543a-"$TAG_2"  lo   > /dev/null &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.37  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED   $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-543b-"$TAG_2"  lo   > /dev/null &
    sleep 2

    ####################################################

    sleep $(expr $(expr $READINESS_END_TIME_SESS_5 - $READINESS_START_TIME_SESS_5) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.3-sess5-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_1 - $READINESS_START_TIME_SESS_1) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.3-sess1-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_2 - $READINESS_START_TIME_SESS_2) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.3-sess2-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_3 - $READINESS_START_TIME_SESS_3) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.3-sess3-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_41 - $READINESS_START_TIME_SESS_41) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.3-sess41-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_42 - $READINESS_START_TIME_SESS_42) / $PLAYBACK_SPEED) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.3-sess42-$(date +'%Y%m%d-%H%M').log

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    mv $LOGFILEDIR/$LOGFILE $LOGFILEDIR/OMD-5.3-$(date +'%Y%m%d-%H%M').log
    small_cleanup

fi

if [[ $RUN_TEST_5_4b -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 5.4b
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 5.4 &
    sleep 7

    echo "runSTest: OMDMcastCaptureReplay"
    cd $CURRENTFOLDER/Testing/OMDMcastCaptureReplay/bin/

    INTENDSTARTTIME=$(expr 1000 \* $(expr 60 + $(date +'%s')))

    FILE_LOCATION_1=$DATAFILEDIR/Data-20131217/
    FILE_LOCATION_2=$DATAFILEDIR/Data-20131218/
    TAG_1=20131217-083000
    TAG_2=20131218-083000

    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-70a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-70b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51000   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-570a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51000   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-570b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51001   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-71a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51001   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-71b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51001   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-571a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51001   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-571b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51002   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-72a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51002   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-72b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51002   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-572a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51002   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-572b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51003   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-73a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51003   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-73b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51003   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-573a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51003   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-573b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51004   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-74a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51004   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-74b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51004   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-574a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51004   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-574b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51005   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-75a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51005   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-75b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51005   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-575a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51005   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-575b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51006   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-76a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51006   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-76b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51006   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-576a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51006   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-576b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51007   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-77a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51007   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-77b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51007   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-577a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51007   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-577b-"$TAG_2"  lo   > /dev/null  &
    sleep 2
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.29    51008   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-78a-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.29  51008   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-78b-"$TAG_2"   lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.1.39    51008   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-578a-"$TAG_2"  lo   > /dev/null  &
    java -Xmx10m McastCaptureReplay  Replay  239.1.127.39  51008   $INTENDSTARTTIME   $PLAYBACK_SPEED2  $READINESS_START_TIME_SESS_5  $READINESS_END_TIME_SESS_42  "$FILE_LOCATION_2"/MCR-Capture-578b-"$TAG_2"  lo   > /dev/null  &
    sleep 2

    ####################################################

    sleep $(expr $(expr $READINESS_END_TIME_SESS_5 - $READINESS_START_TIME_SESS_5) / $PLAYBACK_SPEED2) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.4-sess5-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_1 - $READINESS_START_TIME_SESS_1) / $PLAYBACK_SPEED2) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.4-sess1-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_2 - $READINESS_START_TIME_SESS_2) / $PLAYBACK_SPEED2) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.4-sess2-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_3 - $READINESS_START_TIME_SESS_3) / $PLAYBACK_SPEED2) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.4-sess3-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_41 - $READINESS_START_TIME_SESS_41) / $PLAYBACK_SPEED2) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.4-sess41-$(date +'%Y%m%d-%H%M').log
    sleep $(expr $(expr $READINESS_END_TIME_SESS_42 - $READINESS_START_TIME_SESS_42) / $PLAYBACK_SPEED2) # playback
    #tail -1 $LOGFILEDIR/$LOGFILE > $LOGFILEDIR/OMD-5.4-sess42-$(date +'%Y%m%d-%H%M').log

    pkill -f "java -Xmx10m McastCaptureReplay"
    pkill -f "java DataCaptureReplay"

    mv $LOGFILEDIR/$LOGFILE $LOGFILEDIR/OMD-5.4-$(date +'%Y%m%d-%H%M').log
    small_cleanup

fi

if [[ $CONSOLIDATE_LOG_FILES -eq 1 ]]
then

    rm -f $LOGFILEDIR/OMD-only-sess5.log
    rm -f $LOGFILEDIR/OMD-only-sess1.log
    rm -f $LOGFILEDIR/OMD-only-sess2.log
    rm -f $LOGFILEDIR/OMD-only-sess3.log
    rm -f $LOGFILEDIR/OMD-only-sess41.log
    rm -f $LOGFILEDIR/OMD-only-sess42.log

    for i in $(seq 1 4)
    do
        cat $LOGFILEDIR/OMD-5."$i"-only-sess5.log  >> $LOGFILEDIR/OMD-only-sess5.log
        cat $LOGFILEDIR/OMD-5."$i"-only-sess1.log  >> $LOGFILEDIR/OMD-only-sess1.log
        cat $LOGFILEDIR/OMD-5."$i"-only-sess2.log  >> $LOGFILEDIR/OMD-only-sess2.log
        cat $LOGFILEDIR/OMD-5."$i"-only-sess3.log  >> $LOGFILEDIR/OMD-only-sess3.log
        cat $LOGFILEDIR/OMD-5."$i"-only-sess41.log >> $LOGFILEDIR/OMD-only-sess41.log
        cat $LOGFILEDIR/OMD-5."$i"-only-sess42.log >> $LOGFILEDIR/OMD-only-sess42.log
    done

    cd $CURRENTFOLDER
    ./chkReadinessTestSS.sh

fi


if [[ $RUN_TEST_8_1 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 8.1
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 1.1 & # just load a non-production config file
    sleep 5
    echo reset | nc localhost 8000 &
    sleep 2
    killall nc
    sleep 5

    echo "              ===================================="
    if [[ $(cat $LOGFILEDIR/$LOGFILE | grep TerminalSession | grep reset | wc -l) -eq 0 ]]
    then
        echo "              |    ScenarioTest: Failed          |"
    else
        echo "              |    ScenarioTest: Passed          |"
    fi
    echo "             ====================================="

    small_cleanup
fi

if [[ $RUN_TEST_8_2_a -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 8.2.a
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 5.2 & # just reuse the config file from another test
    sleep 5
    echo refresh | nc localhost 8000 &
    sleep 2
    killall nc
    sleep 5

    bTestResult=1

    for i in 1 2 3 4 70 71 72 73 74 75 76 77 78
    do
       if [[ $(cat $LOGFILEDIR/$LOGFILE | grep TerminalSession | grep "Manually activated" | grep "Channel $i." | wc -l) -eq 0 ]]; then bTestResult=0; fi
    done

    echo "              ===================================="
    if [[ $bTestResult -eq 0 ]]
    then
        echo "              |    ScenarioTest: Failed          |"
    else
        echo "              |    ScenarioTest: Passed          |"
    fi
    echo "             ====================================="

    small_cleanup
fi

if [[ $RUN_TEST_8_2_b -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 8.2.b
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 5.2 & # just reuse the config file from another test
    sleep 5
    echo "refresh 71 76 3" | nc localhost 8000 &
    sleep 2
    killall nc
    sleep 5

    bTestResult=1

    for i in 3 71 76
    do
       if [[ $(cat $LOGFILEDIR/$LOGFILE | grep TerminalSession | grep "Manually activated" | grep "Channel $i." | wc -l) -eq 0 ]]; then bTestResult=0; fi
    done

    for i in 1 2 4 70 72 73 74 75 77 78
    do
       if [[ $(cat $LOGFILEDIR/$LOGFILE | grep TerminalSession | grep "Manually activated" | grep "Channel $i." | wc -l) -gt 0 ]]; then bTestResult=0; fi
    done

    echo "              ===================================="
    if [[ $bTestResult -eq 0 ]]
    then
        echo "              |    ScenarioTest: Failed          |"
    else
        echo "              |    ScenarioTest: Passed          |"
    fi
    echo "             ====================================="

    small_cleanup
fi

if [[ $RUN_TEST_8_3 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 8.2
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 1.1 & # just load a non-production config file
    sleep 5
    echo stop | nc localhost 8000 &
    sleep 2
    killall nc
    sleep 5

    echo "              ===================================="
    if [[ $(cat $LOGFILEDIR/$LOGFILE | grep TerminalSession | grep stop | wc -l) -eq 0 ]]
    then
        echo "              |    ScenarioTest: Failed          |"
    else
        echo "              |    ScenarioTest: Passed          |"
    fi
    echo "             ====================================="

    small_cleanup

fi

if [[ $RUN_TEST_8_4 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 8.2
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    ./Debug/omdc STest 1.1 & # just load a non-production config file
    sleep 5
    echo shutdown | nc localhost 8000 &
    sleep 2
    killall nc
    sleep 5

    echo "              ===================================="
    if [[ $(cat $LOGFILEDIR/$LOGFILE | grep TerminalSession | grep shutdown | wc -l) -eq 0 ]]
    then
        echo "              |    ScenarioTest: Failed          |"
    else
        echo "              |    ScenarioTest: Passed          |"
    fi
    echo "             ====================================="

    small_cleanup

fi

if [[ $RUN_TEST_9_1 -eq 1 ]]
then
    ##--------------------------------------------------
    ## Scenario Test 9.1
    ##--------------------------------------------------
    cd $CURRENTFOLDER
    echo "runSTest: start omdc"
    valgrind ./Debug/omdc
    small_cleanup

fi



large_cleanup
