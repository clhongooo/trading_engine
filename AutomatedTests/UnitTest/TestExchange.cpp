#include "TestExchange.h"

TestExchange::TestExchange() {

}

TestExchange::~TestExchange() {
}

void TestExchange::RunTest()
{

  //--------------------------------------------------
  // IsNewDay
  //--------------------------------------------------
  boost::shared_ptr<Exchange> pExchg = Exchange::Instance();

  {
    YYYYMMDDHHMMSS ymdLast("2015-10-17","14:10:09");
    YYYYMMDDHHMMSS ymdCurrent("2015-10-17","14:11:09");
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HSI",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HHI",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HSIV5",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HHI10000X5",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("939",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("00939",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("02823",ymdCurrent,ymdLast));
  }
  {
    YYYYMMDDHHMMSS ymdLast("2015-10-17","11:59:59");
    YYYYMMDDHHMMSS ymdCurrent("2015-10-17","13:00:09");
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HSI",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HHI",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HSIV5",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HHI10000X5",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("939",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("00939",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("02823",ymdCurrent,ymdLast));
  }
  {
    YYYYMMDDHHMMSS ymdLast("2015-10-17","09:15:59");
    YYYYMMDDHHMMSS ymdCurrent("2015-10-17","16:14:09");
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HSI",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HHI",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HSIV5",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("HHI10000X5",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("939",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("00939",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("02823",ymdCurrent,ymdLast));
  }
  {
    YYYYMMDDHHMMSS ymdLast("2015-10-16","15:59:59");
    YYYYMMDDHHMMSS ymdCurrent("2015-10-17","09:30:01");
    CPPUNIT_ASSERT(pExchg->IsNewDay("HSI",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(pExchg->IsNewDay("HHI",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(pExchg->IsNewDay("HSIV5",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(pExchg->IsNewDay("HHI10000X5",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(pExchg->IsNewDay("939",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(pExchg->IsNewDay("00939",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(pExchg->IsNewDay("02823",ymdCurrent,ymdLast));
  }
  {
    YYYYMMDDHHMMSS ymdLast("2015-10-16","03:59:59");
    YYYYMMDDHHMMSS ymdCurrent("2015-10-16","09:30:01");
    CPPUNIT_ASSERT(pExchg->IsNewDay("SPY",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(pExchg->IsNewDay("VXX",ymdCurrent,ymdLast));
  }
  {
    YYYYMMDDHHMMSS ymdLast("2015-10-15","23:59:59");
    YYYYMMDDHHMMSS ymdCurrent("2015-10-16","09:30:01");
    CPPUNIT_ASSERT(pExchg->IsNewDay("SPY",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(pExchg->IsNewDay("VXX",ymdCurrent,ymdLast));
  }
  {
    YYYYMMDDHHMMSS ymdLast("2015-10-15","23:59:59");
    YYYYMMDDHHMMSS ymdCurrent("2015-10-16","04:30:01");
    CPPUNIT_ASSERT(!pExchg->IsNewDay("SPY",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("VXX",ymdCurrent,ymdLast));
  }
  {
    YYYYMMDDHHMMSS ymdCurrent("2015-10-16","23:59:59");
    YYYYMMDDHHMMSS ymdLast("2015-10-17","00:00:01");
    CPPUNIT_ASSERT(!pExchg->IsNewDay("SPY",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("VXX",ymdCurrent,ymdLast));
  }
  {
    YYYYMMDDHHMMSS ymdCurrent("2015-10-16","09:30:01");
    YYYYMMDDHHMMSS ymdLast("2015-10-17","04:00:00");
    CPPUNIT_ASSERT(pExchg->IsNewDay("SPY",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(pExchg->IsNewDay("VXX",ymdCurrent,ymdLast));
  }
  {
    YYYYMMDDHHMMSS ymdCurrent("2015-10-17","21:30:01");
    YYYYMMDDHHMMSS ymdLast("2015-10-18","04:00:00");
    CPPUNIT_ASSERT(!pExchg->IsNewDay("SPY",ymdCurrent,ymdLast));
    CPPUNIT_ASSERT(!pExchg->IsNewDay("VXX",ymdCurrent,ymdLast));
  }
  //--------------------------------------------------
  {
    pExchg->LoadTradingHours("./UnitTest/trading_hours.txt");
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("HSI",YYYYMMDD("2013-04-08"),5*60) == HHMMSS(155500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("HSI",YYYYMMDD("1900-01-01"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("HSI",YYYYMMDD("2015-12-31"),5*60) == HHMMSS(155500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("BOGUS",YYYYMMDD("2013-04-08"),5*60) == HHMMSS(152500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("BOGUS",YYYYMMDD("1900-01-01"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("BOGUS",YYYYMMDD("2015-12-31"),5*60) == HHMMSS(152500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("BOGUS2",YYYYMMDD("2013-04-08"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("BOGUS2",YYYYMMDD("1900-01-01"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("BOGUS2",YYYYMMDD("2015-12-31"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("BOGUSUS",YYYYMMDD("2013-04-08"),5*60) == HHMMSS(35500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("BOGUSUS",YYYYMMDD("1900-01-01"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecBefClose("BOGUSUS",YYYYMMDD("2015-12-31"),5*60) == HHMMSS(35500));
  }
  {
    pExchg->LoadTradingHours("./UnitTest/trading_hours.txt");
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("HSI",YYYYMMDD("2013-04-08"),5*60) == HHMMSS(93500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("HSI",YYYYMMDD("1900-01-01"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("HSI",YYYYMMDD("2015-12-31"),5*60) == HHMMSS(93500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("BOGUS",YYYYMMDD("2013-04-08"),5*60) == HHMMSS(100500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("BOGUS",YYYYMMDD("1900-01-01"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("BOGUS",YYYYMMDD("2015-12-31"),5*60) == HHMMSS(100500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("BOGUS2",YYYYMMDD("2013-04-08"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("BOGUS2",YYYYMMDD("1900-01-01"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("BOGUS2",YYYYMMDD("2015-12-31"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("BOGUSUS",YYYYMMDD("2013-04-08"),5*60) == HHMMSS(93500));
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("BOGUSUS",YYYYMMDD("1900-01-01"),5*60) == HHMMSS());
    CPPUNIT_ASSERT(pExchg->GetTimeNSecAftOpen("BOGUSUS",YYYYMMDD("2015-12-31"),5*60) == HHMMSS(93500));
  }
  //--------------------------------------------------


  return;
}

void TestExchange::setUp()
{
}

void TestExchange::tearDown()
{
}

CppUnit::Test *TestExchange::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestExchange");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestExchange>(
      "TestExchange", 
      &TestExchange::RunTest));

  return suiteOfTests;
}
