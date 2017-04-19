/*
 * TestRTSResponse.cpp
 *
 *  Created on: Jun 22, 2014
 *      Author: sunny
 */

#include "TestRTSResponse.h"

TestRTSResponse::TestRTSResponse()
{
}

TestRTSResponse::~TestRTSResponse()
{
}

void TestRTSResponse::RunTest()
{
  shared_ptr<SystemConfig> pSysCfg  = SystemConfig::Instance();
  unsigned int uiChannelID          = 91;

  shared_ptr<RTSClient> rtsClt = RTSClient::Instance();

  //--------------------------------------------------
  // 1.1 1 Unknown/Unauthorized channel ID
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: RTS Response: 1.1" << endl << flush;
    UTest ut;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTSRes1.1.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    // 0 Request accepted
    // 1 Unknown/Unauthorized channel ID
    // 2 Messages not available
    // 100 Exceeds maximum sequence range
    // 101 Exceeds maximum requests in a day
    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,21,34,false,false,1,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    BYTE pbMsg[100];
    WRITE_UINT16(&pbMsg[0],100);

    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    pMsgCirBuf->PushMsg(pbMsg,35,Util::getCurrentUnixTime()); // Introduce seq no gap here, gap size == 11

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 1.2 2 Messages not available
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: RTS Response: 1.2" << endl << flush;
    UTest ut;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTSRes1.2.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    // 0 Request accepted
    // 1 Unknown/Unauthorized channel ID
    // 2 Messages not available
    // 100 Exceeds maximum sequence range
    // 101 Exceeds maximum requests in a day
    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,21,34,false,false,2,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    BYTE pbMsg[100];
    WRITE_UINT16(&pbMsg[0],100);

    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    pMsgCirBuf->PushMsg(pbMsg,35,Util::getCurrentUnixTime()); // Introduce seq no gap here, gap size == 11

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 1.3 101 Exceeds maximum requests in a day
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: RTS Response: 1.3" << endl << flush;
    UTest ut;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTSRes1.3.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    // 0 Request accepted
    // 1 Unknown/Unauthorized channel ID
    // 2 Messages not available
    // 100 Exceeds maximum sequence range
    // 101 Exceeds maximum requests in a day
    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,21,34,false,false,101,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    BYTE pbMsg[100];
    WRITE_UINT16(&pbMsg[0],100);

    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    pMsgCirBuf->PushMsg(pbMsg,35,Util::getCurrentUnixTime()); // Introduce seq no gap here, gap size == 11

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 1.4 100 Exceeds maximum sequence range
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: RTS Response: 1.4" << endl << flush;
    UTest ut;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTSRes1.4.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    // 0 Request accepted
    // 1 Unknown/Unauthorized channel ID
    // 2 Messages not available
    // 100 Exceeds maximum sequence range
    // 101 Exceeds maximum requests in a day
    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,21,34,false,false,100,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    BYTE pbMsg[100];
    WRITE_UINT16(&pbMsg[0],100);

    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    pMsgCirBuf->PushMsg(pbMsg,35,Util::getCurrentUnixTime()); // Introduce seq no gap here, gap size == 11

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }


}

void TestRTSResponse::setUp()
{
}

void TestRTSResponse::tearDown()
{
}

CppUnit::Test *TestRTSResponse::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRTSResponse");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestRTSResponse>(
        "TestRTSResponse", 
        &TestRTSResponse::RunTest));

  return suiteOfTests;
}
