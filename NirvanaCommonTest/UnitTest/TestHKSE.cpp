#include "TestHKSE.h"

TestHKSE::TestHKSE() {

}

TestHKSE::~TestHKSE() {
}

void TestHKSE::RunTest()
{
  boost::shared_ptr<HKSE> hkse = HKSE::Instance();
  hkse->LoadHSIConstituents("/home/qy/Dropbox/nirvana/conf/hsi_constit/");

  map<string,double> * m_HSIWeight;
  YYYYMMDD ymd1(201411);
  YYYYMMDD ymd2(20141203);
  YYYYMMDD ymd3(190001);

  m_HSIWeight = hkse->GetHSIConstituents(ymd1);
  CPPUNIT_ASSERT(m_HSIWeight->size() == 50);
  CPPUNIT_ASSERT((*m_HSIWeight)["00939"] == 0.0763);

  m_HSIWeight = hkse->GetHSIConstituents(ymd2);
  CPPUNIT_ASSERT(m_HSIWeight->size() == 50);
  CPPUNIT_ASSERT((*m_HSIWeight)["00939"] == 0.0763);

  m_HSIWeight = hkse->GetHSIConstituents(ymd3);
  CPPUNIT_ASSERT(m_HSIWeight == NULL);

  return;
}

void TestHKSE::setUp()
{
}

void TestHKSE::tearDown()
{
}

CppUnit::Test *TestHKSE::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestHKSE");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestHKSE>(
        "TestHKSE", 
        &TestHKSE::RunTest));

  return suiteOfTests;
}
