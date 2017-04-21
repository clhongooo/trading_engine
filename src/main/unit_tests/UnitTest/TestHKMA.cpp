#include "TestHKMA.h"

TestHKMA::TestHKMA() {

}

TestHKMA::~TestHKMA() {
}

void TestHKMA::RunTest()
{
  boost::shared_ptr<HKMA> hkma = HKMA::Instance();
  hkma->LoadExchgFundBill("/home/qy/Dropbox/nirvana/conf/HKMA_ExchgFundBill.csv");

  double dRate = 0;

  YYYYMMDD ymd(20130107);
  CPPUNIT_ASSERT(hkma->GetExchgFundBillAnnlzd(ymd,dRate));
  CPPUNIT_ASSERT(dRate == 0.1);

  ymd.Set(20110214);
  CPPUNIT_ASSERT(hkma->GetExchgFundBillAnnlzd(ymd,dRate));
  CPPUNIT_ASSERT(dRate == 0.29);

  return;
}

void TestHKMA::setUp()
{
}

void TestHKMA::tearDown()
{
}

CppUnit::Test *TestHKMA::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestHKMA");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestHKMA>(
        "TestHKMA", 
        &TestHKMA::RunTest));

  return suiteOfTests;
}
