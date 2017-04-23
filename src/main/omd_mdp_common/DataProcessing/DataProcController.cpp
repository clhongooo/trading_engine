/*
 * DataProcController.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: sunny
 */

#include "DataProcController.h"

DataProcController::DataProcController()
{
  m_SysCfg = SystemConfig::Instance();
  m_Logger = Logger::Instance();
}

DataProcController::~DataProcController()
{
}

void DataProcController::StartDataProcressing()
{
  const boost::shared_ptr<vector<McastIdentifier> > pvMcastIdentifier = m_SysCfg->GetMcastIdentifiers();
  for (int i = 0; i < pvMcastIdentifier->size(); ++i)
  {
    PreProcessor * p = PreProcessorFactory::GetPreProcessor(m_SysCfg->GetIdentity(),(*pvMcastIdentifier)[i]);
    m_PreProcessors.push_back(p);
    m_BoostThreadGrp.add_thread(new boost::thread(&PreProcessor::Run,p));

    m_Logger->Write(Logger::NOTICE, "DataProcController: Starting PreProcessor thread: ChannelID:%u. IP %s, Port %u",
                    (*pvMcastIdentifier)[i].Channel_ID(),
                    (*pvMcastIdentifier)[i].IP().c_str(),
                    (*pvMcastIdentifier)[i].Port());
  }

  const boost::shared_ptr<vector<unsigned short> > pvActiveMcastChnls = m_SysCfg->GetActiveMcastChnl();

  if (m_SysCfg->CheckIfRunRealTimeProcessor())
  {
    for (int i = 0; i < pvActiveMcastChnls->size(); ++i)
    {
      RealTimeProcessor * p = RealTimeProcessorFactory::GetRealTimeProcessor(m_SysCfg->GetIdentity(), (*pvActiveMcastChnls)[i]);
      m_RealTimeProcessors.push_back(p);
      m_BoostThreadGrp.add_thread(new boost::thread(&RealTimeProcessor::Run,p));
      m_Logger->Write(Logger::NOTICE, "DataProcController: Starting RealTimeProcessor thread: ChannelID:%u.", (*pvActiveMcastChnls)[i]);
    }
  }

  if (m_SysCfg->CheckIfRunRefreshProcessor())
  {
    for (int i = 0; i < pvActiveMcastChnls->size(); ++i)
    {
      RefreshProcessor * p = RefreshProcessorFactory::GetRefreshProcessor(m_SysCfg->GetIdentity(), (*pvActiveMcastChnls)[i]);
      m_RefreshProcessors.push_back(p);
      m_BoostThreadGrp.add_thread(new boost::thread(&RefreshProcessor::Run,p));
      m_Logger->Write(Logger::NOTICE, "DataProcController: Starting RefreshProcessor thread: ChannelID:%u.", (*pvActiveMcastChnls)[i]);
    }
  }

  //block here
  if (m_SysCfg->CheckIfRunRealTimeProcessor() || m_SysCfg->CheckIfRunRefreshProcessor())
  {
    m_BoostThreadGrp.join_all();
  }

  m_Logger->Write(Logger::NOTICE, "DataProcController: All data processing threads have stopped.");

  return;
}
