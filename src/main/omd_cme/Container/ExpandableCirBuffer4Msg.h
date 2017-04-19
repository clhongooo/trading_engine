//**************************************************
//  Author:      Sunny Yan
//  Created On:  Fri Apr 24 13:10 HKT 2014
//  Description: A circular buffer that will allocate more memory
//               when the current buffer is full rather than over-writing the old data.
//               Array index is the offset from the starting sequence number
//
//               Note that the first DIRTYFLAGSIZE bytes of each row is used as the dirty flag,
//               while the next TIMESTAMPSIZE bytes is used internally for time stamp,
//               in the format of local time since epoch (1970/1/1) in millisec.
//
//**************************************************

#ifndef EXPANDABLECIRBUFFER4MSG_H_
#define EXPANDABLECIRBUFFER4MSG_H_

#include <vector>
#include <set>
#include <deque>
#include <cmath>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "../OMD_Global.h"
#include "../MemoryManagement/CentralMemMgr.h"
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

using namespace std;
using namespace boost;

enum StateOfNextSeqNo { ALL_RETRIEVED, SEQNO_AVAILABLE, SEQNO_MISSING };

class ExpandableCirBuffer4Msg
{
  public:
    enum { DIRTYFLAGSIZE = 1 };
    enum { TIMESTAMPSIZE = 8 };

    ExpandableCirBuffer4Msg(const unsigned short,const unsigned int, const unsigned int, shared_ptr<CentralMemMgr>, const unsigned long);
    ~ExpandableCirBuffer4Msg();
    unsigned int AllocatedSize();
    unsigned int Size();
    StateOfNextSeqNo GetMsgSeqNoTStamp(BYTE* &,uint32_t*,uint64_t*);

    bool      CheckDirtyFlagTStamp(const uint32_t,bool&,uint64_t&);
    void      PushMsg(const BYTE*,const uint32_t,const uint64_t);
    void      PopFront();
    bool      Empty();
    void      PrintDebugInfo(const unsigned int);
    bool      GetSmallestMissingSeqNo(uint32_t&);
    bool      GetLargestMissingSeqNo(uint32_t&);
    bool      GetLatestSeqNo(uint32_t&);
    void      PurgeMsgB4SeqNoInclusive(const uint32_t);
    void      Reset();
    uint32_t  GetStartSeqNo();
    BYTE*     GetMsgPtrOfSeqNo(const uint32_t);
    string    GetAllMissingSeqNo();
    void      WaitForData();
    void      NotifyConsumer();
    uint32_t  GetNextSeqNo();

  private:
    //------------------------------
    unsigned int SizeNoLock();
    bool    LocateSeqNo(const uint32_t, int&, int&, int&, int&);
    void    ExpandCapacity();
    void    PopFrontNoLock();
    void    ResetNoLock();
    bool    EmptyNoLock();
    void    PurgeMsgB4SeqNoInclusiveNoLock(uint32_t);
    BYTE*   GetPtrOfSeqNo(const uint32_t);
    BYTE*   GetPtrOfSeqNoNoLock(const uint32_t);
    BYTE*   GetPtrOfSeqNoXNoLock(const uint32_t);

    unsigned short               m_ChannelID;
    boost::shared_mutex          m_SharedMutex;
    // boost::recursive_mutex      m_Mutex;
    boost::mutex                 m_Mutex;
    boost::condition_variable    m_cvDataAvb;
    // volatile bool                m_bDataAvb;
    uint32_t                     m_StartSeqNo;
    deque<vector<BYTE*> *>       m_DqCirBuf;
    deque<int>                   m_DqStartIdx;
    deque<int>                   m_DqEndIdx;
    unsigned int                 m_BlockSize;
    unsigned int                 m_RowSize;
    set<uint32_t>                m_SetMissingSeqNo;
    unsigned long                m_Max1TimeAlloc;
    //------------------------------
    // System objects
    //------------------------------
    shared_ptr<CentralMemMgr>    m_MemMgr;

};

#endif /* EXPANDABLECIRBUFFER4MSG_H_ */
