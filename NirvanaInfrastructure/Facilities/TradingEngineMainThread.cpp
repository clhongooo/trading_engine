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

  p_Logger->Write(Logger::NOTICE,"Main: Nirvana has started.");

  //--------------------------------------------------
  // Static Data: Exchange HKFE HKSE HKMA
  //--------------------------------------------------
  p_Exchg = Exchange::Instance();
  p_Exchg->LoadTradingHours(p_SysCfg->Get_TradingHoursPath());

  p_HKFE = HKFE::Instance();
  p_HKFE->LoadCalendar(p_SysCfg->Get_HKFE_CalendarPath());
  p_Logger->Write(Logger::NOTICE,"Finished loading HKFE."); usleep(100000);

  p_HKSE = HKSE::Instance();
  p_HKSE->LoadHSIConstituents(p_SysCfg->Get_HKSE_HSIConstituentsPath()); 
  p_Logger->Write(Logger::NOTICE,"Finished loading HKSE."); usleep(100000);

  p_HKMA = HKMA::Instance();
  if (p_SysCfg->IsStrategyOn(STY_NIR))
  {
    p_HKMA->LoadExchgFundBill(p_SysCfg->Get_HKMA_ExchgFundBillPath()); 
    p_Logger->Write(Logger::NOTICE,"Finished loading HKMA."); usleep(100000);
  }
  else
  {
    p_Logger->Write(Logger::NOTICE,"HKMA not loaded."); usleep(100000);
  }

  //--------------------------------------------------
  // Static Data: Correl Matrix
  //--------------------------------------------------
  p_CorrelMatrices = CorrelMatrices::Instance();
  if (p_SysCfg->IsStrategyOn(STY_NIR))
  {
    p_CorrelMatrices->LoadCorrelMatrices(p_SysCfg->Get_CorrelMatricesPath());
    p_Logger->Write(Logger::NOTICE,"Finished loading correlation matrices."); usleep(100000);
  }
  else
  {
    p_Logger->Write(Logger::NOTICE,"CorrelMatrices not loaded."); usleep(100000);
  }

  //--------------------------------------------------
  // ProbDistributionGenerator
  //--------------------------------------------------
  ProbDistributionGenerator pdg;
  if (p_SysCfg->IsStrategyOn(STY_NIR))
  {
    pdg.SetCalcIntervalInSec(p_SysCfg->Get_ProbDistrnCalcIntervalInSec());
    pdg.LoadTrainedFSMCData(p_SysCfg->Get_ProbDistrFileFSMC1D());
    p_Logger->Write(Logger::NOTICE,"Finished loading FSMC data."); usleep(100000);
  }
  else
  {
    p_Logger->Write(Logger::NOTICE,"FSMC not loaded."); usleep(100000);
  }

  //--------------------------------------------------
  // VolSurfCalculator
  //--------------------------------------------------
  VolSurfCalculator vsc;
  vsc.SetCalcIntervalInSec(p_SysCfg->Get_VolSurfCalcIntervalInSec());
  p_Logger->Write(Logger::NOTICE,"Finished loading VolSurfCalculator."); usleep(100000);

  //--------------------------------------------------
  // VolSurfaces
  //--------------------------------------------------
  boost::shared_ptr<VolSurfaces> p_VolSurfaces;
  p_VolSurfaces = VolSurfaces::Instance();

  if (p_SysCfg->IsStrategyOn(STY_NIR))
  {
    p_VolSurfaces->LoadHSIVolSurfModelParam(p_SysCfg->Get_VolSurfParamFile1FM()); 
    p_Logger->Write(Logger::NOTICE,"Finished loading volatility surface parameters."); usleep(100000);
  }
  else
  {
    p_Logger->Write(Logger::NOTICE,"VolatilitySurface parameters not loaded."); usleep(100000);
  }

  //--------------------------------------------------
  // TechIndUpdater
  //--------------------------------------------------
  TechIndUpdater tiu;

  //--------------------------------------------------
  // PriceForwarderToNextTier
  //--------------------------------------------------
  PriceForwarderToNextTier pf;

  //--------------------------------------------------
  // PortfolioGenerator
  //--------------------------------------------------
  boost::scoped_ptr<StrategyTest>       styTest;
  boost::scoped_ptr<PortfolioGenerator> pg;
  boost::scoped_ptr<StrategyB1_HKF>     styB1_HKF;
  boost::scoped_ptr<StrategyB2_US1>     styB2_US1;
  boost::scoped_ptr<StrategyB2_US2>     styB2_US2;
  boost::scoped_ptr<StrategyB2_US3>     styB2_US3;
  boost::scoped_ptr<StrategyB2_HK>      styB2_HK;
  boost::scoped_ptr<StrategyNIR1>       styNIR1;
  boost::scoped_ptr<StrategyR7>         styR7;
  boost::scoped_ptr<StrategyR9>         styR9;
  // boost::scoped_ptr<StrategyA1>         styA1;
  // boost::scoped_ptr<StrategyA6>         styA6;
  // boost::scoped_ptr<StrategyR1>         styR1;
  // boost::scoped_ptr<StrategyR3>         styR3;
  // boost::scoped_ptr<StrategyR8>         styR8;
  // boost::scoped_ptr<StrategyS11A>       styS11A;

  //--------------------------------------------------
  // Terminal
  //--------------------------------------------------
  // TerminalThread tthd;

  //--------------------------------------------------
  // MarkToMarket
  //--------------------------------------------------
  MarkToMarket mtm;

  //--------------------------------------------------
  // ThreadHealthMonitor
  //--------------------------------------------------
  boost::shared_ptr<ThreadHealthMonitor> pThm = ThreadHealthMonitor::Instance();

  //--------------------------------------------------
  // Threads
  //--------------------------------------------------
  boost::thread_group m_thread_group;

  m_thread_group.add_thread(new boost::thread(&ThreadHealthMonitor::Run, pThm.get()));
  p_Logger->Write(Logger::NOTICE,"Started thread: ThreadHealthMonitor"); usleep(100000);



  if (p_SysCfg->IsStrategyOn(STY_NIR))
  {
    m_thread_group.add_thread(new boost::thread(&ProbDistributionGenerator::Run, &pdg));
    p_Logger->Write(Logger::NOTICE,"Started thread: ProbDistributionGenerator"); usleep(100000);
  }

  if (p_SysCfg->IsStrategyOn(STY_TEST))
  {
    styTest.reset(new StrategyTest());
    m_thread_group.add_thread(new boost::thread(&StrategyTest::Run, styTest.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyTest"); usleep(100000);
  }

  if (p_SysCfg->IsStrategyOn(STY_B1_HKF))
  {
    styB1_HKF.reset(new StrategyB1_HKF());
    m_thread_group.add_thread(new boost::thread(&StrategyB1_HKF::Run, styB1_HKF.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB1_HKF"); usleep(100000);
  }

  if (p_SysCfg->IsStrategyOn(STY_B2_US1))
  {
    styB2_US1.reset(new StrategyB2_US1());
    m_thread_group.add_thread(new boost::thread(&StrategyB2_US1::Run, styB2_US1.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB2_US1"); usleep(100000);
  }

  if (p_SysCfg->IsStrategyOn(STY_B2_US2))
  {
    styB2_US2.reset(new StrategyB2_US2());
    m_thread_group.add_thread(new boost::thread(&StrategyB2_US2::Run, styB2_US2.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB2_US2"); usleep(100000);
  }

  if (p_SysCfg->IsStrategyOn(STY_B2_US3))
  {
    styB2_US3.reset(new StrategyB2_US3());
    m_thread_group.add_thread(new boost::thread(&StrategyB2_US3::Run, styB2_US3.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB2_US3"); usleep(100000);
  }

  if (p_SysCfg->IsStrategyOn(STY_B2_HK))
  {
    styB2_HK.reset(new StrategyB2_HK());
    m_thread_group.add_thread(new boost::thread(&StrategyB2_HK::Run, styB2_HK.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyB2_HK"); usleep(100000);
  }

  if (p_SysCfg->IsStrategyOn(STY_NIR1))
  {
    styNIR1.reset(new StrategyNIR1());
    m_thread_group.add_thread(new boost::thread(&StrategyNIR1::Run, styNIR1.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyNIR1"); usleep(100000);
  }

  if (p_SysCfg->IsStrategyOn(STY_R7))
  {
    styR7.reset(new StrategyR7());
    m_thread_group.add_thread(new boost::thread(&StrategyR7::Run, styR7.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyR7"); usleep(100000);
  }
  if (p_SysCfg->IsStrategyOn(STY_R9))
  {
    styR9.reset(new StrategyR9());
    m_thread_group.add_thread(new boost::thread(&StrategyR9::Run, styR9.get()));
    p_Logger->Write(Logger::NOTICE,"Started thread: StrategyR9"); usleep(100000);
  }

  // if (p_SysCfg->IsStrategyOn(STY_S11A))
  // {
  //   styS11A.reset(new StrategyS11A());
  //   m_thread_group.add_thread(new boost::thread(&StrategyS11A::Run, styS11A.get()));
  //   p_Logger->Write(Logger::NOTICE,"Started thread: StrategyS11A"); usleep(100000);
  // }
  //
  // if (p_SysCfg->IsStrategyOn(STY_R1))
  // {
  //   styR1.reset(new StrategyR1());
  //   m_thread_group.add_thread(new boost::thread(&StrategyR1::Run, styR1.get()));
  //   p_Logger->Write(Logger::NOTICE,"Started thread: StrategyR1"); usleep(100000);
  // }
  //
  // if (p_SysCfg->IsStrategyOn(STY_R3))
  // {
  //   styR3.reset(new StrategyR3());
  //   m_thread_group.add_thread(new boost::thread(&StrategyR3::Run, styR3.get()));
  //   p_Logger->Write(Logger::NOTICE,"Started thread: StrategyR3"); usleep(100000);
  // }
  //
  // if (p_SysCfg->IsStrategyOn(STY_R8))
  // {
  //   styR8.reset(new StrategyR8());
  //   m_thread_group.add_thread(new boost::thread(&StrategyR8::Run, styR8.get()));
  //   p_Logger->Write(Logger::NOTICE,"Started thread: StrategyR8"); usleep(100000);
  // }


  // if (p_SysCfg->IsStrategyOn(STY_A1))
  // {
  //   styA1.reset(new StrategyA1());
  //   m_thread_group.add_thread(new boost::thread(&StrategyA1::Run, styA1.get()));
  //   p_Logger->Write(Logger::NOTICE,"Started thread: StrategyA1"); usleep(100000);
  // }
  //
  // if (p_SysCfg->IsStrategyOn(STY_A6))
  // {
  //   styA6.reset(new StrategyA6());
  //   m_thread_group.add_thread(new boost::thread(&StrategyA6::Run, styA6.get()));
  //   p_Logger->Write(Logger::NOTICE,"Started thread: StrategyA6"); usleep(100000);
  // }
  //

  {
    pg.reset(new PortfolioGenerator());

    m_thread_group.add_thread(new boost::thread(&VolSurfCalculator::Run          ,&vsc));
    m_thread_group.add_thread(new boost::thread(&TechIndUpdater::Run             ,&tiu));
    m_thread_group.add_thread(new boost::thread(&PriceForwarderToNextTier::Run   ,&pf));
    m_thread_group.add_thread(new boost::thread(&PortfolioGenerator::Run         ,pg.get()));
    // m_thread_group.add_thread(new boost::thread(&TerminalThread::Run          ,&tthd));
    m_thread_group.add_thread(new boost::thread(&MarkToMarket::Run               ,&mtm));
    p_Logger->Write(Logger::NOTICE,"Started thread: VolSurfCalculator");    usleep(100000);
    p_Logger->Write(Logger::NOTICE,"Started thread: TechIndUpdater");       usleep(100000);
    p_Logger->Write(Logger::NOTICE,"Started thread: PortfolioGenerator");   usleep(100000);
    p_Logger->Write(Logger::NOTICE,"Started thread: OrderExecutor");        usleep(100000);
    p_Logger->Write(Logger::NOTICE,"Started thread: TerminalThread");       usleep(100000);
    p_Logger->Write(Logger::NOTICE,"Started thread: MarkToMarket");         usleep(100000);
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
      }
    }
    else
    {
      p_Logger->Write(Logger::NOTICE,"OTI not loaded."); usleep(100000);
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
    p_dataagg.insert(p_dataagg.begin(),iNumOfMDI,boost::shared_ptr<DataAggregator>());
    for (unsigned int i = 0; i < iNumOfMDI; ++i)
    {
      string sFile = p_SysCfg->Get_MDI_File(i);
      string sIP   = p_SysCfg->Get_MDI_IP(i);
      string sPort = p_SysCfg->Get_MDI_Port(i);
      p_Logger->Write(Logger::NOTICE,"Read from SystemConfig: MDI %d: File %s IP %s Port %s",i,sFile.c_str(),sIP.c_str(),sPort.c_str());

      p_dataagg[i].reset(new DataAggregator(i));
      if (sFile == "")
      {
        p_dataagg[i]->SetMDIServer(sIP,sPort);
        m_thread_group.add_thread(new boost::thread(&DataAggregator::Run ,(p_dataagg[i].get())));
      }
      else
      {
        m_thread_group.add_thread(new boost::thread(&DataAggregator::ReadDataFile,(p_dataagg[i].get()),sFile));
      }

      p_Logger->Write(Logger::NOTICE,"Finished loading MDI %d: %s %s",i,sIP.c_str(),sPort.c_str());
      p_Logger->Write(Logger::NOTICE,"Started thread: DataAggregator %d",i);
      usleep(100000);
    }

  }


  m_FinishedInit = true;

  cout << "Nirvana: all threads are started." << endl << flush;
  cout << "Location of log file: " << p_SysCfg->Get_Main_Log_Path() << endl << flush;

  m_thread_group.join_all();

  //--------------------------------------------------
  // Bye
  //--------------------------------------------------
  p_Logger->Write(Logger::NOTICE,"Main: TradingEngine has stopped.");

  return;
}
