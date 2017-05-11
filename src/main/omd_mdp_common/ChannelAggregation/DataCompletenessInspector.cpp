#include "DataCompletenessInspector.h"

DataCompletenessInspector::DataCompletenessInspector(const unsigned short id, const bool bUnitTest) :
  m_ChannelID(id),
  m_DataComplInspectorSleepMillisec(100),
  m_TimeSinceEpoch(0),
  m_TriggerRetransmissionSeqNoGap(0),
  m_TriggerRetransmissionTimeGapMillisec(0),
  m_TriggerRefreshSeqNoGap(0),
  m_TriggerRefreshTimeGapMillisec(0),
  m_RTSRequestMaxSeqNoRange(0),
  m_RTSRequestMaxMsg(0),
  // m_RTSRequestBuffer(0),
  m_UnitTestingMode(bUnitTest),
  m_LastPrintLogTime(boost::posix_time::microsec_clock::local_time())
{
  Init();
}

DataCompletenessInspector::DataCompletenessInspector(const unsigned short id) :
  m_ChannelID(id),
  m_DataComplInspectorSleepMillisec(100),
  m_TimeSinceEpoch(0),
  m_TriggerRetransmissionSeqNoGap(0),
  m_TriggerRetransmissionTimeGapMillisec(0),
  m_TriggerRefreshSeqNoGap(0),
  m_TriggerRefreshTimeGapMillisec(0),
  m_RTSRequestMaxSeqNoRange(0),
  m_RTSRequestMaxMsg(0),
  // m_RTSRequestBuffer(0),
  m_UnitTestingMode(false)
{
  Init();
}

void DataCompletenessInspector::Init()
{
  //--------------------------------------------------
  // Initialization
  //--------------------------------------------------
  m_Logger      = Logger::Instance();
  m_SysCfg      = SystemConfig::Instance();
  m_ShrObj      = SharedObjects::Instance();
  m_RTSClt      = RTSClient::Instance();
  m_ThreadHealthMon = ThreadHealthMonitor::Instance();

  m_Logger->Write(Logger::INFO, "DataCompletenessInspector: ChannelID:%u.", m_ChannelID);

  m_MsgCirBuf_RT = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, m_ChannelID);

  m_TriggerRetransmissionSeqNoGap        = m_SysCfg->GetTriggerRetransmissionSeqNoGap();
  m_TriggerRetransmissionTimeGapMillisec = m_SysCfg->GetTriggerRetransmissionTimeGapMillisec();
  m_TriggerRefreshSeqNoGap               = m_SysCfg->GetTriggerRefreshSeqNoGap();
  m_TriggerRefreshTimeGapMillisec        = m_SysCfg->GetTriggerRefreshTimeGapMillisec();
  m_RTSRequestMaxSeqNoRange              = m_SysCfg->GetRTSRequestMaxSeqNoRange();
  m_RTSRequestMaxMsg                     = m_SysCfg->GetRTSRequestMaxMsg();
  // m_RTSRequestBuffer                     = m_SysCfg->GetRTSRequestBuffer();

  //--------------------------------------------------
  // Time related
  //--------------------------------------------------
  m_DataComplInspectorSleepMillisec = m_SysCfg->GetDataComplInspectorSleepMillisec();
  m_TimeSinceEpoch.reset(new boost::posix_time::ptime(gregorian::date(1970,1,1)));
}


void DataCompletenessInspector::NotifyRTSClient(uint32_t seqno1, uint32_t seqno2, uint32_t latestseqno, bool bCanPrintRTSLog)
{
  // Probably thinking too much
  // if (m_RTSRequestBuffer > (double)m_RTSRequestMaxSeqNoRange*0.1)
  //   m_RTSRequestBuffer = (unsigned long)((double)m_RTSRequestMaxSeqNoRange*0.1);

  //--------------------------------------------------
  // Some checking
  //--------------------------------------------------
  long lLim1 = latestseqno - m_RTSRequestMaxMsg; // + m_RTSRequestBuffer;
  lLim1 = (lLim1 < 1 ? 1 : lLim1);
  if (seqno1 < lLim1) seqno1 = lLim1;

  long lLim2 = seqno1 + m_RTSRequestMaxSeqNoRange;
  if (seqno2 >= lLim2) seqno2 = lLim2;

  if (seqno1 > seqno2) return;

  //--------------------------------------------------
  // Notify RTSClient, change from seq reset adjusted seq num to raw seq num
  //--------------------------------------------------
  uint32_t uiLatestRTOffset = m_ShrObj->CheckRTChnlLatestSeqNoOffset(m_ChannelID);
  long lUnadjSeqNo1 = seqno1 - uiLatestRTOffset;
  long lUnadjSeqNo2 = seqno2 - uiLatestRTOffset;
  if (lUnadjSeqNo1 > 0 && lUnadjSeqNo2 > 0)
  {
    m_RTSClt->AddRTSRequest(m_ChannelID,lUnadjSeqNo1,lUnadjSeqNo2);
    if (bCanPrintRTSLog) m_Logger->Write(Logger::NOTICE,"DataCompletenessInspector: ChannelID:%u. Sent request to RTSClient. from Seq No (raw) %u to (raw) %u.", m_ChannelID, lUnadjSeqNo1, lUnadjSeqNo2);
  }
  else
  {
    if (bCanPrintRTSLog) m_Logger->Write(Logger::NOTICE,"DataCompletenessInspector: ChannelID:%u. Did not send request to RTSClient. from Seq No (raw) %u to (raw) %u. Any problem?", m_ChannelID, lUnadjSeqNo1, lUnadjSeqNo2);
  }

  return;
}

void DataCompletenessInspector::Run()
{
  if (!m_Logger) m_Logger = Logger::Instance();

  for (;;)
  {
    //--------------------------------------------------
    // Report Health
    //--------------------------------------------------
    m_ThreadHealthMon->ReportThatIAmHealthy(ThreadHealthMonitor::DATACOMPLETENESSINSPECTOR, m_ChannelID);

    //--------------------------------------------------
    // System shutdown
    //--------------------------------------------------
    if (m_ShrObj->ThreadShouldExit())
    {
      m_Logger->Write(Logger::NOTICE,"DataCompletenessInspector: ChannelID:%u. Stopping thread.", m_ChannelID);
      return;
    }
    // No need to use signalling for this thread; continue to use sleep
    BYTE * pbMsg = NULL;
    uint32_t uiSmlMissingAdjSeqNo = 0;
    uint32_t uiLrgtMissingAdjSeqNo = 0;
    uint32_t uiLatestAdjSeqNo = 0;
    uint32_t uiStartAdjSeqNo = m_MsgCirBuf_RT->GetStartSeqNo();

    if
      (!m_MsgCirBuf_RT->GetLatestSeqNo(uiLatestAdjSeqNo)               ||
       !m_MsgCirBuf_RT->GetSmallestMissingSeqNo(uiSmlMissingAdjSeqNo)  ||
       !m_MsgCirBuf_RT->GetLargestMissingSeqNo(uiLrgtMissingAdjSeqNo))
      {
        if (m_DataComplInspectorSleepMillisec != 0)
          boost::this_thread::sleep(boost::posix_time::milliseconds(m_DataComplInspectorSleepMillisec));
      }
    else
    {
      bool bDirFlag = false;
      uint64_t uiTStamp = 0;
      if (m_MsgCirBuf_RT->CheckDirtyFlagTStamp(uiSmlMissingAdjSeqNo,bDirFlag,uiTStamp))
      {
        if (!bDirFlag)
        {
          uint64_t uiCurTime = SDateTime::GetCurrentUnixTimeInMillisecGMT();

          bool bCanPrintRTSLog = false;
          boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
          boost::posix_time::time_duration diff = now - m_LastPrintLogTime;
          unsigned long ulTD = diff.total_milliseconds();
          if (ulTD > MILLISEC_RTSLOG)
          {
            bCanPrintRTSLog = true;
            m_LastPrintLogTime = now;
          }

          //--------------------------------------------------
          // Detect seq no gap, seq no gap detection has higher priority than time gap
          //--------------------------------------------------
          if (uiLatestAdjSeqNo > uiSmlMissingAdjSeqNo + m_TriggerRefreshSeqNoGap)
          {
            //--------------------------------------------------
            // Refresh is needed, seq no gap trigger
            //--------------------------------------------------
            if (!m_ShrObj->CheckRefreshActivatnStatus(m_ChannelID))
            {
              m_ShrObj->ActivateRefresh(m_ChannelID);
              // must print log
              m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Start Seq No (adj): %u, Latest Seq No (adj): %u, Smallest Missing Seq No (adj): %u (TStamp=%s), Largest Missing Seq No (adj): %u",
                  m_ChannelID,
                  uiStartAdjSeqNo,
                  uiLatestAdjSeqNo, uiSmlMissingAdjSeqNo,
                  SDateTime::fromUnixTimeToString(uiTStamp, SDateTime::NANOSEC, SDateTime::HKT, SDateTime::HKT).c_str(),
                  uiLrgtMissingAdjSeqNo);
              m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Seq no gap is larger than %u; Refresh mode is now activated.", m_ChannelID, m_TriggerRefreshSeqNoGap);
            }
            else
            {
              if (bCanPrintRTSLog)
              {
                m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Seq no gap is larger than %u; Refresh mode is already activated.", m_ChannelID, m_TriggerRefreshSeqNoGap);
                m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Start Seq No (adj): %u, Latest Seq No (adj): %u, Smallest Missing Seq No (adj): %u (TStamp=%s), Largest Missing Seq No (adj): %u",
                    m_ChannelID,
                    uiStartAdjSeqNo,
                    uiLatestAdjSeqNo, uiSmlMissingAdjSeqNo,
                    SDateTime::fromUnixTimeToString(uiTStamp, SDateTime::NANOSEC, SDateTime::HKT, SDateTime::HKT).c_str(),
                    uiLrgtMissingAdjSeqNo);
              }
            }
          }
          else if (uiLatestAdjSeqNo > uiSmlMissingAdjSeqNo + m_TriggerRetransmissionSeqNoGap)
          {
            //--------------------------------------------------
            // Retransmission, seq no gap trigger
            //--------------------------------------------------
            if (bCanPrintRTSLog)
              m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Start Seq No (adj): %u, Latest Seq No (adj): %u, Smallest Missing Seq No (adj): %u (TStamp=%s), Largest Missing Seq No (adj): %u",
                  m_ChannelID,
                  uiStartAdjSeqNo,
                  uiLatestAdjSeqNo, uiSmlMissingAdjSeqNo,
                  SDateTime::fromUnixTimeToString(uiTStamp, SDateTime::MILLISEC, SDateTime::HKT, SDateTime::HKT).c_str(),
                  uiLrgtMissingAdjSeqNo);

            if (!m_ShrObj->CheckRefreshActivatnStatus(m_ChannelID))
            {
              NotifyRTSClient(uiSmlMissingAdjSeqNo,uiLrgtMissingAdjSeqNo,uiLatestAdjSeqNo,bCanPrintRTSLog);
              if (bCanPrintRTSLog)
                m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Seq no gap is larger than %u; Attempting recovery from Retransmission server.",
                    m_ChannelID, m_TriggerRetransmissionSeqNoGap);
            }
            else
            {
              if (bCanPrintRTSLog)
                m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Seq no gap is larger than %u; But not attempting recovery from Retransmission server because refresh is activated.",
                    m_ChannelID, m_TriggerRetransmissionSeqNoGap);
            }
          }

          //--------------------------------------------------
          // Detect time gap
          //--------------------------------------------------
          else if (uiCurTime > uiTStamp + m_TriggerRefreshTimeGapMillisec)
          {
            //--------------------------------------------------
            // Refresh is needed, time gap trigger
            //--------------------------------------------------
            if (!m_ShrObj->CheckRefreshActivatnStatus(m_ChannelID))
            {
              m_ShrObj->ActivateRefresh(m_ChannelID);
              // must print log
              m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Time gap is larger than %u ms; Refresh mode is now activated.",
                  m_ChannelID, m_TriggerRefreshTimeGapMillisec);
              m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u, Start Seq No (adj): %u, Latest Seq No (adj): %u, Smallest Missing Seq No (adj): %u (TStamp=%s), Largest Missing Seq No (adj): %u",
                  m_ChannelID,
                  uiStartAdjSeqNo,
                  uiLatestAdjSeqNo, uiSmlMissingAdjSeqNo,
                  SDateTime::fromUnixTimeToString(uiTStamp, SDateTime::MILLISEC, SDateTime::HKT, SDateTime::HKT).c_str(),
                  uiLrgtMissingAdjSeqNo);
            }
            else
            {
              if (bCanPrintRTSLog)
              {
                m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Time gap is larger than %u ms; Refresh mode is already activated.",
                    m_ChannelID, m_TriggerRefreshTimeGapMillisec);
                m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u, Start Seq No (adj): %u, Latest Seq No (adj): %u, Smallest Missing Seq No (adj): %u (TStamp=%s), Largest Missing Seq No (adj): %u",
                    m_ChannelID,
                    uiStartAdjSeqNo,
                    uiLatestAdjSeqNo, uiSmlMissingAdjSeqNo,
                    SDateTime::fromUnixTimeToString(uiTStamp, SDateTime::MILLISEC, SDateTime::HKT, SDateTime::HKT).c_str(),
                    uiLrgtMissingAdjSeqNo);
              }
            }

          }
          else if (uiCurTime > uiTStamp + m_TriggerRetransmissionTimeGapMillisec)
          {
            //--------------------------------------------------
            // Retransmission, time trigger
            //--------------------------------------------------
            if (bCanPrintRTSLog)
              m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Start Seq No (adj): %u, Latest Seq No (adj): %u, Smallest Missing Seq No (adj): %u (TStamp=%s), Largest Missing Seq No (adj): %u",
                  m_ChannelID,
                  uiStartAdjSeqNo,
                  uiLatestAdjSeqNo, uiSmlMissingAdjSeqNo,
                  SDateTime::fromUnixTimeToString(uiTStamp, SDateTime::MILLISEC, SDateTime::HKT, SDateTime::HKT).c_str(),
                  uiLrgtMissingAdjSeqNo);

            if (!m_ShrObj->CheckRefreshActivatnStatus(m_ChannelID))
            {
              NotifyRTSClient(uiSmlMissingAdjSeqNo,uiLrgtMissingAdjSeqNo,uiLatestAdjSeqNo,bCanPrintRTSLog);
              if (bCanPrintRTSLog)
                m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Time gap is larger than %u ms; Attempting recovery from Retransmission server.",
                    m_ChannelID, m_TriggerRetransmissionTimeGapMillisec);
            }
            else
            {
              if (bCanPrintRTSLog)
                m_Logger->Write(Logger::NOTICE, "DataCompletenessInspector: ChannelID:%u. Time gap is larger than %u ms; But not attempting recovery from Retransmission server because refresh is activated.",
                    m_ChannelID, m_TriggerRetransmissionTimeGapMillisec);
            }
          }
        }
      }
    }

    //--------------------------------------------------
    // Don't loop indefinitely when unit testing
    //--------------------------------------------------
    if (m_UnitTestingMode) break;

  }
}
