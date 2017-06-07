#!/usr/bin/env python
import zmq
import random
import sys
import time
import collections
from capi_strategy_base import StrategyBase
import capi_util

class StrategyPlaceAndDelete(StrategyBase):

    def strategy_logic(self,md_dict_a,working_oid_sym_px_qty_time_dict_a,sym_pos_dict_a,strat_state_list_a):
        sym = "HSIM7"
        if sym not in md_dict_a:
            return [],[]

        (t,b,a) = md_dict_a[sym]

        if len(working_oid_sym_px_qty_time_dict_a.keys()) == 0:
            instruction = "i,%s,%s,1" % (sym,b-200)
        else:
            instruction = "d,%s" % working_oid_sym_px_qty_time_dict_a.keys()[0]

        return [instruction],[]

if __name__ == '__main__':
    sty = StrategyPlaceAndDelete(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4])
    sty.run()
