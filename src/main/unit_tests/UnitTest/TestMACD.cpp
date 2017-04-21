#include "TestMACD.h"

TestMACD::TestMACD() {

}

TestMACD::~TestMACD() {
}

void TestMACD::RunTest()
{
  BarProvider bp("./UnitTest/test-MACD.csv", "OHLCV", 2, 'F', 'H', 'P');
  MACD m(5,13,6,true);

  for (unsigned int i = 0; i < 19; i++)
  {
    if (bp.HasNextBar())
    {
      bp.MoveNext();
      m.Add(bp.Close());
    }
  }

  CPPUNIT_ASSERT(abs(m.MACDVal()-3.13) < 0.1);
  CPPUNIT_ASSERT(abs(m.MACDDiv()-1.239) < 0.1);


  return;
}

void TestMACD::setUp()
{
}

void TestMACD::tearDown()
{
}

CppUnit::Test *TestMACD::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestMACD");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestMACD>(
        "TestMACD", 
        &TestMACD::RunTest));

  return suiteOfTests;
}
