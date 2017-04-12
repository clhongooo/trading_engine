#include <TradingEngineMainThread.h>

TradingEngineMainThread::TradingEngineMainThread
(const char * sConfPath) :
  m_ConfigPath(string(sConfPath)),
  m_ItrdHighLowFromIB(""),
  m_FinishedInit(false)
{
}

TradingEngineMainThread::TradingEngineMainThread
(const char * sConfPath,
 const char * sItrdHighLowFromIB) :
  m_ConfigPath(string(sConfPath)),
  m_ItrdHighLowFromIB(string(sItrdHighLowFromIB)),
  m_FinishedInit(false)
{
}

TradingEngineMainThread::~TradingEngineMainThread()
{
}


bool TradingEngineMainThread::HasFinishedInit()
{
  return m_FinishedInit;
}

void TradingEngineMainThread::RunMainThread()
{
  string sConfigPath(m_ConfigPath);

  //--------------------------------------------------
  // System Objects
  //--------------------------------------------------
  p_SysCfg = SystemConfig::Instance();
  p_SysCfg->ReadConfig(sConfigPath);
  if (m_ItrdHighLowFromIB != "") p_SysCfg->SetItrdHighLowFromIB(m_ItrdHighLowFromIB);

  //--------------------------------------------------
  p_Logger = Logger::Instance();

  p_Logger->Write(Logger::NOTICE,"Main: Trading Engine has started.");


  //--------------------------------------------------
  p_MktData = MarketData::Instance();
  p_MktData->SetContFutHKFERollFwdTime(p_SysCfg->Get_HKFEContFutRollFwdTime());
  p_MktData->SetContFutCMERollFwdTime(p_SysCfg->Get_CMEContFutRollFwdTime());
  //--------------------------------------------------

  //--------------------------------------------------
  // Threads
  //--------------------------------------------------
  boost::thread_group m_thread_group;


  //--------------------------------------------------
  // Static Data: Exchange HKFE HKSE HKMA
  //--------------------------------------------------
  p_Exchg = Exchange::Instance();
  p_Exchg->LoadTradingHours(p_SysCfg->Get_TradingHoursPath());

  p_HKFE = HKFE::Instance();
  p_HKFE->LoadCalendar(p_SysCfg->Get_HKFE_CalendarPath());
  p_Logger->Write(Logger::NOTICE,"Finished loading HKFE."); ShortSleep();

  p_HKSE = HKSE::Instance();
  p_HKSE->LoadHSIConstituents(p_SysCfg->Get_HKSE_HSIConstituentsPath()); 
  p_Logger->Write(Logger::NOTICE,"Finished loading HKSE."); ShortSleep();

  if (p_SysCfg->IsStrategyOn(STY_NIR))
  {
    boost::shared_ptr<HKMA> p_HKMA;
    p_HKMA = HKMA::Instance();
    p_HKMA->LoadExchgFundBill(p_SysCfg->Get_HKMA_ExchgFundBillPath()); 
    p_Logger->Write(Logger::NOTICE,"Finished loading HKMA."); ShortSleep();

    boost::shared_ptr<CorrelMatrices> p_CorrelMatrices;
    p_CorrelMatrices = CorrelMatrices::Instance();
    p_CorrelMatrices->LoadCorrelMatrices(p_SysCfg->Get_CorrelMatricesPath());
    p_Logger->Write(Logger::NOTICE,"Finished loading correlation matrices."); ShortSleep();

    ProbDistributionGenerator pdg;
    pdg.SetCalcIntervalInSec(p_SysCfg->Get_ProbDistrnCalcIntervalInSec());
    pdg.LoadTrainedFSMCData(p_SysCfg->Get_ProbDistrFileFSMC1D());
    p_Logger->Write(Logger::NOTICE,"Finished loading FSMC data."); ShortSleep();
    m_thread_group.add_thread(new boost::thread(&ProbDistributionGenerator::Run, &pdg));
    p_Logger->Write(Logger::NOTICE,"Started thread: ProbDistributionGenerator"); ShortSleep();

    VolSurfCalculator vsc;
    vsc.SetCalcIntervalInSec(p_SysCfg->Get_VolSurfCalcIntervalInSec());
    p_Logger->Write(Logger::NOTICE,"Finished loading VolSurfCalculator."); ShortSleep();

    boost::shared_ptr<VolSurfaces> p_VolSurfaces;
    p_VolSurfaces = VolSurfaces::Instance();
    p_VolSurfaces->LoadHSIVolSurfModelParam(p_SysCfg->Get_VolSurfParamFile1FM()); 
    p_Logger->Write(Logger::NOTICE,"Finished loading volatility surface parameters."); ShortSleep();

  }
  else
  {
    p_Logger->Write(Logger::NOTICE,"HKMA not loaded."); ShortSleep();
    p_Logger->Write(Logger::NOTICE,"CorrelMatrices not loaded."); ShortSleep();
    p_Logger->Write(Logger::NOTICE,"FSMC not loaded."); ShortSleep();
    p_Logger->Write(Logger::NOTICE,"VolSurfCalculator not loaded."); ShortSleep();
    p_Logger->Write(Logger::NOTICE,"VolatilitySurface parameters not loaded."); ShortSleep();
  }

  //--------------------------------------------------
  // ThreadHealthMonitor
  //--------------------------------------------------
  boost::shared_ptr<ThreadHealthMonitor> pThm = ThreadHealthMonitor::Instance();

  //--------------------------------------------------
  // Health monitor
  //--------------------------------------------------
  m_thread_group.add_thread(new boost::thread(&ThreadHealthMonitor::Run, pThm.get()));
  p_Logger->Write(Logger::NOTICE,"Started thread: ThreadHealthMonitor"); ShortSleep();

  //--------------------------------------------------
  // Strategy threads
  //--------------------------------------------------
  boost::scoped_ptr<StrategyTest>    styTest(new StrategyTest());
  boost::scoped_ptr<StrategyB2_US1>  styB2_US1(new StrategyB2_US1());
  boost::scoped_ptr<StrategyB2_US2>  styB2_US2(new StrategyB2_US2());
  boost::scoped_ptr<StrategyB2_US3>  styB2_US3(new StrategyB2_US3());
  boost::scoped_ptr<StrategyB2_HK>   styB2_HK(new StrategyB2_HK());
  boost::scoped_ptr<StrategyB3_US>   styB3_US(new StrategyB3_US());
  boost::scoped_ptr<StrategyNIR1>    sty_NIR1(new StrategyNIR1());

  if (p_SysCfg->IsStrategyOn(STY_TEST))
  {
    m_thread_group.add_thread(new boost::thread(&StrategyTest::Run, styTest.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyTest"); ShortSleep();
  }

  if (p_SysCfg->IsStrategyOn(STY_B2_US1))
  {
    styB2_US1.reset(new StrategyB2_US1());
    m_thread_group.add_thread(new boost::thread(&StrategyB2_US1::Run, styB2_US1.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB2_US1"); ShortSleep();
  }

  if (p_SysCfg->IsStrategyOn(STY_B2_US2))
  {
    styB2_US2.reset(new StrategyB2_US2());
    m_thread_group.add_thread(new boost::thread(&StrategyB2_US2::Run, styB2_US2.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB2_US2"); ShortSleep();
  }

  if (p_SysCfg->IsStrategyOn(STY_B2_US3))
  {
    styB2_US3.reset(new StrategyB2_US3());
    m_thread_group.add_thread(new boost::thread(&StrategyB2_US3::Run, styB2_US3.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB2_US3"); ShortSleep();
  }

  if (p_SysCfg->IsStrategyOn(STY_B2_HK))
  {
    styB2_HK.reset(new StrategyB2_HK());
    m_thread_group.add_thread(new boost::thread(&StrategyB2_HK::Run, styB2_HK.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB2_HK"); ShortSleep();
  }

  if (p_SysCfg->IsStrategyOn(STY_B3_US))
  {
    styB3_US.reset(new StrategyB3_US());
    m_thread_group.add_thread(new boost::thread(&StrategyB3_US::Run, styB3_US.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB3_US"); ShortSleep();
  }

  if (p_SysCfg->IsStrategyOn(STY_NIR1))
  {
    sty_NIR1.reset(new StrategyNIR1());
    m_thread_group.add_thread(new boost::thread(&StrategyNIR1::Run, sty_NIR1.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyNIR1"); ShortSleep();
  }

  // boost::scoped_ptr<PortfolioGenerator> pg;
  // pg.reset(new PortfolioGenerator());
  // m_thread_group.add_thread(new boost::thread(&PortfolioGenerator::Run, pg.get()));
  // p_Logger->Write(Logger::NOTICE,"Started thread: PortfolioGenerator"); ShortSleep();

  // m_thread_group.add_thread(new boost::thread(&VolSurfCalculator::Run, &vsc));
  // p_Logger->Write(Logger::NOTICE,"Started thread: VolSurfCalculator"); ShortSleep();

  //--------------------------------------------------
  // Other threads
  //--------------------------------------------------
  MarkToMarket mtm;
  m_thread_group.add_thread(new boost::thread(&MarkToMarket::Run, &mtm));
  p_Logger->Write(Logger::NOTICE,"Started thread: MarkToMarket"); ShortSleep();

  TechIndUpdater tiu;
  m_thread_group.add_thread(new boost::thread(&TechIndUpdater::Run, &tiu));
  p_Logger->Write(Logger::NOTICE,"Started thread: TechIndUpdater"); ShortSleep();

  PriceForwarderToNextTier pf;
  if (p_SysCfg->IsPriceFwdrToNextTierOn())
  {
    m_thread_group.add_thread(new boost::thread(&PriceForwarderToNextTier::Run, &pf));
    p_Logger->Write(Logger::NOTICE,"Started thread: PriceForwarderToNextTier"); ShortSleep();
  }

  TerminalThread tthd;
  if (p_SysCfg->GetSystemServicePort() >= 1024)
  {
    m_thread_group.add_thread(new boost::thread(&TerminalThread::Run, &tthd));
    p_Logger->Write(Logger::NOTICE,"Started thread: TerminalThread"); ShortSleep();
  }


  //--------------------------------------------------
  // Start all other threads before the MD threads, otherwise these thread will miss the initial data.
  //--------------------------------------------------
  {
    //--------------------------------------------------
    // Start OTI threads
    //--------------------------------------------------
    //--------------------------------------------------
    // OTI (only support 1 OTI now...)
    //--------------------------------------------------
    p_oe.reset(new OrderExecutor());
    if (p_SysCfg->Get_OTIMode() == SystemConfig::OTI_TCP)
    {
      int iNumOfOTI = p_SysCfg->GetNumOfOTI();
      for (unsigned int i = 0; i < iNumOfOTI; ++i)
      {
        string sIP   = p_SysCfg->Get_OTI_IP(i);
        string sPort = p_SysCfg->Get_OTI_Port(i);
        p_Logger->Write(Logger::NOTICE,"Read from SystemConfig: OTI %d: %s %s",i,sIP.c_str(),sPort.c_str());

        p_oe->SetOTIServer(sIP,sPort);
        p_Logger->Write(Logger::NOTICE,"Finished loading OTI %d.",i);
        m_thread_group.add_thread(new boost::thread(&OrderExecutor::Run           ,(p_oe.get())));
        // m_thread_group.add_thread(new boost::thread(&OrderExecutor::RunChkOrd     ,(p_oe.get())));
        p_Logger->Write(Logger::NOTICE,"Started thread: OrderExecutor %s", sIP.c_str()); ShortSleep();
      }
    }
    else
    {
      p_Logger->Write(Logger::NOTICE,"OTI not loaded."); ShortSleep();
    }

    //--------------------------------------------------
    // start RunPersistPos always
    //--------------------------------------------------
    m_thread_group.add_thread(new boost::thread(&OrderExecutor::RunPersistPos,(p_oe.get())));

    //--------------------------------------------------
    // Start MDI threads
    //--------------------------------------------------
    sleep(1);

    //--------------------------------------------------
    // MDI
    //--------------------------------------------------
    int iNumOfMDI = p_SysCfg->GetNumOfMDI();
    p_dataagg.clear();
    p_dataagg.insert(p_dataagg.begin(),iNumOfMDI,boost::shared_ptr<MDIDataReceiver>());
    for (unsigned int i = 0; i < iNumOfMDI; ++i)
    {
      string sFile = p_SysCfg->Get_MDI_File(i);
      string sIP   = p_SysCfg->Get_MDI_IP(i);
      string sPort = p_SysCfg->Get_MDI_Port(i);
      p_Logger->Write(Logger::NOTICE,"Read from SystemConfig: MDI %d: File %s IP %s Port %s",i,sFile.c_str(),sIP.c_str(),sPort.c_str());

      p_dataagg[i].reset(new MDIDataReceiver(i));
      if (sFile == "")
      {
        p_dataagg[i]->SetMDIServer(sIP,sPort);
        m_thread_group.add_thread(new boost::thread(&MDIDataReceiver::Run ,(p_dataagg[i].get())));
      }
      else
      {
        m_thread_group.add_thread(new boost::thread(&MDIDataReceiver::ReadDataFile,(p_dataagg[i].get()),sFile));
      }

      p_Logger->Write(Logger::NOTICE,"Finished loading MDI %d: %s %s",i,sIP.c_str(),sPort.c_str());
      p_Logger->Write(Logger::NOTICE,"Started thread: MDIDataReceiver %d",i);
      ShortSleep();
    }

  }


  m_FinishedInit = true;

  cout << "Nirvana: all threads are started." << endl << flush;
  cout << "Location of log file: " << p_SysCfg->Get_Main_Log_Path() << endl << flush;

  //--------------------------------------------------
  // Block here
  //--------------------------------------------------
  m_thread_group.join_all();

  //--------------------------------------------------
  // Bye
  //--------------------------------------------------
  p_Logger->Write(Logger::NOTICE,"Main: TradingEngine has stopped.");

  return;
}
