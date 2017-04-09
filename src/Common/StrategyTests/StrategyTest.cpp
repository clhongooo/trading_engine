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

bool StrategyTest::SkipSubseqProcessingForSymbol(const int iTradSym, string & sReason)
{
  return false;
}

void StrategyTest::UpdateInternalData(const int iTradSym)
{
  m_Logger->Write(Logger::INFO,"SunnyDebug: %s::%s (%d) %s m_SymMidQuote: %f",__FILE__,__FUNCTION__,__LINE__, m_TradedSymbols[iTradSym].c_str(), m_SymMidQuote);
}

// //--------------------------------------------------
// // Test roll forward - start
// //--------------------------------------------------
// bool StrategyTest::SkipSubseqProcessingForSymbol(const int iTradSym, string & sReason)
// {
//   if (m_ymdhms_SysTime_HKT.GetYYYYMMDD().Month() == 5) return true;
//   else return false;
// }
//
// void StrategyTest::PreTradePreparation(const int iTradSym)
// {
//   if (m_ymdhms_SysTime_HKT.GetYYYYMMDD().ToInt() == 20130627 && m_ymdhms_SysTime_HKT.GetHHMMSS().ToInt() >= 100500)
//   {
//     m_TargetTradeDir [iTradSym] = TD_LONG;
//     m_TargetAbsPos   [iTradSym] = 3;
//   }
//   else
//   {
//     m_TargetTradeDir [iTradSym] = TD_SHORT;
//     m_TargetAbsPos   [iTradSym] = 1;
//   }
// }
// //--------------------------------------------------
// // Test roll forward - end
// //--------------------------------------------------
