#include <StrategyB2HK.h>

StrategyB2_HK::StrategyB2_HK() :
  StrategyB2()
{
  m_p_ymdhms_SysTime_Local.reset(&m_ymdhms_SysTime_HKT);
  SetStrategyID(STY_B2_HK);
  SetStyDomicileMkt(SDM_HK);
  SetCloseAllPosIfOutsideTrdgHour(ACP_NO);
}

StrategyB2_HK::~StrategyB2_HK()
{
}


