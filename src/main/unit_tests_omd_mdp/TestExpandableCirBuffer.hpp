//**************************************************
//  Author:      Sunny Yan
//  Created On:  Mon Apr 14 10:37:45 HKT 2014
//  Description:
//
//**************************************************

#ifndef TESTEXPANDABLECIRBUFFER_H_
#define TESTEXPANDABLECIRBUFFER_H_

#include "Testing/UTest/UTest.h"
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/shared_ptr.hpp>
#include "Container/ExpandableCirBuffer.h"
#include "MemoryManagement/CentralMemMgr.h"
#include "BinaryTools.h"
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;


ExpandableCirBuffer *ecbPkt;
boost::lockfree::queue<unsigned int> ctrlExptPkt(65536);
const unsigned int iIterationsPkt=(rand()%1000)+2000;

void TestExpandableCirBufProducer(UTest ut)
{
  for (unsigned int i = 0; i < iIterationsPkt; ++i)
  {
    unsigned int iRandj = rand() % 50;
    for (unsigned int j = 0; j < iRandj; ++j)
    {
      unsigned int iRand = rand() % 2000;
      //specially arranged to write to ctrlExptPkt first because at our consumer side, we are relying on the existence of ctrlExptPkt
      //ctrlExptPkt.push_back(iRand);
      ctrlExptPkt.push(iRand);
      BYTE * pw = ecbPkt->GetWritingPtr();
      WRITE_UINT32(pw,iRand);
      WRITE_UINT32(pw+4,iRand);
      ecbPkt->PushBack(sizeof(unsigned int)*2);
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));
  }
}

void TestExpandableCirBuffer(UTest & ut)
{
  // For rand num generation
  time_t seconds;
  time(&seconds);
  srand((unsigned int) seconds);

  unsigned int uiCount = 0;

  // cout << "Is lock-free: " << (ctrlExptPkt.is_lock_free() ? 'T' : 'F') << endl;

  ut.Assert(ecbPkt->Empty(),                 __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0,             __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->GetReadingPtr() == NULL, __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Empty(),                 __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0,             __FILE__, __FUNCTION__, __LINE__);
  ecbPkt->GetWritingPtr();
  ut.Assert(ecbPkt->Empty(),                 __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0,             __FILE__, __FUNCTION__, __LINE__);
  ecbPkt->PushBack(7); // just some strange size
  ut.AssertF(ecbPkt->Empty(),                __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 1,             __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->GetPktSize() == 7,       __FILE__, __FUNCTION__, __LINE__);
  ecbPkt->GetReadingPtr();
  ut.AssertF(ecbPkt->Empty(),                __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 1,             __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->GetPktSize() == 7,       __FILE__, __FUNCTION__, __LINE__);
  ecbPkt->PopFront();
  ut.Assert(ecbPkt->Empty(),                 __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0,             __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->GetPktSize() == 0,       __FILE__, __FUNCTION__, __LINE__);

  for (unsigned int i = 0; i < 10; ++i)
  {
    ecbPkt->GetWritingPtr();
    ecbPkt->PushBack(9); // just some strange size
    ut.AssertF(ecbPkt->Empty(),                __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbPkt->Size() == i+1,           __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbPkt->GetPktSize() == 9,       __FILE__, __FUNCTION__, __LINE__);
  }

  ecbPkt->Reset();
  ut.Assert(ecbPkt->Empty(),                   __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0,               __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->GetPktSize() == 0,         __FILE__, __FUNCTION__, __LINE__);

  unsigned int uiRandTimes = (rand() % 50) + 1000;
  unsigned int uiRandTestNum = (rand() % 50) + 31416;
  cout << "uiRandTimes: " << uiRandTimes << endl;
  cout << "uiRandTestNum: " << uiRandTestNum << endl;
  for (unsigned int i = 0; i < uiRandTimes; ++i)
  {
    BYTE * pw = ecbPkt->GetWritingPtr();
    WRITE_UINT32(pw,uiRandTestNum);
    WRITE_UINT32(pw+4,uiRandTestNum);
    ecbPkt->PushBack(sizeof(unsigned int)*2);
    ut.Assert(ecbPkt->Size() == i+1,           __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbPkt->Empty(),                __FILE__, __FUNCTION__, __LINE__);
  }

  for (unsigned int i = 0; i < uiRandTimes; ++i)
  {
    BYTE * pr = NULL;
    unsigned long ulTS2 = 0;
    ecbPkt->GetReadingPtrTStamp(pr,&ulTS2);
    ut.Assert(READ_UINT32(pr) == uiRandTestNum,                 __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr+4) == uiRandTestNum,               __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbPkt->GetPktSize() == sizeof(unsigned int)*2,   __FILE__, __FUNCTION__, __LINE__);
    ecbPkt->PopFront();
    ut.Assert(ecbPkt->Size() == uiRandTimes-1-i,                __FILE__, __FUNCTION__, __LINE__);
    if (i != uiRandTimes-1) ut.AssertF(ecbPkt->Empty(),         __FILE__, __FUNCTION__, __LINE__);
    else ut.Assert(ecbPkt->Empty(),                             __FILE__, __FUNCTION__, __LINE__);
  }

  for (unsigned int i = 0; i < uiRandTimes; ++i)
  {
    BYTE * pw = ecbPkt->GetWritingPtr();
    WRITE_UINT32(pw,uiRandTestNum);
    WRITE_UINT32(pw+4,uiRandTestNum);
    ecbPkt->PushBack(sizeof(unsigned int)*2);
    ut.Assert(ecbPkt->Size() == i+1,                            __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbPkt->Empty(),                                 __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbPkt->GetPktSize() == sizeof(unsigned int)*2,   __FILE__, __FUNCTION__, __LINE__);
  }

  for (unsigned int i = 0; i < uiRandTimes; ++i)
  {
    const BYTE * pr = ecbPkt->GetReadingPtr();
    ut.Assert(READ_UINT32(pr) == uiRandTestNum,                 __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr+4) == uiRandTestNum,               __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbPkt->GetPktSize() == sizeof(unsigned int)*2,   __FILE__, __FUNCTION__, __LINE__);
    ecbPkt->PopFront();
    ut.Assert(ecbPkt->Size() == uiRandTimes-1-i,                __FILE__, __FUNCTION__, __LINE__);
    if (i != uiRandTimes-1) ut.AssertF(ecbPkt->Empty(),         __FILE__, __FUNCTION__, __LINE__);
    else ut.Assert(ecbPkt->Empty(),                             __FILE__, __FUNCTION__, __LINE__);
  }

  //--------------------------------------------------
  // Test Purge
  //--------------------------------------------------
  for (unsigned int i = 0; i < uiRandTimes; i+=(uiRandTimes/5))
  {
    ecbPkt->Reset();
    for (unsigned int j = 0; j < i+1; ++j)
    {
      BYTE * pw = ecbPkt->GetWritingPtr();
      WRITE_UINT32(pw,uiRandTestNum);
      ecbPkt->PushBack(sizeof(unsigned int));
    }
    ut.Assert(ecbPkt->Size() == i+1,                          __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbPkt->GetPktSize() == sizeof(unsigned int),   __FILE__, __FUNCTION__, __LINE__);
    ecbPkt->Purge();
    ut.Assert(ecbPkt->Size() == 0,                            __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbPkt->GetPktSize() == 0,                      __FILE__, __FUNCTION__, __LINE__);
  }
  //--------------------------------------------------

  // Start thread
  boost::thread t(&TestExpandableCirBufProducer, ut);

  for (;;)
  {
    unsigned int iRandj = rand() % 50;
    for (unsigned int j = 0; j < iRandj; ++j)
    {
      const BYTE * pr = ecbPkt->GetReadingPtr();
      unsigned long ulTS = ecbPkt->GetTimeStamp();
      unsigned long ulTS2 = 0;
      BYTE * pr2 = NULL;
      bool bRtn = ecbPkt->GetReadingPtrTStamp(pr2,&ulTS2);
      if (pr != NULL && bRtn)
      {
        //cout << ulTS << " " << ulTS2 << " " << endl;
        //cout << ecbPkt->Size() << " " << ut.ChkResult() << " " << endl;
        //ut.Assert(READ_UINT32(pr) == ctrlExptPkt.front(), __FILE__, __FUNCTION__, __LINE__);
        ut.Assert(ulTS == ulTS2,                            __FILE__, __FUNCTION__, __LINE__);
        if (ulTS != ulTS2) cout << ulTS << ", " << ulTS2 << endl;
        unsigned int iCtrlExpt;
        ctrlExptPkt.pop(iCtrlExpt);
        ut.Assert(READ_UINT32(pr) == iCtrlExpt,             __FILE__, __FUNCTION__, __LINE__);
        ut.Assert(READ_UINT32(pr+4) == iCtrlExpt,           __FILE__, __FUNCTION__, __LINE__);
        ut.Assert(READ_UINT32(pr2) == iCtrlExpt,            __FILE__, __FUNCTION__, __LINE__);
        ut.Assert(READ_UINT32(pr2+4) == iCtrlExpt,          __FILE__, __FUNCTION__, __LINE__);
        ecbPkt->PopFront();
        //ctrlExptPkt.pop_front();
      }
    }
    ++uiCount; if (uiCount == iIterationsPkt) break;
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));
  }

  t.join();

  // Clean up
  unsigned int finalsize = ecbPkt->Size();
  for (unsigned int j = 0; j < finalsize; ++j)
  {
    const BYTE * pr = ecbPkt->GetReadingPtr();
    unsigned long ulTS = ecbPkt->GetTimeStamp();
    BYTE * pr2 = NULL;
    unsigned long ulTS2 = 0;
    bool bRtn = ecbPkt->GetReadingPtrTStamp(pr2,&ulTS2);

    ut.Assert(bRtn,                                 __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ulTS == ulTS2,                        __FILE__, __FUNCTION__, __LINE__);
    unsigned int iCtrlExpt;
    ctrlExptPkt.pop(iCtrlExpt);
    ut.Assert(READ_UINT32(pr) == iCtrlExpt,         __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr+4) == iCtrlExpt,       __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr2) == iCtrlExpt,        __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr2+4) == iCtrlExpt,      __FILE__, __FUNCTION__, __LINE__);
    ecbPkt->PopFront();
    ut.Assert(ecbPkt->Size() == finalsize-1-j,      __FILE__, __FUNCTION__, __LINE__);
  }
  ut.Assert(ecbPkt->Empty() == ctrlExptPkt.empty(), __FILE__, __FUNCTION__, __LINE__);

  return;
}

#endif
