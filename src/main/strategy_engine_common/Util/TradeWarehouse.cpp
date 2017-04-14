#include <TradeWarehouse.h>

TradeWarehouse::TradeWarehouse() :
  m_LastBasketID(0)
{
}

TradeWarehouse::~TradeWarehouse() {
}


int TradeWarehouse::GetNewBasketID()
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);
  return (++m_LastBasketID);
}

int TradeWarehouse::GetNewTradeIDNoLock()
{
  return (++m_LastTradeID);
}


void TradeWarehouse::SetRefVariableForBasket(const int basketid, const deque<double> & d)
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);
  m_RefVarb[basketid] = d;
  return;
}

deque<double> TradeWarehouse::GetRefVariableForBasket(const int basketid)
{
  boost::shared_lock<boost::shared_mutex> lock(m_Mutex);
  map<int,deque<double> >::iterator it = m_RefVarb.find(basketid);
  if (it == m_RefVarb.end()) return deque<double>();
  return m_RefVarb[basketid];
}


void TradeWarehouse::SetTradeTimeForBasket(const int basketid,const YYYYMMDDHHMMSS & ymdhms)
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);
  m_TradeTime[basketid] = YYYYMMDDHHMMSS(ymdhms);
  return;
}

bool TradeWarehouse::HasOpenPos(const int groupid)
{
  boost::shared_lock<boost::shared_mutex> lock(m_Mutex);
  map<int,set<int> >::iterator it = m_GroupIDBasketID.find(groupid);
  if (it == m_GroupIDBasketID.end()) return false;
  return (!it->second.empty());
}

int TradeWarehouse::OpenPosCount(const int groupid)
{
  boost::shared_lock<boost::shared_mutex> lock(m_Mutex);
  map<int,set<int> >::iterator it = m_GroupIDBasketID.find(groupid);
  if (it == m_GroupIDBasketID.end()) return 0;
  return (it->second.size());
}


set<int> TradeWarehouse::GetAllBasketIDInGroup(const int groupid)
{
  boost::shared_lock<boost::shared_mutex> lock(m_Mutex);
  map<int,set<int> >::iterator it = m_GroupIDBasketID.find(groupid);
  if (it == m_GroupIDBasketID.end()) return set<int>();
  return it->second;
}
set<int> TradeWarehouse::GetAllTradeIDInBasket(const int basketid)
{
  boost::shared_lock<boost::shared_mutex> lock(m_Mutex);
  map<int,set<int> >::iterator it = m_BasketIDTradeID.find(basketid);
  if (it == m_BasketIDTradeID.end()) return set<int>();
  return it->second;
}


void TradeWarehouse::RecordTrade(const int groupid, const int basketid, const string & symbol, const double entry_price, const long signed_pos)
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);
  if (signed_pos == 0) return;

  {
    map<int,set<int> >::iterator it = m_GroupIDBasketID.find(groupid);
    if (it == m_GroupIDBasketID.end())
    {
      m_GroupIDBasketID[groupid] = set<int>();
      it = m_GroupIDBasketID.find(groupid);
    }
    it->second.insert(basketid);
  }

  int iTID = GetNewTradeIDNoLock();
  {
    map<int,set<int> >::iterator it = m_BasketIDTradeID.find(basketid);
    if (it == m_BasketIDTradeID.end())
    {
      m_BasketIDTradeID[basketid] = set<int>();
      it = m_BasketIDTradeID.find(basketid);
    }
    it->second.insert(iTID);
  }

  m_Symbol       [iTID] = symbol;
  m_EntryPrice   [iTID] = entry_price;
  m_SignedPos    [iTID] = signed_pos;

  return;
}

bool TradeWarehouse::GetTradeTimeOfBasketID(const int basketid, YYYYMMDDHHMMSS & ymdhms)
{
  boost::shared_lock<boost::shared_mutex> lock(m_Mutex);
  map<int,YYYYMMDDHHMMSS>::iterator it = m_TradeTime.find(basketid);
  if (it == m_TradeTime.end()) return false;

  ymdhms.Set(m_TradeTime[basketid]);
  return true;

}


bool TradeWarehouse::GetTradeDetails(const int tradeid, string & symbol, double & entry_price, long & signed_pos)
{
  boost::shared_lock<boost::shared_mutex> lock(m_Mutex);

  symbol      = m_Symbol[tradeid];
  entry_price = m_EntryPrice[tradeid];
  signed_pos  = m_SignedPos[tradeid];

  return true;
}

bool TradeWarehouse::RemoveBasketID(const int groupid, const int basketid)
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);

  map<int,set<int> >::iterator it = m_GroupIDBasketID.find(groupid);
  if (it == m_GroupIDBasketID.end()) return false;

  it->second.erase(basketid);
  return true;
}

