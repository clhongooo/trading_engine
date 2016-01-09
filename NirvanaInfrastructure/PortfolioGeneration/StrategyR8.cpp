#include <StrategyR8.h>

StrategyR8::StrategyR8() :
  StrategyBase()
{
  SetStrategyID(STY_R8);
  SetStyDomicileMkt(SDM_HK);
  UnsetConvenienceVarb();
  SetCloseAllPosIfOutsideTrdgHour(ACP_YES);
  m_iNumOfParam = 6;
}

StrategyR8::~StrategyR8()
{
}

void StrategyR8::ReadParam()
{
  m_DownTrendWindowInSec   .clear();
  m_DownTrendMagnitude     .clear();
  m_DownTrendSlopeThreshold.clear();
  m_ReboundWindowInSec     .clear();
  m_TakeProfitMul          .clear();
  m_StopLossMul            .clear();

  m_DownTrendWindowInSec   .insert(m_DownTrendWindowInSec   .begin(),m_TradedSymbols.size(),0);
  m_DownTrendMagnitude     .insert(m_DownTrendMagnitude     .begin(),m_TradedSymbols.size(),0);
  m_DownTrendSlopeThreshold.insert(m_DownTrendSlopeThreshold.begin(),m_TradedSymbols.size(),0);
  m_ReboundWindowInSec     .insert(m_ReboundWindowInSec     .begin(),m_TradedSymbols.size(),0);
  m_TakeProfitMul          .insert(m_TakeProfitMul          .begin(),m_TradedSymbols.size(),0);
  m_StopLossMul            .insert(m_StopLossMul            .begin(),m_TradedSymbols.size(),0);

  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    m_DownTrendWindowInSec   [i] = m_ParamVector[i*m_iNumOfParam+0];
    m_DownTrendMagnitude     [i] = m_ParamVector[i*m_iNumOfParam+1];
    m_DownTrendSlopeThreshold[i] = m_ParamVector[i*m_iNumOfParam+2];
    m_ReboundWindowInSec     [i] = m_ParamVector[i*m_iNumOfParam+3];
    m_TakeProfitMul          [i] = m_ParamVector[i*m_iNumOfParam+4];
    m_StopLossMul            [i] = m_ParamVector[i*m_iNumOfParam+5];
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_DownTrendWindowInSec    %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_DownTrendWindowInSec   [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_DownTrendMagnitude      %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_DownTrendMagnitude     [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_DownTrendSlopeThreshold %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_DownTrendSlopeThreshold[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_ReboundWindowInSec      %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_ReboundWindowInSec     [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_TakeProfitMul           %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_TakeProfitMul          [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_StopLossMul             %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_StopLossMul            [i]);
  }

}

void StrategyR8::ParamSanityCheck()
{
}

void StrategyR8::StartOfDayInit()
{
  m_EntryPrice.clear();
  m_StopLossPrice.clear();
  m_TakeProfitPrice.clear();
  m_vLinRegr.clear();
  m_vHighLowPrice.clear();
  m_vHighLowSlope.clear();

  for (unsigned int iTradSym = 0; iTradSym < m_TradedSymbols.size(); ++iTradSym)
  {
    m_EntryPrice.push_back(NAN);
    m_StopLossPrice.push_back(NAN);
    m_TakeProfitPrice.push_back(NAN);
    m_vLinRegr.push_back(LinRegr(m_DownTrendWindowInSec[iTradSym],true));
    m_vHighLowPrice.push_back(HighLow<double>(m_DownTrendWindowInSec[iTradSym],true));
    m_vHighLowSlope.push_back(HighLow<double>(m_DownTrendWindowInSec[iTradSym],true));
  }
}

void StrategyR8::EndOfDayCleanUp()
{
}

void StrategyR8::UnsetConvenienceVarb()
{
}

void StrategyR8::SetConvenienceVarb(const int iTradSym)
{
}

bool StrategyR8::SkipSubseqProcessingForSymbol(const int iTradSym,string & sReason)
{
  return false;
}

void StrategyR8::InitialWarmUp(const int iTradSym)
{
}

void StrategyR8::UpdateInternalDataTrng(const int iTradSym)
{
}

void StrategyR8::UpdateInternalData(const int iTradSym)
{
  m_vHighLowPrice[iTradSym].Add(m_SymMidQuote);

  m_vLinRegr[iTradSym].AddY(m_SymMidQuote);
  if (m_vLinRegr[iTradSym].Ready())
  {
    m_vHighLowSlope[iTradSym].Add(m_vLinRegr[iTradSym].Value());
  }
}


void StrategyR8::AdjustSamplingInterval(const int iTradSym)
{
  // m_MonitoringIntervalInSec = 10;
  return;
}


void StrategyR8::DetermineRegime(const int iTradSym)
{
}

void StrategyR8::PreTradePreparation(const int iTradSym)
{

  if (
      m_vHighLowSlope[iTradSym].Height() <= m_DownTrendSlopeThreshold[iTradSym] &&
      m_vHighLowPrice[iTradSym].Height() >= m_DownTrendMagnitude[iTradSym]
     )
  {
    SetTradeRelatedParam(iTradSym,TD_NODIR,0);


    m_StopLossPrice[iTradSym]   = 0;
    m_TakeProfitPrice[iTradSym] = 0;
  }




}

void StrategyR8::CalcPosSize(const int iTradSym)
{
  m_TargetAbsPos[iTradSym] = 1;
}


void StrategyR8::LoadOptimalParamsFromTrainingResults()
{
  return;
}


void StrategyR8::EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
  return;
}

void StrategyR8::EndOfDayTrainingRoutine(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}

