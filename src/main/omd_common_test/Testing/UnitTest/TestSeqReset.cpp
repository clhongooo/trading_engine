/*
 * TestSeqReset.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: sunny
 */

#include "TestSeqReset.h"

TestSeqReset::TestSeqReset()
{
}

TestSeqReset::~TestSeqReset()
{
}

void TestSeqReset::RunTest()
{
  boost::shared_ptr<SystemConfig>  pSysCfg  = SystemConfig::Instance();
  boost::shared_ptr<SharedObjects> pShrObj  = SharedObjects::Instance();
  unsigned int uiChannelID                  = 91;

  pSysCfg->Reset();

  McastIdentifier mci_rf(
      uiChannelID, // Channel ID
      "IP", 9394, // dummy IP and port
      McastIdentifier::REFRESH, // REALTIME / REFRESH
      McastIdentifier::A); //any channel

  McastIdentifier mci_rfa(
      uiChannelID, // Channel ID
      "IP", 9394, // dummy IP and port
      McastIdentifier::REFRESH, // REALTIME / REFRESH
      McastIdentifier::A); //any channel

  McastIdentifier mci_rfb(
      uiChannelID, // Channel ID
      "IP", 9394, // dummy IP and port
      McastIdentifier::REFRESH, // REALTIME / REFRESH
      McastIdentifier::B); //any channel

  McastIdentifier mci_rt(
      uiChannelID, // Channel ID
      "IP", 9394, // dummy IP and port
      McastIdentifier::REALTIME, // REALTIME / REFRESH
      McastIdentifier::A); //any channel

  McastIdentifier mci_rta(
      uiChannelID, // Channel ID
      "IP", 9394, // dummy IP and port
      McastIdentifier::REALTIME, // REALTIME / REFRESH
      McastIdentifier::A); //any channel

  McastIdentifier mci_rtb(
      uiChannelID, // Channel ID
      "IP", 9394, // dummy IP and port
      McastIdentifier::REALTIME, // REALTIME / REFRESH
      McastIdentifier::B); //any channel

  //--------------------------------------------------
  // 1.1 RT Seq Reset x 1. RF Seq Reset x 0
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: 1.1" << endl << flush;

    pSysCfg->ReadConfigOptional("Testing/Config-SeqReset1.1.ini");

    pShrObj->ResetRTChnlSeqNoOffset();
    pShrObj->ResetRFChnlSeqNoOffset();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    PreProcessor *pprf = new PreProcessor_OMD(mci_rf);
    boost::thread tpprf(&PreProcessor::Run,pprf);

    PreProcessor *pprt = new PreProcessor_OMD(mci_rt);
    boost::thread tpprt(&PreProcessor::Run,pprt);

    RefreshProcessor *rfp = new RefreshProcessor_OMD(uiChannelID);
    boost::thread trfp(&RefreshProcessor::Run,rfp);

    ExpandableCirBuffer *     pRawCirBuf_RT = pShrObj->GetRawPktCirBufPtr(mci_rt);
    ExpandableCirBuffer *     pRawCirBuf_RF = pShrObj->GetRawPktCirBufPtr(mci_rf);

    ExpandableCirBuffer4Msg * pMsgCirBuf_RT = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    ExpandableCirBuffer4Msg * pMsgCirBuf_RF = pShrObj->GetMsgCirBufPtr(McastIdentifier::REFRESH , uiChannelID);

    pRawCirBuf_RT->Reset();
    pRawCirBuf_RF->Reset();
    pMsgCirBuf_RT->Reset();
    pMsgCirBuf_RF->Reset();

    BYTE pbPkt[1000];


    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RT Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RT->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RT->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RF Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 31; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum   = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RF->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 30);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RT Seq Reset
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Sequence_Reset * osr = (OMD_Sequence_Reset *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMD_Sequence_Reset);
      oph->MsgCount = 1;
      oph->SeqNum = 21;
      oph->SendTime = 8888+21;

      osr->MsgSize  = sizeof(OMD_Sequence_Reset);
      osr->MsgType  = OMD_SEQUENCE_RESET;
      osr->NewSeqNo = 1;

      BYTE *bT = pRawCirBuf_RT->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RT->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    // CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 21);
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == true);

    //--------------------------------------------------
    // RT Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RT->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RT->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // Answer checking
    //--------------------------------------------------
    {
      uint32_t uiLatestSeqNo = 0;

      CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
      CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);

      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 22);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 41);

      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 30);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 1);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 30);

      CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
      CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    //--------------------------------------------------
    // RF Refresh Complete
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Refresh_Complete * orc = (OMD_Refresh_Complete *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMD_Refresh_Complete);
      oph->MsgCount = 1;
      oph->SeqNum   = 31;
      oph->SendTime = 8888+31;

      orc->MsgSize  = sizeof(OMD_Refresh_Complete);
      orc->MsgType  = OMD_REFRESH_COMPLETE;
      orc->LastSeqNum = 10;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // Answer checking
    //--------------------------------------------------
    {
      uint32_t uiLatestSeqNo = 0;

      CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
      CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);

      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 10);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 32);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 41);

      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 32);
      CPPUNIT_ASSERT(!pMsgCirBuf_RF->GetLatestSeqNo(uiLatestSeqNo));
    }

  }

  //--------------------------------------------------
  // 1.2 RT Seq Reset x 0. RF Seq Reset x 1
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: 1.2" << endl << flush;

    pSysCfg->ReadConfigOptional("Testing/Config-SeqReset1.1.ini"); // reuse

    pShrObj->ResetRTChnlSeqNoOffset();
    pShrObj->ResetRFChnlSeqNoOffset();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    PreProcessor *pprf = new PreProcessor_OMD(mci_rf);
    boost::thread tpprf(&PreProcessor::Run,pprf);

    PreProcessor *pprt = new PreProcessor_OMD(mci_rt);
    boost::thread tpprt(&PreProcessor::Run,pprt);

    RefreshProcessor *rfp = new RefreshProcessor_OMD(uiChannelID);
    boost::thread trfp(&RefreshProcessor::Run,rfp);

    ExpandableCirBuffer *     pRawCirBuf_RT = pShrObj->GetRawPktCirBufPtr(mci_rt);
    ExpandableCirBuffer *     pRawCirBuf_RF = pShrObj->GetRawPktCirBufPtr(mci_rf);

    ExpandableCirBuffer4Msg * pMsgCirBuf_RT = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    ExpandableCirBuffer4Msg * pMsgCirBuf_RF = pShrObj->GetMsgCirBufPtr(McastIdentifier::REFRESH , uiChannelID);

    pRawCirBuf_RT->Reset();
    pRawCirBuf_RF->Reset();
    pMsgCirBuf_RT->Reset();
    pMsgCirBuf_RF->Reset();

    BYTE pbPkt[1000];


    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RT Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RT->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RT->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RF Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 31; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum   = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 30);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RF Seq Reset
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Sequence_Reset * osr = (OMD_Sequence_Reset *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMD_Sequence_Reset);
      oph->MsgCount = 1;
      oph->SeqNum = 31;
      oph->SendTime = 8888+31;

      osr->MsgSize  = sizeof(OMD_Sequence_Reset);
      osr->MsgType  = OMD_SEQUENCE_RESET;
      osr->NewSeqNo = 1;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RF->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 31);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false); //Readiness Test case: RF Seq Reset should not affect RT

    //--------------------------------------------------
    // RF Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 31; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RF->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 30);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // Answer checking
    //--------------------------------------------------
    {
      uint32_t uiLatestSeqNo = 0;

      CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
      CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 31);

      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 1);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 20);

      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 30);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 32);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 61);

      CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
      CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    //--------------------------------------------------
    // Activate Refresh first
    //--------------------------------------------------
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));
    pShrObj->ActivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID));
    //--------------------------------------------------
    // RF Refresh Complete
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Refresh_Complete * orc = (OMD_Refresh_Complete *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMD_Refresh_Complete);
      oph->MsgCount = 1;
      oph->SeqNum   = 31;
      oph->SendTime = 8888+31;

      orc->MsgSize  = sizeof(OMD_Refresh_Complete);
      orc->MsgType  = OMD_REFRESH_COMPLETE;
      orc->LastSeqNum = 10;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // Answer checking
    //--------------------------------------------------
    {
      uint32_t uiLatestSeqNo = 0;

      CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
      CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 31);

      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 10);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 11);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 20);

      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 63);
      CPPUNIT_ASSERT(!pMsgCirBuf_RF->GetLatestSeqNo(uiLatestSeqNo));
    }

  }

  //--------------------------------------------------
  // 1.3 RT Seq Reset x 1. RF Seq Reset x 1
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: 1.3" << endl << flush;

    pSysCfg->ReadConfigOptional("Testing/Config-SeqReset1.1.ini"); // reuse

    pShrObj->ResetRTChnlSeqNoOffset();
    pShrObj->ResetRFChnlSeqNoOffset();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    PreProcessor *pprf = new PreProcessor_OMD(mci_rf);
    boost::thread tpprf(&PreProcessor::Run,pprf);

    PreProcessor *pprt = new PreProcessor_OMD(mci_rt);
    boost::thread tpprt(&PreProcessor::Run,pprt);

    RefreshProcessor *rfp = new RefreshProcessor_OMD(uiChannelID);
    boost::thread trfp(&RefreshProcessor::Run,rfp);

    ExpandableCirBuffer *     pRawCirBuf_RT = pShrObj->GetRawPktCirBufPtr(mci_rt);
    ExpandableCirBuffer *     pRawCirBuf_RF = pShrObj->GetRawPktCirBufPtr(mci_rf);

    ExpandableCirBuffer4Msg * pMsgCirBuf_RT = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);
    ExpandableCirBuffer4Msg * pMsgCirBuf_RF = pShrObj->GetMsgCirBufPtr(McastIdentifier::REFRESH , uiChannelID);

    pRawCirBuf_RT->Reset();
    pRawCirBuf_RF->Reset();
    pMsgCirBuf_RT->Reset();
    pMsgCirBuf_RF->Reset();

    BYTE pbPkt[1000];


    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RT Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RT->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RT->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RF Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 31; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum   = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 30);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RT Seq Reset
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Sequence_Reset * osr = (OMD_Sequence_Reset *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMD_Sequence_Reset);
      oph->MsgCount = 1;
      oph->SeqNum = 21;
      oph->SendTime = 8888+21;

      osr->MsgSize  = sizeof(OMD_Sequence_Reset);
      osr->MsgType  = OMD_SEQUENCE_RESET;
      osr->NewSeqNo = 1;

      BYTE *bT = pRawCirBuf_RT->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RT->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == true);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RF Seq Reset
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Sequence_Reset * osr = (OMD_Sequence_Reset *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMD_Sequence_Reset);
      oph->MsgCount = 1;
      oph->SeqNum = 31;
      oph->SendTime = 8888+31;

      osr->MsgSize  = sizeof(OMD_Sequence_Reset);
      osr->MsgType  = OMD_SEQUENCE_RESET;
      osr->NewSeqNo = 1;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 31);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false); //should not activate refresh

    //--------------------------------------------------
    // RT Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RT->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RT->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // RF Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 31; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 30);
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // Answer checking
    //--------------------------------------------------
    {
      uint32_t uiLatestSeqNo = 0;

      CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
      CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 31);

      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 22);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 41);

      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 30);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 32);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 61);

      CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
      CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    //--------------------------------------------------
    // Activate Refresh first
    //--------------------------------------------------
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));
    pShrObj->ActivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    //--------------------------------------------------
    // RF Refresh Complete
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Refresh_Complete * orc = (OMD_Refresh_Complete *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMD_Refresh_Complete);
      oph->MsgCount = 1;
      oph->SeqNum   = 31;
      oph->SendTime = 8888+31;

      orc->MsgSize  = sizeof(OMD_Refresh_Complete);
      orc->MsgType  = OMD_REFRESH_COMPLETE;
      orc->LastSeqNum = 10;

      BYTE *bT = pRawCirBuf_RF->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RF->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RF->Size() << endl << flush;
    CPPUNIT_ASSERT(pRawCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RF->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);

    //--------------------------------------------------
    // Answer checking
    //--------------------------------------------------
    {
      uint32_t uiLatestSeqNo = 0;

      CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
      CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 31);

      pMsgCirBuf_RT->Size();
      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 10);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 32);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 41);

      CPPUNIT_ASSERT(pMsgCirBuf_RF->Size() == 0);
      CPPUNIT_ASSERT(pMsgCirBuf_RF->GetStartSeqNo() == 63);
      CPPUNIT_ASSERT(!pMsgCirBuf_RF->GetLatestSeqNo(uiLatestSeqNo));
    }

  }

  //--------------------------------------------------
  // 1.4 RTA received OMD_SEQUENCE_RESET, RTB missed OMD_SEQUENCE_RESET, RTA leads a lot
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: 1.4" << endl << flush;

    pSysCfg->ReadConfigOptional("Testing/Config-SeqReset1.1.ini"); // reuse

    pShrObj->ResetRTChnlSeqNoOffset();
    pShrObj->ResetRFChnlSeqNoOffset();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    PreProcessor *pprf = new PreProcessor_OMD(mci_rf);
    boost::thread tpprf(&PreProcessor::Run,pprf);

    PreProcessor *pprt = new PreProcessor_OMD(mci_rt);
    boost::thread tpprt(&PreProcessor::Run,pprt);

    RefreshProcessor *rfp = new RefreshProcessor_OMD(uiChannelID);
    boost::thread trfp(&RefreshProcessor::Run,rfp);

    ExpandableCirBuffer *     pRawCirBuf_RTA = pShrObj->GetRawPktCirBufPtr(mci_rta);
    ExpandableCirBuffer *     pRawCirBuf_RTB = pShrObj->GetRawPktCirBufPtr(mci_rtb);

    ExpandableCirBuffer4Msg * pMsgCirBuf_RT  = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);

    pRawCirBuf_RTA->Reset();
    pRawCirBuf_RTB->Reset();
    pMsgCirBuf_RT->Reset();

    BYTE pbPkt[1000];


    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);

    //--------------------------------------------------
    // RTA Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTA->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTA->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);

    //--------------------------------------------------
    // RTB Data (1st part)
    //--------------------------------------------------
    for (unsigned int i = 1; i < 15; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum   = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);

    //--------------------------------------------------
    // RTA Seq Reset
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Sequence_Reset * osr = (OMD_Sequence_Reset *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMD_Sequence_Reset);
      oph->MsgCount = 1;
      oph->SeqNum = 21;
      oph->SendTime = 8888+21;

      osr->MsgSize  = sizeof(OMD_Sequence_Reset);
      osr->MsgType  = OMD_SEQUENCE_RESET;
      osr->NewSeqNo = 1;

      BYTE *bT = pRawCirBuf_RTA->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTA->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == true);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RTB Data (2nd part)
    //--------------------------------------------------
    for (unsigned int i = 15; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum   = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 22);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RTB Missed Seq Reset
    //--------------------------------------------------


    //--------------------------------------------------
    // RTB New Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // Answer checking
    //--------------------------------------------------
    {
      uint32_t uiLatestSeqNo = 0;

      CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
      CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);

      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 22);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 41);

      CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
      CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    }
  }


  //--------------------------------------------------
  // 1.5 RTA missed OMD_SEQUENCE_RESET, RTB received OMD_SEQUENCE_RESET, RTA leads a lot
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: 1.5" << endl << flush;

    pSysCfg->ReadConfigOptional("Testing/Config-SeqReset1.1.ini"); // reuse

    pShrObj->ResetRTChnlSeqNoOffset();
    pShrObj->ResetRFChnlSeqNoOffset();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    PreProcessor *pprf = new PreProcessor_OMD(mci_rf);
    boost::thread tpprf(&PreProcessor::Run,pprf);

    PreProcessor *pprt = new PreProcessor_OMD(mci_rt);
    boost::thread tpprt(&PreProcessor::Run,pprt);

    RefreshProcessor *rfp = new RefreshProcessor_OMD(uiChannelID);
    boost::thread trfp(&RefreshProcessor::Run,rfp);

    ExpandableCirBuffer *     pRawCirBuf_RTA = pShrObj->GetRawPktCirBufPtr(mci_rta);
    ExpandableCirBuffer *     pRawCirBuf_RTB = pShrObj->GetRawPktCirBufPtr(mci_rtb);

    ExpandableCirBuffer4Msg * pMsgCirBuf_RT  = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);

    pRawCirBuf_RTA->Reset();
    pRawCirBuf_RTB->Reset();
    pMsgCirBuf_RT->Reset();

    BYTE pbPkt[1000];


    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);

    //--------------------------------------------------
    // RTA Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTA->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTA->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RTA Missed Seq Reset
    //--------------------------------------------------

    //--------------------------------------------------
    // RTA New Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTA->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTA->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RTB Data (1st part)
    //--------------------------------------------------
    for (unsigned int i = 1; i < 15; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum   = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RTB Data (2nd part)
    //--------------------------------------------------
    for (unsigned int i = 15; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum   = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RTB Seq Reset
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Sequence_Reset * osr = (OMD_Sequence_Reset *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMD_Sequence_Reset);
      oph->MsgCount = 1;
      oph->SeqNum = 21;
      oph->SendTime = 8888+21;

      osr->MsgSize  = sizeof(OMD_Sequence_Reset);
      osr->MsgType  = OMD_SEQUENCE_RESET;
      osr->NewSeqNo = 1;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == true);
    pShrObj->DeactivateRefresh(uiChannelID);



    //--------------------------------------------------
    // RTB New Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);

    //--------------------------------------------------
    // Answer checking
    //--------------------------------------------------
    {
      uint32_t uiLatestSeqNo = 0;

      CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
      CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);

      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 22);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 41);

      CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
      CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    }

  }

  //--------------------------------------------------
  // 1.6 RTA received OMD_SEQUENCE_RESET, RTB received OMD_SEQUENCE_RESET, RTA leads a lot
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: 1.6" << endl << flush;

    pSysCfg->ReadConfigOptional("Testing/Config-SeqReset1.1.ini"); // reuse

    pShrObj->ResetRTChnlSeqNoOffset();
    pShrObj->ResetRFChnlSeqNoOffset();
    pShrObj->DeactivateRefresh(uiChannelID);
    CPPUNIT_ASSERT(!pShrObj->CheckRefreshActivatnStatus(uiChannelID));

    PreProcessor *pprf = new PreProcessor_OMD(mci_rf);
    boost::thread tpprf(&PreProcessor::Run,pprf);

    PreProcessor *pprt = new PreProcessor_OMD(mci_rt);
    boost::thread tpprt(&PreProcessor::Run,pprt);

    RefreshProcessor *rfp = new RefreshProcessor_OMD(uiChannelID);
    boost::thread trfp(&RefreshProcessor::Run,rfp);

    ExpandableCirBuffer *     pRawCirBuf_RTA = pShrObj->GetRawPktCirBufPtr(mci_rta);
    ExpandableCirBuffer *     pRawCirBuf_RTB = pShrObj->GetRawPktCirBufPtr(mci_rtb);

    ExpandableCirBuffer4Msg * pMsgCirBuf_RT  = pShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, uiChannelID);

    pRawCirBuf_RTA->Reset();
    pRawCirBuf_RTB->Reset();
    pMsgCirBuf_RT->Reset();

    BYTE pbPkt[1000];


    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);

    //--------------------------------------------------
    // RTA Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTA->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTA->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RTB Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize  = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum   = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RTA Seq Reset
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Sequence_Reset * osr = (OMD_Sequence_Reset *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMD_Sequence_Reset);
      oph->MsgCount = 1;
      oph->SeqNum = 21;
      oph->SendTime = 8888+21;

      osr->MsgSize  = sizeof(OMD_Sequence_Reset);
      osr->MsgType  = OMD_SEQUENCE_RESET;
      osr->NewSeqNo = 1;

      BYTE *bT = pRawCirBuf_RTA->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTA->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == true);
    pShrObj->DeactivateRefresh(uiChannelID);


    //--------------------------------------------------
    // RTA New Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTA->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTA->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 21);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // RTB Seq Reset
    //--------------------------------------------------
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMD_Sequence_Reset * osr = (OMD_Sequence_Reset *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMD_Sequence_Reset);
      oph->MsgCount = 1;
      oph->SeqNum = 21;
      oph->SendTime = 8888+21;

      osr->MsgSize  = sizeof(OMD_Sequence_Reset);
      osr->MsgType  = OMD_SEQUENCE_RESET;
      osr->NewSeqNo = 1;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 42);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == true);
    pShrObj->DeactivateRefresh(uiChannelID);


    //--------------------------------------------------
    // RTB New Data
    //--------------------------------------------------
    for (unsigned int i = 1; i < 21; ++i)
    {
      OMD_Packet_Header * oph = (OMD_Packet_Header *) (&pbPkt[0]);
      OMDC_Security_Status * oss = (OMDC_Security_Status *) (&pbPkt[sizeof(OMD_Packet_Header)]);

      oph->PktSize = sizeof(OMD_Packet_Header) + sizeof(OMDC_Security_Status);
      oph->MsgCount = 1;
      oph->SeqNum = i;
      oph->SendTime = 8888+i;

      oss->MsgSize  = sizeof(OMDC_Security_Status);
      oss->MsgType  = OMDC_SECURITY_STATUS;

      BYTE *bT = pRawCirBuf_RTB->GetWritingPtr();
      memcpy(bT,oph,oph->PktSize);
      pRawCirBuf_RTB->PushBack();
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pMsgCirBuf_RT->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTA->Size() << endl << flush;
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << pRawCirBuf_RTB->Size() << endl << flush;
    CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
    CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
    CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 42);
    CPPUNIT_ASSERT(pShrObj->CheckRefreshActivatnStatus(uiChannelID) == false);
    pShrObj->DeactivateRefresh(uiChannelID);

    //--------------------------------------------------
    // Answer checking
    //--------------------------------------------------
    {
      uint32_t uiLatestSeqNo = 0;

      CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 42);
      CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);

      CPPUNIT_ASSERT(pMsgCirBuf_RT->Size() == 20);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetStartSeqNo() == 43);
      CPPUNIT_ASSERT(pMsgCirBuf_RT->GetLatestSeqNo(uiLatestSeqNo));
      CPPUNIT_ASSERT(uiLatestSeqNo == 62);

      CPPUNIT_ASSERT(pRawCirBuf_RTA->Size() == 0);
      CPPUNIT_ASSERT(pRawCirBuf_RTB->Size() == 0);
    }

  }

}

void TestSeqReset::setUp()
{
  return;
}

void TestSeqReset::tearDown()
{
  return;
}

CppUnit::Test *TestSeqReset::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSeqReset");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestSeqReset>(
        "TestSeqReset", 
        &TestSeqReset::RunTest));

  return suiteOfTests;
}
