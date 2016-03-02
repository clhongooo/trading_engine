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

  m_PTask_SendMDToken = m_PTask_SendMD.GetNewTokenAndSetIntervalInSec(m_SysCfg->GetPriceFwdrToNextTierIntervalInSec());

  //--------------------------------------------------
  // Init for zmq
  //--------------------------------------------------
  {
    //  Prepare our context and socket
    m_zmqcontext.reset(new zmq::context_t(1));
    // m_zmqsocket.reset(new zmq::socket_t(*m_zmqcontext, ZMQ_REQ));
    m_zmqsocket.reset(new zmq::socket_t(*m_zmqcontext, ZMQ_PUSH));

    string sZMQMDIPPort = m_SysCfg->Get_NextTier_ZMQ_MD_IP_Port();
    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: Initializing ZMQ connection. ZMQ MD IP Port = %s", sZMQMDIPPort.c_str());
    string sConn = "tcp://"+sZMQMDIPPort;
    m_zmqsocket->connect(sConn.c_str());
  }

  //--------------------------------------------------
  for (;;)
  {
    if (m_SystemState->ChkIfThreadShouldStop()) break;

    m_ThrdHlthMon->ReportHealthy(ID_PRICEFWDR);

    //--------------------------------------------------
    YYYYMMDDHHMMSS m_ymdhms_SysTimeHKT = m_MarketData->GetSystemTimeHKT();
    if (!m_PTask_SendMD.CheckIfItIsTimeToWakeUp(m_PTask_SendMDToken,m_ymdhms_SysTimeHKT))
    {
      sleep(1);
      continue;
    }

    //--------------------------------------------------
    // Transmit latest mid quote of each symbol through zmq
    //--------------------------------------------------
    FForEach (m_sAllSym,[&](const string & symbol) {
      double dNP = NAN;
      if (!m_MarketData->GetLatestNominalPrice(symbol, dNP)) return;

      char caMD[4096];
      sprintf(caMD,"%s,%s,%f\0", m_ymdhms_SysTimeHKT.ToCashTimestampStr().c_str(), symbol.c_str(), dNP);

      zmq::message_t zmq_msg(strlen(caMD)+1);
      memcpy((void *)zmq_msg.data(), caMD, strlen(caMD));
      ((char *)zmq_msg.data())[strlen(caMD)] = '\0';
      m_zmqsocket->send(zmq_msg);

      // //--------------------------------------------------
      // // Get ack
      // //--------------------------------------------------
      // zmq::message_t zmq_reply;
      // m_zmqsocket->recv(&zmq_reply);
      // //--------------------------------------------------
    });

  }
  //--------------------------------------------------

  m_Logger->Write(Logger::NOTICE,"PriceForwarderToNextTier has ended.");
  sleep(2);

  return;
}
