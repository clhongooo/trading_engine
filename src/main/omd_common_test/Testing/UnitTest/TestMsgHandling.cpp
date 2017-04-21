/*
 * TestMsgHandling.cpp
 *
 *  Created on: May 23, 2014
 *      Author: sunny
 */

#include "TestMsgHandling.h"

TestMsgHandling::TestMsgHandling() {
  // TODO Auto-generated constructor stub

}

TestMsgHandling::~TestMsgHandling() {
  // TODO Auto-generated destructor stub
}

void TestMsgHandling::RunTest()
{
  boost::shared_ptr<SystemConfig> pSysCfg  = SystemConfig::Instance();
  unsigned int uiChannelID          = 91;

  {
  }

}

void TestMsgHandling::setUp()
{
}
void TestMsgHandling::tearDown()
{
}

CppUnit::Test *TestMsgHandling::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestMsgHandling");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestMsgHandling>(
        "TestMsgHandling", 
        &TestMsgHandling::RunTest));

  return suiteOfTests;
}
