#include <StrategyR7.h>

StrategyR7::StrategyR7() :
  StrategyBase()
{
  SetStrategyID(STY_R7);
  SetStyDomicileMkt(SDM_HK);
  UnsetConvenienceVarb();
  SetCloseAllPosIfOutsideTrdgHour(ACP_YESBUTNOTOVERLUNCH);
  m_iNumOfParam = 5;
}

StrategyR7::~StrategyR7()
{
}

void StrategyR7::ReadParam()
{
  m_EntryThreshold.clear();
  m_MaxSize.clear();
  m_RateOfTimeStopAdj.clear();
  m_NumOfSeeds.clear();
  m_SeedsPtsApart.clear();

  m_EntryThreshold.insert(m_EntryThreshold.begin(),m_TradedSymbols.size(),0);
  m_MaxSize.insert(m_MaxSize.begin(),m_TradedSymbols.size(),0);
  m_RateOfTimeStopAdj.insert(m_RateOfTimeStopAdj.begin(),m_TradedSymbols.size(),0);
  m_NumOfSeeds.insert(m_NumOfSeeds.begin(),m_TradedSymbols.size(),0);
  m_SeedsPtsApart.insert(m_SeedsPtsApart.begin(),m_TradedSymbols.size(),0);

  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    m_EntryThreshold   [i] = m_ParamVector[i*m_iNumOfParam +0];
    m_MaxSize          [i] = m_ParamVector[i*m_iNumOfParam +1];
    m_RateOfTimeStopAdj[i] = m_ParamVector[i*m_iNumOfParam +2];
    m_NumOfSeeds       [i] = m_ParamVector[i*m_iNumOfParam +3];
    m_SeedsPtsApart    [i] = m_ParamVector[i*m_iNumOfParam +4];
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_EntryThreshold    %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_EntryThreshold[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_MaxSize           %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_MaxSize[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_RateOfTimeStopAdj %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_RateOfTimeStopAdj[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_NumOfSeeds        %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_NumOfSeeds[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_SeedsPtsApart     %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_SeedsPtsApart[i]);
  }

}

void StrategyR7::ParamSanityCheck()
{
}

void StrategyR7::StartOfDayInit()
{
  m_RefPrice.clear();
  m_TimeStopPrice.clear();
  m_TheoPosSize.clear();
  m_SeedTradePos.clear();
  m_vNumPos_RefPx.clear();

  vector<double> vNAN;
  vector<double> v0;

  for (int iTradSym = 0; iTradSym < m_TradedSymbols.size(); ++iTradSym)
  {
    vector<double> vNAN;
    vNAN.insert(vNAN.begin(),m_NumOfSeeds[iTradSym],NAN);

    vector<double> v0;
    v0.insert(v0.begin(),m_NumOfSeeds[iTradSym],0);

    m_RefPrice.push_back(vNAN);
    m_TimeStopPrice.push_back(vNAN);
    m_TheoPosSize.push_back(v0);
    m_SeedTradePos.push_back(v0);
    m_vNumPos_RefPx.push_back(NumberPosition());
  };
}

void StrategyR7::EndOfDayCleanUp()
{
}

void StrategyR7::UnsetConvenienceVarb()
{
}

void StrategyR7::SetConvenienceVarb(const int iTradSym)
{
}

bool StrategyR7::SkipSubseqProcessingForSymbol(const int iTradSym,string & sReason)
{
  return false;
}

void StrategyR7::InitialWarmUp(const int iTradSym)
{
}

void StrategyR7::UpdateInternalDataTrng(const int iTradSym)
{
}

void StrategyR7::UpdateInternalData(const int iTradSym)
{
  for (int iSeed = 0; iSeed < m_NumOfSeeds[iTradSym]; ++iSeed)
  {
    if (
      std::isnan(m_RefPrice[iTradSym][iSeed])
      &&
      (
        m_vNumPos_RefPx[iTradSym].Size() == 0
        ||
        (
          m_vNumPos_RefPx[iTradSym].Size() > 0
          &&
          m_vNumPos_RefPx[iTradSym].IsPriceApart(m_SeedsPtsApart[iTradSym],m_SymMidQuote)
        )
      )
      )
    {
      m_RefPrice[iTradSym][iSeed] = m_SymMidQuote;
      m_TimeStopPrice[iTradSym][iSeed] = m_SymMidQuote;
      m_vNumPos_RefPx[iTradSym].AddNum(m_SymMidQuote);
      m_Logger->Write(Logger::DEBUG,"Strategy %s: %s %s Seed %d m_RefPrice updated to %f",
                      GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                      iSeed,
                      m_RefPrice[iTradSym][iSeed]);
    }
  }

}


void StrategyR7::AdjustSamplingInterval(const int iTradSym)
{
  m_MonitoringIntervalInSec = 59;
  return;
}


void StrategyR7::DetermineRegime(const int iTradSym)
{
}

void StrategyR7::PreTradePreparation(const int iTradSym)
{

  for (int iSeed = 0; iSeed < m_NumOfSeeds[iTradSym]; ++iSeed)
  {

    if (std::isnan(m_RefPrice[iTradSym][iSeed])) continue;

    //--------------------------------------------------
    // Open new trades
    //--------------------------------------------------
    if (abs(m_TheoPosSize[iTradSym][iSeed]) < m_MaxSize[iTradSym])
    {
      if (
        m_TheoPosSize[iTradSym][iSeed] <= 0
        &&
        m_SymMidQuote > m_RefPrice[iTradSym][iSeed] + m_EntryThreshold[iTradSym] * (1 + abs(m_TheoPosSize[iTradSym][iSeed]))
        )
      {
        if (m_TheoPosSize[iTradSym][iSeed] == 0) m_TimeStopPrice[iTradSym][iSeed] = m_RefPrice[iTradSym][iSeed];
        m_TheoPosSize[iTradSym][iSeed] -= 1;
        m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s iSeed %d Go Short. m_SymMidQuote = %f m_RefPrice = %f m_TimeStopPrice = %f",
                        GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        iSeed,
                        m_SymMidQuote,
                        m_RefPrice[iTradSym][iSeed],
                        m_TimeStopPrice[iTradSym][iSeed]);
      }
      else if (
        m_TheoPosSize[iTradSym][iSeed] >= 0
        &&
        m_SymMidQuote < m_RefPrice[iTradSym][iSeed] - m_EntryThreshold[iTradSym] * (1 + abs(m_TheoPosSize[iTradSym][iSeed]))

        )
      {
        if (m_TheoPosSize[iTradSym][iSeed] == 0) m_TimeStopPrice[iTradSym][iSeed] = m_RefPrice[iTradSym][iSeed];
        m_TheoPosSize[iTradSym][iSeed] += 1;
        m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s iSeed %d Go Long. m_SymMidQuote = %f m_RefPrice = %f m_TimeStopPrice = %f",
                        GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        iSeed,
                        m_SymMidQuote,
                        m_RefPrice[iTradSym][iSeed],
                        m_TimeStopPrice[iTradSym][iSeed]);
      }
    }

    //--------------------------------------------------
    // Update time stop
    //--------------------------------------------------
    if (m_TheoPosSize[iTradSym][iSeed] > 0)
    {
      m_TimeStopPrice[iTradSym][iSeed] = m_TimeStopPrice[iTradSym][iSeed] - abs(m_TimeStopPrice[iTradSym][iSeed] - m_SymMidQuote) * m_RateOfTimeStopAdj[iTradSym];
      m_Logger->Write(Logger::DEBUG,"Strategy %s: Update: %s %s iSeed %d m_SymMidQuote = %f m_RefPrice = %f m_TimeStopPrice = %f",
                      GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                      iSeed,
                      m_SymMidQuote,
                      m_RefPrice[iTradSym][iSeed],
                      m_TimeStopPrice[iTradSym][iSeed]);
    }
    else if (m_TheoPosSize[iTradSym][iSeed] < 0)
    {
      m_TimeStopPrice[iTradSym][iSeed] = m_TimeStopPrice[iTradSym][iSeed] + abs(m_TimeStopPrice[iTradSym][iSeed] - m_SymMidQuote) * m_RateOfTimeStopAdj[iTradSym];
      m_Logger->Write(Logger::DEBUG,"Strategy %s: Update: %s %s iSeed %d m_SymMidQuote = %f m_RefPrice = %f m_TimeStopPrice = %f",
                      GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                      iSeed,
                      m_SymMidQuote,
                      m_RefPrice[iTradSym][iSeed],
                      m_TimeStopPrice[iTradSym][iSeed]);
    }
    else if (m_TheoPosSize[iTradSym][iSeed] == 0)
    {
      //--------------------------------------------------
      // Move the time stop towards the reference price again
      //--------------------------------------------------
      if (
        (m_TimeStopPrice[iTradSym][iSeed] < m_RefPrice[iTradSym][iSeed]
         &&
         m_TimeStopPrice[iTradSym][iSeed] < m_SymMidQuote
        )
        ||
        (m_TimeStopPrice[iTradSym][iSeed] > m_RefPrice[iTradSym][iSeed]
         &&
         m_TimeStopPrice[iTradSym][iSeed] > m_SymMidQuote
        )
        )
        m_TimeStopPrice[iTradSym][iSeed] = m_SymMidQuote;
    }

    //--------------------------------------------------
    // Close trade
    //--------------------------------------------------
    if (
      (m_TheoPosSize[iTradSym][iSeed] > 0
       &&
       m_TimeStopPrice[iTradSym][iSeed] < m_SymMidQuote
      )
      ||
      (m_TheoPosSize[iTradSym][iSeed] < 0
       &&
       m_TimeStopPrice[iTradSym][iSeed] > m_SymMidQuote
      )
      )
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s iSeed %d Close trade (before). m_SymMidQuote = %f m_RefPrice = %f m_TimeStopPrice = %f",
                      GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                      iSeed,
                      m_SymMidQuote,
                      m_RefPrice[iTradSym][iSeed],
                      m_TimeStopPrice[iTradSym][iSeed]);

      m_TheoPosSize[iTradSym][iSeed] = 0;
      m_vNumPos_RefPx[iTradSym].RemoveNum(m_RefPrice[iTradSym][iSeed]);
      m_RefPrice[iTradSym][iSeed] = NAN;
      m_TimeStopPrice[iTradSym][iSeed] = NAN;

      m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s iSeed %d Close trade (after). m_SymMidQuote = %f m_RefPrice = %f m_TimeStopPrice = %f",
                      GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                      iSeed,
                      m_SymMidQuote,
                      m_RefPrice[iTradSym][iSeed],
                      m_TimeStopPrice[iTradSym][iSeed]);
    }


  }
}

void StrategyR7::CalcPosSize(const int iTradSym)
{

  long lAggPos = 0;
  for_each (m_TheoPosSize[iTradSym],[&](const double pos) { lAggPos += pos; });


  if (lAggPos == 0)
  {
    m_TargetTradeDir[iTradSym] = TD_NODIR;
    m_TargetAbsPos[iTradSym]   = 0;
  }
  else if (lAggPos > 0)
  {
    m_TargetTradeDir[iTradSym] = TD_LONG;
    m_TargetAbsPos[iTradSym]   = abs(lAggPos);
  }
  else if (lAggPos < 0)
  {
    m_TargetTradeDir[iTradSym] = TD_SHORT;
    m_TargetAbsPos[iTradSym]   = abs(lAggPos);
  }

}


void StrategyR7::LoadOptimalParamsFromTrainingResults()
{
  return;
}


void StrategyR7::EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}

void StrategyR7::EndOfDayTrainingRoutine(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}


