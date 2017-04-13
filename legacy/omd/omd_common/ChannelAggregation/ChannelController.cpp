/*
 * ChannelController.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: sunny
 */

#include "ChannelController.h"

McastThreadCls::McastThreadCls(const McastIdentifier & id) : m_McIdentifier(id)
{}

void McastThreadCls::CallIoserviceStop()
{
  m_ios.stop();
  return;
}
void McastThreadCls::Run()
{
  try
  {
    McastReceiver mr (
        m_ios,
        boost::asio::ip::address::from_string(m_McIdentifier.IP()),
        boost::asio::ip::address::from_string(m_McIdentifier.IP()),
        m_McIdentifier.Port(),
        MAX_OMD_PACKET_SIZE,
        m_McIdentifier);

    m_ios.run(); //blocking
  }
  catch (std::exception& e)
  {
    Logger::Instance()->Write(Logger::ERROR, e.what());
  }
}

//------------------------------------------------------------
ChannelController::ChannelController()
{
  m_SysCfg = SystemConfig::Instance();
  m_Logger = Logger::Instance();
  m_ShrObj = SharedObjects::Instance();
}

ChannelController::~ChannelController()
{}

void ChannelController::StopMcastRecvr()
{
  for (unsigned int i = 0; i < m_McastThreadCls.size(); i++)
    m_McastThreadCls[i].CallIoserviceStop();    
  Logger::Instance()->Write(Logger::NOTICE, "McastReceiver io_service have stopped.");
}

void ChannelController::StartRecvMcast()
{
  //--------------------------------------------------
  // Overarching debug option for RTSClient 
  //--------------------------------------------------
  const shared_ptr<vector<McastIdentifier> > pvMcastIdentifier = m_SysCfg->GetMcastIdentifiers();

  for (int i = 0; i < pvMcastIdentifier->size(); ++i)
  {
    string s_IP = (*pvMcastIdentifier)[i].IP();
    unsigned short us_Port = (*pvMcastIdentifier)[i].Port();
    McastThreadCls* mt = new McastThreadCls((*pvMcastIdentifier)[i]);
    m_McastThreadCls.push_back(mt);
    m_McastRecvrBoostThreadGrp.add_thread(new boost::thread(&McastThreadCls::Run,mt));
    m_Logger->Write(
        Logger::NOTICE, "ChannelController: Starting Multicast receiver thread: %s: %s %u",
        (*pvMcastIdentifier)[i].ToString().c_str(),
        (*pvMcastIdentifier)[i].IP().c_str(),
        (*pvMcastIdentifier)[i].Port());
  }

  // block here
  if (!pvMcastIdentifier->empty())
    m_McastRecvrBoostThreadGrp.join_all();

  m_Logger->Write(
      Logger::NOTICE, "ChannelController: McastReceiver threads have stopped.");

  return;
}

void ChannelController::StartDataCompletenessInspectors()
{
  const shared_ptr<vector<unsigned short> > pvActiveMcastChnl = m_SysCfg->GetActiveMcastChnl();

  for (int i = 0; i < pvActiveMcastChnl->size(); ++i)
  {
    DataCompletenessInspector* dci = new DataCompletenessInspector((*pvActiveMcastChnl)[i]);
    m_DataComplInspector.push_back(dci);
    m_DataComplInspectorBoostThreadGrp.add_thread(new boost::thread(&DataCompletenessInspector::Run,dci));
    m_Logger->Write(
        Logger::NOTICE, "ChannelController: Starting DataCompletenessInspector thread: ChannelID:%u.",(*pvActiveMcastChnl)[i]);
  }

  // block here
  if (!pvActiveMcastChnl->empty())
    m_DataComplInspectorBoostThreadGrp.join_all();

  m_Logger->Write(
      Logger::NOTICE, "ChannelController: DataCompletenessInspector threads have stopped.");
  return;
}
