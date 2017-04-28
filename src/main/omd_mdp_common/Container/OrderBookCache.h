/*
 * OrderBookCache.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sunny
 */

#ifndef ORDERBOOKCACHE_H_
#define ORDERBOOKCACHE_H_

#include "OMD_MDP_Constants.h"
#include "OrderBook.h"
#include <map>

class OrderBookCache
{
  public:
    OrderBookCache(const dma::Identity);
    virtual ~OrderBookCache() {};

    virtual OrderBook* GetOrderBook(uint32_t);
    void ResetAllOrderBooks();
    void DeleteAllOrderBooks();

  protected:
    //--------------------------------------------------
    //index = SecurityCode or OrderBookID
    //--------------------------------------------------
    map<uint32_t,OrderBook*> m_OrderBook;
    dma::Identity m_Identity;
};

#endif /* ORDERBOOKCACHE_H_ */
