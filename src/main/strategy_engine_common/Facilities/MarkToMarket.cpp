#include <MarkToMarket.h>

MarkToMarket::MarkToMarket()
{
  m_MarketData      = MarketData::Instance();
  m_SystemState     = SystemState::Instance();
  m_SysCfg          = SystemConfig::Instance();
  m_Logger          = Logger::Instance();
  m_TechInd         = TechIndicators::Instance();
  m_PortAndOrders   = PortfoliosAndOrders::Instance();
  m_ThrdHlthMon     = ThreadHealthMonitor::Instance();
  m_StyCPnLHist     = StyCPnLHist::Instance();
  m_MDIAck          = MDI_Acknowledgement::Instance();

}

MarkToMarket::~MarkToMarket()
{
}

void MarkToMarket::Run()
{
  //--------------------------------------------------
  // Init path
  //--------------------------------------------------
  ofstream fsMTMLog(m_SysCfg->Get_MTM_Log_Path().c_str());
  ofstream fsMTMBySymLog(m_SysCfg->Get_MTMBySym_Log_Path().c_str());
  ofstream fsHoldingsLog(m_SysCfg->Get_Holdings_Log_Path().c_str());

  for (int sty = STY_NIR; sty != STY_LAST; sty++)
  {
    StrategyID sid = static_cast<StrategyID>(sty);
    m_Sty_MTM_Done[sid] = set<int>();
  }

  //--------------------------------------------------
  for (;;)
  {
    // m_MarketData->WaitForData(OTIMDI_HKFE);
    m_TechInd->WaitForData();

    if (m_SystemState->ChkIfThreadShouldStop()) break;

    m_ThrdHlthMon->ReportHealthy(ID_MTM);

    YYYYMMDDHHMMSS ymdhms_MDITime = m_MarketData->GetSystemTimeHKT();

    //--------------------------------------------------
    double dOverallCPnL = 0;

    //--------------------------------------------------
    for (int sty = STY_NIR; sty != STY_LAST; sty++)
    {
      StrategyID sid = static_cast<StrategyID>(sty);

      if (m_SysCfg->IsStrategyOn(sid) &&
          ymdhms_MDITime.GetHHMMSS() >= m_SysCfg->GetStrategyMTMTime(sid) &&
          m_Sty_MTM_Done[sid].find(ymdhms_MDITime.GetYYYYMMDD().ToInt()) == m_Sty_MTM_Done[sid].end())
      {
        double dMidQuote = 0;
        YYYYMMDDHHMMSS ymdhms1;
        set<string> setSym = m_PortAndOrders->GetSymbolsWithPos(sid);

        //--------------------------------------------------
        // provide the price for MTM
        //--------------------------------------------------
        FForEach (setSym,[&](const string & sym) {
          if (!m_MarketData->GetLatestMidQuote(sym,dMidQuote))
          {
            m_Logger->Write(Logger::DEBUG,"Strategy %s: %s MTMLog: %s Can't get latest mid-quote",
                            GetStrategyName(sid).c_str(), ymdhms_MDITime.ToStr().c_str(), sym.c_str());
            return;
          }
          if (!STool::IsValidPriceOrVol(dMidQuote))
          {
            m_Logger->Write(Logger::DEBUG,"Strategy %s: %s MTMLog: %s Mid quote %d is invalid",
                            GetStrategyName(sid).c_str(), ymdhms_MDITime.ToStr().c_str(), sym.c_str(), dMidQuote);
            return;
          }

          m_PortAndOrders->ProvidePriceForMTM(sid, sym, dMidQuote);
          m_Logger->Write(Logger::DEBUG,"Strategy %s: %s MTMLog: ProvidePriceForMTM %s %f",
                          GetStrategyName(sid).c_str(), ymdhms_MDITime.ToStr().c_str(), sym.c_str(), dMidQuote);
        });

        //--------------------------------------------------
        // get the PnL per strategy
        //--------------------------------------------------
        double d_MTM_CPnL_PerSty = 0;
        if (m_PortAndOrders->Get_MTM_CPnL(sid,d_MTM_CPnL_PerSty))
        {
          // fsMTMLog << GetStrategyName(sid) << "\t" << sid << "\t" << ymdhms_MDITime.GetYYYYMMDD().ToInt() << "\t" << ymdhms_MDITime.GetHHMMSS().ToInt() << "\t" << setprecision(2) << fixed << d_MTM_CPnL_PerSty << endl;
          dOverallCPnL += d_MTM_CPnL_PerSty;
          m_Logger->Write(Logger::INFO,"Strategy %s: MTMLog:\t%d\t%s\t%.2f", GetStrategyName(sid).c_str(), sid, ymdhms_MDITime.ToStr().c_str(), d_MTM_CPnL_PerSty);
          m_StyCPnLHist->CPnLHistAddAndTruncate(sty,ymdhms_MDITime.GetYYYYMMDD(),d_MTM_CPnL_PerSty);
          m_StyCPnLHist->CPnLHistWriteToFile(sty);
        }
        else
        {
          // fsMTMLog << GetStrategyName(sid) << "\t" << sid << "\t" << ymdhms_MDITime.GetYYYYMMDD().ToInt() << "\t" << ymdhms_MDITime.GetHHMMSS().ToInt() << "\t" << "N/A" << endl;
          m_Logger->Write(Logger::INFO,"Strategy %s: MTMLog:\t%d\t%s\tN/A", GetStrategyName(sid).c_str(), sid, ymdhms_MDITime.ToStr().c_str());
          FForEach (setSym,[&](const string & sym) {
            m_Logger->Write(Logger::INFO,"Strategy %s: MTMLog:\t%d\t%s\t Symbol with pos includes: %s",
                            GetStrategyName(sid).c_str(), sid, ymdhms_MDITime.ToStr().c_str(), sym.c_str());
          });
        }

        //--------------------------------------------------
        // get the PnL per strategy per symbol
        //--------------------------------------------------
        map<string,double> map_sym_mtm = m_PortAndOrders->GetSymbolsAndMTMPnL(sid);
        FForEach(map_sym_mtm,[&](const std::pair<string,double> & tup)
        {
          fsMTMBySymLog << GetStrategyName(sid) << "\t" << sid << "\t" << ymdhms_MDITime.GetYYYYMMDD().ToInt() << "\t" << ymdhms_MDITime.GetHHMMSS().ToInt() << "\t" << tup.first << "\t" << tup.second << endl;
        });


        //--------------------------------------------------
        // get actual portfolio
        //--------------------------------------------------
        map<string,long> mPort;
        if (m_PortAndOrders->GetActualPortfolio(sid,mPort))
        {
          for (map<string,long>::iterator it = mPort.begin(); it != mPort.end(); ++it)
          {
            if (it->second != 0)
              fsHoldingsLog << GetStrategyName(sid) << "\t" << sid << "\t" << ymdhms_MDITime.GetYYYYMMDD().ToInt() << "\t" << ymdhms_MDITime.GetHHMMSS().ToInt() << "\t" << it->first << "\t" << it->second << endl;

            m_Logger->Write(Logger::INFO,"Strategy %s:HoldingsLog:\t%d\t%s\t%s\t%d", GetStrategyName(sid).c_str(), sid, ymdhms_MDITime.ToStr().c_str(), it->first.c_str(), it->second);
          }
        }

        //--------------------------------------------------
        // mark today as done
        //--------------------------------------------------
        m_Sty_MTM_Done[sid].insert(ymdhms_MDITime.GetYYYYMMDD().ToInt());
      }
    }

    //--------------------------------------------------
    // output the sum of PnL of all strategies
    //--------------------------------------------------
    if (abs(dOverallCPnL) > NIR_EPSILON &&
      m_Overall_MTM_Done.find(ymdhms_MDITime.GetYYYYMMDD().ToInt()) == m_Overall_MTM_Done.end())
    {
      fsMTMLog << ymdhms_MDITime.ToStr() << "\t" << setprecision(2) << fixed << dOverallCPnL << endl;
      m_Overall_MTM_Done.insert(ymdhms_MDITime.GetYYYYMMDD().ToInt());
    }
    //--------------------------------------------------

    ReportAckIfNeeded();
  }

  fsMTMLog.close();
  fsMTMBySymLog.close();
  fsHoldingsLog.close();

  m_Logger->Write(Logger::NOTICE,"MarkToMarket has ended.");
  sleep(2);

  return;
}

void MarkToMarket::ReportAckIfNeeded()
{
  if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK || m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
    m_MDIAck->ReportAckMTM();
  return;
}

