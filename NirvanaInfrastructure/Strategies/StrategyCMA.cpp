#include <StrategyCMA.h>

StrategyCMA::StrategyCMA()
{
  m_MarketData      = MarketData::Instance();
  m_SystemState     = SystemState::Instance();
  m_SysCfg          = SystemConfig::Instance();
  m_Logger          = Logger::Instance();
  m_TechInd         = TechIndicators::Instance();
  m_MDIAck          = MDI_Acknowledgement::Instance();

  SetStrategyID(STY_CMA);
  SetStyDomicileMkt(SDM_HK);
  SetCloseAllPosIfOutsideTrdgHour(ACP_NO);
}

StrategyCMA::~StrategyCMA()
{
}

void StrategyCMA::Run()
{
  //--------------------------------------------------
  // Init path
  //--------------------------------------------------
  ofstream fsSignalLog(m_SysCfg->GetSignalLogPath(STY_CMA).c_str());

  //--------------------------------------------------
  // Init other parameters
  //--------------------------------------------------
  m_SamplingIntervalInSec = 1800;
  m_LastMonitoredTime.Set(m_MarketData->GetSystemTimeHKT());

  //--------------------------------------------------
  for (;;)
  {
    m_TechInd->WaitForData();

    if (m_SystemState->ChkIfThreadShouldStop()) break;

    YYYYMMDDHHMMSS ymdhms_MDITime = m_MarketData->GetSystemTimeHKT();

    if (m_SysCfg->IsStrategyOn(STY_CMA))
    {
    }

    ReportAckIfNeeded();
  }

  m_Logger->Write(Logger::NOTICE,"StrategyCMA has ended.");
  sleep(2);

  return;
}
