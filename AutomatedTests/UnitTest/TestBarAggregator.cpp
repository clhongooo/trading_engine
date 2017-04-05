#include "TestBarAggregator.h"

TestBarAggregator::TestBarAggregator() {

}

TestBarAggregator::~TestBarAggregator() {
}

void TestBarAggregator::RunTest()
{
  YYYYMMDD yyyymmdd(19000101);
  HHMMSS hhmmss(0);
  double dOpen  = 100;
  double dHigh  = 100;
  double dLow   = 100;
  double dClose = 100;
  long lVolume  = 100;

  BarAggregator ba;
  ba.SetBarInterval(BarAggregator::MINUTE);

  //--------------------------------------------------
  // Empty
  //--------------------------------------------------
  CPPUNIT_ASSERT(!ba.GetOHLCBar(yyyymmdd,hhmmss,dOpen,dHigh,dLow,dClose,lVolume));
  CPPUNIT_ASSERT(yyyymmdd == YYYYMMDD(19000101));
  CPPUNIT_ASSERT(hhmmss   == HHMMSS(0));
  CPPUNIT_ASSERT(dOpen    == 100);
  CPPUNIT_ASSERT(dHigh    == 100);
  CPPUNIT_ASSERT(dLow     == 100);
  CPPUNIT_ASSERT(dClose   == 100);
  CPPUNIT_ASSERT(lVolume  == 100);

  //--------------------------------------------------
  // 1 Tick
  //--------------------------------------------------
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140201),200,10);
  CPPUNIT_ASSERT(!ba.GetOHLCBar(yyyymmdd,hhmmss,dOpen,dHigh,dLow,dClose,lVolume));
  //--------------------------------------------------
  // 5 Tick
  //--------------------------------------------------
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140303),201,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140311),210,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140339),100,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140344),150,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140345),300,10);
  //--------------------------------------------------
  // 10 Tick
  //--------------------------------------------------
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140401),202,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140401),210,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140410),100,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140410),150,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140420),199,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140420),220,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140430),280,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140430),350,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140440),320,10);
  ba.AddTick(YYYYMMDD(20150116),HHMMSS(140459),270,10);
  //--------------------------------------------------
  // Next day
  //--------------------------------------------------
  ba.AddTick(YYYYMMDD(20150117),HHMMSS(140410),270,10);
  ba.AddTick(YYYYMMDD(20150117),HHMMSS(140410),280,10);
  ba.AddTick(YYYYMMDD(20150117),HHMMSS(140410),260,10);
  ba.AddTick(YYYYMMDD(20150117),HHMMSS(140410),250,10);
  ba.AddTick(YYYYMMDD(20150117),HHMMSS(140420),240,10);
  ba.AddTick(YYYYMMDD(20150117),HHMMSS(140430),240,10);
  //--------------------------------------------------
  ba.AddTick(YYYYMMDD(20150117),HHMMSS(140505),240,10);

  //--------------------------------------------------
  // 1 Tick
  //--------------------------------------------------
  CPPUNIT_ASSERT(ba.GetOHLCBar(yyyymmdd,hhmmss,dOpen,dHigh,dLow,dClose,lVolume));
  CPPUNIT_ASSERT(yyyymmdd == YYYYMMDD(20150116));
  CPPUNIT_ASSERT(hhmmss   == HHMMSS(140201));
  CPPUNIT_ASSERT(dOpen   == 200);
  CPPUNIT_ASSERT(dHigh   == 200);
  CPPUNIT_ASSERT(dLow    == 200);
  CPPUNIT_ASSERT(dClose  == 200);
  CPPUNIT_ASSERT(lVolume == 10);
  //--------------------------------------------------
  // 5 Tick
  //--------------------------------------------------
  CPPUNIT_ASSERT(ba.GetOHLCBar(yyyymmdd,hhmmss,dOpen,dHigh,dLow,dClose,lVolume));
  CPPUNIT_ASSERT(yyyymmdd == YYYYMMDD(20150116));
  CPPUNIT_ASSERT(hhmmss   == HHMMSS(140345));
  CPPUNIT_ASSERT(dOpen   == 201);
  CPPUNIT_ASSERT(dHigh   == 300);
  CPPUNIT_ASSERT(dLow    == 100);
  CPPUNIT_ASSERT(dClose  == 300);
  CPPUNIT_ASSERT(lVolume == 50);
  //--------------------------------------------------
  // 10 Tick
  //--------------------------------------------------
  CPPUNIT_ASSERT(ba.GetOHLCBar(yyyymmdd,hhmmss,dOpen,dHigh,dLow,dClose,lVolume));
  CPPUNIT_ASSERT(yyyymmdd == YYYYMMDD(20150116));
  CPPUNIT_ASSERT(hhmmss   == HHMMSS(140459));
  CPPUNIT_ASSERT(dOpen   == 202);
  CPPUNIT_ASSERT(dHigh   == 350);
  CPPUNIT_ASSERT(dLow    == 100);
  CPPUNIT_ASSERT(dClose  == 270);
  CPPUNIT_ASSERT(lVolume == 100);
  //--------------------------------------------------
  // Next day
  //--------------------------------------------------
  CPPUNIT_ASSERT(ba.GetOHLCBar(yyyymmdd,hhmmss,dOpen,dHigh,dLow,dClose,lVolume));
  CPPUNIT_ASSERT(yyyymmdd == YYYYMMDD(20150117));
  CPPUNIT_ASSERT(hhmmss   == HHMMSS(140430));
  CPPUNIT_ASSERT(dOpen   == 270);
  CPPUNIT_ASSERT(dHigh   == 280);
  CPPUNIT_ASSERT(dLow    == 240);
  CPPUNIT_ASSERT(dClose  == 240);
  CPPUNIT_ASSERT(lVolume == 60);

  return;
}

void TestBarAggregator::setUp()
{
}

void TestBarAggregator::tearDown()
{
}

CppUnit::Test *TestBarAggregator::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestBarAggregator");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestBarAggregator>(
        "TestBarAggregator",
        &TestBarAggregator::RunTest));

  return suiteOfTests;
}

