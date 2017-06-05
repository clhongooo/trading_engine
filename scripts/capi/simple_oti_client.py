#!/usr/bin/env python
import sys
import time
import zmq
import argparse
from parse_capi_feeds import parse_capi_feed
from threading import Thread

JUSTIFY_COL=200

###################################################
Color_Off="\033[0m"       # Text Reset

# Regular Colors
Black="\033[0;30m"        # Black
Red="\033[0;31m"          # Red
Green="\033[0;32m"        # Green
Yellow="\033[0;33m"       # Yellow
Blue="\033[0;34m"         # Blue
Purple="\033[0;35m"       # Purple
Cyan="\033[0;36m"         # Cyan
White="\033[0;37m"        # White

# Bold
BBlack="\033[1;30m"       # Black
BRed="\033[1;31m"         # Red
BGreen="\033[1;32m"       # Green
BYellow="\033[1;33m"      # Yellow
BBlue="\033[1;34m"        # Blue
BPurple="\033[1;35m"      # Purple
BCyan="\033[1;36m"        # Cyan
BWhite="\033[1;37m"       # White
###################################################


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
    sym_up = sym.upper()
    if sym_up[:3] in ["HSI","HHI","MHI","MCH"]:
        return "HKFE"
    elif sym_up[:2] in ["ES","NQ","YM","GC","SI","HG"]:
        return "CME"
    elif sym_up[:2] in ["AU","AG","CU"]:
        return "SHFE"
    else:
        return "HKFE"

def expand_to_signalfeed(oid_sf_tup_dict,fields_list,oid_suffix=0):
    def construct_tup(sf):
        return (sf, ','.join(map(str,sf)))

    if len(fields_list) == 2:
        oaction,oid = fields_list
        new_sf = oid_sf_tup_dict[oid][0]
        new_sf[9] = "delete"
        return construct_tup(new_sf)
    else:
        augmented_fields_list = (fields_list + [""]*10)[:5]
        oaction,sym,px,qty,openclose = augmented_fields_list

        exchg         = get_exchg(sym)
        ymdhms        = time.strftime("%Y%m%d_%H%M%S")
        hms           = time.strftime("%H%M%S")
        fts           = ymdhms + "_000000"
        oid           = '_'.join(map(str, ["oid",hms,oid_suffix]))
        open_or_close = "open" if openclose == "" else openclose
        buy_or_sell   = "1" if float(qty) >= 0 else "2"
        oaction       = "insert" if oaction.lower() == "i" else "delete"
        otype         = "limit_order"
        oval          = "today"

        sf = [fts, "signalfeed", exchg, sym, oid, px, abs(float(qty)), open_or_close, buy_or_sell, oaction, otype, oval]
        return construct_tup(sf)

def recv_zmq():
    global zmq_socket
    while True:
        m = zmq_socket.recv()
        print right_justify("Received: %s%s%s" % (BYellow,m,Color_Off), JUSTIFY_COL)
        print '\n'.join(parse_capi_feed(m,100))

def send_zmq(msg):
    global zmq_socket
    zmq_socket.send(msg)
    print right_justify("Sent: %s%s%s" % (BWhite,msg,Color_Off), JUSTIFY_COL)

if __name__ == '__main__':
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
        user_input = raw_input("command [s #n | i,sym,price,qty | d,order_id | q]\n").strip()
        print BRed + user_input + Color_Off
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
