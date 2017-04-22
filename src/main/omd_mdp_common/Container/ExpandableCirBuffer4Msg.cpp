#include "ExpandableCirBuffer4Msg.h"


ExpandableCirBuffer4Msg::ExpandableCirBuffer4Msg(
    const unsigned short channelid,
    const unsigned int blocksize,
    const unsigned int rowsize,
    boost::shared_ptr<CentralMemMgr> cmm,
    const unsigned long max1timealloc) :
  m_ChannelID(channelid),
  m_StartSeqNo(0),
  m_BlockSize(blocksize),
  m_RowSize(rowsize),
  m_MemMgr(cmm),
  m_Max1TimeAlloc(max1timealloc)
{
  if (blocksize == 0) return;
}

ExpandableCirBuffer4Msg::~ExpandableCirBuffer4Msg()
{
  ResetNoLock();
}

BYTE* ExpandableCirBuffer4Msg::GetPtrOfSeqNoNoLock(const uint32_t iSeqNo)
{
  return GetPtrOfSeqNoXNoLock(iSeqNo);
}

bool ExpandableCirBuffer4Msg::LocateSeqNo(const uint32_t iSeqNo, int& iNoOfItemsInFirstBlk, int& iPseudoStartSeqNo, int& iBlockNo, int& iPosWithinBlock)
{
  if (iSeqNo == 0 || m_DqCirBuf.size() == 0 || iSeqNo > m_StartSeqNo + SizeNoLock()) return false;

  iNoOfItemsInFirstBlk = (m_DqEndIdx.front() + m_BlockSize - m_DqStartIdx.front())%m_BlockSize;
  iPseudoStartSeqNo    = (m_StartSeqNo-(m_BlockSize-1-iNoOfItemsInFirstBlk));
  iBlockNo             = ((iSeqNo - iPseudoStartSeqNo) / (m_BlockSize - 1)) + 1;
  iPosWithinBlock      = iSeqNo - iPseudoStartSeqNo + 1 - (iBlockNo-1)*(m_BlockSize-1);
  return true;
}

//--------------------------------------------------
// This is a private function that is intended to be used internally 
// Also please make sure the SeqNo in the argument is valid
//--------------------------------------------------
BYTE* ExpandableCirBuffer4Msg::GetPtrOfSeqNoXNoLock(const uint32_t iSeqNo)
{
  BYTE* pT = NULL;

  // Sanity check
  if (iSeqNo == 0 || EmptyNoLock() || iSeqNo < m_StartSeqNo || iSeqNo > m_StartSeqNo+SizeNoLock()-1) return NULL;

  if (iSeqNo == m_StartSeqNo) // Fast Match
  {
    pT = &((*(m_DqCirBuf.front()))[m_DqStartIdx.front()][0]);
  }
  else if (m_DqCirBuf.size() > 1)
  {
    int iNoOfItemsInFirstBlk = 0;
    int iPseudoStartSeqNo    = 0;
    int iBlockNo             = 0;
    int iPosWithinBlock      = 0;

    if (!LocateSeqNo(iSeqNo,iNoOfItemsInFirstBlk,iPseudoStartSeqNo,iBlockNo,iPosWithinBlock)) return NULL;

    if (iBlockNo == 1)
    {
      int iIdxOfPseudoStartSeqNo = (m_DqStartIdx[iBlockNo-1] - (m_StartSeqNo-iPseudoStartSeqNo))%m_BlockSize;
      pT = &(*(m_DqCirBuf[iBlockNo-1]))[(iIdxOfPseudoStartSeqNo+iPosWithinBlock-1)%m_BlockSize][0];
    }
    else
    {
      pT = &(*(m_DqCirBuf[iBlockNo-1]))[(m_DqStartIdx[iBlockNo-1]+iPosWithinBlock-1)%m_BlockSize][0];
    }
  }
  else
  {
    pT = &(*(m_DqCirBuf.front()))[(m_DqStartIdx.front()+iSeqNo-m_StartSeqNo)%m_BlockSize][0];
  }
  return pT;
}

BYTE* ExpandableCirBuffer4Msg::GetMsgPtrOfSeqNo(const uint32_t iSeqNo)
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);

  BYTE* bT = GetPtrOfSeqNoNoLock(iSeqNo);

  if (bT) return bT+DIRTYFLAGSIZE+TIMESTAMPSIZE;
  else    return NULL;
}

//--------------------------------------------------
// Return: if there is missing sequence no 
// Return: (arg) the smallest missing seq no
//--------------------------------------------------
bool ExpandableCirBuffer4Msg::GetSmallestMissingSeqNo(uint32_t & iMissingSeqNo)
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);

  if (m_SetMissingSeqNo.empty()) return false;
  else
  {
    set<uint32_t>::iterator it = m_SetMissingSeqNo.begin();
    iMissingSeqNo = *it;
    return true;
  }
}
bool ExpandableCirBuffer4Msg::GetLargestMissingSeqNo(uint32_t & iMissingSeqNo)
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);

  if (m_SetMissingSeqNo.empty()) return false;
  else
  {
    set<uint32_t>::iterator it = m_SetMissingSeqNo.end();
    iMissingSeqNo = *(--it);
    return true;
  }
}
bool ExpandableCirBuffer4Msg::GetLatestSeqNo(uint32_t& iLatestSeqNo)
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);

  if (EmptyNoLock()) return false;
  else
  {
    iLatestSeqNo = m_StartSeqNo + SizeNoLock() - 1;

    return true;
  }
}

string ExpandableCirBuffer4Msg::GetAllMissingSeqNo()
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);

  bool bSkipped = false;
  uint32_t uiLastSeqNo = 0;
  ostringstream oo;
  std::set<uint32_t>::iterator it;
  for (it = m_SetMissingSeqNo.begin(); it != m_SetMissingSeqNo.end(); ++it)
  {
    if (uiLastSeqNo == 0)
    {
      uiLastSeqNo = *it;
      oo << *it;
      bSkipped = false;
    }
    else if (*it > uiLastSeqNo + 1)
    {
      oo << "-";
      oo << uiLastSeqNo << ",";
      oo << *it;
      uiLastSeqNo = *it;
      bSkipped = false;
    }
    else
    {
      uiLastSeqNo = *it;
      bSkipped = true;
    }
  }
  if (bSkipped) oo << "-" << uiLastSeqNo;

  return oo.str();
}

void ExpandableCirBuffer4Msg::ExpandCapacity()
{
  m_DqCirBuf.push_back(m_MemMgr->AcquireVectorBlock(m_BlockSize,m_RowSize+DIRTYFLAGSIZE+TIMESTAMPSIZE));
  m_DqStartIdx.push_back(0);
  m_DqEndIdx.push_back(0);
}

void ExpandableCirBuffer4Msg::PushMsg(const BYTE* pTArg, const uint32_t iSeqNo, uint64_t uiTStamp)
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);

  unsigned short usMsgSize = READ_UINT16(pTArg);
  if (usMsgSize > MAX_OMD_PACKET_SIZE) return; // ignore erroneous msg

  //--------------------------------------------------
  // Just ignore old data
  //--------------------------------------------------
  if (iSeqNo < m_StartSeqNo) return;

  //--------------------------------------------------
  // Just insertion into existing rows of non-empty circular buffer, no buffer expansion needed
  //--------------------------------------------------
  else if (m_StartSeqNo != 0 && !EmptyNoLock() && iSeqNo <= m_StartSeqNo + SizeNoLock() - 1)
  {
    BYTE* pT = GetPtrOfSeqNoNoLock(iSeqNo);

    //--------------------------------------------------
    // Only need to push message when the dirty flag isn't on 
    //--------------------------------------------------
    if (pT[0] != 'T')
    {
      pT[0] = 'T';
      pT += DIRTYFLAGSIZE;
      WRITE_UINT64(pT,uiTStamp);
      pT += TIMESTAMPSIZE;
      memcpy(pT, (BYTE*)pTArg, usMsgSize);
      //--------------------------------------------------
      // Remove from set of empty seq no
      //--------------------------------------------------
      set<uint32_t>::iterator it = m_SetMissingSeqNo.find(iSeqNo);
      if (it != m_SetMissingSeqNo.end()) // element exists
        m_SetMissingSeqNo.erase(it);
    }
    return;
  }
  //--------------------------------------------------
  // Starting from scratch
  //--------------------------------------------------
  else if (m_StartSeqNo == 0 && EmptyNoLock())
  {
    if (m_DqCirBuf.empty()) ExpandCapacity(); //otherwise reuse our circular buffer

    //--------------------------------------------------
    // m_Max1TimeAlloc
    // Safety guard: If the sequence no gap is very large,
    //               our circular buffer will request tons
    //               of memory and our system may crash.
    //--------------------------------------------------
    if (iSeqNo > m_Max1TimeAlloc)
    {
      m_StartSeqNo = iSeqNo - m_Max1TimeAlloc + 1;
    }
    else
    {
      m_StartSeqNo = 1; // seq no must start from 1 otherwise
    }

    if (iSeqNo > 1)
    {
      unsigned int uiStartIdx = (unsigned int)(max((double)1,(double)iSeqNo-(double)m_Max1TimeAlloc+1));
      for (unsigned int i = uiStartIdx; i < iSeqNo; ++i)
      {
        vector<BYTE* >* pvT = m_DqCirBuf.back();
        BYTE* pTmp = &(*pvT)[m_DqEndIdx.back()][0];
        //--------------------------------------------------
        // Mark record as "not dirty", and
        // the timestamp that it "should have been" received, for the use of DataCompletenessInspector
        // also add to our set of Empty seq no
        //--------------------------------------------------
        pTmp[0] = 'F';
        pTmp += DIRTYFLAGSIZE;
        WRITE_UINT64(pTmp,uiTStamp);
        m_DqEndIdx.back() = m_DqEndIdx.back()+1;
        m_SetMissingSeqNo.insert(i); 

        if ((m_DqEndIdx.back()+1)%m_BlockSize == m_DqStartIdx.back())
        {
          ExpandCapacity();

          BYTE* pTmp2 = &(*m_DqCirBuf.back())[m_DqEndIdx.back()][0];
          pTmp2[0] = 'F';
        }

      }
    }
  }
  //--------------------------------------------------
  // Appending at the back, because already excluded the case of insertion
  //--------------------------------------------------
  else if (m_StartSeqNo != 0)
  {
    //--------------------------------------------------
    // Consumer actually has consumed all
    //--------------------------------------------------
    if (m_DqCirBuf.empty()) ExpandCapacity();

    //--------------------------------------------------
    // m_Max1TimeAlloc
    // Safety guard: If the sequence no gap is very large,
    //               our circular buffer will request tons
    //               of memory and our system may crash.
    //
    // We need to make room for new data, therefore need to purge old data
    //--------------------------------------------------
    uint32_t iLatestSeqNo = m_StartSeqNo + SizeNoLock() - 1;
    if (iSeqNo - iLatestSeqNo > m_Max1TimeAlloc)
    {
      ResetNoLock();
      ExpandCapacity();

      m_StartSeqNo = iSeqNo-m_Max1TimeAlloc+1;

      BYTE* pTmp = &(*m_DqCirBuf.back())[m_DqEndIdx.back()][0];
      pTmp[0] = 'F';
      pTmp += DIRTYFLAGSIZE;
      WRITE_UINT64(pTmp,uiTStamp);

      m_DqEndIdx.back() = (m_DqEndIdx.back()+1)%m_BlockSize;
      m_SetMissingSeqNo.insert(m_StartSeqNo); 
    }

    //--------------------------------------------------
    // Prepare for enough space / rows before the actual insertion
    //--------------------------------------------------
    uint32_t iCurEndSeqNo          = m_StartSeqNo + SizeNoLock() - 1; // the last non-empty seq no
    uint32_t iAvbSpaceInLastBuffer = m_BlockSize-1 - abs(m_DqStartIdx.back()-m_DqEndIdx.back());

    // Allocate enough memory blocks necessary for the msg insertion
    // and mark all empty rows before as non-dirty
    while (iCurEndSeqNo + iAvbSpaceInLastBuffer < iSeqNo)
    {
      vector<BYTE* >* pvT = m_DqCirBuf.back();
      unsigned int uiDqEndIdx = m_DqEndIdx.back();
      for (unsigned int i = 0; i < iAvbSpaceInLastBuffer; ++i)
      {
        BYTE* pTmp = &(*pvT)[uiDqEndIdx][0];
        //--------------------------------------------------
        // Mark record as "not dirty", and
        // the timestamp that it "should have been" received, for the use of DataCompletenessInspector
        // also add to our set of Empty seq no
        //--------------------------------------------------
        pTmp[0] = 'F';
        pTmp += DIRTYFLAGSIZE;
        WRITE_UINT64(pTmp,uiTStamp);
        uiDqEndIdx = (uiDqEndIdx+1)%m_BlockSize;
        m_SetMissingSeqNo.insert(++iCurEndSeqNo); 
      }
      m_DqEndIdx.back() = uiDqEndIdx;
      ExpandCapacity();
      iAvbSpaceInLastBuffer = m_BlockSize-1; // since we have a fresh block now
    }

    // Fill in the rest of the empty rows in our very last block
    if (iSeqNo-iCurEndSeqNo-1 > 0)
    {
      vector<BYTE* >* pvT = m_DqCirBuf.back();
      unsigned int uiDqEndIdx = m_DqEndIdx.back();
      unsigned int uiNoOfIterations = iSeqNo-iCurEndSeqNo-1;
      for (unsigned int i = 0; i < uiNoOfIterations; ++i)
      {
        BYTE* pTmp = &(*pvT)[uiDqEndIdx][0];
        //--------------------------------------------------
        // Mark record as "not dirty", and
        // the timestamp that it "should have been" received, for the use of DataCompletenessInspector
        // also add to our set of missing seq no
        //--------------------------------------------------
        pTmp[0] = 'F';
        pTmp += DIRTYFLAGSIZE;
        WRITE_UINT64(pTmp,uiTStamp);
        uiDqEndIdx = (uiDqEndIdx+1)%m_BlockSize;
        m_SetMissingSeqNo.insert(++iCurEndSeqNo); 
      }
      m_DqEndIdx.back() = uiDqEndIdx;
    }
  }

  //--------------------------------------------------
  // Now doing the actual insertion
  //--------------------------------------------------
  BYTE* pTmp = &(*m_DqCirBuf.back())[m_DqEndIdx.back()][0];
  pTmp[0] = 'T';
  pTmp += DIRTYFLAGSIZE;
  WRITE_UINT64(pTmp,uiTStamp);
  pTmp += TIMESTAMPSIZE;
  memcpy(pTmp, (BYTE*)pTArg, usMsgSize);

  unsigned int uiIdx = (m_DqEndIdx.back()+1)%m_BlockSize;
  m_DqEndIdx.back() = uiIdx;

  BYTE* pTmp2 = &(*m_DqCirBuf.back())[uiIdx][0];
  pTmp2[0] = 'F';

  //--------------------------------------------------
  // Is there enough room for us to add one element? If not then ExpandCapacity
  //--------------------------------------------------
  if ((uiIdx+1)%m_BlockSize == m_DqStartIdx.back())
  {
    ExpandCapacity();

    BYTE* pTmp2 = &(*m_DqCirBuf.back())[m_DqEndIdx.back()][0];
    pTmp2[0] = 'F';
  }

  m_cvDataAvb.notify_one();
  return;
}

StateOfNextSeqNo ExpandableCirBuffer4Msg::GetMsgSeqNoTStamp(BYTE* & pArg, uint32_t* uiSeqNo, uint64_t* ulTS)
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);

  // Sanity check
  if (EmptyNoLock())
  {
    *uiSeqNo = m_StartSeqNo;
    return ALL_RETRIEVED;
  }
  // Size() > 0 and ...
  BYTE* pT1 = &(*m_DqCirBuf.front())[m_DqStartIdx.front()][0];
  if (pT1[0] != 'T')
  {
    *uiSeqNo = m_StartSeqNo;
    return SEQNO_MISSING;
  }

  BYTE* pT2 = &(*m_DqCirBuf.front())[m_DqStartIdx.front()][0];
  *uiSeqNo = m_StartSeqNo;
  pT2 += DIRTYFLAGSIZE;
  *ulTS = READ_UINT64(pT2);
  pT2 += TIMESTAMPSIZE;
  pArg = (pT2);
  return SEQNO_AVAILABLE;
}


//--------------------------------------------------
// Return: Whether sequence no is within our circular buffer range
// Return: (arg) Whether dirty flag is T 
// Return: (arg) Timestamp
//--------------------------------------------------
bool ExpandableCirBuffer4Msg::CheckDirtyFlagTStamp(const uint32_t iSeqNo, bool & bDirtyFlag, uint64_t & ulTStamp)
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);

  BYTE* pbTmp = GetPtrOfSeqNoNoLock(iSeqNo);
  if (!pbTmp)
  {
    return false;
  }
  else
  {
    bDirtyFlag = (*pbTmp == 'T' ? true : false);
    ulTStamp = READ_UINT64(pbTmp+DIRTYFLAGSIZE);
    return true;
  }
}

void ExpandableCirBuffer4Msg::PopFront()
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);
  PopFrontNoLock();
}


void ExpandableCirBuffer4Msg::PopFrontNoLock()
{
  // Sanity check
  if (SizeNoLock() == 0) return;

  // if not empty then we can of course pop front
  m_DqStartIdx.front() = (m_DqStartIdx.front()+1)%m_BlockSize;
  if (m_DqStartIdx.front() == m_DqEndIdx.front())
  {
    m_MemMgr->ReturnVectorBlock(m_DqCirBuf.front(),m_BlockSize,m_RowSize+DIRTYFLAGSIZE+TIMESTAMPSIZE);
    m_DqCirBuf.pop_front();
    m_DqStartIdx.pop_front();
    m_DqEndIdx.pop_front();
  }

  m_SetMissingSeqNo.erase(m_StartSeqNo);
  ++m_StartSeqNo;
}

void ExpandableCirBuffer4Msg::PurgeMsgB4SeqNoInclusive(const uint32_t uiSeqNo)
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);
  PurgeMsgB4SeqNoInclusiveNoLock(uiSeqNo);
}

void ExpandableCirBuffer4Msg::PurgeMsgB4SeqNoInclusiveNoLock(uint32_t uiSeqNo)
{
  // Sanity check
  if (EmptyNoLock() || uiSeqNo == 0 || uiSeqNo < m_StartSeqNo) return;

  uint32_t iLatestSeqNo = m_StartSeqNo + SizeNoLock() - 1;
  if (uiSeqNo > iLatestSeqNo) uiSeqNo = iLatestSeqNo;

  if (m_DqCirBuf.size() > 1)
  {
    int iNoOfItemsInFirstBlk = 0;
    int iPseudoStartSeqNo    = 0;
    int iBlockNo             = 0;
    int iPosWithinBlock      = 0;

    if (!LocateSeqNo(uiSeqNo,iNoOfItemsInFirstBlk,iPseudoStartSeqNo,iBlockNo,iPosWithinBlock)) return;

    if (iBlockNo > 1)
    {
      //--------------------------------------------------
      // Process the first block
      //--------------------------------------------------
      m_StartSeqNo += iNoOfItemsInFirstBlk;

      m_MemMgr->ReturnVectorBlock(m_DqCirBuf.front(),m_BlockSize,m_RowSize+DIRTYFLAGSIZE+TIMESTAMPSIZE);
      m_DqCirBuf.pop_front();
      m_DqStartIdx.pop_front();
      m_DqEndIdx.pop_front();

      //--------------------------------------------------
      // Process other (full) blocks
      //--------------------------------------------------
      for (unsigned int i = 2; i < iBlockNo; ++i)
      {
        m_StartSeqNo += (m_BlockSize-1);

        m_MemMgr->ReturnVectorBlock(m_DqCirBuf.front(),m_BlockSize,m_RowSize+DIRTYFLAGSIZE+TIMESTAMPSIZE);
        m_DqCirBuf.pop_front();
        m_DqStartIdx.pop_front();
        m_DqEndIdx.pop_front();
      }
    }
  }
  //--------------------------------------------------
  // Now in the last block
  //--------------------------------------------------
  while (uiSeqNo >= m_StartSeqNo)
    PopFrontNoLock();

  //--------------------------------------------------
  // Erase Missing Seq No that are purged
  //--------------------------------------------------
  if (!m_SetMissingSeqNo.empty())
  {
    set<uint32_t>::iterator itMissingSeqNoStart;
    itMissingSeqNoStart = m_SetMissingSeqNo.begin();
    if (*itMissingSeqNoStart < m_StartSeqNo)
    {
      set<uint32_t>::iterator itMissingSeqNoEnd = m_SetMissingSeqNo.upper_bound(m_StartSeqNo);
      if (itMissingSeqNoEnd == m_SetMissingSeqNo.end())
      {
        m_SetMissingSeqNo.clear();
      }
      else if (itMissingSeqNoEnd != itMissingSeqNoStart)
      {
        --itMissingSeqNoEnd;
        m_SetMissingSeqNo.erase(itMissingSeqNoStart,itMissingSeqNoEnd);
      }
      else 
      {
        m_SetMissingSeqNo.erase(itMissingSeqNoStart,itMissingSeqNoStart);
      }
    }
  }

  return;
}

unsigned int ExpandableCirBuffer4Msg::AllocatedSize()
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);
  return m_DqCirBuf.size()*m_BlockSize*m_RowSize;
}

unsigned int ExpandableCirBuffer4Msg::Size()
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);

  return SizeNoLock();
}

unsigned int ExpandableCirBuffer4Msg::SizeNoLock()
{
  unsigned int iTot = 0;
  for (unsigned int i = 0; i < m_DqStartIdx.size(); ++i)
  {
    if (m_DqEndIdx[i] > m_DqStartIdx[i])
      iTot += (m_DqEndIdx[i] - m_DqStartIdx[i]);
    else if (m_DqEndIdx[i] < m_DqStartIdx[i])
      iTot += (m_DqEndIdx[i] + m_BlockSize - m_DqStartIdx[i]);
  }

  return iTot;
}

bool ExpandableCirBuffer4Msg::Empty()
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);
  return EmptyNoLock();

}

bool ExpandableCirBuffer4Msg::EmptyNoLock()
{
  if (m_DqCirBuf.empty())                              return true;
  else if (m_DqCirBuf.size() > 1)                      return false;
  //i.e. m_DqCirBuf.size() == 1 for cases below
  else if (m_DqStartIdx.front() == m_DqEndIdx.front()) return true;
  else if (m_DqStartIdx.front() != m_DqEndIdx.front()) return false;
  else                                                 return false; // Impossible case
}

void ExpandableCirBuffer4Msg::Reset()
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);
  ResetNoLock();
}

void ExpandableCirBuffer4Msg::ResetNoLock()
{
  while (!m_DqCirBuf.empty()) {
    m_MemMgr->ReturnVectorBlock(m_DqCirBuf.front(),m_BlockSize,m_RowSize+DIRTYFLAGSIZE+TIMESTAMPSIZE);
    m_DqCirBuf.pop_front();
  }
  m_DqStartIdx.clear();
  m_DqEndIdx.clear();
  m_SetMissingSeqNo.clear();
  m_StartSeqNo = 0;
  return;
}

uint32_t ExpandableCirBuffer4Msg::GetStartSeqNo()
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);
  return m_StartSeqNo;
}


void ExpandableCirBuffer4Msg::PrintDebugInfo(const unsigned int iId) {
  cout << "------------------------------------------" << endl << flush;
  cout << "PrintDebugInfo(" << iId << ") : Start seq no = " << m_StartSeqNo << endl << flush;
  cout << " m_DqCirBuf size: " << m_DqCirBuf.size() << endl << flush;
  cout << " start size: " << m_DqStartIdx.size() << endl << flush;
  cout << " end size: " << m_DqEndIdx.size() << endl << flush;
  for (unsigned int i = 0; i < m_DqStartIdx.size(); ++i) cout << " st: " << m_DqStartIdx[i]; cout << endl << flush;
  for (unsigned int i = 0; i < m_DqEndIdx.size(); ++i) cout << " en: " << m_DqEndIdx[i]; cout << endl << flush;
  //--------------------------------------------------
  // Print first block dirty flag and timestamp as well 
  //--------------------------------------------------
  if (!m_DqCirBuf.empty())
  {
    cout << " first block content" << endl << flush;
    for (unsigned int i = 0;  i < m_BlockSize; ++i)
    {
      cout  << "[" << i <<  "] " << (((*m_DqCirBuf.front())[i])[0] == 'T' ? "T" : "F") << " : ";
      cout  << READ_UINT64((*m_DqCirBuf.front())[i]+DIRTYFLAGSIZE) << endl << flush;
    }
  }
  return;
}

void ExpandableCirBuffer4Msg::WaitForData()
{
  boost::mutex::scoped_lock lock(m_Mutex);
  m_cvDataAvb.wait(lock); //blocks and returns mutex
  return;
}

void ExpandableCirBuffer4Msg::NotifyConsumer()
{
  m_cvDataAvb.notify_one();
}

uint32_t ExpandableCirBuffer4Msg::GetNextSeqNo()
{
  uint32_t uiLatestSeqNo = 0;

  if (!GetLatestSeqNo(uiLatestSeqNo)) return m_StartSeqNo;
  else return uiLatestSeqNo+1;
}


