#include "TestCommissionCalculator.h"

TestCommissionCalculator::TestCommissionCalculator() {
	// TODO Auto-generated constructor stub

}

TestCommissionCalculator::~TestCommissionCalculator() {
	// TODO Auto-generated destructor stub
}

void TestCommissionCalculator::RunTest()
{
  CPPUNIT_ASSERT(CommissionCalculator::CalcCommission(CommissionCalculator::IB,CommissionCalculator::USSTK,CommissionCalculator::SPOT,25,100) == 1);
  CPPUNIT_ASSERT(CommissionCalculator::CalcCommission(CommissionCalculator::IB,CommissionCalculator::USSTK,CommissionCalculator::SPOT,25,1000) == 5);
  CPPUNIT_ASSERT(CommissionCalculator::CalcCommission(CommissionCalculator::IB,CommissionCalculator::USSTK,CommissionCalculator::SPOT,0.25,1000) == 1.25);
  CPPUNIT_ASSERT(CommissionCalculator::CalcCommission(CommissionCalculator::IB,CommissionCalculator::USSTK,CommissionCalculator::SPOT,0.1,100) == 1);

  CPPUNIT_ASSERT( CommissionCalculator::GetTrdgUnitConsidergMinComm(CommissionCalculator::IB,CommissionCalculator::USSTK,CommissionCalculator::SPOT) == 200);

  return;
}

void TestCommissionCalculator::setUp()
{
}

void TestCommissionCalculator::tearDown()
{
}

CppUnit::Test *TestCommissionCalculator::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestCommissionCalculator");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestCommissionCalculator>(
        "TestCommissionCalculator", 
        &TestCommissionCalculator::RunTest));

  return suiteOfTests;
}

