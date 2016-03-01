#include <OrderExecutor.h>

OrderExecutor::OrderExecutor()
{
  m_Logger        = Logger::Instance();
  m_MarketData    = MarketData::Instance();
  m_SystemState   = SystemState::Instance();
  m_PortAndOrders = PortfoliosAndOrders::Instance();
  m_SysCfg        = SystemConfig::Instance();
  m_ThrdHlthMon   = ThreadHealthMonitor::Instance();
}

OrderExecutor::~OrderExecutor()
{
}

void OrderExecutor::SetOTIServer(const string & ip, const string & port)
{
  if (m_SysCfg->Get_OTIMode() == SystemConfig::OTI_RECORD)
  {
    m_Logger->Write(Logger::INFO, "No need to connect to OTI because we are running in OTI record mode.");
  }
  else
  {
    m_OTI_Server_IP = ip;
    m_OTI_Server_Port = port;
    m_oti_server.reset(new ATU_TCPClient(m_OTI_Server_IP,m_OTI_Server_Port));
    m_oti_server->RegisterAfterConnectionCallBack(new ATU_TCPClientAfterConnectionCallBackFunc(boost::bind(&OrderExecutor::OnTCPConnect,this)));
    m_oti_server->RegisterHandleMsgCallBack(new ATU_TCPReadCallBackFunc(boost::bind(&OrderExecutor::OnRecvMsg,this,_1)));
    m_oti_server->register_notify_logfeed_call_back_func(new ATU_logfeed_CallBackFunc(boost::bind(&OrderExecutor::OnNotifyLogfeed,this,_1)));
    m_oti_server->detach();
  }
  return;
}

//--------------------------------------------------
// Xiao Nan
// Handle exceptions, and add filled or cancelled orders to the 'm_completed_orders'
// As for exceptions, the most common one is the failure of cancel order, over 99.9% it is because the order has been filled,
// so no need to do some further operations.
//--------------------------------------------------
void OrderExecutor::OnRecvMsg(string str)
{

  m_Logger->Write(Logger::INFO, "%s : %s : %s (sys time) Received: %s", m_OTI_Server_IP.c_str(), m_OTI_Server_Port.c_str(), m_MarketData->GetSystemTimeHKT().ToStr().c_str(), str.c_str());

  vector<string> feedMsg;
  boost::split(feedMsg, str, is_any_of(","));

  if (feedMsg.size() < 2)
  {
    return;
  }
  else if (feedMsg[1] == "orderfeed")
  {
    HandleOrderFeedRecv(feedMsg);
  }
  else if (feedMsg[1] == "tradefeed")
  {
    HandleTradeFeedRecv(feedMsg);
  }
    // For errorfeed handling ...
  // else
  // {
  // }

  return;
}

void OrderExecutor::HandleOrderFeedRecv(const vector<string> & feedMsg)
{
  //--------------------------------------------------
  // Parse order feed
  //--------------------------------------------------
  ATU_OTI_orderfeed_struct of;
  of.m_timestamp         = feedMsg[0];
  of.m_feedcode          = feedMsg[3];
  of.m_order_id          = feedMsg[4];
  of.m_price             = boost::lexical_cast<double>(feedMsg[5]);
  of.m_qty               = boost::lexical_cast<double>(feedMsg[6]);
  of.m_buy_or_sell       = boost::lexical_cast<int>(feedMsg[8]);
  of.m_qty_filled        = boost::lexical_cast<double>(feedMsg[9]);
  of.m_deleted           = boost::lexical_cast<int>(feedMsg[10]);
  of.m_order_status      = boost::lexical_cast<int>(feedMsg[11]);
  of.m_error_description = feedMsg[12];
  of.m_order_type        = feedMsg[14];

  HandleOrderFeedRecv(of);
  return;
}

void OrderExecutor::HandleOrderFeedRecv(const ATU_OTI_orderfeed_struct & of)
{
  // Cancel order signal will receive 2 orderfeeds, the first one is confirming that the cancel order is received.
  // (Completely the same as the order that is to be cancelled), the second one is for confirming that the cancel
  // operation has been done.
  // Here executor ignores the first of of cancel orders.

  if (of.m_order_status == 1) return;


  if (of.m_deleted == 1 && of.m_qty == of.m_qty_filled)
  {
    m_Logger->Write(Logger::INFO,"OrderExecutor: OrderID: %s Order status changed to FILLED", of.m_order_id.c_str());
    m_PortAndOrders->UpdateOrderState(of.m_order_id, OrderInstruction::FILLED);
  }
  else if (of.m_deleted == 1 && of.m_qty != of.m_qty_filled)
  {
    //--------------------------------------------------
    // code deleted after refactoring PortfoliosAndOrders
    //--------------------------------------------------
    // if (m_PortAndOrders->IsOrderPendingCancelAftSF(of.m_order_id))
    // {
    //   m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: OrderID: %s Order status changed to CANCELLED", of.m_order_id.c_str());
    //   m_PortAndOrders->UpdateOrderState(of.m_order_id, OrderInstruction::CANCELLED);
    // }
    // else
    // {
    m_Logger->Write(Logger::INFO,"OrderExecutor: OrderID: %s Order status changed to ERROR because it's deleted for unknown reasons.", of.m_order_id.c_str());
    m_PortAndOrders->UpdateOrderState(of.m_order_id, OrderInstruction::ERROR);
    // }
  }
  else if (of.m_qty_filled == 0 && of.m_deleted == 0)
  {
    if (m_PortAndOrders->IsOrderPendingAddAftSF(of.m_order_id))
    {
      m_Logger->Write(Logger::INFO,"OrderExecutor: OrderID: %s Order status changed to ADDED", of.m_order_id.c_str());
      m_PortAndOrders->UpdateOrderState(of.m_order_id, OrderInstruction::ADDED);
    }
    //--------------------------------------------------
    // code deleted after refactoring PortfoliosAndOrders
    //--------------------------------------------------
    // else if (m_PortAndOrders->IsOrderPendingCancelAftSF(of.m_order_id))
    // {
    //   m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: OrderID: %s delete acknowledgement received", of.m_order_id.c_str());
    // }
    // else
    // {
    // m_Logger->Write(Logger::INFO,"OrderExecutor: OrderID: %s orderfeed seems to be duplicated and is therefore ignored.", of.m_order_id.c_str());
    // }
  }

  return;
}

void OrderExecutor::HandleTradeFeedRecv(const vector<string> & feedMsg)
{
  ATU_OTI_tradefeed_struct tf;
  tf.m_feedcode             = feedMsg[3];
  tf.m_order_id             = feedMsg[4];
  tf.m_price                = boost::lexical_cast<double>(feedMsg[5]);
  tf.m_qty                  = boost::lexical_cast<double>(feedMsg[6]);
  tf.m_buy_or_sell          = boost::lexical_cast<int>(feedMsg[7]);

  HandleTradeFeedRecv(tf);

  return;
}

void OrderExecutor::HandleTradeFeedRecv(const ATU_OTI_tradefeed_struct & tf)
{
  const string & symbol     = tf.m_feedcode;
  const string & order_id   = tf.m_order_id;
  const double price        = tf.m_price;
  const long signed_qty     = ((tf.m_buy_or_sell == 1) ? 1 : -1) * boost::lexical_cast<long>(tf.m_qty);
  const string & trade_id   = tf.m_trade_id;

  m_PortAndOrders->BookTradeFromOTI(order_id, symbol, price, signed_qty, order_id, trade_id);
  return;
}

void OrderExecutor::OnNotifyLogfeed(ATU_logfeed_struct *s)
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


// Xiao Nan
void OrderExecutor::Run()
{

  if (m_SysCfg->Get_OTIMode() != SystemConfig::OTI_RECORD)
  {
    for (;;)
    {
      m_PortAndOrders->WaitForData();

      if (m_SystemState->ChkIfThreadShouldStop()) break;

      m_ThrdHlthMon->ReportHealthy(ID_ORDEREXEC);

      while (m_PortAndOrders->GetUnprocessedWorkingOrdersCount() > 0)
      {
        vector<string> v;
        m_PortAndOrders->ConstructSignalFeedsAndChgOrderState(v);

        for (int i = 0; i < v.size(); i++)
        {
          m_oti_server->queuemsg(v[i]);
          m_Logger->Write(Logger::INFO, "OrderExecutor: %s : %s : %s Sent: %s",
                          m_OTI_Server_IP.c_str(),
                          m_OTI_Server_Port.c_str(),
                          m_MarketData->GetSystemTimeHKT().ToStr().c_str(),
                          v[i].c_str());
        }
      }
    }

    m_oti_server->stop();
  }

  m_Logger->Write(Logger::NOTICE,"OrderExecutor: Order Executor has ended.");
  sleep(2);
}



// Xiao Nan
// void OrderExecutor::RunChkOrd()
// {
//   if (m_SysCfg->Get_TCPOrEmbeddedMode() != SystemConfig::EMBEDDED)
//   {
//     for (;;)
//     {
//       m_MarketData->WaitForData();
//       m_ThrdHlthMon->ReportHealthy(ID_ORDEREXECCHKORD);
//
//       // Check system time and determine whether there are orders that should be cancelled.
//       m_PortAndOrders->CheckAndReplaceOrders(m_MarketData->GetSystemTimeHKT().GetHHMMSS());
//     }
//   }
// }


void OrderExecutor::RunPersistPos()
{
  for (;;)
  {
    m_ThrdHlthMon->ReportHealthy(ID_ORDEREXEPERSISTPOS);
    m_PortAndOrders->WriteActualPortToPersistentPosFile();
    m_PortAndOrders->WriteActualPortToPersistentPosToLog();
    sleep(5);
  }
}



void OrderExecutor::OnTCPConnect()
{
  m_Logger->Write(Logger::INFO,"OrderExecutor: Connected to ATU OTI.");

  if (m_SysCfg->SendResetOnConnectionToCAPI())
  {
    m_oti_server->queuemsg("123,reset\n");
    m_Logger->Write(Logger::INFO,"OrderExecutor: Sent: 123,reset.");
  }

  return;
}


