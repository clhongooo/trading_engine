#include <StrategyETFR.h>

// StrategyETFR::StrategyETFR()
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
// StrategyETFR::~StrategyETFR()
// {
// }
//
// void StrategyETFR::Run()
// {
//   //--------------------------------------------------
//   // Init path
//   //--------------------------------------------------
//   ofstream fsSignalLog(m_SysCfg->GetSignalLogPath(STY_ETFR).c_str());
//
//   //--------------------------------------------------
//   // Init other parameters
//   //--------------------------------------------------
//   m_SamplingIntervalInSec = 1800;
//   m_LastMonitoredTime.Set(m_MarketData->GetSystemTimeHKT());
//
//   //--------------------------------------------------
//   for (;;)
//   {
//     m_TechInd->WaitForData();
//
//     if (m_SystemState->ChkIfThreadShouldStop()) break;
//
//     YYYYMMDDHHMMSS ymdhms_MDITime = m_MarketData->GetSystemTimeHKT();
//
//     if (m_SysCfg->IsStrategyOn(STY_ETFR))
//     {
//     }
//
//     ReportAckIfNeeded();
//   }
//
//   m_Logger->Write(Logger::NOTICE,"StrategyETFR has ended.");
//   sleep(2);
//
//   return;
// }
//
// void StrategyETFR::ReportAckIfNeeded()
// {
//   if (m_SysCfg->Get_TCPOrEmbeddedMode() == SystemConfig::TCPWITHACK || m_SysCfg->Get_TCPOrEmbeddedMode() == SystemConfig::EMBEDDED)
//     m_MDIAck->ReportAck(STY_ETFR);
//   return;
// }
