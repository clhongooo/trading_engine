#include "SharedObjects.h"

boost::weak_ptr<SharedObjects> SharedObjects::m_pInstance;

SharedObjects::SharedObjects() : m_ProgramStartTime(boost::posix_time::microsec_clock::local_time())
{
  m_SysCfg = SystemConfig::Instance();
  m_Logger = Logger::Instance();
  m_MemMgr = CentralMemMgr::Instance();

  InitializeSharedObjects(true);
  ResetObjects();

  SetProgramStartTime();
}

SharedObjects::~SharedObjects()
{
  DestroyObjects();
}

boost::shared_ptr<SharedObjects> SharedObjects::Instance()
{
  boost::shared_ptr<SharedObjects> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new SharedObjects());
    m_pInstance = instance;
  }
  return instance;
}


void SharedObjects::ResetObjectsPurgeRawCirBuf()
{
  for (int i = 0; i < m_RawPktCirBuf.size(); ++i)
  {
    if (m_RawPktCirBuf[i])
      m_RawPktCirBuf[i]->Purge(); //Purge
  }
  for (int i = 0; i < m_MsgCirBuf.size(); ++i)
  {
    if (m_MsgCirBuf[i])
      m_MsgCirBuf[i]->Reset();
  }
  for (int i = 0; i < m_RTSeqResetOffset.size(); ++i)
  {
    if (m_RTSeqResetOffset[i])
      m_RTSeqResetOffset[i]->clear();
  }
  for (int i = 0; i < m_RFSeqResetOffset.size(); ++i)
  {
    if (m_RFSeqResetOffset[i])
      m_RFSeqResetOffset[i]->clear();
  }
  for (int i = 0; i < m_RTSeqResetAlreadyHandled.size(); ++i)
  {
    if (m_RTSeqResetAlreadyHandled[i])
      m_RTSeqResetAlreadyHandled[i]->clear();
  }
  for (int i = 0; i < m_RFSeqResetAlreadyHandled.size(); ++i)
  {
    if (m_RFSeqResetAlreadyHandled[i])
      m_RFSeqResetAlreadyHandled[i]->clear();
  }
  m_OrderBookCache->ResetAllOrderBooks();
  for (int i = 0; i < m_OrderBookIDInEachChnl.size(); ++i)
  {
    if (m_OrderBookIDInEachChnl[i])
      m_OrderBookIDInEachChnl[i]->clear();
  }
  for (unsigned int i = 0; i < m_RefreshActivated.size(); ++i)
    m_RefreshActivated[i] = 0; // this is not immediately after system start, no need to turn on refresh mode.

  m_bCapTest = false;
  m_NumOfRTSConn = 0;
  m_SysState = SharedObjects::UP;
  return;
}

void SharedObjects::ResetObjects()
{
  ResetObjectsPurgeRawCirBuf();
  for (int i = 0; i < m_RawPktCirBuf.size(); ++i)
  {
    if (m_RawPktCirBuf[i])
      m_RawPktCirBuf[i]->Reset();
  }
  m_SysState = SharedObjects::UP;
  return;
}

void SharedObjects::DestroyObjects()
{
  m_RawPktCirBuf.clear();
  m_MsgCirBuf.clear();
  for (int i = 0; i < m_RTSeqResetOffset.size(); ++i)
  {
    if (m_RTSeqResetOffset[i])
    {
      delete m_RTSeqResetOffset[i];
      m_RTSeqResetOffset[i] = NULL;
    }
  }
  for (int i = 0; i < m_RFSeqResetOffset.size(); ++i)
  {
    if (m_RFSeqResetOffset[i])
    {
      delete m_RFSeqResetOffset[i];
      m_RFSeqResetOffset[i] = NULL;
    }
  }
  for (int i = 0; i < m_RTSeqResetOffsetMutex.size(); ++i)
  {
    if (m_RTSeqResetOffsetMutex[i])
    {
      delete m_RTSeqResetOffsetMutex[i];
      m_RTSeqResetOffsetMutex[i] = NULL;
    }
  }
  for (int i = 0; i < m_RFSeqResetOffsetMutex.size(); ++i)
  {
    if (m_RFSeqResetOffsetMutex[i])
    {
      delete m_RFSeqResetOffsetMutex[i];
      m_RFSeqResetOffsetMutex[i] = NULL;
    }
  }
  for (int i = 0; i < m_RTSeqResetAlreadyHandled.size(); ++i)
  {
    if (m_RTSeqResetAlreadyHandled[i])
    {
      delete m_RTSeqResetAlreadyHandled[i];
      m_RTSeqResetAlreadyHandled[i] = NULL;
    }
  }
  for (int i = 0; i < m_RFSeqResetAlreadyHandled.size(); ++i)
  {
    if (m_RFSeqResetAlreadyHandled[i])
    {
      delete m_RFSeqResetAlreadyHandled[i];
      m_RFSeqResetAlreadyHandled[i] = NULL;
    }
  }
  for (int i = 0; i < m_RTSeqResetAlreadyHandledMutex.size(); ++i)
  {
    if (m_RTSeqResetAlreadyHandledMutex[i])
    {
      delete m_RTSeqResetAlreadyHandledMutex[i];
      m_RTSeqResetAlreadyHandledMutex[i] = NULL;
    }
  }
  for (int i = 0; i < m_RFSeqResetAlreadyHandledMutex.size(); ++i)
  {
    if (m_RFSeqResetAlreadyHandledMutex[i])
    {
      delete m_RFSeqResetAlreadyHandledMutex[i];
      m_RFSeqResetAlreadyHandledMutex[i] = NULL;
    }
  }
  m_OrderBookCache->DeleteAllOrderBooks();
  for (int i = 0; i < m_OrderBookIDInEachChnl.size(); ++i)
  {
    if (m_OrderBookIDInEachChnl[i])
    {
      delete m_OrderBookIDInEachChnl[i];
      m_OrderBookIDInEachChnl[i] = NULL;
    }
  }
  for (int i = 0; i < m_OrderBookIDInEachChnlMutex.size(); ++i)
  {
    if (m_OrderBookIDInEachChnlMutex[i])
    {
      delete m_OrderBookIDInEachChnlMutex[i];
      m_OrderBookIDInEachChnlMutex[i] = NULL;
    }
  }
  for (int i = 0; i < m_RefreshActnMutex.size(); ++i)
  {
    if (m_RefreshActnMutex[i])
    {
      delete m_RefreshActnMutex[i];
      m_RefreshActnMutex[i] = NULL;
    }
  }
}

void SharedObjects::InitializeSharedObjects(bool bFirstUse)
{
  if (bFirstUse)
  {
    for (int i = 0; i < McastIdentifier::GetMaxHash(); ++i)
    {
      m_RawPktCirBuf.push_back(boost::shared_ptr<ExpandableCirBuffer>());
      m_MsgCirBuf.push_back(boost::shared_ptr<ExpandableCirBuffer4Msg>());
      m_RefreshActivated.push_back(1); // initialize to 1 meaning refresh mode is on on system start
      m_RefreshActnMutex.push_back(NULL);
      m_RTSeqResetOffset.push_back(NULL);
      m_RTSeqResetOffsetMutex.push_back(NULL);
      m_RFSeqResetOffset.push_back(NULL);
      m_RFSeqResetOffsetMutex.push_back(NULL);
      m_RTSeqResetAlreadyHandled.push_back(NULL);
      m_RFSeqResetAlreadyHandled.push_back(NULL);
      m_RTSeqResetAlreadyHandledMutex.push_back(NULL);
      m_RFSeqResetAlreadyHandledMutex.push_back(NULL);
      m_OrderBookIDInEachChnl.push_back(NULL);
      m_OrderBookIDInEachChnlMutex.push_back(NULL);
    }
  }

  boost::shared_ptr<vector<McastIdentifier> > vMcastId = m_SysCfg->GetMcastIdentifiers();
  for (int i = 0; i < vMcastId->size(); ++i)
  {
    m_RawPktCirBuf[(*vMcastId)[i].Hash_MC()].reset(new ExpandableCirBuffer((*vMcastId)[i].Channel_ID(),m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr));

    if (!m_MsgCirBuf[(*vMcastId)[i].Hash_Chnl()])
      m_MsgCirBuf[(*vMcastId)[i].Hash_Chnl()].reset(new ExpandableCirBuffer4Msg((*vMcastId)[i].Channel_ID(),m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr,m_SysCfg->GetMaxOneTimeAlloc()));
  }

  boost::shared_ptr<vector<unsigned short> > vActvChnl = m_SysCfg->GetActiveMcastChnl();
  for (int i = 0; i < vActvChnl->size(); ++i)
  {
    if (!m_RefreshActnMutex[(*vActvChnl)[i]])
      m_RefreshActnMutex[(*vActvChnl)[i]] = new boost::shared_mutex();

    if (!m_RTSeqResetOffset[(*vActvChnl)[i]])
      m_RTSeqResetOffset[(*vActvChnl)[i]] = new vector<uint32_t>();

    if (!m_RTSeqResetOffsetMutex[(*vActvChnl)[i]])
      m_RTSeqResetOffsetMutex[(*vActvChnl)[i]] = new boost::shared_mutex();

    if (!m_RFSeqResetOffset[(*vActvChnl)[i]])
      m_RFSeqResetOffset[(*vActvChnl)[i]] = new vector<uint32_t>();

    if (!m_RFSeqResetOffsetMutex[(*vActvChnl)[i]])
      m_RFSeqResetOffsetMutex[(*vActvChnl)[i]] = new boost::shared_mutex();

    if (!m_RTSeqResetAlreadyHandled[(*vActvChnl)[i]])
      m_RTSeqResetAlreadyHandled[(*vActvChnl)[i]] = new vector<long>();

    if (!m_RFSeqResetAlreadyHandled[(*vActvChnl)[i]])
      m_RFSeqResetAlreadyHandled[(*vActvChnl)[i]] = new vector<long>();

    if (!m_RTSeqResetAlreadyHandledMutex[(*vActvChnl)[i]])
      m_RTSeqResetAlreadyHandledMutex[(*vActvChnl)[i]] = new boost::shared_mutex();

    if (!m_RFSeqResetAlreadyHandledMutex[(*vActvChnl)[i]])
      m_RFSeqResetAlreadyHandledMutex[(*vActvChnl)[i]] = new boost::shared_mutex();

    if (!m_OrderBookIDInEachChnl[(*vActvChnl)[i]])
      m_OrderBookIDInEachChnl[(*vActvChnl)[i]] = new set<unsigned long>();

    if (!m_OrderBookIDInEachChnlMutex[(*vActvChnl)[i]])
      m_OrderBookIDInEachChnlMutex[(*vActvChnl)[i]] = new boost::shared_mutex();

  }

  //--------------------------------------------------
  // Order Books
  //--------------------------------------------------
  m_OrderBookCache.reset(new OrderBookCache(m_SysCfg->GetIdentity()));

  //--------------------------------------------------
  // For unit testing
  //--------------------------------------------------
  unsigned short usChannelID = 91;
  {
    McastIdentifier mci_rta( usChannelID, McastIdentifier::REALTIME, McastIdentifier::A);
    McastIdentifier mci_rtb( usChannelID, McastIdentifier::REALTIME, McastIdentifier::B);
    McastIdentifier mci_rfa( usChannelID, McastIdentifier::REFRESH,  McastIdentifier::A);
    McastIdentifier mci_rfb( usChannelID, McastIdentifier::REFRESH,  McastIdentifier::B);

    m_RawPktCirBuf[mci_rta.Hash_MC  ()].reset(new ExpandableCirBuffer     (usChannelID,m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr));
    m_MsgCirBuf   [mci_rta.Hash_Chnl()].reset(new ExpandableCirBuffer4Msg (usChannelID,m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr,m_SysCfg->GetMaxOneTimeAlloc()));
    m_RawPktCirBuf[mci_rfa.Hash_MC  ()].reset(new ExpandableCirBuffer     (usChannelID,m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr));
    m_MsgCirBuf   [mci_rfa.Hash_Chnl()].reset(new ExpandableCirBuffer4Msg (usChannelID,m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr,m_SysCfg->GetMaxOneTimeAlloc()));

    m_RawPktCirBuf[mci_rtb.Hash_MC  ()].reset(new ExpandableCirBuffer     (usChannelID,m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr));
    m_MsgCirBuf   [mci_rtb.Hash_Chnl()].reset(new ExpandableCirBuffer4Msg (usChannelID,m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr,m_SysCfg->GetMaxOneTimeAlloc()));
    m_RawPktCirBuf[mci_rfb.Hash_MC  ()].reset(new ExpandableCirBuffer     (usChannelID,m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr));
    m_MsgCirBuf   [mci_rfb.Hash_Chnl()].reset(new ExpandableCirBuffer4Msg (usChannelID,m_SysCfg->GetMemoryBlockSize(),MAX_OMD_PACKET_SIZE,m_MemMgr,m_SysCfg->GetMaxOneTimeAlloc()));

    if (!m_RefreshActnMutex[usChannelID])
      m_RefreshActnMutex[usChannelID] = new boost::shared_mutex();

    if (!m_RTSeqResetOffset[usChannelID])
      m_RTSeqResetOffset[usChannelID] = new vector<uint32_t>();

    if (!m_RTSeqResetOffsetMutex[usChannelID])
      m_RTSeqResetOffsetMutex[usChannelID] = new boost::shared_mutex();

    if (!m_RFSeqResetOffset[usChannelID])
      m_RFSeqResetOffset[usChannelID] = new vector<uint32_t>();

    if (!m_RFSeqResetOffsetMutex[usChannelID])
      m_RFSeqResetOffsetMutex[usChannelID] = new boost::shared_mutex();

    if (!m_RTSeqResetAlreadyHandled[usChannelID])
      m_RTSeqResetAlreadyHandled[usChannelID] = new vector<long>();

    if (!m_RFSeqResetAlreadyHandled[usChannelID])
      m_RFSeqResetAlreadyHandled[usChannelID] = new vector<long>();

    if (!m_RTSeqResetAlreadyHandledMutex[usChannelID])
      m_RTSeqResetAlreadyHandledMutex[usChannelID] = new boost::shared_mutex();

    if (!m_RFSeqResetAlreadyHandledMutex[usChannelID])
      m_RFSeqResetAlreadyHandledMutex[usChannelID] = new boost::shared_mutex();

    if (!m_OrderBookIDInEachChnl[usChannelID])
      m_OrderBookIDInEachChnl[usChannelID] = new set<unsigned long>();

    if (!m_OrderBookIDInEachChnlMutex[usChannelID])
      m_OrderBookIDInEachChnlMutex[usChannelID] = new boost::shared_mutex();

  }
  return;
}

ExpandableCirBuffer * SharedObjects::GetRawPktCirBufPtr(const McastIdentifier & mi)
{
  return m_RawPktCirBuf[mi.Hash_MC()].get();
}

ExpandableCirBuffer4Msg * SharedObjects::GetMsgCirBufPtr(McastIdentifier::EMcastType type, const unsigned short id)
{
  McastIdentifier mci(id, type, McastIdentifier::A);
  return m_MsgCirBuf[mci.Hash_Chnl()].get();
}

//--------------------------------------------------
// RTS limit of max num of connections per day
//--------------------------------------------------
void SharedObjects::AddCountNumOfRTSConn()
{
  boost::unique_lock<boost::shared_mutex> lock(m_RTSNumOfConnMutex);
  ++m_NumOfRTSConn;
  return;
}

unsigned int SharedObjects::GetCountNumOfRTSConn()
{
  boost::shared_lock<boost::shared_mutex> lock(m_RTSNumOfConnMutex);
  return m_NumOfRTSConn;
}

void SharedObjects::ResetCountNumOfRTSConn()
{
  boost::unique_lock<boost::shared_mutex> lock(m_RTSNumOfConnMutex);
  m_NumOfRTSConn = 0;
  return;
}

//--------------------------------------------------
// Refresh Activation
//--------------------------------------------------
void SharedObjects::ActivateRefresh(unsigned short usChnl)
{
  boost::unique_lock<boost::shared_mutex> lock(*(m_RefreshActnMutex[usChnl]));
  m_RefreshActivated[usChnl] = abs(m_RefreshActivated[usChnl]) + 1;
  return;
}

void SharedObjects::DeactivateRefresh(unsigned short usChnl)
{
  boost::unique_lock<boost::shared_mutex> lock(*(m_RefreshActnMutex[usChnl]));
  m_RefreshActivated[usChnl] = -abs(m_RefreshActivated[usChnl]);
  return;
}

bool SharedObjects::CheckRefreshActivatnStatus(unsigned short usChnl)
{
  boost::shared_lock<boost::shared_mutex> lock(*(m_RefreshActnMutex[usChnl]));
  return (m_RefreshActivated[usChnl] > 0 ? true : false);
}

unsigned short SharedObjects::CheckRefreshActivatnStatusCount(unsigned short usChnl)
{
  boost::shared_lock<boost::shared_mutex> lock(*(m_RefreshActnMutex[usChnl]));
  return abs(m_RefreshActivated[usChnl]);
}


//--------------------------------------------------
// OMD Readiness Test: Session 4: Capacity test
//--------------------------------------------------
void SharedObjects::TurnOnCapTest()
{
  m_bCapTest = true;
}
void SharedObjects::TurnOffCapTest()
{
  m_bCapTest = false;
}
bool SharedObjects::CheckCapTestStatus()
{
  return m_bCapTest;
}



//--------------------------------------------------
// Handling OMD_SEQUENCE_RESET
//--------------------------------------------------
uint32_t SharedObjects::CheckRTChnlLatestSeqNoOffset(const unsigned short channelid)
{
  boost::shared_lock<boost::shared_mutex> lock(*m_RTSeqResetOffsetMutex[channelid]);

  uint32_t uiReturnOffset = 0;

  if (m_RTSeqResetOffset[channelid] && !m_RTSeqResetOffset[channelid]->empty())
  {
    int iLoc = m_RTSeqResetOffset[channelid]->size() - 1;
    uiReturnOffset = (*m_RTSeqResetOffset[channelid])[iLoc];
  }

  return uiReturnOffset;
}

uint32_t SharedObjects::CheckRFChnlLatestSeqNoOffset(const unsigned short channelid)
{
  boost::shared_lock<boost::shared_mutex> lock(*m_RFSeqResetOffsetMutex[channelid]);

  uint32_t uiReturnOffset = 0;

  if (m_RFSeqResetOffset[channelid] && !m_RFSeqResetOffset[channelid]->empty())
  {
    int iLoc = m_RFSeqResetOffset[channelid]->size() - 1;
    uiReturnOffset = (*m_RFSeqResetOffset[channelid])[iLoc];
  }

  return uiReturnOffset;
}

bool SharedObjects::CheckRTSeqResetAlreadyHandled(const unsigned short channelid, const long ulTStamp)
{
  boost::shared_lock<boost::shared_mutex> lock(*m_RTSeqResetAlreadyHandledMutex[channelid]);

  if (m_RTSeqResetAlreadyHandled[channelid] && !m_RTSeqResetAlreadyHandled[channelid]->empty())
  {
    bool bAlrHandled = false;
    for (unsigned int i = 0; i < (m_RTSeqResetAlreadyHandled[channelid])->size(); ++i)
    {
      if (abs((*m_RTSeqResetAlreadyHandled[channelid])[i] - ulTStamp) <= 60000) // 60 seconds
        bAlrHandled = true;
    }
    return bAlrHandled;
  }
  return false;
}
bool SharedObjects::CheckRFSeqResetAlreadyHandled(const unsigned short channelid, const long ulTStamp)
{
  boost::shared_lock<boost::shared_mutex> lock(*m_RFSeqResetAlreadyHandledMutex[channelid]);

  if (m_RFSeqResetAlreadyHandled[channelid] && !m_RFSeqResetAlreadyHandled[channelid]->empty())
  {
    bool bAlrHandled = false;
    for (unsigned int i = 0; i < (m_RFSeqResetAlreadyHandled[channelid])->size(); ++i)
    {
      if (abs((*m_RFSeqResetAlreadyHandled[channelid])[i] - ulTStamp) <= 60000) // 60 seconds
        bAlrHandled = true;
    }
    return bAlrHandled;
  }
  return false;
}



void SharedObjects::AddRTChnlSeqNoOffset(const unsigned short channelid, const uint32_t uiSeqNoOffset)
{
  boost::unique_lock<boost::shared_mutex> lock(*m_RTSeqResetOffsetMutex[channelid]);

  if (!m_RTSeqResetOffset[channelid]) m_RTSeqResetOffset[channelid] = new vector<uint32_t>();

  (*m_RTSeqResetOffset[channelid]).push_back(uiSeqNoOffset);

  return;
}

void SharedObjects::AddRFChnlSeqNoOffset(const unsigned short channelid, const uint32_t uiSeqNoOffset)
{
  boost::unique_lock<boost::shared_mutex> lock(*m_RFSeqResetOffsetMutex[channelid]);

  if (!m_RFSeqResetOffset[channelid]) m_RFSeqResetOffset[channelid] = new vector<uint32_t>();

  (*m_RFSeqResetOffset[channelid]).push_back(uiSeqNoOffset);

  return;
}

void SharedObjects::AddRTSeqResetAlreadyHandled(const unsigned short channelid, const long ulTStamp)
{
  boost::unique_lock<boost::shared_mutex> lock(*m_RTSeqResetAlreadyHandledMutex[channelid]);

  if (!m_RTSeqResetAlreadyHandled[channelid])
    m_RTSeqResetAlreadyHandled[channelid] = new vector<long>();

  m_RTSeqResetAlreadyHandled[channelid]->push_back(ulTStamp);

  return;
}

void SharedObjects::AddRFSeqResetAlreadyHandled(const unsigned short channelid, const long ulTStamp)
{
  boost::unique_lock<boost::shared_mutex> lock(*m_RFSeqResetAlreadyHandledMutex[channelid]);

  if (!m_RFSeqResetAlreadyHandled[channelid])
    m_RFSeqResetAlreadyHandled[channelid] = new vector<long>();

  m_RFSeqResetAlreadyHandled[channelid]->push_back(ulTStamp);

  return;
}

void SharedObjects::ResetRTChnlSeqNoOffset()
{
  for (int i = 0; i < m_RTSeqResetOffset.size(); ++i)
  {
    if (m_RTSeqResetOffset[i])
    {
      boost::unique_lock<boost::shared_mutex> lock(*m_RTSeqResetOffsetMutex[i]);
      m_RTSeqResetOffset[i]->clear();
    }
  }
  return;
}
void SharedObjects::ResetRFChnlSeqNoOffset()
{
  for (int i = 0; i < m_RFSeqResetOffset.size(); ++i)
  {
    if (m_RFSeqResetOffset[i])
    {
      boost::unique_lock<boost::shared_mutex> lock(*m_RFSeqResetOffsetMutex[i]);
      m_RFSeqResetOffset[i]->clear();
    }
  }
  return;
}
void SharedObjects::ResetRTSeqResetAlreadyHandled()
{
  for (int i = 0; i < m_RTSeqResetAlreadyHandled.size(); ++i)
  {
    if (m_RTSeqResetAlreadyHandled[i])
    {
      boost::unique_lock<boost::shared_mutex> lock(*m_RTSeqResetAlreadyHandledMutex[i]);
      m_RTSeqResetAlreadyHandled[i]->clear();
    }
  }
  return;
}
void SharedObjects::ResetRFSeqResetAlreadyHandled()
{
  for (int i = 0; i < m_RFSeqResetAlreadyHandled.size(); ++i)
  {
    if (m_RFSeqResetAlreadyHandled[i])
    {
      boost::unique_lock<boost::shared_mutex> lock(*m_RFSeqResetAlreadyHandledMutex[i]);
      m_RFSeqResetAlreadyHandled[i]->clear();
    }
  }
  return;
}


//--------------------------------------------------
// System State
//--------------------------------------------------
bool SharedObjects::ThreadShouldExit()
{
  //boost::shared_lock<boost::shared_mutex> lock(m_SysStateMutex);
  if (m_SysState == UP) return false;
  else return true;
}

void SharedObjects::SetSystemState(ESysState newstate)
{
  //boost::unique_lock<boost::shared_mutex> lock(m_SysStateMutex);
  m_SysState = newstate;
  return;
}

//--------------------------------------------------
// Order Book
//--------------------------------------------------
boost::shared_ptr<OrderBookCache> SharedObjects::GetOrderBookCache()
{
  return m_OrderBookCache;
}

void SharedObjects::AddOrderBookIDInChnl(unsigned short channelid, unsigned long orderbookid)
{
  boost::unique_lock<boost::shared_mutex> lock(*m_OrderBookIDInEachChnlMutex[channelid]);
  if (m_OrderBookIDInEachChnl[channelid])
    m_OrderBookIDInEachChnl[channelid]->insert(orderbookid);
  return;
}

set<unsigned long>* SharedObjects::GetOrderBookIDInChnl(unsigned short channelid)
{
  return m_OrderBookIDInEachChnl[channelid];
}

void SharedObjects::ResetOrderBooksInChnl(unsigned short channelid)
{
  set<unsigned long> * pset = m_OrderBookIDInEachChnl[channelid];
  for (set<unsigned long>::iterator its = pset->begin(); its != pset->end(); ++its)
    m_OrderBookCache->GetOrderBook(*its)->Reset();
  return;
}


// //--------------------------------------------------
// // Spread Table
// //--------------------------------------------------
// unsigned short SharedObjects::GetSpreadTable(unsigned long ulCode)
// {
//   // no need to lock mutex
//   map<unsigned long, unsigned short>::iterator it = m_SpreadTable.find(ulCode);
//
//   if (it == m_SpreadTable.end()) return 0;
//   else return it->second;
// }
//
// void SharedObjects::AddSpreadTable(unsigned long ulCode, unsigned short ulSpreadTableCode)
// {
//   map<unsigned long, unsigned short>::iterator it = m_SpreadTable.find(ulCode);
//   if (it == m_SpreadTable.end())
//   {
//     boost::unique_lock<boost::shared_mutex> lock(m_SpreadTableMutex);
//     m_SpreadTable[ulCode] = ulSpreadTableCode;
//   }
//   return;
// }

void SharedObjects::SetProgramStartTime()
{
  m_ProgramStartTime = boost::posix_time::microsec_clock::local_time();
}

boost::posix_time::ptime SharedObjects::GetProgramStartTime() const
{
  return m_ProgramStartTime;
}

string SharedObjects::GetSymbolFromInstrumentID(const unsigned long instrumentID)
{
  boost::shared_lock<boost::shared_mutex> lock(m_InstrumentMapMutex);

  map<unsigned long, string>::iterator it = m_InstrumentMap.find(instrumentID);
  if (it == m_InstrumentMap.end()) return boost::lexical_cast<string>(instrumentID);
  else                             return it->second;
}

void SharedObjects::AddInstrumentIDToSymbolMapping(const unsigned long instrumentID, const string & symbol)
{
  boost::upgrade_lock<boost::shared_mutex> lock(m_InstrumentMapMutex);
  if (m_InstrumentMap.find(instrumentID) != m_InstrumentMap.end()) return;

  boost::upgrade_to_unique_lock<boost::shared_mutex> ulock(lock);
  m_InstrumentMap[instrumentID] = symbol;
}
