#!/bin/bash
cd ../../../
./bin/zmq_svr 23432 &
./bin/zmq_clt 127.0.0.1:23432 &

read line

ps aux | egrep 'zmq_svr|zmq_clt' | awk '{print $2}' | xargs kill
