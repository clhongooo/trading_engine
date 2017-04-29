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

    m_Logger->Write(Logger::DEBUG,"RefreshProcessor: ChannelID:%u. m_LastCheckedAdjSeqNo: %u uiRFStartSeqNo: %u ulRFSize: %u",
                    m_ChannelID, m_LastCheckedAdjSeqNo, uiRFStartSeqNo, ulRFSize);

    //--------------------------------------------------
    // System shutdown
    //--------------------------------------------------
    if (m_ShrObj->ThreadShouldExit())
    {
      m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. Stopping.", m_ChannelID);
      return;
    }

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
      m_Logger->Write(Logger::WARNING,"RefreshProcessor: ChannelID:%u. [PROTECTION] Detected abnormally long message circular buffer with size %u. Try to purge all old messages for protection.",
                      m_ChannelID, m_MsgCirBuf_RF->Size());

      m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u",
                      m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);

      m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiLatestAdjSeqNo);
      m_LastCheckedAdjSeqNo = uiLatestAdjSeqNo;

      m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u",
                      m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);
      continue;
    }

    //--------------------------------------------------
    // There is no new refresh msg
    //--------------------------------------------------
    if (m_MsgCirBuf_RF->Empty() || !bR || (bR && uiLatestAdjSeqNo < m_LastCheckedAdjSeqNo))
    {
      boost::this_thread::sleep(boost::posix_time::milliseconds(m_RefreshProcSleepMillisec));
      m_Logger->Write(Logger::DEBUG,"RefreshProcessor: ChannelID:%u. uiLatestAdjSeqNo: %u m_LastCheckedAdjSeqNo: %u.", m_ChannelID, uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);
      continue;
    }

    //--------------------------------------------------
    // We will just keep checking the message circular buffer for the message MDP_REFRESH_COMPLETE.
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
    else if (bRChkDirFlgTS && !bDirtyFlag && uiLatestAdjSeqNo > m_LastCheckedAdjSeqNo + 20)
    {
      // Cir buffer entry still not filled. But we may already be lagging behind. So skip.
      m_LastCheckedAdjSeqNo++;
      continue;
    }
    else if (bRChkDirFlgTS && !bDirtyFlag && uiLatestAdjSeqNo <= m_LastCheckedAdjSeqNo + 20)
    {
      // Cir buffer entry still not filled. Can wait.
      continue;
    }

    //--------------------------------------------------
    // Look at the packet pointed of PktSeqNum m_LastCheckedAdjSeqNo
    //--------------------------------------------------
    const BYTE *pbPkt = m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(m_LastCheckedAdjSeqNo);
    if (!pbPkt)
    {
      m_Logger->Write(Logger::ERROR,"RefreshProcessor: ChannelID:%u. Inconsistent internal state in circular buffer. Please debug.", m_ChannelID);
      m_LastCheckedAdjSeqNo++;
      continue;
    }

    //--------------------------------------------------
    // If not MDP_REFRESH_COMPLETE, keep saving to queue first
    //--------------------------------------------------
    const Modified_MDP_Packet_Header * mmph = (Modified_MDP_Packet_Header *) pbPkt;
    if (mmph->PktSeqNum != MDP_REFRESH_COMPLETE || mmph->PktSize != MDP_REFRESH_COMPLETE)
    {
      m_LastCheckedAdjSeqNo++;
      m_Logger->Write(Logger::DEBUG,"RefreshProcessor: ChannelID:%u. Not MDP_REFRESH_COMPLETE", m_ChannelID);
      continue;
    }

    //--------------------------------------------------
    // If MDP_REFRESH_COMPLETE
    //--------------------------------------------------
    m_Logger->Write(Logger::DEBUG,"RefreshProcessor: ChannelID:%u. MDP_REFRESH_COMPLETE", m_ChannelID);
    VAL & uiAdjSeqNoOfRefCompl = m_LastCheckedAdjSeqNo; // just for easy reading

    bool bProcessThisRefreshBatch = true;

    if (!m_ShrObj->CheckRefreshActivatnStatus(m_ChannelID))
    {
      m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. MDP_REFRESH_COMPLETE received. Will purge this refresh batch since refresh mode is not activated.", m_ChannelID);
      bProcessThisRefreshBatch = false;
    }

    short iNoOfTimesChecked = 0;
    for (;;)
    {
      uint32_t uiSmltMissingAdjSeqNo = 0;
      if (bProcessThisRefreshBatch &&
          (m_MsgCirBuf_RF->GetSmallestMissingSeqNo(uiSmltMissingAdjSeqNo) && uiSmltMissingAdjSeqNo < uiAdjSeqNoOfRefCompl))
      {
        if (iNoOfTimesChecked == 0)
        {
          m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. MDP_REFRESH_COMPLETE received. Refresh mode is activated, however, there are missing seq no in this refresh batch no (e.g. %u). Give 2nd chance. Wait 2 more seconds.",
                          m_ChannelID, uiSmltMissingAdjSeqNo);
          boost::this_thread::sleep(boost::posix_time::seconds(2));
          iNoOfTimesChecked++;
        }
        else if (iNoOfTimesChecked > 0)
        {
          m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. MDP_REFRESH_COMPLETE received. Refresh mode is activated, however, there are missing seq no in this refresh batch no (e.g. %u). Must wait for the next batch of refresh snapshot.",
                          m_ChannelID, uiSmltMissingAdjSeqNo);
          bProcessThisRefreshBatch = false;
          break;
        }
      }
      else
        break;
    }

    {
      //--------------------------------------------------
      // Check if we have enough RT packets to continue the snapshots
      //--------------------------------------------------
      const uint32_t rfStartSeqNo =  m_MsgCirBuf_RF->GetStartSeqNo();
      uint32_t rfLatestSeqNo; m_MsgCirBuf_RF->GetLatestSeqNo(rfLatestSeqNo);

      vector<uint32_t> vSeqNo;
      vSeqNo.push_back(rfStartSeqNo);
      vSeqNo.push_back(rfLatestSeqNo);

      mktdata::MessageHeader mdpMsgHdr;
      FForEach(vSeqNo,[&](const uint32_t iSeqNo) {
        const BYTE* pbPktTmp = m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(iSeqNo);
        vector<uint32_t> vLastMsgSeqNum = m_DataProcFunc->Get_LastMsgSeqNumProcessed(pbPktTmp);
        FForEach(vLastMsgSeqNum,[&](const uint32_t iLastSeqNo) { m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. LastMsgSeqNumProcessed: %u", m_ChannelID, iLastSeqNo); });
      });


      VAL uiRTMsgCirBufStartSeqNo = m_MsgCirBuf_RT->GetStartSeqNo();
    }

    if (!bProcessThisRefreshBatch)
    {
      boost::this_thread::sleep(boost::posix_time::milliseconds(m_RefreshProcSleepMillisec));
      m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiAdjSeqNoOfRefCompl);
      continue;
    }



    m_LastCheckedAdjSeqNo++;

  } // for (;;)
}
