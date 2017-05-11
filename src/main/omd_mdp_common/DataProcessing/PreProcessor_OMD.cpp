#include "PreProcessor.h"

void PreProcessor_OMD::Run()
{
  for (;;)
  {
    //--------------------------------------------------
    // System shutdown
    //--------------------------------------------------
    if (m_ShrObj->ThreadShouldExit())
    {
      m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Stopping thread.", __FILE__, m_ChannelID);
      return;
    }

    //--------------------------------------------------
    // Report Health
    //--------------------------------------------------
    m_ThreadHealthMon->ReportThatIAmHealthy(ThreadHealthMonitor::PREPROCESSOR, m_ChannelID);

    //--------------------------------------------------
    unsigned long ulTStamp = 0;
    BYTE * pbPktMut = NULL;
    if (!m_RawPktCirBuf->GetReadingPtrTStamp(pbPktMut,&ulTStamp))
    {
      m_RawPktCirBuf->WaitForData();
      continue;
    }
    const BYTE * pbPkt = pbPktMut;

    //--------------------------------------------------
    // Output Packet Header info
    //--------------------------------------------------
    OMD_Packet_Header * oph = (OMD_Packet_Header*)(pbPkt);
    if (oph->PktSize > 0 && oph->PktSize <= MAX_OMD_PACKET_SIZE)
    {

      m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. %s (%c): Packet Header: Packet size:   %u", __FILE__, m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), oph->PktSize);
      m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. %s (%c): Packet Header: Msg Count:     %u", __FILE__, m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), oph->MsgCount);
      m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. %s (%c): Packet Header: Base Seq No:   %u", __FILE__, m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), m_LocalLastBaseSeqNo);
      m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. %s (%c): Packet Header: Send Time:     %s", __FILE__, m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), SDateTime::fromUnixTimeToString(oph->SendTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT).c_str());

      //--------------------------------------------------
      // Output other Debug info
      //--------------------------------------------------
      m_Logger->Write(Logger::DEBUG,"%s: %s : %u : m_RawPktCirBuf.Size()          %u", __FILE__, m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_RawPktCirBuf->Size());
      m_Logger->Write(Logger::DEBUG,"%s: %s : %u : m_RawPktCirBuf.AllocatedSize() %u", __FILE__, m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_RawPktCirBuf->AllocatedSize());

      if (m_PrintPreProcSeqNoAsInfo)
        m_Logger->Write(Logger::INFO ,"%s: ChannelID:%u. %s (%c): Packet Header: Actual Seq No: %u", __FILE__, m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), oph->SeqNum);
      else
        m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. %s (%c): Packet Header: Actual Seq No: %u", __FILE__, m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), oph->SeqNum);

    }

    //--------------------------------------------------
    // Error checking
    //--------------------------------------------------
    else if (oph->PktSize > MAX_OMD_PACKET_SIZE)
    {
      m_Logger->Write(Logger::WARNING,"%s: ChannelID:%u. %s (%c): Received erroneous packet size: Send Time: %s, PktSize = %u.",
                      __FILE__,
                      m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), SDateTime::fromUnixTimeToString(oph->SendTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT).c_str(), oph->PktSize);
      m_RawPktCirBuf->PopFront();
      continue;
    }
    else if (oph->PktSize == 0)
    {
      m_RawPktCirBuf->PopFront();
      continue;
    }

    // //--------------------------------------------------
    // // Guarding the crazy Session 2, Test case for security code 288
    // //--------------------------------------------------
    // if (oph->SendTime < m_PrevPktHdrTime - 86400000000000) // # of nano sec in 1 day
    // {
    //   m_Logger->Write(Logger::WARNING,"%s: ChannelID:%u. %s (%c): Trashed packet with erroneous Send Time in packet header: %s.",
    //       __FILE__,
    //       m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), SDateTime::fromUnixTimeToString(oph->SendTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT).c_str());
    //   m_RawPktCirBuf->PopFront();
    //   continue;
    // }
    // else
    // {
    //   m_PrevPktHdrTime = oph->SendTime;
    // }

    //--------------------------------------------------
    // Record canned data, with heartbeat
    //--------------------------------------------------
    m_BinaryRecorder.WriteHKExUnixTime(pbPkt);

    //--------------------------------------------------
    // Heartbeat
    //--------------------------------------------------
    size_t iMsgCnt = READ_UINT8(&(oph->MsgCount));
    if (iMsgCnt == 0)
    {
      m_Logger->Write(Logger::DEBUG,"%s: %s : %u : This is a HEARTBEAT packet", __FILE__, m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port());
      m_RawPktCirBuf->PopFront();
      m_MsgCirBuf->NotifyConsumer();
      continue;
    }

    // //--------------------------------------------------
    // // Record canned data without heartbeat
    // //--------------------------------------------------
    // if (m_bRecordMcast)
    // {
    //   {
    //     //--------------------------------------------------
    //     // HKEx simulator format
    //     //--------------------------------------------------
    //     //--------------------------------------------------
    //     // Record Mcast as "canned data"
    //     // Writing to file (in the format of HKEx simulator)
    //     //--------------------------------------------------
    //     unsigned short iPktLen = READ_UINT16(pbPkt);
    //     unsigned short iMsgCnt = READ_UINT8(pbPkt+2);
    //
    //     //--------------------------------------------------
    //     // If it's Heartbeat, don't record it to file
    //     //--------------------------------------------------
    //     if (iMsgCnt == 0)
    //     {
    //       m_RawPktCirBuf->PopFront();
    //       continue;
    //     }
    //     fwrite(pbPkt+1,1,1,m_CannedMcastFile);
    //     fwrite(pbPkt,1,1,m_CannedMcastFile);
    //     fwrite(pbPkt,1,iPktLen,m_CannedMcastFile);
    //     fflush(m_CannedMcastFile);
    //   }
    // }

    //--------------------------------------------------
    // Message Parsing and Json Output
    //--------------------------------------------------
    uint32_t iStartUnadjSeqNo = oph->SeqNum;
    uint64_t iSendTime = oph->SendTime;
    size_t iOffset = sizeof(OMD_Packet_Header);
    for (int i = 0; i < iMsgCnt; ++i)
    {
      const BYTE *pbMsg = pbPkt+iOffset;
      iOffset += READ_UINT16(pbPkt+iOffset);
      uint32_t iUnadjSeqNo = iStartUnadjSeqNo+i;

      m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. %s : %u : Msg Seq No (unadj): %u", __FILE__, m_ChannelID, m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), iUnadjSeqNo);

      unsigned short usMsgSize = READ_UINT16(pbMsg);
      unsigned short usMsgType = READ_UINT16(pbMsg+2);

      m_Logger->Write(Logger::DEBUG,"%s: MsgType: %u", __FILE__, usMsgType);
      m_Logger->Write(Logger::DEBUG,"%s: MsgSize: %u", __FILE__, usMsgSize);

      //--------------------------------------------------
      // Printing results in log
      //--------------------------------------------------
      if (m_bOutputJson)
      {
        memset(m_JsonBuffer,'\0',JSON_BUFFER_SIZE);
        m_DataProcFunc->OutputJsonToLog(m_NameBuffer, m_ChannelID, m_Logger, pbMsg, usMsgType, m_JsonBuffer, oph->SendTime);
      }

      //--------------------------------------------------
      // Check latest seq reset base num
      // Of course if both streams A and B have missed the OMD_SEQUENCE_RESET message, then there's no way we'll know the correct base number
      //
      // Should check every time now because system reset can be called from the terminal at any given time
      //--------------------------------------------------
      if (m_McastIdentifier.McastType() == McastIdentifier::REALTIME)
        m_LocalLastBaseSeqNo = m_ShrObj->CheckRTChnlLatestSeqNoOffset(m_ChannelID);
      else
        m_LocalLastBaseSeqNo = m_ShrObj->CheckRFChnlLatestSeqNoOffset(m_ChannelID);

      //--------------------------------------------------
      // Handling OMD_SEQUENCE_RESET
      //--------------------------------------------------
      //
      //--------------------------------------------------
      // Where to handle sequence reset
      // Can't handle in RealTimeProcessor and RefreshProcessor, because already need to adjust sequence number with the base offset before putting them in the circular buffer.
      //--------------------------------------------------
      // Why purge msg cir buffer on seq reset?
      // Because if you don't, any missing packets in the cir buffer before reset will trigger recovery.
      // RTS server won't entertain us. Then we wait for refresh. But since the last seq num in refresh are already reset too, we will have to wait almost forever.
      // Then rather than having an offset for the seq no, why not simply purge the msg cir buffer? Because I knew this too late...
      //--------------------------------------------------
      // Refresh activation
      // Upon reception of a RT seq no reset, Refresh mode should be "Activated" so that the latest snapshot can be obtained.
      //--------------------------------------------------
      //
      //--------------------------------------------------
      // Time of seq reset messages
      //--------------------------------------------------
      // Refresh:   09:51       11:20 12:16 14:49 16:04 17:10 17:33
      // Real-time:       10:51 11:20 12:16 14:49 16:04
      //
      //--------------------------------------------------
      // ***
      // From the experience of Readiness test (Session 5 10:51)
      // At 10:51, HKEx sent out seq reset for RT only, but both RT RF have their seq no reset
      // Therefore don't trust HKEx on this seq reset message
      // Not sure about the actual behaviour in production
      //--------------------------------------------------
      // ***
      // Session 2: 288 11280
      // The seq reset at 14:59 had raw seq no 1, which is different from what I expected.
      // I expected the raw seq no to follow from previous messages
      //--------------------------------------------------
      //--------------------------------------------------
      // Session 5: Test case 1: (09:00 - 09:20) ***
      // After the Sequence Reset messages and the Securities Definition messages for market open, OMD sends out another Sequence Reset message in each channel.
      // Clients are expected to clear the securities information in cache and replace it with data from the new Securities Definition sent after the second round of Sequence Reset messages.
      //
      // Session 5: Test case 2: (09:20 - 10:20)
      // OMD simulates a real-time data publisher failure that will trigger a failover of the affected component.
      // This interruption should be transparent to clients except SS clients may receive duplicate trade tickers (i.e. trade ticker ID same as one received previously) which they should discard
      //
      // Session 5: Test case 3: (09:20 - 10:20) ***
      // OMD simulates a refresh (RFS) service failure that will trigger a failover, as a result, a sequence reset (100) message will be transmitted in each of the refresh channel.
      // *** Clients are expected to handle RFS failover properly without affecting the reception of real-time market data and can capture a full latest market image from RFS.
      //
      // Session 5: Test case 4: (09:20 - 10:20)
      // OMD simulates a primary Retransmission (RTS) server failure as a result only the secondary RTS server remains operational to provide service.
      // Clients are expected to detect such failure and auto-switch to the secondary RTS server to make retransmission requests.
      //
      // Session 5: Test case 5: (11:00 - 11:30) ***
      // OMD simulates a primary site failure resulting in failover to the DR site. Upon the takeover of the DR site, OMD will deliver a sequence reset (100) messages from each channels.
      // Clients are expected to clear the data in cache upon receipt of the sequence reset messages and then obtain the latest market image from the refresh service and real-time channels at the DR site.
      //--------------------------------------------------
      if (usMsgType == OMD_SEQUENCE_RESET)// ||                     // explicit seq no reset
        //(iUnadjSeqNo < 20 && iUnadjSeqNo < m_LastUnadjSeqNo))     // implicit seq no reset, this will cause other problems, so disabled
      {

        //--------------------------------------------------
        // Since there are channels A and B, need to guard against doing OMD_SEQUENCE_RESET twice
        //--------------------------------------------------
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff = now - m_ProgramStartTime;
        unsigned long ulRelTime = diff.total_milliseconds();

        if (m_McastIdentifier.McastType() == McastIdentifier::REALTIME && !m_ShrObj->CheckRTSeqResetAlreadyHandled(m_ChannelID, ulRelTime))
        {
          //--------------------------------------------------
          // Special guard against the unexpected / wrong seq no of 1 at OMDC Readiness Test 14:50, Channel 20. It doesn't follow from previous seq no
          //--------------------------------------------------
          if (iUnadjSeqNo == 1 && m_MsgCirBuf->GetStartSeqNo() > 1)
          {
            m_LocalLastBaseSeqNo = m_MsgCirBuf->GetNextSeqNo(); // not +=
          }
          else // normal expected case
          {
            m_LocalLastBaseSeqNo += iUnadjSeqNo;
          }
          m_ShrObj->AddRTSeqResetAlreadyHandled(m_ChannelID, ulRelTime);
        }
        else if (m_McastIdentifier.McastType() == McastIdentifier::REFRESH && !m_ShrObj->CheckRFSeqResetAlreadyHandled(m_ChannelID, ulRelTime))
        {
          //--------------------------------------------------
          // Special guard against the unexpected / wrong seq no of 1 at OMDC Readiness Test 14:50, Channel 20. It doesn't follow from previous seq no
          //--------------------------------------------------
          if (iUnadjSeqNo == 1 && m_MsgCirBuf->GetStartSeqNo() > 1)
          {
            m_LocalLastBaseSeqNo = m_MsgCirBuf->GetNextSeqNo(); // not +=
          }
          else // normal expected case
          {
            m_LocalLastBaseSeqNo += iUnadjSeqNo;
          }
          m_ShrObj->AddRFSeqResetAlreadyHandled(m_ChannelID, ulRelTime);
        }
        else
        {
          m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. R%c (%c) OMD_SEQUENCE_RESET received. But we have already processed it in the other line. Skipping",
                          __FILE__,
                          m_ChannelID,
                          (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? 'T' : 'F'),
                          (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A': 'B'));
          continue;
        }

        if (m_McastIdentifier.McastType() == McastIdentifier::REALTIME)
        {
          //--------------------------------------------------
          // RealTime channel
          //--------------------------------------------------
          m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. RT (%c) OMD_SEQUENCE_RESET received. Seq No of OMD_REFRESH_COMPLETE(raw/unadj): %u, New Base Seq No (adj): %u. Start Seq No (adj, and before seq reset msg): %u.",
                          __FILE__,
                          m_ChannelID,
                          (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A': 'B'),
                          iUnadjSeqNo,
                          m_LocalLastBaseSeqNo,
                          m_MsgCirBuf->GetStartSeqNo());
          m_ShrObj->AddRTChnlSeqNoOffset(m_ChannelID, m_LocalLastBaseSeqNo);

          // if (usMsgType == OMD_SEQUENCE_RESET)
          m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Refresh activated after receiving OMD_SEQUENCE_RESET.", __FILE__, m_ChannelID);
          // else
          //   m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Refresh activated after detecting sequence number reset..", __FILE__, m_ChannelID);

          m_ShrObj->ActivateRefresh(m_ChannelID);

          //--------------------------------------------------
          // Reset Order Books of this particular channel
          //--------------------------------------------------
          set<unsigned long> * pset = m_ShrObj->GetOrderBookIDInChnl(m_ChannelID);
          set<unsigned long>::iterator its;
          for (its = pset->begin(); its != pset->end(); ++its)
          {
            m_ShrObj->GetOrderBookCache()->GetOrderBook(*its)->Reset();
            m_Logger->Write(Logger::INFO,"%s: ChannelID:%u. Resetting OrderBook %u upon reception of OMD_SEQUENCE_RESET.", __FILE__, m_ChannelID, *its);
          }
        } // if (REALTIME)
        else
        {
          //--------------------------------------------------
          // Refresh channel
          //--------------------------------------------------
          m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. RF (%c) OMD_SEQUENCE_RESET received. Current Seq No (raw): %u, New Base Seq No: %u",
                          __FILE__,
                          m_ChannelID,
                          (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A': 'B'),
                          iUnadjSeqNo,
                          m_LocalLastBaseSeqNo);
          m_ShrObj->AddRFChnlSeqNoOffset(m_ChannelID, m_LocalLastBaseSeqNo);
        }

        if (!DealingWithSeqNoGaps(m_LocalLastBaseSeqNo)) continue;

        m_MsgCirBuf->PushMsg(pbMsg,m_LocalLastBaseSeqNo,ulTStamp,READ_UINT16((BYTE*)pbMsg)); //should use our local time rather than the time in the OMD packet header

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

          m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Implicit Seq Reset on Refresh channel after getting OMD_SEQUENCE_RESET in RealTime channel. Artificial Seq No (adj) = New Base Seq No (adj): %u.",
                          __FILE__,
                          m_ChannelID,
                          uiArtificialRFAdjSeqNo);

          m_ShrObj->AddRFChnlSeqNoOffset(m_ChannelID, uiArtificialRFAdjSeqNo);
          m_MsgCirBuf_RF->PushMsg(pbMsg,uiArtificialRFAdjSeqNo,ulTStamp,READ_UINT16((BYTE*)pbMsg));
          m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiArtificialRFAdjSeqNo);
          m_ShrObj->AddRFSeqResetAlreadyHandled(m_ChannelID, ulRelTime);
        }

      } // if (usMsgType == OMD_SEQUENCE_RESET)
      else
      {
        //--------------------------------------------------
        // Non OMD_SEQUENCE_RESET
        //--------------------------------------------------
        if (usMsgType == OMDC_AGGREGATE_ORDER_BOOK_UPDATE || usMsgType == OMDD_AGGREGATE_ORDER_BOOK_UPDATE)
        {
          OMDC_Aggregate_Order_Book_Update *oaobu = (OMDC_Aggregate_Order_Book_Update*) pbMsg;
          m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. Discovery: OrderBookID [%u] is received through this channel.", __FILE__, m_ChannelID, oaobu->SecurityCode);
          m_ShrObj->AddOrderBookIDInChnl(m_ChannelID,oaobu->SecurityCode);
        }

        uint32_t uiAdjSeqNo = iUnadjSeqNo + m_LocalLastBaseSeqNo;
        if (!DealingWithSeqNoGaps(uiAdjSeqNo)) continue;

        m_MsgCirBuf->PushMsg(pbMsg,uiAdjSeqNo,ulTStamp,READ_UINT16((BYTE*)pbMsg));
        // m_LastUnadjSeqNo = iUnadjSeqNo;
      }

    } //for (int i = 0; i < iMsgCnt; ++i)
    //--------------------------------------------------

    m_RawPktCirBuf->PopFront();
    m_MsgCirBuf->NotifyConsumer();
  } // for (;;)
}

bool PreProcessor_OMD::DealingWithSeqNoGaps(uint32_t uiAdjSeqNo)
{
  //--------------------------------------------------
  // Ref: Session 2, Test case for security code 288
  //
  // Initially:
  //           m_LocalLastAdjSeqNo == 0, and we won't check for gaps
  //
  // Ridiculously huge gap:
  //           Activate refresh and dump the packet
  //
  // Huge gap:
  //           Because there is a protection mechanism in our circular buffers (MaxOneTimeAlloc)
  //           Old messages will be purged. It's alright but get the latest snapshot first.
  //--------------------------------------------------
  if (m_LocalLastAdjSeqNo == 0)
  {
    m_LocalLastAdjSeqNo = uiAdjSeqNo;
    return true;
  }
  else if (uiAdjSeqNo > m_LocalLastAdjSeqNo + m_TrashSeqNoGapLargerThan)
  {
    //--------------------------------------------------
    // Ridiculous gap
    //--------------------------------------------------
    m_ShrObj->ActivateRefresh(m_ChannelID);
    return false;
  }
  else if (uiAdjSeqNo > m_LocalLastAdjSeqNo + m_MaxOneTimeAlloc)
  {
    //--------------------------------------------------
    // Huge gap
    //--------------------------------------------------
    m_ShrObj->ActivateRefresh(m_ChannelID);
    m_LocalLastAdjSeqNo = uiAdjSeqNo;
    return true;
  }
  //--------------------------------------------------
  // Normal case
  //--------------------------------------------------
  m_LocalLastAdjSeqNo = uiAdjSeqNo;
  return true;
}
