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
  m_UniqID(0)
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
  if (m_SysCfg->NextTierZMQIsOn())
  {
    //  Prepare our context and socket
    m_zmqcontext.reset(new zmq::context_t(1));
    // m_zmqsocket.reset(new zmq::socket_t(*m_zmqcontext, ZMQ_REQ));
    m_zmqsocket.reset(new zmq::socket_t(*m_zmqcontext, ZMQ_PUSH));

    string sZMQTFIPPort = m_SysCfg->Get_NextTier_ZMQ_TF_IP_Port();
    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: Initializing ZMQ connection. ZMQ TF IP Port = %s", sZMQTFIPPort.c_str());
    string sConn = "tcp://"+sZMQTFIPPort;
    m_zmqsocket->connect(sConn.c_str());
  }
}

PortfoliosAndOrders::~PortfoliosAndOrders()
{
}

bool PortfoliosAndOrders::TradeUltimate(const int port_id, const string & symbol, const long signed_qty, const OrderInstruction::ExecStrategy exec_strat, const vector<double> & exec_strat_param, const bool gen_new_oid, const string & oid_details)
{
  YYYYMMDDHHMMSS SystemTime = m_MarketData->GetSystemTimeHKT();
  m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: %s: Trade: %s Qty: %d ExecStrat: %d ExecStrat param: %d. OID Details: %s.", SystemTime.ToStr().c_str(), symbol.c_str(), signed_qty, exec_strat, exec_strat_param.size(), oid_details.c_str());

  if (signed_qty == 0)
  {
    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: %s: Trade: %s Qty: %d Not trading because qty is zero.",
                    SystemTime.ToStr().c_str(), symbol.c_str(), signed_qty);
    return false; 
  }

  //--------------------------------------------------
  // Get prices
  //--------------------------------------------------
  YYYYMMDDHHMMSS ymdhms;
  double dTP = NAN;
  double dMQ = NAN;
  bool bTP = m_MarketData->GetLatestTradePrice(symbol, dTP, ymdhms);
  bool bMQ = m_MarketData->GetLatestMidQuote(symbol, dMQ, ymdhms);
  double dP = (bTP ? dTP : dMQ);

  //--------------------------------------------------
  // check maximum allowed notional and quantity
  //--------------------------------------------------
  if (
    dP * abs(signed_qty) > m_SysCfg->GetMaxAllowedTradeNotional()
    ||
    abs(signed_qty) > m_SysCfg->GetMaxAllowedTradeQty()
    )
  {
    m_Logger->Write(Logger::ERROR,"PortfoliosAndOrders: %s: Max allowed notional / qty exceeded: sym = %s notional = %f qty = %d.",
                    m_MarketData->GetSystemTimeHKT().ToStr().c_str(),
                    symbol.c_str(),
                    dP * signed_qty,
                    signed_qty);
    return false;
  }

  //--------------------------------------------------
  // signalfeed to zmq
  //--------------------------------------------------
  if (m_SysCfg->NextTierZMQIsOn())
  {
    string sSF = ConstructAugmentedSignalFeed(port_id,symbol,dP,signed_qty,"OID");
    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: %s: (before sending) ZMQ signalfeed: %s.",
                    m_MarketData->GetSystemTimeHKT().ToStr().c_str(), sSF.c_str());
    SendStringToNextTierThruZMQ(sSF);
    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: %s: (after sending) ZMQ signalfeed: %s.",
                    m_MarketData->GetSystemTimeHKT().ToStr().c_str(), sSF.c_str());
  }

  //--------------------------------------------------
  if (m_SysCfg->Get_OTIMode() == SystemConfig::OTI_RECORD)
  {
    if (!bMQ)
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

    AcctTrade(port_id,symbol,signed_qty,dMQ);

    string sTF = ConstructAugmentedTradeFeed(port_id,symbol,dMQ,signed_qty,"OID","TID");
    m_Logger->Write(Logger::INFO,"%s", sTF.c_str());

    if (m_SysCfg->NextTierZMQIsOn())
    {
      SendStringToNextTierThruZMQ(sTF);
      m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: %s: ZMQ tradefeed: %s.",
                      m_MarketData->GetSystemTimeHKT().ToStr().c_str(), sTF.c_str());
    }

    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: %s: Trade: %s Qty: %d Price: %f.",
                    m_MarketData->GetSystemTimeHKT().ToStr().c_str(), symbol.c_str(), signed_qty, dMQ);

    return true;
  }
  //--------------------------------------------------

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

  m_working_orders.AddOrUpdate(oi.m_order_id,oi);

  m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: Added to working orders. m_order_id [%s] m_time_stamp [%s] m_symbol [%s] m_portfolio_id [%d] m_signed_qty [%d] m_order_exec_state [%d] m_exec_strategy [%d] m_exec_strategy_param (size) [%d] m_resend [%d]",
                  oi.m_order_id.c_str(),
                  oi.m_time_stamp.c_str(),
                  oi.m_symbol.c_str(),
                  oi.m_portfolio_id,
                  oi.m_signed_qty,
                  oi.m_order_exec_state,
                  oi.m_exec_strategy,
                  oi.m_exec_strategy_param.size(),
                  oi.m_resend
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

map<string,double> PortfoliosAndOrders::GetSymbolsAndMTMPnL(const int port_id)
{
  boost::unique_lock<boost::shared_mutex> lock(m_acct_mutex);

  map<int,Acct>::iterator it = m_acct.find(port_id);
  if (it == m_acct.end()) return map<string,double>();

  return it->second.GetSymbolsAndMTMPnL();
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
      it->m_price            <<  "," <<   setprecision(2) << fixed <<
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
  // iterate the working orders set and construct signalfeeds.
  const vector<std::pair<string,OrderInstruction> > vWOOI = m_working_orders.ToVector();
  vector<std::pair<string,OrderInstruction> > vWOOI2;

  FForEach(vWOOI,[&](const std::pair<string,OrderInstruction> p) {
    if (p.second.m_order_exec_state == OrderInstruction::PENDING_ADD_BEF_SF)
    {
      //--------------------------------------------------
      // Other order types are not handled
      //--------------------------------------------------
      // if (it_wkg_ord->second.m_exec_strategy == OrderInstruction::MARKET)
      // {
      ATU_OTI_signalfeed_struct strctSigFeed;
      MarketOrderConstructor(p.second, strctSigFeed);
      signalfeeds.push_back(strctSigFeed);

      OrderInstruction oi = p.second;
      oi.m_order_exec_state = OrderInstruction::PENDING_ADD_AFT_SF;
      vWOOI2.push_back(std::pair<string,OrderInstruction>(p.first,oi));
      // }
    }
  });

  m_working_orders.FromVectorReplaceOrAdd(vWOOI2);

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

bool PortfoliosAndOrders::UpdateOrderState(const string & order_id, const OrderInstruction::OrderExecState exec_state)
{

  Option<OrderInstruction> oOI = m_working_orders.Get(order_id);
  if (oOI.IsNone()) return false;

  OrderInstruction oi = oOI.Get();
  oi.m_order_exec_state = exec_state;

  if (exec_state == OrderInstruction::FILLED)
  {
    m_completed_orders.AddOrUpdate(order_id, oi);
    m_working_orders.Remove(order_id);
  }
  else
  {
    m_working_orders.AddOrUpdate(order_id, oi);
  }

  return true;
}

bool PortfoliosAndOrders::BookTradeFromOTI(const string & order_id, const string & symbol, const double price, const long signed_qty, const string & oid, const string & tid)
{
  m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: BookTradeFromOTI orderid %s symbol %s price %f signedqty %d oid %s tid %s",
                  order_id.c_str(),
                  symbol.c_str(),
                  price,
                  signed_qty,
                  oid.c_str(),
                  tid.c_str());

  //--------------------------------------------------
  // try to search the order_id in working orders and completed orders
  //--------------------------------------------------
  Option<OrderInstruction> oWOOI = m_working_orders.Get(order_id);
  Option<OrderInstruction> oCOOI = m_completed_orders.Get(order_id);

  if (oWOOI.IsNone() && oCOOI.IsNone())
  {
    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: BookTradeFromOTI orderid %s was not found in both completed and working orders.", order_id.c_str());
    return false;
  }

  int iPortID = -1;
  string sSymbol = "";

  if (oWOOI.IsSome())
  {
    OrderInstruction oi = oWOOI.Get();
    iPortID = oi.m_portfolio_id;
    sSymbol = oi.m_symbol;
    oi.m_signed_qty -= signed_qty;
    m_working_orders.AddOrUpdate(order_id, oi);

    if (oi.m_signed_qty == 0) UpdateOrderState(order_id, OrderInstruction::FILLED);
  }
  else
  {
    iPortID = oCOOI.Get().m_portfolio_id;
    sSymbol = oCOOI.Get().m_symbol;
  }
  //--------------------------------------------------

  AcctTrade(iPortID, sSymbol, signed_qty, price);

  //--------------------------------------------------
  // report the trade to next tier through zmq
  //--------------------------------------------------
  if (m_SysCfg->NextTierZMQIsOn())
  {
    string sTF = ConstructAugmentedTradeFeed(iPortID, sSymbol, price, signed_qty, oid, tid);
    SendStringToNextTierThruZMQ(sTF);
  }

  return true;
}

int PortfoliosAndOrders::GetUnprocessedWorkingOrdersCount()
{
  int iUnprocOrdCnt = 0;
  vector<std::pair<string,OrderInstruction> > vWO = m_working_orders.ToVector();

  FForEach(vWO,[&](const std::pair<string,OrderInstruction> p) {
    if (p.second.m_order_exec_state == OrderInstruction::PENDING_ADD_BEF_SF) iUnprocOrdCnt++;
  });

  return iUnprocOrdCnt;
}

bool PortfoliosAndOrders::IsOrderPendingAddAftSF(const string order_id)
{
  Option<OrderInstruction> oi = m_working_orders.Get(order_id);
  if (oi.IsNone()) return false;

  return oi.Get().m_order_exec_state == OrderInstruction::PENDING_ADD_AFT_SF;
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
  //--------------------------------------------------
  // Persist actual positions to file only if needed
  //--------------------------------------------------
  if (
    !m_SysCfg->PositionPersistenceIsEnabled()
    ||
    !CheckIfSomeAcctPosChgd()
    )
  {
    return;
  }

  //--------------------------------------------------
  // clear the position change flag first
  // in case somebody updates m_acct while we are writing the file, we will still know position is changed
  //--------------------------------------------------
  AcctClearPosChgFlagNoLock();

  //--------------------------------------------------
  // TODO: FIXME: Will definitely slow down the order execution by writing to file in this same thread
  //--------------------------------------------------
  boost::shared_lock<boost::shared_mutex> lock(m_acct_mutex);

  boost::scoped_ptr<ofstream> p_fsPosPersist;
  if (cOperation == 'f') p_fsPosPersist.reset(new ofstream(m_SysCfg->Get_PositionPersistenceFile().c_str()));

  for (map<int,Acct >::iterator it = m_acct.begin(); it != m_acct.end(); ++it)
  {
    const int & port_id = it->first;

    const map<string,long> map_pos = it->second.GetPortfolio();

    for (map<string,long>::const_iterator it2 = map_pos.begin(); it2 != map_pos.end(); ++it2)
    {
      const string & symbol = it2->first;
      const long signed_qty = it2->second;
      const double signed_notional = it->second.GetSignedNotional(symbol);

      if (cOperation == 'f') *p_fsPosPersist << port_id << "," << symbol << "," << signed_qty << "," << signed_notional << endl;

      m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: m_acct: port_id = %d symbol = %s signed_qty = %d signed_notional = %f",
                      port_id,
                      symbol.c_str(),
                      signed_qty,
                      signed_notional);
    }
  }

  if (cOperation == 'f') p_fsPosPersist->close();

  return;
}

double PortfoliosAndOrders::GetActualSignedPosition(const int portid, const string & symbol)
{
  boost::shared_lock<boost::shared_mutex> lock(m_acct_mutex);
  map<int,Acct>::iterator it = m_acct.find(portid);
  if (it == m_acct.end())
  {
    // if (!m_SysCfg->PositionPersistenceIsEnabled()) return 0;
    // else return NAN; // deliberately return shit to raise alarm
    return 0;
  }
  else return it->second.Pos(symbol);
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

    if (vs.empty()) continue;

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

    // UpdateTargetPortfolio(port_id,symbol,signed_qty,'r');
    AcctSetRecord(port_id,symbol,signed_qty,signed_notional);

    m_Logger->Write(Logger::INFO,"PortfoliosAndOrders: Read from Position persistence file : %d %s %d %f",port_id,symbol.c_str(),signed_qty,signed_notional);
  }

  // //--------------------------------------------------
  // // the case where there is nothing in the position persistion fl
  // //--------------------------------------------------
  // if (dqPosPersistFile.empty())
  // {
  //   for (int sty = STY_NIR; sty != STY_LAST; sty++)
  //   {
  //     if (m_SysCfg->IsStrategyOn(static_cast<StrategyID>(sty))) AcctSetRecord(sty,DUMMY_SYMBOL,0,0);
  //   }
  // }
  // //--------------------------------------------------

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

string PortfoliosAndOrders::ConstructAugmentedSignalFeed(const int port_id, const string & symbol, const double price, const long signed_qty, const string & oid)
{
  return m_MarketData->GetSystemTimeHKT().ToCashTimestampStr() + ",signalfeed,mktid," +
    symbol + "," + oid + "," +
    boost::lexical_cast<string>(price) + "," +
    boost::lexical_cast<string>((long)abs(signed_qty)) + "," +
    boost::lexical_cast<string>(signed_qty > 0 ? 1:2) + ",0," +
    GetStrategyName(static_cast<StrategyID>(port_id));
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

void PortfoliosAndOrders::SendStringToNextTierThruZMQ(const string & s)
{
  //--------------------------------------------------
  // Transmit through zmq
  //--------------------------------------------------
  zmq::message_t zmq_msg(s.length()+1);
  memcpy((void *)zmq_msg.data(), s.c_str(), s.length());
  ((char *)zmq_msg.data())[s.length()] = '\0';
  m_zmqsocket->send(zmq_msg);
}

