#ifndef TRADEWAREHOUSE_H_
#define TRADEWAREHOUSE_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/ptr_container/ptr_deque.hpp>
#include "SDateTime.h"

class TradeWarehouse {
  public:
    TradeWarehouse();
    virtual ~TradeWarehouse();
    void RecordTrade(const int,const int,const string &,const double, const long);
    bool GetTradeTimeOfBasketID(const int,YYYYMMDDHHMMSS &);
    bool GetTradeDetails(const int,string &,double &,long &);
    bool RemoveBasketID(const int,const int);
    int  GetNewBasketID();
    void SetRefVariableForBasket(const int,const deque<double> &);
    deque<double> GetRefVariableForBasket(const int);
    void SetTradeTimeForBasket(const int,const YYYYMMDDHHMMSS &);
    set<int> GetAllBasketIDInGroup(const int);
    bool HasOpenPos(const int);
    int OpenPosCount(const int);
    set<int> GetAllTradeIDInBasket(const int);
  private:
    int  GetNewTradeIDNoLock();

    int                                   m_LastBasketID;
    int                                   m_LastTradeID;

    map<int,set<int> >                    m_GroupIDBasketID;
    map<int,set<int> >                    m_BasketIDTradeID;

    // Key = BasketID
    map<int,deque<double> >               m_RefVarb;
    map<int,YYYYMMDDHHMMSS>               m_TradeTime;

    // Key = TradeID
    map<int,string>   m_Symbol;
    map<int,double>   m_EntryPrice;
    map<int,long>     m_SignedPos;

    shared_mutex  m_Mutex;
};

#endif
