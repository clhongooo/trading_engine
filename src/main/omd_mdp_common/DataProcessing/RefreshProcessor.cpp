/*
 * RefreshProcessor.cpp
 *
 *  Created on: May 19, 2014
 *      Author: sunny
 */

#include "RefreshProcessor.h"

//--------------------------------------------------
// Factory method
//--------------------------------------------------
RefreshProcessor* RefreshProcessorFactory::GetRefreshProcessor(dma::Identity id, const unsigned short us)
{
  if      (id == dma::OMDC) return new RefreshProcessor_OMD(us);
  else if (id == dma::OMDD) return new RefreshProcessor_OMD(us);
  else if (id == dma::MDP ) return new RefreshProcessor_MDP(us);
}

RefreshProcessor::RefreshProcessor(const unsigned short id) :
  m_bOutputJson(false),
  m_bRunRefreshProcessor(false),
  m_PrintRefreshProcSeqNoAsInfo(false),
  m_PrintOrderBookAsInfo(false),
  m_ChannelID(id),
  m_RefreshProcSleepMillisec(0),
  m_LastCheckedAdjSeqNo(0),
  m_MaintainRefreshActn(false),
  m_RFMsgCirBufProtection(30000),
  m_RTSeqNo1(0),
  m_RTSeqNo2(0)
{

  m_Logger                       = Logger::Instance();
  m_SysCfg                       = SystemConfig::Instance();
  m_ShrObj                       = SharedObjects::Instance();
  m_ThreadHealthMon              = ThreadHealthMonitor::Instance();
  m_MsgCirBuf_RT                 = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, m_ChannelID);
  m_MsgCirBuf_RF                 = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REFRESH, m_ChannelID);
  m_RefreshProcSleepMillisec     = m_SysCfg->GetRefreshProcSleepMillisec();
  m_bRunRefreshProcessor         = m_SysCfg->CheckIfRunRefreshProcessor();


  m_Logger->Write(Logger::NOTICE, "RefreshProcessor: ChannelID:%u.", m_ChannelID);

  if ((*(m_SysCfg->GetRefreshProcessorJson()))[m_ChannelID] == 1) m_bOutputJson = true;

  m_DataProcFunc.reset(DataProcFuncFactory::GetDataProcFunc(m_SysCfg->GetIdentity()));

  m_PrintRefreshProcSeqNoAsInfo = m_SysCfg->CheckIfPrintRefreshProcSeqNoAsInfo();
  m_PrintOrderBookAsInfo        = m_SysCfg->CheckIfPrintOrderBookAsInfo();
  m_RFMsgCirBufProtection       = m_SysCfg->GetRFMsgCirBufProtection();
}

void RefreshProcessor_OMD::Run()
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
    uint32_t uiLatestRFBaseSeqNo = m_ShrObj->CheckRFChnlLatestSeqNoOffset(m_ChannelID);
    m_MaintainRefreshActn = false;
    {
      uint32_t uiRFStartSeqNo = m_MsgCirBuf_RF->GetStartSeqNo();
      unsigned long ulRFSize = m_MsgCirBuf_RF->Size();

      if      (m_LastCheckedAdjSeqNo == 0                       ) m_LastCheckedAdjSeqNo = uiRFStartSeqNo;
      else if (m_LastCheckedAdjSeqNo < uiLatestRFBaseSeqNo      ) m_LastCheckedAdjSeqNo = uiLatestRFBaseSeqNo;
      else if (m_LastCheckedAdjSeqNo < uiRFStartSeqNo           ) m_LastCheckedAdjSeqNo = uiRFStartSeqNo;
      else if (m_LastCheckedAdjSeqNo > uiRFStartSeqNo + ulRFSize) m_LastCheckedAdjSeqNo = uiRFStartSeqNo; // the cir buffer must have been reset...
    }

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
    bool bFoundRFCompl = false;
    uint32_t uiLatestAdjSeqNo = 0;
    bool bR = m_MsgCirBuf_RF->GetLatestSeqNo(uiLatestAdjSeqNo);

    //--------------------------------------------------
    // Problem detection:
    // If the circular buffer is too long, something is probably wrong.
    // By "too long", I mean more than m_RFMsgCirBufProtection
    //--------------------------------------------------


    // //--------------------------------------------------
    // // Method 1: Clear front old messages until first OMD_REFRESH_COMPLETE
    // //--------------------------------------------------
    // if (bR && m_MsgCirBuf_RF->Size() > m_RFMsgCirBufProtection)
    // {
    //   m_Logger->Write(Logger::WARNING,"RefreshProcessor: ChannelID:%u. [PROTECTION] Detected abnormally long message circular buffer with size %u. Try to purge old messages for protection.", m_ChannelID, m_MsgCirBuf_RF->Size());
    //
    //   //--------------------------------------------------
    //   // Just get latest
    //   //--------------------------------------------------
    //   m_MsgCirBuf_RF->GetLatestSeqNo(uiLatestAdjSeqNo);
    //   m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] [Chkpt 1] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u",
    //       m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);
    //   uint32_t uiSeqNoToPurgeB4 = uiLatestAdjSeqNo - m_RFMsgCirBufProtection;
    //
    //   while (uiSeqNoToPurgeB4 < uiLatestAdjSeqNo)
    //   {
    //     BYTE *pbMsg = m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(uiSeqNoToPurgeB4);
    //     if (pbMsg)
    //     {
    //       if (READ_UINT16(pbMsg+2) == OMD_REFRESH_COMPLETE)
    //       {
    //         bFoundRFCompl = true;
    //         break;
    //       }
    //       else uiSeqNoToPurgeB4++;
    //     }
    //     else
    //     {
    //       uiSeqNoToPurgeB4++;
    //     }
    //   }
    //
    //   //--------------------------------------------------
    //   // Just get the latest, otherwise maybe out-of-date
    //   //--------------------------------------------------
    //   m_MsgCirBuf_RF->GetLatestSeqNo(uiLatestAdjSeqNo);
    //
    //   m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] [Chkpt 2] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u",
    //       m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);
    //
    //   if (!bFoundRFCompl)
    //   {
    //     m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] There is no OMD_REFRESH_COMPLETE in the last %u messages. Do nothing now and wait for OMD_REFRESH_COMPLETE.", m_ChannelID, m_RFMsgCirBufProtection, uiSeqNoToPurgeB4);
    //   }
    //   else
    //   {
    //     m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] Found OMD_REFRESH_COMPLETE at %u. Purge old messages at and before it.",
    //         m_ChannelID, uiSeqNoToPurgeB4);
    //     m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiSeqNoToPurgeB4);
    //     m_LastCheckedAdjSeqNo = uiSeqNoToPurgeB4+1;
    //   }
    //
    //   m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] [Chkpt 3] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u", m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);
    //   continue;
    // }

    //--------------------------------------------------
    // Method 2: Clear front old messages before last OMD_REFRESH_COMPLETE
    //--------------------------------------------------
    if (bR && m_MsgCirBuf_RF->Size() > m_RFMsgCirBufProtection)
    {
      m_Logger->Write(Logger::WARNING,"RefreshProcessor: ChannelID:%u. [PROTECTION] Detected abnormally long message circular buffer with size %u. Try to purge all old messages before the latest OMD_REFRESH_COMPLETE for protection.",
          m_ChannelID, m_MsgCirBuf_RF->Size());

      //--------------------------------------------------
      // Just get latest
      //--------------------------------------------------
      m_MsgCirBuf_RF->GetLatestSeqNo(uiLatestAdjSeqNo);
      m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] [Chkpt 1] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u",
          m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);
      uint32_t uiSeqNoToPurgeB4 = uiLatestAdjSeqNo;

      while (uiSeqNoToPurgeB4 > uiLatestAdjSeqNo - m_RFMsgCirBufProtection)
      {
        BYTE *pbMsg = m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(uiSeqNoToPurgeB4);
        if (pbMsg)
        {
          if (READ_UINT16(pbMsg+2) == OMD_REFRESH_COMPLETE)
          {
            bFoundRFCompl = true;
            break;
          }
          else uiSeqNoToPurgeB4--;
        }
        else
        {
          uiSeqNoToPurgeB4--;
        }
      }

      //--------------------------------------------------
      // Just get the latest, otherwise maybe out-of-date
      //--------------------------------------------------
      m_MsgCirBuf_RF->GetLatestSeqNo(uiLatestAdjSeqNo);

      m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] [Chkpt 2] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u",
          m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);

      if (!bFoundRFCompl)
      {
        m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] There is no OMD_REFRESH_COMPLETE in the last %u messages. Purge old messages at and before %u.",
            m_ChannelID, m_RFMsgCirBufProtection, uiSeqNoToPurgeB4);
      }
      else
      {
        m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] Found OMD_REFRESH_COMPLETE at %u. Purge old messages at and before it.",
            m_ChannelID, uiSeqNoToPurgeB4);
      }

      m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiSeqNoToPurgeB4);
      m_LastCheckedAdjSeqNo = uiSeqNoToPurgeB4+1;

      m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. [PROTECTION] [Chkpt 3] RF circular buffer size: %u. RF Start Seq No: %u. Latest Adj Seq No: %u. LastCheckedAdjSeqNo: %u", m_ChannelID, m_MsgCirBuf_RF->Size(), m_MsgCirBuf_RF->GetStartSeqNo(), uiLatestAdjSeqNo, m_LastCheckedAdjSeqNo);
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
    // We will just keep checking the message circular buffer for the message OMD_REFRESH_COMPLETE.
    // We don't process / pop any message, yet.
    //--------------------------------------------------
    // Peek the Msg Type of our newly arrived msg
    //--------------------------------------------------
    bool bDirtyFlag = false;
    uint64_t ulTStamp = 0;
    bool bRChkDirFlgTS = m_MsgCirBuf_RF->CheckDirtyFlagTStamp(m_LastCheckedAdjSeqNo,bDirtyFlag,ulTStamp);

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

    BYTE *pbMsg = m_MsgCirBuf_RF->GetMsgPtrOfSeqNo(m_LastCheckedAdjSeqNo);
    if (!pbMsg)
    {
      m_Logger->Write(Logger::ERROR,"RefreshProcessor: ChannelID:%u. Inconsistent internal state in circular buffer. Please debug.", m_ChannelID);
      m_LastCheckedAdjSeqNo++;
      continue;
    }

    //--------------------------------------------------
    // Peek inside
    //--------------------------------------------------
    uint16_t usMsgType = 0;
    usMsgType = READ_UINT16(pbMsg+2);

    //--------------------------------------------------
    // OMD_REFRESH_COMPLETE
    //--------------------------------------------------
    if (usMsgType == OMD_REFRESH_COMPLETE)
    {
      const uint32_t & uiAdjSeqNoOfRefCompl = m_LastCheckedAdjSeqNo; // just for easy reading
      const uint32_t uiLatestRTBaseSeqNo = m_ShrObj->CheckRTChnlLatestSeqNoOffset(m_ChannelID);

      OMD_Refresh_Complete *orc = (OMD_Refresh_Complete*) pbMsg;
      uint32_t uiRefComplUnadjLastSeqNo = orc->LastSeqNum;
      uint32_t uiRefComplAdjLastSeqNo = uiRefComplUnadjLastSeqNo + uiLatestRTBaseSeqNo;

      //--------------------------------------------------
      // If there are missing msg even after arbitration, we can't use this batch
      // If Refresh Activation flag is not set, no need to process refresh msg
      // If Refresh Activation flag is set, but the RealTime circular buffer doesn't contain the last seq no at all
      //--------------------------------------------------
      bool bProcessThisRefreshBatch = true;

      if (!m_ShrObj->CheckRefreshActivatnStatus(m_ChannelID))
      {
        m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. OMD_REFRESH_COMPLETE received. Will purge this refresh batch since refresh mode is not activated.", m_ChannelID);
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
            m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. OMD_REFRESH_COMPLETE received. Refresh mode is activated, however, there are missing seq no in this refresh batch no (e.g. %u). Give 2nd chance. Wait 2 more seconds.",
                m_ChannelID, uiSmltMissingAdjSeqNo);
            boost::this_thread::sleep(boost::posix_time::seconds(2));
            iNoOfTimesChecked++;
          }
          else if (iNoOfTimesChecked > 0)
          {
            m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. OMD_REFRESH_COMPLETE received. Refresh mode is activated, however, there are missing seq no in this refresh batch no (e.g. %u). Must wait for the next batch of refresh snapshot.",
                m_ChannelID, uiSmltMissingAdjSeqNo);
            bProcessThisRefreshBatch = false;
            break;
          }
        }
        else
          break;
      }

      if (bProcessThisRefreshBatch)
      {
        uint32_t uiRTMsgCirBufStartAdjSeqNo = m_MsgCirBuf_RT->GetStartSeqNo();

        if (uiRefComplUnadjLastSeqNo == 0)
        {
          m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. OMD_REFRESH_COMPLETE received. Refresh mode is activated. Last Seq No in the OMD_REFRESH_COMPLETE message is 0.", m_ChannelID);
          bProcessThisRefreshBatch = true;
        }
        else
        {
          if (bProcessThisRefreshBatch && uiRTMsgCirBufStartAdjSeqNo > uiRefComplAdjLastSeqNo)
          {
            bProcessThisRefreshBatch = false; // default is false
            m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. OMD_REFRESH_COMPLETE received. Refresh mode is activated. However, RT circular buffer Start Seq No (adj): %u, RF OMD_REFRESH_COMPLETE field LastSeqNum: (raw) %u (adj) %u. Must wait for the next round of refresh snapshot.",
                m_ChannelID, uiRTMsgCirBufStartAdjSeqNo, uiRefComplUnadjLastSeqNo, uiRefComplAdjLastSeqNo);

            //--------------------------------------------------
            // Calculating the velocity of Refresh Complete coverage
            // And warn if it is estimated to take absurdly long to finish the refresh
            //--------------------------------------------------
            m_RTSeqNo2 = m_RTSeqNo1;
            m_RTSeqNo1 = uiRefComplUnadjLastSeqNo;
            if (m_RTSeqNo1 > 0 && m_RTSeqNo2 > 0)
            {
              long lSeqNoCoveragePerRefCompl = m_RTSeqNo1 - m_RTSeqNo2;
              if (lSeqNoCoveragePerRefCompl > 0)
              {
                long lNoOfTimesReqToCatchUp = (long)((double)(uiRTMsgCirBufStartAdjSeqNo - uiRefComplAdjLastSeqNo) / (double)lSeqNoCoveragePerRefCompl);
                if (lNoOfTimesReqToCatchUp > 3)
                {
                  m_Logger->Write(Logger::WARNING,"RefreshProcessor: ChannelID:%u. Num of Seq No covered per OMD_REFRESH_COMPLETE: %d. Estimated to take %d rounds of OMD_REFRESH_COMPLETE before a Refresh can be done.",
                      m_ChannelID, lSeqNoCoveragePerRefCompl, lNoOfTimesReqToCatchUp);
                  bProcessThisRefreshBatch = true;
                  m_MaintainRefreshActn = true;
                }
                else
                {
                  //--------------------------------------------------
                  // Just 3 rounds, can wait
                  //--------------------------------------------------
                  bProcessThisRefreshBatch = false;
                  m_MaintainRefreshActn = false;
                }
              }
            }
          }
        }

        //--------------------------------------------------
        // Wait for RT to catch up in case RF is too fast
        // Shouldn't happen in real life?
        //--------------------------------------------------
        while (bProcessThisRefreshBatch && uiRefComplAdjLastSeqNo > uiRTMsgCirBufStartAdjSeqNo + m_MsgCirBuf_RT->Size())
        {
          boost::this_thread::sleep(boost::posix_time::milliseconds(m_RefreshProcSleepMillisec));
        }

      }


      if (!bProcessThisRefreshBatch)
      {
        boost::this_thread::sleep(boost::posix_time::milliseconds(m_RefreshProcSleepMillisec));

        m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiAdjSeqNoOfRefCompl);
        continue;
      }

      //--------------------------------------------------
      // Otherwise need to process msg...
      //--------------------------------------------------
      m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. OMD_REFRESH_COMPLETE received. Refresh mode is activated and so will process this batch.", m_ChannelID);

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

      //--------------------------------------------------
      // Process the current batch of refresh messages one by one
      //--------------------------------------------------
      bool bWaitForNextBatch = false;
      for (;;)
      {
        BYTE* pbMsg2 = NULL;
        uint32_t uiSeqNo2 = 0;
        uint64_t ulTS2 = 0;

        StateOfNextSeqNo snsn = m_MsgCirBuf_RF->GetMsgSeqNoTStamp(pbMsg2, &uiSeqNo2, &ulTS2);
        if (snsn == SEQNO_AVAILABLE)
        {
          uint16_t uiMsgSize2 = READ_UINT16(pbMsg2);
          uint16_t usMsgType2 = READ_UINT16(pbMsg2+2);

          //--------------------------------------------------
          // Output Message Header info
          //--------------------------------------------------
          m_Logger->Write(Logger::DEBUG,"RefreshProcessor: ChannelID:%u. Message Header: Msg size:                 %u", m_ChannelID, uiMsgSize2);
          m_Logger->Write(Logger::DEBUG,"RefreshProcessor: ChannelID:%u. Message Header: Msg type:                 %u", m_ChannelID, usMsgType2);
          m_Logger->Write(Logger::DEBUG,"RefreshProcessor: ChannelID:%u. Message Header: Seq No:                   %u", m_ChannelID, uiSeqNo2);
          m_Logger->Write(Logger::DEBUG,"RefreshProcessor: ChannelID:%u. Message Header: RT Latest Base Seq No:    %u", m_ChannelID, uiLatestRTBaseSeqNo);

          if (m_PrintRefreshProcSeqNoAsInfo)
            m_Logger->Write(Logger::INFO ,"RefreshProcessor: ChannelID:%u. Actual Seq No: %u, Latest RT Base Seq No: %u", m_ChannelID, uiSeqNo2, uiLatestRTBaseSeqNo);
          else
            m_Logger->Write(Logger::DEBUG,"RefreshProcessor: ChannelID:%u. Actual Seq No: %u, Latest RT Base Seq No: %u", m_ChannelID, uiSeqNo2, uiLatestRTBaseSeqNo);


          //--------------------------------------------------
          // Printing Json to log
          //--------------------------------------------------
          if (m_bOutputJson)
          {
            memset(m_JsonBuffer,'\0',JSON_BUFFER_SIZE);
            m_DataProcFunc->OutputJsonToLog(__FILE__,m_ChannelID,m_Logger,pbMsg2,m_JsonBuffer);
          }

          //--------------------------------------------------
          // Process message for omd_mdi
          //--------------------------------------------------
          m_DataProcFunc->ProcessMessageForTransmission(pbMsg2, usMsgType2);
          //--------------------------------------------------

          if (usMsgType2 == OMDC_AGGREGATE_ORDER_BOOK_UPDATE || usMsgType2 == OMDD_AGGREGATE_ORDER_BOOK_UPDATE)
          {
            m_DataProcFunc->ProcessOrderBookInstruction(__FILE__,m_Logger,pbMsg2,m_ShrObj,m_PrintOrderBookAsInfo);
          }

          //--------------------------------------------------
          // End of this batch
          //--------------------------------------------------
          if (usMsgType2 == OMD_REFRESH_COMPLETE)
            break;

        }
        else if (snsn == ALL_RETRIEVED)
        {
          m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. Nothing to process in the RF circular buffer. Probably purged elsewhere. Need to wait for the next refresh batch.", m_ChannelID);
          bWaitForNextBatch = true;
          break;
        }
        else if (snsn == SEQNO_MISSING)
        {
          uint32_t uiSmltMissingAdjSeqNo = 0;
          if (m_MsgCirBuf_RF->GetSmallestMissingSeqNo(uiSmltMissingAdjSeqNo))
          {
            m_Logger->Write(Logger::ERROR,"RefreshProcessor: ChannelID:%u. Missing seq no detected but that's impossible. Please check. Current missing Seq No (adj): %u. SmallestMissingSeqNo (adj): %u. OMD_REFRESH_COMPLETE Seq No (adj): %u. Last Seq No in OMD_REFRESH_COMPLETE message: %u (adj) or %u (raw).",
                m_ChannelID, uiSeqNo2, uiSmltMissingAdjSeqNo, uiAdjSeqNoOfRefCompl, uiRefComplAdjLastSeqNo, uiRefComplUnadjLastSeqNo);
          }
          else
          {
            m_Logger->Write(Logger::ERROR,"RefreshProcessor: ChannelID:%u. Missing seq no detected but that's impossible. Please check. Current no missing Seq No in circular buffer. OMD_REFRESH_COMPLETE Seq No (adj): %u. Last Seq No in OMD_REFRESH_COMPLETE message: %u (adj) or %u (raw).",
                m_ChannelID, uiAdjSeqNoOfRefCompl, uiRefComplAdjLastSeqNo, uiRefComplUnadjLastSeqNo);
          }
        }

        m_MsgCirBuf_RF->PopFront();
      }

      if (bWaitForNextBatch) continue;

      {
        uint32_t uiRTStartSeqNo = m_MsgCirBuf_RT->GetStartSeqNo();
        uint32_t uiRTSize       = m_MsgCirBuf_RT->Size();
        uint32_t uiRFStartSeqNo = m_MsgCirBuf_RF->GetStartSeqNo();
        uint32_t uiRFSize       = m_MsgCirBuf_RF->Size();

        //--------------------------------------------------
        // Extra warnings
        //--------------------------------------------------
        if (uiRTStartSeqNo + uiRTSize < uiRefComplAdjLastSeqNo)
        {
          m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. Now we are preparing to purge RT circular buffer. The Last Seq No in OMD_REFRESH_COMPLETE is (raw) %u (adj) %u, but RT circular buffer starts with Seq No (adj) %u and it has Size %u, which may or may not be a problem...",
              m_ChannelID, uiRefComplUnadjLastSeqNo, uiRefComplAdjLastSeqNo, uiRTStartSeqNo, uiRTSize);
        }

        //--------------------------------------------------
        // Before purging the RT circular buffer, try to process it as much as possible. Afterall, the data in RT circular buffer is valuable.
        //--------------------------------------------------
        if (m_bOutputJson)
        {
          const size_t LEN = 256;
          char sSrcDescriptionInJson[LEN];
          memset(sSrcDescriptionInJson, '\0', LEN);
          strcpy(sSrcDescriptionInJson, __FILE__);
          strcat(sSrcDescriptionInJson, " (Possible Duplicate)");

          BYTE* pbMsg2 = NULL;
          uint32_t uiSeqNo2 = 0;
          uint64_t ulTS2 = 0;

          StateOfNextSeqNo snsn = m_MsgCirBuf_RT->GetMsgSeqNoTStamp(pbMsg2, &uiSeqNo2, &ulTS2);
          while (!m_MsgCirBuf_RT->Empty() && uiSeqNo2 <= uiRefComplAdjLastSeqNo)
          {
            if (snsn == SEQNO_AVAILABLE)
            {
              uint16_t uiMsgSize2 = READ_UINT16(pbMsg2);
              uint16_t usMsgType2 = READ_UINT16(pbMsg2+2);

              //--------------------------------------------------
              // Printing Json to log
              //--------------------------------------------------
              memset(m_JsonBuffer,'\0',JSON_BUFFER_SIZE);
              m_DataProcFunc->OutputJsonToLog(sSrcDescriptionInJson,m_ChannelID,m_Logger,pbMsg2,m_JsonBuffer);
            }
            m_MsgCirBuf_RT->PopFront();
            snsn = m_MsgCirBuf_RT->GetMsgSeqNoTStamp(pbMsg2, &uiSeqNo2, &ulTS2);
          }
        }

        //--------------------------------------------------
        // Purge all msg, including this OMD_REFRESH_COMPLETE
        //--------------------------------------------------
        m_MsgCirBuf_RF->PurgeMsgB4SeqNoInclusive(uiAdjSeqNoOfRefCompl);
        m_MsgCirBuf_RT->PurgeMsgB4SeqNoInclusive(uiRefComplAdjLastSeqNo);

        if (uiRTSize == 0)
          m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. Refresh completed. RT circular buffer: Start Seq No: %u (adj), Size %u. (i.e. last seq no that contains data is %u). Seq No before %u (adj) have been purged.",
              m_ChannelID, uiRTStartSeqNo, uiRTSize, uiRTStartSeqNo-1, uiRefComplAdjLastSeqNo);
        else
          m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. Refresh completed. RT circular buffer: Start Seq No: %u (adj), Size %u. Seq No before %u (adj) have been purged.",
              m_ChannelID, uiRTStartSeqNo, uiRTSize, uiRefComplAdjLastSeqNo);

        if (uiRFSize == 0)
          m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. Refresh completed. RF circular buffer: Start Seq No: %u (adj), Size %u. (i.e. last seq no that contains data is %u). Seq No before %u (adj) have been purged.",
              m_ChannelID, uiRFStartSeqNo, uiRFSize, uiRFStartSeqNo-1, uiAdjSeqNoOfRefCompl);
        else
          m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. Refresh completed. RF circular buffer: Start Seq No: %u (adj), Size %u. Seq No before %u (adj) have been purged.",
              m_ChannelID, uiRFStartSeqNo, uiRFSize, uiAdjSeqNoOfRefCompl);

      }


      //--------------------------------------------------
      // Deactivate at the very last, since RealTimeProcessor will start processing again after we reset this flag
      //--------------------------------------------------
      if (!m_MaintainRefreshActn)
      {
        m_ShrObj->DeactivateRefresh(m_ChannelID);
        m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. Deactivated refresh mode.", m_ChannelID);
      }
      else
      {
        m_Logger->Write(Logger::NOTICE,"RefreshProcessor: ChannelID:%u. Maintain refresh activation mode.", m_ChannelID);
        m_MaintainRefreshActn = false;
      }
    } // if (usMsgType == OMD_REFRESH_COMPLETE)

    m_LastCheckedAdjSeqNo++;

  } // for (;;)
}

void RefreshProcessor_MDP::Run()
{
}
