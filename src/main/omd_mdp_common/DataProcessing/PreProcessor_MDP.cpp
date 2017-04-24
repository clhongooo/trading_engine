#include "PreProcessor.h"

void PreProcessor_MDP::Run()
{
  for (;;)
  {
    //--------------------------------------------------
    // System shutdown
    //--------------------------------------------------
    if (m_ShrObj->ThreadShouldExit())
    {
      m_Logger->Write(Logger::NOTICE,"PreProcessor: ChannelID:%u. Stopping thread.", m_ChannelID);
      return;
    }

    //--------------------------------------------------
    // Report Health
    //--------------------------------------------------
    m_ThreadHealthMon->ReportThatIAmHealthy(ThreadHealthMonitor::PREPROCESSOR, m_ChannelID);

    //--------------------------------------------------
    unsigned long ulTStamp = 0;
    BYTE * pbPkt = NULL;
    if (!m_RawPktCirBuf->GetReadingPtrTStamp(pbPkt,&ulTStamp))
    {
      m_RawPktCirBuf->WaitForData();
      continue;
    }

    //--------------------------------------------------
    // Output Packet Header info
    //--------------------------------------------------
    MDP_Packet_Header * mph = (MDP_Packet_Header*)(pbPkt);
    size_t usSizeMDPPktHdr = sizeof(MDP_Packet_Header);
    SDateTime::fromUnixTimeToString(mph->SendingTime);

    //--------------------------------------------------
    // FIXME subseq msg not processed
    //--------------------------------------------------
    MessageHeader msgHdr;
    int16_t iMsgSize = READ_INT16(&pbPkt[usSizeMDPPktHdr]);
    msgHdr.wrap(pbPkt, usSizeMDPPktHdr+sizeof(iMsgSize), MDP_VERSION, usSizeMDPPktHdr+iMsgSize);

    if (m_PrintPreProcSeqNoAsInfo)
      m_Logger->Write(Logger::INFO, "PreProcessor: ChannelID:%u. %s (%c): Packet Header: MsgSeqNum:  %u", m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), mph->MsgSeqNum);
    else
      m_Logger->Write(Logger::DEBUG,"PreProcessor: ChannelID:%u. %s (%c): Packet Header: MsgSeqNum:  %u", m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), mph->MsgSeqNum);
    m_Logger->Write(Logger::DEBUG,"PreProcessor: ChannelID:%u. %s (%c): Packet Header: Send Time:  %s", m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), SDateTime::fromUnixTimeToString(mph->SendingTime, SDateTime::NANOSEC).c_str());

    //--------------------------------------------------
    // Output other Debug info
    //--------------------------------------------------
    m_Logger->Write(Logger::DEBUG,"PreProcessor: %s : %u : m_RawPktCirBuf.Size()          %u", m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_RawPktCirBuf->Size());
    m_Logger->Write(Logger::DEBUG,"PreProcessor: %s : %u : m_RawPktCirBuf.AllocatedSize() %u", m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_RawPktCirBuf->AllocatedSize());

    //--------------------------------------------------
    // FIXME now missing Error checking
    //--------------------------------------------------

    //--------------------------------------------------
    // Record canned data, with heartbeat
    //--------------------------------------------------
    m_BinaryRecorder.WriteHKExUnixTime(pbPkt);

    //--------------------------------------------------
    // Message Parsing and Json Output
    //--------------------------------------------------
    uint32_t iStartUnadjSeqNo = mph->MsgSeqNum;
    uint64_t iSendTime        = mph->SendingTime;
    size_t   iOffset          = usSizeMDPPktHdr;
    //--------------------------------------------------
    // FIXME missing the number of messages from header...
    //--------------------------------------------------
    // for (int i = 0; i < iMsgCnt; ++i)
    {
      const BYTE *pbMsg = pbPkt+iOffset;
      iOffset += READ_UINT16(pbPkt+iOffset);
      uint32_t iUnadjSeqNo = iStartUnadjSeqNo+i;

      m_Logger->Write(Logger::DEBUG,"PreProcessor: ChannelID:%u. %s : %u : Msg Seq No (unadj): %u", m_ChannelID, m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), iUnadjSeqNo);

      unsigned short usMsgSize = READ_UINT16(pbMsg);
      unsigned short usMsgType = READ_UINT16(pbMsg+2);

      m_Logger->Write(Logger::DEBUG,"PreProcessor: MsgType: %u", usMsgType);
      m_Logger->Write(Logger::DEBUG,"PreProcessor: MsgSize: %u", usMsgSize);

      //--------------------------------------------------
      // Printing results in log
      //--------------------------------------------------
      if (m_bOutputJson) m_DataProcFunc->OutputJsonToLog(m_NameBuffer, m_ChannelID, m_Logger, pbMsg, usMsgType, mph->SendTime);

      //--------------------------------------------------
      // Heartbeat
      //--------------------------------------------------
      if (msgHdr.templateId() == MDP_HEARTBEAT)
      {
        m_Logger->Write(Logger::DEBUG,"PreProcessor: %s : %u : This is a HEARTBEAT packet", m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port());
        continue;
      }

      //--------------------------------------------------
      // Channel Reset
      //--------------------------------------------------
      if (msgHdr.templateId() == MDP_CHANNEL_RESET)
      {

        if (m_McastIdentifier.McastType() == McastIdentifier::REALTIME)
        {
          //--------------------------------------------------
          // RealTime channel
          //--------------------------------------------------
          m_Logger->Write(Logger::NOTICE,"PreProcessor: ChannelID:%u. RT (%c) MDP_CHANNEL_RESET received. Seq No of OMD_REFRESH_COMPLETE(raw/unadj): %u, New Base Seq No (adj): %u. Start Seq No (adj, and before seq reset msg): %u.",
                          m_ChannelID,
                          (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A': 'B'),
                          iUnadjSeqNo,
                          m_LocalLastBaseSeqNo,
                          m_MsgCirBuf->GetStartSeqNo());

          // if (usMsgType == OMD_SEQUENCE_RESET)
          m_Logger->Write(Logger::NOTICE,"PreProcessor: ChannelID:%u. Refresh activated after receiving OMD_SEQUENCE_RESET.", m_ChannelID);
          // else
          //   m_Logger->Write(Logger::NOTICE,"PreProcessor: ChannelID:%u. Refresh activated after detecting sequence number reset..", m_ChannelID);

          m_ShrObj->ActivateRefresh(m_ChannelID);

          //--------------------------------------------------
          // Reset Order Books of this particular channel
          //--------------------------------------------------
          set<unsigned long> * pset = m_ShrObj->GetOrderBookIDInChnl(m_ChannelID);
          set<unsigned long>::iterator its;
          for (its = pset->begin(); its != pset->end(); ++its)
          {
            m_ShrObj->GetOrderBookCache()->GetOrderBook(*its)->Reset();
            m_Logger->Write(Logger::INFO,"PreProcessor: ChannelID:%u. Resetting OrderBook %u upon reception of OMD_SEQUENCE_RESET.", m_ChannelID, *its);
          }
        } // if (REALTIME)
        else
        {
          //--------------------------------------------------
          // Refresh channel
          //--------------------------------------------------
          m_Logger->Write(Logger::NOTICE,"PreProcessor: ChannelID:%u. RF (%c) OMD_SEQUENCE_RESET received. Current Seq No (raw): %u, New Base Seq No: %u",
                          m_ChannelID,
                          (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A': 'B'),
                          iUnadjSeqNo,
                          m_LocalLastBaseSeqNo);
          m_ShrObj->AddRFChnlSeqNoOffset(m_ChannelID, m_LocalLastBaseSeqNo);
        }

        if (!DealingWithSeqNoGaps(m_LocalLastBaseSeqNo)) continue;

        m_MsgCirBuf->PushMsg(pbMsg,m_LocalLastBaseSeqNo,ulTStamp); //should use our local time rather than the time in the OMD packet header

        //--------------------------------------------------
        // Now purge everything in m_MsgCirBuf before the seq reset, otherwise any (obsolete) missing seq no before the reset will prevent RealTimeProcessor from doing things
        // Since the seq no of the cir buf isn't reset to zero, we should not reset m_LocalLastBaseSeqNo
        // Both RT and RF channels should be purged
        //--------------------------------------------------
        m_MsgCirBuf->PurgeMsgB4SeqNoInclusive(m_LocalLastBaseSeqNo);

        //--------------------------------------------------
        // Implicitly reset seq no for Refresh channel as well, see comments above
        //--------------------------------------------------
        if (m_McastIdentifier.McastType() == McastIdentifier::REALTIME && !m_ShrObj->CheckRFSeqResetAlreadyHandled(m_ChannelID, ulRelTime))
        {
          ExpandableCirBuffer4Msg * m_MsgCirBuf_RF = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REFRESH, m_McastIdentifier.Channel_ID());
          uint32_t uiArtificialRFAdjSeqNo = m_MsgCirBuf_RF->GetNextSeqNo();

          m_Logger->Write(Logger::NOTICE,"PreProcessor: ChannelID:%u. Implicit Seq Reset on Refresh channel after getting OMD_SEQUENCE_RESET in RealTime channel. Artificial Seq No (adj) = New Base Seq No (adj): %u.",
                          m_ChannelID,
                          uiArtificialRFAdjSeqNo);

          m_ShrObj->AddRFChnlSeqNoOffset(m_ChannelID, uiArtificialRFAdjSeqNo);
          m_MsgCirBuf_RF->PushMsg(pbMsg,uiArtificialRFAdjSeqNo,ulTStamp);
          m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiArtificialRFAdjSeqNo);
          m_ShrObj->AddRFSeqResetAlreadyHandled(m_ChannelID, ulRelTime);
        }

      } // if (usMsgType == MDP_CHANNEL_RESET)
      else
      {
        //--------------------------------------------------
        // Non OMD_SEQUENCE_RESET
        //--------------------------------------------------
        if (usMsgType == OMDC_AGGREGATE_ORDER_BOOK_UPDATE || usMsgType == OMDD_AGGREGATE_ORDER_BOOK_UPDATE)
        {
          OMDC_Aggregate_Order_Book_Update *oaobu = (OMDC_Aggregate_Order_Book_Update*) pbMsg;
          m_Logger->Write(Logger::DEBUG,"PreProcessor: ChannelID:%u. Discovery: OrderBookID [%u] is received through this channel.", m_ChannelID, oaobu->SecurityCode);
          m_ShrObj->AddOrderBookIDInChnl(m_ChannelID,oaobu->SecurityCode);
        }

        uint32_t uiAdjSeqNo = iUnadjSeqNo + m_LocalLastBaseSeqNo;
        if (!DealingWithSeqNoGaps(uiAdjSeqNo)) continue;

        m_MsgCirBuf->PushMsg(pbMsg,uiAdjSeqNo,ulTStamp);
        // m_LastUnadjSeqNo = iUnadjSeqNo;
      }

      //--------------------------------------------------

      m_RawPktCirBuf->PopFront();
      m_MsgCirBuf->NotifyConsumer();
    } // for (;;)
  }
