#ifndef ORDERBOOKSTRUCT_HPP
#define ORDERBOOKSTRUCT_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include "ATU_Abstract_MDI.h"

#define INVALID_PRICE 999999

using namespace std;
using namespace boost;

// Key assumptions:
// Frequency rank of different tasks:
// 1. Check order book integrity (against every HKEx order book instructions)
// 2. Update on order quantity / number of orders (including best or ordinary bids / offers)
// 3. Add / remove best bid / offer price level
// 4. Add / remove price levels other than best bid / offer
// 5. Handle snapshot requests (on subscribed instruments)
// 6. Handling HKEx instructions with minor inconsistencies (leniently).
// 
// Argument for such assumptions about frequency distribution:
// Here's an example order book for an active stock:
// 8620  12 K
// 8615  21 K
// 8610  93 K
// 8605 191 K
// 8600 866 K best offer
// 8595 170 K best bid
// 8590 545 K
// 8585 146 K
// 8580 245 K
// 8575  26 K
// 
// #1. Order book integrity has to be checked on every HKEx order book instruction, so its frequency is the highest.
// #2. There are 10 places where order quantity can change, therefore we expect this to be extremely frequent.
// #3. Best bid / offer will change places only if the orders at best bid / offer are exhausted. It should be less common than #2.
// #4. For active stocks, usually every tick level will have some orders queuing already. Therefore the frequency of adding / removing a price level in the middle of the queue is relatively low.  For inactive stocks, they won't have frequent updates by definition, so don't worry about them.
// #5. Only need to generate snapshots for certain subscribed instruments, which is only expected to be a small subset of the total number of instruments in the whole market.
// 
// For our order book case, the best candidates (in descending order) that give the best performance are:
// A. Circular array of price levels (not tick level) [offers superb performance except #4 and #5]
// B. Double-ended queue of price levels [still needs memory allocation / deallocation, slower than circular array]
// C. Doubly linked list of price levels [problem is with traversal from head / tail is required]
// D. (lightweight) doubly linked list of price levels + circular array of tick levels [maintaining 2 data structures drags performance]
// E. Regular array of tick levels
// F. Any kind of sets / maps. [have to access all elements / keys on every operation; Too slow]
// 
// Trees, (Hash) Maps are inappropriate.
// 
// To enable association of price level with array index, don't store empty price levels.
// 
// Use circular array of pre-allocated structs, or pointers to structs? Use array of pre-allocated structs. Storing pointes don't offer much performance boost, not worth the overhead in memory allocation / de-allocation.
// 
// Each element in the Order Book contains:
// . Aggregate quantity
// . Number of orders
// . Price
// 
// Lenient treatment of erroneous order book instructions:
// It is inevitable that some of the order book instructions contain contradictory information,
// whether due to missing packets or other reasons.
// This class will handle erroneous instructions on a best-effort basis.
//
// On receiving contradictory information:
// - Absolute prices have precedence over relative price levels
// - Change() instructions may be implicitly converted to Add() instructions, and vice versa
// - There is no implicit conversion for Delete()
//
// The roles of Add() and Change() is blurred after adopting lenient treatment.

struct MktOrdElement
{
  uint64_t AggQty;
  uint32_t NumOfOrd;
};

struct Agg11Element
{
  uint64_t AggQty;
  uint32_t NumOfOrd;
};

struct OBElement
{
  uint64_t AggQty;
  uint32_t NumOfOrd;
  int32_t Price;
};

class OrderBook {

  public:
    enum BASide {BID, ASK};
    static const uint16_t _MAXPRICELEVEL = 10;
    static const uint16_t _MAXTICKLEVEL = 10;
    static const uint16_t _CIRARRSIZE = _MAXPRICELEVEL+2; // For buffering in Add()

    OrderBook();
    ~OrderBook();
    void Reset();
    void Reset(BASide);

    virtual bool Add(BASide,int32_t,uint8_t,uint32_t,uint64_t);
    virtual bool Change(BASide,int32_t,uint8_t,uint32_t,uint64_t);
    virtual bool Delete(BASide,int32_t,uint8_t,uint32_t,uint64_t);
    virtual bool DeleteTopLevels(BASide,uint8_t);
    virtual string DumpJson(const uint32_t,BASide) const;
    virtual string Dump(BASide) const;
    virtual void Dump(BASide,int[_MAXPRICELEVEL][3]) const;
    virtual void Dump(ATU_MDI_marketfeed_struct &) const; // for omd_mdi

    // Info
    inline int16_t NumOfPriceLev(BASide) const;
    int16_t LocatePriceLev(BASide,int32_t) const;
    bool AssessPriceIntegrity() const;

  protected:
    boost::shared_ptr<vector<OBElement> > _BIDQ;
    boost::shared_ptr<vector<OBElement> > _ASKQ;
    int16_t _BIDQstart;
    int16_t _BIDQend;
    int16_t _ASKQstart;
    int16_t _ASKQend;

    inline OBElement* GetQue(BASide) const;
    inline int16_t & GetQueStart(BASide);
    inline int16_t & GetQueEnd(BASide);
    inline const int16_t & GetQueStartConst(BASide) const;
    inline const int16_t & GetQueEndConst(BASide) const;

    inline int16_t Bounded(int16_t) const;

};


class OrderBookOMDDSP : public OrderBook {

  public:
    OrderBookOMDDSP();
    ~OrderBookOMDDSP();
    void Reset();
    void Reset(BASide);

    virtual bool Add(BASide,int32_t,uint8_t,uint32_t,uint64_t);
    virtual bool Change(BASide,int32_t,uint8_t,uint32_t,uint64_t);
    virtual bool Delete(BASide,int32_t,uint8_t,uint32_t,uint64_t);
    virtual string DumpJson(const uint32_t,BASide) const;
    virtual string Dump(BASide) const;
    virtual void Dump(BASide,int[_MAXPRICELEVEL][3]) const;

  private:
    Agg11Element _BidAggregated11;
    Agg11Element _AskAggregated11;
    MktOrdElement _BidMktOrder;
    MktOrdElement _AskMktOrder;
};

#endif
