#ifndef BESTBIDASKTRADE
#define BESTBIDASKTRADE

#include "PCH.h"
#include "Constants.h"
#include <boost/cstdint.hpp>
#include <memory>
#include <boost/ptr_container/ptr_deque.hpp>

using namespace std;
using namespace boost;

class BestBidAskTrade
{
  public:
    BestBidAskTrade();
    virtual ~BestBidAskTrade();
    void ChangeBestBid(const double, const size_t);
    void ChangeBestAsk(const double, const size_t);
    void ChangeLastTrade(const double, const size_t);
    void ChangeTime(const unsigned long);
    bool GetBestBidAskTrade(unsigned long&,double&,size_t&,double&,size_t&,double&,size_t&);

  private:
    unsigned long m_HHMMSS;
    size_t m_BestBidVolume;
    size_t m_BestAskVolume;
    double m_BestBidPrice;
    double m_BestAskPrice;
    double m_LastTradePrice;
    double m_LastTradeVolume;
};

#endif
