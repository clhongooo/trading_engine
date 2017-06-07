#!/usr/bin/env python
import zmq
import random
import sys
import time
import collections
from simple_mdi_client import parse_md_feed
from simple_oti_client import expand_to_signalfeed
from threading import Thread
from collections import namedtuple
import capi_util

WorkingOrder = namedtuple('WorkingOrder', 'oid sym px signed_qty time')

def update_working_filled_orders(feed,working_oid_sym_px_qty_time_dict,sym_pos_dict):
    fields = feed.strip('\0').strip().split(',')
    sym = fields[3]
    oid = fields[4]
    px = float(fields[5])
    qty = float(fields[6])

    if fields[1] == "signalfeed":
        signed_qty = (-1 if fields[8] == '2' else 1) * qty
        if fields[9] == "insert":
            new_working_oid_sym_px_qty_time_dict = working_oid_sym_px_qty_time_dict.copy()
            new_working_oid_sym_px_qty_time_dict[oid] = WorkingOrder(oid,sym,px,signed_qty,time.time())
            return (new_working_oid_sym_px_qty_time_dict,sym_pos_dict)

    elif fields[1] == "orderfeed":
        signed_qty = (-1 if fields[8] == '2' else 1) * qty
        isdeleted = True if int(fields[10]) == 1 else False
        new_working_oid_sym_px_qty_time_dict = working_oid_sym_px_qty_time_dict.copy()
        if isdeleted and oid in new_working_oid_sym_px_qty_time_dict:
            del new_working_oid_sym_px_qty_time_dict[oid]
        return (new_working_oid_sym_px_qty_time_dict,sym_pos_dict)

    elif fields[1] == "tradefeed":
        signed_qty = (-1 if fields[7] == '2' else 1) * qty
        new_working_oid_sym_px_qty_time_dict = working_oid_sym_px_qty_time_dict.copy()

        if oid in working_oid_sym_px_qty_time_dict:
            new_qty = working_oid_sym_px_qty_time_dict[oid].signed_qty - signed_qty
            if new_qty == 0:
                del new_working_oid_sym_px_qty_time_dict[oid]
            else:
                new_working_oid_sym_px_qty_time_dict[oid] = WorkingOrder(oid, sym, px, new_qty, time.time())

        new_sym_pos_dict = sym_pos_dict
        if sym not in new_sym_pos_dict:
            new_sym_pos_dict[sym] = 0

        new_sym_pos_dict[sym] += signed_qty

        return (new_working_oid_sym_px_qty_time_dict,new_sym_pos_dict)

    return (working_oid_sym_px_qty_time_dict,sym_pos_dict)

class StrategyBase(object):
    def __init__(self,mdi_ip,mdi_port,oti_ip,oti_port):
        self.mdi_ip   = mdi_ip  
        self.mdi_port = mdi_port
        self.oti_ip   = oti_ip  
        self.oti_port = oti_port

    ###################################################
    # OO polymorphism for overriding strategy logic
    # pure function
    ###################################################
    def strategy_logic(self):
        return

    ###################################################
    # mostly functional style
    ###################################################
    def run(self):
        ###################################################
        # md thread
        ###################################################
        shared_md_deque = collections.deque()

        def start_md_thread():
            md_context = zmq.Context()
            md_sock = md_context.socket(zmq.SUB)
            md_sock.setsockopt(zmq.SUBSCRIBE, "")
            md_sock.connect("tcp://%s:%s" % (self.mdi_ip,self.mdi_port))

            while True:
                msg = md_sock.recv()
                shared_md_deque.append(msg.strip('\0').strip())

        md_thd = Thread(target = start_md_thread)
        md_thd.daemon = True
        md_thd.start()

        ###################################################
        # oti
        ###################################################
        shared_oti_deque = collections.deque()

        oti_context = zmq.Context()
        oti_sock = oti_context.socket(zmq.PAIR)
        oti_sock.connect("tcp://%s:%s" % (self.oti_ip,self.oti_port))

        def start_oti_recv_thread():
            while True:
                msg = oti_sock.recv()
                shared_oti_deque.append(msg.strip('\0').strip())

        oti_thd = Thread(target = start_oti_recv_thread)
        oti_thd.daemon = True
        oti_thd.start()

        ###################################################
        # strategy states
        ###################################################
        md_dict = dict()
        working_oid_sym_px_qty_time_dict = dict()
        sym_pos_dict = dict()
        strat_state_list = []
        oid_sf_tup_dict = {}
        oid_base = 0

        ###################################################
        # the main loop: we have total flow control
        ###################################################
        while True:
            ###################################################
            # get all execution reports
            ###################################################
            while len(shared_oti_deque) > 0:
                feed = shared_oti_deque.popleft()
                print feed
                working_oid_sym_px_qty_time_dict,sym_pos_dict = update_working_filled_orders(feed,working_oid_sym_px_qty_time_dict,sym_pos_dict)

            ###################################################
            # strategy logic
            ###################################################
            instruction_list,strat_state_list = self.strategy_logic(md_dict,working_oid_sym_px_qty_time_dict,sym_pos_dict,strat_state_list)

            ###################################################
            # convert simple instructions to signal feed
            ###################################################
            new_signalfeed_list = map(lambda x: expand_to_signalfeed(oid_sf_tup_dict,x[1],x[0]+oid_base), enumerate(instruction_list))
            oid_base += len(instruction_list)

            for sf,sf_str in new_signalfeed_list:
                oid_sf_tup_dict[sf[4]] = (sf,sf_str)

            new_sf_str_list = map(lambda x: x[1], new_signalfeed_list)

            ###################################################
            # send signalfeed if needed
            ###################################################
            for sf_str in new_sf_str_list:
                print sf_str
                working_oid_sym_px_qty_time_dict,sym_pos_dict = update_working_filled_orders(sf_str,working_oid_sym_px_qty_time_dict,sym_pos_dict)
                oti_sock.send(sf_str)

            ###################################################
            print "working_oid_sym_px_qty_time_dict: %s sym_pos_dict: %s" % (working_oid_sym_px_qty_time_dict,sym_pos_dict)

            ###################################################
            # sleep if no new md
            ###################################################
            ref_time_in_sec = int(time.time())
            while True:
                if len(shared_oti_deque) > 0:
                    break
                if len(shared_md_deque) > 0:
                    (sym,t,b,a) = parse_md_feed(shared_md_deque.popleft())
                    md_dict[sym] = (t,b,a)
                if int(time.time()) > ref_time_in_sec:
                    ###################################################
                    # time's up, purge if can't finish processing data at all
                    ###################################################
                    shared_md_deque.clear()
                    break
                time.sleep(0.1)
