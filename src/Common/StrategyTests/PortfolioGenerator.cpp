#include <PortfolioGenerator.h>

PortfolioGenerator::PortfolioGenerator()
{
  m_MarketData      = MarketData::Instance();
  m_SystemState     = SystemState::Instance();
  m_SysCfg          = SystemConfig::Instance();
  m_Logger          = Logger::Instance();
  m_TechInd         = TechIndicators::Instance();
  m_PortAndOrders   = PortfoliosAndOrders::Instance();
  m_VolSurfaces     = VolSurfaces::Instance();
  m_MDIAck          = MDI_Acknowledgement::Instance();

  m_CalcIntervalInSeconds = 0;
}

PortfolioGenerator::~PortfolioGenerator()
{
}

void PortfolioGenerator::Run()
{
  bool b_placed_ord = false;
  for (;;)
  {
    m_VolSurfaces->WaitForData();

    if (m_SystemState->ChkIfThreadShouldStop()) break;

    YYYYMMDDHHMMSS ymdhms_MDITime = m_MarketData->GetSystemTimeHKT();

    if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK || m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
      m_MDIAck->ReportAck(STY_NIR);

    // //--------------------------------------------------
    // // Portfolio ID 0
    // //--------------------------------------------------
    //
    // if(ymdhms_MDITime.GetHHMMSS().ToHHMMSSi() > 93400 && ymdhms_MDITime.GetHHMMSS().ToHHMMSSi() < 101702)
    //   m_PortAndOrders->SwitchMode(PortfoliosAndOrders::Single_Order);
    //
    // if(ymdhms_MDITime.GetHHMMSS() == HHMMSS(93500) && m_OrderPlacedTime.find(93500) == m_OrderPlacedTime.end()) {
    //   m_OrderPlacedTime.insert(93500);
    //   m_PortAndOrders->Trade(0, "HSIU4"		, 1, OrderInstruction::NONE);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS( 93501) && m_OrderPlacedTime.find(93501) == m_OrderPlacedTime.end()){
    //   m_PortAndOrders->Trade(0,"HSI25000I4"   ,-3, OrderInstruction::NONE);
    //   m_OrderPlacedTime.insert(93501);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS( 94501) && m_OrderPlacedTime.find(94501) == m_OrderPlacedTime.end()) {
    //   m_PortAndOrders->Trade(0,"HSI25200J4"   ,+5, OrderInstruction::NONE);
    //   m_OrderPlacedTime.insert(94501);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS( 95501) && m_OrderPlacedTime.find(95501) == m_OrderPlacedTime.end()) {
    //   m_PortAndOrders->Trade(0,"HSI24600U4"   ,-6, OrderInstruction::NONE);
    //   m_OrderPlacedTime.insert(95501);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS(100501) && m_OrderPlacedTime.find(100501) == m_OrderPlacedTime.end()) {
    //   m_PortAndOrders->Trade(0,"HSI24400V4"   ,2, OrderInstruction::NONE);
    //   m_OrderPlacedTime.insert(100501);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS(101001) && m_OrderPlacedTime.find(101001) == m_OrderPlacedTime.end()) {//101001
    //   m_PortAndOrders->Trade(0,"HSIV4"        ,-3, OrderInstruction::NONE);
    //   m_OrderPlacedTime.insert(101001);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS(101501) && m_OrderPlacedTime.find(101501) == m_OrderPlacedTime.end()){//101501
    //   m_PortAndOrders->Trade(0,"HSIU4"        ,+20, OrderInstruction::NONE);
    //   m_OrderPlacedTime.insert(101501);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS(101701) && m_OrderPlacedTime.find(101701) == m_OrderPlacedTime.end())
    // {
    //
    //   //--------------------------------------------------
    //   // Should have finished establishing portfolio after 2 minutes...
    //   //--------------------------------------------------
    //   m_OrderPlacedTime.insert(101701);
    //
    //   map<string,long> m;
    //   m_PortAndOrders->GetActualPortfolio(0,m);
    //
    //   if (m.size()        !=   6) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL PORTFOLIO size: %d.", __FILE__, __FUNCTION__, __LINE__, m.size());
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  PORTFOLIO SIZE: 6. ACTUAL SIZE: %d.", m.size());
    //   if (m["HSIU4"]      != +21) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL HSIU4: %d. ", __FILE__, __FUNCTION__, __LINE__, m["HSIU4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: 21. ACTUAL HSIU4: %d. ", m["HSIU4"]);
    //   if (m["HSI25000I4"] !=  -3) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL HSI25000I4: %d. ", __FILE__, __FUNCTION__, __LINE__, m["HSI25000I4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: -3. ACTUAL HSI25000I4: %d. ", m["HSI25000I4"]);
    //   if (m["HSI25200J4"] !=  +5) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL HSI25200J4: %d. ", __FILE__, __FUNCTION__, __LINE__, m["HSI25200J4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: 5. ACTUAL HSI25200J4: %d. ", m["HSI25200J4"]);
    //   if (m["HSI24600U4"] !=  -6) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL HSI24600U4: %d. ", __FILE__, __FUNCTION__, __LINE__, m["HSI24600U4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: -6. ACTUAL HSI24600U4: %d. ", m["HSI24600U4"]);
    //   if (m["HSI24400V4"] !=  +2) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL HSI24400V4: %d. ", __FILE__, __FUNCTION__, __LINE__, m["HSI24400V4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: 2. ACTUAL HSI24400V4: %d. ", m["HSI24400V4"]);
    //   if (m["HSIV4"]      !=  -3) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL HSIV4: %d. ", __FILE__, __FUNCTION__, __LINE__, m["HSIV4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: -3. ACTUAL HSIV4: %d. ", m["HSIV4"]);
    //
    // }
    //
    //
    //
    // //--------------------------------------------------
    // // Portfolio ID 1
    // //--------------------------------------------------
    // if(ymdhms_MDITime.GetHHMMSS().ToHHMMSSi() > 113455 && ymdhms_MDITime.GetHHMMSS().ToHHMMSSi() < 114000) //113455 114000
    //   m_PortAndOrders->SwitchMode(PortfoliosAndOrders::Portfolio);
    //
    // if (ymdhms_MDITime.GetHHMMSS() == HHMMSS(113500) && m_OrderPlacedTime.find(113500) == m_OrderPlacedTime.end()) //113500
    // {
    //   m_OrderPlacedTime.insert(113500);
    //
    //   map<string,long> m;
    //   m["HSIV4"] = +1;
    //
    //   m["HSI25400I4"] = -2;
    //   m["HSI25600I4"] = +2;
    //   m["HSI24800U4"] = +2;
    //   m["HSI24800V4"] = +2;
    //   m["HSI24600U4"] = +5;
    //
    //
    //   /*
    //      if(!m_PortAndOrders->IsOrderSetEmpty(1)){
    //      m_PortAndOrders->ClearWorkingOrder();
    //      m_PortAndOrders->WaitForPortfolio();
    //      }
    //    */
    //   m_PortAndOrders->SetTargetPortfolio(1,m);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS(113520) && m_OrderPlacedTime.find(113520) == m_OrderPlacedTime.end()) //113520
    // {
    //   m_OrderPlacedTime.insert(113520);
    //
    //   map<string,long> m;
    //   m["HSIV4"] = -1;
    //
    //   m["HSI25400I4"] = +2;
    //   m["HSI25600I4"] = -2;
    //   m["HSI24800U4"] = -2;
    //   m["HSI24800V4"] = -2;
    //   m["HSI24600U4"] = -5;
    //
    //
    //   /*
    //      if(!m_PortAndOrders->IsOrderSetEmpty(1)){
    //      m_PortAndOrders->ClearWorkingOrder();
    //      m_PortAndOrders->WaitForPortfolio();
    //      }
    //    */
    //   //m_PortAndOrders->SetTargetPortfolio(1,m);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS(113540) && m_OrderPlacedTime.find(113540) == m_OrderPlacedTime.end()) //113540
    // {
    //   m_OrderPlacedTime.insert(113540);
    //   //cout << " 113519 " << endl;
    //
    //   map<string,long> m;
    //
    //   m["HSIV4"] = 1;
    //
    //   m["HSI25400I4"] = +2;
    //   m["HSI25600I4"] = +2;
    //   m["HSI24800U4"] = -1;
    //   m["HSI24800V4"] = -1;
    //   m["HSI24600U4"] = -4;
    //
    //
    //   /*
    //      if(!m_PortAndOrders->IsOrderSetEmpty(1)){
    //      m_PortAndOrders->ClearWorkingOrder();
    //      cout << "before cv ..." << endl;
    //      m_PortAndOrders->WaitForPortfolio();
    //      cout << "after cv ..." << endl;
    //      }
    //    */
    //   m_PortAndOrders->SetTargetPortfolio(1,m);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS(113900) && m_OrderPlacedTime.find(113900) == m_OrderPlacedTime.end()) //113900
    // {
    //   m_OrderPlacedTime.insert(113900);
    //   //--------------------------------------------------
    //   // Should have finished establishing portfolio after 2 minutes...
    //   //--------------------------------------------------
    //   map<string,long> m, m2;
    //   m_PortAndOrders->GetActualPortfolio(1,m);
    //   m_PortAndOrders->GetTargetPortfolio(1, m2);
    //
    //   // m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL PORTFOLIO size: %d.", __FILE__, __FUNCTION__, __LINE__, m.size());
    //   if (m.size()        !=  6) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL PORTFOLIO size: %d.", __FILE__, __FUNCTION__, __LINE__, m.size());
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  PORTFOLIO SIZE: 6. ACTUAL SIZE: %d.", m.size());
    //   if (m["HSIV4"]      != +1) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. TARGET: %d. ACTUAL HSIV4: %d.", __FILE__, __FUNCTION__, __LINE__, m2["HSIV4"], m["HSIV4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: 1. ACTUAL HSIV4: %d. ", m["HSIV4"]);
    //   if (m["HSI25400I4"] != +2) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. TARGET: %d. ACTUAL HSI25400I4: %d.", __FILE__, __FUNCTION__, __LINE__, m2["HSI25400I4"], m["HSI25400I4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: 2. ACTUAL HSI25400I4: %d. ", m["HSI25400I4"]);
    //   if (m["HSI25600I4"] != +2) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. TARGET: %d. ACTUAL HSI25600I4: %d.", __FILE__, __FUNCTION__, __LINE__, m2["HSI25600I4"], m["HSI25600I4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: 2. ACTUAL HSI25600I4: %d. ", m["HSI25600I4"]);
    //   if (m["HSI24800U4"] != -1) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. TARGET: %d. ACTUAL HSI24800U4: %d.", __FILE__, __FUNCTION__, __LINE__, m2["HSI24800U4"], m["HSI24800U4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: -1. ACTUAL HSI24800U4: %d. ", m["HSI24800U4"]);
    //   if (m["HSI24800V4"] != -1) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. TARGET: %d. ACTUAL HSI24800V4: %d.", __FILE__, __FUNCTION__, __LINE__, m2["HSI24800V4"], m["HSI24800V4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: -1. ACTUAL HSI24800V4: %d. ", m["HSI24800V4"]);
    //   if (m["HSI24600U4"] != -4) m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. TARGET: %d. ACTUAL HSI24600U4: %d.", __FILE__, __FUNCTION__, __LINE__, m2["HSI24600U4"], m["HSI24600U4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: -4. ACTUAL HSI24600U4: %d. ", m["HSI24600U4"]);
    //
    // }
    //
    // //--------------------------------------------------
    // // Portfolio ID 2
    // //--------------------------------------------------
    // if(ymdhms_MDITime.GetHHMMSS().ToHHMMSSi() > 133449 && ymdhms_MDITime.GetHHMMSS().ToHHMMSSi() < 143505)
    //   m_PortAndOrders->SwitchMode(PortfoliosAndOrders::Single_Order);
    //
    // if(ymdhms_MDITime.GetHHMMSS() == HHMMSS(133501) && m_OrderPlacedTime.find(133501) == m_OrderPlacedTime.end()){
    //   m_OrderPlacedTime.insert(133501);
    //   m_PortAndOrders->Trade(2,"HSIU4"		,+20,  	OrderInstruction::NONE);
    // }
    // else if(ymdhms_MDITime.GetHHMMSS() == HHMMSS(134001) && m_OrderPlacedTime.find(134001) == m_OrderPlacedTime.end()){
    //   m_OrderPlacedTime.insert(134001);
    //   m_PortAndOrders->Trade(2,"HSI25000I4"   ,-100,  OrderInstruction::NONE);
    // }
    // else if(ymdhms_MDITime.GetHHMMSS() == HHMMSS(134501) && m_OrderPlacedTime.find(134501) == m_OrderPlacedTime.end()){
    //   m_OrderPlacedTime.insert(134501);
    //   m_PortAndOrders->Trade(2,"HSI25200J4"   ,+200,  OrderInstruction::NONE);
    // }
    // else if(ymdhms_MDITime.GetHHMMSS() == HHMMSS(135501) && m_OrderPlacedTime.find(135501) == m_OrderPlacedTime.end()){
    //   m_OrderPlacedTime.insert(135501);
    //   m_PortAndOrders->Trade(2,"HSI24600U4"   ,-70,   OrderInstruction::NONE);
    // }
    // else if(ymdhms_MDITime.GetHHMMSS() == HHMMSS(140501) && m_OrderPlacedTime.find(140501) == m_OrderPlacedTime.end()){
    //   m_OrderPlacedTime.insert(140501);
    //   m_PortAndOrders->Trade(2,"HSI24400V4"   ,+100,  OrderInstruction::NONE);
    // }
    // else if(ymdhms_MDITime.GetHHMMSS() == HHMMSS(141001) && m_OrderPlacedTime.find(141001) == m_OrderPlacedTime.end()){
    //   m_OrderPlacedTime.insert(141001);
    //   m_PortAndOrders->Trade(2,"HSIV4"        ,-25,   OrderInstruction::NONE);
    // }
    // else if(ymdhms_MDITime.GetHHMMSS() == HHMMSS(141501) && m_OrderPlacedTime.find(141501) == m_OrderPlacedTime.end()){
    //   m_OrderPlacedTime.insert(141501);
    //   m_PortAndOrders->Trade(2,"HSIU4"        ,+100,  OrderInstruction::NONE);
    // }
    // else if (ymdhms_MDITime.GetHHMMSS() == HHMMSS(143501) && m_OrderPlacedTime.find(143501) == m_OrderPlacedTime.end())
    // {
    //
    //   m_OrderPlacedTime.insert(143501);
    //
    //   //--------------------------------------------------
    //   // Should have finished establishing portfolio after 20 minutes...
    //   //--------------------------------------------------
    //   map<string,long> m;
    //   m_PortAndOrders->GetActualPortfolio(2,m);
    //
    //   //m_Logger->Write(Logger::ERROR,"TEST CASE FAILED at %s::%s line %d. ACTUAL HSI24600U4: %d.", __FILE__, __FUNCTION__, __LINE__, m["HSI24600U4"]);
    //   if (m.size()        !=  6 ) m_Logger->Write(Logger::ERROR,"PortfolioGenerator: TEST CASE FAILED at %s::%s line %d. ACTUAL PORTFOLIO size: %d.", __FILE__, __FUNCTION__, __LINE__, m.size());
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  PORTFOLIO SIZE: 6. ACTUAL SIZE: %d.", m.size());
    //   if (m["HSIU4"]      != +120) m_Logger->Write(Logger::ERROR,"PortfolioGenerator: TEST CASE FAILED at %s::%s line %d. ACTUAL HSIU4: %d.", __FILE__, __FUNCTION__, __LINE__, m["HSIU4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: 120. ACTUAL HSIU4: %d. ", m["HSIU4"]);
    //   if (m["HSI25000I4"] != -100) m_Logger->Write(Logger::ERROR,"PortfolioGenerator: TEST CASE FAILED at %s::%s line %d. ACTUAL HSI25000I4: %d.", __FILE__, __FUNCTION__, __LINE__, m["HSI25000I4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: -100. ACTUAL HSI25000I4: %d. ", m["HSI25000I4"]);
    //   if (m["HSI25200J4"] != +200) m_Logger->Write(Logger::ERROR,"PortfolioGenerator: TEST CASE FAILED at %s::%s line %d. ACTUAL HSI25200J4: %d.", __FILE__, __FUNCTION__, __LINE__, m["HSI25200J4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: 200. ACTUAL HSI25200J4: %d. ", m["HSI25200J4"]);
    //   if (m["HSI24600U4"] != -70 ) m_Logger->Write(Logger::ERROR,"PortfolioGenerator: TEST CASE FAILED at %s::%s line %d. ACTUAL HSI24600U4: %d.", __FILE__, __FUNCTION__, __LINE__, m["HSI24600U4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: -70. ACTUAL HSI24600U4: %d. ", m["HSI24600U4"]);
    //   if (m["HSI24400V4"] != +100) m_Logger->Write(Logger::ERROR,"PortfolioGenerator: TEST CASE FAILED at %s::%s line %d. ACTUAL HSI24400V4: %d.", __FILE__, __FUNCTION__, __LINE__, m["HSI24400V4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: 100. ACTUAL HSI24400V4: %d. ", m["HSI24400V4"]);
    //   if (m["HSIV4"]      != -25 ) m_Logger->Write(Logger::ERROR,"PortfolioGenerator: TEST CASE FAILED at %s::%s line %d. ACTUAL HHIV4: %d.", __FILE__, __FUNCTION__, __LINE__, m["HHIV4"]);
    //   else m_Logger->Write(Logger::INFO, "TEST CASE PASSED.  TARGET PORTFOLIO: -25. ACTUAL HSIV4: %d. ", m["HSIV4"]);
    //
    // }


  }

  m_Logger->Write(Logger::NOTICE,"PortfolioGenerator has ended.");
  sleep(2);

  return;
}
