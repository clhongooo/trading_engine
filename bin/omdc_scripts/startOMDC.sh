#!/bin/bash

if [[ $(ps aux | grep Debug/omdc   | grep -v grep | wc -l) -gt 0 ||
      $(ps aux | grep Release/omdc | grep -v grep | wc -l) -gt 0 ]]
then
    echo "Another instance of omdc is already running. Please stop it first."
    ps aux | head -1
    ps aux | grep Debug/omdc   | grep -v grep
    ps aux | grep Release/omdc | grep -v grep
else
    nohup ./Debug/omdc &
fi
