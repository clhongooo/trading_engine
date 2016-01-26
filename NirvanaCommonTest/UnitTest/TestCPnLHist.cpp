#include "TestCPnLHist.h"

TestCPnLHist::TestCPnLHist()
{
}

TestCPnLHist::~TestCPnLHist()
{
}

void TestCPnLHist::RunTest()
{

  //--------------------------------------------------
  {
    CPnLHist ch;
    CPPUNIT_ASSERT(ch.LoadFromFile("/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommonTest/UnitTest/cpnlhist.csv"));

    CPPUNIT_ASSERT(ch.GetPnL(2,false) == (48533-47020));
    CPPUNIT_ASSERT(ch.GetPnL(1,false) == (48533-47827));
    CPPUNIT_ASSERT(ch.GetPnL(2,true) == (48533-47020));
    CPPUNIT_ASSERT(ch.GetPnL(1,true) == (48533-47827));
    CPPUNIT_ASSERT(std::isnan(ch.GetPnL(200,false)));
    CPPUNIT_ASSERT(std::isnan(ch.GetPnL(-2,false)));
    CPPUNIT_ASSERT(std::isnan(ch.GetPnL(200,true)));
    CPPUNIT_ASSERT(std::isnan(ch.GetPnL(-2,true)));

    CPPUNIT_ASSERT(ch.GetPnL(YYYYMMDD("2015-10-23"),4,false) == (46620-43720));
    CPPUNIT_ASSERT(ch.GetPnL(YYYYMMDD("2015-10-23"),4,true) == (46620-43720));
    CPPUNIT_ASSERT(ch.GetPnL(YYYYMMDD("2015-10-12"),4,true) == (40196-38590));
    CPPUNIT_ASSERT(std::isnan(ch.GetPnL(YYYYMMDD("2015-10-23"),1004,false)));
    CPPUNIT_ASSERT(std::isnan(ch.GetPnL(YYYYMMDD("2015-10-23"),1004,true)));

    CPPUNIT_ASSERT(ch.GetPnL(YYYYMMDD("2015-07-31"),YYYYMMDD("2015-08-10")) == (10666-6713));
    CPPUNIT_ASSERT(std::isnan(ch.GetPnL(YYYYMMDD("2015-07-18"),YYYYMMDD("2015-08-10"))));

    //--------------------------------------------------
    ch.AddAndTruncate(YYYYMMDD("2015-10-24"),47021);

    CPPUNIT_ASSERT(ch.GetPnL(YYYYMMDD("2015-10-24"),1,false) == 401);
    CPPUNIT_ASSERT(ch.GetPnL(YYYYMMDD("2015-10-24"),2,false) == 1361);
    CPPUNIT_ASSERT(ch.GetPnL(YYYYMMDD("2015-10-24"),1,true) == 401);
    CPPUNIT_ASSERT(ch.GetPnL(YYYYMMDD("2015-10-24"),2,true) == 1361);
    CPPUNIT_ASSERT(std::isnan(ch.GetPnL(YYYYMMDD("2015-10-25"),1,false)));
    CPPUNIT_ASSERT(std::isnan(ch.GetPnL(YYYYMMDD("2015-10-24"),YYYYMMDD("2015-10-25"))));

    //--------------------------------------------------
    CPPUNIT_ASSERT(ch.IsDateAvailable(YYYYMMDD("2015-10-24")));
    CPPUNIT_ASSERT(!ch.IsDateAvailable(YYYYMMDD("2015-10-25")));
  }
  //--------------------------------------------------

  //--------------------------------------------------
  {
    CPnLHist ch;
    CPPUNIT_ASSERT(ch.LoadFromFile("/home/qy/Dropbox/nirvana/workspaceCN/NirvanaCommonTest/UnitTest/cpnlhist_2.csv"));

    CPPUNIT_ASSERT(abs(ch.GetEstWinningProb(20))-0.45 < 0.001);
    CPPUNIT_ASSERT(abs(ch.GetEstNetOdds(20))-1.1304 < 0.001);
  }
  //--------------------------------------------------


  return;
}

void TestCPnLHist::setUp()
{
}

void TestCPnLHist::tearDown()
{
}

CppUnit::Test *TestCPnLHist::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestCPnLHist");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestCPnLHist>(
        "TestCPnLHist", 
        &TestCPnLHist::RunTest));

  return suiteOfTests;
}
