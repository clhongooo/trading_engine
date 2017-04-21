#include "TestCountingFunction.h"

TestCountingFunction::TestCountingFunction()
{

}

TestCountingFunction::~TestCountingFunction()
{
}

void TestCountingFunction::setUp()
{
}

void TestCountingFunction::tearDown()
{
}

void TestCountingFunction::RunTest()
{
  CountingFunction cf;

  CPPUNIT_ASSERT(cf.Size()==0);
  //--------------------------------------------------
  // Cnt: 01234567
  // Val: 12334455
  //--------------------------------------------------
  cf.Add(true);
  cf.Add(true);
  cf.Add(true);
  cf.Add(false);
  cf.Add(true);
  cf.Add(false);
  cf.Add(true);
  cf.Add(false);

  CPPUNIT_ASSERT(cf.Count(4,3)==-1);
  CPPUNIT_ASSERT(cf.Count(4,4)==-1);
  CPPUNIT_ASSERT(cf.Count(4,8)==1);
  CPPUNIT_ASSERT(cf.Count(4,7)==1);
  CPPUNIT_ASSERT(cf.Count(0,2)==1);

  CPPUNIT_ASSERT(cf.CountInclusive(4,3)==-1);
  CPPUNIT_ASSERT(cf.CountInclusive(4,4)==0);
  CPPUNIT_ASSERT(cf.CountInclusive(4,8)==-1);
  CPPUNIT_ASSERT(cf.CountInclusive(4,7)==1);
  CPPUNIT_ASSERT(cf.CountInclusive(0,2)==2);

  CPPUNIT_ASSERT(cf.CountLastNElements(2)==0);
  CPPUNIT_ASSERT(cf.CountLastNElements(3)==1);
  CPPUNIT_ASSERT(cf.CountLastNElements(4)==1);

  CPPUNIT_ASSERT(cf.HowManyElemFromTailToAccum(1)==3);
  CPPUNIT_ASSERT(cf.HowManyElemFromTailToAccum(2)==5);
  CPPUNIT_ASSERT(cf.HowManyElemFromTailToAccum(3)==7);

  CPPUNIT_ASSERT(cf.HowManyElemFromTailToAccumOpp(1)==2);
  CPPUNIT_ASSERT(cf.HowManyElemFromTailToAccumOpp(2)==4);
  CPPUNIT_ASSERT(cf.HowManyElemFromTailToAccumOpp(3)==6);

  cf.Reset();
  CPPUNIT_ASSERT(cf.Size()==0);

  return;
}
CppUnit::Test *TestCountingFunction::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestCountingFunction");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestCountingFunction>(
        "TestCountingFunction", 
        &TestCountingFunction::RunTest));

  return suiteOfTests;
}
