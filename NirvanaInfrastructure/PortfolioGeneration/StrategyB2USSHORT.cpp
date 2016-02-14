#include <StrategyB2USSHORT.h>

StrategyB2_USSHORT::StrategyB2_USSHORT() :
  StrategyB2()
{
  m_p_ymdhms_SysTime_Local.reset(&m_ymdhms_SysTime_EST);
  SetStrategyID(STY_B2_USSHT);
  SetStyDomicileMkt(SDM_US);
  SetCloseAllPosIfOutsideTrdgHour(ACP_NO);
}

StrategyB2_USSHORT::~StrategyB2_USSHORT()
{
}


