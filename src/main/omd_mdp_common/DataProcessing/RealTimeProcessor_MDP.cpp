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
      m_Logger->Write(Logger::NOTICE,"RealTimeProcessor: ChannelID:%u. Stopping thread.", m_ChannelID);
      return;
    }

    //--------------------------------------------------
    // Report Health
    //--------------------------------------------------
    m_ThreadHealthMon->ReportThatIAmHealthy(ThreadHealthMonitor::REALTIMEPROCESSOR, m_ChannelID);

    //--------------------------------------------------
    // Must stop RealTimeProcessor if Refresh mode is activated
    // Remember that we can trigger refresh manually from our command terminal
    //--------------------------------------------------
    if (!m_ShrObj->CheckCapTestStatus() && m_ShrObj->CheckRefreshActivatnStatus(m_ChannelID))
    {
      boost::this_thread::sleep(boost::posix_time::seconds(1)); // force sleep 1 sleep
      m_Logger->Write(Logger::NOTICE,"RealTimeProcessor: ChannelID:%u. Refresh mode is on. RealTimeProcessor will halt processing until refresh is done.", m_ChannelID);

      //--------------------------------------------------
      // Warn when cir buffer is too long
      //--------------------------------------------------
      if (m_MsgCirBuf->Size() > 10000)
        m_Logger->Write(Logger::WARNING,"RealTimeProcessor: ChannelID:%u. Detected abnormally long message circular buffer with size %u. Please investigate.", m_ChannelID, m_MsgCirBuf->Size());

      continue;
    }

    //--------------------------------------------------
    // Try get data from queue
    //--------------------------------------------------
    BYTE *pbPktMut = NULL;
    uint16_t usMsgSize = 0;
    uint32_t uiPktSeqNo = 0;
    uint64_t ulTS = 0;
    StateOfNextSeqNo snsn = m_MsgCirBuf->GetMsgSeqNoTStamp(pbPktMut, &uiPktSeqNo, &ulTS, &usMsgSize);
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
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: ChannelID:%u. m_MsgCirBuf.Size()          %u", m_ChannelID, m_MsgCirBuf->Size());
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: ChannelID:%u. m_MsgCirBuf.AllocatedSize() %u", m_ChannelID, m_MsgCirBuf->AllocatedSize());
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: ChannelID:%u. Pkt Seq No:                 %u", m_ChannelID, uiPktSeqNo);
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: ChannelID:%u. Message Header: Local Timestamp of Pkt: %s", m_ChannelID, SDateTime::fromUnixTimeToString(ulTS,SDateTime::MILLISEC).c_str());


    //--------------------------------------------------
    // Record canned data
    //--------------------------------------------------
    m_BinaryRecorder.WriteHKExUnixTime(pbPkt);

    //--------------------------------------------------
    m_DataProcFunc->HandleMDPRaw(pbPkt, m_ChannelID, McastIdentifier::REALTIME, usMsgSize);

    m_MsgCirBuf->PopFront();
  }
}
