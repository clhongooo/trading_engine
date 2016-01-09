#include <StrategyA1.h>

// StrategyA1::StrategyA1()
// {
//   m_MarketData      = MarketData::Instance();
//   m_SystemState     = SystemState::Instance();
//   m_SysCfg          = SystemConfig::Instance();
//   m_Logger          = Logger::Instance();
//   m_TechInd         = TechIndicators::Instance();
//   m_PortAndOrders   = PortfoliosAndOrders::Instance();
//   m_MDIAck          = MDI_Acknowledgement::Instance();
// }
//
// StrategyA1::~StrategyA1() {
// }
//
// void StrategyA1::Run()
// {
//   //--------------------------------------------------
//   // Init path
//   //--------------------------------------------------
//   ofstream fsSignalLog(m_SysCfg->GetSignalLogPath(STY_A1).c_str());
//
//   //--------------------------------------------------
//   // Init other parameters
//   //--------------------------------------------------
//   m_ParamVector = m_SysCfg->GetParamVector(STY_A1);
//
//   //--------------------------------------------------
//   for (;;)
//   {
//     m_MarketData->WaitForData();
//
//     if (m_SystemState->ChkIfThreadShouldStop()) break;
//
//     YYYYMMDDHHMMSS ymdhms_MDITime = m_MarketData->GetSystemTimeHKT();
//
//     if (!m_SysCfg->IsStrategyOn(STY_A1))
//     {
//       ReportAckIfNeeded();
//       m_Logger->Write(Logger::DEBUG,"StrategyA1 off");
//       continue;
//     }
//
//
//     ReportAckIfNeeded();
//   }
//
//   m_Logger->Write(Logger::NOTICE,"StrategyA1 has ended.");
//   sleep(2);
//
//   return;
// }
//
// void StrategyA1::ReportAckIfNeeded()
// {
//   if (m_SysCfg->Get_TCPOrEmbeddedMode() == SystemConfig::TCPWITHACK || m_SysCfg->Get_TCPOrEmbeddedMode() == SystemConfig::EMBEDDED)
//     m_MDIAck->ReportAck(STY_A1);
//   return;
// }
