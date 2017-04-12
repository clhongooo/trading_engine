#!/bin/bash
ps aux | grep simulation_capi_mdi | awk '{print $2}' | xargs kill
