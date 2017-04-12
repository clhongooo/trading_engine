#!/bin/bash

Kill_hehe()
{
    killall TradingEngine
    killall slowcat
    ps ux | grep 21010 | awk '{print $2}' | xargs kill
    ps ux | grep 22010 | awk '{print $2}' | xargs kill
    ps ux | grep optimization.sh | awk '{print $2}' | xargs kill
}

Kill_hehe

