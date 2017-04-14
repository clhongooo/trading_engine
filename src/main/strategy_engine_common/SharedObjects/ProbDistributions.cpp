#include <ProbDistributions.h>

boost::weak_ptr<ProbDistributions> ProbDistributions::m_pInstance;

boost::shared_ptr<ProbDistributions> ProbDistributions::Instance() {
  boost::shared_ptr<ProbDistributions> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new ProbDistributions());
    m_pInstance = instance;
  }
  return instance;
}

ProbDistributions::ProbDistributions()
{
}

ProbDistributions::~ProbDistributions()
{
  for (map<string,ProbMassFunc*>::iterator it = m_pmf.begin(); it != m_pmf.end(); ++it)
  {
    delete it->second;
  }
  for (map<string,shared_mutex*>::iterator it = m_ProbDistrnMutex.begin(); it != m_ProbDistrnMutex.end(); ++it)
  {
    delete it->second;
  }
}

shared_mutex * ProbDistributions::GetProbDistrnMutex(const string & symbol)
{
  map<string,shared_mutex *>::iterator it = m_ProbDistrnMutex.find(symbol);
  if (it != m_ProbDistrnMutex.end())
  {
    return it->second;
  }
  else
  {
    shared_mutex * p = new shared_mutex();
    m_ProbDistrnMutex[symbol] = p;
    return p;
  }
}

void ProbDistributions::SetPMF(const string & symbol, const long yyyymmddhhmmss, const ProbMassFunc & pmfa)
{
  boost::unique_lock<boost::shared_mutex> lock(*(GetProbDistrnMutex(symbol)));

  map<string,ProbMassFunc*>::iterator it = m_pmf.find(symbol);
  if (it == m_pmf.end())
  {
    ProbMassFunc * p = new ProbMassFunc();
    m_pmf[symbol] = p;
    it = m_pmf.find(symbol);
  }

  it->second->Reset(yyyymmddhhmmss);
  it->second->ImportAndNmlz(yyyymmddhhmmss,pmfa,yyyymmddhhmmss,1);

  return;
}

bool ProbDistributions::GetPMF(const string & symbol, const long yyyymmddhhmmss, ProbMassFunc & pmfa)
{
  boost::shared_lock<boost::shared_mutex> lock(*(GetProbDistrnMutex(symbol)));

  map<string,ProbMassFunc*>::iterator it = m_pmf.find(symbol);
  if (it == m_pmf.end()) return false;

  pmfa.Reset(yyyymmddhhmmss);
  pmfa.ImportAndNmlz(yyyymmddhhmmss,*(it->second),yyyymmddhhmmss,1);

  return true;
}

