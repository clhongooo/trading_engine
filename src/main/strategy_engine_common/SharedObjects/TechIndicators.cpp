#include <TechIndicators.h>

boost::weak_ptr<TechIndicators> TechIndicators::m_pInstance;

boost::shared_ptr<TechIndicators> TechIndicators::Instance() {
  boost::shared_ptr<TechIndicators> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new TechIndicators());
    m_pInstance = instance;
  }
  return instance;
}

TechIndicators::TechIndicators()
{
  m_Logger = Logger::Instance();
  m_SysCfg = SystemConfig::Instance();
  m_HKFE   = HKFE::Instance();
}

TechIndicators::~TechIndicators()
{
  for (map<string,GKYZ*>::iterator it = m_GKYZ.begin(); it != m_GKYZ.end(); ++it)
  {
    delete it->second;
    it->second = NULL;
  }
  for (map<string,FSMC*>::iterator it = m_FSMC.begin(); it != m_FSMC.end(); ++it)
  {
    delete it->second;
    it->second = NULL;
  }
  for (map<string,Sma<double>*>::iterator it = m_MinuteBarSma.begin(); it != m_MinuteBarSma.end(); ++it)
  {
    delete it->second;
    it->second = NULL;
  }
  for (map<string,TickReturn*>::iterator it = m_TickReturn.begin(); it != m_TickReturn.end(); ++it)
  {
    delete it->second;
    it->second = NULL;
  }
  for (map<int,Sma<double>*>::iterator it = m_lnHSIHHIRatioSma.begin(); it != m_lnHSIHHIRatioSma.end(); ++it)
  {
    delete it->second;
    it->second = NULL;
  }
  for (map<int,Ema<double>*>::iterator it = m_lnHSIHHIRatioEma.begin(); it != m_lnHSIHHIRatioEma.end(); ++it)
  {
    delete it->second;
    it->second = NULL;
  }
  for (map<string,MeanAccumulator*>::iterator it = m_1MinAvgPrice.begin(); it != m_1MinAvgPrice.end(); ++it)
  {
    delete it->second;
    it->second = NULL;
  }
}

void TechIndicators::AddBarGKYZ(const string & symbol,double O,double H,double L,double C)
{
  if (std::isnan(O) || std::isnan(H) || std::isnan(L) || std::isnan(C)) return;
  if (O <= 0 || H <= 0 || L <= 0 || C <= 0) return;

  boost::unique_lock<boost::shared_mutex> lock(m_GKYZMutex);

  map<string,GKYZ*>::iterator it = m_GKYZ.find(symbol);
  if (it == m_GKYZ.end())
  {
    m_GKYZ[symbol] = new GKYZ(m_SysCfg->Get_GKYZ_Window_Size(), true);
    it = m_GKYZ.find(symbol);
  }

  it->second->Add(O,H,L,C);

  //--------------------------------------------------
  // update history
  //--------------------------------------------------
  map<string,deque<double> >::iterator it2 = m_HistGKYZ.find(symbol);
  if (it2 == m_HistGKYZ.end())
  {
    m_HistGKYZ[symbol] = deque<double>();
    it2 = m_HistGKYZ.find(symbol);
  }
  it2->second.push_back(it->second->Value());

  //--------------------------------------------------
  // update the diff time series
  //--------------------------------------------------
  map<string,deque<double> >::iterator it3 = m_HistGKYZDiff.find(symbol);
  if (it3 == m_HistGKYZDiff.end())
  {
    m_HistGKYZDiff[symbol] = deque<double>();
    it3 = m_HistGKYZDiff.find(symbol);
  }

  if (it2->second.size() >= 2)
  {
    it3->second.push_back(it2->second.back() - *(it2->second.end()-2));
  }

  return;
}

GKYZ TechIndicators::GetGKYZClone(const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_GKYZMutex);

  map<string,GKYZ*>::iterator it = m_GKYZ.find(symbol);
  if (it == m_GKYZ.end()) return GKYZ();

  return *(it->second);
}

double TechIndicators::GetGKYZValue(const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_GKYZMutex);

  map<string,GKYZ*>::iterator it = m_GKYZ.find(symbol);
  if (it == m_GKYZ.end()) return NAN;

  return it->second->Value();
}


bool TechIndicators::GetHistGKYZImpl(const string & symbol, const int noofdays, deque<double> & dq, const map<string,deque<double> > & mapSrc)
{
  dq.clear();

  boost::shared_lock<boost::shared_mutex> lock(m_GKYZMutex);

  map<string,deque<double> >::const_iterator it1 = mapSrc.find(symbol);
  if (it1 == mapSrc.end()) return false;

  const deque<double> & dqSrc = it1->second;

  if (dqSrc.size() < noofdays) return false;

  int iCnt = 0;

  deque<double>::const_iterator it = dqSrc.end();

  for (unsigned int i = 0; i < noofdays; ++i) it--;

  for (unsigned int i = 0; i < noofdays; ++i)
  {
    dq.push_back(*it);
    it++;
  }

  return true;
}

bool TechIndicators::GetHistGKYZ(const string & symbol, const int noofdays, deque<double> & dq)
{
  return GetHistGKYZImpl(symbol, noofdays, dq, m_HistGKYZ);
}

bool TechIndicators::GetHistGKYZDiff(const string & symbol, const int noofdays, deque<double> & dq)
{
  return GetHistGKYZImpl(symbol, noofdays, dq, m_HistGKYZDiff);
}


double TechIndicators::GetEstGKYZValue(const string & symbol)
{
  double o = GetDayOpen(symbol);
  double h = GetDayHigh(symbol);
  double l = GetDayLow(symbol);
  double c = GetDayClose(symbol);

  if (
      std::isnan(o) ||
      std::isnan(h) ||
      std::isnan(l) ||
      std::isnan(c)
     )
  {
    return GetGKYZValue(symbol);
  }

  GKYZ gkyz = GetGKYZClone(symbol);
  gkyz.Add(o,h,l,c);
  return gkyz.Value();
}

double TechIndicators::GetHistGKYZPctgChange(const string & symbol)
{
  map<string,deque<double> >::iterator it = m_HistGKYZDiff.find(symbol);
  if (it == m_HistGKYZDiff.end())
  {
    m_HistGKYZDiff[symbol] = deque<double>();
    it = m_HistGKYZDiff.find(symbol);
  }
  if (it->second.empty()) return NAN;


  //--------------------------------------------------
  map<string,deque<double> >::iterator it2 = m_HistGKYZ.find(symbol);
  if (it2 == m_HistGKYZ.end()) return NAN;
  if (it2->second.size() < 2) return NAN;


  return (it->second.back() / *(it2->second.end()-2));
}


void TechIndicators::AddFSMC(const string & symbol,double H,double L,double C,double VHSI)
{
  if (std::isnan(H) || std::isnan(L) || std::isnan(C) || std::isnan(VHSI)) return;
  if (H <= 0 || L <= 0 || C <= 0 || VHSI <= 0) return;

  boost::unique_lock<boost::shared_mutex> lock(m_FSMCMutex);

  map<string,FSMC*>::iterator it = m_FSMC.find(symbol);
  if (it == m_FSMC.end())
  {
    m_FSMC[symbol] = new FSMC2();
    it = m_FSMC.find(symbol);
  }

  it->second->Add(H,L,C,VHSI);
  return;
}

int TechIndicators::GetFSMCState(const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_FSMCMutex);

  map<string,FSMC*>::iterator it = m_FSMC.find(symbol);
  if (it == m_FSMC.end()) return -1;
  return it->second->State();
}

void TechIndicators::AddMinuteBarSma(const string & symbol,const int period,const double P)
{
  if (std::isnan(P)) return;
  if (P <= 0) return;

  boost::unique_lock<boost::shared_mutex> lock(m_MinuteBarSmaMutex);

  string sAugmentedSymbol = symbol + "_" + boost::lexical_cast<string>(period);
  map<string,Sma<double>*>::iterator it = m_MinuteBarSma.find(sAugmentedSymbol);
  if (it == m_MinuteBarSma.end())
  {
    m_MinuteBarSma[sAugmentedSymbol] = new Sma<double>(period,true);
    it = m_MinuteBarSma.find(sAugmentedSymbol);
  }

  it->second->Add(P);
  return;
}
double TechIndicators::GetMinuteBarSma(const string & symbol,const int period)
{
  boost::shared_lock<boost::shared_mutex> lock(m_MinuteBarSmaMutex);

  string sAugmentedSymbol = symbol + "_" + boost::lexical_cast<string>(period);
  map<string,Sma<double>*>::iterator it = m_MinuteBarSma.find(sAugmentedSymbol);
  if (it == m_MinuteBarSma.end()) return NAN;
  if (!it->second->Ready()) return NAN;
  return it->second->Value();
}
void TechIndicators::AddlnHSIHHIRatioSma(const int period,const double P)
{
  if (std::isnan(P)) return;
  if (P <= 0) return;

  boost::unique_lock<boost::shared_mutex> lock(m_lnHSIHHIRatioSmaMutex);

  map<int,Sma<double>*>::iterator it = m_lnHSIHHIRatioSma.find(period);
  if (it == m_lnHSIHHIRatioSma.end())
  {
    m_lnHSIHHIRatioSma[period] = new Sma<double>(period,true);
    it = m_lnHSIHHIRatioSma.find(period);
  }

  it->second->Add(P);
  return;
}
double TechIndicators::GetlnHSIHHIRatioSma(const int period)
{
  boost::shared_lock<boost::shared_mutex> lock(m_lnHSIHHIRatioSmaMutex);

  map<int,Sma<double>*>::iterator it = m_lnHSIHHIRatioSma.find(period);
  if (it == m_lnHSIHHIRatioSma.end()) return NAN;
  if (!it->second->Ready()) return NAN;
  return it->second->Value();
}
void TechIndicators::AddlnHSIHHIRatioEma(const int period,const double P)
{
  if (std::isnan(P)) return;
  if (P <= 0) return;
  if (period <= 0) return;

  boost::unique_lock<boost::shared_mutex> lock(m_lnHSIHHIRatioEmaMutex);

  map<int,Ema<double>*>::iterator it = m_lnHSIHHIRatioEma.find(period);
  if (it == m_lnHSIHHIRatioEma.end())
  {
    m_lnHSIHHIRatioEma[period] = new Ema<double>((double)1/(double)period,true);
    it = m_lnHSIHHIRatioEma.find(period);
  }

  it->second->Add(P);
  return;
}
double TechIndicators::GetlnHSIHHIRatioEma(const int period)
{
  boost::shared_lock<boost::shared_mutex> lock(m_lnHSIHHIRatioEmaMutex);

  map<int,Ema<double>*>::iterator it = m_lnHSIHHIRatioEma.find(period);
  if (it == m_lnHSIHHIRatioEma.end()) return NAN;
  if (!it->second->Ready()) return NAN;
  return it->second->Value();
}

void TechIndicators::AddTickReturn(const string & symbol, const int periodInSec, const YYYYMMDDHHMMSS & curDateTime, const double price)
{
  if (std::isnan(price)) return;
  if (price <= 0) return;
  if (periodInSec <= 0) return;

  boost::unique_lock<boost::shared_mutex> lock(m_TickReturnMutex);

  string sAugmentedSymbol = symbol + "_" + boost::lexical_cast<string>(periodInSec);
  map<string,TickReturn*>::iterator it = m_TickReturn.find(sAugmentedSymbol);
  if (it == m_TickReturn.end())
  {
    m_TickReturn[sAugmentedSymbol] = new TickReturn(periodInSec);
    it = m_TickReturn.find(sAugmentedSymbol);
  }

  it->second->Add(curDateTime,price);
  return;
}


double TechIndicators::GetLnTickReturn(const string & symbol, const int periodInSec)
{
  boost::shared_lock<boost::shared_mutex> lock(m_TickReturnMutex);

  string sAugmentedSymbol = symbol + "_" + boost::lexical_cast<string>(periodInSec);
  map<string,TickReturn*>::iterator it = m_TickReturn.find(sAugmentedSymbol);
  if (it == m_TickReturn.end()) return NAN;
  if (!it->second->Ready()) return NAN;
  return it->second->LnReturnR();
}

void TechIndicators::WaitForData()
{
  boost::mutex::scoped_lock lock(m_ObserverMutex);
  m_cvDataAvb.wait(lock); //blocks and returns mutex
  return;
}

void TechIndicators::NotifyConsumers()
{
  m_cvDataAvb.notify_all();
}


void TechIndicators::Add1MinAvgPrice(const string & symbol, const double price)
{
  if (std::isnan(price)) return;
  if (price <= 0) return;

  boost::unique_lock<boost::shared_mutex> lock(m_1MinAvgPriceMutex);

  map<string,MeanAccumulator*>::iterator it = m_1MinAvgPrice.find(symbol);
  if (it == m_1MinAvgPrice.end())
  {
    m_1MinAvgPrice[symbol] = new MeanAccumulator();
    it = m_1MinAvgPrice.find(symbol);
  }

  it->second->Add(price);

  return;
}

double TechIndicators::Get1MinAvgPrice(const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_1MinAvgPriceMutex);

  map<string,MeanAccumulator*>::iterator it = m_1MinAvgPrice.find(symbol);
  if (it == m_1MinAvgPrice.end()) return NAN;
  if (!it->second->Ready()) return NAN;
  return it->second->GetMean();
}

void TechIndicators::Reset1MinAvgPrice(const string & symbol)
{
  boost::unique_lock<boost::shared_mutex> lock(m_1MinAvgPriceMutex);

  map<string,MeanAccumulator*>::iterator it = m_1MinAvgPrice.find(symbol);
  if (it == m_1MinAvgPrice.end()) return;
  it->second->Reset();
  return;
}



void TechIndicators::AddDayOHLC(const string & symbol, const double price)
{
  if (std::isnan(price)) return;
  if (price <= 0) return;

  boost::unique_lock<boost::shared_mutex> lock(m_DayOHLCMutex);

  map<string,OHLC>::iterator it = m_DayOHLC.find(symbol);
  if (it == m_DayOHLC.end())
  {
    m_DayOHLC[symbol] = OHLC();
    it = m_DayOHLC.find(symbol);
  }
  it->second.Add(price);

  return;
}

void TechIndicators::AddDayOHLC(const string & symbol, const double o, const double h, const double l, const double c)
{
  if (std::isnan(o) || std::isnan(h) || std::isnan(l) || std::isnan(c)) return;
  if (o <= 0 || h <= 0 || l <= 0 || c <= 0) return;

  boost::unique_lock<boost::shared_mutex> lock(m_DayOHLCMutex);

  map<string,OHLC>::iterator it = m_DayOHLC.find(symbol);
  if (it == m_DayOHLC.end())
  {
    m_DayOHLC[symbol] = OHLC();
    it = m_DayOHLC.find(symbol);
  }
  it->second.Add(o,h,l,c);

  return;
}

double TechIndicators::GetDayOpen(const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_DayOHLCMutex);
  map<string,OHLC>::iterator it = m_DayOHLC.find(symbol);
  if (it == m_DayOHLC.end()) return NAN;
  return it->second.Open();
}

double TechIndicators::GetDayHigh(const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_DayOHLCMutex);
  map<string,OHLC>::iterator it = m_DayOHLC.find(symbol);
  if (it == m_DayOHLC.end()) return NAN;
  return it->second.High();
}

double TechIndicators::GetDayLow(const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_DayOHLCMutex);
  map<string,OHLC>::iterator it = m_DayOHLC.find(symbol);
  if (it == m_DayOHLC.end()) return NAN;
  return it->second.Low();
}

double TechIndicators::GetDayClose(const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_DayOHLCMutex);
  map<string,OHLC>::iterator it = m_DayOHLC.find(symbol);
  if (it == m_DayOHLC.end()) return NAN;
  return it->second.Close();
}

void TechIndicators::ResetDayOHLC(const string & symbol)
{
  boost::unique_lock<boost::shared_mutex> lock(m_DayOHLCMutex);

  map<string,OHLC>::iterator it = m_DayOHLC.find(symbol);
  if (it == m_DayOHLC.end()) return;
  it->second.Reset();
  return;
}

