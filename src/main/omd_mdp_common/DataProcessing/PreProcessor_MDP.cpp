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
      m_Logger->Write(Logger::NOTICE,"PreProcessor: ChannelID: %u. Stopping thread.", m_ChannelID);
      return;
    }

    //--------------------------------------------------
    // Report Health
    //--------------------------------------------------
    m_ThreadHealthMon->ReportThatIAmHealthy(ThreadHealthMonitor::PREPROCESSOR, m_ChannelID);

    //--------------------------------------------------
    // Try get data from queue
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
    MDP_Packet_Header          * mph  = (MDP_Packet_Header*)(pbPkt);
    Modified_MDP_Packet_Header * mmph = (Modified_MDP_Packet_Header*)(pbPkt);
    m_Logger->Write(m_PrintPreProcSeqNoAsInfo ? Logger::INFO : Logger::DEBUG, "PreProcessor: ChannelID: %u. %s(%c): Packet Header: PktSeqNum:  %u", m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), mph->PktSeqNum);
    m_Logger->Write(Logger::DEBUG,"PreProcessor: ChannelID: %u. %s(%c): Packet Header: Send Time:  %s", m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), SDateTime::fromUnixTimeToString(mph->SendingTime, SDateTime::NANOSEC).c_str());

    //--------------------------------------------------
    // Output other Debug info
    //--------------------------------------------------
    m_Logger->Write(Logger::DEBUG,"PreProcessor: %s : %u : m_RawPktCirBuf.Size()          %u", m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_RawPktCirBuf->Size());
    m_Logger->Write(Logger::DEBUG,"PreProcessor: %s : %u : m_RawPktCirBuf.AllocatedSize() %u", m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_RawPktCirBuf->AllocatedSize());
    m_Logger->Write(Logger::DEBUG,"PreProcessor: %s : %u : m_RawPktCirBuf.GetPktSize()    %u", m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_RawPktCirBuf->GetPktSize());

    //--------------------------------------------------
    // Record canned data, with heartbeat
    //--------------------------------------------------
    m_BinaryRecorder.WriteHKExUnixTime(pbPkt);

    //--------------------------------------------------
    // We replace the original packet header timestamp with the packet length, so that RealTimeProcessor knows how long the packet is.
    //--------------------------------------------------
    const size_t uiPktSize = m_RawPktCirBuf->GetPktSize();
    WRITE_UINT64(&(mmph->PktSize),uiPktSize);

    //--------------------------------------------------
    // Manually insert an MDP_REFRESH_COMPLETE message into the queue because unlike OMD, CME just cycles the packet sequence number.
    //--------------------------------------------------
    if (mph->PktSeqNum == 1 && m_McastIdentifier.McastType() == McastIdentifier::REFRESH)
    {
      m_MsgCirBuf->GetLatestSeqNo(m_LocalLastAdjSeqNo);
      m_LocalLastAdjSeqNo++;
      m_MsgCirBuf->PushMsg(&m_MDP_Refresh_Complete[0],m_LocalLastAdjSeqNo,ulTStamp);
      m_Logger->Write(Logger::DEBUG,"PreProcessor: %s : %u : Manually inserted MDP_REFRESH_COMPLETE. m_LocalLastAdjSeqNo: %u", m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_LocalLastAdjSeqNo);
    }

    //--------------------------------------------------
    // Because CME keeps the sequence number of packets rather than messages,
    // we defer our message parsing to RealTimeProcessor.
    //--------------------------------------------------
    m_MsgCirBuf->PushMsg(pbPkt,m_LocalLastAdjSeqNo+mph->PktSeqNum,ulTStamp);
    m_RawPktCirBuf->PopFront();
    m_MsgCirBuf->NotifyConsumer();
  }
}
