#!/usr/bin/env python
'''
modified from boreaslaw's code written in May 2, 2014
'''
import cashAlgoAPI
import random
import time
import Queue
import threading
   
                        
class Strategy:
    #Initialize Strategy
    def __init__(self,config,mgr):
        self.cnt = 0
        self.lastDate=""
        self.mgr=mgr
        self.lastTime=""
    #Process Market Data
    def onMarketDataUpdate(self,market, code, md):
        print md.timestamp+" "+md.productCode

#     if (self.cnt % 50 == 0):
#        order = cashAlgoAPI.Order(md.timestamp, "HFE", md.productCode, str(self.cnt), md.bidPrice1, 1, "open", 2, "insert", "limit_order", "today")
#        print "Place an SELL order at %s" %md.timestamp
#
#        self.mgr.insertOrder(order)
#        self.mgr.getPnLPerformance()
#    
#    self.cnt=self.cnt+1
    return

    #Process Order
    def onOrderFeed(self,of):
        print of
        return

    #Process Trade
    def onTradeFeed(self,tf):
        print tf
        return

    #Process Position
    def onPortfolioFeed(self,portfolioFeed):
       return

    def onPnlperffeed(self,pnlfeed):
        print "PnL ",pnlfeed.dailyPnL,pnlfeed.monthlyPnL,pnlfeed.yearlyPnL
        return
        
if __name__ == '__main__':
    print "Please make sure subscription date and feedcode is correct."
    feedcode="HSIM5"
 #   myQuoter = Quoter(feedcode,cutgain,cutloss,startsteps,nsteps,tickinc)
 #   myBidAskMemory = BidAskMemory()
    try:
        mgr = cashAlgoAPI.CASHOrderManager("Dummy", "DummyUser", "Pa55w0rd", "20150618", "20150618")
        mgr.start()
        myStrategy=Strategy("",mgr)

        mgr.subscribeMarketData(myStrategy.onMarketDataUpdate, "HFE", feedcode)
        mgr.registerOrderFeed(myStrategy.onOrderFeed)
        mgr.registerTradeFeed(myStrategy.onTradeFeed)
        mgr.registerPortfolioFeed(myStrategy.onPortfolioFeed)
        mgr.registerPnlperffeed(myStrategy.onPnlperffeed)
        time.sleep(1)
        time.sleep(10000)
    except:
        print "Error"
        mgr.stop()
