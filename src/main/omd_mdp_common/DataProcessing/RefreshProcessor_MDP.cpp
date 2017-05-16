#include "RefreshProcessor.h"

void RefreshProcessor_MDP::Run()
{
  for (;;)
  {
    //--------------------------------------------------
    // Report Health
    //--------------------------------------------------
    m_ThreadHealthMon->ReportThatIAmHealthy(ThreadHealthMonitor::REFRESHPROCESSOR, m_ChannelID);

    //--------------------------------------------------
    // System shutdown
    //--------------------------------------------------
    if (m_ShrObj->ThreadShouldExit())
    {
      m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Stopping.", __FILENAME__, m_ChannelID);
      return;
    }

    //--------------------------------------------------
    // Init / Self-correction
    //--------------------------------------------------
    // FIXME what's this
    m_MaintainRefreshActn = false;
    //--------------------------------------------------

    VAL uiRFStartSeqNo = m_MsgCirBuf_RF->GetStartSeqNo();
    VAL ulRFSize       = m_MsgCirBuf_RF->Size();

    if      (m_LastCheckedAdjSeqNo == 0                       ) m_LastCheckedAdjSeqNo = uiRFStartSeqNo;
    else if (m_LastCheckedAdjSeqNo < uiRFStartSeqNo           ) m_LastCheckedAdjSeqNo = uiRFStartSeqNo;
    else if (m_LastCheckedAdjSeqNo > uiRFStartSeqNo + ulRFSize) m_LastCheckedAdjSeqNo = uiRFStartSeqNo; // the cir buffer must have been reset...

    //--------------------------------------------------
    // Get latest seq no
    //--------------------------------------------------
    uint32_t uiLatestAdjSeqNo = 0;
    VAL bR = m_MsgCirBuf_RF->GetLatestSeqNo(uiLatestAdjSeqNo);

    //--------------------------------------------------
    // Problem detection:
    // If the circular buffer is too long, something is probably wrong.
    // By "too long", I mean more than m_RFMsgCirBufProtection
    //--------------------------------------------------
    if (bR && m_MsgCirBuf_RF->Size() > m_RFMsgCirBufProtection)
    {
      m_Logger->Write(Logger::WARNING,"%s: ChannelID:%u. [PROTECTION] Detected abnormally long message circular buffer with size %u. Try to purge all old messages for protection.",
                      __FILENAME__, m_ChannelID, m_MsgCirBuf_RF->Size());

      m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. [PROTECTION] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u",
                      __FILENAME__, m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);

      m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiLatestAdjSeqNo);
      m_LastCheckedAdjSeqNo = uiLatestAdjSeqNo;

      m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. [PROTECTION] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u",
                      __FILENAME__, m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);
      continue;
    }

    //--------------------------------------------------
    // There is no new refresh msg
    //--------------------------------------------------
    if (m_MsgCirBuf_RF->Empty() || !bR || (bR && uiLatestAdjSeqNo < m_LastCheckedAdjSeqNo))
    {
      boost::this_thread::sleep(boost::posix_time::milliseconds(m_RefreshProcSleepMillisec));
      continue;
    }

    //--------------------------------------------------
    // We will just keep checking the message circular buffer for the message MDP_CYCLE_COMPLETE.
    // We don't process / pop any message, yet.
    //--------------------------------------------------
    bool bDirtyFlag = false;
    uint64_t ulTStamp = 0;
    VAL bRChkDirFlgTS = m_MsgCirBuf_RF->CheckDirtyFlagTStamp(m_LastCheckedAdjSeqNo,bDirtyFlag,ulTStamp);

    //--------------------------------------------------
    // Cases where we need to move to the next m_LastCheckedAdjSeqNo
    //--------------------------------------------------
    if (!bRChkDirFlgTS && m_LastCheckedAdjSeqNo < uiLatestAdjSeqNo)
    {
      // Way too old m_LastCheckedAdjSeqNo
      m_LastCheckedAdjSeqNo = m_MsgCirBuf_RF->GetStartSeqNo();
      continue;
    }
    else if (!bRChkDirFlgTS && m_LastCheckedAdjSeqNo > uiLatestAdjSeqNo)
    {
      m_LastCheckedAdjSeqNo = uiLatestAdjSeqNo;
      continue;
    }

    //--------------------------------------------------
    // Look at the packet pointed of PktSeqNum m_LastCheckedAdjSeqNo
    //--------------------------------------------------
    const BYTE *pbPkt = m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(m_LastCheckedAdjSeqNo);
    const uint16_t usMsgSize = m_MsgCirBuf_RF->GetMsgSizeOfSeqNo(m_LastCheckedAdjSeqNo);
    if (!pbPkt)
    {
      m_Logger->Write(Logger::ERROR,"%s: ChannelID:%u. Inconsistent internal state in circular buffer. Please debug.", __FILENAME__, m_ChannelID);
      m_LastCheckedAdjSeqNo++;
      continue;
    }

    //--------------------------------------------------
    // Peek
    //--------------------------------------------------
    m_DataProcFunc->HandleMDPRaw(pbPkt, m_ChannelID, McastIdentifier::ToString(McastIdentifier::REFRESH), usMsgSize, DataProcFunctions::DPF_PRINT_ONLY);

    {
      //--------------------------------------------------
      // If not MDP_CYCLE_COMPLETE, keep saving to queue first
      //--------------------------------------------------
      const MDP_Packet_Header * mph = (MDP_Packet_Header *) pbPkt;
      if (mph->PktSeqNum != MDP_CYCLE_COMPLETE)
      {
        m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. Not MDP_CYCLE_COMPLETE. m_LastCheckedAdjSeqNo: %u uiRFStartSeqNo: %u ulRFSize: %u uiLatestAdjSeqNo: %u usMsgSize: %u mph->PktSeqNum: %u mph->SendingTime: %s",
                        __FILENAME__, m_ChannelID, m_LastCheckedAdjSeqNo, uiRFStartSeqNo, ulRFSize, uiLatestAdjSeqNo, usMsgSize, mph->PktSeqNum, SDateTime::fromUnixTimeToString(mph->SendingTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::GMT).c_str());
        m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. Not MDP_CYCLE_COMPLETE. Peeking inside...", __FILENAME__, m_ChannelID);
        m_DataProcFunc->PeekTemplateID(pbPkt, m_ChannelID, McastIdentifier::ToString(McastIdentifier::REFRESH), usMsgSize);
        m_LastCheckedAdjSeqNo++;
        continue;
      }
    }

    //--------------------------------------------------
    // If MDP_CYCLE_COMPLETE
    //--------------------------------------------------
    VAL & uiAdjSeqNoOfRefCompl = m_LastCheckedAdjSeqNo; // just for easy reading
    m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. Received MDP_CYCLE_COMPLETE. m_MsgCirBuf_RF->Size(): %u uiAdjSeqNoOfRefCompl: %u", __FILENAME__, m_ChannelID, m_MsgCirBuf_RF->Size(),  uiAdjSeqNoOfRefCompl);

    //--------------------------------------------------
    // Just to make sure packets before MDP_CYCLE_COMPLETE has all arrived.
    //--------------------------------------------------
    boost::this_thread::sleep(boost::posix_time::seconds(1));

    if (!m_ShrObj->CheckRefreshActivatnStatus(m_ChannelID))
    {
      m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. MDP_CYCLE_COMPLETE received. Will purge this refresh batch since refresh mode is not activated.", __FILENAME__, m_ChannelID);

      boost::this_thread::sleep(boost::posix_time::milliseconds(m_RefreshProcSleepMillisec));
      m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiAdjSeqNoOfRefCompl);
      m_LastCheckedAdjSeqNo = uiAdjSeqNoOfRefCompl;
      continue;
    }

    {
      //--------------------------------------------------
      // Check for missing sequence number
      //--------------------------------------------------
      uint32_t uiLargestMissingSeqNo = 0;
      if (m_MsgCirBuf_RF->GetLargestMissingSeqNo(uiLargestMissingSeqNo))
      {
        m_LastCheckedAdjSeqNo = max(uiAdjSeqNoOfRefCompl,uiLargestMissingSeqNo);
        m_Logger->Write(Logger::INFO,"%s: ChannelID:%u. There are missing messages in m_MsgCirBuf_RF. Largest missing seq no %u. Wait for next refresh batch. Purge m_MsgCirBuf_RF up to %u", __FILENAME__, m_ChannelID, uiLargestMissingSeqNo, m_LastCheckedAdjSeqNo);
        m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(m_LastCheckedAdjSeqNo);
        m_MsgCirBuf_RF->GetLargestMissingSeqNo(uiLargestMissingSeqNo);
        m_Logger->Write(Logger::INFO,"%s: ChannelID:%u. m_MsgCirBuf_RF: Largest missing seq no %u. Size: %u", __FILENAME__, m_ChannelID, uiLargestMissingSeqNo, m_MsgCirBuf_RF->Size());
        continue;
      }

      //--------------------------------------------------
      // Check for missing sequence number
      //--------------------------------------------------
      if (m_MsgCirBuf_RT->GetLargestMissingSeqNo(uiLargestMissingSeqNo))
      {
        m_Logger->Write(Logger::INFO,"%s: ChannelID:%u. There are missing messages in m_MsgCirBuf_RT. Largest missing seq no %u. Purge m_MsgCirBuf_RT up to the missing seq no.", __FILENAME__, m_ChannelID, uiLargestMissingSeqNo);
        m_MsgCirBuf_RT->PurgeMsgB4SeqNoInclusive(uiLargestMissingSeqNo);
      }
    }

    //--------------------------------------------------
    // Get sequence no range
    //--------------------------------------------------
    const uint32_t uiRfStartSeqNo = m_MsgCirBuf_RF->GetStartSeqNo();
    uint32_t rfLatestSeqNo; m_MsgCirBuf_RF->GetLatestSeqNo(rfLatestSeqNo); const uint32_t & uiRfLatestSeqNo = rfLatestSeqNo;
    vector<uint32_t> vSeqNoRange; FRange(vSeqNoRange,uiRfStartSeqNo,uiRfLatestSeqNo);

    //--------------------------------------------------
    // Check if we have enough RT packets to continue the snapshots
    //--------------------------------------------------
    set<uint32_t> sLastMsgSeqNumProc;
    FForEach(vSeqNoRange,[&](const uint32_t & iSeqNo) {
      //--------------------------------------------------
      // FIXME m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(iSeqNo), m_MsgCirBuf_RF->GetMsgSize(iSeqNo)
      //--------------------------------------------------
      vector<uint32_t> vLastMsgSeqNumProcTmp = m_DataProcFunc->Get_LastMsgSeqNumProcessed(m_ChannelID, m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(iSeqNo), m_MsgCirBuf_RF->GetMsgSizeOfSeqNo(iSeqNo));
      for (int i = 0; i < vLastMsgSeqNumProcTmp.size(); ++i) sLastMsgSeqNumProc.insert(vLastMsgSeqNumProcTmp[i]);
    });

    FForEach(sLastMsgSeqNumProc,[&](const uint32_t iLastSeqNo) { m_Logger->Write(Logger::DEBUG,"%s: ChannelID:%u. LastMsgSeqNumProcessed: %u", __FILENAME__, m_ChannelID, iLastSeqNo); });
    //--------------------------------------------------
    // Some checkings to make sure the messages are present in m_MsgCirBuf_RT and m_MsgCirBuf_RF
    //--------------------------------------------------
    const uint32_t uiMinLastMsgSeqNumProc = FMin(sLastMsgSeqNumProc);
    uint32_t dummy = 0;
    const uint32_t uiRTMsgCirBufStartSeqNo = m_MsgCirBuf_RT->GetStartSeqNo();
    if (uiRTMsgCirBufStartSeqNo > uiMinLastMsgSeqNumProc + 1)
    {
      m_Logger->Write(Logger::INFO,"%s: ChannelID:%u. uiRTMsgCirBufStartSeqNo %u is larger than uiMinLastMsgSeqNumProc %u. Wait for next refresh batch.",
                      __FILENAME__, m_ChannelID, uiRTMsgCirBufStartSeqNo, uiMinLastMsgSeqNumProc);
      m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiAdjSeqNoOfRefCompl);
      m_LastCheckedAdjSeqNo = uiAdjSeqNoOfRefCompl;
      continue;
    }

    //--------------------------------------------------
    // Is our refresh batch complete? Check for seq no 1
    //--------------------------------------------------
    {
      const MDP_Packet_Header * mph = (MDP_Packet_Header *) m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(m_MsgCirBuf_RF->GetStartSeqNo());
      if (mph->PktSeqNum != 1)
      {
        m_Logger->Write(Logger::INFO,"%s: ChannelID:%u. There are missing messages in this refresh batch. Wait for next refresh batch.", __FILENAME__, m_ChannelID);
        m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiAdjSeqNoOfRefCompl);
        m_LastCheckedAdjSeqNo = uiAdjSeqNoOfRefCompl;
        continue;
      }
    }

    //--------------------------------------------------
    // Handle all messages for now
    //--------------------------------------------------
    for (vector<uint32_t>::iterator it = vSeqNoRange.begin(); it != vSeqNoRange.end(); ++it)
    {
      const uint32_t & iSeqNo = *it;
      const uint16_t usMsgSizeTmp = m_MsgCirBuf_RF->GetMsgSizeOfSeqNo(iSeqNo);
      const BYTE* pbPktTmp = m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(iSeqNo);
      m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. HandleMDPRaw() Looping through refresh messages. iSeqNo %u", __FILENAME__, m_ChannelID, iSeqNo);
      m_DataProcFunc->HandleMDPRaw(pbPktTmp, m_ChannelID, McastIdentifier::ToString(McastIdentifier::REFRESH), usMsgSizeTmp, DataProcFunctions::DPF_DO_ACTUAL_PROCESSING);
    }
    m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. HandleMDPRaw() Finished looping through refresh messages.", __FILENAME__, m_ChannelID);

    //--------------------------------------------------
    // Clean queues
    //--------------------------------------------------
    m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiAdjSeqNoOfRefCompl);
    m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Refresh completed. RF circular buffer: Seq No before %u (adj) have been purged.", __FILENAME__, m_ChannelID, uiAdjSeqNoOfRefCompl);
    //--------------------------------------------------
    // FIXME!!  m_MsgCirBuf_RT->PurgeMsgB4SeqNoInclusive(uiMinLastMsgSeqNumProc);
    //--------------------------------------------------
    m_MsgCirBuf_RT->PurgeMsgB4SeqNoInclusive(uiMinLastMsgSeqNumProc);
    m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Refresh completed. RT circular buffer: Seq No before %u (adj) have been purged.", __FILENAME__, m_ChannelID, uiMinLastMsgSeqNumProc);

    //--------------------------------------------------
    // DeactivateRefresh
    //--------------------------------------------------
    m_ShrObj->DeactivateRefresh(m_ChannelID);
    m_Logger->Write(Logger::NOTICE,"%s: ChannelID:%u. Deactivated refresh mode.", __FILENAME__, m_ChannelID);

    m_LastCheckedAdjSeqNo = uiAdjSeqNoOfRefCompl;

  } // for (;;)
}
