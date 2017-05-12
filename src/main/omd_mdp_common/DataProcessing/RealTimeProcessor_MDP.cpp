#include "RealTimeProcessor.h"

void RealTimeProcessor_MDP::Run()
{
  for (;;)
  {
    //--------------------------------------------------
    // System shutdown
    //--------------------------------------------------
    if (m_ShrObj->ThreadShouldExit())
    {
      m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Stopping thread.", __FILENAME__, m_ChannelID);
      return;
    }

    //--------------------------------------------------
    // Report Health
    //--------------------------------------------------
    m_ThreadHealthMon->ReportThatIAmHealthy(ThreadHealthMonitor::REALTIMEPROCESSOR, m_ChannelID);

    //--------------------------------------------------
    // Warn when cir buffer is too long
    //--------------------------------------------------
    if (m_MsgCirBuf->Size() > 10000)
    {
      m_Logger->Write(Logger::WARNING,"%s: ChannelID:%u. Detected abnormally long message circular buffer with size %u. Please investigate. But I will try to purge.", __FILENAME__, m_ChannelID, m_MsgCirBuf->Size());

      uint32_t uiLargestMissingSeqNo = 0;
      if (m_MsgCirBuf->GetLargestMissingSeqNo(uiLargestMissingSeqNo))
      {
        m_Logger->Write(Logger::INFO,"%s: ChannelID:%u. There are missing messages in m_MsgCirBuf. Largest missing seq no %u. Purge m_MsgCirBuf up to that.", __FILENAME__, m_ChannelID, uiLargestMissingSeqNo);
        m_MsgCirBuf->PurgeMsgB4SeqNoInclusive(uiLargestMissingSeqNo);
        continue;
      }
    }

    //--------------------------------------------------
    // Must stop RealTimeProcessor if Refresh mode is activated
    // Remember that we can trigger refresh manually from our command terminal
    //--------------------------------------------------
    if (!m_ShrObj->CheckCapTestStatus() && m_ShrObj->CheckRefreshActivatnStatus(m_ChannelID))
    {
      boost::this_thread::sleep(boost::posix_time::seconds(1)); // force sleep 1 sleep
      m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Refresh mode is on. RealTimeProcessor will halt processing until refresh is done.", __FILENAME__, m_ChannelID);
      continue;
    }

    //--------------------------------------------------
    // Try get data from queue
    //--------------------------------------------------
    BYTE *pbPktMut = NULL;
    uint16_t usMsgSize = 0;
    uint32_t uiPktSeqNo = 0;
    uint64_t ulTS = 0;
    StateOfNextSeqNo snsn = m_MsgCirBuf->GetPtrMsgSeqNoTStamp(pbPktMut, &uiPktSeqNo, &ulTS, &usMsgSize);
    if (snsn == ALL_RETRIEVED)
    {
      m_MsgCirBuf->WaitForData();
      continue;
    }
    else if (snsn == SEQNO_MISSING)
    {
      if (m_ShrObj->CheckCapTestStatus()) m_MsgCirBuf->PopFront();
      else                                m_MsgCirBuf->WaitForData();
      continue;
    }
    const BYTE *pbPkt = pbPktMut;

    //--------------------------------------------------
    // Output Debug info
    //--------------------------------------------------
    m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. m_MsgCirBuf.Size()          %u",             __FILENAME__, m_ChannelID, m_MsgCirBuf->Size());
    m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. m_MsgCirBuf.AllocatedSize() %u",             __FILENAME__, m_ChannelID, m_MsgCirBuf->AllocatedSize());
    m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. Pkt Seq No:                 %u",             __FILENAME__, m_ChannelID, uiPktSeqNo);
    m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. Message Header: Local Timestamp of Pkt: %s", __FILENAME__, m_ChannelID, SDateTime::fromUnixTimeToString(ulTS,SDateTime::MILLISEC).c_str());

    //--------------------------------------------------
    // Record canned data
    //--------------------------------------------------
    m_BinaryRecorder.WriteHKExUnixTime(pbPkt);

    //--------------------------------------------------
    m_DataProcFunc->HandleMDPRaw(pbPkt, m_ChannelID, McastIdentifier::ToString(McastIdentifier::REALTIME), usMsgSize);

    m_MsgCirBuf->PopFront();
  }
}
