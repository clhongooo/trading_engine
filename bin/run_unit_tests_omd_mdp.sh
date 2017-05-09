#!/bin/bash
LD_LIBRARY_PATH="../lib:$LD_LIBRARY_PATH"

if [[ $1 == "d" ]]
then
    gdb --args ./unit_tests_omd_mdp
elif [[ $1 == "v" ]]
then
    valgrind -v --tool=memcheck ./unit_tests_omd_mdp
else
    ./unit_tests_omd_mdp
fi
