#include <CPnLHist.h>

CPnLHist::CPnLHist()
{
}

CPnLHist::~CPnLHist()
{
}

void CPnLHist::Reset()
{
  m_map_hist_cpnl.clear();
  return;
}

bool CPnLHist::LoadFromFile(const string & sCPnLHistPersistFile)
{
  deque<string> dqCPnLPersistFile;
  STool::ReadFile(sCPnLHistPersistFile,dqCPnLPersistFile);
  vector<string> vs;

  for (unsigned int i = 0; i < dqCPnLPersistFile.size(); ++i)
  {
    vs.clear();
    boost::split(vs, dqCPnLPersistFile[i], is_any_of(","));

    if (vs.size() != 2)
    {
      return false;
    }

    //--------------------------------------------------
    // Update internal data struct accordingly
    //--------------------------------------------------
    YYYYMMDD ymd(vs[0]);
    double cpnl = boost::lexical_cast<double>(vs[1]);

    AddAndTruncate(ymd,cpnl);
  }

  return true;
}

void CPnLHist::WriteToFile(const string & sCPnLHistPersistFile)
{
  ofstream fsCPnLHistPersist(sCPnLHistPersistFile.c_str());

  for (map<YYYYMMDD,cpnl_idx_tuple>::iterator it = m_map_hist_cpnl.begin(); it != m_map_hist_cpnl.end(); ++it)
  {
    fsCPnLHistPersist << it->first.ToStr_() << "," << it->second.m_cpnl << endl;
  }

  fsCPnLHistPersist.close();
  return;
}


void CPnLHist::AddAndTruncate(const YYYYMMDD & ymd, const double cpnl)
{
  if (std::isnan(cpnl)) return;

  if (m_map_hist_cpnl.empty())
  {
    cpnl_idx_tuple cit;
    cit.m_cpnl = cpnl;
    cit.m_idx  = 0;

    m_map_hist_cpnl[ymd] = cit;
    return;
  }

  map<YYYYMMDD,cpnl_idx_tuple>::iterator it = m_map_hist_cpnl.find(ymd);
  if (it != m_map_hist_cpnl.end())
  {
    cpnl_idx_tuple cit;
    cit.m_cpnl = cpnl;
    cit.m_idx  = it->second.m_idx;

    m_map_hist_cpnl.erase(it,m_map_hist_cpnl.end());
    m_map_hist_cpnl[ymd] = cit;
    return;
  }

  //--------------------------------------------------
  it = m_map_hist_cpnl.end();
  it--;

  //--------------------------------------------------
  // Last one
  //--------------------------------------------------
  if (it->first < ymd)
  {
    cpnl_idx_tuple cit;
    cit.m_cpnl = cpnl;
    cit.m_idx  = it->second.m_idx+1;

    m_map_hist_cpnl[ymd] = cit;
    return;
  }

  //--------------------------------------------------
  // Search from tail
  //--------------------------------------------------
  while (it->first > ymd && it != m_map_hist_cpnl.begin()) it--;

  it++;

  cpnl_idx_tuple cit;
  cit.m_cpnl = cpnl;
  cit.m_idx  = it->second.m_idx;

  m_map_hist_cpnl.erase(it,m_map_hist_cpnl.end());
  m_map_hist_cpnl[ymd] = cit;

  return;
}

double CPnLHist::GetPnL(const YYYYMMDD & ymd_start, const YYYYMMDD & ymd_end)
{
  if (ymd_end < ymd_start) return NAN;

  map<YYYYMMDD,cpnl_idx_tuple>::iterator its;
  map<YYYYMMDD,cpnl_idx_tuple>::iterator ite;
  its = m_map_hist_cpnl.find(ymd_start);
  ite = m_map_hist_cpnl.find(ymd_end);

  if (
      its == m_map_hist_cpnl.end() ||
      ite == m_map_hist_cpnl.end()
     )
    return NAN;

  return (ite->second.m_cpnl - its->second.m_cpnl);
}

double CPnLHist::GetPnL(const YYYYMMDD & ymd, const int ndays, const bool dense)
{
  if (ndays <= 0) return NAN;
  map<YYYYMMDD,cpnl_idx_tuple>::iterator it = m_map_hist_cpnl.find(ymd);

  if (it == m_map_hist_cpnl.end()) return NAN;
  if (it->second.m_idx < ndays)    return NAN;

  double cpnl_end = it->second.m_cpnl;

  if (!dense)
  {
    for (unsigned int i = 0; i < ndays; ++i) it--;
  }
  else
  {
    double dLastCPnL = cpnl_end;
    long lNumOfDenseDays = 0;

    while (it != m_map_hist_cpnl.begin())
    {
      it--;
      if (it->second.m_cpnl != dLastCPnL) lNumOfDenseDays++;
      if (lNumOfDenseDays >= ndays) break;
      dLastCPnL = it->second.m_cpnl;
    }
  }

  return (cpnl_end - it->second.m_cpnl);
}

double CPnLHist::GetPnL(const int ndays, const bool dense)
{
  if (ndays <= 0) return NAN;
  if (m_map_hist_cpnl.size() <= ndays) return NAN;

  map<YYYYMMDD,cpnl_idx_tuple>::iterator it = m_map_hist_cpnl.end();
  it--;

  double cpnl_end = it->second.m_cpnl;
  if (!dense)
  {
    for (unsigned int i = 0; i < ndays; ++i) it--;
  }
  else
  {
    double dLastCPnL = cpnl_end;
    long lNumOfDenseDays = 0;

    while (it != m_map_hist_cpnl.begin())
    {
      it--;
      if (it->second.m_cpnl != dLastCPnL) lNumOfDenseDays++;
      if (lNumOfDenseDays >= ndays) break;
      dLastCPnL = it->second.m_cpnl;
    }
  }

  return (cpnl_end - it->second.m_cpnl);
}

void CPnLHist::GetInternalData(map<string,double> & outMap)
{
  outMap.clear();

  map<YYYYMMDD,cpnl_idx_tuple>::iterator it;
  for (it = m_map_hist_cpnl.begin(); it != m_map_hist_cpnl.end(); ++it)
  {
    outMap[it->first.ToStr_()] = it->second.m_cpnl;
  }
}

bool CPnLHist::IsDateAvailable(const YYYYMMDD & ymd)
{
  map<YYYYMMDD,cpnl_idx_tuple>::iterator it = m_map_hist_cpnl.find(ymd);
  if (it == m_map_hist_cpnl.end()) return false;
  else return true;
}


double CPnLHist::GetEstWinningProb(const int iPeriod)
{
  if (iPeriod == 0) return NAN;
  vector<double> vWin;
  vector<double> vLoss;
  if (!ProduceWinLossSeries(iPeriod, vWin, vLoss)) return NAN;

  return (double)vWin.size() / (double)(vWin.size() + vLoss.size());
}


double CPnLHist::GetEstNetOdds(const int iPeriod)
{
  if (iPeriod == 0) return NAN;
  vector<double> vWin;
  vector<double> vLoss;
  if (!ProduceWinLossSeries(iPeriod, vWin, vLoss)) return NAN;

  double dSumWin  = 0;
  double dSumLoss = 0;
  double dAvgWin  = 0;
  double dAvgLoss = 0;
  for (unsigned int i = 0; i < vWin.size(); ++i) dSumWin += vWin[i];
  for (unsigned int i = 0; i < vLoss.size(); ++i) dSumLoss += vLoss[i];

  dAvgWin = dSumWin / vWin.size(); 
  dAvgLoss = dSumLoss / vLoss.size(); 

  return abs(dAvgWin) / abs(dAvgLoss);
}


bool CPnLHist::ProduceWinLossSeries(const int iPeriod, vector<double> & vWin, vector<double> & vLoss)
{
  if (m_map_hist_cpnl.size() <= iPeriod) return false;

  vWin.clear();
  vLoss.clear();

  map<YYYYMMDD,cpnl_idx_tuple>::iterator it = m_map_hist_cpnl.end();
  it--;

  double dLastCPnL = it->second.m_cpnl;
  for (unsigned int i = 0; i < iPeriod; ++i)
  {

    it--;
    double dPnL = dLastCPnL - it->second.m_cpnl;

    if      (dPnL >  EPSILON) vWin.push_back(dPnL);
    else if (dPnL < -EPSILON) vLoss.push_back(dPnL);

    dLastCPnL = it->second.m_cpnl;

  }

  if (vWin.empty() || vLoss.empty()) return false;

  return true;
}

