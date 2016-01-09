#include "HKMA.h"

boost::weak_ptr<HKMA> HKMA::m_pInstance;

boost::shared_ptr<HKMA> HKMA::Instance() {
  boost::shared_ptr<HKMA> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new HKMA());
    m_pInstance = instance;
  }
  return instance;
}

HKMA::HKMA()
{
}


HKMA::~HKMA()
{
}

void HKMA::LoadExchgFundBill(const string & sPath)
{
  deque<string> dqRawFile;
  STool::ReadFile(sPath,dqRawFile);
  if (dqRawFile.empty()) return;
  for (unsigned int i = 0; i < dqRawFile.size(); ++i)
  {
    vector<string> vs;
    boost::split(vs, dqRawFile[i], boost::is_any_of(","));
    if (vs.size() == 2)
    {
      m_ExchgFundBill[boost::lexical_cast<int>(vs[0])] = boost::lexical_cast<double>(vs[1]);
    }
  }

  return;
}

bool HKMA::GetExchgFundBillAnnlzd(const YYYYMMDD & ymd, double & dRate)
{
  map<int,double>::iterator it = m_ExchgFundBill.find(ymd.ToInt());
  if (it == m_ExchgFundBill.end())
  {
    return false;
  }
  else
  {
    dRate = (double) (it->second / 100);
    return true;
  }
}

