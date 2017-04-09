#include "HKSE.h"

boost::weak_ptr<HKSE> HKSE::m_pInstance;

boost::shared_ptr<HKSE> HKSE::Instance() {
  boost::shared_ptr<HKSE> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new HKSE());
    m_pInstance = instance;
  }
  return instance;
}

HKSE::HKSE()
{
}


HKSE::~HKSE()
{
}


void HKSE::LoadHSIConstituents(const string & sPath)
{

  namespace fs = boost::filesystem;
  fs::path someDir(sPath);
  fs::directory_iterator end_iter;

  if (fs::exists(someDir) && fs::is_directory(someDir))
  {
    for(fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
    {
      if (fs::is_regular_file(dir_iter->status()) )
      {

        //--------------------------------------------------
        // Read individual file content
        //--------------------------------------------------
        int iYYYYMM = boost::lexical_cast<int>(dir_iter->path().filename().string());
        m_HSI_constituents[iYYYYMM] = map<string,double>();

        map<string,double> & mStkCodeWeight = m_HSI_constituents[iYYYYMM];

        deque<string> dqRawFile;
        STool::ReadFile(dir_iter->path().string(),dqRawFile);
        if (dqRawFile.empty()) continue;
        for (unsigned int i = 0; i < dqRawFile.size(); ++i)
        {
          vector<string> vs;
          boost::split(vs, dqRawFile[i], boost::is_any_of(","));
          mStkCodeWeight[vs[0]] = boost::lexical_cast<double>(vs[1]);
        }

      }
    }
  }

}

map<string,double> * HKSE::GetHSIConstituents(const YYYYMMDD & ymd)
{
  map<int,map<string,double> >::iterator it;
  it = m_HSI_constituents.find(ymd.ToInt());

  //--------------------------------------------------
  // Exact match
  //--------------------------------------------------
  if (it != m_HSI_constituents.end()) return &(it->second);

  //--------------------------------------------------
  // Try matching YYYYMM
  //--------------------------------------------------
  int iYYYYMM = ymd.Year()*100+ymd.Month();
  it = m_HSI_constituents.find(iYYYYMM);
  if (it != m_HSI_constituents.end()) return &(it->second);

  //--------------------------------------------------
  // No match
  //--------------------------------------------------
  return NULL;
}

bool HKSE::IsAProductInThisMarket(const string & symbol)
{
  if (symbol == UNDERLYING_HSI) return true;
  if (symbol == UNDERLYING_HHI) return true;

  int iSym = 0;
  try
  {
    iSym = boost::lexical_cast<int>(symbol);
  }
  catch (boost::bad_lexical_cast &)
  {
    return false;
  }

  if (iSym > 0 && iSym < 99999) return true;
  return false;
}

