#!/usr/bin/env python
import sys
import time
import zmq
import argparse
from threading import Thread

JUSTIFY_COL=200

def read_file(file_loc):
    with open(file_loc,'r') as f:
        return [line for line in f]

def justify_str(s,totlen,left_right="right",padchar=' '):
    def extra(s,totlen):
        return ''.join(map(lambda x: padchar, range(totlen - len(s))))
    s = str(s)
    if left_right == "left":
        return s + extra(s,totlen)
    elif left_right == "right":
        return extra(s,totlen) + s
    else:
        return s

def right_justify(s,totlen):
    return justify_str(s,totlen,"right")
def left_justify(s,totlen):
    return justify_str(s,totlen,"left")

def get_exchg(sym):
    if sym[:3] in ["HSI","HHI","MHI","MCH"]:
        return "HKFE"
    elif sym[:2] in ["ES","NQ","YM","GC","SI"]:
        return "CME"
    else:
        return "HKFE"

def expand_to_signalfeed(oid_sf_tup_dict,fields_list,oid_suffix=0):
    def construct_tup(sf):
        return (sf, ','.join(sf))

    if len(fields_list) == 2:
        oaction,oid = fields_list
        new_sf = oid_sf_tup_dict[oid][0]
        new_sf[9] = "delete"
        return construct_tup(new_sf)
    else:
        oaction,sym,px,qty = fields_list

        exchg         = get_exchg(sym)
        ymdhms        = time.strftime("%Y%m%d_%H%M%S")
        hms           = time.strftime("%H%M%S")
        fts           = ymdhms + "_000000"
        oid           = '_'.join(map(str, ["oid",hms,oid_suffix]))
        open_or_close = "open"
        buy_or_sell   = "0" if qty >= 0 else "1"
        oaction       = "insert" if oaction.lower() == "i" else "delete"
        otype         = "limit_order"
        oval          = "today"

        sf = [fts, "signalfeed", exchg, sym, oid, px, qty, open_or_close, buy_or_sell, oaction, otype, oval]
        return construct_tup(sf)

def recv_zmq():
    global zmq_socket
    while True:
        print right_justify("Received: %s" % zmq_socket.recv(), JUSTIFY_COL)

def send_zmq(msg):
    global zmq_socket
    zmq_socket.send(msg)
    print right_justify("Sent: %s" % msg, JUSTIFY_COL)

###################################################
parser = argparse.ArgumentParser(description = "Connects to ZMQ server and places orders by constructing signal feed upon user input.")
parser.add_argument('-i','--ip', dest='ip', help='Server IP', required=True)
parser.add_argument('-p','--port', dest='port', help='Server Port', required=True)
parser.add_argument('-f','--file', dest='signalfile', help='Signal file', required=False)
args = parser.parse_args()

###################################################
context = zmq.Context()
zmq_socket= context.socket(zmq.PAIR)
zmq_socket.connect("tcp://%s:%s" % (args.ip,args.port))

recv_thd = Thread(target = recv_zmq)
recv_thd.daemon = True
recv_thd.start()
###################################################


signalfeed_list = map(lambda x: expand_to_signalfeed({},x[1].strip().split(','), x[0]), enumerate(read_file(args.signalfile))) if args.signalfile is not None else []
signalfeed_dict = dict(map(lambda x: (x[0][4], (x[0],x[1])), signalfeed_list))

def recursion(oid_sf_tup_dict):
    print '\n'.join(map(lambda x: right_justify(x[0],8) + "   " + x[1], enumerate(map(lambda x: oid_sf_tup_dict[x][1], sorted(map(lambda x: x[0][4], oid_sf_tup_dict.values()))))))
    user_input = raw_input("command [s oid | i,sym,price,qty | d,order_id | q]\n").strip()
    if len(user_input) == 0:
        recursion(oid_sf_tup_dict)
    if user_input[0] == "q":
        sys.exit(0)
    if user_input[0] == "s":
        oid = sorted(map(lambda x: x[0][4], oid_sf_tup_dict.values()))[int(user_input.split()[1])]
        send_zmq(oid_sf_tup_dict[oid][1])
        recursion(oid_sf_tup_dict)
    # else
    delim = ',' if ',' in user_input else ' '
    sf,sf_str = expand_to_signalfeed(oid_sf_tup_dict,user_input.strip().split(delim))
    send_zmq(sf_str)
    new_oid_sf_dict = oid_sf_tup_dict.copy()
    new_oid_sf_dict[sf[4]] = (sf,sf_str)
    recursion(new_oid_sf_dict)

recursion(signalfeed_dict)
