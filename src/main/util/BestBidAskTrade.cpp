#include "BestBidAskTrade.h"

BestBidAskTrade::BestBidAskTrade() :
  m_HHMMSS(0),
  m_BestBidVolume(0),
  m_BestAskVolume(0),
  m_BestBidPrice(0),
  m_BestAskPrice(0),
  m_LastTradePrice(0),
  m_LastTradeVolume(0)
{
}

BestBidAskTrade::~BestBidAskTrade()
{
}

void BestBidAskTrade::ChangeTime(const unsigned long hhmmss)
{
  m_HHMMSS = hhmmss;
  return;
}

void BestBidAskTrade::ChangeBestBid(const double price, const size_t volume)
{
  m_BestBidPrice  = price;
  m_BestBidVolume = volume;
  return;
}

void BestBidAskTrade::ChangeBestAsk(const double price, const size_t volume)
{
  m_BestAskPrice  = price;
  m_BestAskVolume = volume;
  return;
}

void BestBidAskTrade::ChangeLastTrade(const double price, const size_t volume)
{
  m_LastTradePrice  = price;
  m_LastTradeVolume = volume;
  return;
}

bool BestBidAskTrade::GetBestBidAskTrade(
    unsigned long& hhmmss,
    double& bidprice, size_t& bidvolume,
    double& askprice, size_t& askvolume,
    double& tradeprice, size_t& tradevolume)
{
  if (m_HHMMSS == 0 ||
      m_BestBidPrice == 0 || m_BestBidVolume == 0 ||
      m_BestAskPrice == 0 || m_BestAskVolume == 0 ||
      m_LastTradePrice == 0 // || m_LastTradeVolume == 0
      ) return false;

  hhmmss      = m_HHMMSS;
  bidprice    = m_BestBidPrice;
  bidvolume   = m_BestBidVolume;
  askprice    = m_BestAskPrice;
  askvolume   = m_BestAskVolume;
  tradeprice  = m_LastTradePrice;
  tradevolume = m_LastTradeVolume;

  return true;
}
