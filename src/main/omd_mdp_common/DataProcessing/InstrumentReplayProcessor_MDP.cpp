#include "InstrumentReplayProcessor.h"

void InstrumentReplayProcessor_MDP::Run()
{
  for (;;)
  {
    //--------------------------------------------------
    // System shutdown
    //--------------------------------------------------
    if (m_ShrObj->ThreadShouldExit())
    {
      m_Logger->Write(Logger::NOTICE,"InstrumentReplayProcessor: ChannelID:%u. Stopping thread.", m_ChannelID);
      return;
    }

    //--------------------------------------------------
    // Report Health
    //--------------------------------------------------
    m_ThreadHealthMon->ReportThatIAmHealthy(ThreadHealthMonitor::INSTRUMENTREPLAYPROCESSOR, m_ChannelID);

    //--------------------------------------------------
    // Warn when cir buffer is too long
    //--------------------------------------------------
    if (m_MsgCirBuf_IR->Size() > 10000)
    {
      m_Logger->Write(Logger::WARNING,"InstrumentReplayProcessor: ChannelID:%u. Detected abnormally long message circular buffer with size %u. Please investigate. But I will try to purge.", m_ChannelID, m_MsgCirBuf_IR->Size());

      uint32_t uiLargestMissingSeqNo = 0;
      if (m_MsgCirBuf_IR->GetLargestMissingSeqNo(uiLargestMissingSeqNo))
      {
        m_Logger->Write(Logger::INFO,"RefreshProcessor: ChannelID:%u. There are missing messages in m_MsgCirBuf_IR. Largest missing seq no %u. Purge m_MsgCirBuf_IR up to that.", m_ChannelID, uiLargestMissingSeqNo);
        m_MsgCirBuf_IR->PurgeMsgB4SeqNoInclusive(uiLargestMissingSeqNo);
        continue;
      }
    }

    //--------------------------------------------------
    // Try get data from queue
    //--------------------------------------------------
    BYTE *pbPktMut = NULL;
    uint16_t usMsgSize = 0;
    uint32_t uiPktSeqNo = 0;
    uint64_t ulTS = 0;
    StateOfNextSeqNo snsn = m_MsgCirBuf_IR->GetPtrMsgSeqNoTStamp(pbPktMut, &uiPktSeqNo, &ulTS, &usMsgSize);
    if (snsn == ALL_RETRIEVED)
    {
      m_MsgCirBuf_IR->WaitForData();
      continue;
    }
    else if (snsn == SEQNO_MISSING)
    {
      if (m_ShrObj->CheckCapTestStatus()) m_MsgCirBuf_IR->PopFront();
      else                                m_MsgCirBuf_IR->WaitForData();
      continue;
    }
    const BYTE *pbPkt = pbPktMut;

    //--------------------------------------------------
    // Output Debug info
    //--------------------------------------------------
    m_Logger->Write(Logger::DEBUG,"InstrumentReplayProcessor: ChannelID:%u. m_MsgCirBuf_IR.Size()          %u", m_ChannelID, m_MsgCirBuf_IR->Size());
    m_Logger->Write(Logger::DEBUG,"InstrumentReplayProcessor: ChannelID:%u. m_MsgCirBuf_IR.AllocatedSize() %u", m_ChannelID, m_MsgCirBuf_IR->AllocatedSize());
    m_Logger->Write(Logger::DEBUG,"InstrumentReplayProcessor: ChannelID:%u. Pkt Seq No:                    %u", m_ChannelID, uiPktSeqNo);
    m_Logger->Write(Logger::DEBUG,"InstrumentReplayProcessor: ChannelID:%u. Message Header: Local Timestamp of Pkt: %s", m_ChannelID, SDateTime::fromUnixTimeToString(ulTS,SDateTime::MILLISEC).c_str());

    //--------------------------------------------------
    m_DataProcFunc->HandleMDPRaw(pbPkt, m_ChannelID, McastIdentifier::INSTRUMENTREPLAY, usMsgSize);

    m_MsgCirBuf_IR->PopFront();
  }
}
