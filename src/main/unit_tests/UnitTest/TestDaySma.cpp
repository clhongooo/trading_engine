#include "TestDaySma.h"

TestDaySma::TestDaySma() {

}

TestDaySma::~TestDaySma() {
}

void TestDaySma::RunTest()
{
  DaySma ds(10);

  ds.Add(YYYYMMDD(20150130), 811);
  ds.Add(YYYYMMDD(20150201), 467);
  ds.Add(YYYYMMDD(20150203), 797);
  ds.Add(YYYYMMDD(20150205), 675);
  ds.Add(YYYYMMDD(20150207), 62);
  ds.Add(YYYYMMDD(20150208), 340);
  ds.Add(YYYYMMDD(20150209), 95);    CPPUNIT_ASSERT(abs(ds.Value() - 406        ) < 0.1);
  ds.Add(YYYYMMDD(20150211), 313);   CPPUNIT_ASSERT(abs(ds.Value() - 380.3333333) < 0.1);
  ds.Add(YYYYMMDD(20150213), 355);   CPPUNIT_ASSERT(abs(ds.Value() - 306.6666667) < 0.1);
  ds.Add(YYYYMMDD(20150215), 624);   CPPUNIT_ASSERT(abs(ds.Value() - 298.1666667) < 0.1);
  ds.Add(YYYYMMDD(20150216), 969);   CPPUNIT_ASSERT(abs(ds.Value() - 394        ) < 0.1);
  ds.Add(YYYYMMDD(20150218), 684);   CPPUNIT_ASSERT(abs(ds.Value() - 506.6666667) < 0.1);
  ds.Add(YYYYMMDD(20150219), 240);   CPPUNIT_ASSERT(abs(ds.Value() - 530.8333333) < 0.1);
  ds.Add(YYYYMMDD(20150221), 282);   CPPUNIT_ASSERT(abs(ds.Value() - 525.6666667) < 0.1);
  ds.Add(YYYYMMDD(20150223), 163);   CPPUNIT_ASSERT(abs(ds.Value() - 493.6666667) < 0.1);
  ds.Add(YYYYMMDD(20150224), 781);   CPPUNIT_ASSERT(abs(ds.Value() - 534.7142857) < 0.1);
  ds.Add(YYYYMMDD(20150226), 195);   CPPUNIT_ASSERT(abs(ds.Value() - 390.8333333) < 0.1);
  ds.Add(YYYYMMDD(20150227), 789);   CPPUNIT_ASSERT(abs(ds.Value() - 447.7142857) < 0.1);

  return;
}

void TestDaySma::setUp()
{
}

void TestDaySma::tearDown()
{
}

CppUnit::Test *TestDaySma::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestDaySma");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestDaySma>(
        "TestDaySma", 
        &TestDaySma::RunTest));

  return suiteOfTests;
}
