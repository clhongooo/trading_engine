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
#include "OMD_Global.h"
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;


ExpandableCirBuffer *ecbPkt;
boost::lockfree::queue<unsigned int> ctrlExptPkt(65536);
const unsigned int iIterationsPkt=2000;

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
      ecbPkt->PushBack();
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

  cout << "Is lock-free: " << (ctrlExptPkt.is_lock_free() ? 'T' : 'F') << endl;

  ut.Assert(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0, __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->GetReadingPtr() == NULL, __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0, __FILE__, __FUNCTION__, __LINE__);
  ecbPkt->GetWritingPtr();
  ut.Assert(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0, __FILE__, __FUNCTION__, __LINE__);
  ecbPkt->PushBack();
  ut.AssertF(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 1, __FILE__, __FUNCTION__, __LINE__);
  ecbPkt->GetReadingPtr();
  ut.AssertF(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 1, __FILE__, __FUNCTION__, __LINE__);
  ecbPkt->PopFront();
  ut.Assert(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0, __FILE__, __FUNCTION__, __LINE__);

  for (unsigned int i = 0; i < 10; ++i)
  {
    ecbPkt->GetWritingPtr();
    ecbPkt->PushBack();
    ut.AssertF(ecbPkt->Empty(),                __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ecbPkt->Size() == i+1,           __FILE__, __FUNCTION__, __LINE__);
  }

  ecbPkt->Reset();
  ut.Assert(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  ut.Assert(ecbPkt->Size() == 0, __FILE__, __FUNCTION__, __LINE__);

  for (unsigned int i = 0; i < 467; ++i)
  {
    BYTE * pw = ecbPkt->GetWritingPtr();
    WRITE_UINT32(pw,31416);
    WRITE_UINT32(pw+4,31416);
    ecbPkt->PushBack();
    ut.Assert(ecbPkt->Size() == i+1, __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  }

  for (unsigned int i = 0; i < 467; ++i)
  {
    BYTE * pr = NULL;
    unsigned long ulTS2 = 0;
    ecbPkt->GetReadingPtrTStamp(pr,&ulTS2);
    ut.Assert(READ_UINT32(pr) == 31416, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr+4) == 31416, __FILE__, __FUNCTION__, __LINE__);
    ecbPkt->PopFront();
    ut.Assert(ecbPkt->Size() == 467-1-i, __FILE__, __FUNCTION__, __LINE__);
    if (i != 466) ut.AssertF(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
    else ut.Assert(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  }

  for (unsigned int i = 0; i < 465; ++i)
  {
    BYTE * pw = ecbPkt->GetWritingPtr();
    WRITE_UINT32(pw,31416);
    WRITE_UINT32(pw+4,31416);
    ecbPkt->PushBack();
    ut.Assert(ecbPkt->Size() == i+1, __FILE__, __FUNCTION__, __LINE__);
    ut.AssertF(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  }

  for (unsigned int i = 0; i < 465; ++i)
  {
    const BYTE * pr = ecbPkt->GetReadingPtr();
    ut.Assert(READ_UINT32(pr) == 31416, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr+4) == 31416, __FILE__, __FUNCTION__, __LINE__);
    ecbPkt->PopFront();
    ut.Assert(ecbPkt->Size() == 465-1-i, __FILE__, __FUNCTION__, __LINE__);
    if (i != 464) ut.AssertF(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
    else ut.Assert(ecbPkt->Empty(), __FILE__, __FUNCTION__, __LINE__);
  }

  //--------------------------------------------------
  // Test Purge
  //--------------------------------------------------
  for (unsigned int i = 0; i < 500; i+=50)
  {
    ecbPkt->Reset();
    for (unsigned int j = 0; j < i; ++j)
    {
      BYTE * pw = ecbPkt->GetWritingPtr();
      WRITE_UINT32(pw,31416);
      ecbPkt->PushBack();
    }
    ut.Assert(ecbPkt->Size() == i, __FILE__, __FUNCTION__, __LINE__);
    ecbPkt->Purge();
    ut.Assert(ecbPkt->Size() == 0, __FILE__, __FUNCTION__, __LINE__);

  }

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
        ut.Assert(ulTS == ulTS2, __FILE__, __FUNCTION__, __LINE__);
        if (ulTS != ulTS2) cout << ulTS << ", " << ulTS2 << endl;
        unsigned int iCtrlExpt;
        ctrlExptPkt.pop(iCtrlExpt);
        ut.Assert(READ_UINT32(pr) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
        ut.Assert(READ_UINT32(pr+4) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
        ut.Assert(READ_UINT32(pr2) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
        ut.Assert(READ_UINT32(pr2+4) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
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

    ut.Assert(bRtn, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(ulTS == ulTS2, __FILE__, __FUNCTION__, __LINE__);
    unsigned int iCtrlExpt;
    ctrlExptPkt.pop(iCtrlExpt);
    ut.Assert(READ_UINT32(pr) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr+4) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr2) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
    ut.Assert(READ_UINT32(pr2+4) == iCtrlExpt, __FILE__, __FUNCTION__, __LINE__);
    ecbPkt->PopFront();
    ut.Assert(ecbPkt->Size() == finalsize-1-j, __FILE__, __FUNCTION__, __LINE__);
  }
  ut.Assert(ecbPkt->Empty() == ctrlExptPkt.empty(), __FILE__, __FUNCTION__, __LINE__);

  return;
}

#endif
