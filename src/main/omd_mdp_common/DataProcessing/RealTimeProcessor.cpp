#include "RealTimeProcessor.h"

//--------------------------------------------------
// Factory method
//--------------------------------------------------
RealTimeProcessor* RealTimeProcessorFactory::GetRealTimeProcessor(SystemConfig::Identity id, const unsigned short us)
{
  if      (id == SystemConfig::OMDC) return new RealTimeProcessor_OMD(us);
  else if (id == SystemConfig::OMDD) return new RealTimeProcessor_OMD(us);
  else if (id == SystemConfig::MDP ) return new RealTimeProcessor_MDP(us);
}

RealTimeProcessor::RealTimeProcessor(const unsigned short id) :
  m_CannedProcessedDataFile(NULL),
  m_bRecordProcessedData(false),
  m_bOutputJson(false),
  m_bRunRealTimeProcessor(false),
  m_PrintRealTimeSeqNoAsInfo(false),
  m_PrintOrderBookAsInfo(false),
  m_ChannelID(id)
{
  m_Logger = Logger::Instance();
  m_SysCfg = SystemConfig::Instance();
  m_ShrObj = SharedObjects::Instance();
  m_ThreadHealthMon = ThreadHealthMonitor::Instance();
  m_Logger->Write(Logger::NOTICE, "RealTimeProcessor: ChannelID:%u,", m_ChannelID);
  m_MsgCirBuf = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, m_ChannelID);
  m_bRunRealTimeProcessor = m_SysCfg->CheckIfRunRealTimeProcessor();

  boost::shared_ptr<vector<unsigned short> > pviCannedChnl = m_SysCfg->GetRealTimeProcCannedChnl();
  for (int i = 0; i < (*pviCannedChnl).size(); ++i)
    if ((*pviCannedChnl)[i] == m_ChannelID) m_bRecordProcessedData = true;

  if ((*(m_SysCfg->GetRealTimeProcessorJson()))[m_ChannelID] == 1) m_bOutputJson = true;


  if (m_bRecordProcessedData)
  {
    ostringstream oo;
    oo << SystemConfig::Instance()->GetCannedProcessedDataFilePath() << "_" << m_ChannelID;
    m_CannedProcessedDataFile = fopen(oo.str().c_str(), m_SysCfg->GetCannedProcessedDataFopenFlag().c_str());
  }

  m_DataProcFunc.reset(DataProcFuncFactory::GetDataProcFunc(m_SysCfg->GetIdentity()));

  m_PrintRealTimeSeqNoAsInfo = m_SysCfg->CheckIfPrintRealTimeProcSeqNoAsInfo();
  m_PrintOrderBookAsInfo     = m_SysCfg->CheckIfPrintOrderBookAsInfo();

}

RealTimeProcessor::~RealTimeProcessor()
{
  if (m_CannedProcessedDataFile)
  {
    fclose(m_CannedProcessedDataFile);
    delete m_CannedProcessedDataFile;
    m_CannedProcessedDataFile = NULL;
  }
}
