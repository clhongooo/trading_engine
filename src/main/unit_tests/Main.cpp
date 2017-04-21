#include "UnitTest/ut-ac.hpp"
// #include "UnitTest/ut-atr.hpp"
// #include "UnitTest/ut-bisec.hpp"
// #include "UnitTest/ut-bp.hpp"
// #include "UnitTest/ut-bs.hpp"
// #include "UnitTest/ut-cdq.hpp"
// #include "UnitTest/ut-dlyq.hpp"
// #include "UnitTest/ut-ema.hpp"
// #include "UnitTest/ut-gs.hpp"
// #include "UnitTest/ut-hkfe.hpp"
// #include "UnitTest/ut-hl.hpp"
// #include "UnitTest/ut-lr.hpp"
// #include "UnitTest/ut-obs.hpp"
// #include "UnitTest/ut-re.hpp"
// #include "UnitTest/ut-slb.hpp"
// #include "UnitTest/ut-sm1.hpp"
// #include "UnitTest/ut-sma.hpp"
// #include "UnitTest/ut-snys.hpp"
// #include "UnitTest/ut-sp.hpp"
// #include "UnitTest/ut-st.hpp"
// #include "UnitTest/ut-ts.hpp"
// #include "UnitTest/ut-wma.hpp"
// #include "UnitTest/ut-wr.hpp"
// #include "UnitTest/ut-sm.hpp"
// #include "UnitTest/ut-bm.hpp"
// #include "UnitTest/ut-msm.hpp"
// #include "UnitTest/ut-oblma.hpp"
#include <iostream>

// #include "UnitTest/TestVolSurf.h"
#include "UnitTest/TestProbDistrnFunc.h"
#include "UnitTest/TestProbMassFunc.h"
#include "UnitTest/TestMACD.h"
#include "UnitTest/TestRsi.h"
#include "UnitTest/TestOrderBook.h"
#include "UnitTest/TestLinearSysSolver.h"
#include "UnitTest/TestLeastSqRegression.h"
#include "UnitTest/TestTrdTickAna.h"
#include "UnitTest/TestTickReturn.h"
#include "UnitTest/TestBarAggregator.h"
#include "UnitTest/TestHKMA.h"
#include "UnitTest/TestHKSE.h"
#include "UnitTest/TestGKYZ.h"
#include "UnitTest/TestDaySma.h"
#include "UnitTest/TestPeriodSma.h"
#include "UnitTest/TestDistributions.h"
#include "UnitTest/TestCountingFunction.h"
#include "UnitTest/TestCommissionCalculator.h"
#include "UnitTest/TestCPnLHist.h"
#include "UnitTest/TestKolmogorovSmirnov.h"
#include "UnitTest/TestNYSECME.h"
#include "UnitTest/TestTradingHours.h"
#include "UnitTest/TestPeriodicTask.h"
#include "UnitTest/TestExchange.h"
#include "UnitTest/TestRglrMiniCtrtMgr.h"
#include "UnitTest/TestSFunctional.h"
#include "UnitTest/TestSDateTime.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestResult.h>

using namespace std;

int main ( int argc, const char* argv[] )
{
  cout << "Starting Unit Tests..." << endl;

  // TestAcct();
  // TestATR();
  // TestBisec();
  // TestBP();
  // TestBS();
  // TestCDQ();
  // TestDlydQ();
  // TestSNYS();
  // TestEMA();
  // TestGS();
  // TestHKFE();
  // TestHL();
  // TestLR();
  // TestOBS();
  // TestRE();
  // TestSLB();
  // TestSM1();
  // TestSMA();
  // TestSP();
  // TestST();
  // TestTS();
  // TestWMA();
  // TestWR();
  // TestBM();
  // TestSM();
  // TestMSM();
  // TestOBLMA();

  CppUnit::TextUi::TestRunner utTestRunner;
  // utTestRunner.addTest(TestVolSurf::suite());
  // utTestRunner.addTest(TestTrdTickAna::suite());
  // utTestRunner.addTest(TestTickReturn::suite());
  // utTestRunner.addTest(TestProbDistrnFunc::suite());
  // utTestRunner.addTest(TestProbMassFunc::suite());
  // utTestRunner.addTest(TestMACD::suite());
  // utTestRunner.addTest(TestOrderBook::suite());
  // utTestRunner.addTest(TestRsi::suite());
  // utTestRunner.addTest(TestLinearSysSolver::suite());
  // utTestRunner.addTest(TestLeastSqRegression::suite());
  // utTestRunner.addTest(TestBarAggregator::suite());
  // utTestRunner.addTest(TestHKMA::suite());
  // utTestRunner.addTest(TestHKSE::suite());
  // utTestRunner.addTest(TestGKYZ::suite());
  // utTestRunner.addTest(TestDaySma::suite());
  // utTestRunner.addTest(TestPeriodSma::suite());
  // utTestRunner.addTest(TestDistributions::suite());
  // utTestRunner.addTest(TestCountingFunction::suite());
  // utTestRunner.addTest(TestCommissionCalculator::suite());
  // utTestRunner.addTest(TestCPnLHist::suite());
  // utTestRunner.addTest(TestKolmogorovSmirnov::suite());
  // utTestRunner.addTest(TestNYSECME::suite());
  // utTestRunner.addTest(TestTradingHours::suite());
  // utTestRunner.addTest(TestExchange::suite());
  // utTestRunner.addTest(TestPeriodicTask::suite());
  // utTestRunner.addTest(TestRglrMiniCtrtMgr::suite());
  utTestRunner.addTest(TestSFunctional::suite());
  utTestRunner.addTest(TestSDateTime::suite());
  utTestRunner.run();

  cout << "Finished." << endl;
  return 0;
}
