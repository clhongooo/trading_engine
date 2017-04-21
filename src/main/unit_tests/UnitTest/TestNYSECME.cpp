#include "TestNYSECME.h"


TestNYSECME::TestNYSECME() {

}

TestNYSECME::~TestNYSECME() {
}

void TestNYSECME::RunTest()
{
  boost::shared_ptr<CME> cme = CME::Instance();

  // CPPUNIT_ASSERT(cme->IsTradingHour(YYYYMMDD(20151108), HHMMSS(120015)));
  // CPPUNIT_ASSERT(cme->IsTradingHour(YYYYMMDD(20151108), HHMMSS(20015)));
  // CPPUNIT_ASSERT(cme->IsTradingHour(YYYYMMDD(20151108), HHMMSS(230015)));

  CPPUNIT_ASSERT(cme->IsAProductInThisMarket("ESM5"));
  CPPUNIT_ASSERT(!cme->IsAProductInThisMarket("EQM5"));
  CPPUNIT_ASSERT(cme->IsAProductInThisMarket("ESc1"));
  CPPUNIT_ASSERT(cme->IsAProductInThisMarket("NQM5"));
  CPPUNIT_ASSERT(cme->IsAProductInThisMarket("YMM5"));


  CPPUNIT_ASSERT(cme->GetContractMultiplier("ESM5")==50);
  CPPUNIT_ASSERT(cme->GetContractMultiplier("NQM5")==20);
  CPPUNIT_ASSERT(cme->GetContractMultiplier("YMM5")==5);
  CPPUNIT_ASSERT(cme->GetContractMultiplier("TFM5")==100);

  CPPUNIT_ASSERT(cme->GetCtrtCode1(CME::FUTURES,YYYYMMDD(20131108),1)=="Z3");
  CPPUNIT_ASSERT(cme->GetCtrtCode1(CME::FUTURES,YYYYMMDD(20131211),1)=="Z3");
  CPPUNIT_ASSERT(cme->GetCtrtCode1(CME::FUTURES,YYYYMMDD(20131212),1)=="H4");
  CPPUNIT_ASSERT(cme->GetCtrtCode1(CME::FUTURES,YYYYMMDD(20131213),1)=="H4");

  CPPUNIT_ASSERT(cme->GetCtrtCode1(CME::FUTURES,YYYYMMDD(20131108),2)=="H4");
  CPPUNIT_ASSERT(cme->GetCtrtCode1(CME::FUTURES,YYYYMMDD(20131211),2)=="H4");
  CPPUNIT_ASSERT(cme->GetCtrtCode1(CME::FUTURES,YYYYMMDD(20131212),2)=="M4");
  CPPUNIT_ASSERT(cme->GetCtrtCode1(CME::FUTURES,YYYYMMDD(20131213),2)=="M4");

  CPPUNIT_ASSERT(cme->GetCtrtCode2(CME::FUTURES,YYYYMMDD(20131108),1)=="Z13");
  CPPUNIT_ASSERT(cme->GetCtrtCode2(CME::FUTURES,YYYYMMDD(20131211),1)=="Z13");
  CPPUNIT_ASSERT(cme->GetCtrtCode2(CME::FUTURES,YYYYMMDD(20131212),1)=="H14");
  CPPUNIT_ASSERT(cme->GetCtrtCode2(CME::FUTURES,YYYYMMDD(20131213),1)=="H14");

  CPPUNIT_ASSERT(cme->GetCtrtCode2(CME::FUTURES,YYYYMMDD(20131108),2)=="H14");
  CPPUNIT_ASSERT(cme->GetCtrtCode2(CME::FUTURES,YYYYMMDD(20131211),2)=="H14");
  CPPUNIT_ASSERT(cme->GetCtrtCode2(CME::FUTURES,YYYYMMDD(20131212),2)=="M14");
  CPPUNIT_ASSERT(cme->GetCtrtCode2(CME::FUTURES,YYYYMMDD(20131213),2)=="M14");


  boost::shared_ptr<NYSE> nyse = NYSE::Instance();
  CPPUNIT_ASSERT(nyse->IsAProductInThisMarket("SPY"));
  CPPUNIT_ASSERT(nyse->IsAProductInThisMarket("JNK"));
  CPPUNIT_ASSERT(!nyse->IsAProductInThisMarket("ESM5"));
  CPPUNIT_ASSERT(!nyse->IsAProductInThisMarket("HSIc1"));
  CPPUNIT_ASSERT(!nyse->IsAProductInThisMarket("HSIM5"));

  // CPPUNIT_ASSERT(!nyse->IsTradingHour(YYYYMMDD(20151108),HHMMSS(3000)));
  // CPPUNIT_ASSERT(!nyse->IsTradingHour(YYYYMMDD(20151108),HHMMSS(83000)));
  // CPPUNIT_ASSERT(nyse->IsTradingHour(YYYYMMDD(20151108),HHMMSS(93100)));
  // CPPUNIT_ASSERT(nyse->IsTradingHour(YYYYMMDD(20151108),HHMMSS(153100)));
  // CPPUNIT_ASSERT(nyse->IsTradingHour(YYYYMMDD(20151108),HHMMSS(163100)));
  // CPPUNIT_ASSERT(!nyse->IsTradingHour(YYYYMMDD(20151108),HHMMSS(173100)));

  return;
}

void TestNYSECME::setUp()
{
}

void TestNYSECME::tearDown()
{
}

CppUnit::Test *TestNYSECME::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestNYSECME");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestNYSECME>(
        "TestNYSECME", 
        &TestNYSECME::RunTest));

  return suiteOfTests;
}
