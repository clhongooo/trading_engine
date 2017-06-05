#!/usr/bin/env python
import zmq
import random
import sys
import time
import collections
from simple_mdi_client import parse_md_feed
from threading import Thread

class StrategyBase(object):
    def __init__(self,mdi_ip,mdi_port,oti_ip,oti_port):
        self.mdi_ip   = mdi_ip  
        self.mdi_port = mdi_port
        self.oti_ip   = oti_ip  
        self.oti_port = oti_port

    ###################################################
    # OO polymorphism for overriding strategy logic
    ###################################################
    def strategy_logic(self,md_dict_a,oid_sf_tup_dict_a,last_ord_num):
        return md_dict_a,oid_sf_tup_dict_a,last_ord_num

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
        oid_sf_tup_dict = dict()
        last_ord_num = 0

        ###################################################
        # the main loop: we have total flow control
        ###################################################
        while True:
            ref_time_in_sec = time.time()

            ###################################################
            # get all execution reports
            ###################################################
            while len(shared_oti_deque) > 0:
                print shared_oti_deque.popleft()

            ###################################################
            oid_sf_tup_dict,sf_str_list,last_ord_num = self.strategy_logic(md_dict,oid_sf_tup_dict,last_ord_num)

            ###################################################
            # send signalfeed if needed
            ###################################################
            for sf_str in sf_str_list:
                print sf_str
                oti_sock.send(sf_str)

            ###################################################
            # parse md
            ###################################################
            unix_time_now = time.time()
            while len(shared_md_deque) > 0 and unix_time_now < ref_time_in_sec + 1:
                (sym,t,b,a) = parse_md_feed(shared_md_deque.popleft())
                md_dict[sym] = (t,b,a)
                unix_time_now = time.time()

            ###################################################
            # purge if too much data
            ###################################################
            if len(shared_md_deque) > 1000:
                shared_md_deque.clear()

            ###################################################
            # sleep if no new md
            ###################################################
            while True:
                if (unix_time_now - ref_time_in_sec > 1) or (len(shared_md_deque) > 0):
                    break
                unix_time_now = time.time()
                time.sleep(0.1)
