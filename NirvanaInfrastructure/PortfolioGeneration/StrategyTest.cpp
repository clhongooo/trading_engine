#include <StrategyTest.h>

StrategyTest::StrategyTest() :
  StrategyBase()
{
  SetStrategyID(STY_TEST);
  SetStyDomicileMkt(SDM_HK);
  UnsetConvenienceVarb();
  SetCloseAllPosIfOutsideTrdgHour(ACP_NO);
  m_iNumOfParam = 1;
}


StrategyTest::~StrategyTest()
{
}


//--------------------------------------------------
// Test roll forward - start
//--------------------------------------------------
bool StrategyTest::SkipSubseqProcessingForSymbol(const int iTradSym, string & sReason)
{
  if (m_ymdhms_SysTime_HKT.GetYYYYMMDD().Month() == 5) return true;
  else return false;
}

void StrategyTest::PreTradePreparation(const int iTradSym)
{
  if (m_ymdhms_SysTime_HKT.GetYYYYMMDD().ToInt() == 20130627 && m_ymdhms_SysTime_HKT.GetHHMMSS().ToInt() >= 100500)
  {
    m_TargetTradeDir [iTradSym] = TD_LONG;
    m_TargetAbsPos   [iTradSym] = 3;
  }
  else
  {
    m_TargetTradeDir [iTradSym] = TD_SHORT;
    m_TargetAbsPos   [iTradSym] = 1;
  }
}
//--------------------------------------------------
// Test roll forward - end
//--------------------------------------------------


