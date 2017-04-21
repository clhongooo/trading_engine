/*
 * TestRealTime.cpp
 *
 *  Created on: May 30, 2014
 *      Author: sunny
 */

#include "TestRealTime.h"

TestRealTime::TestRealTime() {
  // TODO Auto-generated constructor stub

}

TestRealTime::~TestRealTime() {
  // TODO Auto-generated destructor stub
}

void TestRealTime::RunTest()
{
  shared_ptr<SystemConfig> pSysCfg  = SystemConfig::Instance();
  unsigned int uiChannelID          = 91;

  shared_ptr<RTSClient> rtsClt = RTSClient::Instance();

  //--------------------------------------------------
  // 1.1 Should stop processing and popping msg when the RefreshActivation flag is on
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RealTime: 1.1" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RT-1.1.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->ActivateRefresh(uiChannelID);

    RealTimeProcessor *rtp = new RealTimeProcessor(uiChannelID);
    boost::thread t(&RealTimeProcessor::Run,rtp);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    for (uint32_t i = 1; i < 1001; ++i) pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    boost::this_thread::sleep(boost::posix_time::seconds(2));
    CPPUNIT_ASSERT(pMsgCirBuf->GetStartSeqNo() == 1);
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 1000);

    pShrObj->DeactivateRefresh(uiChannelID);

    boost::this_thread::sleep(boost::posix_time::seconds(2));
    CPPUNIT_ASSERT(pMsgCirBuf->GetStartSeqNo() == 1001);
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);

  }

  return;
}

void TestRealTime::setUp()
{
}

void TestRealTime::tearDown()
{
}

CppUnit::Test *TestRealTime::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRealTime");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestRealTime>(
        "TestRealTime", 
        &TestRealTime::RunTest));

  return suiteOfTests;
}
