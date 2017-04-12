#include <StrategyB2US2.h>

StrategyB2_US2::StrategyB2_US2() :
  StrategyB2()
{
  m_p_ymdhms_SysTime_Local.reset(&m_ymdhms_SysTime_EST);
  SetStrategyID(STY_B2_US2);
  SetStyDomicileMkt(SDM_US);
  SetCloseAllPosIfOutsideTrdgHour(ACP_NO);
}

StrategyB2_US2::~StrategyB2_US2()
{
}


