#include "PriceForwarderToNextTier.h"

PriceForwarderToNextTier::PriceForwarderToNextTier()
{
  m_MarketData      = MarketData::Instance();
  m_SystemState     = SystemState::Instance();
  m_SysCfg          = SystemConfig::Instance();
  m_Logger          = Logger::Instance();
  m_ThrdHlthMon     = ThreadHealthMonitor::Instance();

  for (unsigned int i = 0; i < MAX_OTIMDI; ++i)
  {
    vector<string> vs = m_SysCfg->GetMDISubscriptionSymbols(i);
    m_sAllSym.insert(vs.begin(),vs.end());
  }


}

PriceForwarderToNextTier::~PriceForwarderToNextTier()
{
}

void PriceForwarderToNextTier::Run()
{

  if (!m_SysCfg->IsPriceFwdrToNextTierOn())
  {
    m_Logger->Write(Logger::INFO,"PriceForwarderToNextTier is not configured to run. Ending thread.");
    sleep(2);
    return;
  }

  //--------------------------------------------------
  // Init for zmq
  //--------------------------------------------------
  if (m_SysCfg->Get_OrderRoutingMode() == SystemConfig::ORDER_ROUTE_NEXTTIERZMQ)
  {
    //  Prepare our context and socket
    m_zmqcontext.reset(new zmq::context_t(2));
    m_zmqsocket.reset(new zmq::socket_t(*m_zmqcontext, ZMQ_REQ));

    string sZMQIPPort = m_SysCfg->Get_NextTier_ZMQ_IP_Port();
    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: Initializing ZMQ connection. ZMQ IP Port = %s", sZMQIPPort.c_str());
    string sConn = "tcp://"+sZMQIPPort;
    m_zmqsocket->connect(sConn.c_str());
  }

  //--------------------------------------------------
  for (;;)
  {
    m_MarketData->WaitForData();

    if (m_SystemState->ChkIfThreadShouldStop()) break;

    m_ThrdHlthMon->ReportHealthy(ID_PRICEFWDR);

    //--------------------------------------------------
    // Transmit latest mid quote of each symbol through zmq
    //--------------------------------------------------
    for_each (m_sAllSym,[&](const string & symbol) {
      double dMidQuote = NAN;
      YYYYMMDDHHMMSS ymdhms;
      if (m_MarketData->GetLatestMidQuote(symbol, dMidQuote, ymdhms)) return;

      char caMD[4096];
      sprintf(caMD,"%s,%s,%f\0", m_MarketData->GetSystemTimeHKT().ToCashTimestampStr().c_str(), symbol.c_str(), dMidQuote);

      zmq::message_t zmq_msg(strlen(caMD)+1);
      memcpy((void *)zmq_msg.data(), caMD, strlen(caMD));
      ((char *)zmq_msg.data())[strlen(caMD)] = '\0';
      m_zmqsocket->send(zmq_msg);

      //--------------------------------------------------
      // Get ack
      //--------------------------------------------------
      zmq::message_t zmq_reply;
      m_zmqsocket->recv(&zmq_reply);
      //--------------------------------------------------
    });

  }
  //--------------------------------------------------

  m_Logger->Write(Logger::NOTICE,"PriceForwarderToNextTier has ended.");
  sleep(2);

  return;
}
