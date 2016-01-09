#!/usr/bin/python

import datetime
import time
import socket
import thread
from threading import Thread
from inspect import currentframe, getframeinfo
import sys
import signal

if len(sys.argv) < 2:
  print 'No argument, exiting...'
  sys.exit()

'''
Param
'''
MDI_IP='192.168.91.132'
MDI_PORT=29010

# MKTDATAFILE="/mnt/d/data/ebroker_data/hkfut4_frontmth_snap60sec_allinone.csv"
# MKTDATAFILE="/mnt/d/data/nirvana/usequities_adj_allinone.csv"
MKTDATAFILE="/mnt/d/data/nirvana/usequities_adj_allinone_recent.csv"
# MKTDATAFILE="/mnt/d/data/nirvana/usequities_unadj_allinone_recent.csv"
# MKTDATAFILE="/mnt/d/data/ibcaptured_usstk/20151007.csv"


if   (sys.argv[1] == "1"):
  MDI_PORT=21010
elif (sys.argv[1] == "2"):
  MDI_PORT=22010
elif (sys.argv[1] == "3"):
  MDI_PORT=23010
  MDI_IP='192.168.91.95'
elif (sys.argv[1] == "4"):
  MDI_PORT=24010
elif (sys.argv[1] == "5"):
  MDI_PORT=25010
elif (sys.argv[1] == "6"):
  MDI_PORT=26010
  MDI_IP='192.168.91.95'
elif (sys.argv[1] == "7"):
  MDI_PORT=27010
elif (sys.argv[1] == "8"):
  MDI_PORT=28010
elif (sys.argv[1] == "9"):
  MDI_PORT=29010
elif (sys.argv[1] == "hk"):
  MDI_PORT=29010
  # MKTDATAFILE="/mnt/d/data/ebroker_data/hkfut4_frontmth_snap60sec_allinone.csv"
  # MKTDATAFILE="/mnt/d/data/hkex/hkex_hsi_allinone.csv"
  MKTDATAFILE="/mnt/d/data/hkex/hkex_hsi_60m_allinone.csv"
  # MKTDATAFILE="/mnt/d/data/nirvana/hkequities_adj_allinone.csv"
elif (sys.argv[1] == "scenb2spy"):
  MDI_PORT=29010
  MKTDATAFILE="/mnt/d/data/nirvana/usequities_scentestspy.csv"
elif (sys.argv[1] == "b2spy"):
  MDI_PORT=29010
  MKTDATAFILE="/mnt/d/data/nirvana/usequities_adj_allinone.csv"
elif (sys.argv[1] == "b26p"):
  MDI_PORT=29010
  MKTDATAFILE="/mnt/d/data/nirvana/usequities_adj_allinone_recent.csv"



def handler(signal, frame):
    print "Ctrl-C.... Exiting"
    sys.exit(0)


def safe_cast(val, to_type, default=None):
    try:
        return to_type(val)
    except ValueError:
        return default


def readlines(sock, recv_buffer=4096, delim='\n'):
  buffer = ''
  data = True
  while data:
    data = sock.recv(recv_buffer)
    buffer += data

    while buffer.find(delim) != -1:
      line, buffer = buffer.split('\n', 1)
      yield line
  return


class MDIServer:

    def ThreadFunc1(self):
        try:
            for sktline in readlines(self.mdiconn, 4096, '\n'):
                if (sktline.find("subscri") >= 0):
                   self.subscribedSymbols.add(sktline.split(',')[3])
                elif (sktline.find("ack") >= 0):
                   self.acked = True
        except Exception as e:
           self.mdiconn.close()
           print e

    def ThreadFunc2(self):
        try:
           self.acked = True
           with open(MKTDATAFILE) as datafile:
               for fileline in datafile:
                   i=1
                   while (self.acked == False):
                       i=i+1
                   if (fileline.split(',')[1] in self.subscribedSymbols):
                       self.mdiconn.send(fileline)
                       self.acked = False
           datafile.close()
           self.mdiconn.close()
           sys.exit(0)
        except Exception as e:
           self.mdiconn.close()
           print e

    def Run(self):

        #############################################################
        # Open MDI server socket
        #############################################################
        self.mdiSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.mdiSocket.bind((MDI_IP, MDI_PORT))
        self.mdiSocket.listen(1)

        # while True:
        #############################################################
        try:
           self.mdiconn, self.addr = self.mdiSocket.accept()
           print 'MDI connection address:', self.addr

           #############################################################
           self.subscribedSymbols=set()

           t1 = Thread(target=self.ThreadFunc1)
           time.sleep(1)
           t1.daemon = True
           t1.start()

           t2 = Thread(target=self.ThreadFunc2)
           time.sleep(1)
           t2.daemon = True
           t2.start()

           t2.join()
           self.mdiconn.close()

        except Exception as e:
           self.mdiconn.close()
           print e
        #############################################################


if __name__ == '__main__':

  signal.signal(signal.SIGINT, handler)
  mdi = MDIServer()
  mdi.Run()
  while True:
      signal.pause()
