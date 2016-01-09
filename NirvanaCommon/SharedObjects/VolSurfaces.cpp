#include <VolSurfaces.h>

boost::weak_ptr<VolSurfaces> VolSurfaces::m_pInstance;

boost::shared_ptr<VolSurfaces> VolSurfaces::Instance() {
  boost::shared_ptr<VolSurfaces> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new VolSurfaces());
    m_pInstance = instance;
  }
  return instance;
}

VolSurfaces::VolSurfaces() :
  m_NirVHSI(NAN)
{
  m_MarketData = MarketData::Instance();

  m_3DaySma_NirVHSI.Reset(3);
  m_3DaySma_OfficialVHSI.Reset(3);
  m_1DayHistReturn_NirVHSI.Reset(24*60*60);
  m_1DayHistReturn_OfficialVHSI.Reset(24*60*60);
}

VolSurfaces::~VolSurfaces()
{
  for (map<string,VolSurf*>::iterator it = m_VolSurf.begin(); it != m_VolSurf.end(); ++it)
  {
    delete it->second;
  }
  for (map<string,shared_mutex*>::iterator it = m_VolSurfMutex.begin(); it != m_VolSurfMutex.end(); ++it)
  {
    delete it->second;
  }
}

void VolSurfaces::UpdateUndlIVByStrike(const string & symbol, const unsigned long ymdhms, const double strike, const double undly, const double iv)
{
  boost::unique_lock<boost::shared_mutex> lock(*(GetVolSurfMutex(symbol)));

  map<string,VolSurf*>::iterator it = m_VolSurf.find(symbol);
  if (it == m_VolSurf.end())
  {
    VolSurf* vs = new VolSurf(); 
    vs->UpdateUndlIVByStrike(ymdhms,strike,undly,iv);
    m_VolSurf[symbol] = vs;
  }
  else
  {
    it->second->UpdateUndlIVByStrike(ymdhms,strike,undly,iv);
  }

  return;
}
void VolSurfaces::UpdateUndly(const string & symbol, const unsigned long ymdhms, const double undly)
{
  boost::unique_lock<boost::shared_mutex> lock(*(GetVolSurfMutex(symbol)));

  map<string,VolSurf*>::iterator it = m_VolSurf.find(symbol);
  if (it == m_VolSurf.end())
  {
    VolSurf* vs = new VolSurf(); 
    vs->UpdateUndly(ymdhms,undly);
    m_VolSurf[symbol] = vs;
  }
  else
  {
    it->second->UpdateUndly(ymdhms,undly);
  }

  return;
}

double VolSurfaces::GetIVByStrike(const string & symbol, const unsigned long ymdhms, const double strike)
{
  boost::shared_lock<boost::shared_mutex> lock(*(GetVolSurfMutex(symbol)));

  map<string,VolSurf*>::iterator it = m_VolSurf.find(symbol);
  if (it == m_VolSurf.end())
  {
    return NAN;
  }
  else
  {
    return it->second->GetIVByStrike(ymdhms, strike);
  }
}

double VolSurfaces::GetIVByMoneyness(const string & symbol, const unsigned long ymdhms, const double moneyness)
{
  boost::shared_lock<boost::shared_mutex> lock(*(GetVolSurfMutex(symbol)));

  map<string,VolSurf*>::iterator it = m_VolSurf.find(symbol);
  if (it == m_VolSurf.end())
  {
    return NAN;
  }
  else
  {
    return it->second->GetIVByMoneyness(ymdhms, moneyness);
  }
}

void VolSurfaces::ResetSurfaces(const string & symbol)
{
  boost::unique_lock<boost::shared_mutex> lock(*(GetVolSurfMutex(symbol)));

  map<string,VolSurf*>::iterator it = m_VolSurf.find(symbol);
  if (it == m_VolSurf.end())
  {
    return;
  }
  else
  {
    return it->second->Reset();
  }
}

void VolSurfaces::FitVolSmile(const string & symbol, unsigned long ymd)
{
  boost::shared_lock<boost::shared_mutex> lock(*(GetVolSurfMutex(symbol)));

  map<string,VolSurf*>::iterator it = m_VolSurf.find(symbol);
  if (it == m_VolSurf.end())
  {
    return;
  }
  else
  {
    return it->second->FitVolSmile(ymd);
  }
  return;
}

// short VolSurfaces::GetOrderOfFittedPolynomial(const string & symbol, const unsigned long ymd)
// {
//   boost::shared_lock<boost::shared_mutex> lock(*(GetVolSurfMutex(symbol)));
//
//   map<string,VolSurf*>::iterator it = m_VolSurf.find(symbol);
//   if (it == m_VolSurf.end())
//   {
//     return NAN;
//   }
//   else
//   {
//     return it->second->GetOrderOfFittedPolynomial(ymd);
//   }
// }

shared_mutex * VolSurfaces::GetVolSurfMutex(const string & symbol)
{
  map<string,shared_mutex *>::iterator it = m_VolSurfMutex.find(symbol);
  if (it != m_VolSurfMutex.end())
  {
    return it->second;
  }
  else
  {
    shared_mutex * p = new shared_mutex();
    m_VolSurfMutex[symbol] = p;
    return p;
  }
}

void VolSurfaces::UpdateNirVHSI(const YYYYMMDDHHMMSS & ymdhms, const double vhsi)
{
  m_NirVHSI = vhsi;
  m_3DaySma_NirVHSI.Add(ymdhms.GetYYYYMMDD(),vhsi);
  m_1DayHistReturn_NirVHSI.Add(ymdhms,vhsi);
}

double VolSurfaces::GetNirVHSI() const
{
  return m_NirVHSI;
}


double VolSurfaces::GetOfficialVHSIPrevEOD() const
{
  YYYYMMDDHHMMSS ymdhms_MDITime = m_MarketData->GetSystemTimeHKT();

  vector<YYYYMMDD> vYMD;
  vector<HHMMSS>   vHMS;
  vector<double>   vOpen;
  vector<double>   vHigh;
  vector<double>   vLow;
  vector<double>   vClose;
  vector<long>     vVol;

  YMD ymdStart(ymdhms_MDITime.GetYYYYMMDD());
  ymdStart.SubtractDay(10);

  YMD ymdEnd(ymdhms_MDITime.GetYYYYMMDD());
  ymdEnd.SubtractDay(1);

  m_MarketData->GetSuppD1BarOHLCVInDateRange(
      UNDERLYING_VHSI,
      YYYYMMDD(ymdStart),
      YYYYMMDD(ymdEnd),
      vYMD,
      vOpen,
      vHigh,
      vLow,
      vClose,
      vVol);

  if (vClose.empty()) return NAN;
  return vClose.back()/100;
}

double VolSurfaces::GetOfficialVHSI() const
{
  return GetOfficialVHSI(m_MarketData->GetSystemTimeHKT());
}

double VolSurfaces::GetOfficialVHSI(const YYYYMMDDHHMMSS & ymdhms_MDITime) const
{
  vector<YYYYMMDD> vYMD;
  vector<HHMMSS>   vHMS;
  vector<double>   vOpen;
  vector<double>   vHigh;
  vector<double>   vLow;
  vector<double>   vClose;
  vector<long>     vVol;

  YMD ymdStart(ymdhms_MDITime.GetYYYYMMDD());
  ymdStart.SubtractDay(10);

  m_MarketData->GetSuppM1BarOHLCVInDateTimeRange(
      UNDERLYING_VHSI,
      YYYYMMDD(ymdStart),
      HHMMSS(0),
      ymdhms_MDITime.GetYYYYMMDD(),
      ymdhms_MDITime.GetHHMMSS(),
      vYMD,
      vHMS,
      vOpen,
      vHigh,
      vLow,
      vClose,
      vVol);

  if (vClose.empty()) return NAN;
  return vClose.back()/100;
}

double VolSurfaces::GetNirVHSI3DaySma()
{
  return m_3DaySma_NirVHSI.Value();
}
double VolSurfaces::GetOfficialVHSI3DaySma()
{
  return m_3DaySma_OfficialVHSI.Value();
}

double VolSurfaces::GetNirVHSI1DayHistReturn()
{
  return m_1DayHistReturn_NirVHSI.Return();
}
double VolSurfaces::GetOfficialVHSI1DayHistReturn()
{
  return m_1DayHistReturn_OfficialVHSI.Return();
}

long VolSurfaces::HashKey(const int yyyymm, const double moneyness, const HSIVolSurfModelParam param)
{
  return yyyymm * 100000 + (long)STool::Round(moneyness * 10000,0) + param;
}

void VolSurfaces::LoadHSIVolSurfModelParam(const string & sPath)
{

  namespace fs = boost::filesystem;
  fs::path someDir(sPath);
  fs::directory_iterator end_iter;

  if ( fs::exists(someDir) && fs::is_directory(someDir))
  {
    for( fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
    {
      if (fs::is_regular_file(dir_iter->status()) )
      {

        deque<string> dqRawFile;
        STool::ReadFile(dir_iter->path().string(),dqRawFile);
        if (dqRawFile.empty()) return;
        for (unsigned int i = 0; i < dqRawFile.size(); ++i)
        {
          vector<string> vs;
          boost::split(vs, dqRawFile[i], boost::is_any_of(","));
          if (vs.size() == 3)
          {

            HSIVolSurfModelParam param;
            if      (vs[1] == "theta") param = VolSurfaces::theta;
            else if (vs[1] == "phi")   param = VolSurfaces::phi;
            else if (vs[1] == "psi")   param = VolSurfaces::psi;
            else if (vs[1] == "h")     param = VolSurfaces::h;
            else if (vs[1] == "a")     param = VolSurfaces::a;
            else if (vs[1] == "b1")    param = VolSurfaces::b1;
            else if (vs[1] == "b2")    param = VolSurfaces::b2;
            else if (vs[1] == "c")     param = VolSurfaces::c;

            long lHashKey = HashKey(boost::lexical_cast<long>(dir_iter->path().filename().string().substr(1,6)), boost::lexical_cast<double>(vs[0]), param);
            m_HSIVolSurfModelParam[lHashKey] = boost::lexical_cast<double>(vs[2]);

          }
        }

      }
    }

  }
}

double VolSurfaces::Predict_HSI_IV()
{
  return NAN;
}

void VolSurfaces::WaitForData()
{
  boost::mutex::scoped_lock lock(m_ObserverMutex);
  m_cvDataAvb.wait(lock); //blocks and returns mutex
  return;
}

void VolSurfaces::NotifyConsumers()
{
  m_cvDataAvb.notify_all();
}

