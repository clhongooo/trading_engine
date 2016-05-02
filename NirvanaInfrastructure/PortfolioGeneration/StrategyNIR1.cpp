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
  m_MidQuote.AddOrUpdate(m_TradedSymbols[iTradSym], m_SymMidQuote);
  m_SgndPos.AddOrUpdate(m_TradedSymbols[iTradSym], GetPrevTheoSgndPos(m_TradedSymbols[iTradSym]));
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

  // vector<std::pair<string,int> > vSgndPos = m_SgndPos.ToVector();

  vector<std::pair<string,int> > vSgndPos;
  vSgndPos.push_back(std::pair<string,int>("HSIK6",1));
  vSgndPos.push_back(std::pair<string,int>("HSI20000Q6",1));

  // m_Logger->Write(Logger::INFO,"StrategyNIR1: IsCall %s", m_HKFE->IsCall("HSI20000Q6")?"T":"F");
  // m_Logger->Write(Logger::INFO,"StrategyNIR1: IsPut %s", m_HKFE->IsPut("HSI20000Q6")?"T":"F");
  // m_Logger->Write(Logger::INFO,"StrategyNIR1: IsFut %s", m_HKFE->IsFut("HSIK6")?"T":"F");


  FForEach(vSgndPos,[&](const std::pair<string,int> sym_sgndpos) {
    if (sym_sgndpos.second == 0) return;

    const string & symbol = sym_sgndpos.first;
    const double iSgndPos = sym_sgndpos.second;

    Option<double> oMQ = m_MidQuote.Get(symbol);
    if (oMQ.IsNone()) return;
    double dMQ = oMQ.Get();

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
    }

  });


  m_Logger->Write(Logger::INFO,"StrategyNIR1: %s %s Portfolio Delta %f",
                  m_ymdhms_SysTime_HKT.ToStr().c_str(),
                  m_TradedSymbols[iTradSym].c_str(),
                  dPortfolioDelta);




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

