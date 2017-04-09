#include <StrategyB3US.h>

StrategyB3_US::StrategyB3_US() :
  StrategyB3()
{
  m_p_ymdhms_SysTime_Local.reset(&m_ymdhms_SysTime_EST);
  SetStrategyID(STY_B3_US);
  SetStyDomicileMkt(SDM_US);
  SetCloseAllPosIfOutsideTrdgHour(ACP_NO);
}

StrategyB3_US::~StrategyB3_US() {
}
