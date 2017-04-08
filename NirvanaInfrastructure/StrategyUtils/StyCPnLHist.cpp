#include <StyCPnLHist.h>

boost::weak_ptr<StyCPnLHist> StyCPnLHist::m_pInstance;

boost::shared_ptr<StyCPnLHist> StyCPnLHist::Instance() {
  boost::shared_ptr<StyCPnLHist> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new StyCPnLHist());
    m_pInstance = instance;
  }
  return instance;
}

StyCPnLHist::StyCPnLHist()
{
  m_Logger = Logger::Instance();
  m_SysCfg = SystemConfig::Instance();
}

StyCPnLHist::~StyCPnLHist() {
}

void StyCPnLHist::LoadPersistedCPnLHistFromFile()
{
  boost::unique_lock<boost::shared_mutex> lock(m_cpnl_hist_mutex);

  for (int sty = STY_NIR; sty != STY_LAST; sty++)
  {
    StrategyID sid = static_cast<StrategyID>(sty);
    if (!m_SysCfg->IsStrategyOn(sid)) continue;

    if (m_map_cpnl_hist.find(sty) == m_map_cpnl_hist.end()) m_map_cpnl_hist[sty] = CPnLHist();

    if (m_SysCfg->IsCPnLHistPersistenceEnabled(sid))
    {
      if (!m_map_cpnl_hist[sty].LoadFromFile(m_SysCfg->Get_CPnLHistPersistenceFile(sid)))
      {
        m_Logger->Write(Logger::ERROR,"StyCPnLHist: CPnL history persistence file is broken. Expecting format: <YYYY-MM-DD>,<CPnL>. Now exiting.");
        sleep(1);
        exit(1);
      }
      OutputCPnLHistNoLock(sty,'l');
    }
  }
}

void StyCPnLHist::OutputCPnLHistNoLock(const int id, const char cOperation)
{
  StrategyID sid = static_cast<StrategyID>(id);
  if (!m_SysCfg->IsCPnLHistPersistenceEnabled(sid)) return;

  map<int,CPnLHist>::iterator it = m_map_cpnl_hist.find(id);
  if (it == m_map_cpnl_hist.end()) return;

  if (cOperation == 'f')
  {
    it->second.WriteToFile(m_SysCfg->Get_CPnLHistPersistenceFile(sid));
  }
  else if (cOperation == 'l')
  {
    map<string,double> map_YYYYMMDD_CPnL;
    it->second.GetInternalData(map_YYYYMMDD_CPnL);

    map<string,double>::iterator it;
    for (it = map_YYYYMMDD_CPnL.begin(); it != map_YYYYMMDD_CPnL.end(); ++it)
    {
      m_Logger->Write(Logger::INFO,"StyCPnLHist: CPnL history internal data: %s %f",
          it->first.c_str(),
          it->second);
    }
  }
}

void StyCPnLHist::CPnLHistWriteToFile(const int id)
{
  boost::shared_lock<boost::shared_mutex> lock(m_cpnl_hist_mutex);
  OutputCPnLHistNoLock(id,'f');
}

void StyCPnLHist::CPnLHistAddAndTruncate(const int id, const YYYYMMDD & ymd, const double cpnl)
{
  if (std::isnan(cpnl)) return;
  boost::unique_lock<boost::shared_mutex> lock(m_cpnl_hist_mutex);
  map<int,CPnLHist>::iterator it = m_map_cpnl_hist.find(id);
  if (it == m_map_cpnl_hist.end()) return;
  it->second.AddAndTruncate(ymd,cpnl);
}

double StyCPnLHist::CPnLHistGetPnL(const int id, const YYYYMMDD & ymd1, const YYYYMMDD & ymd2)
{
  boost::unique_lock<boost::shared_mutex> lock(m_cpnl_hist_mutex);
  map<int,CPnLHist>::iterator it = m_map_cpnl_hist.find(id);
  if (it == m_map_cpnl_hist.end()) return NAN;
  it->second.GetPnL(ymd1,ymd2);
}
double StyCPnLHist::CPnLHistGetPnL(const int id, const YYYYMMDD & ymd, const int iPeriod, const bool bDense)
{
  boost::unique_lock<boost::shared_mutex> lock(m_cpnl_hist_mutex);
  map<int,CPnLHist>::iterator it = m_map_cpnl_hist.find(id);
  if (it == m_map_cpnl_hist.end()) return NAN;
  it->second.GetPnL(ymd,iPeriod,bDense);
}
double StyCPnLHist::CPnLHistGetPnL(const int id, const int iPeriod, const bool bDense)
{
  boost::unique_lock<boost::shared_mutex> lock(m_cpnl_hist_mutex);
  map<int,CPnLHist>::iterator it = m_map_cpnl_hist.find(id);
  if (it == m_map_cpnl_hist.end()) return NAN;
  it->second.GetPnL(iPeriod,bDense);
}

bool StyCPnLHist::CPnLHistIsDateAvb(const int id, const YYYYMMDD & ymd)
{
  boost::shared_lock<boost::shared_mutex> lock(m_cpnl_hist_mutex);
  map<int,CPnLHist>::iterator it = m_map_cpnl_hist.find(id);
  if (it == m_map_cpnl_hist.end()) return false;

  return it->second.IsDateAvailable(ymd);
}

double StyCPnLHist::CPnLHistGetEstWinningProb(const int id, const int iPeriod)
{
  boost::shared_lock<boost::shared_mutex> lock(m_cpnl_hist_mutex);
  map<int,CPnLHist>::iterator it = m_map_cpnl_hist.find(id);
  if (it == m_map_cpnl_hist.end()) return NAN;

  return it->second.GetEstWinningProb(iPeriod);
}

double StyCPnLHist::CPnLHistGetEstNetOdds(const int id, const int iPeriod)
{
  boost::shared_lock<boost::shared_mutex> lock(m_cpnl_hist_mutex);
  map<int,CPnLHist>::iterator it = m_map_cpnl_hist.find(id);
  if (it == m_map_cpnl_hist.end()) return NAN;

  return it->second.GetEstNetOdds(iPeriod);
}

