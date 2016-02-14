#include "Acct.h"

Acct::Acct() :
  m_dCPnL(0),
  m_PosChgd(false)
{
  Reset();
}

void Acct::Reset()
{
  m_dCPnL = 0;
  m_AggPx.clear();
  m_Pos.clear();
  m_PosChgd = false;
}

double Acct::CPnL()
{
  return GetCPnL();
}

double Acct::GetCPnL()
{
  vector<string> vSym;
  map<string,long>::iterator itm;
  for (itm = m_Pos.begin(); itm != m_Pos.end(); ++itm)
  {
    if (itm->second == 0)
    {
      vSym.push_back(itm->first);
      m_dCPnL -= m_AggPx[itm->first];
    }
  }
  vector<string>::iterator itv;
  for (itv = vSym.begin(); itv != vSym.end(); ++itv)
  {
    m_Pos.erase(*itv);
    m_AggPx.erase(*itv);
  }
  return m_dCPnL;
}



void Acct::TradeWithTC(const string & symbol, long lSignedQty, double dPx, const double dPriceEquivTC)
{
  if      (lSignedQty > 0)    Trade(symbol, lSignedQty, dPx+dPriceEquivTC);
  else if (lSignedQty < 0)    Trade(symbol, lSignedQty, dPx-dPriceEquivTC);
}

void Acct::Trade(const string & symbol, long lSignedQty, double dPx)
{
  if (m_Pos.find(symbol) != m_Pos.end())
  {
    m_Pos[symbol]   = m_Pos[symbol] + lSignedQty;
    m_AggPx[symbol] = m_AggPx[symbol] + lSignedQty*dPx;
  }
  else
  {
    m_Pos[symbol]   = lSignedQty;
    m_AggPx[symbol] = lSignedQty*dPx;
  }
  m_PosChgd = true;
}

long Acct::Pos(const string & symbol)
{
  map<string,long>::iterator it = m_Pos.find(symbol);
  if (it != m_Pos.end()) return it->second;
  else return 0;
}

double Acct::GetSignedNotional(const string & symbol)
{
  map<string,double>::iterator it = m_AggPx.find(symbol);
  if (it != m_AggPx.end()) return it->second;
  else return 0;
}

const map<string,long> & Acct::GetPortfolio()
{
  return m_Pos;
}


void Acct::ProvidePriceForMTM(const string & symbol, const double price)
{
  m_PxForMTM[symbol] = price;
  return;
}


bool Acct::Get_MTM_CPnL(double & mtm_cpnl)
{
  const map<string,long> cPort = GetPortfolio();
  map<string,long> m_existing_port;

  m_existing_port.insert(cPort.begin(), cPort.end());

  //--------------------------------------------------
  // Check if prices are available for MTM
  //--------------------------------------------------
  for (map<string,long>::iterator it = m_existing_port.begin(); it != m_existing_port.end(); ++it)
  {
    string symbol = it->first;
    if (m_PxForMTM.find(symbol) == m_PxForMTM.end()) return false;
  }

  //--------------------------------------------------
  // Close positions with MTM price first
  //--------------------------------------------------
  for (map<string,long>::iterator it = m_existing_port.begin(); it != m_existing_port.end(); ++it)
  {
    string symbol = it->first;
    long pos      = it->second;
    Trade(symbol, -pos, m_PxForMTM[symbol]);
  }

  mtm_cpnl = GetCPnL();

  //--------------------------------------------------
  // Reopen positions with MTM price
  //--------------------------------------------------
  for (map<string,long>::iterator it = m_existing_port.begin(); it != m_existing_port.end(); ++it)
  {
    string symbol = it->first;
    long pos      = it->second;
    Trade(symbol, pos, m_PxForMTM[symbol]);
  }

  return true;
}


set<string> Acct::GetSymbolsWithPos()
{
  set<string> setSym;
  FForEach (m_Pos,[&](std::pair<string,long> tuple) { setSym.insert(tuple.first); });
  return setSym;
}


void Acct::SetCPnL(const double dCPnL)
{
  m_dCPnL = dCPnL;
}


void Acct::SetRecord(const string & symbol, long pos, double signed_notional)
{
  m_AggPx[symbol] = signed_notional;
  m_Pos[symbol]   = pos;
  m_PosChgd = true;
}

bool Acct::CheckIfPosChgd()
{
  return m_PosChgd;
}

void Acct::ClearPosChgFlag()
{
  m_PosChgd = false;
}

bool Acct::LoadFromFile(const string & sPosPersistFile)
{
  deque<string> dqPosPersistFile;
  STool::ReadFile(sPosPersistFile,dqPosPersistFile);
  vector<string> vs;

  for (unsigned int i = 0; i < dqPosPersistFile.size(); ++i)
  {
    vs.clear();
    boost::split(vs, dqPosPersistFile[i], is_any_of(","));

    if (vs.size() != 3) return false;

    //--------------------------------------------------
    // Update internal data struct accordingly
    //--------------------------------------------------
    string & symbol           = vs[0];
    long     signed_qty       = boost::lexical_cast<long>(vs[1]);
    double   signed_notional  = boost::lexical_cast<double>(vs[2]);

    SetRecord(symbol,signed_qty,signed_notional);
  }

  return true;
}

void Acct::WriteToFile(const string & sPosPersistFile)
{
  ofstream fsPosPersist(sPosPersistFile.c_str());

  set<string> setSym = GetSymbolsWithPos();

  for (set<string>::iterator it = setSym.begin(); it != setSym.end(); ++it)
  {
    const string & symbol = *it;
    fsPosPersist << symbol << "," << m_Pos[symbol] << "," << m_AggPx[symbol] << endl;
  }

  fsPosPersist.close();
  return;
}

