#!/bin/bash

echo "Before killing omdc"
ps aux | head -1
ps aux | grep omd   | grep -v grep

while [[ $(ps aux | grep omd | grep -v grep | wc -l) -gt 0 ]]
do
    kill $(ps aux | grep omd | grep -v grep | awk '{print $2}')
done

echo "After killing omdc"
ps aux | head -1
ps aux | grep omd   | grep -v grep
