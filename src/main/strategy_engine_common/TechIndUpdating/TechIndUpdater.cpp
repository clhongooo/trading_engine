#include <TechIndUpdater.h>

TechIndUpdater::TechIndUpdater() :
  m_temp_open(0),
  m_temp_high(0),
  m_temp_low(0),
  m_temp_close(0),
  m_temp_volume(0),
  m_WarmedUp(false)
{
  m_MarketData      = MarketData::Instance();
  m_SystemState     = SystemState::Instance();
  m_SysCfg          = SystemConfig::Instance();
  m_Logger          = Logger::Instance();
  m_TechInd         = TechIndicators::Instance();
  // m_VolSurfaces     = VolSurfaces::Instance();
  m_Exchg           = Exchange::Instance();
  m_HKFE            = HKFE::Instance();
  m_HKSE            = HKSE::Instance();
  m_NYSE            = NYSE::Instance();
  m_MDIAck          = MDI_Acknowledgement::Instance();
  // m_HKItrdSea       = HKIntradaySeasonality::Instance();
  m_ThrdHlthMon     = ThreadHealthMonitor::Instance();

  m_sHKc1Sym.insert(HSI_CONT_FUT_1);
  m_sHKc1Sym.insert(HHI_CONT_FUT_1);

  m_sHKIdx.insert(UNDERLYING_HSI);
  m_sHKIdx.insert(UNDERLYING_HHI);

  for (unsigned int i = 0; i < MAX_OTIMDI; ++i)
  {
    vector<string> vs = m_SysCfg->GetMDISubscriptionSymbols(i);
    m_sOtherSym.insert(vs.begin(),vs.end());
  }

  //--------------------------------------------------
  m_sAllSym.insert(m_sHKc1Sym.begin(),m_sHKc1Sym.end());
  m_sAllSym.insert(m_sHKIdx.begin(),m_sHKIdx.end());
  m_sAllSym.insert(m_sOtherSym.begin(),m_sOtherSym.end());
  //--------------------------------------------------

  FForEach(m_sAllSym,[&](const string & sym) {
    m_PTask_PrintDayHighLowToken[sym] = m_PTask_PrintDayHighLow.GetNewTokenAndSetIntervalInSec(10);
  });
}

TechIndUpdater::~TechIndUpdater()
{
}

void TechIndUpdater::Run()
{
  //--------------------------------------------------
  // Param vector of strategies
  //--------------------------------------------------
  vector<double> m_S11A_ParamVector     = m_SysCfg->GetParamVector(STY_S11A);

  vector<long> m_S11A_T1GBMDeltaTPrds          = m_SysCfg->Get_S11AT1GBMDeltaTPeriods();
  double       m_S11A_T1GumbelCauchyDeltaTPrds = m_SysCfg->Get_S11AT1GumbelCauchyPeriod();
  double       m_S11A_T1GumbelGaussDeltaTPrds  = m_SysCfg->Get_S11AT1GumbelGaussPeriod();
  vector<long> m_S11A_T2MAPeriods              = m_SysCfg->Get_S11AT2MAPeriods();


  //--------------------------------------------------
  // Load intraday high low from IB
  // AGG,110.94,110.95,110.95,110.97,110.75,,,,
  // BND,83.11,83.12,83.12,83.14,82.93,,,,
  // LQD,119.46,119.47,119.47,119.48,119.14,,,,
  // TLT,131.98,131.99,131.98,132.16,131.25,,,,
  //--------------------------------------------------
  const string sLoadItrdHighLowFromIB = m_SysCfg->Get_LoadItrdHighLowFromIB();

  m_Logger->Write(Logger::INFO,"TechIndUpdater: Get_LoadItrdHighLowFromIB [%s]", sLoadItrdHighLowFromIB.c_str());
  if (sLoadItrdHighLowFromIB != "")
  {
    deque<string> dqS;
    STool::ReadFile(sLoadItrdHighLowFromIB, dqS);

    FForEach(dqS,[&](const string sLine) {
      vector<string> vs;
      boost::split(vs, sLine, boost::is_any_of(","));

      if (vs[0] == "Contract" ||
          vs[4] == "NoMD" ||
          vs[5] == "NoMD" ||
          vs[0] == "" ||
          vs[4] == "" ||
          vs[5] == "") return;
      m_TechInd->AddDayOHLC(vs[0],boost::lexical_cast<double>(vs[4]));
      m_TechInd->AddDayOHLC(vs[0],boost::lexical_cast<double>(vs[5]));
    });
  }


  //--------------------------------------------------
  // Warmup with supplementary data
  //--------------------------------------------------
  vector<YYYYMMDD> vYMD;
  vector<double>   vOpen;
  vector<double>   vHigh;
  vector<double>   vLow;
  vector<double>   vClose;
  vector<long>     vVol;


  // m_HKItrdSea->InitAndResetHistHKItrdSeasonalBucketData();

  //--------------------------------------------------
  for (;;)
  {
    m_MarketData->WaitForData();
    m_ymdhms_SysTimeHKT = m_MarketData->GetSystemTimeHKT();


    if (m_SystemState->ChkIfThreadShouldStop()) break;

    m_ThrdHlthMon->ReportThatIAmHealthy(ID_TECHINDUPDATER);

    //--------------------------------------------------
    if (!m_ymdhms_LastMonitorTime.IsValid())
    {
      m_ymdhms_LastMonitorTime = m_MarketData->GetSystemTimeHKT();
    }
    //--------------------------------------------------

    //--------------------------------------------------
    // Warmup with supplementary data
    //--------------------------------------------------
    if (!m_WarmedUp)
    {
      for (set<string>::iterator it = m_sAllSym.begin(); it != m_sAllSym.end(); ++it)
      {
        const string & sym = *it;

        if (!m_SysCfg->CheckIfSymToBeWarmedUpWithDayBars(sym)) continue;

        m_Logger->Write(Logger::INFO,"TechIndUpdater: GetSuppD1BarOHLCVInDateRange before %s %d", __FILE__, __LINE__);
        m_MarketData->GetSuppD1BarOHLCVInDateRange(sym, YYYYMMDD(19000101), m_ymdhms_SysTimeHKT.GetYYYYMMDD(), vYMD, vOpen, vHigh, vLow, vClose, vVol);
        m_Logger->Write(Logger::INFO,"TechIndUpdater: GetSuppD1BarOHLCVInDateRange after %s %d", __FILE__, __LINE__);

        if (!vYMD.empty() && (m_ymdhms_SysTimeHKT.GetYYYYMMDD() > vYMD.back() && m_ymdhms_SysTimeHKT.GetYYYYMMDD() - vYMD.back() > 5))
        {
          m_Logger->Write(Logger::ERROR,"TechIndUpdater: Supplementary day bar may be out-of-date. Sym=%s Current date: %s. Last supp day bar date: %s.",
                          sym.c_str(), m_ymdhms_SysTimeHKT.GetYYYYMMDD().ToStr_().c_str(), (vYMD.empty() ? "Nil" : vYMD.back().ToStr_().c_str()));
          // sleep(1);
          // exit(1);
        }

        for (unsigned int j = 0; j < vYMD.size(); ++j)
        {
          if (vYMD[j] != m_ymdhms_SysTimeHKT.GetYYYYMMDD())
          {
            m_TechInd->AddBarGKYZ(sym, vOpen[j], vHigh[j], vLow[j], vClose[j]);
            m_Logger->Write(Logger::INFO,"TechIndUpdater: Warmup GKYZ with supplementary day bar data: Sym=%s %d OHLC = %f %f %f %f", sym.c_str(), vYMD[j].ToInt(), vOpen[j], vHigh[j], vLow[j], vClose[j]);
          }
        }
      }
      m_WarmedUp = true;
    }
    //--------------------------------------------------


    //--------------------------------------------------
    // New day
    //--------------------------------------------------
    if (m_ymdhms_SysTimeHKT - m_ymdhms_LastMonitorTime > 300)
    {
      bool bHKIdxNewDay = false;

      for (set<string>::iterator it = m_sAllSym.begin(); it != m_sAllSym.end(); ++it)
      {
        const string & sSym = *it;
        if (m_Exchg->IsNewDay(sSym, m_ymdhms_SysTimeHKT, m_ymdhms_LastMonitorTime))
        {
          m_TechInd->Reset1MinAvgPrice(sSym);
          m_TechInd->ResetDayOHLC(sSym);

          //--------------------------------------------------
          // HSI HHI D1 Bar from supplementary data
          //--------------------------------------------------
          if (m_sHKIdx.find(sSym) != m_sHKIdx.end())
          {
            vector<YYYYMMDD> vYMD;
            vector<double>   vOpen;
            vector<double>   vHigh;
            vector<double>   vLow;
            vector<double>   vClose;
            vector<long>     vVol;

            m_Logger->Write(Logger::INFO,"TechIndUpdater: GetSuppD1BarOHLCVInDateRange before %s %d", __FILE__, __LINE__);
            m_MarketData->GetSuppD1BarOHLCVInDateRange(sSym, m_ymdhms_LastMonitorTime.GetYYYYMMDD(), m_ymdhms_LastMonitorTime.GetYYYYMMDD(), vYMD, vOpen, vHigh, vLow, vClose, vVol);
            m_Logger->Write(Logger::INFO,"TechIndUpdater: GetSuppD1BarOHLCVInDateRange after %s %d", __FILE__, __LINE__);
            for (unsigned int j = 0; j < vYMD.size(); ++j)
            {
              if (vYMD[j] != m_ymdhms_SysTimeHKT.GetYYYYMMDD())
              {
                m_TechInd->AddBarGKYZ(sSym, vOpen[j], vHigh[j], vLow[j], vClose[j]);
                m_Logger->Write(Logger::INFO,"TechIndUpdater: Updated GKYZ with supplementary day bar data: Sym = %s %d OHLC = %f %f %f %f", sSym.c_str(), vYMD[j].ToInt(), vOpen[j], vHigh[j], vLow[j], vClose[j]);
              }
            }

            bHKIdxNewDay = true;
          }
        }
      }

      // if (bHKIdxNewDay)
      // {
      //   //--------------------------------------------------
      //   // Investigate the intraday seasonality pattern in HK
      //   //--------------------------------------------------
      //   m_HKItrdSea->AnalyzeIntrdVolatility();
      //   m_HKItrdSea->InitAndResetHistHKItrdSeasonalBucketData();
      //   //--------------------------------------------------
      // }

      m_ymdhms_LastMonitorTime = m_ymdhms_SysTimeHKT;
    }

    //--------------------------------------------------
    set<string> setUpdatedSymbols;
    m_MarketData->GetUpdatedSymbols(setUpdatedSymbols);
    //--------------------------------------------------



    //--------------------------------------------------
    for (set<string>::iterator itupdsym = setUpdatedSymbols.begin(); itupdsym != setUpdatedSymbols.end(); ++itupdsym)
    {

      const string & sUpdatedSym = (*itupdsym);

      //--------------------------------------------------
      // If outside trading hour, don't process
      //--------------------------------------------------
      if (!m_Exchg->IsTradingHour(sUpdatedSym,m_ymdhms_SysTimeHKT))
      {
        m_Logger->Write(Logger::DEBUG,"TechIndUpdater: %s %s Discarded data because it's outside its trading hour.", sUpdatedSym.c_str(), m_ymdhms_SysTimeHKT.ToStr().c_str());
        continue;
      }


      // //--------------------------------------------------
      // // HKIntradaySeasonality
      // //--------------------------------------------------
      // if (m_Exchg->GetPrimaryExchange(sUpdatedSym) == EX_HKFE)
      // {
      //   double dMQ = 0;
      //   if (m_MarketData->GetLatestMidQuote(sUpdatedSym,dMQ))
      //   {
      //     m_HKItrdSea->UpdateHistHKItrdSeasonalBucketData(
      //       sUpdatedSym,
      //       m_ymdhms_SysTimeHKT.GetHHMMSS(),
      //       dMQ);
      //   }
      // }
      // //--------------------------------------------------

      if (m_SysCfg->IsStrategyOn(STY_NIR))
      {
        //--------------------------------------------------
        // HK Stocks - HSI constitutents
        //--------------------------------------------------
        map<string,double> * p_mHSIConstituents = m_HKSE->GetHSIConstituents(m_ymdhms_SysTimeHKT.GetYYYYMMDD());
        for (map<string,double>::iterator it = (*p_mHSIConstituents).begin(); it != (*p_mHSIConstituents).end(); ++it)
        {
          if (!m_MarketData->Get1DayOHLCBar(it->first,m_temp_yyyymmdd,m_temp_hhmmss,m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume)) continue;

          m_Logger->Write(Logger::DEBUG,"TechIndUpdater: Stock price [%s] =\t%d\t%d\t%f\t%f\t%f\t%f\t%d.", it->first.c_str(), m_temp_yyyymmdd.ToInt(), m_temp_hhmmss.ToInt(), m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume);

          // //--------------------------------------------------
          // // FSMC state
          // //--------------------------------------------------
          // double dVHSI = m_VolSurfaces->GetOfficialVHSI();
          // if (dVHSI > 0)
          // {
          //   m_TechInd->AddFSMC(it->first,m_temp_high,m_temp_low,m_temp_close,dVHSI);
          //   int iFSMCState = m_TechInd->GetFSMCState(it->first);
          //   if (iFSMCState > 0)
          //     m_Logger->Write(Logger::DEBUG,"TechIndUpdater: Stock [%s]\t%d\tFSMC state = %d.", it->first.c_str(), m_temp_yyyymmdd.ToInt(), iFSMCState);
          // }

          //--------------------------------------------------
          // GKYZ
          //--------------------------------------------------
          m_TechInd->AddBarGKYZ(it->first,m_temp_open,m_temp_high,m_temp_low,m_temp_close);

        }
      }



      //--------------------------------------------------
      // HK Continuous Futures
      //--------------------------------------------------
      if (m_sHKc1Sym.find(sUpdatedSym) != m_sHKc1Sym.end())
      {

        double dMQ = 0;
        //--------------------------------------------------
        // HSIc1 HHIc1 weighted mid quote
        //--------------------------------------------------
        if (m_MarketData->GetLatestMidQuote(sUpdatedSym,dMQ))
        {
          //--------------------------------------------------
          // S11A
          //--------------------------------------------------
          if (m_SysCfg->IsStrategyOn(STY_S11A))
          {
            for (unsigned int j = 0; j < m_S11A_T1GBMDeltaTPrds.size(); ++j)
            {
              m_TechInd->AddTickReturn(sUpdatedSym,boost::lexical_cast<int>(m_S11A_T1GBMDeltaTPrds[j]),m_MarketData->GetSystemTimeHKT(),dMQ);
            }
            m_TechInd->AddTickReturn(sUpdatedSym,boost::lexical_cast<int>(m_S11A_T1GumbelCauchyDeltaTPrds),m_MarketData->GetSystemTimeHKT(),dMQ);
            m_TechInd->AddTickReturn(sUpdatedSym,boost::lexical_cast<int>(m_S11A_T1GumbelGaussDeltaTPrds),m_MarketData->GetSystemTimeHKT(),dMQ);
          }
        }

        //--------------------------------------------------
        // we prefer the last trade price, but take the mid quote if this is not available
        //--------------------------------------------------
        double dPrice = 0;

        bool bTPorMQ = (
          m_MarketData->GetLatestTradePrice(sUpdatedSym, dPrice) ||
          m_MarketData->GetLatestMidQuote(sUpdatedSym, dPrice)
          );

        if (bTPorMQ)
        {
          //--------------------------------------------------
          // B1_HKF
          // B2_HK
          //--------------------------------------------------
          if (
            m_SysCfg->IsStrategyOn(STY_B1_HKF) ||
            m_SysCfg->IsStrategyOn(STY_B2_HK)
            )
          {
            m_TechInd->AddDayOHLC(sUpdatedSym,dPrice);

            if (m_PTask_PrintDayHighLow.CheckIfItIsTimeToWakeUp(m_PTask_PrintDayHighLowToken[sUpdatedSym],m_ymdhms_SysTimeHKT))
            {
              m_Logger->Write(Logger::INFO,"TechIndUpdater: %s %s TradedPrice / MidQuote = %f DayHigh = %f DayLow = %f",
                              m_ymdhms_SysTimeHKT.ToStr().c_str(), sUpdatedSym.c_str(), dPrice, m_TechInd->GetDayHigh(sUpdatedSym), m_TechInd->GetDayLow(sUpdatedSym));
            }
          }
        }


        //--------------------------------------------------
        // HSIc1 HHIc1 M1 Bar
        //--------------------------------------------------
        if (m_MarketData->Get1MinOHLCBar(sUpdatedSym,m_temp_yyyymmdd,m_temp_hhmmss,m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume))
        {
          m_Logger->Write(Logger::INFO,"TechIndUpdater: 1-Min Bar: %s Continuous Futures =\t%d\t%d\t%f\t%f\t%f\t%f\t%d.", sUpdatedSym.c_str(), m_temp_yyyymmdd.ToInt(), m_temp_hhmmss.ToInt(), m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume);

          double dAvg = (m_temp_open+m_temp_high+m_temp_low+m_temp_close)/4;
          //--------------------------------------------------
          // B2_HK
          //--------------------------------------------------
          if (m_SysCfg->IsStrategyOn(STY_B2_HK))
          {
            m_TechInd->Add1MinAvgPrice(sUpdatedSym,dAvg);
          }
          //--------------------------------------------------
          // S11A
          //--------------------------------------------------
          double dHSIPrice = 0;
          if (m_SysCfg->IsStrategyOn(STY_S11A) && sUpdatedSym == HHI_CONT_FUT_1 && m_MarketData->GetLatestMidQuote(HSI_CONT_FUT_1,dHSIPrice))
          {
            for (vector<long>::iterator it = m_S11A_T2MAPeriods.begin(); it != m_S11A_T2MAPeriods.end(); ++it)
            {
              m_TechInd->AddlnHSIHHIRatioSma(boost::lexical_cast<long>(*it),log(dHSIPrice/m_temp_close));
              m_TechInd->AddlnHSIHHIRatioEma(boost::lexical_cast<long>(*it),log(dHSIPrice/m_temp_close));
            }
          }
        }
        //--------------------------------------------------

        //--------------------------------------------------
        // HSIc1 HHIc1 D1 bar
        //--------------------------------------------------
        if (m_MarketData->Get1DayOHLCBar(sUpdatedSym,m_temp_yyyymmdd,m_temp_hhmmss,m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume))
        {
          m_Logger->Write(Logger::INFO,"TechIndUpdater: 1-Day Bar: %s Continuous Futures =\t%d\t%d\t%f\t%f\t%f\t%f\t%d.", sUpdatedSym.c_str(), m_temp_yyyymmdd.ToInt(), m_temp_hhmmss.ToInt(), m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume);
          m_TechInd->AddBarGKYZ(sUpdatedSym,m_temp_open,m_temp_high,m_temp_low,m_temp_close);
        }
      }

      //--------------------------------------------------
      // HK Indices
      //--------------------------------------------------
      if (m_sHKIdx.find(sUpdatedSym) != m_sHKIdx.end())
      {
        //--------------------------------------------------
        // HSI HHI M1 Bar
        //--------------------------------------------------
        if (m_MarketData->Get1MinOHLCBar(sUpdatedSym,m_temp_yyyymmdd,m_temp_hhmmss,m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume))
        {
          m_Logger->Write(Logger::INFO,"TechIndUpdater: 1-Min Bar: %s Index =\t%d\t%d\t%f\t%f\t%f\t%f\t%d.", sUpdatedSym.c_str(), m_temp_yyyymmdd.ToInt(), m_temp_hhmmss.ToInt(), m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume);

          double dAvg = (m_temp_open+m_temp_high+m_temp_low+m_temp_close)/4;
          //--------------------------------------------------
          // B2_HK
          //--------------------------------------------------
          if (m_SysCfg->IsStrategyOn(STY_B2_HK))
          {
            m_TechInd->Add1MinAvgPrice(sUpdatedSym,m_temp_close);
          }
        }

        double dMQ = 0;
        if (m_MarketData->GetLatestMidQuote(sUpdatedSym,dMQ))
        {
          //--------------------------------------------------
          // B1_HKF
          //--------------------------------------------------
          if (m_SysCfg->IsStrategyOn(STY_B1_HKF))
          {
            m_TechInd->AddDayOHLC(sUpdatedSym,dMQ);

            if (m_PTask_PrintDayHighLow.CheckIfItIsTimeToWakeUp(m_PTask_PrintDayHighLowToken[sUpdatedSym],m_ymdhms_SysTimeHKT))
            {
              m_Logger->Write(Logger::INFO,"TechIndUpdater: %s %s MidQuote = %f DayHigh = %f DayLow = %f",
                              m_ymdhms_SysTimeHKT.ToStr().c_str(), sUpdatedSym.c_str(), dMQ, m_TechInd->GetDayHigh(sUpdatedSym), m_TechInd->GetDayLow(sUpdatedSym));
            }
          }
        }

      }
      //--------------------------------------------------


      //--------------------------------------------------
      // Other Symbols
      //--------------------------------------------------
      if (m_sOtherSym.find(sUpdatedSym) != m_sOtherSym.end())
      {

        //--------------------------------------------------
        // we prefer the last trade price, but take the mid quote if this is not available
        //--------------------------------------------------
        double dPrice = 0;

        bool bTPorMQ = (
          m_MarketData->GetLatestTradePrice(sUpdatedSym, dPrice) ||
          m_MarketData->GetLatestMidQuote(sUpdatedSym, dPrice)
          );

        if (bTPorMQ)
        {
          //--------------------------------------------------
          // B2_US1
          // B2_US2
          // B2_US3
          // B2_HK
          // B3_US
          //--------------------------------------------------
          if (
            m_SysCfg->IsStrategyOn(STY_B3_US)  ||
            m_SysCfg->IsStrategyOn(STY_B2_US1) ||
            m_SysCfg->IsStrategyOn(STY_B2_US2) ||
            m_SysCfg->IsStrategyOn(STY_B2_US3) ||
            m_SysCfg->IsStrategyOn(STY_B2_HK)
            )
          {
            m_TechInd->AddDayOHLC(sUpdatedSym,dPrice);

            if (m_PTask_PrintDayHighLow.CheckIfItIsTimeToWakeUp(m_PTask_PrintDayHighLowToken[sUpdatedSym],m_ymdhms_SysTimeHKT))
            {
              m_Logger->Write(Logger::INFO,"TechIndUpdater: %s %s TradedPrice / MidQuote = %f DayHigh = %f DayLow = %f",
                              m_ymdhms_SysTimeHKT.ToStr().c_str(), sUpdatedSym.c_str(), dPrice, m_TechInd->GetDayHigh(sUpdatedSym), m_TechInd->GetDayLow(sUpdatedSym));
            }
          }
        }

        //--------------------------------------------------
        // M1 Bar
        //--------------------------------------------------
        if (m_MarketData->Get1MinOHLCBar(sUpdatedSym,m_temp_yyyymmdd,m_temp_hhmmss,m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume))
        {
          m_Logger->Write(Logger::INFO,"TechIndUpdater: 1-Min Bar: %s =\t%d\t%d\t%f\t%f\t%f\t%f\t%d.", sUpdatedSym.c_str(), m_temp_yyyymmdd.ToInt(), m_temp_hhmmss.ToInt(), m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume);
        }
        //--------------------------------------------------

        //--------------------------------------------------
        // D1 bar
        //--------------------------------------------------
        if (m_MarketData->Get1DayOHLCBar(sUpdatedSym,m_temp_yyyymmdd,m_temp_hhmmss,m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume))
        {
          m_Logger->Write(Logger::INFO,"TechIndUpdater: 1-Day Bar: %s =\t%d\t%d\t%f\t%f\t%f\t%f\t%d.", sUpdatedSym.c_str(), m_temp_yyyymmdd.ToInt(), m_temp_hhmmss.ToInt(), m_temp_open,m_temp_high,m_temp_low,m_temp_close,m_temp_volume);
          m_TechInd->AddBarGKYZ(sUpdatedSym,m_temp_open,m_temp_high,m_temp_low,m_temp_close);
        }
      }
      //--------------------------------------------------

    }

    ReportAckIfNeeded();
    m_TechInd->NotifyConsumers();
  }

  ReportAckIfNeeded();
  m_TechInd->NotifyConsumers();

  m_Logger->Write(Logger::NOTICE,"TechIndUpdater has ended.");
  sleep(2);

  return;
}

void TechIndUpdater::ReportAckIfNeeded()
{
  if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK || m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
    m_MDIAck->ReportAckTechIndUpd();
  return;
}

