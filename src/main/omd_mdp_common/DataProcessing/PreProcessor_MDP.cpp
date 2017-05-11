#include "PreProcessor.h"

PreProcessor_MDP::PreProcessor_MDP(const McastIdentifier & m) : PreProcessor(m)
{
  //--------------------------------------------------
  // set MDP_REFRESH_COMPLETE
  //--------------------------------------------------
  m_MDP_Refresh_Complete.reserve(sizeof(MDP_Packet_Header));
  m_MDP_Refresh_Complete.insert(m_MDP_Refresh_Complete.begin(), sizeof(MDP_Packet_Header), '\0');
  MDP_Packet_Header mph;
  mph.PktSeqNum = mph.SendingTime = MDP_REFRESH_COMPLETE;
  memcpy(&m_MDP_Refresh_Complete[0],&mph,sizeof(MDP_Packet_Header));
}

void PreProcessor_MDP::Run()
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
    // Record canned data, with heartbeat
    //--------------------------------------------------
    m_BinaryRecorder.WriteHKExUnixTime(pbPkt);

    //--------------------------------------------------
    // Output other Debug info
    //--------------------------------------------------
    m_Logger->Write(Logger::DEBUG,"%s: %s : %u ChannelID:%u. %s(%c): m_RawPktCirBuf.Size()          %u", __FILE__, m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), m_RawPktCirBuf->Size());
    m_Logger->Write(Logger::DEBUG,"%s: %s : %u ChannelID:%u. %s(%c): m_RawPktCirBuf.AllocatedSize() %u", __FILE__, m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), m_RawPktCirBuf->AllocatedSize());
    m_Logger->Write(Logger::DEBUG,"%s: %s : %u ChannelID:%u. %s(%c): m_RawPktCirBuf.GetPktSize()    %u", __FILE__, m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_ChannelID, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"), (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'), m_RawPktCirBuf->GetPktSize());

    //--------------------------------------------------
    // Output Packet Header info
    //--------------------------------------------------
    MDP_Packet_Header * mph  = (MDP_Packet_Header*)(pbPkt);
    m_Logger->Write(m_PrintPreProcSeqNoAsInfo ? Logger::INFO : Logger::DEBUG, "%s: %s : %u ChannelID:%u. %s(%c): Packet Header: PktSeqNum:  %u",
                    __FILE__,
                    m_McastIdentifier.IP().c_str(),
                    m_McastIdentifier.Port(),
                    m_ChannelID,
                    (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"),
                    (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'),
                    mph->PktSeqNum);
    m_Logger->Write(Logger::DEBUG,"%s: %s : %u ChannelID:%u. %s(%c): Packet Header: Send Time:  %s",
                    __FILE__,
                    m_McastIdentifier.IP().c_str(),
                    m_McastIdentifier.Port(),
                    m_ChannelID,
                    (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? "RT" : "RF"),
                    (m_McastIdentifier.Channel() == McastIdentifier::A ? 'A':'B'),
                    SDateTime::fromUnixTimeToString(mph->SendingTime,SDateTime::NANOSEC).c_str());

    //--------------------------------------------------
    // We replace the original packet header timestamp with the packet length, so that RealTimeProcessor knows how long the packet is.
    //--------------------------------------------------
    const size_t uiPktSize = m_RawPktCirBuf->GetPktSize();

    //--------------------------------------------------
    // Manually insert an MDP_REFRESH_COMPLETE message into the queue because unlike OMD, CME just cycles the packet sequence number.
    //--------------------------------------------------
    if (mph->PktSeqNum == 1 && m_McastIdentifier.McastType() == McastIdentifier::REFRESH)
    {
      m_MsgCirBuf->GetLatestSeqNo(m_LocalLastAdjSeqNo);
      m_Logger->Write(Logger::DEBUG,"%s: %s : %u Manually inserted MDP_REFRESH_COMPLETE. m_LocalLastAdjSeqNo: %u Size before %u",
                      __FILE__,
                      m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_LocalLastAdjSeqNo, m_MsgCirBuf->Size());
      m_LocalLastAdjSeqNo++;
      m_MsgCirBuf->PushMsg(&m_MDP_Refresh_Complete[0],m_LocalLastAdjSeqNo,ulTStamp,sizeof(MDP_Packet_Header));
      m_Logger->Write(Logger::DEBUG,"%s: %s : %u Manually inserted MDP_REFRESH_COMPLETE. m_LocalLastAdjSeqNo: %u Size after %u",
                      __FILE__,
                      m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(), m_LocalLastAdjSeqNo, m_MsgCirBuf->Size());
    }

    //--------------------------------------------------
    // Because CME keeps the sequence number of packets rather than messages,
    // we defer our message parsing to RealTimeProcessor.
    //--------------------------------------------------
    m_MsgCirBuf->PushMsg(pbPkt,m_LocalLastAdjSeqNo+mph->PktSeqNum,ulTStamp,uiPktSize);
    m_Logger->Write(Logger::INFO,"%s: %s : %u uiPktSize: %u", __FILE__, m_McastIdentifier.IP().c_str(), m_McastIdentifier.Port(),uiPktSize);
    m_RawPktCirBuf->PopFront();
    m_MsgCirBuf->NotifyConsumer();
  }
}
