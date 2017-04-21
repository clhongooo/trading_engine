/*
 * OrderBookCache.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sunny
 */

#ifndef ORDERBOOKCACHE_H_
#define ORDERBOOKCACHE_H_

#include "OrderBook.h"
#include <map>

class OrderBookCache
{
  public:
    OrderBookCache() {}
    virtual ~OrderBookCache() {};

    virtual OrderBook* GetOrderBook(uint32_t) = 0;
    void ResetAllOrderBooks();
    void DeleteAllOrderBooks();

  protected:
    map<uint32_t,OrderBook*> m_OrderBook; //index = SecurityCode or OrderBookID
};

class OrderBookCacheOMDC : public OrderBookCache
{
  public:
    OrderBookCacheOMDC() {}
    virtual ~OrderBookCacheOMDC() {}

    OrderBook* GetOrderBook(uint32_t);
};

class OrderBookCacheOMDD : public OrderBookCache
{
  public:
    OrderBookCacheOMDD() {}
    virtual ~OrderBookCacheOMDD() {}

    OrderBook* GetOrderBook(uint32_t);
};

#endif /* ORDERBOOKCACHE_H_ */
