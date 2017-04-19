#include "ExpandableCirBuffer.h"

ExpandableCirBuffer::ExpandableCirBuffer(
    const unsigned short channelid,
    const unsigned int blocksize,
    const unsigned int rowsize,
    boost::shared_ptr<CentralMemMgr> cmm) :
  m_ChannelID(channelid),
  m_BlockSize(blocksize),
  m_RowSize(rowsize),
  m_MemMgr(cmm)
{
  if (blocksize == 0) return;
  m_TimeSinceEpoch.reset(new boost::posix_time::ptime(gregorian::date(1970,1,1)));
}

ExpandableCirBuffer::~ExpandableCirBuffer()
{
  ResetNoLock();
}

void ExpandableCirBuffer::ExpandCapacity()
{
  m_DqCirBuf.push_back(m_MemMgr->AcquireVectorBlock(m_BlockSize,m_RowSize+TIMESTAMPSIZE));
  m_DqStartIdx.push_back(0);
  m_DqEndIdx.push_back(0);
}

BYTE * ExpandableCirBuffer::GetWritingPtr()
{
  // Sanity check
  if (m_DqCirBuf.empty()) ExpandCapacity();

  BYTE* pT = (*m_DqCirBuf.back())[m_DqEndIdx.back()];
  return (BYTE*)((BYTE*)pT+TIMESTAMPSIZE); //because the first N bytes are designated to time stamp
}

void ExpandableCirBuffer::PushBack()
{
  BYTE * pTmp = (BYTE*)(*m_DqCirBuf.back())[m_DqEndIdx.back()];
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
  boost::posix_time::time_duration diff = now - (*m_TimeSinceEpoch);
  unsigned long ulTS = diff.total_milliseconds();
  WRITE_UINT64(pTmp,ulTS);

  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);
  // boost::unique_lock<boost::recursive_mutex> lock(m_SharedMutex);

  unsigned int uiIdx = (m_DqEndIdx.back()+1)%m_BlockSize;
  m_DqEndIdx.back() = uiIdx;

  // Is there enough room for us to add one element? If not then ExpandCapacity
  if ((uiIdx+1)%m_BlockSize == m_DqStartIdx.back())
    ExpandCapacity();

  m_cvDataAvb.notify_one();
  return;
}

const BYTE* ExpandableCirBuffer::GetReadingPtr()
{
  // Sanity check
  if (Empty()) return NULL;
  BYTE* pT = (*m_DqCirBuf.front())[m_DqStartIdx.front()];
  return (pT+TIMESTAMPSIZE);
}

unsigned long ExpandableCirBuffer::GetTimeStamp()
{
  // Sanity check
  if (Empty()) return 0;

  unsigned long ulTS=0;

  BYTE* pT = (*m_DqCirBuf.front())[m_DqStartIdx.front()];
  memcpy((BYTE*)&ulTS,pT,TIMESTAMPSIZE);
  return ulTS;
}

bool ExpandableCirBuffer::GetReadingPtrTStamp(BYTE* & pArg, unsigned long * ulTS)
{
  // Sanity check
  if (Empty())
  {
    pArg = NULL;
    ulTS = 0;
    return false;
  }
  BYTE* pT = (*m_DqCirBuf.front())[m_DqStartIdx.front()];
  memcpy((BYTE*)ulTS,pT,TIMESTAMPSIZE);
  pArg = (BYTE*)((BYTE*)pT+TIMESTAMPSIZE);
  return true;
}

void ExpandableCirBuffer::PopFront()
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);
  // boost::unique_lock<boost::recursive_mutex> lock(m_SharedMutex);
  PopFrontNoLock();
  return;
}

void ExpandableCirBuffer::PopFrontNoLock()
{
  // Sanity check
  if (EmptyNoLock()) return;

  m_DqStartIdx.front() = (m_DqStartIdx.front()+1)%m_BlockSize;
  if (!EmptyNoLock() && m_DqStartIdx.front() == m_DqEndIdx.front())
  {
    m_MemMgr->ReturnVectorBlock(m_DqCirBuf.front(),m_BlockSize,m_RowSize+TIMESTAMPSIZE);
    m_DqCirBuf.pop_front();
    m_DqStartIdx.pop_front();
    m_DqEndIdx.pop_front();
  }

}

void ExpandableCirBuffer::Purge()
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);
  // boost::unique_lock<boost::recursive_mutex> lock(m_SharedMutex);

  if (m_DqCirBuf.size() == 0) return;
  else if (m_DqCirBuf.size() > 1)
  {
    deque<vector<BYTE*> *>::iterator it;
    deque<vector<BYTE*> *>::iterator itE = m_DqCirBuf.end()-1;

    for (it = m_DqCirBuf.begin(); it != itE; ++it)
      m_MemMgr->ReturnVectorBlock(*it,m_BlockSize,m_RowSize+TIMESTAMPSIZE);

    m_DqCirBuf.erase(m_DqCirBuf.begin(),m_DqCirBuf.end()-1);
    m_DqStartIdx.erase(m_DqStartIdx.begin(),m_DqStartIdx.end()-1);
    m_DqEndIdx.erase(m_DqEndIdx.begin(),m_DqEndIdx.end()-1);
  }

  unsigned int uiSize = SizeNoLock();
  for (unsigned int i = 0; i < uiSize; ++i) PopFrontNoLock();

  return;
}

unsigned int ExpandableCirBuffer::AllocatedSize()
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);
  // boost::shared_lock<boost::recursive_mutex> lock(m_SharedMutex);
  // boost::unique_lock<boost::recursive_mutex> lock(m_SharedMutex);
  return m_DqCirBuf.size()*m_BlockSize*m_RowSize;
}

unsigned int ExpandableCirBuffer::SizeNoLock()
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
unsigned int ExpandableCirBuffer::Size()
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);
  // boost::shared_lock<boost::recursive_mutex> lock(m_SharedMutex);
  // boost::unique_lock<boost::recursive_mutex> lock(m_SharedMutex);
  return SizeNoLock();
}

bool ExpandableCirBuffer::EmptyNoLock()
{
  if (m_DqCirBuf.empty())                              return true;
  else if (m_DqCirBuf.size() > 1)                      return false;
  //i.e. m_DqCirBuf.size() == 1 for cases below
  else if (m_DqStartIdx.front() == m_DqEndIdx.front()) return true;
  else if (m_DqStartIdx.front() != m_DqEndIdx.front()) return false;
  else                                                 return false; // Impossible case
}
bool ExpandableCirBuffer::Empty()
{
  boost::shared_lock<boost::shared_mutex> lock(m_SharedMutex);
  // boost::shared_lock<boost::recursive_mutex> lock(m_SharedMutex);
  // boost::unique_lock<boost::recursive_mutex> lock(m_SharedMutex);
  return EmptyNoLock();
}

void ExpandableCirBuffer::PrintDebugInfo(const unsigned int iId) {
  cout << "Push" << iId << endl << flush;
  cout << " m_DqCirBuf size: " << m_DqCirBuf.size() << endl << flush;
  cout << " start size: " << m_DqStartIdx.size() << endl << flush;
  cout << " end size: " << m_DqEndIdx.size() << endl << flush;
  //cout << " m_DqMemBlockNum size: " << m_DqMemBlockNum.size() << endl << flush;
  for (unsigned int i = 0; i < m_DqStartIdx.size(); ++i) cout << " st: " << m_DqStartIdx[i]; cout << endl << flush;
  for (unsigned int i = 0; i < m_DqEndIdx.size(); ++i) cout << " en: " << m_DqEndIdx[i]; cout << endl << flush;
  //for (unsigned int i = 0; i < m_DqMemBlockNum.size(); ++i) cout << " mem: " << m_DqMemBlockNum[i]; cout << endl << flush;

  return;
}

void ExpandableCirBuffer::Reset()
{
  boost::unique_lock<boost::shared_mutex> lock(m_SharedMutex);
  // boost::unique_lock<boost::recursive_mutex> lock(m_SharedMutex);
  ResetNoLock();
}

void ExpandableCirBuffer::ResetNoLock()
{
  while (!m_DqCirBuf.empty()) {
    m_MemMgr->ReturnVectorBlock(m_DqCirBuf.front(),m_BlockSize,m_RowSize+TIMESTAMPSIZE);
    m_DqCirBuf.pop_front();
  }
  m_DqStartIdx.clear();
  m_DqEndIdx.clear();
  return;
}

void ExpandableCirBuffer::WaitForData()
{
  boost::mutex::scoped_lock lock(m_Mutex);
  m_cvDataAvb.wait(lock); //blocks and returns mutex
  return;
}

void ExpandableCirBuffer::NotifyConsumer()
{
  m_cvDataAvb.notify_one();
}
