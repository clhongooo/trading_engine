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


  m_Logger->Write(Logger::NOTICE, "%s: ChannelID:%u.", __FILENAME__, m_ChannelID);

  if ((*(m_SysCfg->GetRefreshProcessorJson()))[m_ChannelID] == 1) m_bOutputJson = true;

  m_DataProcFunc.reset(DataProcFuncFactory::GetDataProcFunc(m_SysCfg->GetIdentity()));

  m_PrintRefreshProcSeqNoAsInfo = m_SysCfg->CheckIfPrintRefreshProcSeqNoAsInfo();
  m_PrintOrderBookAsInfo        = m_SysCfg->CheckIfPrintOrderBookAsInfo();
  m_RFMsgCirBufProtection       = m_SysCfg->GetRFMsgCirBufProtection();
}
