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
      m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Stopping thread.", __FILENAME__, m_ChannelID);
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
      m_Logger->Write(Logger::WARNING,"%s: ChannelID:%u. Detected abnormally long message circular buffer with size %u. Please investigate. But I will try to purge.", __FILENAME__, m_ChannelID, m_MsgCirBuf_IR->Size());

      uint32_t uiLargestMissingSeqNo = 0;
      if (m_MsgCirBuf_IR->GetLargestMissingSeqNo(uiLargestMissingSeqNo))
      {
        m_Logger->Write(Logger::INFO,"%s: ChannelID:%u. There are missing messages in m_MsgCirBuf_IR. Largest missing seq no %u. Purge m_MsgCirBuf_IR up to that.", __FILENAME__, m_ChannelID, uiLargestMissingSeqNo);
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
      boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
      continue;
    }
    else if (snsn == SEQNO_MISSING)
    {
      //--------------------------------------------------
      // Just keep going without recovery
      //--------------------------------------------------
      m_MsgCirBuf_IR->PopFront();
      continue;
    }
    const BYTE *pbPkt = pbPktMut;

    // //--------------------------------------------------
    // // Output Debug info
    // //--------------------------------------------------
    // m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. m_MsgCirBuf_IR.Size()          %u",          __FILENAME__, m_ChannelID, m_MsgCirBuf_IR->Size());
    // m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. m_MsgCirBuf_IR.AllocatedSize() %u",          __FILENAME__, m_ChannelID, m_MsgCirBuf_IR->AllocatedSize());
    // m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. Pkt Seq No:                    %u",          __FILENAME__, m_ChannelID, uiPktSeqNo);
    // m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. Message Header: Local Timestamp of Pkt: %s", __FILENAME__, m_ChannelID, SDateTime::fromUnixTimeToString(ulTS,SDateTime::MILLISEC).c_str());

    //--------------------------------------------------
    m_DataProcFunc->HandleMDPRaw(pbPkt, m_ChannelID, McastIdentifier::ToString(McastIdentifier::INSTRUMENTREPLAY), usMsgSize, DataProcFunctions::DPF_DO_ACTUAL_PROCESSING);

    m_MsgCirBuf_IR->PopFront();
  }
}
