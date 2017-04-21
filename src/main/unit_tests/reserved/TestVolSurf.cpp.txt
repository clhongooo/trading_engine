#include "TestVolSurf.h"


double TestVolSurf::GetIVByStrike(double K)
{
  return (
      3 * K * K +
      1 * K  +
      -1
      );
}

TestVolSurf::TestVolSurf() {

}

TestVolSurf::~TestVolSurf() {
}

void TestVolSurf::RunTest()
{
  VolSurf vs;

  // {
  //   double undly = 21999;
  //   vs.UpdateUndlIVByStrike(201406271600,22000,undly,23);
  //   vs.UpdateUndlIVByStrike(201406271600,24000,undly,20);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,24200) == -1);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,24000) == 20);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,23800) == -1);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,22000) == 23);
  //
  //   vs.RemoveIVByStrike(201406271600,24000);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,24200) == -1);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,24000) == -1);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,23800) == -1);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,22000) == 23);
  //
  //   vs.UpdateUndlIVByStrike(201406271600,22000,undly,25);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,22000) == 25);
  //
  //   vs.Reset();
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,22000) == -1);
  //   CPPUNIT_ASSERT(vs.GetIVByStrike(201406271600,24000) == -1);
  // }

  // {
  //   double undly = 21999;
  //   vs.Reset();
  //   for (unsigned int i = 0; i < 4000; i+=200)
  //   {
  //     vs.UpdateUndlIVByStrike(201406271600,20000+i,undly,GetIVByStrike(20000+i));
  //   }
  //
  //   vs.FitVolSmile(201406271600,2);
  //   vector<double> v = vs.GetFittedVolSmileInStrikes(201406271600);
  //   CPPUNIT_ASSERT(v.size() == 3);
  //
  //   CPPUNIT_ASSERT(abs(v[2] - 3) < 0.1);
  //   CPPUNIT_ASSERT(abs(v[1] - 1) < 0.1);
  //   CPPUNIT_ASSERT(abs(v[0] - -1) < 0.1);
  // }

  return;
}

void TestVolSurf::setUp()
{
}

void TestVolSurf::tearDown()
{
}

CppUnit::Test *TestVolSurf::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestVolSurf");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestVolSurf>(
        "TestVolSurf", 
        &TestVolSurf::RunTest));

  return suiteOfTests;
}
