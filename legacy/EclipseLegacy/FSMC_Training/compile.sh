#!/bin/bash

MAKE_JOB=$(expr $(nproc) - 1)
cd Debug
make clean && make -j$MAKE_JOB all
