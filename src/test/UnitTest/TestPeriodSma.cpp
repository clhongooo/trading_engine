#include "TestPeriodSma.h"

TestPeriodSma::TestPeriodSma() {

}

TestPeriodSma::~TestPeriodSma() {
}

void TestPeriodSma::RunTest()
{
  PeriodSma ps(24*60*60);

  ps.Add(YYYYMMDDHHMMSS(20150130,  80031), 94 );   CPPUNIT_ASSERT(abs(ps.Value() - 94          ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150130, 103011), 716);   CPPUNIT_ASSERT(abs(ps.Value() - 405         ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150130, 125991), 494);   CPPUNIT_ASSERT(abs(ps.Value() - 434.6666667 ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150130, 148971), 22 );   CPPUNIT_ASSERT(abs(ps.Value() - 331.5       ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150130, 171951), 353);   CPPUNIT_ASSERT(abs(ps.Value() - 335.8       ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150131,  90135), 946);   CPPUNIT_ASSERT(abs(ps.Value() - 506.2       ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150131, 115731), 674);   CPPUNIT_ASSERT(abs(ps.Value() - 497.8       ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150131, 141327), 842);   CPPUNIT_ASSERT(abs(ps.Value() - 567.4       ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150131, 165023), 591);   CPPUNIT_ASSERT(abs(ps.Value() - 681.2       ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150201,  70511), 11 );   CPPUNIT_ASSERT(abs(ps.Value() - 612.8       ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150201,  83348), 211);   CPPUNIT_ASSERT(abs(ps.Value() - 545.8333333 ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150201, 102131), 572);   CPPUNIT_ASSERT(abs(ps.Value() - 483.5       ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150201, 120914), 471);   CPPUNIT_ASSERT(abs(ps.Value() - 449.6666667 ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150201, 130617), 134);   CPPUNIT_ASSERT(abs(ps.Value() - 404.5714286 ) < 0.1);
  ps.Add(YYYYMMDDHHMMSS(20150201, 152450), 740);   CPPUNIT_ASSERT(abs(ps.Value() - 390         ) < 0.1);

  return;
}

void TestPeriodSma::setUp()
{
}

void TestPeriodSma::tearDown()
{
}

CppUnit::Test *TestPeriodSma::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestPeriodSma");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestPeriodSma>(
        "TestPeriodSma", 
        &TestPeriodSma::RunTest));

  return suiteOfTests;
}
