#include "TestRsi.h"

TestRsi::TestRsi() {
}

TestRsi::~TestRsi() {
}

// Ans: 6 50.069
// Ans: 12 52.968
void TestRsi::RunTest()
{
  {
    Rsi rsi6(6,true,Rsi::EMA);
    Rsi rsi12(12,true,Rsi::EMA);

    BarProvider bp("./UnitTest/0001-rsi.HK.csv", "DOHLCV", 2, 'F', 'H', 'P');

    bp.ResetPsn();
    while (bp.HasNextBar())
    {
      bp.MoveNext();
      rsi6.Add(bp.Close());
      rsi12.Add(bp.Close());
    }

    CPPUNIT_ASSERT(abs(rsi6.Value() - 50.06) < 0.1);
    CPPUNIT_ASSERT(abs(rsi12.Value() - 52.97) < 0.1);
  }
  {
    Rsi rsi(14,true,Rsi::SMA);

    BarProvider bp("./UnitTest/rsi.csv", "DOHLCV", 4, 'F', 'N', 'P');

    bp.ResetPsn();
    while (bp.HasNextBar())
    {
      bp.MoveNext();
      rsi.Add(bp.Close());
    }

    CPPUNIT_ASSERT(abs(rsi.Value() - 48.3986) < 0.1);
  }

  return;
}

void TestRsi::setUp()
{
}

void TestRsi::tearDown()
{
}

CppUnit::Test *TestRsi::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRsi");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestRsi>(
        "TestRsi", 
        &TestRsi::RunTest));

  return suiteOfTests;
}
