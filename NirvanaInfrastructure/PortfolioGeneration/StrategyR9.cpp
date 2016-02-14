#include <StrategyR9.h>

StrategyR9::StrategyR9() :
  StrategyBase()
{
  SetStrategyID(STY_R9);
  SetStyDomicileMkt(SDM_HK);
  UnsetConvenienceVarb();
  SetCloseAllPosIfOutsideTrdgHour(ACP_YESBUTNOTOVERLUNCH);
  m_iNumOfParam = 5;
}

StrategyR9::~StrategyR9()
{
}

void StrategyR9::ReadParam()
{
  m_SARThreshold.clear();
  m_TargetProfit.clear();
  m_NumOfSeeds.clear();
  m_SeedsPtsApart.clear();
  m_FixedOrTrailingStop.clear();

  m_SARThreshold.insert(m_SARThreshold.begin(),m_TradedSymbols.size(),0);
  m_TargetProfit.insert(m_TargetProfit.begin(),m_TradedSymbols.size(),0);
  m_NumOfSeeds.insert(m_NumOfSeeds.begin(),m_TradedSymbols.size(),0);
  m_SeedsPtsApart.insert(m_SeedsPtsApart.begin(),m_TradedSymbols.size(),0);
  m_FixedOrTrailingStop.insert(m_FixedOrTrailingStop.begin(),m_TradedSymbols.size(),0);

  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    m_SARThreshold       [i] = m_ParamVector[i*m_iNumOfParam +0];
    m_TargetProfit       [i] = m_ParamVector[i*m_iNumOfParam +1];
    m_NumOfSeeds         [i] = m_ParamVector[i*m_iNumOfParam +2];
    m_SeedsPtsApart      [i] = m_ParamVector[i*m_iNumOfParam +3];
    m_FixedOrTrailingStop[i] = m_ParamVector[i*m_iNumOfParam +4];
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_SARThreshold        %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_SARThreshold[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_TargetProfit        %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_TargetProfit[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_NumOfSeeds          %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_NumOfSeeds[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_SeedsPtsApart       %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_SeedsPtsApart[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_FixedOrTrailingStop %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_SeedsPtsApart[i]);
  }

}

void StrategyR9::ParamSanityCheck()
{
}

void StrategyR9::StartOfDayInit()
{
  m_StopLossPrice.clear();
  m_RefPrice.clear();
  m_SeedTradePos.clear();
  m_vNumPos_RefPx.clear();

  for (int iTradSym = 0; iTradSym < m_TradedSymbols.size(); ++iTradSym)
  {
    vector<double> vNAN;
    vNAN.insert(vNAN.begin(),m_NumOfSeeds[iTradSym],NAN);

    vector<double> v0;
    v0.insert(v0.begin(),m_NumOfSeeds[iTradSym],0);

    m_StopLossPrice.push_back(vNAN);
    m_RefPrice.push_back(vNAN);
    m_SeedTradePos.push_back(v0);
    m_vNumPos_RefPx.push_back(NumberPosition());
  };
}

void StrategyR9::EndOfDayCleanUp()
{
}

void StrategyR9::UnsetConvenienceVarb()
{
}

void StrategyR9::SetConvenienceVarb(const int iTradSym)
{
}

bool StrategyR9::SkipSubseqProcessingForSymbol(const int iTradSym,string & sReason)
{
  return false;
}

void StrategyR9::InitialWarmUp(const int iTradSym)
{
}

void StrategyR9::UpdateInternalDataTrng(const int iTradSym)
{
}

void StrategyR9::UpdateInternalData(const int iTradSym)
{
  for (int iSeed = 0; iSeed < m_NumOfSeeds[iTradSym]; ++iSeed)
  {
    if (
      std::isnan(m_RefPrice[iTradSym][iSeed])
      &&
      (
        (iSeed == 0)
        ||
        (iSeed > 0 &&
         !std::isnan(m_RefPrice[iTradSym][iSeed-1]) &&
         m_vNumPos_RefPx[iTradSym].IsPriceApart(m_SeedsPtsApart[iTradSym],m_SymMidQuote))
      )
      )
    {
      m_RefPrice[iTradSym][iSeed] = m_SymMidQuote;
      m_vNumPos_RefPx[iTradSym].AddNum(m_SymMidQuote);
      m_Logger->Write(Logger::DEBUG,"Strategy %s: %s %s Seed %d m_RefPrice updated to %f",
                      GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                      iSeed,
                      m_RefPrice[iTradSym][iSeed]);
    }
  }

}


void StrategyR9::AdjustSamplingInterval(const int iTradSym)
{
  m_MonitoringIntervalInSec = 10;
  return;
}


void StrategyR9::DetermineRegime(const int iTradSym)
{
  // double dExpHLRange = m_HKItrdSea->GetExpectedHLRange(m_TradedSymbols[iTradSym], m_ymdhms_SysTime_HKT);
  //
  // if (dExpHLRange > (2*10 + 2*m_SARThreshold[iTradSym] + m_TargetProfit[iTradSym])) m_Regime[iTradSym] = RGM_HIGHVOL;
  // else m_Regime[iTradSym] = RGM_LOWVOL;
  //
  // m_Logger->Write(Logger::DEBUG,"StrategyR9: %s %s Market regime = %d Expected HL Range = %f", m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(), m_Regime[iTradSym], dExpHLRange);

}

void StrategyR9::PreTradePreparation(const int iTradSym)
{

  for (int iSeed = 0; iSeed < m_NumOfSeeds[iTradSym]; ++iSeed)
  {
    if (m_SeedTradePos[iTradSym][iSeed] == 0)
    {
      if (!std::isnan(m_RefPrice[iTradSym][iSeed]))
      {
        if (m_SymMidQuote > m_RefPrice[iTradSym][iSeed] + m_SARThreshold[iTradSym])
        {
          m_SeedTradePos[iTradSym][iSeed] = 1;
          m_StopLossPrice[iTradSym][iSeed] = m_SymMidQuote - m_SARThreshold[iTradSym];
          m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s Seed %d Go Long. m_SymMidQuote = %f m_RefPrice = %f m_StopLossPrice = %f",
                          GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                          iSeed,
                          m_SymMidQuote,m_RefPrice[iTradSym][iSeed],m_StopLossPrice[iTradSym][iSeed]);
        }
        else if (m_SymMidQuote < m_RefPrice[iTradSym][iSeed] - m_SARThreshold[iTradSym])
        {
          m_SeedTradePos[iTradSym][iSeed] = -1;
          m_StopLossPrice[iTradSym][iSeed] = m_SymMidQuote + m_SARThreshold[iTradSym];
          m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s Seed %d Go Short. m_SymMidQuote = %f m_RefPrice = %f m_StopLossPrice = %f",
                          GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                          iSeed,
                          m_SymMidQuote,m_RefPrice[iTradSym][iSeed],m_StopLossPrice[iTradSym][iSeed]);
        }
      }
    }
    else if (m_SeedTradePos[iTradSym][iSeed] == 1)
    {
      if (m_SymMidQuote < m_StopLossPrice[iTradSym][iSeed])
      {
        m_SeedTradePos[iTradSym][iSeed] = -1;
        m_StopLossPrice[iTradSym][iSeed] = m_SymMidQuote + m_SARThreshold[iTradSym];
        m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s Seed %d Stop Long. Go Short. m_SymMidQuote = %f m_RefPrice = %f m_StopLossPrice = %f",
                        GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        iSeed,
                        m_SymMidQuote,m_RefPrice[iTradSym][iSeed],m_StopLossPrice[iTradSym][iSeed]);
      }
      else
      {
        if (m_FixedOrTrailingStop[iTradSym] == FOT_TRAILING)
        {
          //--------------------------------------------------
          // Trailing stop update
          //--------------------------------------------------
          double dPotentialNewThrshd = m_SymMidQuote - m_SARThreshold[iTradSym];
          if (dPotentialNewThrshd > m_StopLossPrice[iTradSym][iSeed])
          {
            m_StopLossPrice[iTradSym][iSeed] = dPotentialNewThrshd;
            m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s Seed %d Longing. Updated trailing stop. m_SymMidQuote = %f m_RefPrice = %f m_StopLossPrice = %f",
                            GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                            iSeed,
                            m_SymMidQuote,m_RefPrice[iTradSym][iSeed],m_StopLossPrice[iTradSym][iSeed]);
          }
        }
      }
    }
    else if (m_SeedTradePos[iTradSym][iSeed] == -1)
    {
      if (m_SymMidQuote > m_StopLossPrice[iTradSym][iSeed])
      {
        m_SeedTradePos[iTradSym][iSeed] = 1;
        m_StopLossPrice[iTradSym][iSeed] = m_SymMidQuote - m_SARThreshold[iTradSym];
        m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s Seed %d Stop short. Go Long. m_SymMidQuote = %f m_RefPrice = %f m_StopLossPrice = %f",
                        GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        iSeed,
                        m_SymMidQuote,m_RefPrice[iTradSym][iSeed],m_StopLossPrice[iTradSym][iSeed]);
      }
      else
      {
        if (m_FixedOrTrailingStop[iTradSym] == FOT_TRAILING)
        {
          //--------------------------------------------------
          // Trailing stop update
          //--------------------------------------------------
          double dPotentialNewThrshd = m_SymMidQuote + m_SARThreshold[iTradSym];
          if (dPotentialNewThrshd < m_StopLossPrice[iTradSym][iSeed])
          {
            m_StopLossPrice[iTradSym][iSeed] = dPotentialNewThrshd;
            m_Logger->Write(Logger::INFO,"Strategy %s: Trading signal: %s %s Seed %d Shorting. Updated trailing stop. m_SymMidQuote = %f m_RefPrice = %f m_StopLossPrice = %f",
                            GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                            iSeed,
                            m_SymMidQuote,m_RefPrice[iTradSym][iSeed],m_StopLossPrice[iTradSym][iSeed]);
          }
        }
      }
    }
  }
}

void StrategyR9::CalcPosSize(const int iTradSym)
{

  long lAggPos = 0;
  FForEach (m_SeedTradePos[iTradSym],[&](const double pos) { lAggPos += pos; });


  if (lAggPos == 0)
  {
    m_TargetTradeDir[iTradSym] = TD_NODIR;
    m_TargetAbsPos[iTradSym]   = 0;

    // m_RefPrice[iTradSym].clear();
    // m_RefPrice[iTradSym].insert(m_RefPrice[iTradSym].begin(),m_NumOfSeeds[iTradSym],NAN);
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

  // //--------------------------------------------------
  // // Testing
  // //--------------------------------------------------
  // m_TargetAbsPos[iTradSym] *= 0.3625;
}


void StrategyR9::LoadOptimalParamsFromTrainingResults()
{
  return;
}


void StrategyR9::EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}

void StrategyR9::EndOfDayTrainingRoutine(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}

