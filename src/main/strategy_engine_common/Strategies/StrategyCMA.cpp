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

