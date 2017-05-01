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
