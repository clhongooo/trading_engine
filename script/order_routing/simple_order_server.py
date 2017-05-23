#!/usr/bin/env python
import sys
import time
import zmq
import argparse
from threading import Thread

received_from_client = []

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

def recv_zmq():
    global zmq_socket
    global received_from_client
    while True:
        m = zmq_socket.recv()
        print right_justify("Received: %s" % m, 200)
        received_from_client.append(m)

def send_zmq():
    global zmq_socket
    global received_from_client
    while True:
        if len(received_from_client) > 0:
            zmq_socket.send(received_from_client[0])
            print "Sent: %s" % received_from_client[0]
            del received_from_client[0]
        else:
            time.sleep(1)

###################################################
parser = argparse.ArgumentParser(description = "Pretends to be ZMQ server.")
parser.add_argument('-p','--port', dest='port', help='Server Port', required=True)
args = parser.parse_args()

###################################################
context = zmq.Context()
zmq_socket = context.socket(zmq.PAIR)
zmq_socket.bind("tcp://*:%s" % args.port)

recv_thd = Thread(target = recv_zmq)
recv_thd.daemon = True
recv_thd.start()

send_thd = Thread(target = send_zmq)
send_thd.daemon = True
send_thd.start()

while True:
    time.sleep(1)
