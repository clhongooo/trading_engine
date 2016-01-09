#!/bin/bash

Kill_hehe()
{
    killall TradingEngine
    killall slowcat
    ps ux | grep 21010 | awk '{print $2}' | xargs kill
}

Kill_hehe

for i in $(seq 300 300 7200)
do
    rm -f /home/qy/d/nir/nirvana_mtm.log

    STRING="s/^B2_SamplingIntervalInSec.*$/B2_SamplingIntervalInSec="$i"/"
    cat /home/qy/Dropbox/nirvana/conf/nirvana.ini | sed -e "$STRING" > haha
    mv haha /home/qy/Dropbox/nirvana/conf/nirvana.ini

    rm -f core* ./Debug/core*
    ./Debug/TradingEngine /home/qy/Dropbox/nirvana/conf/nirvana.ini &
    sleep 2
    #/home/qy/Dropbox/utilities/slowcat /mnt/d/nir/HSI_only_2006_201503 10000 500 | nc -l -p 21010 &
    cat /mnt/d/nir/HSI_only_2006_201503 | nc -l -p 21010 &
    
    while [ 1 ]
    do
        if [[ $(cat /home/qy/d/nir/nirvana_mtm.log | grep 20150320 | grep 161 | wc -l) -gt 0 ]]
        then
            killall TradingEngine
            killall slowcat
            cat /home/qy/d/nir/nirvana_mtm.log   | sed -e "s/^B2/B2\t$i/" > /home/qy/dr/nirvana/results/B2/smplgintvl/mtm_"$i"
            cat /home/qy/d/nir/nirvana_b2015.log | sed -e "s/^B2/B2\t$i/" > /home/qy/dr/nirvana/results/B2/smplgintvl/signal_"$i"
            break
        fi
        sleep 1
    done
done

Kill_hehe
