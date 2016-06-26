#include <StrategyBase.h>

StrategyBase::StrategyBase() :
  m_StyID(STY_LAST),
  m_MonitoringIntervalInSec(-1),
  m_iNumOfParam(-1),
  m_StartTimeBufferInSec(60),
  m_EndTimeBufferInSec(60),
  m_BreakTimeBufferInSec(60),
  m_SymMidQuote(NAN),
  m_SymFM1MidQuote(NAN),
  m_CloseAllPosIfOutsideTrdgHour(ACP_UNSET),
  m_StyRunMode(SRM_UNSET),
  m_StyDomicileMkt(SDM_UNSET),
  m_AlreadyPerformedEODRoutines(false),
  m_TreatRegularMiniAsOne(false),
  m_FirstNewDay(true)
{
  m_MarketData      = MarketData::Instance();
  m_SystemState     = SystemState::Instance();
  m_SysCfg          = SystemConfig::Instance();
  m_Logger          = Logger::Instance();
  m_TechInd         = TechIndicators::Instance();
  m_VolSurf         = VolSurfaces::Instance();
  m_PortAndOrders   = PortfoliosAndOrders::Instance();
  m_MDIAck          = MDI_Acknowledgement::Instance();
  m_Exchg           = Exchange::Instance();
  m_HKFE            = HKFE::Instance();
  m_HKSE            = HKSE::Instance();
  m_NYSE            = NYSE::Instance();
  m_CME             = CME::Instance();
  m_HKItrdSea       = HKIntradaySeasonality::Instance();
  m_ThrdHlthMon     = ThreadHealthMonitor::Instance();
  m_StyCPnLHist     = StyCPnLHist::Instance();

  m_ymdhms_LastMonitorTime.Invalidate();
  m_hmsStyGlobalMTMTime.Invalidate();

  memset(m_caStyName,'\0',HEALTHKEYLEN+1);
}

StrategyBase::~StrategyBase()
{
}

void StrategyBase::ReportAckIfNeeded()
{
  if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK || m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
    m_MDIAck->ReportAck(m_StyID);
  return;
}

void StrategyBase::SetStrategyID(const StrategyID a)
{
  m_StyID = a;
}

void StrategyBase::SetStyDomicileMkt(const STY_DOMICILE_MKT a)
{
  m_StyDomicileMkt = a;
}

StrategyID StrategyBase::GetStrategyID()
{
  return m_StyID;
}

StrategyBase::STY_DOMICILE_MKT StrategyBase::GetStyDomicileMkt()
{
  return m_StyDomicileMkt;
}

void StrategyBase::SetCloseAllPosIfOutsideTrdgHour(const AUTOCLOSEPOS b)
{
  m_CloseAllPosIfOutsideTrdgHour = b;
}


void StrategyBase::SetStyRunMode(const STY_RUN_MODE s)
{
  m_StyRunMode = s;
}

void StrategyBase::GetFutFrontMonthSymbols()
{
  m_FutFrontMthSym.clear();

  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    if      (m_TradedSymbols[i] == HSI_CONT_FUT_1) m_FutFrontMthSym.push_back(UNDERLYING_HSI + m_HKFE->GetCtrtCode1_WithDTM (HKFE::FUTURES, m_ymdhms_SysTime_HKT.GetYYYYMMDD(), 2));
    else if (m_TradedSymbols[i] == HHI_CONT_FUT_1) m_FutFrontMthSym.push_back(UNDERLYING_HHI + m_HKFE->GetCtrtCode1_WithDTM (HKFE::FUTURES, m_ymdhms_SysTime_HKT.GetYYYYMMDD(), 2));
    else if (m_TradedSymbols[i] == MHI_CONT_FUT_1) m_FutFrontMthSym.push_back(UNDERLYING_MHI + m_HKFE->GetCtrtCode1_WithDTM (HKFE::FUTURES, m_ymdhms_SysTime_HKT.GetYYYYMMDD(), 2));
    else if (m_TradedSymbols[i] == MCH_CONT_FUT_1) m_FutFrontMthSym.push_back(UNDERLYING_MCH + m_HKFE->GetCtrtCode1_WithDTM (HKFE::FUTURES, m_ymdhms_SysTime_HKT.GetYYYYMMDD(), 2));
    else if (m_TradedSymbols[i] ==  ES_CONT_FUT_1) m_FutFrontMthSym.push_back(UNDERLYING_ES  + m_CME ->GetCtrtCode1         ( CME::FUTURES, m_ymdhms_SysTime_EST.GetYYYYMMDD(), 1));
    else if (m_TradedSymbols[i] ==  NQ_CONT_FUT_1) m_FutFrontMthSym.push_back(UNDERLYING_NQ  + m_CME ->GetCtrtCode1         ( CME::FUTURES, m_ymdhms_SysTime_EST.GetYYYYMMDD(), 1));
    else if (m_TradedSymbols[i] ==  YM_CONT_FUT_1) m_FutFrontMthSym.push_back(UNDERLYING_YM  + m_CME ->GetCtrtCode1         ( CME::FUTURES, m_ymdhms_SysTime_EST.GetYYYYMMDD(), 1));
    else if (m_TradedSymbols[i] ==  TF_CONT_FUT_1) m_FutFrontMthSym.push_back(UNDERLYING_TF  + m_CME ->GetCtrtCode1         ( CME::FUTURES, m_ymdhms_SysTime_EST.GetYYYYMMDD(), 1));

    if (m_FutFrontMthSym.size() < i+1) m_FutFrontMthSym.push_back(m_TradedSymbols[i]);
  }

  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: Front month symbols: %s %s",
                    GetStrategyName(m_StyID).c_str(),
                    m_ymdhms_SysTime_Domicile.ToStr().c_str(),
                    m_FutFrontMthSym[i].c_str());
  }

  return;
}

bool StrategyBase::ShouldRollForward(const int iTradSym)
{
  if (!ContFut::IsSymContFut(m_TradedSymbols[iTradSym])) return false;
  if (
    (m_FutFrontMthSym[iTradSym] != m_PreviousTradedSym[iTradSym] && m_PreviousTradedSym[iTradSym] != "")
    &&
    (GetPrevTheoSgndPos(m_PreviousTradedSym[iTradSym]) != 0)
    &&
    (
      //--------------------------------------------------
      // the first condition means even though the time to roll forward has not arrived, we roll forward early because we have trading signal
      // the other conditions refer to the situation where no trading signal occurred before the roll forward time
      //--------------------------------------------------
      ((m_TargetTradeDir[iTradSym] == TD_SHORT ? -1 : 1) * m_TargetAbsPos[iTradSym] != GetPrevTheoSgndPos(m_PreviousTradedSym[iTradSym]))
      ||
      (m_Exchg->GetPrimaryExchange(m_TradedSymbols[iTradSym]) == EX_HKFE &&
       m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_SysCfg->Get_HKFEContFutRollFwdTime())
      ||
      (m_Exchg->GetPrimaryExchange(m_TradedSymbols[iTradSym]) == EX_CME &&
       m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_SysCfg->Get_CMEContFutRollFwdTime())
    )
    )
  {
    return true;
  }

  return false;
}

void StrategyBase::PlaceOrder(const int iTradSym)
{
  if (m_StyRunMode == SRM_UNSET)
  {
    m_Logger->Write(Logger::ERROR,"Strategy %s: PlaceOrder: STY_RUN_MODE not set.",GetStrategyName(m_StyID).c_str());
    sleep(1);
    exit(1);
  }

  //--------------------------------------------------
  // Trading Halts
  //--------------------------------------------------
  if (
    (
      (
        m_Exchg->GetPrimaryExchange(m_TargetTradedSym[iTradSym]) == EX_NYSE ||
        m_Exchg->GetPrimaryExchange(m_TargetTradedSym[iTradSym]) == EX_CME
      )
      &&
      m_SysCfg->CheckIfHaltTrading(m_TargetTradedSym[iTradSym],m_ymdhms_SysTime_EST.GetYYYYMMDD())
    )
    ||
    (m_SysCfg->CheckIfHaltTrading(m_TargetTradedSym[iTradSym],m_ymdhms_SysTime_HKT.GetYYYYMMDD()))
    )
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: %s %s Trade wasn't initiated because today is in the list of trading halts.",
                    GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_EST.ToStr().c_str(),m_TargetTradedSym[iTradSym].c_str());
    return;
  }
  //--------------------------------------------------

  double dTargetSgndPos = 0;
  if      (m_TargetTradeDir[iTradSym] == TD_LONG ) dTargetSgndPos =  abs(m_TargetAbsPos[iTradSym]);
  else if (m_TargetTradeDir[iTradSym] == TD_SHORT) dTargetSgndPos = -abs(m_TargetAbsPos[iTradSym]);

  //--------------------------------------------------
  double dMQ = 0;
  YYYYMMDDHHMMSS ymdhms_mq;
  bool bRetGLMQ = m_MarketData->GetLatestMidQuote(m_TargetTradedSym[iTradSym],dMQ,ymdhms_mq);
  //--------------------------------------------------

  long lTargetSgndPos    = (long)round(dTargetSgndPos);
  long lTargetUnsgndPos  = (long)abs(round(dTargetSgndPos));
  long lSignedQtyToTrade = (long)round(dTargetSgndPos-GetPrevTheoSgndPos(m_TargetTradedSym[iTradSym]));

  //--------------------------------------------------
  m_Logger->Write(Logger::INFO,"Strategy %s: PlaceOrder: Target symbol: %s |Qty| = %d Buy/Sell = %d lSignedQtyToTrade = %d GetPrevTheoSgndPos = %f",
                  GetStrategyName(m_StyID).c_str(), m_TargetTradedSym[iTradSym].c_str(), lTargetUnsgndPos, m_TargetTradeDir[iTradSym], lSignedQtyToTrade, GetPrevTheoSgndPos(m_TargetTradedSym[iTradSym]));

  (*m_fsSignalLog) << GetStrategyName(m_StyID) << "\t" << m_ymdhms_SysTime_HKT.GetYYYYMMDD().ToInt() << "\t" <<
    m_ymdhms_SysTime_HKT.GetHHMMSS().ToInt() << "\t" << m_TargetTradedSym[iTradSym] << "\tTgtSgndPos\t" <<
    lTargetSgndPos << "\tTradeDir\t" << m_TargetTradeDir[iTradSym] <<
    "\tNotnlAmt\t" << m_SymMidQuote * lTargetSgndPos << endl;

  m_Logger->Write(Logger::INFO,"Strategy %s: SignalLog:\t%s\t%s\tTgtSgndPos\t%d\tTradeDir\t%d\tNotnlAmt\t%f",
                  GetStrategyName(m_StyID).c_str(),
                  m_ymdhms_SysTime_Domicile.ToStr().c_str(),
                  m_TargetTradedSym[iTradSym].c_str(),
                  lTargetSgndPos,
                  m_TargetTradeDir[iTradSym],
                  m_SymMidQuote * lTargetSgndPos);

  //--------------------------------------------------
  if (!bRetGLMQ && !m_SysCfg->ChkIfProceedStyForceExcnEvenIfNoMD(m_TradedSymbols[iTradSym]))
  {
    m_Logger->Write(Logger::ERROR,"StrategyBase: %s: Price not available can't check notional amount, therefore won't trade: sym = %s",
                    m_MarketData->GetSystemTimeHKT().ToStr().c_str(),
                    m_TradedSymbols[iTradSym].c_str());
    return;
  }
  //--------------------------------------------------

  //--------------------------------------------------
  // check maximum allowed notional and quantity
  //--------------------------------------------------
  if (
    dMQ * abs(lSignedQtyToTrade) > m_SysCfg->GetMaxAllowedTradeNotional()
    ||
    abs(lSignedQtyToTrade) > m_SysCfg->GetMaxAllowedTradeQty()
    )
  {
    m_Logger->Write(Logger::ERROR,"StrategyBase: %s: Max allowed notional / qty exceeded: sym = %s notional = %f qty = %d.",
                    m_MarketData->GetSystemTimeHKT().ToStr().c_str(),
                    m_TradedSymbols[iTradSym].c_str(),
                    dMQ * lSignedQtyToTrade,
                    lSignedQtyToTrade);
    return;
  }
  //--------------------------------------------------


  if (lSignedQtyToTrade != 0)
  {
    if (m_StyRunMode == SRM_BKT_OR_LIVE)
    {
      m_PortAndOrders->Trade(m_StyID, m_TargetTradedSym[iTradSym], lSignedQtyToTrade);
    }
    else if (m_StyRunMode == SRM_TRNG)
    {
      m_TrainingAcct.Trade(m_TargetTradedSym[iTradSym],lSignedQtyToTrade,dMQ);
    }
    if (ContFut::IsSymContFut(m_TradedSymbols[iTradSym]))
    {
      //--------------------------------------------------
      // Update the continuous futures position as well
      //--------------------------------------------------
      double dPrevTheoSgndPosOfContFut = GetPrevTheoSgndPos(m_TradedSymbols[iTradSym]);
      SetPrevTheoSgndPos(m_TradedSymbols[iTradSym],lSignedQtyToTrade+dPrevTheoSgndPosOfContFut);
    }
    SetPrevTheoSgndPos(m_TargetTradedSym[iTradSym],dTargetSgndPos);

    m_PreviousTradedSym[iTradSym] = m_TargetTradedSym[iTradSym];
  }

}


void StrategyBase::SetTradeRelatedParam(const int iTradSym, const TRADEDIR td, const double abspos)
{
  if (iTradSym < 0 || iTradSym >= m_TradedSymbols.size())
  {
    m_Logger->Write(Logger::ERROR,"Strategy %s: iTradSym = %d is not recognized.",GetStrategyName(m_StyID).c_str(), iTradSym);
    sleep(1);
    exit(1);
  }
  m_TargetTradeDir[iTradSym]    = td;
  m_TargetAbsPos[iTradSym]      = abspos;
  m_TargetTradedSym[iTradSym]   = m_TradedSymbols[iTradSym];
}


void StrategyBase::ResetTradeRelatedParam(const int iTradSym)
{
  m_TargetTradeDir[iTradSym]   = TD_NODIR;
  m_TargetAbsPos[iTradSym]     = 0;
  m_TargetTradedSym[iTradSym]  = m_TradedSymbols[iTradSym];
  m_TrainingAcct.Reset();
}
void StrategyBase::ResumeOrInitTradeTargets(const int iTradSym)
{
  if (m_PreviousTradedSym[iTradSym] != "")
  {
    //--------------------------------------------------
    // continuous futures && previous position == 0: always get front month symbol
    // continuous futures && previous position != 0: always get previous symbol
    // not continuous futures                      : always get previous symbol
    //--------------------------------------------------
    if (ContFut::IsSymContFut(m_TargetTradedSym[iTradSym]) && GetPrevTheoSgndPos(m_TradedSymbols[iTradSym]) == 0)
    {
      m_TargetTradedSym[iTradSym] = m_FutFrontMthSym[iTradSym];
    }
    else 
    {
      m_TargetTradedSym[iTradSym] = m_PreviousTradedSym[iTradSym];
    }
  }
  else
  {
    //--------------------------------------------------
    // Init
    //--------------------------------------------------
    if (ContFut::IsSymContFut(m_TargetTradedSym[iTradSym]))
    {
      m_TargetTradedSym[iTradSym] = m_FutFrontMthSym[iTradSym];
    }
    else
    {
      m_TargetTradedSym[iTradSym] = m_TradedSymbols[iTradSym];
    }
  }

  double dPrevTheoSgndPosOfTgtSym = GetPrevTheoSgndPos(m_TargetTradedSym[iTradSym]);

  //--------------------------------------------------
  // Set the trade related param to previous values
  //--------------------------------------------------
  if      (dPrevTheoSgndPosOfTgtSym == 0) m_TargetTradeDir[iTradSym] = TD_NODIR;
  else if (dPrevTheoSgndPosOfTgtSym >  0) m_TargetTradeDir[iTradSym] = TD_LONG;
  else if (dPrevTheoSgndPosOfTgtSym <  0) m_TargetTradeDir[iTradSym] = TD_SHORT;

  m_TargetAbsPos[iTradSym] = abs(dPrevTheoSgndPosOfTgtSym);

}


void StrategyBase::TradeRelatedProcedure(const int iTradSym)
{

  if (!m_TreatRegularMiniAsOne)
  {
    DetermineRegime(iTradSym);
    PreTradePreparation(iTradSym);
    CalcPosSize(iTradSym);
  }
  else if (m_Exchg->IsRegularFutCtrt(m_TradedSymbols[iTradSym]))
  {
    DetermineRegime(iTradSym);
    PreTradePreparation(iTradSym);
    CalcPosSize(iTradSym);

    m_Logger->Write(Logger::INFO,"Strategy %s: %s %s Theoretical position size in regular contract. %f",
                    GetStrategyName(m_StyID).c_str(),
                    m_ymdhms_SysTime_Domicile.ToStr().c_str(),
                    m_TradedSymbols[iTradSym].c_str(),
                    m_TargetAbsPos[iTradSym]);

    double dDirVarb = 0;
    if      (m_TargetTradeDir[iTradSym] == TD_LONG ) dDirVarb =  1;
    else if (m_TargetTradeDir[iTradSym] == TD_SHORT) dDirVarb = -1;

    double dSgndTgtPos = dDirVarb * m_TargetAbsPos[iTradSym];
    m_RegularMiniCtrtMgr.UpdateRegularCtrtTgtSgnl(m_TradedSymbols[iTradSym], dSgndTgtPos);

    long lSugCtrtSize = m_RegularMiniCtrtMgr.GetSuggestedCtrtSize(m_TradedSymbols[iTradSym]);

    m_Logger->Write(Logger::INFO,"Strategy %s: Trade regular contract. %s %s %d",
                    GetStrategyName(m_StyID).c_str(),
                    m_ymdhms_SysTime_Domicile.ToStr().c_str(),
                    m_TradedSymbols[iTradSym].c_str(),
                    lSugCtrtSize);

    m_TargetTradeDir[iTradSym] = (lSugCtrtSize > 0 ? TD_LONG : TD_SHORT);
    m_TargetAbsPos  [iTradSym] = abs(lSugCtrtSize);
  }
  else if (m_Exchg->IsMiniFutCtrt(m_TradedSymbols[iTradSym]))
  {
    long lSugCtrtSize = m_RegularMiniCtrtMgr.GetSuggestedCtrtSize(m_TradedSymbols[iTradSym]);

    m_Logger->Write(Logger::INFO,"Strategy %s: Trade mini contract. %s %s %d",
                    GetStrategyName(m_StyID).c_str(),
                    m_ymdhms_SysTime_Domicile.ToStr().c_str(),
                    m_TradedSymbols[iTradSym].c_str(),
                    lSugCtrtSize);

    m_TargetTradeDir[iTradSym] = (lSugCtrtSize > 0 ? TD_LONG : TD_SHORT);
    m_TargetAbsPos  [iTradSym] = abs(lSugCtrtSize);
  }

  //--------------------------------------------------
  if (ContFut::IsSymContFut(m_TradedSymbols[iTradSym]))
  {
    //--------------------------------------------------
    // Persist before possible roll forward
    //--------------------------------------------------
    bool bShouldRoll = ShouldRollForward(iTradSym);
    if (bShouldRoll)
    {
      TRADEDIR      td_persist = m_TargetTradeDir[iTradSym];
      double       tap_persist = m_TargetAbsPos[iTradSym];

      m_TargetTradeDir [iTradSym] = TD_NODIR;
      m_TargetTradedSym[iTradSym] = m_PreviousTradedSym[iTradSym];
      m_TargetAbsPos   [iTradSym] = 0;

      PlaceOrder(iTradSym);

      m_TargetTradeDir[iTradSym]  = td_persist;
      m_TargetAbsPos[iTradSym]    = tap_persist;
    }

    if (
      m_FutFrontMthSym[iTradSym] == m_PreviousTradedSym[iTradSym] ||
      m_PreviousTradedSym[iTradSym] == ""                         ||
      bShouldRoll
      )
    {
      m_TargetTradedSym[iTradSym] = m_FutFrontMthSym[iTradSym];
    }
    else if (
      m_FutFrontMthSym[iTradSym] != m_PreviousTradedSym[iTradSym] &&
      GetPrevTheoSgndPos(m_TradedSymbols[iTradSym]) == 0
      )
    {
      m_TargetTradedSym[iTradSym] = m_FutFrontMthSym[iTradSym];
    }
    else
    {
      m_TargetTradedSym[iTradSym] = m_PreviousTradedSym[iTradSym];
    }

    PlaceOrder(iTradSym);

    //--------------------------------------------------
    // in case there is no trading signal at all, update the m_TargetTradedSym to the latest front month too
    //--------------------------------------------------
    if (
      GetPrevTheoSgndPos(m_PreviousTradedSym[iTradSym]) == 0 &&
      m_TargetAbsPos[iTradSym] == 0
      )
    {
      m_TargetTradedSym  [iTradSym] = m_FutFrontMthSym[iTradSym];
      m_PreviousTradedSym[iTradSym] = m_FutFrontMthSym[iTradSym];
    }

  }
  else
  {
    PlaceOrder(iTradSym);
  }


}



void StrategyBase::AdjustSamplingInterval(const int iTradSym)
{
  return;
}

void StrategyBase::PerformEndOfDayTraining(const int iTradSym)
{

  if (m_StyDomicileMkt == SDM_HK)
  {
    for (unsigned int iHKItrdSeaBkt = 0; iHKItrdSeaBkt < m_HKItrdSea->GetTotNumSeasonalityBucket(); ++iHKItrdSeaBkt)
    {
      //--------------------------------------------------
      // Looping through seasonal time buckets
      //--------------------------------------------------
      const map<HHMMSS,double> map_HistDataInTimeBucket = m_HKItrdSea->GetHistHKItrdSeasonalBucketData(m_TradedSymbols[iTradSym],iHKItrdSeaBkt);

      if (!map_HistDataInTimeBucket.empty())
      {
        //--------------------------------------------------
        // For each bucket
        //--------------------------------------------------
        EndOfDayTrainingForEachTimeBucket(iTradSym,map_HistDataInTimeBucket);
      }
    }
  }
  if (m_StyDomicileMkt == SDM_US)
  {
    map<HHMMSS,double> mapTmp;
    EndOfDayTrainingForEachTimeBucket(iTradSym,mapTmp);
  }
}


void StrategyBase::PerformTrainingJustBeforeTrading(const int iTradSym)
{
  return;
}

void StrategyBase::EndOfDayTrainingRoutine(const int iTradSym, const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
  StartOfDayInit();
  ResetTradeRelatedParam(iTradSym);
  //--------------------------------------------------
  // Replay data
  //--------------------------------------------------
  for (map<HHMMSS,double>::const_iterator it = map_HistDataInTimeBucket.begin(); it != map_HistDataInTimeBucket.end(); ++it)
  {
    m_Logger->Write(Logger::DEBUG,"Strategy %s: EOD Training: SysTime: %s %s HistTime: %s HistPrice = %f",
                    GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_Domicile.ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(), it->first.ToStr_().c_str(), it->second);
    m_SymMidQuote = it->second;
    UpdateInternalDataTrng(iTradSym);
    AdjustSamplingInterval(iTradSym);
    TradeRelatedProcedure(iTradSym);
  }

  //--------------------------------------------------
  // Close all positions
  //--------------------------------------------------
  set<string> setSym = m_TrainingAcct.GetSymbolsWithPos();
  for (set<string>::iterator it = setSym.begin(); it != setSym.end(); ++it)
  {
    m_TrainingAcct.Trade(*it,-m_TrainingAcct.Pos(*it),m_SymMidQuote);
  }

  EndOfDayCleanUp();
}

void StrategyBase::Run()
{

  StyInitAndChecking();

  //--------------------------------------------------
  for (;;)
  {
    //--------------------------------------------------
    // Wait for m_TechInd if in ack mode
    //--------------------------------------------------
    if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK || m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
      m_TechInd->WaitForData();
    else
      m_MarketData->WaitForData();

    if (m_SystemState->ChkIfThreadShouldStop()) break;


    //--------------------------------------------------
    if (m_caStyName[0] == '\0' || m_caStyName[0] == '_') GetStrategyNameCStr(m_StyID,m_caStyName);
    m_ThrdHlthMon->ReportHealthy(m_caStyName);
    //--------------------------------------------------

    m_ymdhms_SysTime_HKT = m_MarketData->GetSystemTimeHKT();
    m_ymdhms_SysTime_EST = m_MarketData->GetSystemTimeEST();
    if (m_StyDomicileMkt == SDM_US) m_ymdhms_SysTime_Domicile = m_ymdhms_SysTime_EST;
    else m_ymdhms_SysTime_Domicile = m_ymdhms_SysTime_HKT;

    if (!m_SysCfg->IsStrategyOn(m_StyID))
    {
      ReportAckIfNeeded();
      m_Logger->Write(Logger::DEBUG,"Strategy %s is off",GetStrategyName(m_StyID).c_str());
      continue;
    }

    StyEODSODRoutine();

    if (m_MonitoringIntervalInSec > 0 &&
        m_ymdhms_SysTime_HKT - m_ymdhms_LastMonitorTime < m_MonitoringIntervalInSec)
    {
      ReportAckIfNeeded();
      continue;
    }

    m_ymdhms_LastMonitorTime.Set(m_ymdhms_SysTime_HKT);

    //--------------------------------------------------
    // For each m_TradedSymbols
    //--------------------------------------------------
    for (unsigned int iTradSym = 0; iTradSym < m_TradedSymbols.size(); ++iTradSym)
    {

      //--------------------------------------------------
      // Just for info
      //--------------------------------------------------
      if (m_PTask_PrintPrevTheoSgndPos.CheckIfItIsTimeToWakeUp(m_PTask_PrintPrevTheoSgndPosToken[m_TradedSymbols[iTradSym]],m_ymdhms_SysTime_HKT))
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s PrevTheoSgndPos for symbol %s = %f",
                        GetStrategyName(m_StyID).c_str(),
                        m_ymdhms_SysTime_Domicile.ToStr().c_str(),
                        m_TradedSymbols[iTradSym].c_str(),
                        GetPrevTheoSgndPos(m_TradedSymbols[iTradSym]));
      }
      //--------------------------------------------------

      StyForceRollContFutIfNec(iTradSym);

      //--------------------------------------------------
      // Get latest mid-quote
      //--------------------------------------------------
      if (
        !m_SysCfg->ChkIfProceedStyForceExcnEvenIfNoMD(m_TradedSymbols[iTradSym])
        &&
        (
          (!m_MarketData->GetLatestMidQuote(m_TradedSymbols[iTradSym],m_SymMidQuote,m_ymdhms_midquote))
          // ||
          // (abs(m_ymdhms_SysTime_HKT - m_ymdhms_midquote) > 20)
        )
        )
      {
        //--------------------------------------------------
        // Not our concerned symbols
        //--------------------------------------------------
        continue;
      }

      if (ContFut::IsSymContFut(m_TradedSymbols[iTradSym]))
      {
        m_MarketData->GetLatestMidQuote(m_FutFrontMthSym[iTradSym],m_SymFM1MidQuote,m_ymdhms_midquote);
      }
      else
      {
        m_SymFM1MidQuote = m_SymMidQuote;
      }

      //--------------------------------------------------
      // Use the regular symbol to trigger signals
      //--------------------------------------------------
      if (!m_TreatRegularMiniAsOne || !m_Exchg->IsMiniFutCtrt(m_TradedSymbols[iTradSym]))
      {
        UnsetConvenienceVarb();
        SetConvenienceVarb(iTradSym);

        //--------------------------------------------------
        if (!m_HasWarmedUpInternalData[iTradSym])
        {
          InitialWarmUp(iTradSym);
          m_HasWarmedUpInternalData[iTradSym] = true;
        }

        UpdateInternalData(iTradSym);

      }

      m_StyRunMode = SRM_BKT_OR_LIVE;

      ResumeOrInitTradeTargets(iTradSym);

      //--------------------------------------------------
      // For m_TreatRegularMiniAsOne == true and Mini contract, it is ok to let it pass through, no harm
      //--------------------------------------------------
      if (!m_TreatRegularMiniAsOne || !m_Exchg->IsMiniFutCtrt(m_TradedSymbols[iTradSym]))
      {
        string sSkipReason;
        if (SkipSubseqProcessingForSymbol(iTradSym,sSkipReason))
        {
          m_Logger->Write(Logger::DEBUG,"Strategy %s: %s %s Skip reason: %s",
                          GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_Domicile.ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),sSkipReason.c_str());
          continue;
        }

        if ((m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK || m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
            &&
            (
              !m_MarketData->CheckIfSymbolIsUpdated(m_TradedSymbols[iTradSym])
              &&
              !m_SysCfg->ChkIfProceedStyForceExcnEvenIfNoMD(m_TradedSymbols[iTradSym])
            )
           )
        {
          continue;
        }
        //--------------------------------------------------

        //--------------------------------------------------
        // Wait for all the synchronized symbols
        //--------------------------------------------------
        {
          set<string> setSyncGrp = m_SysCfg->GetSynchronizedSymbols(m_StyID,m_TradedSymbols[iTradSym]);
          bool b1OrMoreStale = false;
          FForEach (setSyncGrp,[&](const string & sym) {
            double dMQ = 0;
            YYYYMMDDHHMMSS ymdhms_mq;
            if (!m_MarketData->GetLatestMidQuote(sym,dMQ,ymdhms_mq)) b1OrMoreStale = true;
            if (m_ymdhms_SysTime_HKT - ymdhms_mq > m_SysCfg->GetSynchronizedSymbolsSec(m_StyID))
            {
              m_Logger->Write(Logger::DEBUG,"Strategy %s: %s in the synchronized group is out-of-date. %s",
                              GetStrategyName(m_StyID).c_str(),
                              sym.c_str(),
                              ymdhms_mq.ToStr().c_str());
              b1OrMoreStale = true;
            }
          });

          if (b1OrMoreStale)
          {
            string sGrp;
            FForEach (setSyncGrp,[&](const string & s) { sGrp += s + ","; });
            m_Logger->Write(Logger::DEBUG,"Strategy %s: %s %s Not all symbols in the synchronized group are up-to-date. %s",
                            GetStrategyName(m_StyID).c_str(),
                            m_ymdhms_SysTime_Domicile.ToStr().c_str(),
                            m_TradedSymbols[iTradSym].c_str(),
                            sGrp.c_str());
            continue;
          }
        }
      }

      if (m_Exchg->IsTradingHour(m_TradedSymbols[iTradSym], m_ymdhms_SysTime_HKT, m_StartTimeBufferInSec, m_EndTimeBufferInSec, m_BreakTimeBufferInSec))
      {
        //--------------------------------------------------
        // Within our trading time
        //--------------------------------------------------
        if (m_WhetherToRetrain && !m_AlreadyPerformedJustBefTrdgTraining[iTradSym])
        {
          UnsetConvenienceVarb();
          SetConvenienceVarb(iTradSym);
          PerformTrainingJustBeforeTrading(iTradSym);
          m_AlreadyPerformedJustBefTrdgTraining[iTradSym] = true;
        }
        TradeRelatedProcedure(iTradSym);
      }
      else
      {
        //--------------------------------------------------
        // Outside our trading time
        //--------------------------------------------------
        if (
          m_CloseAllPosIfOutsideTrdgHour == ACP_YES
          ||
          (
            m_CloseAllPosIfOutsideTrdgHour == ACP_YESBUTNOTOVERLUNCH
            &&
            m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_Exchg->GetTimeNSecBefClose(m_TradedSymbols[iTradSym],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),m_EndTimeBufferInSec)
          )
          )
        {

          if (!m_TreatRegularMiniAsOne)
          {
            if (GetPrevTheoSgndPos(m_PreviousTradedSym[iTradSym]) != 0)
            {
              m_Logger->Write(Logger::INFO,"Strategy %s: Automatically close outstanding positions as this is outside our allowed trading time. %s %s",
                              GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_Domicile.ToStr().c_str(),m_PreviousTradedSym[iTradSym].c_str());
              m_TargetTradeDir [iTradSym] = TD_NODIR;
              m_TargetAbsPos   [iTradSym] = 0;
              m_TargetTradedSym[iTradSym] = m_PreviousTradedSym[iTradSym];

              PlaceOrder(iTradSym);
            }
          }
          else
          {
            if (m_Exchg->IsRegularFutCtrt(m_TradedSymbols[iTradSym]))
              m_RegularMiniCtrtMgr.UpdateRegularCtrtTgtSgnl(m_TradedSymbols[iTradSym], 0);

            long lSugCtrtSize = m_RegularMiniCtrtMgr.GetSuggestedCtrtSize(m_TradedSymbols[iTradSym]);

            m_Logger->Write(Logger::INFO,"Strategy %s: Automatically close outstanding positions as this is outside our allowed trading time. %s %s",
                            GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_Domicile.ToStr().c_str(),m_PreviousTradedSym[iTradSym].c_str());
            m_TargetTradeDir [iTradSym] = (lSugCtrtSize > 0 ? TD_LONG : TD_SHORT);
            m_TargetAbsPos   [iTradSym] = abs(lSugCtrtSize);
            m_TargetTradedSym[iTradSym] = m_PreviousTradedSym[iTradSym];

            PlaceOrder(iTradSym);
          }
        }

      }

    }

    ReportAckIfNeeded();

  }

  m_Logger->Write(Logger::NOTICE,"Strategy %s has ended.", GetStrategyName(m_StyID).c_str());
  sleep(2);

  return;
}

void StrategyBase::SetPrevTheoSgndPos(const string & sym, const double pos)
{
  m_PrevTheoSgndPos[sym] = pos;
}
double StrategyBase::GetPrevTheoSgndPos(const string & sym)
{
  map<string,double>::iterator it = m_PrevTheoSgndPos.find(sym);
  if (it == m_PrevTheoSgndPos.end())
  {
    double dActPos = 0;

    if (m_SysCfg->PositionPersistenceIsEnabled())
    {
      //--------------------------------------------------
      // keep trying to get until success
      //--------------------------------------------------
      dActPos = m_PortAndOrders->GetActualSignedPosition(m_StyID, sym);
      while (std::isnan(dActPos))
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Oops. m_PortAndOrders->GetActualSignedPosition(m_StyID, sym) returning NAN",
                        GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_Domicile.ToStr().c_str());
        usleep(500000);
        dActPos = m_PortAndOrders->GetActualSignedPosition(m_StyID, sym);
      }
    }

    m_PrevTheoSgndPos[sym] = dActPos;
    return dActPos;
  }
  else return it->second;
}


void StrategyBase::StyInitAndChecking()
{
  if (m_StyID == STY_LAST)
  {
    m_Logger->Write(Logger::ERROR,"Strategy %s: Strategy ID not set.",GetStrategyName(m_StyID).c_str());
    sleep(1);
    exit(1);
  }

  if (m_CloseAllPosIfOutsideTrdgHour == ACP_UNSET)
  {
    m_Logger->Write(Logger::ERROR,"Strategy %s: m_CloseAllPosIfOutsideTrdgHour not set.",GetStrategyName(m_StyID).c_str());
    sleep(1);
    exit(1);
  }

  if (m_StyDomicileMkt == SDM_UNSET)
  {
    m_Logger->Write(Logger::ERROR,"Strategy %s: m_StyDomicileMkt not set.",GetStrategyName(m_StyID).c_str());
    sleep(1);
    exit(1);
  }

  m_StyRunMode = SRM_BKT_OR_LIVE;

  //--------------------------------------------------
  m_fsSignalLog.reset(new ofstream(m_SysCfg->GetSignalLogPath(m_StyID).c_str()));

  //--------------------------------------------------

  m_ymdhms_LastMonitorTime.Set(m_MarketData->GetSystemTimeHKT());
  m_hmsStyGlobalMTMTime   = m_SysCfg->GetStrategyMTMTime(m_StyID);
  m_StartTimeBufferInSec  = m_SysCfg->GetStrategyStartTimeBufferInSec(m_StyID);
  m_EndTimeBufferInSec    = m_SysCfg->GetStrategyEndTimeBufferInSec(m_StyID);
  m_BreakTimeBufferInSec  = m_SysCfg->GetStrategyBreakTimeBufferInSec(m_StyID);
  m_TradedSymbols         = m_SysCfg->GetTradedSymbols(m_StyID);
  m_TreatRegularMiniAsOne = m_SysCfg->ChkIfTreatRegularMiniAsOne(m_StyID);

  for (unsigned int i = 0; i < NUMOFMETHODS; ++i)
  {
    m_map_ymdhms_LastTrainTime[i] = map<string,YYYYMMDDHHMMSS>();
    for (unsigned int j = 0; j < m_TradedSymbols.size(); ++j)
    {
      m_map_ymdhms_LastTrainTime[i][m_TradedSymbols[j]] = YYYYMMDDHHMMSS();
      // m_map_ymdhms_LastTrainTime[i][m_TradedSymbols[j]].Set(m_MarketData->GetSystemTimeHKT()); // don't set to current time, otherwise it won't train initially
    }
  }

  m_AlreadyPerformedJustBefTrdgTraining.clear();
  m_AlreadyPerformedJustBefTrdgTraining.insert(m_AlreadyPerformedJustBefTrdgTraining.begin(),m_TradedSymbols.size(),false);

  m_HasWarmedUpInternalData.clear();
  m_HasWarmedUpInternalData.insert(m_HasWarmedUpInternalData.begin(),m_TradedSymbols.size(),false);

  //--------------------------------------------------
  m_ParamVector             = m_SysCfg->GetParamVector(m_StyID);
  m_WhetherToRetrain        = m_SysCfg->GetWhetherToRetrain(m_StyID);
  m_TradedSymbols           = m_SysCfg->GetTradedSymbols(m_StyID);
  m_MonitoringIntervalInSec = m_SysCfg->GetMonitoringIntervalInSec(m_StyID);
  ReadParam();
  ParamSanityCheck();

  if (m_iNumOfParam < 0)
  {
    m_Logger->Write(Logger::ERROR,"Strategy %s: Wrong parameter vector. the number of fields is incorrect. Exiting.", GetStrategyName(m_StyID).c_str());
    sleep(1);
    exit(1);
  }


  //--------------------------------------------------
  m_TargetTradeDir.clear();
  m_TargetTradeDir.insert(m_TargetTradeDir.begin(),m_TradedSymbols.size(),TD_NODIR);

  m_TargetAbsPos.clear();
  m_TargetAbsPos.insert(m_TargetAbsPos.begin(),m_TradedSymbols.size(),NAN);

  m_PrevTheoSgndPos.clear();

  m_TargetTradedSym.clear();
  m_TargetTradedSym.insert(m_TargetTradedSym.begin(),m_TradedSymbols.size(),"");

  m_PreviousTradedSym.clear();
  m_PreviousTradedSym.insert(m_PreviousTradedSym.begin(),m_TradedSymbols.size(),"");


  //--------------------------------------------------
  // The purpose of calling it once is to get the actual position before everything; that is so important
  //--------------------------------------------------
  FForEach (m_TradedSymbols,[&](const string & sym) {

    double dPrevTheoSgndPos = GetPrevTheoSgndPos(sym);
    m_Logger->Write(Logger::INFO,"Strategy %s: PrevTheoSgndPos for symbol %s = %f",
                    GetStrategyName(m_StyID).c_str(),
                    sym.c_str(),
                    dPrevTheoSgndPos);

    m_PTask_PrintPrevTheoSgndPosToken[sym] = m_PTask_PrintPrevTheoSgndPos.GetNewTokenAndSetIntervalInSec(60);

  });
  //--------------------------------------------------

}


void StrategyBase::StyEODSODRoutine()
{
  //--------------------------------------------------
  bool bNewDay = false;
  for (int iTradSym = 0; iTradSym < m_TradedSymbols.size(); ++iTradSym)
  {
    if (m_Exchg->IsNewDay(m_TradedSymbols[iTradSym],m_ymdhms_SysTime_HKT,m_ymdhms_LastMonitorTime))
      bNewDay = true;
  }
  if (m_FirstNewDay)
  {
    bNewDay = true;
    m_FirstNewDay = false;
  }
  //--------------------------------------------------
  if (
    (
      !m_AlreadyPerformedEODRoutines
      &&
      (!m_ymdhms_LastMonitorTime.IsValid() || bNewDay)
    )
    )
  {
    if (m_WhetherToRetrain)
    {
      m_StyRunMode = SRM_TRNG;

      GetFutFrontMonthSymbols();
      for (unsigned int iTradSym = 0; iTradSym < m_TradedSymbols.size(); ++iTradSym)
      {
        UnsetConvenienceVarb();
        SetConvenienceVarb(iTradSym);
        PerformEndOfDayTraining(iTradSym);
      }

      m_StyRunMode = SRM_BKT_OR_LIVE;
    }
    EndOfDayCleanUp();
    if (m_TreatRegularMiniAsOne)
    {
      m_RegularMiniCtrtMgr.NetOffRndOffRegularMini();
      for (unsigned int iTradSym = 0; iTradSym < m_TradedSymbols.size(); ++iTradSym)
      {
        if (ContFut::IsSymContFut(m_TradedSymbols[iTradSym]))
        {
          long lSugCtrtSize = m_RegularMiniCtrtMgr.GetSuggestedCtrtSize(m_TradedSymbols[iTradSym]);
          m_PrevTheoSgndPos[m_TradedSymbols[iTradSym]]     = lSugCtrtSize;
          m_PrevTheoSgndPos[m_PreviousTradedSym[iTradSym]] = lSugCtrtSize;
        }
      };
    }
    m_AlreadyPerformedEODRoutines = true;
  }

  //--------------------------------------------------
  if (!m_ymdhms_LastMonitorTime.IsValid() || bNewDay)
  {
    StartOfDayInit();
    LoadOptimalParamsFromTrainingResults();
    GetFutFrontMonthSymbols();
    m_ymdhms_LastMonitorTime.Set(m_ymdhms_SysTime_HKT);
    m_AlreadyPerformedEODRoutines = false;
    m_AlreadyPerformedJustBefTrdgTraining.clear();
    m_AlreadyPerformedJustBefTrdgTraining.insert(m_AlreadyPerformedJustBefTrdgTraining.begin(),m_TradedSymbols.size(),false);

  }

}

void StrategyBase::StyForceRollContFutIfNec(const int iTradSym)
{
  //--------------------------------------------------
  // Force roll forward if not already done so
  //--------------------------------------------------
  if (
    ContFut::IsSymContFut(m_TradedSymbols[iTradSym])
    &&
    GetPrevTheoSgndPos(m_PreviousTradedSym[iTradSym]) != 0
    &&
    (
      (m_Exchg->GetPrimaryExchange(m_TradedSymbols[iTradSym]) == EX_HKFE &&
       m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_SysCfg->Get_HKFEContFutRollFwdTime())
      ||
      (m_Exchg->GetPrimaryExchange(m_TradedSymbols[iTradSym]) == EX_CME &&
       m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_SysCfg->Get_CMEContFutRollFwdTime())
    )
    )
  {
    //--------------------------------------------------
    // Persist before possible roll forward
    //--------------------------------------------------
    double ptsp_persist = GetPrevTheoSgndPos(m_PreviousTradedSym[iTradSym]);

    if (ShouldRollForward(iTradSym))
    {
      m_TargetTradeDir [iTradSym] = TD_NODIR;
      m_TargetTradedSym[iTradSym] = m_PreviousTradedSym[iTradSym];
      m_TargetAbsPos   [iTradSym] = 0;

      PlaceOrder(iTradSym);

      //--------------------------------------------------
      // Re-establish position in the new front month
      //--------------------------------------------------
      m_TargetTradeDir[iTradSym]  = (ptsp_persist > 0 ? TD_LONG : TD_SHORT);
      m_TargetAbsPos[iTradSym]    = abs(ptsp_persist);
      m_TargetTradedSym[iTradSym] = m_FutFrontMthSym[iTradSym];

      PlaceOrder(iTradSym);
    }
  }
}
