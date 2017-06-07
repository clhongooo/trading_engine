#!/usr/bin/env python
import zmq
import random
import sys
import time
import collections
from itertools import groupby
from capi_strategy_base import StrategyBase
from collections import namedtuple
from capi_util import read_file

class StrategyPortfolioRebal(StrategyBase):
    def set_instruction_file(self,instruction_file):
        self.instruction_list = read_file(instruction_file)
        self.time_threshold = 3

    def strategy_logic(self,md_dict_a,working_oid_sym_px_qty_time_dict_a,sym_pos_dict_a,strat_state_list_a):
        if len(strat_state_list_a) == 0:
            flds_list = map(lambda x: x.split(','), self.instruction_list)
            sym_target_pos_list = map(lambda x: (x[1],int(x[3])), flds_list)
            sym_target_pos_dict = dict(sym_target_pos_list)
            
            ###################################################
            target_sym_list = map(lambda x: x[0], sym_target_pos_list)
            uniq_target_sym_list = set(target_sym_list)
            no_trading = True if len(target_sym_list) != len(uniq_target_sym_list) else False
            if no_trading:
                print "Conflicting signals in input file. Please check."
            ###################################################

            sym_ori_px_dict = dict(map(lambda x: (x[1],float(x[2])), flds_list))

            return self.instruction_list,[no_trading,sym_target_pos_list,sym_target_pos_dict,sym_ori_px_dict]
        else:
            no_trading,sym_target_pos_list,sym_target_pos_dict,sym_ori_px_dict = tuple(strat_state_list_a)
            if no_trading:
                return [],strat_state_list_a
            concerned_sym_with_md_list = filter(lambda x: x in md_dict_a, set(map(lambda x: x[0], sym_target_pos_list)))

            ###################################################
            # for simplicity, just delete all overdue orders
            ###################################################
            wkg_ord_to_del = filter(lambda x: time.time() > x.time + self.time_threshold, working_oid_sym_px_qty_time_dict_a.values())
            del_oid_list = map(lambda x: x.oid, wkg_ord_to_del)
            del_sym_qty_list = map(lambda x: (x.sym,x.signed_qty), wkg_ord_to_del)
            del_sym_qty_dict = dict([(sym,sum(map(lambda x: x[1], it_sym_qty))) for sym,it_sym_qty in groupby(sorted(del_sym_qty_list, key=lambda x: x[0]), lambda x: x[0])])
            del_instruction_list = map(lambda x: ','.join(map(str,["d",x])), del_oid_list)
            ###################################################

            working_sym_qty_list = map(lambda x: (x.sym,x.signed_qty), working_oid_sym_px_qty_time_dict_a.values())
            working_sym_qty_dict = dict([(sym,sum(map(lambda x: x[1], it_sym_qty))) for sym,it_sym_qty in groupby(sorted(working_sym_qty_list, key=lambda x: x[0]), lambda x: x[0])])

            sym_diff_dict = dict(filter(lambda x: x[1] != 0, map(lambda sym: (sym, sym_target_pos_dict[sym] - sym_pos_dict_a.get(sym,0) - working_sym_qty_dict.get(sym,0) + del_sym_qty_dict.get(sym,0)), concerned_sym_with_md_list)))
            print "sym_diff_dict: %s" % sym_diff_dict
            sym_need_adjm_list = filter(lambda x: x is not None, map(lambda x: x[0] if x[1] != 0 else None, sym_diff_dict.items()))

            new_sym_ori_px_dict = dict(map(lambda x: (x[0], x[1] + ((1 if sym_diff_dict.get(x[0],0) >= 0 else -1) if x[0] in sym_need_adjm_list else 0)), sym_ori_px_dict.items()))
            ins_instruction_list = map(lambda s: ','.join(map(str, ["i",s,new_sym_ori_px_dict[s],sym_diff_dict[s]])), sym_need_adjm_list)

            return del_instruction_list+ins_instruction_list,[no_trading,sym_target_pos_list,sym_target_pos_dict,new_sym_ori_px_dict]

if __name__ == '__main__':
    sty = StrategyPortfolioRebal(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4])
    sty.set_instruction_file("port_rebal_file.txt") # you may choose not to hardcode it if you want
    sty.run()
