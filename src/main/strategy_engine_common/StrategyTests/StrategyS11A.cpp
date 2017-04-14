#include <StrategyS11A.h>

StrategyS11A::StrategyS11A() : m_bFirstTrade(false)
{
  m_MarketData      = MarketData::Instance();
  m_SystemState     = SystemState::Instance();
  m_SysCfg          = SystemConfig::Instance();
  m_Logger          = Logger::Instance();
  m_TechInd         = TechIndicators::Instance();
  m_PortAndOrders   = PortfoliosAndOrders::Instance();
  m_HKFE            = HKFE::Instance();
  m_MDIAck          = MDI_Acknowledgement::Instance();

  m_On_S11A_T1GBM          = m_SysCfg->Get_OnOff_S11A_T1GBM         ();
  m_On_S11A_T1GUMBELCAUCHY = m_SysCfg->Get_OnOff_S11A_T1GUMBELCAUCHY();
  m_On_S11A_T1GUMBELGAUSS  = m_SysCfg->Get_OnOff_S11A_T1GUMBELGAUSS ();
  m_On_S11A_T2GBM          = m_SysCfg->Get_OnOff_S11A_T2GBM         ();
  m_On_S11A_T2PCTL         = m_SysCfg->Get_OnOff_S11A_T2PCTL        ();
  m_On_S11A_T2COPU         = m_SysCfg->Get_OnOff_S11A_T2COPU        ();

  //--------------------------------------------------
  // Read param file Hist SD Correl
  //--------------------------------------------------
  {
    m_ParamFileHistSDCorrel = m_SysCfg->Get_S11AParamFileHistSDCorrel();
    deque<string> dqHistSDCorrel;
    STool::ReadFile(m_ParamFileHistSDCorrel,dqHistSDCorrel);
    if (dqHistSDCorrel.empty()) return;

    if (dqHistSDCorrel.size() > 0)
    {

      //--------------------------------------------------
      // Excluding header
      //--------------------------------------------------
      for (unsigned int i = 1; i < dqHistSDCorrel.size(); ++i)
      {
        vector<string> vs;
        boost::split(vs, dqHistSDCorrel[i], boost::is_any_of(","));

        string sDate         = vs[0];
        double dHSIHistSD    = boost::lexical_cast<double>(vs[1]);
        double dHHIHistSD    = boost::lexical_cast<double>(vs[2]);
        double dHSIHHICorrel = boost::lexical_cast<double>(vs[3]);
        // double dT2_05Pctl = boost::lexical_cast<double>(vs[4]);
        // double dT2_50Pctl = boost::lexical_cast<double>(vs[5]);
        // double dT2_95Pctl = boost::lexical_cast<double>(vs[6]);

        YYYYMMDD ymd;
        ymd.Set(vs[0]);
        m_map_HSI_HistSD       [ymd.ToInt()] = dHSIHistSD;
        m_map_HHI_HistSD       [ymd.ToInt()] = dHHIHistSD;
        m_map_HSI_HHI_Correl   [ymd.ToInt()] = dHSIHHICorrel;
        // m_map_T2_05Pctl     [ymd.ToInt()] = dT2_05Pctl;
        // m_map_T2_50Pctl     [ymd.ToInt()] = dT2_50Pctl;
        // m_map_T2_95Pctl     [ymd.ToInt()] = dT2_95Pctl;

      }

    }
  }
  //--------------------------------------------------

  //--------------------------------------------------
  // Read param file HSI HHI IV
  //--------------------------------------------------
  {
    m_ParamFileHSIHHIIV = m_SysCfg->Get_S11AParamFileHSIHHIIV();
    deque<string> dqHSIHHIIV;
    STool::ReadFile(m_ParamFileHSIHHIIV,dqHSIHHIIV);
    if (dqHSIHHIIV.empty()) return;

    if (dqHSIHHIIV.size() > 0)
    {

      //--------------------------------------------------
      // Excluding header
      //--------------------------------------------------
      for (unsigned int i = 1; i < dqHSIHHIIV.size(); ++i)
      {
        vector<string> vs;
        boost::split(vs, dqHSIHHIIV[i], boost::is_any_of(","));

        string sDate         = vs[0];
        double dHSIIV        = boost::lexical_cast<double>(vs[1]);
        double dHHIIV        = boost::lexical_cast<double>(vs[2]);

        YYYYMMDD ymd;
        ymd.Set(vs[0]);
        m_map_HSI_IV        [ymd.ToInt()] = dHSIIV;
        m_map_HHI_IV        [ymd.ToInt()] = dHHIIV;

      }

    }
  }
  //--------------------------------------------------

  //--------------------------------------------------
  // Read param file T1GumbelCauchy
  //--------------------------------------------------
  {
    m_ParamFileT1GumbelCauchy = m_SysCfg->Get_S11AParamFileT1GumbelCauchy();
    deque<string> dqT1GumbelCauchy;
    STool::ReadFile(m_ParamFileT1GumbelCauchy,dqT1GumbelCauchy);
    if (dqT1GumbelCauchy.empty()) return;

    if (dqT1GumbelCauchy.size() > 0)
    {

      //--------------------------------------------------
      // Excluding header
      //--------------------------------------------------
      for (unsigned int i = 1; i < dqT1GumbelCauchy.size(); ++i)
      {
        vector<string> vs;
        boost::split(vs, dqT1GumbelCauchy[i], boost::is_any_of(","));

        string sDate               = vs[0];
        double dGumbelCauchyTheta  = boost::lexical_cast<double>(vs[1]);
        double dHSI_CauchyLocation = boost::lexical_cast<double>(vs[2]);
        double dHSI_CauchyScale    = boost::lexical_cast<double>(vs[3]);
        double dHHI_CauchyLocation = boost::lexical_cast<double>(vs[4]);
        double dHHI_CauchyScale    = boost::lexical_cast<double>(vs[5]);

        YYYYMMDD ymd;
        ymd.Set(vs[0]);

        m_map_T1_GumbelCauchyTheta  [ymd] = dGumbelCauchyTheta ;
        m_map_T1_HSI_CauchyLocation [ymd] = dHSI_CauchyLocation;
        m_map_T1_HSI_CauchyScale    [ymd] = dHSI_CauchyScale   ;
        m_map_T1_HHI_CauchyLocation [ymd] = dHHI_CauchyLocation;
        m_map_T1_HHI_CauchyScale    [ymd] = dHHI_CauchyScale   ;
      }
    }
  }

  //--------------------------------------------------
  // Read param file T1GumbelGauss
  //--------------------------------------------------
  {
    m_ParamFileT1GumbelGauss = m_SysCfg->Get_S11AParamFileT1GumbelGauss();
    deque<string> dqT1GumbelGauss;
    STool::ReadFile(m_ParamFileT1GumbelGauss,dqT1GumbelGauss);
    if (dqT1GumbelGauss.empty()) return;

    if (dqT1GumbelGauss.size() > 0)
    {

      //--------------------------------------------------
      // Excluding header
      //--------------------------------------------------
      for (unsigned int i = 1; i < dqT1GumbelGauss.size(); ++i)
      {
        vector<string> vs;
        boost::split(vs, dqT1GumbelGauss[i], boost::is_any_of(","));

        string sDate               = vs[0];
        double dGumbelGaussTheta   = boost::lexical_cast<double>(vs[1]);
        double dHSI_LnRtnGaussMean = boost::lexical_cast<double>(vs[2]);
        double dHSI_LnRtnGaussSD   = boost::lexical_cast<double>(vs[3]);
        double dHHI_LnRtnGaussMean = boost::lexical_cast<double>(vs[4]);
        double dHHI_LnRtnGaussSD   = boost::lexical_cast<double>(vs[5]);

        YYYYMMDD ymd;
        ymd.Set(vs[0]);

        m_map_T1_GumbelGaussTheta   [ymd] = dGumbelGaussTheta  ;
        m_map_T1_HSI_LnRtnGaussMean [ymd] = dHSI_LnRtnGaussMean;
        m_map_T1_HSI_LnRtnGaussSD   [ymd] = dHSI_LnRtnGaussSD  ;
        m_map_T1_HHI_LnRtnGaussMean [ymd] = dHHI_LnRtnGaussMean;
        m_map_T1_HHI_LnRtnGaussSD   [ymd] = dHHI_LnRtnGaussSD  ;
      }
    }
  }

}

StrategyS11A::~StrategyS11A()
{
}



string StrategyS11A::GetSubSystemName(const S11A_SubSystem s)
{
  switch(s)
  {
    case     S11A_T1GBM           : return "S11A_T1GBM";
    case     S11A_T1GUMBELCAUCHY  : return "S11A_T1GUMBELCAUCHY";
    case     S11A_T1GUMBELGAUSS   : return "S11A_T1GUMBELGAUSS";
    case     S11A_T2GBM           : return "S11A_T2GBM";
    case     S11A_T2PCTL          : return "S11A_T2PCTL";
    case     S11A_T2COPU          : return "S11A_T2COPU";
    default                       : return "UNKNOWN_S11A_SUBSYSTEM";
  }
}

// void StrategyS11A::Run()
// {
//
//   //--------------------------------------------------
//   // Init path
//   //--------------------------------------------------
//   ofstream fsSignalLog(m_SysCfg->GetSignalLogPath(STY_S11A).c_str());
//
//   InitFromSysConfig();
//
//   m_ymdhms_LastMonitorTime = m_MarketData->GetSystemTimeHKT();
//   //--------------------------------------------------
//
//   //--------------------------------------------------
//   // m_PortAndOrders->SwitchMode(PortfoliosAndOrders::SINGLE_ORDER);
//   int iLastSysYYYYMMDD = -1;
//   //--------------------------------------------------
//
//
//   for (;;)
//   {
//     m_TechInd->WaitForData();
//
//     if (m_SystemState->ChkIfThreadShouldStop()) break;
//
//     ymdhms_SysTime = m_MarketData->GetSystemTimeHKT();
//
//     if (!m_SysCfg->IsStrategyOn(STY_S11A))
//     {
//       ReportAckIfNeeded();
//       m_Logger->Write(Logger::DEBUG,"StrategyS11A off");
//       continue;
//     }
//
//     if (!m_ymdhms_LastMonitorTime.IsValid())
//     {
//       m_ymdhms_LastMonitorTime = m_MarketData->GetSystemTimeHKT();
//     }
//
//     //--------------------------------------------------
//     // Here comes a new day!
//     //--------------------------------------------------
//     if (ymdhms_SysTime.GetYYYYMMDD().ToInt() != iLastSysYYYYMMDD)
//     {
//       iLastSysYYYYMMDD = ymdhms_SysTime.GetYYYYMMDD().ToInt();
//       sHSI_Fut_DTM_GT_1  = UNDERLYING_HSI+m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,ymdhms_SysTime.GetYYYYMMDD(),1 );
//       sHHI_Fut_DTM_GT_1  = UNDERLYING_HHI+m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,ymdhms_SysTime.GetYYYYMMDD(),1 );
//       sMHI_Fut_DTM_GT_1  = UNDERLYING_MHI+m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,ymdhms_SysTime.GetYYYYMMDD(),1 );
//       sMCH_Fut_DTM_GT_1  = UNDERLYING_MCH+m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,ymdhms_SysTime.GetYYYYMMDD(),1 );
//       sHSI_Fut_DTM_GT_10 = UNDERLYING_HSI+m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,ymdhms_SysTime.GetYYYYMMDD(),10);
//       sHHI_Fut_DTM_GT_10 = UNDERLYING_HHI+m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,ymdhms_SysTime.GetYYYYMMDD(),10);
//       sMHI_Fut_DTM_GT_10 = UNDERLYING_MHI+m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,ymdhms_SysTime.GetYYYYMMDD(),10);
//       sMCH_Fut_DTM_GT_10 = UNDERLYING_MCH+m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,ymdhms_SysTime.GetYYYYMMDD(),10);
//       {
//         ostringstream oo;
//         oo << sHSI_Fut_DTM_GT_1.at(3) << ymdhms_SysTime.GetYYYYMMDD().GetDecade() << sHSI_Fut_DTM_GT_1.at(4); //constructing format e.g F15
//         ymdExpDate_DTM_GT_1 = m_HKFE->GetExpyFmtMxx(HKFE::FUTURES, oo.str());
//       }
//       {
//         ostringstream oo;
//         oo << sHSI_Fut_DTM_GT_10.at(3) << ymdhms_SysTime.GetYYYYMMDD().GetDecade() << sHSI_Fut_DTM_GT_10.at(4); //constructing format e.g F15
//         ymdExpDate_DTM_GT_10 = m_HKFE->GetExpyFmtMxx(HKFE::FUTURES, oo.str());
//       }
//       GetSDCorrel();
//     }
//
//     if (!GetLegPrices())
//     {
//       ReportAckIfNeeded();
//       continue;
//     }
//
//     int iPeriod = 0;
//     vector<long>::iterator it_T1GBM_DeltaTPeriods = m_T1GBM_DeltaTPeriods.begin();
//     vector<long>::iterator it_T2GBM_DeltaTPeriods = m_T2GBM_DeltaTPeriods.begin();
//     S11A_SubSystem SubSys = S11A_T1GBM;
//
//     for (;;)
//     {
//
//       if (SubSys == S11A_T1GBM)
//       {
//         if (it_T1GBM_DeltaTPeriods != m_T1GBM_DeltaTPeriods.end())
//         {
//           iPeriod = boost::lexical_cast<int>(*it_T1GBM_DeltaTPeriods);
//           it_T1GBM_DeltaTPeriods++;
//         }
//         else
//         {
//           SubSys = S11A_T2GBM;
//           iPeriod = (*it_T2GBM_DeltaTPeriods);
//           it_T2GBM_DeltaTPeriods++;
//         }
//       }
//       else if (SubSys == S11A_T2GBM)
//       {
//         if (it_T2GBM_DeltaTPeriods != m_T2GBM_DeltaTPeriods.end())
//         {
//           iPeriod = boost::lexical_cast<int>(*it_T2GBM_DeltaTPeriods);
//           it_T2GBM_DeltaTPeriods++;
//         }
//         else
//         {
//           SubSys = S11A_T2PCTL;
//           iPeriod = dT2PctlPeriod;
//         }
//       }
//       else if (SubSys == S11A_T2PCTL)
//       {
//         SubSys = S11A_T1GUMBELCAUCHY;
//         iPeriod = dT1GumbelCauchyPeriod;
//       }
//       else if (SubSys == S11A_T1GUMBELCAUCHY)
//       {
//         SubSys = S11A_T1GUMBELGAUSS;
//         iPeriod = dT1GumbelGaussPeriod;
//       }
//       else if (SubSys == S11A_T1GUMBELGAUSS)
//       {
//         SubSys = S11A_T2COPU;
//         iPeriod = dT2CopuPeriod;
//       }
//       else if (SubSys == S11A_T2COPU)
//       {
//         break;
//       }
//
//       CalcNumCtrt();
//
//
//       if (SubSys == S11A_T1GBM || SubSys == S11A_T1GUMBELCAUCHY || SubSys == S11A_T1GUMBELGAUSS)
//       {
//         sHSI_Fut = sHSI_Fut_DTM_GT_1;
//         sHHI_Fut = sHHI_Fut_DTM_GT_1;
//         sMHI_Fut = sMHI_Fut_DTM_GT_1;
//         sMCH_Fut = sMCH_Fut_DTM_GT_1;
//         ymdExpDate = ymdExpDate_DTM_GT_1;
//
//         iNoOfCtrtHSI = iNoOfCtrtHSI_1;
//         iNoOfCtrtMHI = iNoOfCtrtMHI_1;
//         iNoOfCtrtHHI = iNoOfCtrtHHI_1;
//         iNoOfCtrtMCH = iNoOfCtrtMCH_1;
//
//         dDeltaT = (double)iPeriod/(6*60*60*252); // Period is in seconds!
//
//       }
//       else if (SubSys == S11A_T2GBM)
//       {
//         sHSI_Fut = sHSI_Fut_DTM_GT_1;
//         sHHI_Fut = sHHI_Fut_DTM_GT_1;
//         sMHI_Fut = sMHI_Fut_DTM_GT_1;
//         sMCH_Fut = sMCH_Fut_DTM_GT_1;
//         ymdExpDate = ymdExpDate_DTM_GT_1;
//
//         iNoOfCtrtHSI = iNoOfCtrtHSI_2;
//         iNoOfCtrtMHI = iNoOfCtrtMHI_2;
//         iNoOfCtrtHHI = iNoOfCtrtHHI_2;
//         iNoOfCtrtMCH = iNoOfCtrtMCH_2;
//
//         dDeltaT = (double)iPeriod/(6*60*252); // Period is in minutes!
//       }
//       else if (SubSys == S11A_T2PCTL)
//       {
//         sHSI_Fut = sHSI_Fut_DTM_GT_10;
//         sHHI_Fut = sHHI_Fut_DTM_GT_10;
//         sMHI_Fut = sMHI_Fut_DTM_GT_10;
//         sMCH_Fut = sMCH_Fut_DTM_GT_10;
//         ymdExpDate = ymdExpDate_DTM_GT_10;
//
//         iNoOfCtrtHSI = iNoOfCtrtHSI_2;
//         iNoOfCtrtMHI = iNoOfCtrtMHI_2;
//         iNoOfCtrtHHI = iNoOfCtrtHHI_2;
//         iNoOfCtrtMCH = iNoOfCtrtMCH_2;
//
//         dDeltaT  = (double)iPeriod/(6*60*252); // Period is in minutes!
//       }
//
//
//       CalcLnRtnGBMParam(iPeriod);
//       OutputStratParam(SubSys,iPeriod);
//       TRADEDIR iTradeDir = OpenTrade(SubSys,iPeriod);
//       CloseTrade(SubSys, iTradeDir,iPeriod);
//     }
//
//
//     TradeSynchronizedOrders();
//
//     ReportAckIfNeeded();
//   }
//
//   m_Logger->Write(Logger::NOTICE,"StrategyS11A has ended.");
//   sleep(2);
//
//   return;
// }

void StrategyS11A::ReportAckIfNeeded()
{
  // if (m_SysCfg->Get_TCPOrEmbeddedMode() == SystemConfig::TCPWITHACK || m_SysCfg->Get_TCPOrEmbeddedMode() == SystemConfig::EMBEDDED)
  //   m_MDIAck->ReportAck(STY_S11A);
  // return;
}


StrategyS11A::TRADEDIR StrategyS11A::OpenTrade(const S11A_SubSystem SubSys, const int iPeriod)
{
  //
  // //--------------------------------------------------
  // // S11A_T1GUMBELCAUCHY
  // // S11A_T1GUMBELGAUSS
  // //--------------------------------------------------
  // GumbelCopula gumbelCauchyCopula;
  // gumbelCauchyCopula.SetParam(
  //     m_map_T1_GumbelCauchyTheta[ymdhms_SysTime.GetYYYYMMDD()]
  //     +
  //     dT1GumbelCauchyThetaMeanReversion *
  //     (dT1GumbelCauchyThetaMean - m_map_T1_GumbelCauchyTheta[ymdhms_SysTime.GetYYYYMMDD()])
  //     );
  // GumbelCopula gumbelGaussCopula;
  // gumbelGaussCopula.SetParam(
  //     m_map_T1_GumbelGaussTheta[ymdhms_SysTime.GetYYYYMMDD()]
  //     +
  //     dT1GumbelGaussThetaMeanReversion *
  //     (dT1GumbelGaussThetaMean - m_map_T1_GumbelGaussTheta[ymdhms_SysTime.GetYYYYMMDD()])
  //     );
  //
  // CauchyDistribution hsiCauchyDist;
  // hsiCauchyDist.SetParam(
  //     m_map_T1_HSI_CauchyLocation[ymdhms_SysTime.GetYYYYMMDD()],
  //     m_map_T1_HSI_CauchyScale[ymdhms_SysTime.GetYYYYMMDD()]
  //     );
  // CauchyDistribution hhiCauchyDist;
  // hhiCauchyDist.SetParam(
  //     m_map_T1_HHI_CauchyLocation[ymdhms_SysTime.GetYYYYMMDD()],
  //     m_map_T1_HHI_CauchyScale[ymdhms_SysTime.GetYYYYMMDD()]
  //     );
  //
  // GaussianDistribution hsiLnRtnGaussDist;
  // hsiLnRtnGaussDist.SetParam(
  //     m_map_T1_HSI_LnRtnGaussMean[ymdhms_SysTime.GetYYYYMMDD()],
  //     // m_map_T1_HSI_LnRtnGaussSD[ymdhms_SysTime.GetYYYYMMDD()] * m_map_HSI_IV[ymdhms_SysTime.GetYYYYMMDD()] / m_map_HSI_HistSD[ymdhms_SysTime.GetYYYYMMDD()]
  //     m_map_HSI_IV[ymdhms_SysTime.GetYYYYMMDD()] / dT1GumbelGaussMarginalSDScaling
  //     );
  // GaussianDistribution hhiLnRtnGaussDist;
  // hhiLnRtnGaussDist.SetParam(
  //     m_map_T1_HHI_LnRtnGaussMean[ymdhms_SysTime.GetYYYYMMDD()],
  //     // m_map_T1_HHI_LnRtnGaussSD[ymdhms_SysTime.GetYYYYMMDD()] * m_map_HHI_IV[ymdhms_SysTime.GetYYYYMMDD()] / m_map_HHI_HistSD[ymdhms_SysTime.GetYYYYMMDD()]
  //     m_map_HHI_IV[ymdhms_SysTime.GetYYYYMMDD()] / dT1GumbelGaussMarginalSDScaling
  //     );
  // //--------------------------------------------------
  //
  //
  // //--------------------------------------------------
  // // Open Trade: Go through open trade logic anyway for Close Trade logic to make ref
  // //--------------------------------------------------
  // int iBasketID = -1;
  // TRADEDIR iTradeDir = NODIR;
  // string sOIDDetails;
  //
  // YMD ymdSysDate;
  // ymdhms_SysTime.GetYMD(ymdSysDate);
  //
  // HMS hmsMrngOpen ;
  // HMS hmsMrngClose;
  // HMS hmsAftnOpen ;
  // HMS hmsAftnClose;
  //
  // m_HKFE->GetMrngOpen (ymdSysDate, hmsMrngOpen );
  // m_HKFE->GetMrngClose(ymdSysDate, hmsMrngClose);
  // m_HKFE->GetAftnOpen (ymdSysDate, hmsAftnOpen );
  // m_HKFE->GetAftnClose(ymdSysDate, hmsAftnClose);
  //
  // HMS hmsT1MrngOpen (hmsMrngOpen ); hmsT1MrngOpen .AddSecond     ((iPeriod*max(dT1GumbelCauchyEOLPeriod,dT1GumbelGaussEOLPeriod)+(double)AVOID_TRADING_NEAR_MKTOPEN_SEC +EXTRABUFFER_OPENTRADE_SEC));
  // HMS hmsT1MrngClose(hmsMrngClose); hmsT1MrngClose.SubtractSecond(                                                              +(double)AVOID_TRADING_NEAR_MKTCLOSE_SEC+EXTRABUFFER_OPENTRADE_SEC);
  // HMS hmsT1AftnOpen (hmsAftnOpen ); hmsT1AftnOpen .AddSecond     (                                                              +(double)AVOID_TRADING_NEAR_MKTOPEN_SEC +EXTRABUFFER_OPENTRADE_SEC);
  // HMS hmsT1AftnClose(hmsAftnClose); hmsT1AftnClose.SubtractSecond((iPeriod*max(dT1GumbelCauchyEOLPeriod,dT1GumbelGaussEOLPeriod)+(double)AVOID_TRADING_NEAR_MKTCLOSE_SEC+EXTRABUFFER_OPENTRADE_SEC));
  //
  // hmsMrngOpen .AddSecond     (AVOID_TRADING_NEAR_MKTOPEN_SEC +EXTRABUFFER_OPENTRADE_SEC);
  // hmsMrngClose.SubtractSecond(AVOID_TRADING_NEAR_MKTCLOSE_SEC+EXTRABUFFER_OPENTRADE_SEC);
  // hmsAftnOpen .AddSecond     (AVOID_TRADING_NEAR_MKTOPEN_SEC +EXTRABUFFER_OPENTRADE_SEC);
  // hmsAftnClose.SubtractSecond(AVOID_TRADING_NEAR_MKTCLOSE_SEC+EXTRABUFFER_OPENTRADE_SEC);
  //
  // //--------------------------------------------------
  // // Open Trade
  // //--------------------------------------------------
  // if (
  //     (
  //      (SubSys == S11A_T1GBM || SubSys == S11A_T1GUMBELCAUCHY || SubSys == S11A_T1GUMBELGAUSS)
  //      &&
  //      ( 
  //       (ymdhms_SysTime.GetHHMMSS() >= hmsT1MrngOpen && ymdhms_SysTime.GetHHMMSS() < hmsT1MrngClose) ||
  //       (ymdhms_SysTime.GetHHMMSS() >= hmsT1AftnOpen && ymdhms_SysTime.GetHHMMSS() < hmsT1AftnClose)
  //      )
  //      &&
  //      !std::isnan(dHSILnRtn) && !std::isnan(dHHILnRtn)
  //     )
  //
  //     ||
  //
  //     (
  //      (SubSys == S11A_T2GBM || SubSys == S11A_T2PCTL)
  //      &&
  //      (
  //       (ymdhms_SysTime.GetHHMMSS() >= hmsMrngOpen && ymdhms_SysTime.GetHHMMSS() < hmsMrngClose) ||
  //       (ymdhms_SysTime.GetHHMMSS() >= hmsAftnOpen && ymdhms_SysTime.GetHHMMSS() < hmsAftnClose)
  //      )
  //      &&
  //      dLongerTermLnHSIHHIRatio > 0
  //     )
  //       )
  //       {
  //
  //         if (SubSys == S11A_T1GBM
  //             &&
  //             m_On_S11A_T1GBM
  //             &&
  //             dLnRtnDiff > dEDeltaGamma + dT1GBMTriggerThreshold * dSDDeltaGamma)
  //         {
  //           sOIDDetails = GetSubSystemName(SubSys) + "_P" + boost::lexical_cast<string>(iPeriod) + "_S";
  //           iTradeDir = SHORT;
  //         }
  //         else if (SubSys == S11A_T1GBM
  //             &&
  //             m_On_S11A_T1GBM
  //             &&
  //             dLnRtnDiff < dEDeltaGamma - dT1GBMTriggerThreshold * dSDDeltaGamma)
  //         {
  //           sOIDDetails = GetSubSystemName(SubSys) + "_P" + boost::lexical_cast<string>(iPeriod) + "_L";
  //           iTradeDir = LONG;
  //         }
  //         else if (SubSys == S11A_T1GUMBELCAUCHY
  //             &&
  //             m_On_S11A_T1GUMBELCAUCHY
  //             )
  //         {
  //           double u = hsiCauchyDist.F(dHSILnRtn);
  //           double v = hhiCauchyDist.F(dHHILnRtn);
  //           double p = gumbelCauchyCopula.Prob_U_smlr_u_V_eq_v(u,v);
  //
  //           m_Logger->Write(Logger::DEBUG,"StrategyS11A: SubSystem: %s. dHSILnRtn = %f dHHILnRtn = %f u = %f v = %f P(U<u|V=v) = %f",GetSubSystemName(SubSys).c_str(),dHSILnRtn,dHHILnRtn,u,v,p);
  //
  //           if (p > 1-dT1GumbelCauchyTriggerThreshold)
  //           {
  //             sOIDDetails = GetSubSystemName(SubSys) + "_P" + boost::lexical_cast<string>(iPeriod) + "_S";
  //             iTradeDir = SHORT;
  //           }
  //           else if (p < dT1GumbelCauchyTriggerThreshold)
  //           {
  //             sOIDDetails = GetSubSystemName(SubSys) + "_P" + boost::lexical_cast<string>(iPeriod) + "_L";
  //             iTradeDir = LONG;
  //           }
  //         }
  //         else if (SubSys == S11A_T1GUMBELGAUSS
  //             &&
  //             m_On_S11A_T1GUMBELGAUSS
  //             )
  //         {
  //           double u = hsiLnRtnGaussDist.F(-dHSILnRtn); // taking -ve to flip + / - around for Gumbel
  //           double v = hhiLnRtnGaussDist.F(-dHHILnRtn); // taking -ve to flip + / - around for Gumbel
  //           double p = gumbelGaussCopula.Prob_U_smlr_u_V_eq_v(u,v);
  //
  //           m_Logger->Write(Logger::DEBUG,"StrategyS11A: SubSystem: %s. dHSILnRtn = %f dHHILnRtn = %f u = %f v = %f P(U<u|V=v) = %f",GetSubSystemName(SubSys).c_str(),dHSILnRtn,dHHILnRtn,u,v,p);
  //
  //           if (p > 1-dT1GumbelGaussTriggerThreshold)
  //           {
  //             sOIDDetails = GetSubSystemName(SubSys) + "_P" + boost::lexical_cast<string>(iPeriod) + "_L";
  //             iTradeDir = LONG;
  //           }
  //           else if (p < dT1GumbelGaussTriggerThreshold)
  //           {
  //             sOIDDetails = GetSubSystemName(SubSys) + "_P" + boost::lexical_cast<string>(iPeriod) + "_S";
  //             iTradeDir = SHORT;
  //           }
  //
  //           // //--------------------------------------------------
  //           // if (dHSILnRtn * dHHILnRtn > 0 && abs(dHSILnRtn) < abs(dHHILnRtn))
  //           // {
  //           //   iNoOfCtrtHSI = floor((double)iNoOfCtrtHSI/2);
  //           //   iNoOfCtrtMHI = floor((double)iNoOfCtrtMHI/2);
  //           //   iNoOfCtrtHHI = iNoOfCtrtHHI;
  //           //   iNoOfCtrtMCH = iNoOfCtrtMCH;
  //           // }
  //           // else if (dHSILnRtn * dHHILnRtn > 0 && abs(dHSILnRtn) > abs(dHHILnRtn))
  //           // {
  //           //   iNoOfCtrtHSI = iNoOfCtrtHSI;
  //           //   iNoOfCtrtMHI = iNoOfCtrtMHI;
  //           //   iNoOfCtrtHHI = floor((double)iNoOfCtrtHHI/2);
  //           //   iNoOfCtrtMCH = floor((double)iNoOfCtrtMCH/2);
  //           // }
  //           // //--------------------------------------------------
  //         }
  //
  //         else if (
  //             SubSys == S11A_T2GBM
  //             &&
  //             m_On_S11A_T2GBM
  //             &&
  //             (dCurLnHSIHHIRatio > dLongerTermLnHSIHHIRatio + dEGamma + dT2GBMTriggerThreshold * dSDGamma)
  //             )
  //         {
  //           sOIDDetails = GetSubSystemName(SubSys) + "_P" + boost::lexical_cast<string>(iPeriod) + "_S";
  //           iTradeDir = SHORT;
  //         }
  //         else if (
  //             SubSys == S11A_T2GBM
  //             &&
  //             m_On_S11A_T2GBM
  //             &&
  //             (dCurLnHSIHHIRatio < dLongerTermLnHSIHHIRatio + dEGamma - dT2GBMTriggerThreshold * dSDGamma)
  //             )
  //         {
  //           sOIDDetails = GetSubSystemName(SubSys) + "_P" + boost::lexical_cast<string>(iPeriod) + "_L";
  //           iTradeDir = LONG;
  //         }
  //         // else if (
  //         //     SubSys == S11A_T2PCTL
  //         //     &&
  //         //     m_On_S11A_T2PCTL
  //         //     &&
  //         //     (dCurLnHSIHHIRatio - dLongerTermLnHSIHHIRatio < dT2_05Pctl)
  //         //     )
  //         // {
  //         //   sOIDDetails = GetSubSystemName(SubSys) + "_L";
  //         //   iTradeDir = LONG;
  //         // }
  //         // else if (
  //         //     SubSys == S11A_T2PCTL
  //         //     &&
  //         //     m_On_S11A_T2PCTL
  //         //     &&
  //         //     (dCurLnHSIHHIRatio - dLongerTermLnHSIHHIRatio > dT2_95Pctl)
  //         //     )
  //         // {
  //         //   sOIDDetails = GetSubSystemName(SubSys) + "_S";
  //         //   iTradeDir = SHORT;
  //         // }
  //
  //         m_Logger->Write(Logger::DEBUG,"StrategyS11A: SubSystem: %s %s TradeDir: %d",GetSubSystemName(SubSys).c_str(),sOIDDetails.c_str(),iTradeDir);
  //
  //         //--------------------------------------------------
  //         // Open Trade
  //         //--------------------------------------------------
  //         if (!m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].HasOpenPos(iPeriod) && iTradeDir != NODIR) m_bFirstTrade = true;
  //
  //         if (ymdhms_SysTime - m_ymdhms_LastMonitorTime < m_SysCfg->Get_S11AMonitoringIntervalInSec()) return iTradeDir;
  //
  //         //--------------------------------------------------
  //         // Limit the number of open trades
  //         //--------------------------------------------------
  //         if (SubSys == S11A_T1GUMBELGAUSS
  //             &&
  //             iTradeDir != NODIR
  //             &&
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].OpenPosCount(iPeriod) >= m_SysCfg->Get_S11AT1GumbelGaussSignalLimit())
  //         {
  //
  //           set<int> setAllBktID = m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetAllBasketIDInGroup(iPeriod);
  //
  //           map<HHMMSS,int> mapTrdTimeBktID;
  //           for (set<int>::iterator it_basketid = setAllBktID.begin(); it_basketid != setAllBktID.end(); ++it_basketid)
  //           {
  //             deque<double> dqRefVarb = m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetRefVariableForBasket(*it_basketid);
  //
  //             if (dqRefVarb[0] == iTradeDir)
  //             {
  //               YYYYMMDDHHMMSS ymdhms;
  //               m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetTradeTimeOfBasketID(*it_basketid,ymdhms);
  //               mapTrdTimeBktID[ymdhms.GetHHMMSS()] = *it_basketid;
  //             }
  //           }
  //
  //           if (!mapTrdTimeBktID.empty())
  //           {
  //             // m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RemoveBasketID(iPeriod,(mapTrdTimeBktID.begin())->second);
  //
  //             int iBasketID = (mapTrdTimeBktID.begin())->second;
  //             deque<double> dqRefVarb = m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetRefVariableForBasket(iBasketID);
  //             dqRefVarb.clear();
  //             dqRefVarb.push_back(iTradeDir);
  //             dqRefVarb.push_back(dHSIPrice);
  //             dqRefVarb.push_back(dHHIPrice);
  //             dqRefVarb.push_back(ymdExpDate.ToInt());
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].SetRefVariableForBasket(iBasketID,dqRefVarb);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].SetTradeTimeForBasket(iBasketID,ymdhms_SysTime);
  //
  //           }
  //
  //         }
  //         else
  //         {
  //           if (iTradeDir == LONG)
  //           {
  //             if (m_SysCfg->Get_S11AMonitoringIntervalInSec() <= 0)
  //             {
  //               if (iNoOfCtrtHSI != 0) m_PortAndOrders->Trade(static_cast<int>(STY_S11A),sHSI_Fut, iNoOfCtrtHSI,true,sOIDDetails);
  //               if (iNoOfCtrtMHI != 0) m_PortAndOrders->Trade(static_cast<int>(STY_S11A),sMHI_Fut, iNoOfCtrtMHI,true,sOIDDetails);
  //               if (iNoOfCtrtHHI != 0) m_PortAndOrders->Trade(static_cast<int>(STY_S11A),sHHI_Fut,-iNoOfCtrtHHI,true,sOIDDetails);
  //               if (iNoOfCtrtMCH != 0) m_PortAndOrders->Trade(static_cast<int>(STY_S11A),sMCH_Fut,-iNoOfCtrtMCH,true,sOIDDetails);
  //             }
  //             else
  //             {
  //               if (m_SynchronizedTrades.find(sHSI_Fut) == m_SynchronizedTrades.end()) m_SynchronizedTrades[sHSI_Fut] = 0;
  //               if (m_SynchronizedTrades.find(sMHI_Fut) == m_SynchronizedTrades.end()) m_SynchronizedTrades[sMHI_Fut] = 0;
  //               if (m_SynchronizedTrades.find(sHHI_Fut) == m_SynchronizedTrades.end()) m_SynchronizedTrades[sHHI_Fut] = 0;
  //               if (m_SynchronizedTrades.find(sMCH_Fut) == m_SynchronizedTrades.end()) m_SynchronizedTrades[sMCH_Fut] = 0;
  //
  //               m_SynchronizedTrades[sHSI_Fut] = m_SynchronizedTrades[sHSI_Fut] +  iNoOfCtrtHSI;
  //               m_SynchronizedTrades[sMHI_Fut] = m_SynchronizedTrades[sMHI_Fut] +  iNoOfCtrtMHI;
  //               m_SynchronizedTrades[sHHI_Fut] = m_SynchronizedTrades[sHHI_Fut] + -iNoOfCtrtHHI;
  //               m_SynchronizedTrades[sMCH_Fut] = m_SynchronizedTrades[sMCH_Fut] + -iNoOfCtrtMCH;
  //             }
  //
  //             iBasketID = m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetNewBasketID();
  //             deque<double> dqRefVarb;
  //             dqRefVarb.push_back((double)LONG);
  //             dqRefVarb.push_back(dHSIPrice);
  //             dqRefVarb.push_back(dHHIPrice);
  //             dqRefVarb.push_back(ymdExpDate.ToInt());
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].SetRefVariableForBasket(iBasketID,dqRefVarb);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].SetTradeTimeForBasket(iBasketID,ymdhms_SysTime);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RecordTrade(iPeriod,iBasketID,sHSI_Fut,0,iNoOfCtrtHSI);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RecordTrade(iPeriod,iBasketID,sMHI_Fut,0,iNoOfCtrtMHI);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RecordTrade(iPeriod,iBasketID,sHHI_Fut,0,-iNoOfCtrtHHI);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RecordTrade(iPeriod,iBasketID,sMCH_Fut,0,-iNoOfCtrtMCH);
  //
  //           }
  //           else if (iTradeDir == SHORT)
  //           {
  //             if (m_SysCfg->Get_S11AMonitoringIntervalInSec() <= 0)
  //             {
  //               if (iNoOfCtrtHSI != 0) m_PortAndOrders->Trade(static_cast<int>(STY_S11A),sHSI_Fut,-iNoOfCtrtHSI,true,sOIDDetails);
  //               if (iNoOfCtrtMHI != 0) m_PortAndOrders->Trade(static_cast<int>(STY_S11A),sMHI_Fut,-iNoOfCtrtMHI,true,sOIDDetails);
  //               if (iNoOfCtrtHHI != 0) m_PortAndOrders->Trade(static_cast<int>(STY_S11A),sHHI_Fut, iNoOfCtrtHHI,true,sOIDDetails);
  //               if (iNoOfCtrtMCH != 0) m_PortAndOrders->Trade(static_cast<int>(STY_S11A),sMCH_Fut, iNoOfCtrtMCH,true,sOIDDetails);
  //             }
  //             else
  //             {
  //               if (m_SynchronizedTrades.find(sHSI_Fut) == m_SynchronizedTrades.end()) m_SynchronizedTrades[sHSI_Fut] = 0;
  //               if (m_SynchronizedTrades.find(sMHI_Fut) == m_SynchronizedTrades.end()) m_SynchronizedTrades[sMHI_Fut] = 0;
  //               if (m_SynchronizedTrades.find(sHHI_Fut) == m_SynchronizedTrades.end()) m_SynchronizedTrades[sHHI_Fut] = 0;
  //               if (m_SynchronizedTrades.find(sMCH_Fut) == m_SynchronizedTrades.end()) m_SynchronizedTrades[sMCH_Fut] = 0;
  //
  //               m_SynchronizedTrades[sHSI_Fut] = m_SynchronizedTrades[sHSI_Fut] + -iNoOfCtrtHSI;
  //               m_SynchronizedTrades[sMHI_Fut] = m_SynchronizedTrades[sMHI_Fut] + -iNoOfCtrtMHI;
  //               m_SynchronizedTrades[sHHI_Fut] = m_SynchronizedTrades[sHHI_Fut] +  iNoOfCtrtHHI;
  //               m_SynchronizedTrades[sMCH_Fut] = m_SynchronizedTrades[sMCH_Fut] +  iNoOfCtrtMCH;
  //             }
  //
  //             iBasketID = m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetNewBasketID();
  //             deque<double> dqRefVarb;
  //             dqRefVarb.push_back((double)SHORT);
  //             dqRefVarb.push_back(dHSIPrice);
  //             dqRefVarb.push_back(dHHIPrice);
  //             dqRefVarb.push_back(ymdExpDate.ToInt());
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].SetRefVariableForBasket(iBasketID,dqRefVarb);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].SetTradeTimeForBasket(iBasketID,ymdhms_SysTime);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RecordTrade(iPeriod,iBasketID,sHSI_Fut,0,-iNoOfCtrtHSI);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RecordTrade(iPeriod,iBasketID,sMHI_Fut,0,-iNoOfCtrtMHI);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RecordTrade(iPeriod,iBasketID,sHHI_Fut,0,iNoOfCtrtHHI);
  //             m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RecordTrade(iPeriod,iBasketID,sMCH_Fut,0,iNoOfCtrtMCH);
  //
  //           } 
  //         }
  //
  //       }
  //
  // return iTradeDir;
}

void StrategyS11A::CloseTrade(const S11A_SubSystem SubSys, const TRADEDIR iTradeDir, const int iPeriod)
{
  // //--------------------------------------------------
  // // Close Trade
  // //--------------------------------------------------
  // set<int> setBasketID = m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetAllBasketIDInGroup(iPeriod);
  //
  //
  // HMS hmsMrngOpen ;
  // HMS hmsMrngClose;
  // HMS hmsAftnOpen ;
  // HMS hmsAftnClose;
  //
  // YMD ymdSysDate;
  // ymdhms_SysTime.GetYMD(ymdSysDate);
  //
  // m_HKFE->GetMrngOpen (ymdSysDate, hmsMrngOpen );
  // m_HKFE->GetMrngClose(ymdSysDate, hmsMrngClose);
  // m_HKFE->GetAftnOpen (ymdSysDate, hmsAftnOpen );
  // m_HKFE->GetAftnClose(ymdSysDate, hmsAftnClose);
  //
  // hmsMrngOpen .AddSecond     (AVOID_TRADING_NEAR_MKTOPEN_SEC );
  // hmsMrngClose.SubtractSecond(AVOID_TRADING_NEAR_MKTCLOSE_SEC);
  // hmsAftnOpen .AddSecond     (AVOID_TRADING_NEAR_MKTOPEN_SEC );
  // hmsAftnClose.SubtractSecond(AVOID_TRADING_NEAR_MKTCLOSE_SEC);
  //
  // if (
  //     (ymdhms_SysTime.GetHHMMSS() >= hmsMrngOpen && ymdhms_SysTime.GetHHMMSS() < hmsMrngClose) ||
  //     (ymdhms_SysTime.GetHHMMSS() >= hmsAftnOpen && ymdhms_SysTime.GetHHMMSS() < hmsAftnClose)
  //    )
  // {
  //   for (set<int>::iterator it_basketid = setBasketID.begin(); it_basketid != setBasketID.end(); ++it_basketid)
  //   {
  //     m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetTradeTimeOfBasketID(*it_basketid,ymdhms_TmpOpenTradeTime);
  //
  //     deque<double> dqRefVarb = m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetRefVariableForBasket(*it_basketid);
  //
  //     bool bShouldClose = false;
  //     string sOIDDetails;
  //
  //     sOIDDetails = GetSubSystemName(SubSys) + "_P" + boost::lexical_cast<string>(iPeriod) + "_C";
  //
  //     double dLnRtnDiffFromEntry   = log(dHSIPrice/dqRefVarb[1]) - log(dHHIPrice/dqRefVarb[2]);
  //     double dLnHSIHHIRatioAtEntry = log(dqRefVarb[1]/dqRefVarb[2]);
  //
  //     m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: %s Period: %d. dqRefVarb: %f %f %f %f. dLnRtnDiffFromEntry = %f. dLnHSIHHIRatioAtEntry = %f.",
  //         ymdhms_SysTime.ToStr().c_str(),
  //         GetSubSystemName(SubSys).c_str(),
  //         iPeriod,
  //         dqRefVarb[0],
  //         dqRefVarb[1],
  //         dqRefVarb[2],
  //         dqRefVarb[3],
  //         dLnRtnDiffFromEntry,
  //         dLnHSIHHIRatioAtEntry
  //         );
  //
  //
  //     if (ymdhms_SysTime - ymdhms_TmpOpenTradeTime > MIN_HOLDINGPERIOD_SEC)
  //     {
  //       m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: %s", ymdhms_SysTime.ToStr().c_str(), GetSubSystemName(SubSys).c_str());
  //
  //       if (
  //           SubSys == S11A_T1GBM
  //           &&
  //           (
  //            ((dqRefVarb[0] == LONG ) && (dLnRtnDiffFromEntry >  dT1GBMTakeProfitThreshold * dSDDeltaGamma)) ||
  //            ((dqRefVarb[0] == SHORT) && (dLnRtnDiffFromEntry < -dT1GBMTakeProfitThreshold * dSDDeltaGamma))
  //           )
  //          )
  //       {
  //         bShouldClose = true;
  //         sOIDDetails += "_TKPFT";
  //       }
  //       else if (
  //           SubSys == S11A_T1GBM
  //           &&
  //           (
  //            ((dqRefVarb[0] == LONG ) && (dLnRtnDiffFromEntry < -dT1GBMStopLossThreshold * dSDDeltaGamma)) ||
  //            ((dqRefVarb[0] == SHORT) && (dLnRtnDiffFromEntry >  dT1GBMStopLossThreshold * dSDDeltaGamma))
  //           )
  //           )
  //       {
  //         bShouldClose = true;
  //         sOIDDetails += "_STPLOS";
  //       }
  //       else if (
  //           SubSys == S11A_T1GBM
  //           &&
  //           ymdhms_SysTime - ymdhms_TmpOpenTradeTime > iPeriod * 2
  //           )
  //       {
  //         bShouldClose = true;
  //         sOIDDetails += "_EOL";
  //       }
  //
  //       else if (
  //           SubSys == S11A_T1GUMBELCAUCHY
  //           &&
  //           (
  //            ((dqRefVarb[0] == LONG ) && (dLnRtnDiffFromEntry >  dT1GumbelCauchyTakeProfitThreshold * dSDDeltaGamma)) ||
  //            ((dqRefVarb[0] == SHORT) && (dLnRtnDiffFromEntry < -dT1GumbelCauchyTakeProfitThreshold * dSDDeltaGamma))
  //           )
  //           )
  //       {
  //         bShouldClose = true;
  //         sOIDDetails += "_TKPFT";
  //       }
  //       else if (
  //           SubSys == S11A_T1GUMBELCAUCHY
  //           &&
  //           (
  //            ((dqRefVarb[0] == LONG ) && (dLnRtnDiffFromEntry < -dT1GumbelCauchyStopLossThreshold * dSDDeltaGamma)) ||
  //            ((dqRefVarb[0] == SHORT) && (dLnRtnDiffFromEntry >  dT1GumbelCauchyStopLossThreshold * dSDDeltaGamma))
  //           )
  //           )
  //       {
  //         bShouldClose = true;
  //         sOIDDetails += "_STPLOS";
  //       }
  //       else if (
  //           SubSys == S11A_T1GUMBELCAUCHY
  //           &&
  //           ymdhms_SysTime - ymdhms_TmpOpenTradeTime > iPeriod * dT1GumbelCauchyEOLPeriod
  //           )
  //       {
  //         bShouldClose = true;
  //         sOIDDetails += "_EOL";
  //       }
  //
  //
  //       // else
  //       if (
  //           SubSys == S11A_T1GUMBELGAUSS
  //           &&
  //           (
  //            ((dqRefVarb[0] == LONG ) && (dLnRtnDiffFromEntry >  dT1GumbelGaussTakeProfitThreshold * dSDDeltaGamma)) ||
  //            ((dqRefVarb[0] == SHORT) && (dLnRtnDiffFromEntry < -dT1GumbelGaussTakeProfitThreshold * dSDDeltaGamma))
  //           )
  //          )
  //       {
  //         m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: Side: %f dLnRtnDiffFromEntry: %f dLnHSIHHIRatioAtEntry: %f dT1GumbelGaussTakeProfitThreshold: %f dSDDeltaGamma: %f",
  //             GetSubSystemName(SubSys).c_str(),
  //             dqRefVarb[0],
  //             dLnRtnDiffFromEntry,
  //             dLnHSIHHIRatioAtEntry,
  //             dT1GumbelGaussTakeProfitThreshold,
  //             dSDDeltaGamma);
  //
  //         bShouldClose = true;
  //         sOIDDetails += "_TKPFT";
  //       }
  //       else if (
  //           SubSys == S11A_T1GUMBELGAUSS
  //           &&
  //           (
  //            ((dqRefVarb[0] == LONG ) && (dLnRtnDiffFromEntry < -dT1GumbelGaussStopLossThreshold * dSDDeltaGamma)) ||
  //            ((dqRefVarb[0] == SHORT) && (dLnRtnDiffFromEntry >  dT1GumbelGaussStopLossThreshold * dSDDeltaGamma))
  //           )
  //           )
  //       {
  //         m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: Side: %f dLnRtnDiffFromEntry: %f dLnHSIHHIRatioAtEntry: %f dT1GumbelGaussStopLossThreshold: %f dSDDeltaGamma: %f",
  //             GetSubSystemName(SubSys).c_str(),
  //             dqRefVarb[0],
  //             dLnRtnDiffFromEntry,
  //             dLnHSIHHIRatioAtEntry,
  //             dT1GumbelGaussStopLossThreshold,
  //             dSDDeltaGamma);
  //
  //         bShouldClose = true;
  //         sOIDDetails += "_STPLOS";
  //       }
  //       else if (
  //           SubSys == S11A_T1GUMBELGAUSS
  //           &&
  //           ymdhms_SysTime - ymdhms_TmpOpenTradeTime > iPeriod * dT1GumbelGaussEOLPeriod
  //           )
  //       {
  //         bShouldClose = true;
  //         sOIDDetails += "_EOL";
  //       }
  //
  //       else if (
  //           SubSys == S11A_T2GBM
  //           &&
  //           (
  //            ((dqRefVarb[0] == LONG ) && (dCurLnHSIHHIRatio > dLnHSIHHIRatioAtEntry + dEGamma + dT2GBMTakeProfitThreshold * dSDGamma)) ||
  //            ((dqRefVarb[0] == SHORT) && (dCurLnHSIHHIRatio < dLnHSIHHIRatioAtEntry + dEGamma - dT2GBMTakeProfitThreshold * dSDGamma))
  //           )
  //           )
  //       {
  //         // take profit
  //         bShouldClose = true;
  //         sOIDDetails += "_TKPFT";
  //       }
  //       else if (
  //           SubSys == S11A_T2GBM
  //           &&
  //           (
  //            ((dqRefVarb[0] == LONG ) && (dCurLnHSIHHIRatio < dLnHSIHHIRatioAtEntry + dEGamma - dT2GBMStopLossThreshold * dSDGamma)) ||
  //            ((dqRefVarb[0] == SHORT) && (dCurLnHSIHHIRatio > dLnHSIHHIRatioAtEntry + dEGamma + dT2GBMStopLossThreshold * dSDGamma))
  //           )
  //           )
  //       {
  //         // take profit
  //         bShouldClose = true;
  //         sOIDDetails += "_STPLOS";
  //       }
  //       else if (
  //           SubSys == S11A_T2GBM
  //           &&
  //           (abs(dCurLnHSIHHIRatio - dLongerTermLnHSIHHIRatio) < 0.5 * dSDGamma)
  //           )
  //       {
  //         // end of life, end of patience, the dLongerTermLnHSIHHIRatio will catch up with the current ratio eventually
  //         bShouldClose = true;
  //         sOIDDetails += "_EOL";
  //       }
  //       // else if (
  //       //     SubSys == S11A_T2PCTL
  //       //     &&
  //       //     (
  //       //      ((dqRefVarb[0] == LONG ) && (dCurLnHSIHHIRatio - dLongerTermLnHSIHHIRatio > dT2_50Pctl)) ||
  //       //      ((dqRefVarb[0] == SHORT) && (dCurLnHSIHHIRatio - dLongerTermLnHSIHHIRatio < dT2_50Pctl)) ||
  //       //      (((int)(dqRefVarb[3]) == ymdhms_SysTime.GetYYYYMMDD().ToInt()) && ymdhms_SysTime.GetHHMMSS() > HHMMSS(100000)) ||
  //       //      (((int)(dqRefVarb[3]) <  ymdhms_SysTime.GetYYYYMMDD().ToInt()))
  //       //     )
  //       //     )
  //       // {
  //       //   bShouldClose = true;
  //       //   sOIDDetails += "_EOL";
  //       // }
  //
  //     }
  //
  //     //--------------------------------------------------
  //     // If Close Trade conflicts with Open Trade, delay close
  //     //--------------------------------------------------
  //     if (bShouldClose
  //         &&
  //         (iTradeDir == (TRADEDIR)(int)dqRefVarb[0])
  //        )
  //     {
  //       bShouldClose = false;
  //       m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: SubSystem: %s %s TradeDir: %d. Close over-ridden by open signal.",ymdhms_SysTime.ToStr().c_str(),GetSubSystemName(SubSys).c_str(),sOIDDetails.c_str(),iTradeDir);
  //     }
  //     //--------------------------------------------------
  //
  //     string sCloseTradeSymbol;
  //     double dCloseEntryPrice = 0;
  //     long lCloseTradeSignedPos = 0;
  //
  //     if (bShouldClose)
  //     {
  //       set<int> setTradeid = m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetAllTradeIDInBasket(*it_basketid);
  //
  //       for (set<int>::iterator it_tradeid = setTradeid.begin(); it_tradeid != setTradeid.end(); ++it_tradeid)
  //       {
  //         m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].GetTradeDetails(*it_tradeid,sCloseTradeSymbol,dCloseEntryPrice,lCloseTradeSignedPos);
  //
  //         if (m_SysCfg->Get_S11AMonitoringIntervalInSec() <= 0)
  //         {
  //           m_PortAndOrders->Trade(static_cast<int>(STY_S11A),sCloseTradeSymbol,-lCloseTradeSignedPos,true,sOIDDetails);
  //         }
  //         else
  //         {
  //           if (m_SynchronizedTrades.find(sCloseTradeSymbol) == m_SynchronizedTrades.end()) m_SynchronizedTrades[sCloseTradeSymbol] = 0;
  //
  //           m_SynchronizedTrades[sCloseTradeSymbol] = m_SynchronizedTrades[sCloseTradeSymbol] + -lCloseTradeSignedPos;
  //         }
  //       }
  //
  //       m_TradeWarehouse[boost::lexical_cast<int>(SubSys)].RemoveBasketID(iPeriod,*it_basketid);
  //     }
  //   }
  // }
  //
  // return;
}

void StrategyS11A::InitFromSysConfig()
{
  //
  // m_StartTime                              = m_SysCfg->GetStrategyStartTime(STY_S11A);
  // m_EndTime                                = m_SysCfg->GetStrategyEndTime(STY_S11A);
  //
  // dNotionalValuePerLeg                     = m_SysCfg->Get_S11ANotionalValuePerLeg();
  // dT1GBMTriggerThreshold                   = m_SysCfg->Get_S11AT1GBMTriggerThreshold();
  // dT1GBMTakeProfitThreshold                = m_SysCfg->Get_S11AT1GBMTakeProfitThreshold();
  // dT1GBMStopLossThreshold                  = m_SysCfg->Get_S11AT1GBMStopLossThreshold();
  // dT2GBMTriggerThreshold                   = m_SysCfg->Get_S11AT2GBMTriggerThreshold();
  // dT2GBMTakeProfitThreshold                = m_SysCfg->Get_S11AT2GBMTakeProfitThreshold();
  // dT2GBMStopLossThreshold                  = m_SysCfg->Get_S11AT2GBMStopLossThreshold();
  // dT1GumbelCauchyTriggerThreshold          = m_SysCfg->Get_S11AT1GumbelCauchyTriggerThreshold();
  // dT1GumbelCauchyTakeProfitThreshold       = m_SysCfg->Get_S11AT1GumbelCauchyTakeProfitThreshold();
  // dT1GumbelCauchyStopLossThreshold         = m_SysCfg->Get_S11AT1GumbelCauchyStopLossThreshold();
  // dT1GumbelCauchyPeriod                    = m_SysCfg->Get_S11AT1GumbelCauchyPeriod();
  // dT1GumbelCauchyEOLPeriod                 = m_SysCfg->Get_S11AT1GumbelCauchyEOLPeriod();
  // dT1GumbelCauchyThetaMean                 = m_SysCfg->Get_S11AT1GumbelCauchyThetaMean();
  // dT1GumbelCauchyThetaMeanReversion        = m_SysCfg->Get_S11AT1GumbelCauchyThetaMeanReversion();
  // dT1GumbelCauchyMarginalSDScaling         = m_SysCfg->Get_S11AT1GumbelCauchyMarginalSDScaling();
  //
  // dT1GumbelGaussTriggerThreshold           = m_SysCfg->Get_S11AT1GumbelGaussTriggerThreshold();
  // dT1GumbelGaussTakeProfitThreshold        = m_SysCfg->Get_S11AT1GumbelGaussTakeProfitThreshold();
  // dT1GumbelGaussStopLossThreshold          = m_SysCfg->Get_S11AT1GumbelGaussStopLossThreshold();
  // dT1GumbelGaussPeriod                     = m_SysCfg->Get_S11AT1GumbelGaussPeriod();
  // dT1GumbelGaussEOLPeriod                  = m_SysCfg->Get_S11AT1GumbelGaussEOLPeriod();
  // dT1GumbelGaussThetaMean                  = m_SysCfg->Get_S11AT1GumbelGaussThetaMean();
  // dT1GumbelGaussThetaMeanReversion         = m_SysCfg->Get_S11AT1GumbelGaussThetaMeanReversion();
  // dT1GumbelGaussMarginalSDScaling          = m_SysCfg->Get_S11AT1GumbelGaussMarginalSDScaling();
  //
  // dT2CopuTriggerThreshold                  = m_SysCfg->Get_S11AT2CopuTriggerThreshold();
  // dT2CopuTakeProfitThreshold               = m_SysCfg->Get_S11AT2CopuTakeProfitThreshold();
  // dT2CopuStopLossThreshold                 = m_SysCfg->Get_S11AT2CopuStopLossThreshold();
  // m_T2_MAPeriods                           = m_SysCfg->Get_S11AT2MAPeriods();
  //
  // dT2PctlPeriod                            = m_SysCfg->Get_S11AT2PctlPeriod();
  // dT2CopuPeriod                            = m_SysCfg->Get_S11AT2CopuPeriod();
  //
  // m_T1GBM_DeltaTPeriods                    = m_SysCfg->Get_S11AT1GBMDeltaTPeriods();
  // m_T2GBM_DeltaTPeriods                    = m_SysCfg->Get_S11AT2GBMDeltaTPeriods();
  // return;
}



bool StrategyS11A::GetLegPrices()
{
  dHSIPrice     = 0;
  dHHIPrice     = 0;
  YYYYMMDDHHMMSS ymdhms1;
  YYYYMMDDHHMMSS ymdhms2;
  m_MarketData->GetLatestMidQuote(HSI_CONT_FUT_1,dHSIPrice,ymdhms1);
  m_MarketData->GetLatestMidQuote(HHI_CONT_FUT_1,dHHIPrice,ymdhms2);

  m_Logger->Write(Logger::DEBUG,"StrategyS11A HSIc1: %f %s.",dHSIPrice, ymdhms1.ToStr().c_str());
  m_Logger->Write(Logger::DEBUG,"StrategyS11A HHIc1: %f %s.",dHHIPrice, ymdhms2.ToStr().c_str());
  if (abs(ymdhms1 - ymdhms2) > 5) return false;

  return true;
}


void StrategyS11A::GetSDCorrel()
{
  dHSIHistSD    = m_map_HSI_HistSD[ymdhms_SysTime.GetYYYYMMDD().ToInt()];
  dHHIHistSD    = m_map_HHI_HistSD[ymdhms_SysTime.GetYYYYMMDD().ToInt()];
  dHSIIV        = m_map_HSI_IV[ymdhms_SysTime.GetYYYYMMDD().ToInt()];
  dHHIIV        = m_map_HHI_IV[ymdhms_SysTime.GetYYYYMMDD().ToInt()];
  dHSIHistVar   = dHSIHistSD * dHSIHistSD;
  dHHIHistVar   = dHHIHistSD * dHHIHistSD;
  dHSIIVar      = dHSIIV * dHSIIV;
  dHHIIVar      = dHHIIV * dHHIIV;
  dHSIHHICorrel = m_map_HSI_HHI_Correl[ymdhms_SysTime.GetYYYYMMDD().ToInt()];
  // double dT2_05Pctl    = m_map_T2_05Pctl[ymdhms_SysTime.GetYYYYMMDD().ToInt()];
  // double dT2_50Pctl    = m_map_T2_50Pctl[ymdhms_SysTime.GetYYYYMMDD().ToInt()];
  // double dT2_95Pctl    = m_map_T2_95Pctl[ymdhms_SysTime.GetYYYYMMDD().ToInt()];

  return;
}




void StrategyS11A::TradeSynchronizedOrders()
{
  //--------------------------------------------------
  if (
      m_bFirstTrade ||
      (
       (m_SysCfg->Get_S11AMonitoringIntervalInSec() > 0)
       &&
       (ymdhms_SysTime - m_ymdhms_LastMonitorTime >= m_SysCfg->Get_S11AMonitoringIntervalInSec())
      )
     )
  {
    for (map<string,double>::iterator it = m_SynchronizedTrades.begin(); it != m_SynchronizedTrades.end(); ++it)
    {
      if (it->second != 0)
        m_PortAndOrders->Trade(static_cast<int>(STY_S11A),it->first,it->second,true,"SynchronizedTrades");
    }

    m_SynchronizedTrades.clear();
    m_ymdhms_LastMonitorTime = ymdhms_SysTime;
    m_bFirstTrade = false;
  }
  //--------------------------------------------------
  return;
}


void StrategyS11A::CalcNumCtrt()
{
  //--------------------------------------------------
  dHHILegSize   = dNotionalValuePerLeg / dHHIPrice / 50;
  dHSILegSize_1 = dHHIIV * dHHIPrice / dHSIIV / dHSIPrice * dHHILegSize;
  iNoOfCtrtHSI_1   = round(dHSILegSize_1);
  iNoOfCtrtMHI_1   = (int)(round)((dHSILegSize_1 - (double)iNoOfCtrtHSI_1)*5);
  iNoOfCtrtHHI_1   = round(dHHILegSize);
  iNoOfCtrtMCH_1   = (int)(round)((dHHILegSize - (double)iNoOfCtrtHHI_1)*5);

  dHSILegSize_2 = dHHIPrice * dHHILegSize / dHSIPrice;
  iNoOfCtrtHSI_2   = round(dHSILegSize_2);
  iNoOfCtrtMHI_2   = (int)(round)((dHSILegSize_2 - (double)iNoOfCtrtHSI_2)*5);
  iNoOfCtrtHHI_2   = round(dHHILegSize);
  iNoOfCtrtMCH_2   = (int)(round)((dHHILegSize - (double)iNoOfCtrtHHI_2)*5);

  iNoOfCtrtHSI = 0;
  iNoOfCtrtMHI = 0;
  iNoOfCtrtHHI = 0;
  iNoOfCtrtMCH = 0;
  //--------------------------------------------------

  m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: dHSIPrice = %f. dHHIPrice = %f. dHSILegSize_1 = %f. dHHILegSize = %f, HSI MHI HHI MCH # contracts: %d %d %d %d. dHSILegSize_2 = %f. HSI MHI HHI MCH # contracts: %d %d %d %d.",
      ymdhms_SysTime.ToStr().c_str(),
      dHSIPrice,
      dHHIPrice,
      dHSILegSize_1,
      dHHILegSize,
      iNoOfCtrtHSI_1,
      iNoOfCtrtMHI_1,
      iNoOfCtrtHHI_1,
      iNoOfCtrtMCH_1,
      dHSILegSize_2,
      iNoOfCtrtHSI_2,
      iNoOfCtrtMHI_2,
      iNoOfCtrtHHI_2,
      iNoOfCtrtMCH_2
      );

  return;
}


void StrategyS11A::OutputStratParam(const S11A_SubSystem SubSys, const int iPeriod)
{
  if (SubSys == S11A_T1GBM)
  {
    m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: T1GBM Period: %d. HSI LnTickReturn = %f. HHI LnTickReturn = %f. Diff = %f. DeltaT = %f. dEDeltaGamma = %f. dSDDeltaGamma = %f.",
        ymdhms_SysTime.ToStr().c_str(),
        iPeriod,
        dHSILnRtn,
        dHHILnRtn,
        dLnRtnDiff,
        dDeltaT,
        dEDeltaGamma,
        dSDDeltaGamma
        );
  }
  else if (SubSys == S11A_T1GUMBELCAUCHY || SubSys == S11A_T1GUMBELGAUSS)
  {
    m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: T1GUMBELCAUCHY / T1GUMBELGAUSS Period: %d. HSI LnTickReturn = %f. HHI LnTickReturn = %f. Diff = %f. DeltaT = %f. CurHSIHHIRatio = %f. PrevHSIHHIRatio = %f.",
        ymdhms_SysTime.ToStr().c_str(),
        iPeriod,
        dHSILnRtn,
        dHHILnRtn,
        dLnRtnDiff,
        dDeltaT,
        dCurLnHSIHHIRatio,
        dPrevLnHSIHHIRatio
        );
  }
  else if (SubSys == S11A_T2GBM)
  {
    m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: T2GBM Period: %d. ln(HSI/HHI) = %f. LT ln(HSI/HHI) = %f. DeltaT = %f. dEGamma = %f. dSDGamma = %f. Lower = %f. Upper = %f.",
        ymdhms_SysTime.ToStr().c_str(),
        iPeriod,
        dCurLnHSIHHIRatio,
        dLongerTermLnHSIHHIRatio,
        dDeltaT,
        dEGamma,
        dSDGamma,
        dLongerTermLnHSIHHIRatio + dEGamma - dT2GBMTriggerThreshold * dSDGamma,
        dLongerTermLnHSIHHIRatio + dEGamma + dT2GBMTriggerThreshold * dSDGamma
        );
  }
  else if (SubSys == S11A_T2PCTL)
  {
    m_Logger->Write(Logger::DEBUG,"StrategyS11A: %s: T2PCTL ln(HSI/HHI) = %f. LT ln(HSI/HHI) = %f. Dist = %f.", // 05Pctl = %f. 50Pctl = %f. 95Pctl = %f",
        ymdhms_SysTime.ToStr().c_str(),
        dCurLnHSIHHIRatio,
        dLongerTermLnHSIHHIRatio,
        dCurLnHSIHHIRatio-dLongerTermLnHSIHHIRatio
        // dT2_05Pctl,
        // dT2_50Pctl,
        // dT2_95Pctl
        );
  }
  return;
}


void StrategyS11A::CalcLnRtnGBMParam(const int iPeriod)
{
  dEDeltaGamma  = ((dHHIIVar/2) - (dHSIIVar/2)) * dDeltaT;
  dSDDeltaGamma = sqrt((dHSIIVar + dHHIIVar - 2 * dHSIHHICorrel * dHSIIV * dHHIIV) * dDeltaT);
  // dSDDeltaGamma = sqrt((dHSIIVar + dHHIIVar) * dDeltaT); //intraday correlation seems lower than daily

  dEGamma  = dEDeltaGamma;
  dSDGamma = dSDDeltaGamma;

  dCurLnHSIHHIRatio  = log(dHSIPrice / dHHIPrice);
  dHSILnRtn          = m_TechInd->GetLnTickReturn(HSI_CONT_FUT_1,iPeriod);
  dHHILnRtn          = m_TechInd->GetLnTickReturn(HHI_CONT_FUT_1,iPeriod);
  dLnRtnDiff         = (dHSILnRtn-dHHILnRtn);
  dPrevLnHSIHHIRatio = dCurLnHSIHHIRatio - dLnRtnDiff;
  // dLongerTermLnHSIHHIRatio = m_TechInd->GetlnHSIHHIRatioSma(m_T2_MAPeriods[0]);
  dLongerTermLnHSIHHIRatio = m_TechInd->GetlnHSIHHIRatioEma(m_T2_MAPeriods[0]);

  return;
}

