#!/bin/bash
echo "Press [Enter] to start, and another [Enter] to stop"
read line

cd ../../../
./bin/zmq_svr 23432 &
./bin/zmq_clt 127.0.0.1:23432 &

read line

ps aux | egrep 'zmq_svr|zmq_clt' | awk '{print $2}' | xargs kill
