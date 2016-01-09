#include <StrategyB2USSTK.h>

StrategyB2_USSTK::StrategyB2_USSTK() :
  StrategyB2()
{
  m_p_ymdhms_SysTime_Local.reset(&m_ymdhms_SysTime_EST);
  SetStrategyID(STY_B2_USSTK);
  SetStyDomicileMkt(SDM_US);
  SetCloseAllPosIfOutsideTrdgHour(ACP_NO);
}

StrategyB2_USSTK::~StrategyB2_USSTK()
{
}


