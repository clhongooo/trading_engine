#!/usr/bin/env python
import sys
import time
import zmq
import argparse
# import random
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

def in_colour(s,colour):
    return colour + str(s) + Color_Off
###################################################

MD_DICT = {} # non-threadsafe, anyway

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

def parse_md_feed(mf):
    fields = mf.strip().split(',')
    if len(fields) != 26:
        return (None,None,None,None)
    t = float(fields[2])
    b = float(fields[5])
    a = float(fields[16])
    return (fields[1],t,b,a)

def recv_zmq():
    global zmq_socket
    while True:
        sym,t,b,a = parse_md_feed(zmq_socket.recv())
        if sym is None:
            continue
        MD_DICT[sym] = (t,b,a)

def send_zmq(msg):
    global zmq_socket
    zmq_socket.send(msg)
    print right_justify("Sent: %s%s%s" % (BWhite,msg,Color_Off), JUSTIFY_COL)

###################################################
parser = argparse.ArgumentParser(description = "Connects to ZMQ server and gets market data for symbols.")
parser.add_argument('-i','--ip', dest='ip', help='Server IP', required=True)
parser.add_argument('-p','--port', dest='port', help='Server Port', required=True)
parser.add_argument('-f','--file', dest='symbolfile', help='Symbol file', required=False)
args = parser.parse_args()

###################################################
context = zmq.Context()
zmq_socket= context.socket(zmq.SUB)
zmq_socket.setsockopt(zmq.SUBSCRIBE, "")
zmq_socket.connect("tcp://%s:%s" % (args.ip,args.port))

recv_thd = Thread(target = recv_zmq)
recv_thd.daemon = True
recv_thd.start()
###################################################


while True:
    print in_colour("%s" % time.strftime("%Y-%m-%d %H:%M:%S"), BBlack)

    # ###################################################
    # # for testing
    # ###################################################
    # MD_DICT = dict(map(lambda x: ("sym"+str(x),(30.+random.randint(1, 10)/10.,30.3,30.6)),range(1,10)))
    # ###################################################

    for k,v in sorted(MD_DICT.items()):
        t,b,a = tuple(map(float,v))
        sys.stdout.write (in_colour(k,BBlue) + " " + right_justify("T: ",5) + right_justify(t,10) + right_justify("B/A: ",10))

        price6 = [t if t  < b else "",
                  t if t == b else b,
                  t if (t  > b and t  < a) else "",
                  t if t == a else a,
                  t if t  > a else "",
                  str(round((a-b)/(a+b)*200.,2))+"%" if (abs(a) > 0.0001 and abs(b) > 0.0001) else "Nil"]

        displ6 = [BBlack,
                  Red if t <= b else Color_Off,
                  Yellow,
                  Green if t >= a else Color_Off,
                  BBlack,
                  Color_Off]

        print ' '.join(map(lambda x: in_colour(right_justify(x[0],10),x[1]), zip(price6,displ6)))

    time.sleep(1)
