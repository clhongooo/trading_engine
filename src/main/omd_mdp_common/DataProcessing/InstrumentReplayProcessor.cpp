#include "InstrumentReplayProcessor.h"

//--------------------------------------------------
// Factory method
//--------------------------------------------------
InstrumentReplayProcessor* InstrumentReplayProcessorFactory::GetInstrumentReplayProcessor(dma::Identity id, const unsigned short us)
{
  if      (id == dma::OMDC) return NULL;
  else if (id == dma::OMDD) return NULL;
  else if (id == dma::MDP ) return new InstrumentReplayProcessor_MDP(us);
}

InstrumentReplayProcessor::InstrumentReplayProcessor(const unsigned short id) :
  m_bRunInstrumentReplayProcessor(false),
  m_PrintInstrumentReplaySeqNoAsInfo(false),
  m_ChannelID(id)
{
  m_Logger = Logger::Instance();
  m_SysCfg = SystemConfig::Instance();
  m_ShrObj = SharedObjects::Instance();
  m_ThreadHealthMon = ThreadHealthMonitor::Instance();
  m_Logger->Write(Logger::NOTICE, "%s: ChannelID:%u,", __FILENAME__, m_ChannelID);
  m_MsgCirBuf_IR = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::INSTRUMENTREPLAY, m_ChannelID);
  m_bRunInstrumentReplayProcessor = m_SysCfg->CheckIfRunInstrumentReplayProcessor();

  m_DataProcFunc.reset(DataProcFuncFactory::GetDataProcFunc(m_SysCfg->GetIdentity()));
}
