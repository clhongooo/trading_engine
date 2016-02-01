#include <PortfoliosAndOrders.h>

boost::weak_ptr<PortfoliosAndOrders> PortfoliosAndOrders::m_pInstance;

boost::shared_ptr<PortfoliosAndOrders> PortfoliosAndOrders::Instance() {
  boost::shared_ptr<PortfoliosAndOrders> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new PortfoliosAndOrders());
    m_pInstance = instance;
  }
  return instance;
}


PortfoliosAndOrders::PortfoliosAndOrders() :
  m_UniqID(0),
  m_CurrentMode(SINGLE_ORDER)
{
  m_MarketData = MarketData::Instance();
  m_SysCfg     = SystemConfig::Instance();
  m_Logger     = Logger::Instance();
  m_StyCPnLHist= StyCPnLHist::Instance();

  LoadPersistedPositionFromFile();
  m_StyCPnLHist->LoadPersistedCPnLHistFromFile();
  WriteActualPortToPersistentPosToLog();

  //--------------------------------------------------
  // Init for zmq
  //--------------------------------------------------
  if (
    m_SysCfg->Get_OrderRoutingMode() == SystemConfig::ORDER_ROUTE_NEXTTIERZMQ
    ||
    m_SysCfg->Get_OrderRoutingMode() == SystemConfig::ORDER_ROUTE_OTINXTIERZMQ
    )
  {
    //  Prepare our context and socket
    m_zmqcontext.reset(new zmq::context_t(1));
    m_zmqsocket.reset(new zmq::socket_t(*m_zmqcontext, ZMQ_REQ));

    string sZMQTFIPPort = m_SysCfg->Get_NextTier_ZMQ_TF_IP_Port();
    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: Initializing ZMQ connection. ZMQ TF IP Port = %s", sZMQTFIPPort.c_str());
    string sConn = "tcp://"+sZMQTFIPPort;
    m_zmqsocket->connect(sConn.c_str());
  }
}

PortfoliosAndOrders::~PortfoliosAndOrders()
{
}

void PortfoliosAndOrders::SetTargetPortfolio(const int port_id, const map<string,long> & portfolio)
{
  boost::unique_lock<boost::shared_mutex> lock(m_target_portfolios_mutex);

  map<int, map<string,long> >::iterator targetIter = m_target_portfolios.find(port_id);

  // If the port id doesn't exist, initialize an empty target portfolio.
  if(targetIter == m_target_portfolios.end()) {
    map<string ,long> m;
    m_target_portfolios[port_id] = m;
    targetIter = m_target_portfolios.find(port_id);
  }

  // Clear rather than modify in place, because some assets may not be in
  // the new target portfolio.
  targetIter->second.clear();

  // Update the target portfolio.
  for(map<string, long>::const_iterator i = portfolio.begin(); i != portfolio.end(); i++){
    targetIter->second[i->first] = i->second;
  }

  // If working orders exist, it means executor is now establishing previous target portfolio.
  // So here interrupt the establishing process by clearing all working orders.
  if(!IsPortfolioWorkingOrderEmpty(port_id))
  {
    CancelAllWorkingOrdersInPortfolio(port_id);
  }
  // If there's no working order, new target portfolio can be established now with safety.
  else{
    lock.unlock();
    ConstructActualPortfolio(port_id);
  }
  return;
}

void PortfoliosAndOrders::GetTargetPortfolio(const int port_id, map<string,long> &mReturnMap)
{
  boost::unique_lock<boost::shared_mutex> lock(m_target_portfolios_mutex);

  map<int, map<string,long> >::iterator mm_iter = m_target_portfolios.find(port_id);

  if (mm_iter == m_target_portfolios.end()) return ;

  mReturnMap.clear();

  for (map<string,long>::iterator m_iter = mm_iter->second.begin(); m_iter != mm_iter->second.end(); ++m_iter) {
    mReturnMap[m_iter->first] = m_iter->second;
  }

  return ;
}

void PortfoliosAndOrders::ConstructActualPortfolio(const int port_id)
{
  boost::unique_lock<boost::shared_mutex> lock1(m_target_portfolios_mutex);

  // Get the current target portfolio.
  map<int, map<string,long> >::iterator it_mm_target = m_target_portfolios.find(port_id);
  if (it_mm_target == m_target_portfolios.end()) return;

  for (map<string,long>::iterator it_m_target = it_mm_target->second.begin();
       it_m_target != it_mm_target->second.end();
       it_m_target++)
  {
    const string & symbol     = it_m_target->first;
    const long tgt_signed_qty = it_m_target->second;
    const long act_signed_qty = AcctCheckPos(port_id, symbol);

    if (tgt_signed_qty != act_signed_qty)
      Trade(port_id, symbol, tgt_signed_qty - act_signed_qty);
  }

  return ;
}

bool PortfoliosAndOrders::TradeUltimate(const int port_id, const string & symbol, const long signed_qty, const OrderInstruction::ExecStrategy exec_strat, const vector<double> & exec_strat_param, const bool gen_new_oid, const string & oid_details)
{
  YYYYMMDDHHMMSS SystemTime = m_MarketData->GetSystemTimeHKT();
  m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: %s: Trade: %s Qty: %d ExecStrat: %d ExecStrat param: %d. OID Details: %s.", SystemTime.ToStr().c_str(), symbol.c_str(), signed_qty, exec_strat, exec_strat_param.size(), oid_details.c_str());

  if (m_SysCfg->Get_OrderRoutingMode() == SystemConfig::ORDER_ROUTE_RECORD
      ||
      m_SysCfg->Get_OrderRoutingMode() == SystemConfig::ORDER_ROUTE_NEXTTIERZMQ)
  {
    double dMidQuote = NAN;
    YYYYMMDDHHMMSS ymdhms;
    if (!m_MarketData->GetLatestMidQuote(symbol, dMidQuote, ymdhms))
    {
      if (
        !m_SysCfg->ChkIfProceedStyForceExcnEvenIfNoMD(symbol)
        &&
        !m_SysCfg->ChkIfProceedStyForceExcnEvenIfNoMD(ContFut::GetContFutFrRglrSym(symbol,1))
        )
      {
        m_Logger->Write(Logger::ERROR,"PortfoliosAndOrders: %s: Trade: %s Qty: %d. Trade is not executed because the mid quote is unavailable.",
                        m_MarketData->GetSystemTimeHKT().ToStr().c_str(), symbol.c_str(), signed_qty);
        return false;
      }
    }

    if (std::isnan(dMidQuote))
    {
      vector<YYYYMMDD> vYMD;
      vector<double>   vOpen;
      vector<double>   vHigh;
      vector<double>   vLow;
      vector<double>   vClose;
      vector<long>     vVol;

      if (m_MarketData->ChkAvbyOfSuppD1BarOHLCV(symbol))
      {
        m_MarketData->GetSuppD1BarOHLCVInDateRange(
          symbol,
          YYYYMMDD(19000101),
          SystemTime.GetYYYYMMDD(),
          vYMD,
          vOpen,
          vHigh,
          vLow,
          vClose,
          vVol);
      }
      else
      {
        string sContFut = ContFut::GetContFutFrRglrSym(symbol,1);

        if (m_MarketData->ChkAvbyOfSuppD1BarOHLCV(sContFut))

          m_MarketData->GetSuppD1BarOHLCVInDateRange(
            sContFut,
            YYYYMMDD(19000101),
            SystemTime.GetYYYYMMDD(),
            vYMD,
            vOpen,
            vHigh,
            vLow,
            vClose,
            vVol);
      }

      if (vClose.empty())
      {
        m_Logger->Write(Logger::ERROR,"PortfoliosAndOrders: %s: Trade: %s Qty: %d. Trade is not executed because the supp day bar is unavailable.",
                        m_MarketData->GetSystemTimeHKT().ToStr().c_str(), symbol.c_str(), signed_qty);
        return false;
      }
      dMidQuote = vClose.back();
    }

    UpdateTargetPortfolio(port_id,symbol,signed_qty,'a');
    AcctTrade(port_id,symbol,signed_qty,dMidQuote);

    string sTF = ConstructAugmentedTradeFeed(port_id,symbol,dMidQuote,signed_qty,"OID","TID");
    m_Logger->Write(Logger::INFO,"%s", sTF.c_str());

    if (m_SysCfg->Get_OrderRoutingMode() == SystemConfig::ORDER_ROUTE_NEXTTIERZMQ)
    {
      SendTFToNextTierThruZMQ(port_id, symbol, dMidQuote, signed_qty,"OID","TID");
    }

    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: %s: Trade: %s Qty: %d Price: %f.",
                    m_MarketData->GetSystemTimeHKT().ToStr().c_str(), symbol.c_str(), signed_qty, dMidQuote);

    return true;
  }

  // Get the limit time.
  int i_LimitTimeInMillisec = 0;
  if (exec_strat != OrderInstruction::MARKET &&
      !exec_strat_param.empty())
    i_LimitTimeInMillisec = exec_strat_param[0];

  HMS hmsAttnTime(SystemTime.GetHHMMSS());
  hmsAttnTime.AddSecond(i_LimitTimeInMillisec / 1000 + 1);
  OrderInstruction oi;
  if (gen_new_oid)
    oi.m_order_id          = "OID_" + SystemTime.ToCashTimestampStrNoMillisec() + "_" + oid_details + "_" + boost::lexical_cast<string>(m_UniqID++);
  else
    oi.m_order_id          = oid_details + "E";
  oi.m_time_stamp          = SystemTime.ToCashTimestampStr();
  oi.m_symbol              = symbol;
  oi.m_portfolio_id        = port_id;
  oi.m_signed_qty          = signed_qty;
  oi.m_order_exec_state    = OrderInstruction::PENDING_ADD_BEF_SF;
  oi.m_exec_strategy       = exec_strat;
  oi.m_exec_strategy_param = exec_strat_param;
  oi.m_resend              = true;
  oi.m_create_time         = SystemTime.GetHHMMSS();
  oi.m_attention_time      = hmsAttnTime.ToHHMMSS();

  {
    boost::unique_lock<boost::shared_mutex> lock(m_orders_mutex);

    // Add the working orders to m_working_orders.
    m_working_orders[oi.m_order_id] = oi;
  }

  m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: Added to working orders. m_order_id [%s] m_time_stamp [%s] m_symbol [%s] m_portfolio_id [%d] m_signed_qty [%d] m_order_exec_state [%d] m_exec_strategy [%d] m_exec_strategy_param (size) [%d] m_resend [%d] m_create_time [%s] m_attention_time [%s]",
                  oi.m_order_id.c_str(),
                  oi.m_time_stamp.c_str(),
                  oi.m_symbol.c_str(),
                  oi.m_portfolio_id,
                  oi.m_signed_qty,
                  oi.m_order_exec_state,
                  oi.m_exec_strategy,
                  oi.m_exec_strategy_param.size(),
                  oi.m_resend,
                  oi.m_create_time.ToStr().c_str(),
                  oi.m_attention_time.ToStr().c_str()
                 );

  NotifyConsumers();
  return true;
}

bool PortfoliosAndOrders::DetermineExecStrategy(const string & symbol, OrderInstruction::ExecStrategy & exec_strat, vector<double> & exec_strat_param)
{
  //--------------------------------------------------
  // Just bogus code for now
  // TODO: replace with real logic for determining execution strategy later
  //--------------------------------------------------
  if (
    symbol.length() == 5
    &&
    (symbol.substr(0,3) == UNDERLYING_HSI || symbol.substr(0,3) == UNDERLYING_HHI)
    )
  {
    //--------------------------------------------------
    // Assume futures contract
    //--------------------------------------------------
    // exec_strat = OrderInstruction::LIMIT_AT_BBO_THEN_MARKET;

    // exec_strat = OrderInstruction::ALWAYS_LIMIT_AT_MID_QUOTE;
    // exec_strat = OrderInstruction::LIMIT_AT_MIDQUOTE_THEN_MARKET;
    exec_strat = OrderInstruction::MARKET;

    exec_strat_param.clear();
    exec_strat_param.push_back(5000);
    return true;
  }
  else if (
    symbol.length() == 5
    &&
    (symbol.substr(0,3) == UNDERLYING_MHI || symbol.substr(0,3) == UNDERLYING_MCH)
    )
  {
    //--------------------------------------------------
    // Assume futures contract
    //--------------------------------------------------
    // exec_strat = OrderInstruction::LIMIT_AT_MIDQUOTE_THEN_MARKET;

    // exec_strat = OrderInstruction::ALWAYS_LIMIT_AT_MID_QUOTE;
    // exec_strat = OrderInstruction::LIMIT_AT_BBO_THEN_MARKET;
    exec_strat = OrderInstruction::MARKET;

    exec_strat_param.clear();
    exec_strat_param.push_back(5000);
    return true;
  }
  // else
  // {
  //   //--------------------------------------------------
  //   // Assume HSI options contract
  //   //--------------------------------------------------
  //   double dBestBid  = 0;
  //   double dBestAsk  = 0;
  //   double dMidQuote = 0;
  //   YYYYMMDDHHMMSS ymdhms;
  //   if (m_MarketData->GetLatestBestBidAskMid(symbol,dBestBid,dBestAsk,dMidQuote,ymdhms))
  //   {
  //     if (dMidQuote > 100)
  //     {
  //       exec_strat = OrderInstruction::MARKET;
  //       exec_strat_param.clear();
  //       exec_strat_param.push_back(5000);
  //     }
  //     else if (dMidQuote > 50)
  //     {
  //       exec_strat = OrderInstruction::LIMIT_AT_MIDQUOTE_THEN_MARKET;
  //       exec_strat_param.clear();
  //       exec_strat_param.push_back(5000);
  //     }
  //     else
  //     {
  //       exec_strat = OrderInstruction::LIMIT_AT_BBO_THEN_MARKET;
  //       exec_strat_param.clear();
  //       exec_strat_param.push_back(5000);
  //     }
  //     return true;
  //   }
  //   else
  //   {
  //     //--------------------------------------------------
  //     // No price for now.. do nothing until later
  //     //--------------------------------------------------
  //     return false;
  //   }
  // }
  else
  {
    exec_strat = OrderInstruction::MARKET;
    exec_strat_param.clear();
    exec_strat_param.push_back(5000);
    return true;
  }

  return false;
}


bool PortfoliosAndOrders::Trade(const int port_id, const string & symbol, const long signed_qty)
{
  return Trade(port_id, symbol, signed_qty, true, "");
}

bool PortfoliosAndOrders::Trade(const int port_id, const string & symbol, const long signed_qty, const bool gen_new_oid, const string & oid_details)
{
  OrderInstruction::ExecStrategy exec_strat;
  vector<double> exec_strat_param;

  if (!DetermineExecStrategy(symbol,exec_strat,exec_strat_param))
  {
    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: DetermineExecStrategy failed. PortID: [%d] Symbol [%s] Signed Qty [%d]", port_id, symbol.c_str(), signed_qty);
    return false;
  }

  return TradeUltimate(port_id, symbol, signed_qty, exec_strat, exec_strat_param, gen_new_oid, oid_details);
}


void PortfoliosAndOrders::WaitForData()
{
  boost::mutex::scoped_lock lock(m_ObserverMutex);
  m_cvOrdersAvb.wait(lock); //blocks and returns mutex
  return;
}

void PortfoliosAndOrders::NotifyConsumers()
{
  m_cvOrdersAvb.notify_all();
}

bool PortfoliosAndOrders::GetActualPortfolio(const int port_id, map<string,long> & mReturnMap)
{
  boost::unique_lock<boost::shared_mutex> lock(m_acct_mutex);
  map<int,Acct>::iterator it = m_acct.find(port_id);
  if (it == m_acct.end()) return false;

  mReturnMap.clear();
  const map<string,long> & ac_port = it->second.GetPortfolio();
  for (map<string,long>::const_iterator it2 = ac_port.begin(); it2 != ac_port.end(); ++it2)
  {
    mReturnMap[it2->first] = it2->second;
  }
  return true;
}


bool PortfoliosAndOrders::GetAndEraseWorkingOrder(OrderInstruction & oi)
{
  boost::unique_lock<boost::shared_mutex> lock(m_orders_mutex);
  if (!m_working_orders.empty())
  {
    map<string,OrderInstruction>::iterator it = m_working_orders.begin();

    oi.m_order_id             = it->second.m_order_id;
    oi.m_symbol               = it->second.m_symbol;
    oi.m_signed_qty           = it->second.m_signed_qty;
    oi.m_order_exec_state     = it->second.m_order_exec_state;
    oi.m_exec_strategy        = it->second.m_exec_strategy;
    oi.m_exec_strategy_param  = it->second.m_exec_strategy_param;

    m_working_orders.erase(it);
    return true;
  }
  else
  {
    return false;
  }
}

bool PortfoliosAndOrders::Get_MTM_CPnL(const int port_id, double & mtm_cpnl)
{
  boost::unique_lock<boost::shared_mutex> lock(m_acct_mutex);

  map<int,Acct>::iterator it = m_acct.find(port_id);
  if (it == m_acct.end())
  {
    mtm_cpnl = 0;
    return true;
  }

  return it->second.Get_MTM_CPnL(mtm_cpnl);
}

double PortfoliosAndOrders::GetCPnL(const int port_id)
{
  boost::unique_lock<boost::shared_mutex> lock(m_acct_mutex);

  map<int,Acct>::iterator it = m_acct.find(port_id);
  if (it == m_acct.end()) return 0;

  return it->second.CPnL();
}

void PortfoliosAndOrders::ProvidePriceForMTM(const int port_id, const string & symbol, const double price)
{
  boost::unique_lock<boost::shared_mutex> lock(m_acct_mutex);

  map<int,Acct>::iterator it = m_acct.find(port_id);
  if (it == m_acct.end()) return;

  it->second.ProvidePriceForMTM(symbol,price);
  return;
}

set<string> PortfoliosAndOrders::GetSymbolsWithPos(const int port_id)
{
  boost::shared_lock<boost::shared_mutex> lock(m_acct_mutex);

  map<int,Acct>::iterator it = m_acct.find(port_id);
  if (it == m_acct.end()) return set<string>();

  return it->second.GetSymbolsWithPos();
}

void AttentionTime::Add(const HHMMSS & hmsAttnTime,const string & oid)
{
  boost::unique_lock<boost::shared_mutex> lock(m_attention_time_mutex);
  m_map_attention_time[hmsAttnTime].insert(oid);
  return;
}


void PortfoliosAndOrders::ConstructSignalFeedsAndChgOrderState(vector<string>& vStrSF)
{
  vStrSF.clear();

  vector<ATU_OTI_signalfeed_struct> vSF;
  ConstructSignalFeedsAndChgOrderState(vSF);


  for (vector<ATU_OTI_signalfeed_struct>::iterator it = vSF.begin(); it != vSF.end(); ++it)
  {
    ostringstream oo;

    oo <<
      std::setprecision(NUMOFDECIMALPLACEINPRICE) <<
      it->m_timestamp        <<  ",signalfeed," <<
      it->m_market           <<  "," <<
      it->m_feedcode         <<  "," <<
      it->m_order_id         <<  "," <<
      it->m_price            <<  "," <<
      it->m_qty              <<  "," <<
      it->m_open_or_close    <<  "," <<
      it->m_buy_or_sell      <<  "," <<
      it->m_order_action     <<  "," <<
      it->m_order_type       <<  "," <<
      it->m_order_validity   <<  "\n";

    vStrSF.push_back(oo.str());
  }

}

void PortfoliosAndOrders::ConstructSignalFeedsAndChgOrderState(vector<ATU_OTI_signalfeed_struct>& signalfeeds)
{
  boost::unique_lock<boost::shared_mutex> lock(m_orders_mutex);

  // iterate the working orders set and construct signalfeeds.
  for (map<string,OrderInstruction>::iterator it_wkg_ord = m_working_orders.begin(); it_wkg_ord != m_working_orders.end(); it_wkg_ord++)
  {
    ATU_OTI_signalfeed_struct strctSigFeed;

    if (it_wkg_ord->second.m_order_exec_state == OrderInstruction::PENDING_ADD_BEF_SF)
    {
      if (
        it_wkg_ord->second.m_exec_strategy == OrderInstruction::LIMIT_AT_MIDQUOTE_THEN_MARKET ||
        it_wkg_ord->second.m_exec_strategy == OrderInstruction::ALWAYS_LIMIT_AT_MID_QUOTE     ||
        it_wkg_ord->second.m_exec_strategy == OrderInstruction::LIMIT_AT_BBO_THEN_MARKET
        )
      {
        LimitOrderConstructor(it_wkg_ord->second, strctSigFeed);
        m_attn_time.Add(it_wkg_ord->second.m_attention_time,it_wkg_ord->second.m_order_id);
      }
      else if (it_wkg_ord->second.m_exec_strategy == OrderInstruction::MARKET)
      {
        MarketOrderConstructor(it_wkg_ord->second, strctSigFeed);
      }
      //--------------------------------------------------
      // Other order types are not handled
      //--------------------------------------------------

      it_wkg_ord->second.m_order_exec_state = OrderInstruction::PENDING_ADD_AFT_SF;
      signalfeeds.push_back(strctSigFeed);
    }
    else if (it_wkg_ord->second.m_order_exec_state == OrderInstruction::PENDING_CANCEL_BEF_SF)
    {
      CancelOrderConstructor(it_wkg_ord->second, strctSigFeed);
      it_wkg_ord->second.m_order_exec_state = OrderInstruction::PENDING_CANCEL_AFT_SF;
      signalfeeds.push_back(strctSigFeed);
    }

  }

  return;
}

void PortfoliosAndOrders::MarketOrderConstructor(const OrderInstruction oi, ATU_OTI_signalfeed_struct& strctMktOrder)
{
  double dBestBid, dBestAsk, dMidQuote;
  YYYYMMDDHHMMSS ymdhms;
  m_MarketData->GetLatestBestBidAskMid(oi.m_symbol, dBestBid, dBestAsk, dMidQuote, ymdhms);

  strctMktOrder.m_timestamp      = oi.m_time_stamp;
  strctMktOrder.m_market         = "HKIF";
  strctMktOrder.m_feedcode       = oi.m_symbol;
  strctMktOrder.m_order_id       = oi.m_order_id;
  strctMktOrder.m_price          = (oi.m_signed_qty > 0) ? dBestAsk * 1.02 : dBestBid * 0.98;
  strctMktOrder.m_qty            = fabs(oi.m_signed_qty);
  strctMktOrder.m_open_or_close  = "open";
  strctMktOrder.m_buy_or_sell    = (oi.m_signed_qty > 0) ? 1 : 2;
  strctMktOrder.m_order_action   = "insert";
  strctMktOrder.m_order_type     = "market_order";
  strctMktOrder.m_order_validity = "today";

  return ;
}

void PortfoliosAndOrders::LimitOrderConstructor(const OrderInstruction oi, ATU_OTI_signalfeed_struct& strctLimitOrder)
{
  // Get the limit price.
  vector<string> orderTimestamp;
  boost::split(orderTimestamp, oi.m_time_stamp, is_any_of("_"));

  YYYYMMDDHHMMSS ymdhms(boost::lexical_cast<int>(orderTimestamp[0]), boost::lexical_cast<int>(orderTimestamp[1]));

  double dBestBid, dBestAsk, dMidQuote;
  m_MarketData->GetLatestBestBidAskMid(oi.m_symbol, dBestBid, dBestAsk, dMidQuote, ymdhms);
  //

  strctLimitOrder.m_timestamp      = oi.m_time_stamp;
  strctLimitOrder.m_market         = "HKIF";
  strctLimitOrder.m_feedcode       = oi.m_symbol;
  strctLimitOrder.m_order_id       = oi.m_order_id;
  strctLimitOrder.m_qty            = fabs(oi.m_signed_qty);
  strctLimitOrder.m_open_or_close  = "open";
  strctLimitOrder.m_buy_or_sell    = (oi.m_signed_qty > 0) ? 1 : 2;
  strctLimitOrder.m_order_action   = "insert";
  strctLimitOrder.m_order_type     = "limit_order";
  strctLimitOrder.m_order_validity = "today";

  if(oi.m_exec_strategy != OrderInstruction::LIMIT_AT_BBO_THEN_MARKET)
  {
    // Limit price at mid quote.
    strctLimitOrder.m_price = dMidQuote;
  }
  else
  {
    // Limit price at BBO.
    strctLimitOrder.m_price = (oi.m_signed_qty > 0) ? dBestBid : dBestAsk;
  }

  return ;
}

bool PortfoliosAndOrders::CancelWorkingOrder(const string order_id, const bool isResend)
{
  boost::unique_lock<boost::shared_mutex> lock(m_orders_mutex);

  // No mutex, since this method is always called by other methods which are holding mutex.
  map<string, OrderInstruction>::iterator it_wkg_ord = m_working_orders.find(order_id);

  if (it_wkg_ord == m_working_orders.end()) return false;

  it_wkg_ord->second.m_time_stamp = m_MarketData->GetSystemTimeHKT().ToCashTimestampStr();
  it_wkg_ord->second.m_order_exec_state = OrderInstruction::PENDING_CANCEL_BEF_SF;
  // Distinguish the reason for this cancel operation,
  // an over-time cancel or a order-clearing cancel
  it_wkg_ord->second.m_resend = isResend;

  NotifyConsumers();

  return true;
}

void PortfoliosAndOrders::CancelOrderConstructor(const OrderInstruction oi, ATU_OTI_signalfeed_struct& strctCancelOrder)
{
  double dBestBid, dBestAsk, dMidQuote;
  YYYYMMDDHHMMSS ymdhms;
  m_MarketData->GetLatestBestBidAskMid(oi.m_symbol, dBestBid, dBestAsk, dMidQuote, ymdhms);

  strctCancelOrder.m_timestamp      = oi.m_time_stamp;
  strctCancelOrder.m_market         = "HKIF";
  strctCancelOrder.m_feedcode       = oi.m_symbol;
  strctCancelOrder.m_order_id       = oi.m_order_id;
  strctCancelOrder.m_price          = dMidQuote;
  strctCancelOrder.m_qty            = fabs(oi.m_signed_qty);
  strctCancelOrder.m_open_or_close  = "";
  strctCancelOrder.m_buy_or_sell    = (oi.m_signed_qty > 0) ? 1 : 2;
  strctCancelOrder.m_order_action   = "delete";
  strctCancelOrder.m_order_type     = "limit_order";
  strctCancelOrder.m_order_validity = "today";

  return ;
}

bool PortfoliosAndOrders::UpdateOrderState(const string order_id, const OrderInstruction::OrderExecState exec_state)
{
  boost::unique_lock<boost::shared_mutex> lock(m_orders_mutex);

  map<string,OrderInstruction>::iterator it_wkg_ord = m_working_orders.find(order_id);

  if (it_wkg_ord == m_working_orders.end()) return false;

  //--------------------------------------------------
  // Update exec_state
  //--------------------------------------------------
  it_wkg_ord->second.m_order_exec_state = exec_state;

  if (exec_state == OrderInstruction::CANCELLED)
  {
    // Backup useful fields first.
    int                            port_id          = it_wkg_ord->second.m_portfolio_id;
    string                         symbol           = it_wkg_ord->second.m_symbol;
    long                           qty              = it_wkg_ord->second.m_signed_qty;
    OrderInstruction::ExecStrategy exec_strat       = it_wkg_ord->second.m_exec_strategy;
    vector<double>                 exec_strat_param = it_wkg_ord->second.m_exec_strategy_param;

    // Remove the cancelled order and add it to completed orders set.
    m_completed_orders[it_wkg_ord->first] = it_wkg_ord->second;
    m_working_orders.erase(it_wkg_ord);

    lock.unlock();

    // Conditions should be satisfied to adjust portfolio.
    // 1. This cancel order doesn't need to be re-sent;
    // 2. This cancel order is the last order to be cancelled for this portfolio;
    // 3. Executor is at Portfolio mode.
    if (!it_wkg_ord->second.m_resend && m_CurrentMode == PORTFOLIO && IsPortfolioWorkingOrderEmpty(port_id))
    {
      ConstructActualPortfolio(port_id);
      // If not re-send, it means this cancel order is put by a target portfolio update.
      return true;
    }

    // The executor is at Single_order mode. This means this cancelled order is cancelled for over-time reason.
    // It needs to be re-sent.
    if (exec_strat == OrderInstruction::LIMIT_AT_MIDQUOTE_THEN_MARKET ||
        exec_strat == OrderInstruction::LIMIT_AT_BBO_THEN_MARKET)
    {
      // Trade(port_id, symbol, qty, OrderInstruction::MARKET,vector<double>(),"OriOID_"+order_id);
      TradeUltimate(port_id, symbol, qty, OrderInstruction::MARKET,vector<double>(),false,order_id);
    }
    else if (exec_strat == OrderInstruction::ALWAYS_LIMIT_AT_MID_QUOTE)
    {
      // Trade(port_id, symbol, qty, OrderInstruction::ALWAYS_LIMIT_AT_MID_QUOTE, exec_strat_param,"OriOID_"+order_id);
      TradeUltimate(port_id, symbol, qty, OrderInstruction::ALWAYS_LIMIT_AT_MID_QUOTE, exec_strat_param,false,order_id);
    }
  }
  else if (exec_state == OrderInstruction::FILLED)
  {
    // New exec_state is not for cancel operations.
    // This order is a limit order. Besides signing the order as completed, executor should also
    // remove it from the attention time map.
    // if (it_wkg_ord->second.m_exec_strategy == OrderInstruction::LIMIT_AT_MIDQUOTE_THEN_MARKET ||
    //     it_wkg_ord->second.m_exec_strategy == OrderInstruction::LIMIT_AT_BBO_THEN_MARKET      ||
    //     it_wkg_ord->second.m_exec_strategy == OrderInstruction::ALWAYS_LIMIT_AT_MID_QUOTE)
    {
      m_attn_time.RemoveOrderIDFromAttentionTimeMap(it_wkg_ord->second.m_attention_time, order_id);
    }

    // Remove the cancelled order and add it to completed orders set.
    m_completed_orders[it_wkg_ord->first] = it_wkg_ord->second;
    m_working_orders.erase(it_wkg_ord);

    lock.unlock();

    // If executor is at Portfolio mode, the working orders set can be emptified when trades happens.
    // So adjustment in actual portfolio is also necessary.
    long port_id = it_wkg_ord->second.m_portfolio_id;
    if (m_CurrentMode == PORTFOLIO && IsPortfolioWorkingOrderEmpty(port_id))
      ConstructActualPortfolio(port_id);

  }
  else if (exec_state == OrderInstruction::ERROR)
  {
    //--------------------------------------------------
    // The order was somehow cancelled
    // Try to reproduce the previous state
    //--------------------------------------------------
  }

  return true;
}

bool PortfoliosAndOrders::BookTradeFromOTI(const string & order_id, const string & symbol, const double price, const long signed_qty, const string & oid, const string & tid)
{

  //--------------------------------------------------
  // try to search the order_id in working orders and completed orders
  //--------------------------------------------------
  boost::unique_lock<boost::shared_mutex> lock(m_orders_mutex);

  int iPortID = -1;
  string sSymbol = "";

  map<string, OrderInstruction>::iterator it_wkg_ord = m_working_orders.find(order_id);
  if (it_wkg_ord != m_working_orders.end())
  {
    iPortID = it_wkg_ord->second.m_portfolio_id;
    sSymbol = it_wkg_ord->second.m_symbol;
  }
  else
  {
    map<string, OrderInstruction>::iterator it_comp_ord = m_completed_orders.find(order_id);
    if (it_comp_ord != m_completed_orders.end())
    {
      iPortID = it_comp_ord->second.m_portfolio_id;
      sSymbol = it_comp_ord->second.m_symbol;
    }
  }
  if (iPortID == -1 || sSymbol == "") return false;
  //--------------------------------------------------

  it_wkg_ord->second.m_signed_qty -= signed_qty;

  if (it_wkg_ord->second.m_signed_qty == 0)
  {
    //--------------------------------------------------
    // FIXME: Ugly code by XN.
    //--------------------------------------------------
    // Attention!!! Since UpdateOrderState also require the lock, so before calling the UpdateOrderState,
    // you should first unlock, or the UpdateOrderState will keep on waiting for the lock, thus deadlock happens
    lock.unlock();
    UpdateOrderState(order_id, OrderInstruction::FILLED);
  }

  //--------------------------------------------------
  AcctTrade(iPortID, sSymbol, signed_qty, price);
  //--------------------------------------------------

  //--------------------------------------------------
  // report the trade to next tier through zmq
  //--------------------------------------------------
  if (m_SysCfg->Get_OrderRoutingMode() == SystemConfig::ORDER_ROUTE_OTINXTIERZMQ)
  {
    SendTFToNextTierThruZMQ(iPortID, sSymbol, price, signed_qty, oid, tid);
  }

  return true;
}

int PortfoliosAndOrders::GetUnprocessedWorkingOrdersCount()
{
  boost::shared_lock<boost::shared_mutex> lock(m_orders_mutex);

  int iUnprocOrdCnt = 0;
  for (map<string,OrderInstruction>::iterator it = m_working_orders.begin(); it != m_working_orders.end(); ++it)
  {

    if (
      (it->second.m_order_exec_state == OrderInstruction::PENDING_ADD_BEF_SF   ) ||
      (it->second.m_order_exec_state == OrderInstruction::PENDING_CANCEL_BEF_SF)
      )
    {
      iUnprocOrdCnt++;
    }

  }
  return iUnprocOrdCnt;
}

void PortfoliosAndOrders::SwitchMode(const Mode m)
{
  m_CurrentMode = m;
}

bool PortfoliosAndOrders::IsPortfolioWorkingOrderEmpty(const long port_id)
{
  boost::shared_lock<boost::shared_mutex> lock(m_orders_mutex);

  for (map<string, OrderInstruction>::iterator it = m_working_orders.begin(); it != m_working_orders.end(); it++){
    if (it->second.m_portfolio_id == port_id)
      return false;
  }

  return true;
}

void PortfoliosAndOrders::CheckAndReplaceOrders(const HHMMSS & curTime)
{
  deque<HHMMSS> dq_hms_AttnTime;
  deque<string> dq_s_OIDToCancel;

  int iNum = m_attn_time.GetWorkingOrdersPastAttentionTime(curTime,dq_hms_AttnTime,dq_s_OIDToCancel);

  for (unsigned int i = 0; i < iNum; ++i)
  {
    CancelWorkingOrder(dq_s_OIDToCancel[i], true);
    m_attn_time.RemoveOrderIDFromAttentionTimeMap(dq_hms_AttnTime[i], dq_s_OIDToCancel[i]);
  }
  m_attn_time.CleanAttentionTimeMap(curTime);

  return;
}

int AttentionTime::GetWorkingOrdersPastAttentionTime(const HHMMSS & curTime, deque<HHMMSS> & dq_hms_AttnTime, deque<string> & dq_s_OIDToCancel)
{
  boost::shared_lock<boost::shared_mutex> lock(m_attention_time_mutex);

  dq_hms_AttnTime.clear();
  dq_s_OIDToCancel.clear();

  map<HHMMSS,set<string> >::iterator it_attn_time = m_map_attention_time.begin();

  while (it_attn_time != m_map_attention_time.end())
  {
    if (it_attn_time->first < curTime)
    {
      for (set<string>::iterator it_order_id = it_attn_time->second.begin(); it_order_id != it_attn_time->second.end(); it_order_id++)
      {
        dq_hms_AttnTime.push_back(it_attn_time->first);
        dq_s_OIDToCancel.push_back(*it_order_id);
      }
      it_attn_time++;
    }
    else break;
  }

  return dq_s_OIDToCancel.size();
}

bool AttentionTime::RemoveOrderIDFromAttentionTimeMap(const HHMMSS & attentionTime, const string & order_id)
{
  boost::unique_lock<boost::shared_mutex> lock(m_attention_time_mutex);
  int no_of_elem_erased = m_map_attention_time[attentionTime].erase(order_id);
  return (no_of_elem_erased == 1);
}

void AttentionTime::CleanAttentionTimeMap(const HHMMSS & curTime)
{
  boost::unique_lock<boost::shared_mutex> lock(m_attention_time_mutex);

  map<HHMMSS,set<string> >::iterator it = m_map_attention_time.begin();
  while (it != m_map_attention_time.end())
  {
    if (it->second.empty() && it->first < curTime)
    {
      m_map_attention_time.erase(it->first);
      it = m_map_attention_time.begin();
    }
    else
    {
      ++it;
    }
  }
  return;
}

bool PortfoliosAndOrders::IsOrderPendingCancelAftSF(const string order_id)
{
  boost::shared_lock<boost::shared_mutex> lock(m_orders_mutex);

  map<string,OrderInstruction>::iterator it = m_working_orders.find(order_id);

  if (it == m_working_orders.end()) return false;

  return it->second.m_order_exec_state == OrderInstruction::PENDING_CANCEL_AFT_SF;
}

bool PortfoliosAndOrders::IsOrderPendingAddAftSF(const string order_id)
{
  boost::shared_lock<boost::shared_mutex> lock(m_orders_mutex);

  map<string,OrderInstruction>::iterator it = m_working_orders.find(order_id);

  if (it == m_working_orders.end()) return false;

  return it->second.m_order_exec_state == OrderInstruction::PENDING_ADD_AFT_SF;
}

void PortfoliosAndOrders::CancelAllWorkingOrdersInPortfolio(const int port_id)
{
  boost::unique_lock<boost::shared_mutex> lock(m_orders_mutex);

  bool bNeedsToCancel = false;

  for (map<string, OrderInstruction>::iterator it_wkg_ord = m_working_orders.begin(); it_wkg_ord != m_working_orders.end(); it_wkg_ord++)
  {
    if (it_wkg_ord->second.m_portfolio_id     == port_id &&
        it_wkg_ord->second.m_order_exec_state != OrderInstruction::PENDING_CANCEL_BEF_SF &&
        it_wkg_ord->second.m_order_exec_state != OrderInstruction::PENDING_CANCEL_AFT_SF
       )
    {
      bNeedsToCancel = true;

      it_wkg_ord->second.m_order_exec_state = OrderInstruction::PENDING_CANCEL_BEF_SF;
      it_wkg_ord->second.m_resend = false;

      // If the order to be cleared is a limit order, the record in relevant attention time map
      // should be erased too.
      // if (
      //     it_wkg_ord->second.m_exec_strategy == OrderInstruction::LIMIT_AT_MIDQUOTE_THEN_MARKET ||
      //     it_wkg_ord->second.m_exec_strategy == OrderInstruction::ALWAYS_LIMIT_AT_MID_QUOTE     ||
      //     it_wkg_ord->second.m_exec_strategy == OrderInstruction::LIMIT_AT_BBO_THEN_MARKET
      //    )
      {
        m_attn_time.RemoveOrderIDFromAttentionTimeMap(it_wkg_ord->second.m_attention_time, it_wkg_ord->second.m_order_id);
      }
    }
  }

  // Only when cancel happens, the executor should be notified.
  // Or it will cause inconsistency in orders set.
  if(bNeedsToCancel) NotifyConsumers();

  return ;
}


void PortfoliosAndOrders::WriteActualPortToPersistentPosFile()
{
  OutputActualPortToPersistentPos('f');
}

void PortfoliosAndOrders::WriteActualPortToPersistentPosToLog()
{
  OutputActualPortToPersistentPos('l');
}

void PortfoliosAndOrders::OutputActualPortToPersistentPos(const char cOperation)
{
  // Persist actual positions to file only if needed
  if (
    !m_SysCfg->PositionPersistenceIsEnabled()
    ||
    !CheckIfSomeAcctPosChgd()
    )
  {
    return;
  }

  //--------------------------------------------------
  // TODO: FIXME: Will definitely slow down the order execution by writing to file in this same thread
  //--------------------------------------------------
  boost::shared_lock<boost::shared_mutex> lock(m_acct_mutex);

  ofstream fsPosPersist(m_SysCfg->Get_PositionPersistenceFile().c_str());
  for (map<int,Acct >::iterator it = m_acct.begin(); it != m_acct.end(); ++it)
  {
    const int & port_id = it->first;

    const map<string,long> map_pos = it->second.GetPortfolio();

    for (map<string,long>::const_iterator it2 = map_pos.begin(); it2 != map_pos.end(); ++it2)
    {
      const string & symbol = it2->first;
      const long signed_qty = it2->second;
      const double signed_notional = it->second.GetSignedNotional(symbol);

      if (cOperation == 'f')
      {
        fsPosPersist << port_id << "," << symbol << "," << signed_qty << "," << signed_notional << endl;
      }
      else if (cOperation == 'l') 
      {
        m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: m_acct: port_id = %d symbol = %s signed_qty = %d signed_notional = %f",
                        port_id,
                        symbol.c_str(),
                        signed_qty,
                        signed_notional);
      }
    }
  }

  if (cOperation == 'f')
  {
    fsPosPersist.close();
    AcctClearPosChgFlagNoLock();
  }
  return;
}


double PortfoliosAndOrders::GetActualSignedPosition(const int portid, const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_acct_mutex);
  map<int,Acct>::iterator it = m_acct.find(portid);
  if (it == m_acct.end())
  {
    if (!m_SysCfg->PositionPersistenceIsEnabled()) return 0;
    else return NAN; // deliberately return shit to raise alarm
  }
  else return it->second.Pos(symbol);
}

void PortfoliosAndOrders::UpdateTargetPortfolio(const int port_id, const string & symbol, const long signed_qty, const char add_or_replace)
{
  boost::unique_lock<boost::shared_mutex> lock(m_target_portfolios_mutex);
  map<int, map<string,long> >::iterator it = m_target_portfolios.find(port_id);
  if (it == m_target_portfolios.end())
  {
    m_target_portfolios[port_id] = map<string,long>();
    it = m_target_portfolios.find(port_id);
  }
  switch(add_or_replace)
  {
    case 'a':
      {
        it->second[symbol] += signed_qty;
        break;
      }
    case 'r':
      {
        it->second[symbol] = signed_qty;
        break;
      }
    default:
      {

        break;
      }
  }
  return;
}

void PortfoliosAndOrders::LoadPersistedPositionFromFile()
{
  if (!m_SysCfg->PositionPersistenceIsEnabled()) return;

  string sPosPersistFile = m_SysCfg->Get_PositionPersistenceFile();

  deque<string> dqPosPersistFile;
  STool::ReadFile(sPosPersistFile,dqPosPersistFile);
  vector<string> vs;

  for (unsigned int i = 0; i < dqPosPersistFile.size(); ++i)
  {
    vs.clear();
    boost::split(vs, dqPosPersistFile[i], is_any_of(","));

    if (vs.size() != 4)
    {
      m_Logger->Write(Logger::ERROR,"PortfoliosAndOrders: Position persistence file is broken. Expecting format: <port_id>,<symbol>,<signed_qty>,<signed_notional>. Now exiting.");
      sleep(1);
      exit(1);
    }

    //--------------------------------------------------
    // Update internal data struct accordingly
    //--------------------------------------------------
    int      port_id          = boost::lexical_cast<int>(vs[0]);
    string & symbol           = vs[1];
    long     signed_qty       = boost::lexical_cast<long>(vs[2]);
    double   signed_notional  = boost::lexical_cast<double>(vs[3]);

    UpdateTargetPortfolio(port_id,symbol,signed_qty,'r');
    AcctSetRecord(port_id,symbol,signed_qty,signed_notional);

    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: Read from Position persistence file : %d %s %d %f",port_id,symbol.c_str(),signed_qty,signed_notional);
  }

  //--------------------------------------------------
  // the case where there is nothing in the position persistion fl
  //--------------------------------------------------
  if (dqPosPersistFile.empty())
  {
    for (int sty = STY_NIR; sty != STY_LAST; sty++)
    {
      if (m_SysCfg->IsStrategyOn(static_cast<StrategyID>(sty))) AcctSetRecord(sty,"DUMMY_SYMBOL",0,0);
    }
  }
  //--------------------------------------------------

  return;
}



void PortfoliosAndOrders::AcctSetRecord(const int port_id, const string & symbol, const long signed_qty, const double signed_notional)
{
  boost::unique_lock<boost::shared_mutex> lock(m_acct_mutex);

  map<int,Acct>::iterator it = m_acct.find(port_id);
  if (it == m_acct.end())
  {
    m_acct[port_id] = Acct();
    it = m_acct.find(port_id);
  }
  it->second.SetRecord(symbol,signed_qty,signed_notional);
}

void PortfoliosAndOrders::AcctTrade(const int port_id, const string & symbol, const long signed_qty, const double price)
{
  boost::unique_lock<boost::shared_mutex> lock(m_acct_mutex);

  map<int,Acct>::iterator it = m_acct.find(port_id);
  if (it == m_acct.end())
  {
    m_acct[port_id] = Acct();
    it = m_acct.find(port_id);
  }
  it->second.Trade(symbol,signed_qty,price);
}

void PortfoliosAndOrders::AcctClearPosChgFlagNoLock()
{
  for (map<int,Acct>::iterator it = m_acct.begin(); it != m_acct.end(); ++it)
    it->second.ClearPosChgFlag();
}

bool PortfoliosAndOrders::CheckIfSomeAcctPosChgd()
{
  boost::shared_lock<boost::shared_mutex> lock(m_acct_mutex);

  bool bSomeAcctPosChgd = false;
  for (map<int,Acct>::iterator it = m_acct.begin(); it != m_acct.end(); ++it)
  {
    if (it->second.CheckIfPosChgd()) bSomeAcctPosChgd = true;
  }
  return bSomeAcctPosChgd;
}


double PortfoliosAndOrders::AcctCheckPos(const int port_id, const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_acct_mutex);

  map<int,Acct>::iterator it = m_acct.find(port_id);

  if (it == m_acct.end())
  {
    if (!m_SysCfg->PositionPersistenceIsEnabled()) return 0;
    else return NAN; // deliberately return shit to raise alarm
  }

  return it->second.Pos(symbol);
}

string PortfoliosAndOrders::ConstructAugmentedTradeFeed(const int port_id, const string & symbol, const double price, const long signed_qty, const string & oid, const string & tid)
{
  return m_MarketData->GetSystemTimeHKT().ToCashTimestampStr() + ",tradefeed,mktid," +
    symbol + "," + oid + "," +
    boost::lexical_cast<string>(price) + "," +
    boost::lexical_cast<string>((long)abs(signed_qty)) + "," +
    boost::lexical_cast<string>(signed_qty > 0 ? 1:2) + "," + tid + ",0," +
    GetStrategyName(static_cast<StrategyID>(port_id));
}

void PortfoliosAndOrders::SendTFToNextTierThruZMQ(const int port_id, const string & symbol, const double price, const long signed_qty, const string & oid, const string & tid)
{
  //--------------------------------------------------
  // Transmit through zmq
  //--------------------------------------------------
  string sATF = ConstructAugmentedTradeFeed(port_id,symbol,price,signed_qty, oid, tid);

  zmq::message_t zmq_msg(sATF.length()+1);
  memcpy((void *)zmq_msg.data(), sATF.c_str(), sATF.length());
  ((char *)zmq_msg.data())[sATF.length()] = '\0';
  m_zmqsocket->send(zmq_msg);

  //--------------------------------------------------
  // Get ack
  //--------------------------------------------------
  zmq::message_t zmq_reply;
  m_zmqsocket->recv(&zmq_reply);

  m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: %s: Ack received through ZMQ: %s",
                  m_MarketData->GetSystemTimeHKT().ToStr().c_str(), (char *)zmq_reply.data());
  //--------------------------------------------------
}

