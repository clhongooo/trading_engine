#include <StrategyNIR1.h>

StrategyNIR1::StrategyNIR1() :
  StrategyBase()
{
  SetStrategyID(STY_NIR1);
  SetStyDomicileMkt(SDM_HK);
  UnsetConvenienceVarb();
  SetCloseAllPosIfOutsideTrdgHour(ACP_YESBUTNOTOVERLUNCH);
  m_iNumOfParam = 1;
}

StrategyNIR1::~StrategyNIR1()
{
}

void StrategyNIR1::ReadParam()
{
  m_MaturityYYYYMMDD = (int)m_ParamVector[0];
  m_Logger->Write(Logger::INFO,"Strategy %s: m_MaturityYYYYMMDD %d",
                  GetStrategyName(m_StyID).c_str(),
                  m_MaturityYYYYMMDD);

  FForEach(m_TradedSymbols,[&](const string & symbol) {
    long lPos = GetPrevTheoSgndPos(symbol);
    if (lPos != 0) m_map_SgndPos.AddOrUpdate(symbol, lPos);
  });

}

void StrategyNIR1::ParamSanityCheck()
{
}

void StrategyNIR1::StartOfDayInit()
{
}

void StrategyNIR1::EndOfDayCleanUp()
{
}

void StrategyNIR1::UnsetConvenienceVarb()
{
}

void StrategyNIR1::SetConvenienceVarb(const int iTradSym)
{
}

bool StrategyNIR1::SkipSubseqProcessingForSymbol(const int iTradSym,string & sReason)
{
  return false;
}

void StrategyNIR1::InitialWarmUp(const int iTradSym)
{
}

void StrategyNIR1::UpdateInternalDataTrng(const int iTradSym)
{
}

void StrategyNIR1::UpdateInternalData(const int iTradSym)
{
  if (m_HKFE->IsFut(m_TradedSymbols[iTradSym])) m_UnderlyingMQ = m_SymMidQuote;
}


void StrategyNIR1::AdjustSamplingInterval(const int iTradSym)
{
  return;
}


void StrategyNIR1::DetermineRegime(const int iTradSym)
{
}

void StrategyNIR1::PreTradePreparation(const int iTradSym)
{
  double dPortfolioDelta = 0;
  double dNetPremiumDollar = 0;
  bool bMQAllReady = true;


  vector<std::pair<string,int> > vSgndPos = m_map_SgndPos.ToVector();
  // FForEach(vSgndPos,[&](const std::pair<string,int> sym_sgndpos) {
  //   m_Logger->Write(Logger::INFO,"StrategyNIR1: %s m_map_SgndPos %s",
  //                   m_ymdhms_SysTime_HKT.ToStr().c_str(),
  //                   sym_sgndpos.first.c_str());
  // });

  // vector<std::pair<string,int> > vSgndPos;
  // vSgndPos.push_back(std::pair<string,int>("HSIK6",1));
  // vSgndPos.push_back(std::pair<string,int>("HSI20000Q6",1));

  FForEach(vSgndPos,[&](const std::pair<string,int> sym_sgndpos) {
    if (sym_sgndpos.second == 0) return;

    const string & symbol = sym_sgndpos.first;
    const double iSgndPos = sym_sgndpos.second;

    //--------------------------------------------------
    // ad-hoc
    //--------------------------------------------------
    double dBid = NAN;
    double dAsk = NAN;
    double dMQ  = NAN;
    YYYYMMDDHHMMSS ymdhms;
    if (
      !MarketData::Instance()->GetLatestBestBidAskMid(symbol,dBid,dAsk,dMQ,ymdhms)
      ||
      m_ymdhms_SysTime_HKT - ymdhms > 3600
      ||
      abs(dMQ) < NIR_EPSILON
      ||
      abs(dBid) < NIR_EPSILON
      ||
      abs(dAsk) < NIR_EPSILON
      ||
      !STool::IsValidPriceOrVol(dBid)
      ||
      !STool::IsValidPriceOrVol(dAsk)
      )
    {
      bMQAllReady = false;
      return;
    }

    if (m_HKFE->IsFut(symbol))
    {
      dPortfolioDelta += (double)iSgndPos * m_HKFE->GetContractMultiplier(symbol) / 50.0;
    }
    else if (m_HKFE->IsCall(symbol)) 
    {
      const double rf = 0.001;
      const double dStrike = m_HKFE->GetStrike(symbol);
      // double d_1FM_TTM = m_HKFE->GetTTM(m_ymdhms_SysTime_HKT,1);
      // double dTau = d_1FM_TTM;
      const double dTau = (YYYYMMDDHHMMSS(YYYYMMDD(m_MaturityYYYYMMDD),HHMMSS(160000)) - m_ymdhms_SysTime_HKT) / 60.0 / 60.0 / 24.0 / 252.0;
      const double dIV = BlackScholes::ImpldVolatilityFut(m_HKFE->IsCall(symbol), m_UnderlyingMQ, dStrike, rf, dTau, dMQ, 0.2);

      const double dDelta = BlackScholes::CallDelta(m_UnderlyingMQ, dStrike, rf, 0, dTau, dIV);
      m_Logger->Write(Logger::INFO,"StrategyNIR1: %s %s Underlying %f dMQ %f dTau %f Delta %f",
                      m_ymdhms_SysTime_HKT.ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      m_UnderlyingMQ,
                      dMQ,
                      dTau,
                      dDelta);
      dPortfolioDelta += (double)iSgndPos * dDelta * m_HKFE->GetContractMultiplier(symbol) / 50.0;
      dNetPremiumDollar += (double)iSgndPos * dMQ * m_HKFE->GetContractMultiplier(symbol);
    }
    else if (m_HKFE->IsPut(symbol)) 
    {
      const double rf = 0.001;
      const double dStrike = m_HKFE->GetStrike(symbol);
      // double d_1FM_TTM = m_HKFE->GetTTM(m_ymdhms_SysTime_HKT,1);
      // double dTau = d_1FM_TTM;
      const double dTau = (YYYYMMDDHHMMSS(YYYYMMDD(m_MaturityYYYYMMDD),HHMMSS(160000)) - m_ymdhms_SysTime_HKT) / 60.0 / 60.0 / 24.0 / 252.0;
      const double dIV = BlackScholes::ImpldVolatilityFut(m_HKFE->IsCall(symbol), m_UnderlyingMQ, dStrike, rf, dTau, dMQ, 0.2);

      const double dDelta = BlackScholes::PutDelta(m_UnderlyingMQ, dStrike, rf, 0, dTau, dIV);
      m_Logger->Write(Logger::INFO,"StrategyNIR1: %s %s Underlying %f dMQ %f dTau %f Delta %f",
                      m_ymdhms_SysTime_HKT.ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      m_UnderlyingMQ,
                      dMQ,
                      dTau,
                      dDelta);
      dPortfolioDelta += (double)iSgndPos * dDelta * m_HKFE->GetContractMultiplier(symbol) / 50.0;
      dNetPremiumDollar += (double)iSgndPos * dMQ * m_HKFE->GetContractMultiplier(symbol);
    }

  });

  if (bMQAllReady)
  {
    m_Logger->Write(Logger::INFO,"StrategyNIR1: %s Portfolio Delta %f Net Premium $ %.0f",
                    m_ymdhms_SysTime_HKT.ToStr().c_str(),
                    dPortfolioDelta,
                    dNetPremiumDollar);
  }
  else
  {
    m_Logger->Write(Logger::INFO,"StrategyNIR1: %s Portfolio Delta cannot be calculated.",
                    m_ymdhms_SysTime_HKT.ToStr().c_str());
  }

  {
    //--------------------------------------------------
    // vertical spread net premium
    //--------------------------------------------------
    const long lUpperStrike1 = round((m_UnderlyingMQ+1000.0)/200.0) * 200;
    const long lUpperStrike2 = lUpperStrike1 + 400;
    const string sBullSpreadSymbol1 = "HSI" + boost::lexical_cast<string>(lUpperStrike1) + "E6";
    const string sBullSpreadSymbol2 = "HSI" + boost::lexical_cast<string>(lUpperStrike2) + "E6";
    double dUpperStrike1MQ = NAN;
    double dUpperStrike2MQ = NAN;

    double dMQ = NAN;
    YYYYMMDDHHMMSS ymdhms;

    if (!MarketData::Instance()->GetLatestMidQuote(sBullSpreadSymbol1,dMQ,ymdhms)) return;
    dUpperStrike1MQ = dMQ;

    if (!MarketData::Instance()->GetLatestMidQuote(sBullSpreadSymbol2,dMQ,ymdhms)) return;
    dUpperStrike2MQ = dMQ;

    m_Logger->Write(Logger::INFO,"StrategyNIR1: %s Bull Spread: %s %s Net Premium = %f",
                    m_ymdhms_SysTime_HKT.ToStr().c_str(),
                    sBullSpreadSymbol1.c_str(),
                    sBullSpreadSymbol2.c_str(),
                    dUpperStrike1MQ - dUpperStrike2MQ);
  }
  
  {
    //--------------------------------------------------
    // vertical spread net premium
    //--------------------------------------------------
    const long lLowerStrike1 = round((m_UnderlyingMQ-1000.0)/200.0) * 200;
    const long lLowerStrike2 = lLowerStrike1 - 400;
    const string sBullSpreadSymbol1 = "HSI" + boost::lexical_cast<string>(lLowerStrike1) + "Q6";
    const string sBullSpreadSymbol2 = "HSI" + boost::lexical_cast<string>(lLowerStrike2) + "Q6";
    double dLowerStrike1MQ = NAN;
    double dLowerStrike2MQ = NAN;

    double dMQ = NAN;
    YYYYMMDDHHMMSS ymdhms;

    if (!MarketData::Instance()->GetLatestMidQuote(sBullSpreadSymbol1,dMQ,ymdhms)) return;
    dLowerStrike1MQ = dMQ;

    if (!MarketData::Instance()->GetLatestMidQuote(sBullSpreadSymbol2,dMQ,ymdhms)) return;
    dLowerStrike2MQ = dMQ;

    m_Logger->Write(Logger::INFO,"StrategyNIR1: %s Bear Spread: %s %s Net Premium = %f",
                    m_ymdhms_SysTime_HKT.ToStr().c_str(),
                    sBullSpreadSymbol1.c_str(),
                    sBullSpreadSymbol2.c_str(),
                    dLowerStrike1MQ - dLowerStrike2MQ);
  }


}

void StrategyNIR1::CalcPosSize(const int iTradSym)
{

}


void StrategyNIR1::LoadOptimalParamsFromTrainingResults()
{
  return;
}


void StrategyNIR1::EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}

void StrategyNIR1::EndOfDayTrainingRoutine(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}

