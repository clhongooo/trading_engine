//**************************************************
//  Author:      Sunny Yan
//  Created On:  Mon Apr 14 10:37:45 HKT 2014
//  Description:
//
//**************************************************

#ifndef TESTEXPANDABLECIRBUFFER4MSG_H_
#define TESTEXPANDABLECIRBUFFER4MSG_H_

#include "Testing/UTest/UTest.h"
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/shared_ptr.hpp>
#include "ExpandableCirBuffer4Msg.h"
#include "CentralMemMgr.h"
#include "BinaryTools.h"
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

ExpandableCirBuffer4Msg *ecbMsg;
ExpandableCirBuffer4Msg *ecbMsg2;
ExpandableCirBuffer4Msg *ecbMsg3;
ExpandableCirBuffer4Msg *ecbMsg4;
ExpandableCirBuffer4Msg *ecbMsg5;
ExpandableCirBuffer4Msg *ecbMsg6;
boost::lockfree::queue<unsigned int> ctrlExptMsg(65536);
const unsigned int iIterationsMsg=20;
const unsigned int MSGSIZE = 100;
char baTestMsg[MSGSIZE];

void TestExpandableCirBuf4MsgProducer(UTest ut)
{
  unsigned int iSeqNoP = 1800;
  //unsigned int iBaseSeqNo = 0;
  for (unsigned int i = 0; i < iIterationsMsg; ++i)
  {
    unsigned int iRandj = rand() % 50;
    for (unsigned int j = 0; j < iRandj; ++j)
    {
      unsigned int iRand = rand() % 2000;
      //specially arranged to write to ctrlExptMsg first because at our consumer side, we are relying on the existence of ctrlExptMsg
      ctrlExptMsg.push(iRand);
      WRITE_UINT32(baTestMsg+2,iRand);
      ecbMsg->PushMsg((BYTE*)baTestMsg,iSeqNoP++,8888,READ_UINT16((BYTE*)baTestMsg));
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(30));
  }
}

void TestExpandableCirBuffer4Msg(UTest & ut)
{
  // For rand num generation
  time_t seconds;
  time(&seconds);
  srand((unsigned int) seconds);

  // init our test message
  strcpy(baTestMsg,"  TESTING...TESTING...TESTING...TESTING...\0");
  WRITE_UINT16(&baTestMsg[0],44);

  // cout << "Is lock-free: " << (ctrlExptMsg.is_lock_free() ? 'T' : 'F') << endl;

  //--------------------------------------------------
  // testing init states 
  //--------------------------------------------------
  {
    uint16_t msgsz;
    uint32_t seqno;
    uint64_t tstamp;
    BYTE *pMsg;
    bool bDirFlag;
    ut.Assert(ecbMsg->Empty(),              __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->Size() == 0,          __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetStartSeqNo() == 0, __FILE__, __FUNCTION__, __LINE__);

    ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1,bDirFlag,tstamp),      __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(999,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),               __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),                __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1) == NULL,                   __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(2) == NULL,                   __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(3) == NULL,                   __FILE__, __FUNCTION__, __LINE__);

    ecbMsg->GetPtrMsgSeqNoTStamp(pMsg,&seqno,&tstamp,&msgsz);
    ut.Assert(ecbMsg->Empty(),              __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->Size() == 0,          __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetStartSeqNo() == 0, __FILE__, __FUNCTION__, __LINE__);

    ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),             __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1) == NULL,                 __FILE__, __FUNCTION__, __LINE__);

    seqno = 999;
    tstamp = 8888;
    ecbMsg->PushMsg((BYTE*)baTestMsg,seqno,tstamp,READ_UINT16((BYTE*)baTestMsg));
    ut.AssertF(ecbMsg->Empty(),             __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->Size() == 999,        __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetStartSeqNo() == 1, __FILE__, __FUNCTION__, __LINE__);

    ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(0,bDirFlag,tstamp),       __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1,bDirFlag,tstamp),        __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(bDirFlag,                                              __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->CheckDirtyFlagTStamp(2,bDirFlag,tstamp),        __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(bDirFlag,                                              __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->CheckDirtyFlagTStamp(998,bDirFlag,tstamp),      __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(bDirFlag,                                              __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->CheckDirtyFlagTStamp(999,bDirFlag,tstamp),      __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(bDirFlag,                                               __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(tstamp == 8888,                                         __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno),                 __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(seqno == 1,                                             __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno),                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(seqno == 998,                                           __FILE__, __FUNCTION__, __LINE__); //should remain unchanged
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(0)     == NULL,                __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1)    == NULL,                __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(2)    == NULL,                __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(5)    == NULL,                __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(998)  == NULL,                __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(999)  == NULL,                __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1000)  == NULL,                __FILE__, __FUNCTION__, __LINE__);

    ut.AssertF(ecbMsg->Empty(),                 __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->Size()          == 999,   __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetStartSeqNo() == 1,     __FILE__, __FUNCTION__, __LINE__);

    for (unsigned int i = 0; i < 998; ++i)
    {
      BYTE * pr;
      ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
      ut.Assert(seqno   == 1+i,   __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp  == 8888,  __FILE__, __FUNCTION__, __LINE__);

      ecbMsg->PopFront();
      ut.AssertF(ecbMsg->Empty(),               __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->Size() == 999-1-i,      __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetStartSeqNo() == 2+i, __FILE__, __FUNCTION__, __LINE__);
    }

    BYTE * pr;
    ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
    ut.Assert(seqno                   == 999,   __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(tstamp                  == 8888,  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(pr[2]                   == 'T',   __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(pr[3]                   == 'E',   __FILE__, __FUNCTION__, __LINE__);

    ecbMsg->PopFront();
    ut.Assert(ecbMsg->Empty(),                 __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->Size() == 0,             __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetStartSeqNo() == 1000, __FILE__, __FUNCTION__, __LINE__);


    ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(0,bDirFlag,tstamp),       __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1,bDirFlag,tstamp),       __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(2,bDirFlag,tstamp),       __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(3,bDirFlag,tstamp),       __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(999,bDirFlag,tstamp),     __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),                __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),                 __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(0)   == NULL,                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1)   == NULL,                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(2)   == NULL,                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(5)   == NULL,                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(999) == NULL,                  __FILE__, __FUNCTION__, __LINE__);

  }


  //--------------------------------------------------
  // Seq insertion without gaps
  //--------------------------------------------------
  for (unsigned int i = 0; i < 467; ++i)
  {
    uint32_t seqno = 1000+i;
    uint64_t tstamp = 888+i;
    WRITE_UINT32(baTestMsg+2,i); // +2 to skip msg size
    ecbMsg->PushMsg((BYTE*)baTestMsg,seqno,tstamp,READ_UINT16((BYTE*)baTestMsg));
    ut.Assert(ecbMsg->Size() == i+1, __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->Empty(), __FILE__, __FUNCTION__, __LINE__);

    {
      bool bDirFlag = false;

      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(0,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(2,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(3,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(999,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(seqno,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(tstamp==888+i, __FILE__, __FUNCTION__, __LINE__);

      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(seqno+1,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);

      ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),  __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(0)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(2)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(5)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(999)     == NULL, __FILE__, __FUNCTION__, __LINE__);

      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(seqno)  == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(seqno+1) == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(seqno+2) == NULL, __FILE__, __FUNCTION__, __LINE__);
    }

  }

  //--------------------------------------------------
  // Check those data
  //--------------------------------------------------
  ut.Assert(ecbMsg->Size() == 467, __FILE__, __FUNCTION__, __LINE__);

  for (unsigned int i = 0; i < 467; ++i)
  {
    BYTE * pr;
    uint16_t msgsz;
    uint32_t seqno;
    uint64_t tstamp;
    StateOfNextSeqNo snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
    ut.Assert(snsn == SEQNO_AVAILABLE, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr+2) == i, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(seqno == 1000+i, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(tstamp == 888+i, __FILE__, __FUNCTION__, __LINE__);
    ecbMsg->PopFront();
    ut.Assert(ecbMsg->Size() == 467-1-i, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetStartSeqNo() == 1000+i+1, __FILE__, __FUNCTION__, __LINE__);
    if (i != 467-1) ut.AssertF(ecbMsg->Empty(), __FILE__, __FUNCTION__, __LINE__);
    else ut.Assert(ecbMsg->Empty(), __FILE__, __FUNCTION__, __LINE__);


    {
      bool bDirFlag = false;

      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(0,bDirFlag,tstamp),        __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1,bDirFlag,tstamp),        __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(2,bDirFlag,tstamp),        __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(3,bDirFlag,tstamp),        __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(999,bDirFlag,tstamp),      __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(seqno,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(seqno-1,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),                 __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),                  __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(0)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(2)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(5)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(999)     == NULL, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(seqno)   == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(seqno-1) == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(seqno-2) == NULL, __FILE__, __FUNCTION__, __LINE__);
    }
  }

  //--------------------------------------------------
  // Insert with gap
  //--------------------------------------------------
  {
    ut.Assert(ecbMsg->Size() == 0, __FILE__, __FUNCTION__, __LINE__);
    uint16_t msgsz;
    uint32_t seqno = 1467;
    uint64_t tstamp = 998;
    WRITE_UINT32(baTestMsg+2,9394);
    ecbMsg->PushMsg((BYTE*)baTestMsg,seqno,tstamp,READ_UINT16((BYTE*)baTestMsg));
    ut.Assert(ecbMsg->Size() == 1, __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->Empty(), __FILE__, __FUNCTION__, __LINE__);

    {
      bool bDirFlag = false;

      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(0,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(2,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(3,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(999,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1467,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 998, __FILE__, __FUNCTION__, __LINE__);

      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1466,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(0)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(2)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(5)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(999)     == NULL, __FILE__, __FUNCTION__, __LINE__);

      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1467)    == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1466)   == NULL,   __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1465)   == NULL,   __FILE__, __FUNCTION__, __LINE__);
    }

    seqno = 1480;
    tstamp = 999;
    WRITE_UINT32(baTestMsg+2,9395);
    ecbMsg->PushMsg((BYTE*)baTestMsg,seqno,tstamp,READ_UINT16((BYTE*)baTestMsg));
    ut.Assert(ecbMsg->Size() == 14, __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->Empty(), __FILE__, __FUNCTION__, __LINE__);

    {
      bool bDirFlag = false;

      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(0,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(2,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(3,bDirFlag,tstamp),    __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(999,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1467,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 998, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1475,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 999, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1478,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 999, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1479,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 999, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1480,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 999, __FILE__, __FUNCTION__, __LINE__);

      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1466,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1481,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1468,              __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479,              __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(0)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(2)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(5)       == NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(999)     == NULL, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1466)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1467)     ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1468)     ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1469)     ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1475)     ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1480)     ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1481)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
    }

    seqno = 1475;
    tstamp = 1000;
    WRITE_UINT32(baTestMsg+2,9396);
    ecbMsg->PushMsg((BYTE*)baTestMsg,seqno,tstamp,READ_UINT16((BYTE*)baTestMsg));
    ut.Assert(ecbMsg->Size() == 14, __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg->Empty(), __FILE__, __FUNCTION__, __LINE__);

    {
      bool bDirFlag = false;

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1467,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 998, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1475,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 1000, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1478,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 999, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1479,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 999, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1480,bDirFlag,tstamp), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 999, __FILE__, __FUNCTION__, __LINE__);

      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1466,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1481,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1468,              __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479,              __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1466)       ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1467)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1468)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1469)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1475)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1480)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1481)       ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
    }

    {
      seqno = 1468;
      tstamp = 1000;
      WRITE_UINT32(baTestMsg+2,9399);
      ecbMsg->PushMsg((BYTE*)baTestMsg,seqno,tstamp,READ_UINT16((BYTE*)baTestMsg));
      ut.Assert(ecbMsg->Size() == 14, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->Empty(), __FILE__, __FUNCTION__, __LINE__);

      bool bDirFlag = false;
      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1467,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 998,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1468,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 1000,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1469, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1467) ==  NULL, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1468) ==  NULL, __FILE__, __FUNCTION__, __LINE__);
    }

    //--------------------------------------------------
    // check results 
    //--------------------------------------------------
    BYTE * pr;

    StateOfNextSeqNo snsn;

    {
      bool bDirFlag = false;
      snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
      ut.Assert(snsn == SEQNO_AVAILABLE, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(READ_UINT32(pr+2) == 9394, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1467, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 998, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1467,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 998,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1469, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1467) ==  NULL, __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1468,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 1000,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1469, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1468) ==  NULL, __FILE__, __FUNCTION__, __LINE__);

      ecbMsg->PopFront();

      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1468,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 1000,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1469, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1468) ==  NULL, __FILE__, __FUNCTION__, __LINE__);

    }

    {
      bool bDirFlag = false;
      snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
      ut.Assert(snsn == SEQNO_AVAILABLE, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(READ_UINT32(pr+2) == 9399, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1468, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 1000, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1468,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 1000,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1469, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1468) ==  NULL, __FILE__, __FUNCTION__, __LINE__);
      ecbMsg->PopFront();
    }

    for (unsigned int i = 0; i < 6; ++i)
    {
      bool bDirFlag = false;
      snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
      ut.Assert(snsn == SEQNO_MISSING, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1469+i,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(bDirFlag,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1469+i, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1469+i) ==  NULL, __FILE__, __FUNCTION__, __LINE__);
      ecbMsg->PopFront();
    }

    {
      bool bDirFlag = false;
      snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
      ut.Assert(snsn == SEQNO_AVAILABLE, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(READ_UINT32(pr+2) == 9396, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1475, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 1000, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1475,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(bDirFlag,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(tstamp == 1000,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1476, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1475) ==  NULL, __FILE__, __FUNCTION__, __LINE__);
      ecbMsg->PopFront();
    }


    for (unsigned int i = 0; i < 4; ++i)
    {
      bool bDirFlag = false;
      snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
      ut.Assert(snsn == SEQNO_MISSING, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->CheckDirtyFlagTStamp(1476+i,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(bDirFlag,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1476+i, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == 1479, __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(1476+i) ==  NULL, __FILE__, __FUNCTION__, __LINE__);
      ecbMsg->PopFront();
    }
    snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
    ut.Assert(snsn == SEQNO_AVAILABLE, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr+2) == 9395, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(seqno == 1480, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(tstamp == 999, __FILE__, __FUNCTION__, __LINE__);
    ecbMsg->PopFront();


    {
      bool bDirFlag = false;

      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1466,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1467,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1475,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1478,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1479,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1480,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(1481,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
      ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),               __FILE__, __FUNCTION__, __LINE__);

      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1466)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1467)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1468)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1469)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1475)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1480)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(1481)      ==  NULL,  __FILE__, __FUNCTION__, __LINE__);

    }
  }

  ut.Assert(ecbMsg->Size() == 0, __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbMsg->Empty(), __FILE__, __FUNCTION__, __LINE__);

  //--------------------------------------------------
  // testing multithreaded producer and consumer
  //--------------------------------------------------
  // Start producer thread
  boost::thread t(&TestExpandableCirBuf4MsgProducer, ut);

  boost::this_thread::sleep(boost::posix_time::milliseconds(1000));


  //--------------------------------------------------
  // Should have some missing seq no between 1481 and 1800 
  //--------------------------------------------------
  {
    uint32_t seqno;
    ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(seqno == 1481, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(seqno == 1799, __FILE__, __FUNCTION__, __LINE__);
  }

  //--------------------------------------------------
  // Our consumer 
  //--------------------------------------------------
  unsigned int iMissingSeqNo = 1481;
  unsigned int iSeqNoC = 1800;
  for (unsigned int i = 0; i < iIterationsMsg; ++i)
  {
    unsigned int iRandj = rand() % 50;
    for (unsigned int j = 0; j < iRandj; ++j)
    {
      BYTE * pr = NULL;
      uint16_t msgsz;
      uint32_t seqno;
      uint64_t tstamp;
      StateOfNextSeqNo snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);

      if (snsn == SEQNO_MISSING || snsn == ALL_RETRIEVED)
      {
        cout << "Consumer: MISSING / ALL_RETRIEVED. SeqNo=" << seqno << ", TS=" << tstamp << endl << flush;
        ecbMsg->PopFront();
        if (seqno >= 1799) //remember that's seq no before popping
        {
          ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
        }
        else
        {
          ut.Assert(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(seqno == ++iMissingSeqNo, __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(seqno == 1799, __FILE__, __FUNCTION__, __LINE__);
        }

        snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);
      }
      else if (snsn == SEQNO_AVAILABLE)
      {
        cout << "Consumer: SeqNo=" << iSeqNoC << ", Size=" << ecbMsg->Size() << endl;

        unsigned int iCtrlExpt;
        ctrlExptMsg.pop(iCtrlExpt);
        ut.Assert(READ_UINT32(pr+2) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
        if (seqno != iSeqNoC)
        {
        }
        ut.Assert(seqno == iSeqNoC++, __FILE__, __FUNCTION__, __LINE__);

        ut.Assert(tstamp == 8888, __FILE__, __FUNCTION__, __LINE__);

        {
          bool bDirFlag = false;
          ut.Assert(ecbMsg->CheckDirtyFlagTStamp(iSeqNoC,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(tstamp == 8888, __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(iSeqNoC) == NULL, __FILE__, __FUNCTION__, __LINE__);
        }

        ecbMsg->PopFront();

        {
          bool bDirFlag = false;
          ut.Assert(ecbMsg->CheckDirtyFlagTStamp(iSeqNoC,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(tstamp == 8888, __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(iSeqNoC) == NULL, __FILE__, __FUNCTION__, __LINE__);
        }

      }
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(50));
  }

  t.join();

  //--------------------------------------------------
  // Clean up 
  //--------------------------------------------------
  unsigned int finalsize = ecbMsg->Size();
  unsigned int j = 0;
  while (!ctrlExptMsg.empty() && ecbMsg->Size() > 0)
  {
    BYTE * pr;
    uint16_t msgsz;
    uint32_t seqno;
    uint64_t tstamp;
    StateOfNextSeqNo snsn = ecbMsg->GetPtrMsgSeqNoTStamp(pr,&seqno,&tstamp,&msgsz);

    if (snsn == SEQNO_AVAILABLE)
    {
      unsigned int iCtrlExpt;

      {
        bool bDirFlag = false;
        if (!ctrlExptMsg.empty())
        {
          bool bDirFlag = false;
          ut.Assert(ecbMsg->CheckDirtyFlagTStamp(iSeqNoC,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(tstamp == 8888, __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(iSeqNoC) == NULL,  __FILE__, __FUNCTION__, __LINE__);
        }
        else
        {
          ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(iSeqNoC,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(iSeqNoC) == NULL,  __FILE__, __FUNCTION__, __LINE__);
        }
      }

      ctrlExptMsg.pop(iCtrlExpt);
      ut.Assert(READ_UINT32(pr+2) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
      ut.Assert(seqno == iSeqNoC, __FILE__, __FUNCTION__, __LINE__); ++iSeqNoC;
      ut.Assert(tstamp == 8888, __FILE__, __FUNCTION__, __LINE__);
      ecbMsg->PopFront();

      {
        bool bDirFlag = false;
        if (!ctrlExptMsg.empty())
        {
          ut.Assert(ecbMsg->CheckDirtyFlagTStamp(iSeqNoC,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(bDirFlag, __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(tstamp == 8888, __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetMsgPtrOfSeqNo(iSeqNoC) == NULL,  __FILE__, __FUNCTION__, __LINE__);
        }
        else
        {
          ut.AssertF(ecbMsg->CheckDirtyFlagTStamp(iSeqNoC,bDirFlag,tstamp),  __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetSmallestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
          ut.AssertF(ecbMsg->GetLargestMissingSeqNo(seqno),              __FILE__, __FUNCTION__, __LINE__);
          ut.Assert(ecbMsg->GetMsgPtrOfSeqNo(iSeqNoC) == NULL,  __FILE__, __FUNCTION__, __LINE__);
        }
      }

      ut.Assert(ecbMsg->Size() == finalsize-1-j, __FILE__, __FUNCTION__, __LINE__);
      ++j;
    }
    else ecbMsg->PopFront();
  }
  ut.Assert(ecbMsg->Empty() == ctrlExptMsg.empty(), __FILE__, __FUNCTION__, __LINE__);

  return;
}

void TestExpandableCirBuffer4Msg2(UTest & ut)
{
  BYTE pbMsg[100];
  WRITE_UINT16(&pbMsg[0],100);

  ut.Assert(ecbMsg2->GetAllMissingSeqNo() == ""               , __FILE__, __FUNCTION__, __LINE__);

  uint32_t i = 1;
  for (i = 1; i < 11; ++i) ecbMsg2->PushMsg(pbMsg,i,8888+i,READ_UINT16((BYTE*)pbMsg));
  ecbMsg2->PushMsg(pbMsg,21,8888+31,READ_UINT16((BYTE*)pbMsg)); // Introduce gap here 

  uint32_t seqno;
  ut.Assert(ecbMsg2->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(seqno == 11                            , __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbMsg2->GetLargestMissingSeqNo(seqno) , __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(seqno == 20                            , __FILE__, __FUNCTION__, __LINE__);

  ut.Assert(ecbMsg2->GetAllMissingSeqNo() == "11-20"          , __FILE__, __FUNCTION__, __LINE__);

  ecbMsg2->PushMsg(pbMsg,31,8888+31,READ_UINT16((BYTE*)pbMsg)); // Introduce gap here 
  ut.Assert(ecbMsg2->GetAllMissingSeqNo() == "11-20,22-30"    , __FILE__, __FUNCTION__, __LINE__);

  ecbMsg2->PushMsg(pbMsg,33,8888+31,READ_UINT16((BYTE*)pbMsg)); // Introduce gap here 
  ut.Assert(ecbMsg2->GetAllMissingSeqNo() == "11-20,22-30,32" , __FILE__, __FUNCTION__, __LINE__);


  return;
}


void TestExpandableCirBuffer4Msg3(UTest & ut)
{
  BYTE pbMsg[100];
  WRITE_UINT16(&pbMsg[0],100);
  uint32_t uiSmlSeqNo;

  for (unsigned int j = 10; j < 4000; j+=19) // make sure it's smaller than 409600, which is the max 1 time allocation set in Main.cpp
  {
    ecbMsg3->PushMsg(pbMsg,1,8888,READ_UINT16((BYTE*)pbMsg));
    ecbMsg3->PushMsg(pbMsg,j,8889,READ_UINT16((BYTE*)pbMsg));

    ut.Assert(ecbMsg3->Size() == j,                               __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg3->Empty(),                                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetStartSeqNo() == 1,                      __FILE__, __FUNCTION__, __LINE__);
    ecbMsg3->GetSmallestMissingSeqNo(uiSmlSeqNo);
    ut.Assert(uiSmlSeqNo == 2,                                    __FILE__, __FUNCTION__, __LINE__);
    ecbMsg3->PurgeMsgB4SeqNoInclusive(0);

    ut.Assert(ecbMsg3->Size() == j,                               __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg3->Empty(),                                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetStartSeqNo() == 1,                      __FILE__, __FUNCTION__, __LINE__);
    ecbMsg3->GetSmallestMissingSeqNo(uiSmlSeqNo);
    ut.Assert(uiSmlSeqNo == 2,                                    __FILE__, __FUNCTION__, __LINE__);
    ecbMsg3->PurgeMsgB4SeqNoInclusive(j/2);

    ut.Assert(ecbMsg3->Size() == j - (j/2),                       __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg3->Empty(),                                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetStartSeqNo() == (j/2 + 1),              __FILE__, __FUNCTION__, __LINE__);
    ecbMsg3->GetSmallestMissingSeqNo(uiSmlSeqNo);
    ut.Assert(uiSmlSeqNo == (j/2)+1,                              __FILE__, __FUNCTION__, __LINE__);
    ecbMsg3->PurgeMsgB4SeqNoInclusive(j-1);

    ut.Assert(ecbMsg3->Size() == 1,                               __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg3->Empty(),                                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetStartSeqNo() == j,                      __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(!ecbMsg3->GetSmallestMissingSeqNo(uiSmlSeqNo),      __FILE__, __FUNCTION__, __LINE__);
    ecbMsg3->PurgeMsgB4SeqNoInclusive(j);

    ut.Assert(ecbMsg3->Size() == 0,                               __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->Empty(),                                   __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetStartSeqNo() == j+1,                    __FILE__, __FUNCTION__, __LINE__);

    ecbMsg3->PushMsg(pbMsg,j+100,9000,READ_UINT16((BYTE*)pbMsg));
    ecbMsg3->GetSmallestMissingSeqNo(uiSmlSeqNo);
    ut.Assert(uiSmlSeqNo == j+1,                                  __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->Size() == 100,                             __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbMsg3->Empty(),                                  __FILE__, __FUNCTION__, __LINE__);
    ecbMsg3->Reset();
    ut.Assert(ecbMsg3->Size() == 0,                               __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->Empty(),                                   __FILE__, __FUNCTION__, __LINE__);
  }

  //--------------------------------------------------
  // Now test big allocations
  //--------------------------------------------------
  uint32_t uiLatestSeqNo;
  uint32_t uiSmlMissSeqNo;
  ecbMsg3->Reset();

  ecbMsg3->PushMsg(pbMsg,1,8888,READ_UINT16((BYTE*)pbMsg));
  unsigned long ulLargeNo = 900000;
  unsigned long ulMaxAlloc = 409600;
  ecbMsg3->PushMsg(pbMsg,ulLargeNo,8888,READ_UINT16((BYTE*)pbMsg));

  ecbMsg3->GetLatestSeqNo(uiLatestSeqNo);

  ut.Assert(ecbMsg3->GetStartSeqNo() == ulLargeNo-ulMaxAlloc+1,   __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbMsg3->Size() == ulMaxAlloc,                        __FILE__, __FUNCTION__, __LINE__); //max 1 time alloc

  for (unsigned int i = 1; i < 51; ++i)
  {
    ecbMsg3->PushMsg(pbMsg,ulLargeNo+i,8888,READ_UINT16((BYTE*)pbMsg));

    ut.Assert(ecbMsg3->GetStartSeqNo() == ulLargeNo-ulMaxAlloc+1, __FILE__, __FUNCTION__, __LINE__); //max 1 time alloc
    ut.Assert(ecbMsg3->Size() == ulMaxAlloc+i,                    __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetLatestSeqNo(uiLatestSeqNo),             __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(uiLatestSeqNo == ulLargeNo+i,                       __FILE__, __FUNCTION__, __LINE__);
  }

  {
    unsigned long ulPreStart    = ecbMsg3->GetStartSeqNo();
    unsigned long ulPreSize     = ecbMsg3->Size();
    unsigned long ulPreLatest   = uiLatestSeqNo;
    unsigned long ulPurgeBefore = (ulLargeNo/2);

    ecbMsg3->PurgeMsgB4SeqNoInclusive(ulPurgeBefore);

    ut.Assert(ecbMsg3->GetStartSeqNo() == ulPreStart,               __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->Size() == ulPreSize,                         __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetLatestSeqNo(uiLatestSeqNo),               __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(uiLatestSeqNo == ulPreLatest,                         __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetSmallestMissingSeqNo(uiSmlMissSeqNo),     __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(uiSmlMissSeqNo == ulPreStart,                         __FILE__, __FUNCTION__, __LINE__);
  }

  {
    unsigned long ulPreStart    = ecbMsg3->GetStartSeqNo();
    unsigned long ulPreSize     = ecbMsg3->Size();
    unsigned long ulPreLatest   = uiLatestSeqNo;
    unsigned long ulPurgeBefore = (ulPreStart+1000);

    ecbMsg3->PurgeMsgB4SeqNoInclusive(ulPurgeBefore);

    ut.Assert(ecbMsg3->GetStartSeqNo() == ulPurgeBefore+1,          __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->Size() == ulPreLatest-(ulPurgeBefore+1)+1,   __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetLatestSeqNo(uiLatestSeqNo),               __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(uiLatestSeqNo == ulPreLatest,                         __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbMsg3->GetSmallestMissingSeqNo(uiSmlMissSeqNo),     __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(uiSmlMissSeqNo == ulPurgeBefore+1,                    __FILE__, __FUNCTION__, __LINE__);
  }

  return;
}

//--------------------------------------------------
// Test Maximum 1 time allocation
// Test the safety check against large chunk of missing seq no
//--------------------------------------------------
void TestExpandableCirBuffer4Msg4(UTest & ut)
{
  BYTE pbMsg[100];
  WRITE_UINT16(&pbMsg[0],100);

  //--------------------------------------------------
  // Starting from scratch
  //--------------------------------------------------
  ecbMsg4->PushMsg(pbMsg,4,8999,READ_UINT16((BYTE*)pbMsg));
  ecbMsg4->PushMsg(pbMsg,4100,9000,READ_UINT16((BYTE*)pbMsg));

  uint32_t seqno;

  ut.Assert(ecbMsg4->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(seqno == 1, __FILE__, __FUNCTION__, __LINE__);

  ut.Assert(ecbMsg4->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(seqno == 4099, __FILE__, __FUNCTION__, __LINE__);

  ut.Assert(ecbMsg4->GetStartSeqNo() == 1, __FILE__, __FUNCTION__, __LINE__);

  ut.Assert(ecbMsg4->GetLatestSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(seqno == 4100, __FILE__, __FUNCTION__, __LINE__);

  ut.Assert(ecbMsg4->Size() == 4100, __FILE__, __FUNCTION__, __LINE__);
  ut.AssertF(ecbMsg4->Empty(), __FILE__, __FUNCTION__, __LINE__);

  // //--------------------------------------------------
  // // Non-Empty
  // //--------------------------------------------------
  // // No purge
  // ecbMsg4->PushMsg(pbMsg,4100+4096-1500,9009,READ_UINT16((BYTE*)pbMsg));
  // ut.Assert(ecbMsg4->Size() == 4096+4096-1500, __FILE__, __FUNCTION__, __LINE__);
  // ut.AssertF(ecbMsg4->Empty(), __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(ecbMsg4->GetLatestSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(seqno == 4100+4096-1500, __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(ecbMsg4->GetStartSeqNo() == 5, __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(ecbMsg4->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(seqno == 4100+4096-1500-1, __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(ecbMsg4->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(seqno == 5, __FILE__, __FUNCTION__, __LINE__);
  //
  // // Purge
  // ecbMsg4->PushMsg(pbMsg,4100+4096-1500+4096+100,9010,READ_UINT16((BYTE*)pbMsg));
  // ut.Assert(ecbMsg4->Size() == 4096, __FILE__, __FUNCTION__, __LINE__);
  // ut.AssertF(ecbMsg4->Empty(), __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(ecbMsg4->GetLatestSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(seqno == 4100+4096-1500+4096+100, __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(ecbMsg4->GetStartSeqNo() == 4100+4096-1500+4096+100-4096+1, __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(ecbMsg4->GetLargestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(seqno == 4100+4096-1500+4096+100-1, __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(ecbMsg4->GetSmallestMissingSeqNo(seqno), __FILE__, __FUNCTION__, __LINE__);
  // ut.Assert(seqno == 4100+4096-1500+4096+100-4096+1, __FILE__, __FUNCTION__, __LINE__);

  return;
}

void TestExpandableCirBuffer4Msg5(UTest & ut)
{

  BYTE *pT = ecbMsg5->GetMsgPtrOfSeqNo(0);
  ut.Assert(pT == NULL, __FILE__, __FUNCTION__, __LINE__);

  pT = ecbMsg5->GetMsgPtrOfSeqNo(1);
  ut.Assert(pT == NULL, __FILE__, __FUNCTION__, __LINE__);

  for (unsigned int i = 1; i < 50; ++i)
  {
    BYTE pbMsg[100];
    WRITE_UINT16(&pbMsg[0],i);

    ecbMsg5->PushMsg(pbMsg,i,i,READ_UINT16((BYTE*)pbMsg));

    for (unsigned int j = 1; j < i; ++j)
    {
      BYTE *pT = ecbMsg5->GetMsgPtrOfSeqNo(j);
      ut.Assert(READ_UINT16(pT) == j, __FILE__, __FUNCTION__, __LINE__);
    }
  }

  return;
}

void TestExpandableCirBuffer4Msg6(UTest & ut)
{
  BYTE pbMsg[100];
  WRITE_UINT16(&pbMsg[0],100);

  for (unsigned int j = 1; j < 4096; j++)
    ecbMsg6->PushMsg(pbMsg,j,j,READ_UINT16((BYTE*)pbMsg));

  ecbMsg6->PurgeMsgB4SeqNoInclusive(-1000);
  ecbMsg6->PurgeMsgB4SeqNoInclusive(-1);
  ecbMsg6->PurgeMsgB4SeqNoInclusive(0);
  ecbMsg6->PurgeMsgB4SeqNoInclusive(100);
  ecbMsg6->PurgeMsgB4SeqNoInclusive(100000);
  ecbMsg6->PurgeMsgB4SeqNoInclusive(500000);
  return;
}

#endif
