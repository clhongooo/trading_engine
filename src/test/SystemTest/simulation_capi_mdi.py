#!/usr/bin/env python
import asyncore
import socket
import logging
from datetime import datetime
import random
import time
from threading import Thread

class Server(asyncore.dispatcher):
    def __init__(self, address):
        asyncore.dispatcher.__init__(self)
        self.logger = logging.getLogger('Server')
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.set_reuse_addr()
        self.bind(address)
        self.address = self.socket.getsockname()
        self.logger.debug('binding to %s', self.address)
        self.listen(5)

    def handle_accept(self):
        # Called when a client connects to our socket
        client_info = self.accept()
        if client_info is not None:
            self.logger.debug('handle_accept() -> %s', client_info[1])
            ClientHandler(client_info[0], client_info[1])

class ClientHandler(asyncore.dispatcher):
    def __init__(self, sock, address):
        asyncore.dispatcher.__init__(self, sock)
        self.logger = logging.getLogger('Client ' + str(address))
        self.md_tuple_list = []

        self.subscribed_symbols_set = set()
        self.subscribed_symbols_list = []
        self.last_price = {}

        self.gen_md_thd = Thread(target = self.gen_new_mkt_data)
        self.gen_md_thd.start()

        self.send_md_thd = Thread(target = self.send_mkt_data)
        self.send_md_thd.start()

    def writable(self):
        return bool(self.md_tuple_list)

    def handle_write(self):
        pass

    def handle_read(self):
        data = self.recv(1024)
        self.logger.debug('handle_read() -> "%s"', data)
        data_list = data.split('\n')
        for da in data_list:
            data_csv = map(lambda x: x.strip(), da.split(','))
            if len(data_csv) >= 4 and data_csv[1] == "subscription":
                self.subscribed_symbols_set.add(data_csv[3])
                self.subscribed_symbols_list = sorted(list(self.subscribed_symbols_set))
            # self.md_tuple_list.insert(0, "12345,acknowledgement,0,from_capi_simulator")

    def handle_close(self):
        self.logger.debug('handle_close()')
        self.close()

    def gen_new_mkt_data(self):
        ###################################################
        # new MD
        ###################################################
        while True:
            if len(self.subscribed_symbols_list) == 0 or len(self.subscribed_symbols_list) > 100:
                time.sleep(1)
                continue
            sym = self.subscribed_symbols_list[random.randint(0, len(self.subscribed_symbols_list)-1)]
            self.md_tuple_list.append((sym,random.random()/50.0))
            # self.logger.debug("len(md_tuple_list): %s" % len(self.md_tuple_list))
            time.sleep(random.random()/3.0)

    def send_mkt_data(self):
        while True:
            if len(self.md_tuple_list) == 0:
                time.sleep(0.01)
                continue

            sym,pctg_chg = self.md_tuple_list.pop()

            if sym not in self.last_price:
                self.last_price[sym] = random.randint(1,100)

            cur_px = round(float(self.last_price[sym]) * (1+pctg_chg),3)
            cur_vol = random.randint(1,10)

            self.logger.debug("%s: %s" % (sym,cur_px))

            # 20160101_003000_000000,AGG,106.805,10000,B,106.805,10000000,999999,0,999999,0,999999,0,999999,0,A,106.805,10000000,999999,0,999999,0,999999,0,999999,0
            dtstr = datetime.strftime(datetime.now(), "%Y%m%d_%H%M%S_%f")
            data = "%s,%s,%s,%s,B,%s,1,999999,0,999999,0,999999,0,999999,0,A,%s,1,999999,0,999999,0,999999,0,999999,0\n" % (dtstr,sym,cur_px,cur_vol,cur_px,cur_px)
            sent = self.send(data[:1024])
            if sent < len(data):
                remaining = data[sent:]
                self.data.to_write.append(remaining)
            # self.logger.debug('handle_write() -> (%d) "%s"', sent, data[:sent].rstrip())

def main():
    logging.basicConfig(level=logging.DEBUG, format='%(name)s:[%(levelname)s]: %(message)s')
    HOST = '0.0.0.0'
    PORT = 55030
    s = Server((HOST,PORT))
    asyncore.loop()

if __name__ == '__main__':
     main() 
