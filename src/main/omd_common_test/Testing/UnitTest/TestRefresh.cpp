/*
 * TestRefresh.cpp
 *
 *  Created on: May 22, 2014
 *      Author: sunny
 */

#include "TestRefresh.h"

TestRefresh::TestRefresh()
{

}

TestRefresh::~TestRefresh()
{
}


void TestRefresh::RunTest()
{
  shared_ptr<SystemConfig> pSysCfg  = SystemConfig::Instance();
  unsigned int uiChannelID          = 91;

  //--------------------------------------------------
  // 1.1 Seq num gaps (seq no gap larger than trigger threshold, time gap smaller than threshold)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger Refresh: 1.1" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RF-1.1.ini");

    //--------------------------------------------------
    // Deactivate first
    //--------------------------------------------------
    pShrObj->DeactivateRefresh(uiChannelID);

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

    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    pMsgCirBuf->PushMsg(pbMsg,1021,Util::getCurrentUnixTime()); // Introduce seq no gap here, gap size == 1000

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap less than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID));
  }
  //--------------------------------------------------
  // 1.2 Seq num gaps (seq no gap smaller than trigger threshold, time gap smaller than threshold)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger Refresh: 1.2" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RF-1.2.ini");

    //--------------------------------------------------
    // Deactivate first
    //--------------------------------------------------
    pShrObj->DeactivateRefresh(uiChannelID);

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

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    pMsgCirBuf->PushMsg(pbMsg,421,Util::getCurrentUnixTime()); // Introduce seq no gap here, gap size == 4, but that's smaller than our setting

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap less than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    CPPUNIT_ASSERT(!(pShrObj->CheckRefreshActivatnStatus(uiChannelID)));
  }
  //--------------------------------------------------
  // 2.1. Time gaps (time gap larger than trigger threshold, seq no gap smaller than threshold)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger Refresh: 2.1" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RF-2.1.ini");

    //--------------------------------------------------
    // Deactivate first
    //--------------------------------------------------
    pShrObj->DeactivateRefresh(uiChannelID);

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

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime()); // in i-th millisec

    pMsgCirBuf->PushMsg(pbMsg,24,Util::getCurrentUnixTime()); // Introduce seq no. gap here, but the seq no gap is smaller than trigger threshold.
    boost::this_thread::sleep(boost::posix_time::seconds(6)); // Introduce time gap here, time gap larger than threshold

    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID));
  }
  //--------------------------------------------------
  // 2.2. Time gaps (time gap smaller than trigger threshold, seq no gap smaller than threshold)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger Refresh: 2.2" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RF-2.2.ini");

    //--------------------------------------------------
    // Deactivate first
    //--------------------------------------------------
    pShrObj->DeactivateRefresh(uiChannelID);

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

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime()); // in i-th millisec
    pMsgCirBuf->PushMsg(pbMsg,24,Util::getCurrentUnixTime()); // Introduce seq no. gap here, but the seq no gap is smaller than trigger threshold.

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    CPPUNIT_ASSERT(!(pShrObj->CheckRefreshActivatnStatus(uiChannelID)));
  }

  //--------------------------------------------------
  // 2.3. Time gaps (time gap smaller than trigger threshold, no seq no gap at all)
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger Refresh: 2.3" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RF-2.3.ini");

    //--------------------------------------------------
    // Deactivate first
    //--------------------------------------------------
    pShrObj->DeactivateRefresh(uiChannelID);

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

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime()); // in i-th millisec

    pMsgCirBuf->PushMsg(pbMsg,21,Util::getCurrentUnixTime()); // no seq num gap

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap smaller than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    CPPUNIT_ASSERT(!(pShrObj->CheckRefreshActivatnStatus(uiChannelID)));
  }

  //--------------------------------------------------
  // 3.1. All Gaps larger than threshold
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger Refresh: 3.1" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RF-3.1.ini");

    //--------------------------------------------------
    // Deactivate first
    //--------------------------------------------------
    pShrObj->DeactivateRefresh(uiChannelID);

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

    uint32_t i = 1;
    for (i = 1; i < 21; ++i) pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime()); // in i-th millisec

    pMsgCirBuf->PushMsg(pbMsg,2020,Util::getCurrentUnixTime()); // Introduce seq no. gap here, seq no gap is larger than trigger threshold.

    boost::this_thread::sleep(boost::posix_time::seconds(6)); // time gap larger than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID));
  }
  //--------------------------------------------------
  // 5.1: Same as 1.1, Except that Refresh is activated, and RTS is not underway
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger Refresh: 5.1" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RF-1.1.ini");

    pShrObj->ActivateRefresh(uiChannelID);
    // pShrObj->UnsetRTSUnderway();
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatusCount(uiChannelID) == 1);

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


    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    pMsgCirBuf->PushMsg(pbMsg,1021,Util::getCurrentUnixTime()); // Introduce seq no gap here, gap size == 1000

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap less than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatusCount(uiChannelID) == 1);
  }
  //--------------------------------------------------
  // 5.2: Same as 1.1, Except that Refresh is activated, and RTS is underway
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: Trigger Refresh: 5.2" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RF-1.1.ini");

    pShrObj->ActivateRefresh(uiChannelID);
    // pShrObj->SetRTSUnderway();
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatusCount(uiChannelID) == 1);

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

    uint32_t i = 1;
    for (i = 1; i < 21; ++i)
      pMsgCirBuf->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    pMsgCirBuf->PushMsg(pbMsg,1021,Util::getCurrentUnixTime()); // Introduce seq no gap here, gap size == 1000

    boost::this_thread::sleep(boost::posix_time::seconds(1)); // time gap less than threshold
    DataCompletenessInspector dci(uiChannelID,true);
    boost::thread dciThd(&DataCompletenessInspector::Run,&dci);

    boost::this_thread::sleep(boost::posix_time::seconds(5));

    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatusCount(uiChannelID) == 1);
  }


  //--------------------------------------------------
  // 4.1. Test RefreshProcessor Behaviour
  //--------------------------------------------------
  {
    uint32_t uiSeqNo = 0;
    cout << __FILE__ << ": Unit Test: Trigger Refresh: 4.1" << endl << flush;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-RF-4.1.ini");

    pShrObj->DeactivateRefresh(uiChannelID);
    // pShrObj->UnsetRTSUnderway();
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    RefreshProcessor rp(uiChannelID);
    boost::thread t(&RefreshProcessor::Run,&rp);

    //--------------------------------------------------
    // Insert dummy data into circular buffer
    //--------------------------------------------------
    BYTE pbMsg[100];

    OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    ExpandableCirBuffer4Msg * pMsgCirBuf_RT = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    ExpandableCirBuffer4Msg * pMsgCirBuf_RF = pShrObj->GetMsgCirBufPtr(McastIdentifier::REFRESH , uiChannelID);

    oss = (OMDC_Security_Status*) (&pbMsg[0]);
    oss->MsgSize = sizeof(OMDC_Security_Status);
    oss->MsgType = OMDC_SECURITY_STATUS;
    oss->SecurityCode = 9394;
    oss->SecurityTradingStatus = 0;

    for (uint32_t i = 1; i < 21; ++i) pMsgCirBuf_RF->PushMsg(pbMsg,i,Util::getCurrentUnixTime());
    for (uint32_t i = 1; i < 51; ++i) pMsgCirBuf_RT->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 1);
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 20);
    CPPUNIT_ASSERT(pMsgCirBuf_RF->GetLatestSeqNo(uiSeqNo));
    CPPUNIT_ASSERT(uiSeqNo == 20);
    CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 1);
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 50);
    CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiSeqNo));
    CPPUNIT_ASSERT(uiSeqNo == 50);

    {
      //--------------------------------------------------
      // Refresh mode activated
      //--------------------------------------------------
      CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));
      pShrObj->ActivateRefresh(uiChannelID);

      OMD_Refresh_Complete* orc = (OMD_Refresh_Complete*)&pbMsg[0];
      orc->MsgSize = sizeof(OMD_Refresh_Complete);
      orc->MsgType = OMD_REFRESH_COMPLETE;
      orc->LastSeqNum = 10;

      pMsgCirBuf_RF->PushMsg(pbMsg,21,Util::getCurrentUnixTime());

      boost::this_thread::sleep(boost::posix_time::seconds(1));
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 22);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
      CPPUNIT_ASSERT(!pMsgCirBuf_RF->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 11);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 40);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(uiSeqNo == 50);
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    {
      //--------------------------------------------------
      // Refresh mode not activated
      //--------------------------------------------------
      CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

      OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
      oss->MsgSize = sizeof(OMDC_Security_Status);
      oss->MsgType = OMDC_SECURITY_STATUS;
      oss->SecurityCode = 9394;
      oss->SecurityTradingStatus = 0;

      for (uint32_t i = 22; i < 31; ++i) pMsgCirBuf_RF->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

      OMD_Refresh_Complete* orc = (OMD_Refresh_Complete*)&pbMsg[0];
      orc->MsgSize = sizeof(OMD_Refresh_Complete);
      orc->MsgType = OMD_REFRESH_COMPLETE;
      orc->LastSeqNum = 15;

      pMsgCirBuf_RF->PushMsg(pbMsg,31,Util::getCurrentUnixTime());

      boost::this_thread::sleep(boost::posix_time::seconds(1));
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 32);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
      CPPUNIT_ASSERT(!pMsgCirBuf_RF->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 11);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 40);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(uiSeqNo == 50);
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    {
      //--------------------------------------------------
      // Gap in refresh channel
      //--------------------------------------------------
      CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));
      pShrObj->ActivateRefresh(uiChannelID);

      OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
      oss->MsgSize = sizeof(OMDC_Security_Status);
      oss->MsgType = OMDC_SECURITY_STATUS;
      oss->SecurityCode = 9394;
      oss->SecurityTradingStatus = 0;

      for (uint32_t i = 32; i < 35; ++i) pMsgCirBuf_RF->PushMsg(pbMsg,i,Util::getCurrentUnixTime());
      for (uint32_t i = 36; i < 91; ++i) pMsgCirBuf_RF->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

      OMD_Refresh_Complete* orc = (OMD_Refresh_Complete*)&pbMsg[0];
      orc->MsgSize = sizeof(OMD_Refresh_Complete);
      orc->MsgType = OMD_REFRESH_COMPLETE;
      orc->LastSeqNum = 20;

      pMsgCirBuf_RF->PushMsg(pbMsg,91,Util::getCurrentUnixTime());

      boost::this_thread::sleep(boost::posix_time::seconds(3)); //since there's a grace waiting period of 2 seconds for the missing msg
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 92);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
      CPPUNIT_ASSERT(!pMsgCirBuf_RF->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 11);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 40);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(uiSeqNo == 50);
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    {
      //--------------------------------------------------
      // Last seq no is smaller than all RT msg
      //--------------------------------------------------
      CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID)); // we want refresh activation to be continually on

      OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
      oss->MsgSize = sizeof(OMDC_Security_Status);
      oss->MsgType = OMDC_SECURITY_STATUS;
      oss->SecurityCode = 9394;
      oss->SecurityTradingStatus = 0;

      for (uint32_t i = 92; i < 101; ++i) pMsgCirBuf_RF->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

      OMD_Refresh_Complete* orc = (OMD_Refresh_Complete*)&pbMsg[0];
      orc->MsgSize = sizeof(OMD_Refresh_Complete);
      orc->MsgType = OMD_REFRESH_COMPLETE;
      orc->LastSeqNum = 5;

      pMsgCirBuf_RF->PushMsg(pbMsg,101,Util::getCurrentUnixTime());

      boost::this_thread::sleep(boost::posix_time::seconds(1));
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 102);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
      CPPUNIT_ASSERT(!pMsgCirBuf_RF->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 11);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 40);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(uiSeqNo == 50);
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    {
      //--------------------------------------------------
      // Back to normal, successful refresh
      //--------------------------------------------------
      CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID)); // we want refresh activation to be continually on

      OMDC_Security_Status * oss = (OMDC_Security_Status*) (&pbMsg[0]);
      oss->MsgSize = sizeof(OMDC_Security_Status);
      oss->MsgType = OMDC_SECURITY_STATUS;
      oss->SecurityCode = 9394;
      oss->SecurityTradingStatus = 0;

      for (uint32_t i = 102; i < 152; ++i) pMsgCirBuf_RF->PushMsg(pbMsg,i,Util::getCurrentUnixTime());

      OMD_Refresh_Complete* orc = (OMD_Refresh_Complete*)&pbMsg[0];
      orc->MsgSize = sizeof(OMD_Refresh_Complete);
      orc->MsgType = OMD_REFRESH_COMPLETE;
      orc->LastSeqNum = 30;

      pMsgCirBuf_RF->PushMsg(pbMsg,152,Util::getCurrentUnixTime());

      boost::this_thread::sleep(boost::posix_time::seconds(1));
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 153);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
      CPPUNIT_ASSERT(!pMsgCirBuf_RF->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 31);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiSeqNo));
      CPPUNIT_ASSERT(uiSeqNo == 50);
    }
  }
  return;
}

void TestRefresh::setUp()
{
}

void TestRefresh::tearDown()
{
}

CppUnit::Test *TestRefresh::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRefresh");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestRefresh>(
        "TestRefresh", 
        &TestRefresh::RunTest));

  return suiteOfTests;
}
