#include "TestTradingHours.h"

TestTradingHours::TestTradingHours() {
}

TestTradingHours::~TestTradingHours() {
}

void TestTradingHours::RunTest()
{
  {
    TradingHours * pth = NULL;
    for (unsigned int iTestCase = 0; iTestCase < 3; ++iTestCase)
    {
      switch(iTestCase)
      {
        case 0: { pth = new TradingHours(                          ); break; }
        case 1: { pth = new TradingHours(TradingHours::DFT_DISALLOW); break; }
        case 2: { pth = new TradingHours(TradingHours::DFT_ALLOW   ); break; }
      }

      TradingHours & th = *pth;

      //--------------------------------------------------
      // Config file
      //--------------------------------------------------
      CPPUNIT_ASSERT(!th.IsCfgFileLoaded());
      th.ReadConfig("./UnitTest/trading_hours.txt");
      CPPUNIT_ASSERT(th.IsCfgFileLoaded());

      //--------------------------------------------------
      // Non-existent symbol
      //--------------------------------------------------
      if (iTestCase == 0 || iTestCase == 1)
      {
        CPPUNIT_ASSERT(!th.IsTradingHour("FUCK", YYYYMMDD(20150113),HHMMSS(101010)));
      }
      else if (iTestCase == 2)
      {
        CPPUNIT_ASSERT(th.IsTradingHour("FUCK", YYYYMMDD(20150113),HHMMSS(101010)));
      }

      //--------------------------------------------------
      // Ancient dates
      //--------------------------------------------------
      if (iTestCase == 0 || iTestCase == 1)
      {
        CPPUNIT_ASSERT(!th.IsTradingHour("HSIF3",YYYYMMDD(19900105),HHMMSS( 81010)));
        CPPUNIT_ASSERT(!th.IsTradingHour("HSIF3",YYYYMMDD(19900105),HHMMSS(101010)));
        CPPUNIT_ASSERT(!th.IsTradingHour("HSIF3",YYYYMMDD(19900105),HHMMSS(191010)));
      }
      else if (iTestCase == 2)
      {
        CPPUNIT_ASSERT(th.IsTradingHour("HSIF3",YYYYMMDD(19900105),HHMMSS( 81010)));
        CPPUNIT_ASSERT(th.IsTradingHour("HSIF3",YYYYMMDD(19900105),HHMMSS(101010)));
        CPPUNIT_ASSERT(th.IsTradingHour("HSIF3",YYYYMMDD(19900105),HHMMSS(191010)));
      }

      //--------------------------------------------------
      // 20130408
      //--------------------------------------------------
      CPPUNIT_ASSERT( th.IsTradingHour("HSIJ3",YYYYMMDD(20130408),HHMMSS(101010)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIJ3",YYYYMMDD(20130408),HHMMSS(171010)));

      //--------------------------------------------------
      // Soon after 20130408
      //--------------------------------------------------
      CPPUNIT_ASSERT( th.IsTradingHour("HSIJ3",YYYYMMDD(20130410),HHMMSS(101010)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIJ3",YYYYMMDD(20130410),HHMMSS(171010)));

      //--------------------------------------------------
      // Normal cases
      //--------------------------------------------------
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS( 81010)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS( 91010)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS( 91359)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS( 91501)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(101010)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(115959)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(120001)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(131001)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(141010)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(151010)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(161010)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(161459)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(161501)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(161600)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(171010)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIF5",YYYYMMDD(20150113),HHMMSS(181010)));

      //--------------------------------------------------
      // Normal cases
      //--------------------------------------------------
      CPPUNIT_ASSERT( th.IsTradingHour("HSIJ3",YYYYMMDD(20130418),HHMMSS(101010)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIJ3",YYYYMMDD(20130419),HHMMSS(101010)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIJ3",YYYYMMDD(20130420),HHMMSS(101010)));
      CPPUNIT_ASSERT(!th.IsTradingHour("HSIJ3",YYYYMMDD(20130421),HHMMSS(101010)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIJ3",YYYYMMDD(20130422),HHMMSS(101010)));
      CPPUNIT_ASSERT( th.IsTradingHour("HSIJ3",YYYYMMDD(20130423),HHMMSS(101010)));

      if (pth) delete pth;
    }

  }
  {
    TradingHours th;
    th.ReadConfig("./UnitTest/trading_hours.txt");

    CPPUNIT_ASSERT(!th.IsTradingHourStartEndBreakBuffer("HSIZ5",YYYYMMDD(20150418),HHMMSS(101000),3600,0,0));
    CPPUNIT_ASSERT(!th.IsTradingHourStartEndBreakBuffer("HSIZ5",YYYYMMDD(20150418),HHMMSS(101000),0,0,3600));
    CPPUNIT_ASSERT(!th.IsTradingHourStartEndBreakBuffer("HSIZ5",YYYYMMDD(20150418),HHMMSS(152000),0,3600,0));
    CPPUNIT_ASSERT(!th.IsTradingHourStartEndBreakBuffer("HSIZ5",YYYYMMDD(20150418),HHMMSS(152000),0,0,3600));

    CPPUNIT_ASSERT( th.IsTradingHourStartEndBreakBuffer("HSIZ5",YYYYMMDD(20150418),HHMMSS(102000),3600,0,0));
    CPPUNIT_ASSERT( th.IsTradingHourStartEndBreakBuffer("HSIZ5",YYYYMMDD(20150418),HHMMSS(102000),0,0,3600));
    CPPUNIT_ASSERT( th.IsTradingHourStartEndBreakBuffer("HSIZ5",YYYYMMDD(20150418),HHMMSS(151000),0,3600,0));
    CPPUNIT_ASSERT( th.IsTradingHourStartEndBreakBuffer("HSIZ5",YYYYMMDD(20150418),HHMMSS(151000),0,0,3600));

    CPPUNIT_ASSERT(th.GetTimeNSecBefClose("HSI",YYYYMMDD(20150418),300) == HHMMSS(155500));
    CPPUNIT_ASSERT(th.GetTimeNSecBefClose("HSI",YYYYMMDD(20250418),300) == HHMMSS(155500));
    CPPUNIT_ASSERT(th.GetTimeNSecBefClose("HSI",YYYYMMDD(19000101),300) == HHMMSS());

    CPPUNIT_ASSERT(th.GetTimeNSecBefClose("BOGUS",YYYYMMDD(20150418),300) == HHMMSS(152500));
    CPPUNIT_ASSERT(th.GetTimeNSecBefClose("BOGUS2",YYYYMMDD(20150418),300) == HHMMSS());
  }
}

void TestTradingHours::setUp()
{
}

void TestTradingHours::tearDown()
{
}

CppUnit::Test *TestTradingHours::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestTradingHours");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestTradingHours>(
      "TestTradingHours", 
      &TestTradingHours::RunTest));

  return suiteOfTests;
}
