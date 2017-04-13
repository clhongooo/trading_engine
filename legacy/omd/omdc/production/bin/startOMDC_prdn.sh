#!/bin/bash

export LD_LIBRARY_PATH=.:/usr/local/lib/:$LD_LIBRARY_PATH

if [[ $(ps aux | grep omdc | grep -v grep | wc -l) -gt 0 ]]
then
    echo "Another instance of omdc is already running. Please stop it first."
    ps aux | head -1
    ps aux | grep omdc   | grep -v grep
else
    #nohup ./omdc ../conf/Config-Production-SP-Minimal.ini &
    #nohup ./omdc ../conf/Config-Readiness-SP-918.ini &
     nohup ./omdc ../conf/Config-Readiness-SP-918-testing.ini &
    echo "OMD-C started."
fi
