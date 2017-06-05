#!/usr/bin/env python
import zmq
import random
import sys
import time
import collections
from capi_strategy_base import StrategyBase
from simple_oti_client import expand_to_signalfeed

class StrategyPlaceAndDelete(StrategyBase):
    def strategy_logic(self,md_dict_a,oid_sf_tup_dict_a,last_ord_num_a):
        new_oid_sf_tup_dict = oid_sf_tup_dict_a.copy()
        sym = "HSIM7"
        if sym not in md_dict_a:
            return new_oid_sf_tup_dict,[],last_ord_num_a

        (t,b,a) = md_dict_a[sym]

        if len(new_oid_sf_tup_dict) == 0:
            input_str = "i,%s,%s,1" % (sym,b-200)
            sf,sf_str = expand_to_signalfeed(new_oid_sf_tup_dict,input_str.split(','),last_ord_num_a)
            new_oid_sf_tup_dict[sf[4]] = (sf,sf_str)
        else:
            input_str = "d,%s" % new_oid_sf_tup_dict.keys()[0]
            sf,sf_str = expand_to_signalfeed(new_oid_sf_tup_dict,input_str.split(','),last_ord_num_a)
            del new_oid_sf_tup_dict[sf[4]]

        return new_oid_sf_tup_dict,[sf_str],last_ord_num_a+1

if __name__ == '__main__':
    sty = StrategyPlaceAndDelete(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4])
    sty.run()
