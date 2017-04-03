#include <StrategyR1.h>

// StrategyR1::StrategyR1()
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
// StrategyR1::~StrategyR1() {
// }
//
// void StrategyR1::Run()
// {
//   //--------------------------------------------------
//   // Init path
//   //--------------------------------------------------
//   ofstream fsSignalLog(m_SysCfg->GetSignalLogPath(STY_R1).c_str());
//
//   //--------------------------------------------------
//   // Init other parameters
//   //--------------------------------------------------
//   m_ParamVector = m_SysCfg->GetParamVector(STY_R1);
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
//     if (m_SysCfg->IsStrategyOn(STY_R1))
//     {
//         m_Logger->Write(Logger::INFO,"SunnyDebug: %s::%s (%d)",__FILE__,__FUNCTION__,__LINE__);
//     }
//
//     ReportAckIfNeeded();
//   }
//
//   m_Logger->Write(Logger::NOTICE,"StrategyR1 has ended.");
//   sleep(2);
//
//   return;
// }
//
// void StrategyR1::ReportAckIfNeeded()
// {
//   if (m_SysCfg->Get_TCPOrEmbeddedMode() == SystemConfig::TCPWITHACK || m_SysCfg->Get_TCPOrEmbeddedMode() == SystemConfig::EMBEDDED)
//     m_MDIAck->ReportAck(STY_R1);
//   return;
// }
