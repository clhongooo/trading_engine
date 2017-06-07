#!/usr/bin/env python
import sys
import time
import zmq
import argparse
from parse_capi_feeds import parse_capi_feed
from threading import Thread
from capi_util import expand_to_signalfeed

JUSTIFY_COL=200

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

    signalfeed_list = map(lambda x: expand_to_signalfeed({},x[1], x[0]), enumerate(read_file(args.signalfile))) if args.signalfile is not None else []
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
        sf,sf_str = expand_to_signalfeed(oid_sf_tup_dict,user_input,delimiter=delim)
        send_zmq(sf_str)
        new_oid_sf_dict = oid_sf_tup_dict.copy()
        new_oid_sf_dict[sf[4]] = (sf,sf_str)
        recursion(new_oid_sf_dict)

    recursion(signalfeed_dict)
