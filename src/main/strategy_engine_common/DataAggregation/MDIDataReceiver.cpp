#include <MDIDataReceiver.h>

MDIDataReceiver::MDIDataReceiver(const int otimdiid) :
  m_Aggregator_Identity(otimdiid)
{
  m_Logger          = Logger::Instance();
  m_MarketData      = MarketData::Instance();
  m_SystemState     = SystemState::Instance();
  m_SysCfg          = SystemConfig::Instance();
  m_MDIAck          = MDI_Acknowledgement::Instance();
  m_ThrdHlthMon     = ThreadHealthMonitor::Instance();
}

MDIDataReceiver::~MDIDataReceiver()
{
}

void MDIDataReceiver::SetMDIServer(const string & ip, const string & port)
{
  if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
  {
    m_Logger->Write(Logger::INFO,"MDIDataReceiver: Running in read file mode. No need to connect to ATU MDI %s %s", ip.c_str(), port.c_str());
  }
  else
  {
    m_Logger->Write(Logger::INFO,"MDIDataReceiver: Connecting to ATU MDI %s %s", ip.c_str(), port.c_str());
    m_MDI_Server_IP = ip;
    m_MDI_Server_Port = port;
    m_mdi_server.reset(new ATU_TCPClient(m_MDI_Server_IP,m_MDI_Server_Port));
    m_mdi_server->RegisterAfterConnectionCallBack(new ATU_TCPClientAfterConnectionCallBackFunc(boost::bind(&MDIDataReceiver::OnTCPConnect,this)));
    m_mdi_server->RegisterHandleMsgCallBack(new ATU_TCPReadCallBackFunc(boost::bind(&MDIDataReceiver::OnRecvMsgCSV,this,_1)));
    m_mdi_server->register_notify_logfeed_call_back_func(new ATU_logfeed_CallBackFunc(boost::bind(&MDIDataReceiver::OnNotifyLogfeed,this,_1)));
    m_mdi_server->detach();
  }
  return;
}


void MDIDataReceiver::ReadDataFile(const string & datafileloc)
{
  std::ifstream filein(datafileloc);

  vector<string> vsubsym = m_SysCfg->GetMDISubscriptionSymbols(m_Aggregator_Identity);

  set<string> ssubsym;
  ssubsym.insert(vsubsym.begin(),vsubsym.end());

  for (std::string line; std::getline(filein, line); )
  {
    //--------------------------------------------------
    // scrap those unsubscribed symbols
    //--------------------------------------------------
    const string sSym = STool::GetNthItemFromCSV(line,2);
    if (ssubsym.find(sSym) != ssubsym.end()) OnRecvMsgCSV(line);
  }
}


void MDIDataReceiver::OnNotifyLogfeed(ATU_logfeed_struct *s)
{
  if (s->m_logArgType!=NULL) free(s->m_logArgType);
  if (s->m_logFmt!=NULL)     free(s->m_logFmt);
  if (s->m_logData!=NULL)    free(s->m_logData);
  delete s;

  // if      (s->m_logSeverity == ATU_logfeed_struct::EMERGENCY) m_Logger->Write(Logger::EMERGENCY,"%s [%f]: %s", s->m_logSource.c_str(), s->m_timeInMS, s->m_logData);
  // else if (s->m_logSeverity == ATU_logfeed_struct::ALERT    ) m_Logger->Write(Logger::ALERT    ,"%s [%f]: %s", s->m_logSource.c_str(), s->m_timeInMS, s->m_logData);
  // else if (s->m_logSeverity == ATU_logfeed_struct::CRITICAL ) m_Logger->Write(Logger::CRITICAL ,"%s [%f]: %s", s->m_logSource.c_str(), s->m_timeInMS, s->m_logData);
  // else if (s->m_logSeverity == ATU_logfeed_struct::ERROR    ) m_Logger->Write(Logger::ERROR    ,"%s [%f]: %s", s->m_logSource.c_str(), s->m_timeInMS, s->m_logData);
  // else if (s->m_logSeverity == ATU_logfeed_struct::WARNING  ) m_Logger->Write(Logger::WARNING  ,"%s [%f]: %s", s->m_logSource.c_str(), s->m_timeInMS, s->m_logData);
  // else if (s->m_logSeverity == ATU_logfeed_struct::NOTICE   ) m_Logger->Write(Logger::NOTICE   ,"%s [%f]: %s", s->m_logSource.c_str(), s->m_timeInMS, s->m_logData);
  // else if (s->m_logSeverity == ATU_logfeed_struct::INFO     ) m_Logger->Write(Logger::INFO     ,"%s [%f]: %s", s->m_logSource.c_str(), s->m_timeInMS, s->m_logData);
  // else if (s->m_logSeverity == ATU_logfeed_struct::DEBUG    ) m_Logger->Write(Logger::DEBUG    ,"%s [%f]: %s", s->m_logSource.c_str(), s->m_timeInMS, s->m_logData);
  return;
}

void MDIDataReceiver::OnRecvMsgCSV(string strMDI)
{
  m_Logger->Write(Logger::DEBUG, "MDIDataReceiver: OnRecvMsgCSV() Received: %s", strMDI.c_str());

  if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK || m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
  {
    m_MDIAck->ReportDataArrived(m_Aggregator_Identity);
  }

  //--------------------------------------------------
  ATU_MDI_marketfeed_struct structMDI;
  if (!ATU_MDI_marketfeed_struct::ParseMDIString(strMDI,structMDI))
  {
    m_Logger->Write(Logger::DEBUG,"MarketData: Discarded marketdata with incorrect format. Received MD: %s",strMDI.c_str());

    if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK ||
        m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
    {
      m_MDIAck->ReportNotMktData();
    }
    m_Logger->Write(Logger::INFO,"MDIDataReceiver: Failed to parse string from MDI: %s", strMDI.c_str());
  }
  else
  {
    if (m_MarketData->UpdateMarketData(structMDI))
    {
      if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK || m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
        m_MDIAck->WaitForAck();
    }
  }
  //--------------------------------------------------

  if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK || m_SysCfg->Get_MDIMode() == SystemConfig::MDI_READFILE)
  {
    string s = "9394,acknowledgement,0,fromtradingengine\n";
    if (m_mdi_server) m_mdi_server->queuemsg(s);
    m_MarketData->ClearUpdatedSymbols();
  }
}

void MDIDataReceiver::Run()
{
  for (;;)
  {
    if (m_SystemState->ChkIfThreadShouldStop()) break;

    m_ThrdHlthMon->ReportThatIAmHealthy(ID_DATAAGG);

    sleep(2);
  }

  if (m_mdi_server) m_mdi_server->stop();
  m_Logger->Write(Logger::NOTICE,"MDIDataReceiver for ATU MDI (id=%d) has ended.", m_Aggregator_Identity);
  sleep(2);
  return;
}

void MDIDataReceiver::OnTCPConnect()
{
  m_Logger->Write(Logger::INFO,"MDIDataReceiver: Connected to ATU MDI (id=%d)", m_Aggregator_Identity);

  usleep(500000);

  vector<string> vsubsym = m_SysCfg->GetMDISubscriptionSymbols(m_Aggregator_Identity);
  string sstart          = m_SysCfg->GetMDISubscriptionStartDate(m_Aggregator_Identity);
  string send            = m_SysCfg->GetMDISubscriptionEndDate(m_Aggregator_Identity);
  string sreplayspd      = m_SysCfg->GetMDISubscriptionReplaySpeed(m_Aggregator_Identity);

  BatchMktFeedSubscription(vsubsym, sstart, send, sreplayspd);

  return;
}

//--------------------------------------------------
// Adapted from Xiao Nan
//--------------------------------------------------
void MDIDataReceiver::BatchMktFeedSubscription(const vector<string>& symbols, const string & begDate, const string & endDate, const string & replayspeed)
{
  m_Logger->Write(Logger::INFO, "BatchMktFeedSubscription: Start subscribing to marketfeed. (id=%d)", m_Aggregator_Identity);

  string s_time = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_000000();

  if (m_SysCfg->SendResetOnConnectionToCAPI())
  {
    string s = s_time + ",reset\n";
    m_mdi_server->queuemsg(s);
    m_Logger->Write(Logger::INFO, "BatchMktFeedSubscription: Sent: %s", s.c_str());
    sleep(1);
  }

  for (int i = 0; i < symbols.size(); i++)
  {
    string sMkt = "SMART";

    ostringstream oo;
    oo << s_time ;
    oo << ",subscription," ;
    oo << sMkt ;
    oo << "," ;
    oo << symbols[i] ;
    oo << "," ;
    oo << begDate ;
    oo << "," ;
    oo << endDate ;
    if (m_SysCfg->Get_MDIMode() == SystemConfig::MDI_TCPWITHACK)
    {
      oo << ",ack_mode=YES";
    }
    else
    {
      if (boost::lexical_cast<int>(replayspeed) > 0)
      {
        oo << ",replay_speed=" ;
        oo << replayspeed ;
      }
    }
    oo << "\n";
    string subscription = oo.str();

    m_mdi_server->queuemsg(subscription);
    m_Logger->Write(Logger::INFO, subscription.c_str());
  }

  m_Logger->Write(Logger::INFO, "BatchMktFeedSubscription: Finished subscribing to marketfeed. (id=%d)", m_Aggregator_Identity);
  return;
}

void MDIDataReceiver::BatchMktFeedSubscription(const vector<string>& symbols, const string & begDate, const string & endDate, const string & replayspeed, vector<ATU_MDI_subscription_struct> & vSub)
{
  m_Logger->Write(Logger::INFO, "BatchMktFeedSubscription: Start subscribing to marketfeed. (id=%d)", m_Aggregator_Identity);

  vSub.clear();

  ATU_MDI_subscription_struct ss;
  ss.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_000000();
  ss.m_market = "SMART";

  for (int i = 0; i < symbols.size(); i++)
  {
    ss.m_instrument  = symbols[i];
    ss.m_begindate = begDate;
    ss.m_enddate   = endDate;
    // ss.m_subscription_attribute;
    vSub.push_back(ss);
  }

  m_Logger->Write(Logger::INFO, "BatchMktFeedSubscription: Finished subscribing to marketfeed. (id=%d)", m_Aggregator_Identity);
  return;
}
