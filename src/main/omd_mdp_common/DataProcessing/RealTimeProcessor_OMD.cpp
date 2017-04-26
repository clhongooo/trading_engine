#include "RealTimeProcessor.h"

void RealTimeProcessor_OMD::Run()
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
    BYTE *pbMsg = NULL;
    uint32_t uiAdjSeqNo = 0;
    uint64_t ulTS = 0;
    StateOfNextSeqNo snsn = m_MsgCirBuf->GetMsgSeqNoTStamp(pbMsg, &uiAdjSeqNo, &ulTS);
    if (snsn == ALL_RETRIEVED)
    {
      m_MsgCirBuf->WaitForData();
      continue;
    }
    else if (snsn == SEQNO_MISSING)
    {
      if (m_ShrObj->CheckCapTestStatus())
      {
        m_MsgCirBuf->PopFront();
      }
      else
      {
        m_MsgCirBuf->WaitForData();
      }
      continue;
    }

    //--------------------------------------------------
    // Must stop RealTimeProcessor if Refresh mode is activated, otherwise we'll 
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

    //----------------------------------------------------------------------------------------------------
    // Running RealTimeProcessor
    //----------------------------------------------------------------------------------------------------
    uint16_t uiMsgSize = READ_UINT16(pbMsg);
    uint16_t usMsgType = READ_UINT16(pbMsg+2);

    //--------------------------------------------------
    // Output Message Header info 
    //--------------------------------------------------
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: ChannelID:%u. Message Header: Msg size:    %u", m_ChannelID, uiMsgSize);
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: ChannelID:%u. Message Header: Msg type:    %u", m_ChannelID, usMsgType);
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: ChannelID:%u. Message Header: Seq No (adj):%u", m_ChannelID, uiAdjSeqNo);
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: ChannelID:%u. Message Header: Send Time:   %u", m_ChannelID, ulTS);

    //--------------------------------------------------
    // Output other Debug info
    //--------------------------------------------------
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: m_MsgCirBuf.Size()          %u", m_MsgCirBuf->Size());
    m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: m_MsgCirBuf.AllocatedSize() %u", m_MsgCirBuf->AllocatedSize());

    if (m_PrintRealTimeSeqNoAsInfo)
      m_Logger->Write(Logger::INFO ,"RealTimeProcessor: ChannelID:%u. Seq No (adj): %u", m_ChannelID, uiAdjSeqNo);
    else
      m_Logger->Write(Logger::DEBUG,"RealTimeProcessor: ChannelID:%u. Seq No (adj): %u", m_ChannelID, uiAdjSeqNo);

    //--------------------------------------------------
    // Record canned data
    //--------------------------------------------------
    m_BinaryRecorder.WriteHKExUnixTime(pbMsg);

    //--------------------------------------------------
    // Printing results in log
    //--------------------------------------------------
    if (m_bOutputJson)
    {
      memset(m_JsonBuffer,'\0',JSON_BUFFER_SIZE);
      m_DataProcFunc->OutputJsonToLog(__FILE__, m_ChannelID, m_Logger, pbMsg, m_JsonBuffer);
    }

    //--------------------------------------------------
    // Process message for omd_mdi
    //--------------------------------------------------
    m_DataProcFunc->ProcessMessageForTransmission(pbMsg, usMsgType);
    //--------------------------------------------------

    //--------------------------------------------------
    // Special handling for order book 
    //--------------------------------------------------
    if (usMsgType == OMDC_AGGREGATE_ORDER_BOOK_UPDATE || usMsgType == OMDD_AGGREGATE_ORDER_BOOK_UPDATE)
    {
      m_DataProcFunc->ProcessOrderBookInstruction(__FILE__,m_Logger,pbMsg,m_ShrObj,m_PrintOrderBookAsInfo);
    }

    m_MsgCirBuf->PopFront();
  }
}
