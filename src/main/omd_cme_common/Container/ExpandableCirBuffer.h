//**************************************************
//  Author:      Sunny Yan
//  Created On:  Fri Apr 11 16:07:17 HKT 2014
//  Description: A circular buffer that will allocate more memory
//               when the current buffer is full rather than over-writing the old data.
//               Note that the first TIMESTAMPSIZE bytes of each row is used internally for time stamp,
//               in the format of local time since epoch (1970/1/1) in millisec.
//
//**************************************************

#ifndef EXPANDABLECIRBUFFER_H_
#define EXPANDABLECIRBUFFER_H_

#include <vector>
#include <deque>
#include <iostream>
#include "BinaryTools.h"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "CentralMemMgr.h"
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

using namespace std;
using namespace boost;

class ExpandableCirBuffer
{
  public:
    enum { TIMESTAMPSIZE = 8 };

    ExpandableCirBuffer(const unsigned short, const unsigned int, const unsigned int, boost::shared_ptr<CentralMemMgr>);
    ~ExpandableCirBuffer();
    BYTE * GetWritingPtr(); //Returns the pointer to the circular buffer entry that we can insert data to
    void PushBack();
    const BYTE* GetReadingPtr(); //Returns the pointer to the circular buffer entry that we can read data from
    bool GetReadingPtrTStamp(BYTE* &, unsigned long *); //Returns the pointer to the circular buffer entry that we can read data from, as well as the timestamp
    unsigned long GetTimeStamp();
    void PopFront();
    void PopFrontNoLock();
    void Purge();
    unsigned int AllocatedSize();
    unsigned int SizeNoLock();
    unsigned int Size();
    bool EmptyNoLock();
    bool Empty();
    void PrintDebugInfo(const unsigned int);
    void Reset();
    void WaitForData();
    void NotifyConsumer();

  private:
    //------------------------------
    void ExpandCapacity();
    void ResetNoLock();

    unsigned short                         m_ChannelID;
    boost::shared_mutex                    m_SharedMutex;
    // boost::recursive_mutex                    m_Mutex;
    boost::mutex                           m_Mutex;
    boost::condition_variable              m_cvDataAvb;
    // volatile bool                          m_bDataAvb;
    deque<vector<BYTE*> *>                 m_DqCirBuf;
    deque<unsigned int>                    m_DqStartIdx;
    deque<unsigned int>                    m_DqEndIdx;
    unsigned int                           m_BlockSize;
    unsigned int                           m_RowSize;
    scoped_ptr<boost::posix_time::ptime>   m_TimeSinceEpoch;
    //------------------------------
    // System objects
    //------------------------------
    boost::shared_ptr<CentralMemMgr>              m_MemMgr;

};

#endif /* EXPANDABLECIRBUFFER_H_ */
