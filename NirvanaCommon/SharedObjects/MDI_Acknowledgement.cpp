#include <MDI_Acknowledgement.h>

boost::weak_ptr<MDI_Acknowledgement> MDI_Acknowledgement::m_pInstance;

boost::shared_ptr<MDI_Acknowledgement> MDI_Acknowledgement::Instance() {
  boost::shared_ptr<MDI_Acknowledgement> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new MDI_Acknowledgement());
    m_pInstance = instance;
  }
  return instance;
}

MDI_Acknowledgement::MDI_Acknowledgement() : m_NoOfAckReceived_techind(0)
{
  m_Logger = Logger::Instance();
  m_SysCfg = SystemConfig::Instance();

  ResetRecvAckNoLock();
}

MDI_Acknowledgement::~MDI_Acknowledgement()
{
}

void MDI_Acknowledgement::ReportAckTechIndUpd()
{
  boost::unique_lock<boost::shared_mutex> lock(m_ReceivedAck_mutex);
  m_NoOfAckReceived_techind = min(m_NoOfAckReceived_techind+1,1);
  if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) m_NoOfAckReceived_techind = %d",__FILE__,__FUNCTION__,__LINE__,m_NoOfAckReceived_techind);

  return;
}

void MDI_Acknowledgement::ReportAck(const StrategyID sid)
{
  boost::unique_lock<boost::shared_mutex> lock(m_ReceivedAck_mutex);
  m_NoOfAckReceived_sty[static_cast<int>(sid)] = min(m_NoOfAckReceived_sty[static_cast<int>(sid)]+1,1);
  if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) m_NoOfAckReceived_sty[%s] = %d",__FILE__,__FUNCTION__,__LINE__,GetStrategyName(sid).c_str(),m_NoOfAckReceived_sty[static_cast<int>(sid)]);

  // if (CheckIfAllAckRecvdNoLock())
  // {
  //   ClearAckReportedFromStyNoLock();
  //   NotifyConsumers();
  // }
  return;
}


void MDI_Acknowledgement::ClearAckReportedFromStyNoLock()
{
  for (int sty = STY_NIR; sty != STY_LAST; sty++)
  {
    if (m_SysCfg->IsStrategyOn(static_cast<StrategyID>(sty)))
    {
      m_NoOfAckReceived_sty[sty] = max(m_NoOfAckReceived_sty[sty]-1,-1);
      if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) m_NoOfAckReceived_sty[%s] = %d",__FILE__,__FUNCTION__,__LINE__,GetStrategyName(static_cast<StrategyID>(sty)).c_str(),m_NoOfAckReceived_sty[sty]);
    }
  }
  m_NoOfAckReceived_techind = max(m_NoOfAckReceived_techind-1,-1);
  if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) m_NoOfAckReceived_techind = %d",__FILE__,__FUNCTION__,__LINE__,m_NoOfAckReceived_techind);
  if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) Cleared internal ack record.",__FILE__,__FUNCTION__,__LINE__);
  return;
}


bool MDI_Acknowledgement::CheckIfAllAckRecvdLock()
{
  boost::unique_lock<boost::shared_mutex> lock(m_ReceivedAck_mutex);
  return CheckIfAllAckRecvdNoLock();
}
bool MDI_Acknowledgement::CheckIfAllAckRecvdNoLock()
{
  //--------------------------------------------------
  // Check if all ack are received
  // on multiple mdi, if the data come simultaneously,
  // or strategy is slow, then the strategy may be
  // notified less than the data, therefore check == 0
  //--------------------------------------------------
  bool bAllAckReceived = true;
  for (int sty = STY_NIR; sty != STY_LAST; sty++)
  {
    if (m_SysCfg->IsStrategyOn(static_cast<StrategyID>(sty)) && m_NoOfAckReceived_sty[sty] <= 0) //< m_MDIThatNeedAck.size()
    {
      if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) m_NoOfAckReceived_sty[%s] = %d",__FILE__,__FUNCTION__,__LINE__,GetStrategyName(static_cast<StrategyID>(sty)).c_str(),m_NoOfAckReceived_sty[sty]);
      bAllAckReceived = false;
    }
  }

  if (m_NoOfAckReceived_techind <= 0)
  {
    if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) m_NoOfAckReceived_techind = %d",__FILE__,__FUNCTION__,__LINE__,m_NoOfAckReceived_techind);
    bAllAckReceived = false;
  }

  return bAllAckReceived;
}


void MDI_Acknowledgement::ReportNotMktData()
{
  boost::unique_lock<boost::shared_mutex> lock(m_ReceivedAck_mutex);
  for (int sty = STY_NIR; sty != STY_LAST; sty++)
  {
    if (m_SysCfg->IsStrategyOn(static_cast<StrategyID>(sty)))
    {
      m_NoOfAckReceived_sty[sty] = min(m_NoOfAckReceived_sty[sty]+1,1);
      if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) m_NoOfAckReceived_sty[%s] = %d",__FILE__,__FUNCTION__,__LINE__,GetStrategyName(static_cast<StrategyID>(sty)).c_str(),m_NoOfAckReceived_sty[sty]);
    }

    m_NoOfAckReceived_techind = min(m_NoOfAckReceived_techind+1,1);
    if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) m_NoOfAckReceived_techind = %d",__FILE__,__FUNCTION__,__LINE__,m_NoOfAckReceived_techind);
  }
  return;
}


void MDI_Acknowledgement::ClearAckReportedFromStyLock()
{
  boost::unique_lock<boost::shared_mutex> lock(m_ReceivedAck_mutex);
  ClearAckReportedFromStyNoLock();
  return;
}


void MDI_Acknowledgement::ResetRecvAckNoLock()
{
  m_MDIThatNeedAck.clear();
  m_NoOfAckReceived_sty.clear();
  m_NoOfAckReceived_sty.reserve(static_cast<int>(STY_LAST));
  m_NoOfAckReceived_sty.insert(m_NoOfAckReceived_sty.begin(),static_cast<int>(STY_LAST),0);
  m_NoOfAckReceived_techind = 0;

  return;
}
void MDI_Acknowledgement::WaitForAck()
{
  if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) Start waiting for ack.",__FILE__,__FUNCTION__,__LINE__);
  long lCnt = 0;
  while (!CheckIfAllAckRecvdLock())
  {
    // boost::mutex::scoped_lock lock2(m_ObserverMutex_Ack);
    // m_cvAck.wait(lock2); //blocks and returns mutex
    //--------------------------------------------------
    // Yes, the shortest period of time
    //--------------------------------------------------
    boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    lCnt++;
    if (lCnt % 5000 == 0)
    {
      for (int sty = STY_NIR; sty != STY_LAST; sty++)
      {
        StrategyID sid = static_cast<StrategyID>(sty);
        if (m_SysCfg->IsStrategyOn(sid))
        {
          //--------------------------------------------------
          // Report for the strategy
          //--------------------------------------------------
          m_Logger->Write(Logger::NOTICE,"%s::%s (%d) Been waiting for ack for a long time: m_NoOfAckReceived_sty[%s] = %d",__FILE__,__FUNCTION__,__LINE__,GetStrategyName(sid).c_str(),m_NoOfAckReceived_sty[sty]);
          ReportAck(sid);
          m_Logger->Write(Logger::NOTICE,"%s::%s (%d) Been waiting for ack for a long time: m_NoOfAckReceived_sty[%s] = %d",__FILE__,__FUNCTION__,__LINE__,GetStrategyName(sid).c_str(),m_NoOfAckReceived_sty[sty]);
        }
      }
      m_Logger->Write(Logger::NOTICE,"%s::%s (%d) Been waiting for ack for a long time: m_NoOfAckReceived_techind = %d",__FILE__,__FUNCTION__,__LINE__,m_NoOfAckReceived_techind);
      ReportAckTechIndUpd();
      m_Logger->Write(Logger::NOTICE,"%s::%s (%d) Been waiting for ack for a long time: m_NoOfAckReceived_techind = %d",__FILE__,__FUNCTION__,__LINE__,m_NoOfAckReceived_techind);
    }
  }
  if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) All Ack received.",__FILE__,__FUNCTION__,__LINE__);
  ClearAckReportedFromStyLock();
  if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) Cleared internal ack record.",__FILE__,__FUNCTION__,__LINE__);
  return;
}

// void MDI_Acknowledgement::NotifyConsumers()
// {
//   m_cvAck.notify_all();
// }

void MDI_Acknowledgement::ReportDataArrived(const int mdi_id)
{
  if (MDI_ACK_DEBUG_MSG) m_Logger->Write(Logger::DEBUG,"%s::%s (%d) ",__FILE__,__FUNCTION__,__LINE__);
  m_MDIThatNeedAck.insert(mdi_id);
  return;
}

