#include "PreProcessor.h"

//--------------------------------------------------
// Factory method
//--------------------------------------------------
PreProcessor* PreProcessorFactory::GetPreProcessor(SystemConfig::Identity id, const McastIdentifier & m)
{
  if      (id == SystemConfig::OMDC) return new PreProcessor_OMD(m);
  else if (id == SystemConfig::OMDD) return new PreProcessor_OMD(m);
  else if (id == SystemConfig::MDP ) return new PreProcessor_MDP(m);
}

PreProcessor::PreProcessor(const McastIdentifier & mi) :
  m_bRecordMcast(false),
  m_bOutputJson(false),
  m_PrintPreProcSeqNoAsInfo(false),
  m_McastIdentifier(mi),
  m_ChannelID(0),
  // m_LastUnadjSeqNo(0),
  // m_PrevPktHdrTime(0),
  m_MaxOneTimeAlloc(0),
  m_TrashSeqNoGapLargerThan(0),
  m_LocalLastAdjSeqNo(0)
{
  //--------------------------------------------------
  // Initialization
  //--------------------------------------------------
  m_Logger = Logger::Instance();
  m_SysCfg = SystemConfig::Instance();
  m_ShrObj = SharedObjects::Instance();
  m_ThreadHealthMon = ThreadHealthMonitor::Instance();

  m_Logger->Write(Logger::NOTICE, "PreProcessor: ChannelID:%u. IP %s, Port %u", mi.Channel_ID(), mi.IP().c_str(), mi.Port());

  m_ChannelID = m_McastIdentifier.Channel_ID();

  m_RawPktCirBuf = m_ShrObj->GetRawPktCirBufPtr(m_McastIdentifier);
  m_MsgCirBuf = m_ShrObj->GetMsgCirBufPtr(m_McastIdentifier.McastType(), m_McastIdentifier.Channel_ID());
  m_MaxOneTimeAlloc = SystemConfig::Instance()->GetMaxOneTimeAlloc();
  m_TrashSeqNoGapLargerThan = SystemConfig::Instance()->GetTrashSeqNoGapLargerThan();

  if (mi.McastType() == McastIdentifier::REALTIME)
  {
    boost::shared_ptr<vector<unsigned short> > pviRTCannedChnl = m_SysCfg->GetPreProcessorRTCannedChnl();
    for (int i = 0; i < (*pviRTCannedChnl).size(); ++i)
      if ((*pviRTCannedChnl)[i] == mi.Channel_ID()) m_bRecordMcast = true;

    if ((*(m_SysCfg->GetPreProcessorRTJson()))[mi.Channel_ID()] == 1) m_bOutputJson = true;
  }
  else if (mi.McastType() == McastIdentifier::REFRESH)
  {
    boost::shared_ptr<vector<unsigned short> > pviRFCannedChnl = m_SysCfg->GetPreProcessorRFCannedChnl();
    for (int i = 0; i < (*pviRFCannedChnl).size(); ++i)
      if ((*pviRFCannedChnl)[i] == mi.Channel_ID()) m_bRecordMcast = true;

    if ((*(m_SysCfg->GetPreProcessorRFJson()))[mi.Channel_ID()] == 1) m_bOutputJson = true;
  }

  //--------------------------------------------------
  strcpy(m_NameBuffer, __FILE__);
  strcat(m_NameBuffer, (m_McastIdentifier.McastType() == McastIdentifier::REALTIME ? ": RealTime(" : ": Refresh("));
  strcat(m_NameBuffer, (m_McastIdentifier.Channel() == McastIdentifier::A ? "A)" : "B)"));

  //--------------------------------------------------
  // Canned data
  //--------------------------------------------------
  m_ProgramStartTime = m_ShrObj->GetProgramStartTime();

  ostringstream oo;
  oo << SystemConfig::Instance()->GetCannedMcastFilePath() << "_" <<
    m_McastIdentifier.Channel_ID() << "_" <<
    ((m_McastIdentifier.McastType() == McastIdentifier::REALTIME) ? "RT" : "RF") << "_" <<
    ((m_McastIdentifier.Channel() == McastIdentifier::A) ? "A" : "B") << "_" <<
    to_iso_string(m_ProgramStartTime).substr(0,15);
  //m_McastIdentifier.IP() << "_" << (unsigned short)(m_McastIdentifier.Port());

  m_BinaryRecorder.SetIfWriteATUMDIStruct(m_bRecordMcast);
  if (m_bRecordMcast && !m_BinaryRecorder.SetOutFilePathAndOpen(oo.str().c_str(), m_SysCfg->GetCannedMcastFopenFlag().c_str()))
    m_Logger->Write(Logger::ERROR,"PreProcessor: ChannelID:%u. The file %s could not be opened.", m_ChannelID, oo.str().c_str());

  m_DataProcFunc.reset(DataProcFuncFactory::GetDataProcFunc(m_SysCfg->GetIdentity()));

  //--------------------------------------------------
  // For Debug purpose
  //--------------------------------------------------
  m_PrintPreProcSeqNoAsInfo= m_SysCfg->CheckIfPrintPreProcSeqNoAsInfo();

}

PreProcessor::~PreProcessor()
{
}
