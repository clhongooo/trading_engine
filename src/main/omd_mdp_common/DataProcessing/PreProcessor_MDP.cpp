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
    m_ThreadHealthMon->ReportHealthy(ThreadHealthMonitor::PREPROCESSOR, m_ChannelID);

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
    MDP_Binary_Packet_Header * mph = (MDP_Binary_Packet_Header*)(pbPkt);


    //--------------------------------------------------

    m_RawPktCirBuf->PopFront();
  } // for (;;)
}
