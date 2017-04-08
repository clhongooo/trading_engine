#include <HKIntradaySeasonality.h>

boost::weak_ptr<HKIntradaySeasonality> HKIntradaySeasonality::m_pInstance;

boost::shared_ptr<HKIntradaySeasonality> HKIntradaySeasonality::Instance() {
  boost::shared_ptr<HKIntradaySeasonality> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new HKIntradaySeasonality());
    m_pInstance = instance;
  }
  return instance;
}

HKIntradaySeasonality::HKIntradaySeasonality() :
  m_OnOff(false),
  m_TrainingResultPath(""),
  m_WindowLength(44)
{
  m_Logger          = Logger::Instance();
  m_MarketData      = MarketData::Instance();
  m_VolSurfaces     = VolSurfaces::Instance();
  m_SysCfg          = SystemConfig::Instance();

  m_sAllSym.clear();
  m_sAllSym.insert(HSI_CONT_FUT_1);
  m_sAllSym.insert(HHI_CONT_FUT_1);

  m_OnOff              = m_SysCfg->Get_HKIntradaySeasonalityOnOff();
  m_TrainingResultPath = m_SysCfg->Get_HKIntradaySeasonalityPath();
  m_WindowLength       = m_SysCfg->Get_HKIntradaySeasonalityWindow();

  if (m_OnOff)
  {
    ResetTrainingResults();
    LoadTrainingResultsFromFile();
  }
}

HKIntradaySeasonality::~HKIntradaySeasonality()
{

  for (unsigned int i = 0; i < HKC1INDEX_TOT; ++i)
  {
    for (map<YYYYMMDD,vector<double> *>::iterator itTrngRes = m_TrainingResults[i].begin(); itTrngRes != m_TrainingResults[i].end(); ++itTrngRes)
    {
      delete itTrngRes->second;
      itTrngRes->second = NULL;
    }
  }

}

void HKIntradaySeasonality::ResetTrainingResults()
{
  m_TrainingResults.clear();
  m_SumInRollingWin.clear();
  m_AvgInRollingWin.clear();

  m_TrainingResults.insert(m_TrainingResults.begin(),HKC1INDEX_TOT,map<YYYYMMDD,vector<double> *>());
  m_SumInRollingWin.insert(m_SumInRollingWin.begin(),HKC1INDEX_TOT,map<YYYYMMDD,vector<double>  >());
  m_AvgInRollingWin.insert(m_AvgInRollingWin.begin(),HKC1INDEX_TOT,map<YYYYMMDD,vector<double>  >());
}


int HKIntradaySeasonality::ClassifySeasonalityBucket(const HHMMSS & hms)
{
  if       (hms <  HHMMSS( 93000)) return (TOTNUMSEASONALITYBUCKET - 24); //  0
  else if  (hms <  HHMMSS( 94500)) return (TOTNUMSEASONALITYBUCKET - 23); //  1
  else if  (hms <  HHMMSS(100000)) return (TOTNUMSEASONALITYBUCKET - 22); //  2
  else if  (hms <  HHMMSS(101500)) return (TOTNUMSEASONALITYBUCKET - 21); //  3
  else if  (hms <  HHMMSS(103000)) return (TOTNUMSEASONALITYBUCKET - 20); //  4
  else if  (hms <  HHMMSS(104500)) return (TOTNUMSEASONALITYBUCKET - 19); //  5
  else if  (hms <  HHMMSS(110000)) return (TOTNUMSEASONALITYBUCKET - 18); //  6
  else if  (hms <  HHMMSS(111500)) return (TOTNUMSEASONALITYBUCKET - 17); //  7
  else if  (hms <  HHMMSS(113000)) return (TOTNUMSEASONALITYBUCKET - 16); //  8
  else if  (hms <  HHMMSS(114500)) return (TOTNUMSEASONALITYBUCKET - 15); //  9
  else if  (hms <  HHMMSS(120000)) return (TOTNUMSEASONALITYBUCKET - 14); // 10
  else if  (hms <  HHMMSS(131500)) return (TOTNUMSEASONALITYBUCKET - 13); // 11
  else if  (hms <  HHMMSS(133000)) return (TOTNUMSEASONALITYBUCKET - 12); // 12
  else if  (hms <  HHMMSS(134500)) return (TOTNUMSEASONALITYBUCKET - 11); // 13
  else if  (hms <  HHMMSS(140000)) return (TOTNUMSEASONALITYBUCKET - 10); // 14
  else if  (hms <  HHMMSS(141500)) return (TOTNUMSEASONALITYBUCKET -  9); // 15
  else if  (hms <  HHMMSS(143000)) return (TOTNUMSEASONALITYBUCKET -  8); // 16
  else if  (hms <  HHMMSS(144500)) return (TOTNUMSEASONALITYBUCKET -  7); // 17
  else if  (hms <  HHMMSS(150000)) return (TOTNUMSEASONALITYBUCKET -  6); // 18
  else if  (hms <  HHMMSS(151500)) return (TOTNUMSEASONALITYBUCKET -  5); // 19
  else if  (hms <  HHMMSS(153000)) return (TOTNUMSEASONALITYBUCKET -  4); // 20
  else if  (hms <  HHMMSS(154500)) return (TOTNUMSEASONALITYBUCKET -  3); // 21
  else if  (hms <  HHMMSS(160000)) return (TOTNUMSEASONALITYBUCKET -  2); // 22
  else if  (hms <= HHMMSS(161500)) return (TOTNUMSEASONALITYBUCKET -  1); // 23
  else                             return (TOTNUMSEASONALITYBUCKET -  0); // 24

}

int HKIntradaySeasonality::GetTotNumSeasonalityBucket()
{
  return TOTNUMSEASONALITYBUCKET;
}

int HKIntradaySeasonality::GetNumOfMinutesInSeasonalityBucket()
{
  return 15;
}

const map<HHMMSS,double> HKIntradaySeasonality::GetHistHKItrdSeasonalBucketData(const string & symbol, const int iHKItrdSeaBkt)
{
  if (!m_OnOff) return map<HHMMSS,double>();

  boost::shared_lock<boost::shared_mutex> lock(m_HistHKItrdSeasonalBucketDataMutex);
  if (m_HistHKItrdSeasonalBucketData.empty()) return map<HHMMSS,double>();
  if (m_HistHKItrdSeasonalBucketData[symbol].empty()) return map<HHMMSS,double>();

  return (m_HistHKItrdSeasonalBucketData[symbol][iHKItrdSeaBkt]);
}

void HKIntradaySeasonality::UpdateHistHKItrdSeasonalBucketData(const string & symbol, const HHMMSS & hms, const double price)
{
  if (!m_OnOff) return;

  boost::unique_lock<boost::shared_mutex> lock(m_HistHKItrdSeasonalBucketDataMutex);

  if (m_HistHKItrdSeasonalBucketData.empty()) return;

  map<string,vector<map<HHMMSS,double> > >::iterator it = m_HistHKItrdSeasonalBucketData.find(symbol);
  if (it == m_HistHKItrdSeasonalBucketData.end()) return;

  int iHKItrdSeaBkt = HKIntradaySeasonality::ClassifySeasonalityBucket(hms);
  if (iHKItrdSeaBkt >= HKIntradaySeasonality::GetTotNumSeasonalityBucket()) return;

  (it->second)[iHKItrdSeaBkt][hms] = price;

}

void HKIntradaySeasonality::InitAndResetHistHKItrdSeasonalBucketData()
{
  if (!m_OnOff) return;

  boost::unique_lock<boost::shared_mutex> lock(m_HistHKItrdSeasonalBucketDataMutex);

  if (m_HistHKItrdSeasonalBucketData.empty())
  {
    for (set<string>::iterator it = m_sAllSym.begin(); it != m_sAllSym.end(); ++it)
    {
      m_HistHKItrdSeasonalBucketData[(*it)] = vector<map<HHMMSS,double> >(HKIntradaySeasonality::GetTotNumSeasonalityBucket(),map<HHMMSS,double>());
    }
  }

  for (map<string,vector<map<HHMMSS,double> > >::iterator it1 = m_HistHKItrdSeasonalBucketData.begin(); it1 != m_HistHKItrdSeasonalBucketData.end(); ++it1)
  {
    for (unsigned int iHKItrdSeaBkt = 0; iHKItrdSeaBkt < HKIntradaySeasonality::GetTotNumSeasonalityBucket(); ++iHKItrdSeaBkt)
    {
      it1->second[iHKItrdSeaBkt].clear();
    }
  }

  return;
}

void HKIntradaySeasonality::LoadTrainingResultsFromFile()
{

  deque<string> dqRawFile;
  STool::ReadFile(m_TrainingResultPath,dqRawFile);
  if (dqRawFile.empty()) return;

  if (dqRawFile.size() > 0)
  {

    vector<string> vs;
    for (unsigned int i = 0; i < dqRawFile.size(); ++i)
    {
      vs.clear();
      boost::split(vs, dqRawFile[i], boost::is_any_of(","));
      if (vs.size() != 4)
      {
        m_Logger->Write(Logger::ERROR,"HKIntradaySeasonality: The file format of the TrainingResults file is incorrect. Exiting. Offending line: %s", dqRawFile[i].c_str());
        sleep(1);
        exit(1);
      }

      YYYYMMDD ymd(vs[1]);
      string sSym              = vs[0];
      int iTimeBucket          = boost::lexical_cast<int>(vs[2]);
      double dNmlzdHLRangePctg = boost::lexical_cast<double>(vs[3]);

      UpdateNmlzdHLRangePctg(sSym,ymd,iTimeBucket,dNmlzdHLRangePctg);

      m_Logger->Write(Logger::INFO,"HKIntradaySeasonality: Read training result data: %s %s %d %f",
          sSym.c_str(),
          ymd.ToStr_().c_str(),
          iTimeBucket,
          dNmlzdHLRangePctg);
    }
  }

  return;
}

void HKIntradaySeasonality::AnalyzeIntrdVolatility()
{
  if (!m_OnOff) return;

  YYYYMMDDHHMMSS ymdhms_SysTime = m_MarketData->GetSystemTimeHKT();

  for (set<string>::iterator itHKc1Sym = m_sAllSym.begin(); itHKc1Sym != m_sAllSym.end(); ++itHKc1Sym)
  {
    double dHKc1MQ = 0;
    if (!m_MarketData->GetLatestMidQuote(*itHKc1Sym,dHKc1MQ)) continue;

    for (unsigned int iHKItrdSeaBkt = 0; iHKItrdSeaBkt < GetTotNumSeasonalityBucket(); ++iHKItrdSeaBkt)
    {
      //--------------------------------------------------
      // Looping through seasonal time buckets
      //--------------------------------------------------
      const map<HHMMSS,double> map_HistDataInTimeBucket = GetHistHKItrdSeasonalBucketData(*itHKc1Sym,iHKItrdSeaBkt);

      if (!map_HistDataInTimeBucket.empty())
      {
        //--------------------------------------------------
        // For each bucket
        //--------------------------------------------------
        HighLow<double> hl(GetNumOfMinutesInSeasonalityBucket(),true);
        for (map<HHMMSS,double>::const_iterator it = map_HistDataInTimeBucket.begin(); it != map_HistDataInTimeBucket.end(); ++it)
        {
          hl.Add(it->second);
        }

        HHMMSS hms                    = (map_HistDataInTimeBucket.begin())->first;
        YYYYMMDDHHMMSS ymdhms(ymdhms_SysTime.GetYYYYMMDD(),hms);

        double dHeight                = hl.Height();
        double dVHSI                  = m_VolSurfaces->GetOfficialVHSI(ymdhms);
        double dHLRangePctg           = dHeight/dHKc1MQ;
        double dNmlzdHLRangePctg      = dHeight/dHKc1MQ/dVHSI;

        m_Logger->Write(Logger::INFO,"HKIntradaySeasonality: AnalyzeIntrdVolatility %s %s %s High Low range = %f Nmlzd High Low range = %f",
            (*itHKc1Sym).c_str(),
            ymdhms_SysTime.GetYYYYMMDD().ToStr_().c_str(),
            hms.ToStr().c_str(),
            dHLRangePctg,
            dNmlzdHLRangePctg
            );

        //--------------------------------------------------
        // Save the result
        //--------------------------------------------------
        UpdateNmlzdHLRangePctg((*itHKc1Sym),ymdhms_SysTime.GetYYYYMMDD(),ClassifySeasonalityBucket(hms),dNmlzdHLRangePctg);
      }
    }
  }
  OutputTrainingResultsToFile();
  CalcAvgInRollingWin(ymdhms_SysTime.GetYYYYMMDD());

  return;
}



string HKIntradaySeasonality::GetSymbolName(const int h)
{
  return GetSymbolName(static_cast<HKC1INDEX>(h));
}

string HKIntradaySeasonality::GetSymbolName(const HKC1INDEX h)
{
  switch(h)
  {
    case HKC1INDEX_HSI: { return HSI_CONT_FUT_1; }
    case HKC1INDEX_HHI: { return HHI_CONT_FUT_1; }
    default           : { return "";             }
  }
  return "";
}


void HKIntradaySeasonality::OutputTrainingResultsToFile()
{
  ofstream fs(m_TrainingResultPath.c_str());


  for (unsigned int i = 0; i < HKC1INDEX_TOT; ++i)
  {
    for (map<YYYYMMDD,vector<double> *>::iterator itTrngRes = m_TrainingResults[i].begin(); itTrngRes != m_TrainingResults[i].end(); ++itTrngRes)
    {
      for (unsigned int iHKItrdSeaBkt = 0; iHKItrdSeaBkt < GetTotNumSeasonalityBucket(); ++iHKItrdSeaBkt)
      {
        fs << GetSymbolName(i) << "," << itTrngRes->first.ToStr_() << "," << iHKItrdSeaBkt << "," << (*itTrngRes->second)[iHKItrdSeaBkt] << endl;
      }
    }
  }

  fs.close();
  return;
}


void HKIntradaySeasonality::UpdateNmlzdHLRangePctg(const string & symbol, const YYYYMMDD & ymd, const int iTimeBucket, const double dNmlzdHLRangePctg)
{
  if (dNmlzdHLRangePctg < 0.00001) return;
  if (std::isnan(dNmlzdHLRangePctg)) return;

  HKC1INDEX iHKC1Idx;
  if      (symbol.substr(0,3) == UNDERLYING_HSI) iHKC1Idx = HKC1INDEX_HSI;
  else if (symbol.substr(0,3) == UNDERLYING_HHI) iHKC1Idx = HKC1INDEX_HHI;
  else return;

  map<YYYYMMDD,vector<double> *>::iterator itMapVec;
  itMapVec = m_TrainingResults[iHKC1Idx].find(ymd);
  if (itMapVec == m_TrainingResults[iHKC1Idx].end())
  {
    m_TrainingResults[iHKC1Idx][ymd] = new vector<double>(GetTotNumSeasonalityBucket(),NAN);
    itMapVec = m_TrainingResults[iHKC1Idx].find(ymd);
  }

  (*itMapVec->second)[iTimeBucket] = dNmlzdHLRangePctg;

  return;
}

double HKIntradaySeasonality::GetNmlzdHLRangePctg(const string & symbol, const YYYYMMDD & ymd, const int iTimeBucket)
{
  map<YYYYMMDD,vector<double> *>::iterator it;

  HKC1INDEX iHKC1Idx;
  if      (symbol.substr(0,3) == UNDERLYING_HSI) iHKC1Idx = HKC1INDEX_HSI;
  else if (symbol.substr(0,3) == UNDERLYING_HHI) iHKC1Idx = HKC1INDEX_HHI;
  else return NAN;


  it = m_TrainingResults[iHKC1Idx].find(ymd);
  if (it == m_TrainingResults[iHKC1Idx].end()) return NAN;
  else return (*it->second)[iTimeBucket];


  return NAN;
}

void HKIntradaySeasonality::CalcAvgInRollingWin(const YYYYMMDD & ymd)
{

  //--------------------------------------------------
  // FIXME: Low B implementation
  //--------------------------------------------------
  for (unsigned int iHKC1Idx = 0; iHKC1Idx < HKC1INDEX_TOT; ++iHKC1Idx)
  {
    map<YYYYMMDD,vector<double> >::iterator itSumInRllgWin = m_SumInRollingWin[iHKC1Idx].find(ymd);
    if (itSumInRllgWin == m_SumInRollingWin[iHKC1Idx].end())
    {
      m_SumInRollingWin[iHKC1Idx][ymd] = vector<double>();
      itSumInRllgWin = m_SumInRollingWin[iHKC1Idx].find(ymd);
    }

    (itSumInRllgWin->second).clear();
    (itSumInRllgWin->second).insert((itSumInRllgWin->second).begin(),GetTotNumSeasonalityBucket(),0);

    //--------------------------------------------------
    map<YYYYMMDD,vector<double> >::iterator itAvgInRllgWin = m_AvgInRollingWin[iHKC1Idx].find(ymd);
    if (itAvgInRllgWin == m_AvgInRollingWin[iHKC1Idx].end())
    {
      m_AvgInRollingWin[iHKC1Idx][ymd] = vector<double>();
      itAvgInRllgWin = m_AvgInRollingWin[iHKC1Idx].find(ymd);
    }

    (itAvgInRllgWin->second).clear();
    (itAvgInRllgWin->second).insert((itAvgInRllgWin->second).begin(),GetTotNumSeasonalityBucket(),0);

    //--------------------------------------------------
    map<YYYYMMDD,vector<double> *>::iterator it = m_TrainingResults[iHKC1Idx].find(ymd);
    if (it == m_TrainingResults[iHKC1Idx].end()) return;

    int iRemainingCnt = m_WindowLength;
    vector<int> viActualCount(GetTotNumSeasonalityBucket(),0);

    while (it != m_TrainingResults[iHKC1Idx].begin() && iRemainingCnt > 0) // Will be short of 1, but so what?
    {
      for (unsigned int iHKItrdSeaBkt = 0; iHKItrdSeaBkt < GetTotNumSeasonalityBucket(); ++iHKItrdSeaBkt)
      {
        if (!std::isnan((*it->second)[iHKItrdSeaBkt]))
        {
          (itSumInRllgWin->second)[iHKItrdSeaBkt] += (*it->second)[iHKItrdSeaBkt];
          viActualCount[iHKItrdSeaBkt]++;
        }
      }
      it--;
      iRemainingCnt--;
    }

    for (unsigned int iHKItrdSeaBkt = 0; iHKItrdSeaBkt < GetTotNumSeasonalityBucket(); ++iHKItrdSeaBkt)
    {
      (itAvgInRllgWin->second)[iHKItrdSeaBkt] = (double)(itSumInRllgWin->second)[iHKItrdSeaBkt] / (double)viActualCount[iHKItrdSeaBkt];

      m_Logger->Write(Logger::INFO,"HKIntradaySeasonality: CalcAvgInRollingWin: %s Symbol = %s TimeBucket = %d # of Samples = %d Nmlzd HL Range = %f",
          ymd.ToStr_().c_str(),
          (GetSymbolName(iHKC1Idx)).c_str(),
          iHKItrdSeaBkt,
          viActualCount[iHKItrdSeaBkt],
          (itAvgInRllgWin->second)[iHKItrdSeaBkt]
          );
    }

  }

  return;
}



double HKIntradaySeasonality::GetExpectedHLRange(const string & symbol, const YYYYMMDDHHMMSS & ymdhms)
{
  //--------------------------------------------------
  // FIXME: HHI should have it's own VHHI
  //--------------------------------------------------
  double dVHSI = m_VolSurfaces->GetOfficialVHSI(ymdhms);

  HKC1INDEX iHKC1Idx;
  if      (ContFut::GetContFutUndly(symbol) == UNDERLYING_HSI) iHKC1Idx = HKC1INDEX_HSI;
  else if (ContFut::GetContFutUndly(symbol) == UNDERLYING_HHI) iHKC1Idx = HKC1INDEX_HHI;
  else return NAN;

  double dHKc1MQ = 0;
  if (!m_MarketData->GetLatestMidQuote(symbol,dHKc1MQ)) return NAN;

  int iHKItrdSeaBkt = ClassifySeasonalityBucket(ymdhms.GetHHMMSS());

  map<YYYYMMDD,vector<double> >::iterator itAvgInRllgWin = m_AvgInRollingWin[iHKC1Idx].find(ymdhms.GetYYYYMMDD());
  if (itAvgInRllgWin == m_AvgInRollingWin[iHKC1Idx].end()) return NAN;
  return (itAvgInRllgWin->second)[iHKItrdSeaBkt] * dVHSI * dHKc1MQ;

  return NAN;
}

