/*
 * TestSwitchRTSSvr.cpp
 *
 *  Created on: May 13, 2014
 *      Author: sunny
 */

#include "TestSwitchRTSSvr.h"

TestSwitchRTSSvr::TestSwitchRTSSvr()
{
}

TestSwitchRTSSvr::~TestSwitchRTSSvr()
{
}

void TestSwitchRTSSvr::RunTest()
{
  shared_ptr<SystemConfig> pSysCfg  = SystemConfig::Instance();
  unsigned int uiChannelID          = 91;

  shared_ptr<RTSClient> rtsClt = RTSClient::Instance();

  //--------------------------------------------------
  // 1.1 First RTS server ok, Second not ok
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: 1.1" << endl << flush;
    UTest ut;

    pSysCfg->ReadConfigOptional("Testing/Config-RTS-SWH-1.1.ini");
    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();

    RTSTestingServerThread rtsTestgSvr1((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,21,34,false,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr1);

    rtsClt->AddRTSRequest(uiChannelID,21,34);

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 1.2 First and second RTS server not ok, Third ok
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: 1.2" << endl << flush;
    UTest ut;

    pSysCfg->ReadConfigOptional("Testing/Config-RTS-SWH-1.2.ini");
    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();

    RTSTestingServerThread rtsTestgSvr1((*pSysCfg->GetRTSServerPort())[2],ut,uiChannelID,21,34,false,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr1);

    rtsClt->AddRTSRequest(uiChannelID,21,34);

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }
}

void TestSwitchRTSSvr::setUp()
{
  return;
}

void TestSwitchRTSSvr::tearDown()
{
  return;
}

CppUnit::Test *TestSwitchRTSSvr::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSwitchRTSSvr");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestSwitchRTSSvr>(
        "TestSwitchRTSSvr", 
        &TestSwitchRTSSvr::RunTest));

  return suiteOfTests;
}
