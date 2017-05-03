/*
 * TestTriggeringRTSClient.cpp
 *
 *  Created on: May 12, 2014
 *      Author: sunny
 */

#include "TestTriggeringRTSClient.h"

TestTriggeringRTSClient::TestTriggeringRTSClient()
{
}

TestTriggeringRTSClient::~TestTriggeringRTSClient()
{
}

void TestTriggeringRTSClient::RunTest()
{
  boost::shared_ptr<SystemConfig> pSysCfg  = SystemConfig::Instance();
  unsigned int uiChannelID          = 91;

  boost::shared_ptr<RTSClient> rtsClt = RTSClient::Instance();

  //--------------------------------------------------
  // 5.1. Test ignoring RTS response if channel is incorrect
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 5.1" << endl << flush;
    UTest ut;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS5.1.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,15,30,false,true,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    rtsClt->AddRTSRequest(uiChannelID,15,30);

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());

    //--------------------------------------------------
    // Test AddCountNumOfRTSConn
    //--------------------------------------------------
    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 1);


    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 6.1: Same as 1.1, except that Refresh is activated
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 6.1" << endl << flush;
    UTest ut;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS6.1.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->ActivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,0,0,true,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg));

    pMsgCirBuf->PushMsg(pbMsg,35,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // Introduce seq no gap here, gap size == 11

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold
    DataCompletenessInspector *dci = new DataCompletenessInspector(uiChannelID,true); //deliberately use heap rather than stack, so that undead threads won't crash
    boost::thread dciThd(&DataCompletenessInspector::Run,dci);

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    //--------------------------------------------------
    // Test AddCountNumOfRTSConn
    //--------------------------------------------------
    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 0);

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 1.1 Seq num gaps (seq no gap larger than trigger threshold, time gap smaller than threshold)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 1.1" << endl << flush;
    UTest ut;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS1.1.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,21,34,false,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg));

    pMsgCirBuf->PushMsg(pbMsg,35,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // Introduce seq no gap here, gap size == 11

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold
    DataCompletenessInspector *dci = new DataCompletenessInspector(uiChannelID,true); //deliberately use heap rather than stack, so that undead threads won't crash
    boost::thread dciThd(&DataCompletenessInspector::Run,dci);

    boost::this_thread::sleep(boost::posix_time::seconds(10));

    //--------------------------------------------------
    // Test AddCountNumOfRTSConn
    //--------------------------------------------------
    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 1);

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 1.2 Seq num gaps (seq no gap smaller than trigger threshold, time gap smaller than threshold)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 1.2" << endl << flush;
    UTest ut;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS1.2.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,0,0,true,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg));

    pMsgCirBuf->PushMsg(pbMsg,25,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // Introduce seq no gap here, gap size == 4, but that's smaller than our setting

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold

    DataCompletenessInspector *dci = new DataCompletenessInspector(uiChannelID,true); //deliberately use heap rather than stack, so that undead threads won't crash
    boost::thread dciThd(&DataCompletenessInspector::Run,dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    //--------------------------------------------------
    // Test AddCountNumOfRTSConn
    //--------------------------------------------------
    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 0);

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 2.1. Time gaps (time gap larger than trigger threshold, seq no gap smaller than threshold)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 2.1" << endl << flush;
    UTest ut;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS2.1.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,21,23,false,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // in i-th millisec

    pMsgCirBuf->PushMsg(pbMsg,24,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // Introduce seq no. gap here, but the seq no gap is smaller than trigger threshold.

    boost::this_thread::sleep(boost::posix_time::seconds(9)); // time gap larger than threshold

    DataCompletenessInspector *dci = new DataCompletenessInspector(uiChannelID,true); //deliberately use heap rather than stack, so that undead threads won't crash
    boost::thread dciThd(&DataCompletenessInspector::Run,dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    //--------------------------------------------------
    // Test AddCountNumOfRTSConn
    //--------------------------------------------------
    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 1);

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 2.2. Time gaps (time gap smaller than trigger threshold, seq no gap smaller than threshold)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 2.2" << endl << flush;
    UTest ut;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS2.2.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,0,0,true,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // in i-th millisec

    pMsgCirBuf->PushMsg(pbMsg,24,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // Introduce seq no. gap here, but the seq no gap is smaller than trigger threshold.

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold

    DataCompletenessInspector *dci = new DataCompletenessInspector(uiChannelID,true); //deliberately use heap rather than stack, so that undead threads won't crash
    boost::thread dciThd(&DataCompletenessInspector::Run,dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    //--------------------------------------------------
    // Test AddCountNumOfRTSConn
    //--------------------------------------------------
    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 0);

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 2.3. Time gaps (time gap smaller than trigger threshold, no seq no gap at all)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 2.3" << endl << flush;
    UTest ut;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS2.3.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,0,0,true,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // in i-th millisec

    pMsgCirBuf->PushMsg(pbMsg,21,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg));

    DataCompletenessInspector *dci = new DataCompletenessInspector(uiChannelID,true); //deliberately use heap rather than stack, so that undead threads won't crash
    boost::thread dciThd(&DataCompletenessInspector::Run,dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    //--------------------------------------------------
    // Test AddCountNumOfRTSConn
    //--------------------------------------------------
    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 0);

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }


  //--------------------------------------------------
  // 3.1. All Gaps larger than threshold
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 3.1" << endl << flush;
    UTest ut;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS3.1.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,21,98,false,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // in i-th millisec

    pMsgCirBuf->PushMsg(pbMsg,99,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // Introduce seq no. gap here, seq no gap is larger than trigger threshold.

    boost::this_thread::sleep(boost::posix_time::seconds(3)); // time gap larger than threshold

    DataCompletenessInspector *dci = new DataCompletenessInspector(uiChannelID,true); //deliberately use heap rather than stack, so that undead threads won't crash
    boost::thread dciThd(&DataCompletenessInspector::Run,dci);

    boost::this_thread::sleep(boost::posix_time::seconds(15));

    //--------------------------------------------------
    // Test AddCountNumOfRTSConn
    //--------------------------------------------------
    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 1);

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }


  //--------------------------------------------------
  // 4.1. Test reaching max daily connection limit
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 4.1" << endl << flush;
    UTest ut;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS4.1.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,0,0,true,false,0,false);
    boost::thread rtsTestgSvrThrd(&RTSTestingServerThread::Run, &rtsTestgSvr);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // in i-th millisec
    pMsgCirBuf->PushMsg(pbMsg,999,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // Introduce seq no. gap here

    DataCompletenessInspector *dci = new DataCompletenessInspector(uiChannelID,true); //deliberately use heap rather than stack, so that undead threads won't crash
    boost::thread dciThd(&DataCompletenessInspector::Run,dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    //--------------------------------------------------
    // Test AddCountNumOfRTSConn
    //--------------------------------------------------
    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 0);

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 7.1: Test closing stale connections and switch to the next RTS server
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger RTS: 7.1" << endl << flush;
    UTest ut;
    UTest ut_dummy;
    boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RTS7.1.ini");

    rtsClt->Init();
    rtsClt->PurgeAllRTSRequests();
    pShrObj->DeactivateRefresh(uiChannelID);
    pShrObj->ResetCountNumOfRTSConn();

    RTSTestingServerThread rtsTestgSvr1((*pSysCfg->GetRTSServerPort())[0],ut,uiChannelID,21,34,false,false,0,true);
    boost::thread rtsTestgSvrThrd1(&RTSTestingServerThread::Run, &rtsTestgSvr1);
    RTSTestingServerThread rtsTestgSvr2((*pSysCfg->GetRTSServerPort())[1],ut_dummy,uiChannelID,22,34,false,false,0,false); // after getting the 1st msg from RTS1, it will stall, therefore now request 22-34
    boost::thread rtsTestgSvrThrd2(&RTSTestingServerThread::Run, &rtsTestgSvr2);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    ExpandableCirBuffer4Msg * pMsgCirBuf = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    pMsgCirBuf->Reset();
    CPPUNIT_ASSERT(pMsgCirBuf->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf->Empty());
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);

    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg));

    pMsgCirBuf->PushMsg(pbMsg,35,SDateTime::GetCurrentUnixTimeInMillisecGMT(),READ_UINT16((BYTE*)pbMsg)); // Introduce seq no gap here, gap size == 11

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold
    DataCompletenessInspector *dci = new DataCompletenessInspector(uiChannelID,false); //deliberately use heap rather than stack, so that undead threads won't crash
    boost::thread dciThd(&DataCompletenessInspector::Run,dci);

    boost::this_thread::sleep(boost::posix_time::seconds(60));

    cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << pShrObj->GetCountNumOfRTSConn() << endl;
    CPPUNIT_ASSERT(pShrObj->GetCountNumOfRTSConn() == 2); //RTSClient should have exited and started for the second time

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  // //--------------------------------------------------
  // // For threads to finish most jobs, to avoid seg fault
  // //--------------------------------------------------
  // boost::this_thread::sleep(boost::posix_time::seconds(5));

}

void TestTriggeringRTSClient::setUp()
{
}

void TestTriggeringRTSClient::tearDown()
{
}

CppUnit::Test *TestTriggeringRTSClient::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestTriggeringRTSClient");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestTriggeringRTSClient>(
        "TestTriggeringRTSClient", 
        &TestTriggeringRTSClient::RunTest));

  return suiteOfTests;
}
