/*
 * ATU_IB_OTI.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: jasonlin
 */

#include "ATU_IB_OTI.h"

ATU_IB_OTI::ATU_IB_OTI(boost::shared_ptr<ExpandableCirBuffer> p, const string &p_host, unsigned int p_port, int p_clientId) :
  m_host(p_host)
  ,m_port(p_port)
  ,m_clientId(p_clientId)
  ,m_account(IBString(""))
  // ,m_isMaster(false)
  ,m_isMaster(false)
  ,m_nextReqId(0)
  ,m_isDataReady(false)
  ,m_hasReqOpenOrders(false)
  ,m_filenameOrderMapping("")
  ,m_nextValidOrderId(-1)
  ,m_fileForAppendingOrderMapping(NULL)
  ,m_clientSocket(this)
  ,m_contractExtractor(NULL)
  ,m_ecbOrd(p)
{
  m_Logger = StdStreamLogger::Instance();
}

ATU_IB_OTI::ATU_IB_OTI(boost::shared_ptr<ExpandableCirBuffer> p) :
  m_host("")
  ,m_port(-1)
  ,m_clientId(-1)
  ,m_account(IBString(""))
  // ,m_isMaster(false)
  ,m_isMaster(false)
  ,m_nextReqId(0)
  ,m_isDataReady(false)
  ,m_hasReqOpenOrders(false)
  ,m_filenameOrderMapping("")
  ,m_nextValidOrderId(-1)
  ,m_clientSocket(this)
  ,m_contractExtractor(NULL)
  ,m_ecbOrd(p)
{
  m_Logger = StdStreamLogger::Instance();
}

ATU_IB_OTI::~ATU_IB_OTI()
{
  for (map<IBString, ATU_OTI_tradefeed_struct*>::iterator iter = m_tradefeedList.begin(); iter != m_tradefeedList.end(); iter++)
    delete iter->second;

  if (m_contractExtractor != NULL) {
    delete m_contractExtractor;
  }

  m_fileForAppendingOrderMapping.close();
}

bool ATU_IB_OTI::isReady() const
{
  return m_nextValidOrderId > 0 && m_isDataReady;
}

//--------------------------------------------------
// setters
//--------------------------------------------------
void ATU_IB_OTI::setIP(const string & s)
{
  m_host = s;
}
void ATU_IB_OTI::setPort(const unsigned int p)
{
  m_port = p;
}
void ATU_IB_OTI::setClientID(const int id)
{
  m_clientId = id;
}
void ATU_IB_OTI::setAccount(const string & s)
{
  m_account = IBString(s);
}
void ATU_IB_OTI::setOrderMappingFile(const string & s)
{
    m_filenameOrderMapping = s;
}
//--------------------------------------------------


void ATU_IB_OTI::readContracts(string p_filepath) {
  m_contractExtractor = new ContractManager();
  cout << "Reading contracts..." << endl;
  IniContractReader* iniReader = new IniContractReader(p_filepath);
  m_contractExtractor->extract(iniReader);
  delete iniReader;
}

void ATU_IB_OTI::run()
{
  if (m_host.size() == 0 || m_port == -1 || m_clientId == -1) {
    m_Logger->Write(StdStreamLogger::ALERT,"%s: --->>> IP, port, and client ID are all necessary", __FILENAME__);
    return;
  }

  if (isReady())
    return;

  m_Logger->Write(StdStreamLogger::INFO,"%s: --->>> Initiating OTI connection to TWS...", __FILENAME__);

  bool bConn = m_clientSocket.eConnect(m_host.c_str(), m_port, m_clientId);
  if (bConn) {

    m_Logger->Write(StdStreamLogger::INFO,"%s: --->>> OTI connected to TWS!", __FILENAME__);

    m_clientSocket.startHandlingSendRecvMsg();

    if (!m_isMaster) {
      // Get working orders and trade history
      m_clientSocket.reqOpenOrders();
      // Get trade history will be called when openOrderEnd() is called.

      // Loop until it is ready
      while (!isReady()) {}
    } else {
      m_isDataReady = true;
    }
    m_Logger->Write(StdStreamLogger::INFO,"%s: --->>> OTI is ready.", __FILENAME__);
  } else {
    m_Logger->Write(StdStreamLogger::ALERT,"%s: --->>> OTI failed to connect to TWS.", __FILENAME__);
  }
}

bool ATU_IB_OTI::on_process_signalfeed(ATU_OTI_signalfeed_struct &s)
{
  Contract contract;
  AtuContract* atuContract = NULL;
  const string sFeedcode = string(s.m_instrument);

  // Firstly, look up the product code in the contractExtractor
  if ( m_contractExtractor->hasProductCode(sFeedcode) ) {
    atuContract = m_contractExtractor->getContract(sFeedcode);
    Utility::initIBContractFromAtuContract(&contract, atuContract, m_contractExtractor);
  }
  // The content of feedcode follows the pattern: {symbol}-{localSymbol}-{secType} or {your feedcode}_{contractId}
  else {
    bool wrongFeedcode = false;
    vector<string> parts;
    boost::split(parts, sFeedcode, boost::is_any_of("-"));
    if (parts.size() >= 3) {
      contract.symbol = parts[0];
      contract.localSymbol = parts[1];
      contract.secType = parts[2];
      if (parts.size() > 3) {
        contract.currency = parts[3];
      }
    } else if (sFeedcode.find('_') != string::npos) {
      boost::split(parts, sFeedcode, boost::is_any_of("_"));
      if (parts.size() == 2) {
        contract.conId = boost::lexical_cast<long>(parts[1]);
      } else {
        wrongFeedcode = true;
      }
    } else {
      wrongFeedcode = true;
    }

    if (wrongFeedcode) {
      m_Logger->Write(StdStreamLogger::WARNING,"%s: Feedcode is incorrect. it should include symbol-localSymbol-secType, or feedcodeName_contractId.", __FILENAME__);
      return false;
    }

    contract.exchange = s.m_market;
  }

  Order order;
  bool hasError = false;
  string errMsg;

  // For nonGuaranteed combo, set smartComboRoutingParams of the order
  if ( (atuContract != NULL) && (atuContract->get("secType") == "COMBO") && (atuContract->get("nonGuaranteed") == "true") ) {
    TagValueSPtr pa(new TagValue("NonGuaranteed", "1"));
    order.smartComboRoutingParams = TagValueListSPtr(new TagValueList());
    order.smartComboRoutingParams->push_back(pa);
  }

  m_checkOrderIdMutex.lock();
  // If order id already exists
  if (m_userOrderId2IBOrderId.find(s.m_order_id) != m_userOrderId2IBOrderId.end()) {
    if (s.m_order_action == "insert") {
      errMsg = "The order id has been used";
      hasError = true;
    } else if (s.m_order_action == "delete") {
      // If it has been explicitly deleted, or, has error (has error also means it has been deleted)
      if ( 		(m_userOrderId2IBOrderId[s.m_order_id] == -1) ||
          (m_orderId2filledQty[m_userOrderId2IBOrderId[s.m_order_id]] == ORDER_FILLED_QTY_ERROR) ||
          (m_orderId2filledQty[m_userOrderId2IBOrderId[s.m_order_id]] == ORDER_FILLED_QTY_COMPLETED)
         ) {
        errMsg = "Cannot delete the order (not in the list of working orders)";
        hasError = true;
      } else {
        order.orderId = m_userOrderId2IBOrderId[s.m_order_id];
      }
    } else {
      errMsg = "The order action should be either insert or delete";
      hasError = true;
    }
  } else {
    if (s.m_order_action == "delete") {
      errMsg = "Order id doesn't exist";
      hasError = true;
    } else {
      // Newly inserted order
      order.orderId = m_nextValidOrderId;
      m_userOrderId2IBOrderId[s.m_order_id] = order.orderId;
      m_IBOrderId2userOrderId[order.orderId] = s.m_order_id;
      // Update the next valid order id.
      m_nextValidOrderId++;
    }
  }
  m_checkOrderIdMutex.unlock();

  order.action = IBString( s.m_buy_or_sell == 1 ? "BUY" : "SELL" );
  order.totalQuantity = s.m_qty;
  order.orderRef = s.m_order_id;

  if (s.m_order_type == "market_order")
    order.orderType = IBString("MKT");
  else if (s.m_order_type == "limit_order")
    order.orderType = IBString("LMT");
  else if (s.m_order_action == "insert") {
    errMsg = "Currently only accepts market_order and limit_order";
    hasError = true;
  }

  order.lmtPrice = s.m_price;

  if (s.m_order_validity == "today") {
    order.tif = IBString("DAY");
  } else if (s.m_order_validity == "fill_and_kill") {
    order.tif = IBString("IOC");
  } else if (s.m_order_validity == "fill_or_kill") {
    order.tif = IBString("FOK");
    //		order.allOrNone = 1;
  } else if (s.m_order_action == "insert") {
    errMsg = "There are only three options for order validity: today or fill_and_kill or fill_or_kill";
    hasError = true;
  }

  // IB is weird: If deleting an open order, the status becomes closed
  // So force the openClose to remain as the original value
  // Ignore IB's behavior now: order.openClose = IBString( s.m_open_or_close == "open" ? "O" : "C" );
  order.openClose = IBString(s.m_open_or_close);

  if (hasError) {
    ATU_OTI_orderfeed_struct of;
    of.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
    of.m_market = contract.exchange;
    of.m_instrument = sFeedcode;
    of.m_order_id = s.m_order_id;
    of.m_price = s.m_price;
    of.m_qty = s.m_qty;
    of.m_open_or_close = s.m_open_or_close;
    of.m_buy_or_sell = s.m_buy_or_sell;
    of.m_qty_filled = 0;
    of.m_deleted = 1;
    of.m_order_status = 1;
    of.m_error_description = errMsg;
    of.m_source = 0;
    of.m_order_type = s.m_order_type;

    notify_orderfeed(of);

    return false;
  }

  order.account = m_account;

  if (s.m_order_action == "insert") {
    m_clientSocket.placeOrderSync(order.orderId, contract, order);
    // -1 is used to mark the order is just created. If orderStatus() is called, it is set to 0
    // error() will set it to -2
    // So, judging the value of it:
    //     If it is still -1, that means the orderStatus() hasn't been called and it is still an just-sent order
    //     If it is -2, that means the order is not a successful order.
    // e.g. if sending an order with an incorrect feedcode, orderStatus() is not called. so it remains -1. This
    //		is used to judge an order in get_working_orders.
    m_orderId2filledQty[order.orderId] = ORDER_FILLED_QTY_INIT;
  }
  else if (s.m_order_action == "delete") {
    m_clientSocket.cancelOrderSync(order.orderId);
  }

  if (s.m_order_action != "delete") {
    m_orderId2Contract[order.orderId] = contract;
    m_orderId2feedcode[order.orderId] = sFeedcode;
    m_orderId2Order[order.orderId] = order;

    ostringstream oss;
    oss << s.m_order_id;
    oss << ",";
    oss << order.orderId;
    oss << ",";
    oss << sFeedcode;
    oss << ",";
    oss << s.m_open_or_close;
    oss << ",";
    oss << s.m_buy_or_sell;
    string line = oss.str();
    m_fileForAppendingOrderMapping << line << endl;
    m_Logger->Write(StdStreamLogger::INFO,"%s: Appended order mapping: %s", __FILENAME__, line.c_str());
  }

  return true;
}

bool ATU_IB_OTI::on_process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &p_s)
{
  m_orderId2filledQtyMutex.lock();

  for (map<OrderId, int>::iterator iter = m_orderId2filledQty.begin(); iter != m_orderId2filledQty.end(); iter++) {
    OrderId orderId = iter->first;
    int filledQty = iter->second;

    // If it has error
    if ( (filledQty == ORDER_FILLED_QTY_ERROR) || (filledQty == ORDER_FILLED_QTY_COMPLETED) )
      continue;

    ATU_OTI_orderfeed_struct of;
    const Contract &contract = m_orderId2Contract[orderId];
    const Order &order = m_orderId2Order[orderId];
    of.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
    of.m_market = contract.exchange;
    of.m_instrument = m_orderId2feedcode[orderId];
    of.m_order_id = m_IBOrderId2userOrderId[orderId];
    of.m_price = order.lmtPrice;
    of.m_qty = order.totalQuantity;
    // Ignore IB's behavior: of.m_open_or_close = order.openClose == "O" ? "open" : "close";
    of.m_open_or_close = order.openClose;
    of.m_buy_or_sell = order.action == "BUY" ? 1 : 2;
    of.m_qty_filled = ( filledQty == ORDER_FILLED_QTY_INIT ? 0 : filledQty );
    of.m_deleted = 0;
    of.m_order_status = 0;
    of.m_error_description = "";
    of.m_source = 1;
    of.m_islast = 0;
    of.m_order_type = order.orderType == "LMT" ? "limit_order" : "market_order";

    notify_orderfeed(of);
  }

  m_orderId2filledQtyMutex.unlock();
  m_orderId2filledQtyCond.notify_all();

  // Last feed
  ATU_OTI_orderfeed_struct of;
  of.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  of.m_source = 1;
  of.m_islast = 1;
  notify_orderfeed(of);

  return true;
}

bool ATU_IB_OTI::on_process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &p_s)
{
  // Get today
  string strToday = SDateTime::GetCurrentTimeYYYYMMDD();

  m_getTradeHistoryMutex.lock();

  for (map<IBString, ATU_OTI_tradefeed_struct*>::iterator iter = m_tradefeedList.begin(); iter != m_tradefeedList.end(); iter++) {

    if ( ( p_s.m_period == "today" ) && ( strToday != (iter->second->m_timestamp).substr(0, 8) ) )
      continue;

    iter->second->m_source = 1;
    //	Should use traded time, not read time.   (*iter)->m_timestamp = Toolbox::timeStamp_full();
    notify_tradefeed(*(iter->second));
  }

  m_getTradeHistoryMutex.unlock();

  ATU_OTI_tradefeed_struct tf;
  tf.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  tf.m_islast = 1;
  tf.m_source = 1;
  notify_tradefeed(tf);

  return true;
}

void ATU_IB_OTI::orderStatus(OrderId p_orderId, const IBString &p_status, int p_filled, int p_remaining, double p_avgFillPrice, int p_permId,
                             int p_parentId, double p_lastFillPrice, int p_clientId, const IBString &p_whyHeld)
{
  // TODO: Handle Master API Client ID to receive and save all orders
  //	cout << ">>>>>>>>>>>>> ClientID:" << m_orderId2Order[p_orderId].clientId << ", OrderID:" << m_orderId2Order[p_orderId].orderId << endl;

  // As our API has no pendingsubmitted, presubmitted, and submitted status, all these status will be regardes as successful submittion

  m_Logger->Write(StdStreamLogger::INFO,"%s: Order status: orderid:%d, status:%s", __FILENAME__, p_orderId, p_status.c_str());


  // IB has a new status : ApiCancelled
  bool isCancelled = ( (p_status == "ApiCancelled") || (p_status == "Cancelled") || (p_status == "Inactive") );

  // if the order has been deleted (to avoid duplicate completed orderfeed)
  if ( !m_isMaster && (m_orderId2filledQty[p_orderId] == ORDER_FILLED_QTY_COMPLETED || (!isCancelled && p_filled == m_orderId2filledQty[p_orderId])) )
    return;

  ATU_OTI_orderfeed_struct of;
  const Contract &contract = m_orderId2Contract[p_orderId];
  const Order &order = m_orderId2Order[p_orderId];
  of.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  of.m_market = contract.exchange;
  of.m_instrument = !m_isMaster ? m_orderId2feedcode[p_orderId] : (contract.symbol + "-" + contract.localSymbol + "-" + contract.secType);
  of.m_order_id = !m_isMaster ? m_IBOrderId2userOrderId[p_orderId] : (boost::lexical_cast<string>(p_clientId) + ":" + boost::lexical_cast<string>(p_orderId));
  of.m_price = order.lmtPrice;
  of.m_qty = order.totalQuantity;

  //	cout << "[openClose]" << order.openClose << endl;

  // Ignore IB's behavior: of.m_open_or_close = order.openClose == "O" ? "open" : "close";
  of.m_open_or_close = order.openClose;
  of.m_buy_or_sell = order.action == "BUY" ? 1 : 2;
  of.m_qty_filled = p_filled;
  m_orderId2filledQty[p_orderId] = p_filled;
  of.m_order_status = 0;
  of.m_error_description = "";

  // error msg should appear in the err descption field
  if (m_orderId2errMsg.find(p_orderId) != m_orderId2errMsg.end()) {
    // In IB, error does not equal to real error. Maybe just warning.
    of.m_order_status = 1;
    of.m_error_description = m_orderId2errMsg[p_orderId];
    m_orderId2errMsg.erase(p_orderId);
  }
  of.m_deleted = (isCancelled || p_filled == order.totalQuantity) ? 1 : 0;
  of.m_source = 0;
  of.m_order_type = order.orderType == "LMT" ? "limit_order" : "market_order";

  if (of.m_deleted == 1 && m_orderId2filledQty.find(p_orderId) != m_orderId2filledQty.end()) {
    m_orderId2filledQty[p_orderId] = ORDER_FILLED_QTY_COMPLETED;
    string userOrderId = m_IBOrderId2userOrderId[p_orderId];
    //		m_IBOrderId2userOrderId.erase(p_orderId);
    //		m_orderId2Contract.erase(p_orderId);
    //		m_orderId2Order.erase(p_orderId);
    // Mark it as deleted. Cannot erase, still be used to check duplicate order id
    m_userOrderId2IBOrderId[userOrderId] = -1;
  }

  if (isReady())
    notify_orderfeed(of);
}

void ATU_IB_OTI::error(const int p_id, const int p_errorCode, const IBString p_errorString)
{
  //--------------------------------------------------
  // TODO
  // IB has very weird cases. The main triggered functions are : error() and orderStatus()
  //
  // --------------------------------------------------------------------------------------------------------
  // | CASE 1: user deletes order | CASE 2: incorrect feedcode |	CASE 3: normal order
  // error():		|		   CALLED			 |			CALLED			  |
  // orderStatus():	|		   CALLED			 |			  				  |			CALLED
  // --------------------------------------------------------------------------------------------------------
  //
  // (1) What error() returns is not always exactly error. It might be just a warning. Therefore, we cannot delete an order upon error.
  // But it is not a good idea to evaluate if the error message contains "Warning".
  // (2) Considering the CASE 1 and CASE 2 above, the solutions are adopted currently to work around the problem
  // - For CASE 1: leave error message in orderfeed. It is not consistent with our protocal because the orderfeed associated with a normally-deleted
  // order never contains error.
  // - For CASE 2: simply throw out the error message (errorfeed in future).
  //
  // The following cases will not call orderStatus() again:
  // TODO:
  // Code  |  errmsg
  // 110   |  The price does not conform to the minimum price variation for this contract
  // 135	  |  Can't find order with id =
  // 200	  |  No security definition has been found for the request.
  //
  // Currently, these cases will make the fillQty as -2 because no orderStatus() is called
  //--------------------------------------------------

  // Save error to future errorFeed

  // Replace special symbols
  string strErrorString(p_errorString);
  std::replace(strErrorString.begin(), strErrorString.end(), '\n', '.');
  std::replace(strErrorString.begin(), strErrorString.end(), ',', ';');

  // When the orderStatus is 'Inactive', orderStatus() will be called before error()
  if ( m_orderId2filledQty.find(p_id) != m_orderId2filledQty.end() ) {
    // The order has been deleted
    m_orderId2filledQty[p_id] = ORDER_FILLED_QTY_ERROR;
    m_orderId2errMsg[p_id] = strErrorString;
  }

  const string errormsg = boost::lexical_cast<string>(p_errorCode) + " | " + strErrorString;
  const string errsource = m_IBOrderId2userOrderId[p_id];

  m_orderId2errMsg.erase(p_id);
  const string filteredMsg = string("Filter errorfeed: source=") + errsource + string("; errormsg=") + errormsg;
  m_Logger->Write(StdStreamLogger::ERROR,"%s: Error %s", __FILENAME__, filteredMsg.c_str());
}

void ATU_IB_OTI::openOrder(OrderId p_orderId, const Contract &p_contract, const Order &p_order, const OrderState &p_orderState)
{
  IBString originalOpenClose;
  if (m_orderId2Order.find(p_orderId) != m_orderId2Order.end()) {
    originalOpenClose = m_orderId2Order[p_orderId].openClose;
  } else {
    originalOpenClose = m_tmpOrderId2OpenClose[p_orderId];
  }
  // In case the order is modified by IB (e.g. stop order)
  m_orderId2Contract[p_orderId] = p_contract;
  m_orderId2Order[p_orderId] = p_order;
  if (!m_isMaster) {
    m_orderId2Order[p_orderId].openClose = originalOpenClose;
  }
  //	cout << "open order" << endl;
}

void ATU_IB_OTI::openOrderEnd()
{
  if (m_hasReqOpenOrders) {
    return;
  }
  m_hasReqOpenOrders = true;

  ExecutionFilter filter;
  filter.m_acctCode = m_account;
  filter.m_clientId = m_clientId;
  filter.m_time = "20140101-00:00:00";
  m_clientSocket.reqExecutions(1, filter);
}

// This is called when just connecting to TWS, for determining the next valid order id
void ATU_IB_OTI::nextValidId(OrderId p_orderId)
{
  m_nextValidOrderId = p_orderId;
}

void ATU_IB_OTI::deltaNeutralValidation(int p_reqId, const UnderComp &p_underComp)
{

}

inline string ibTradeTime2AtuTime(const string& p_ibTime) {
  char charArr[23];
  strcpy(charArr, p_ibTime.c_str());
  charArr[8] = '_';
  char* ptr = charArr + 9;
  for (int pos = 10; pos < p_ibTime.size(); pos++) {
    if ( isdigit(p_ibTime[pos]) ) {
      *ptr = p_ibTime[pos];
      ptr++;
    }
  }
  charArr[15] = '_';
  for (int idx = 16; idx < 22; idx++) {
    charArr[idx] = '0';
  }
  charArr[22] = 0;
  return string(charArr);
}

void ATU_IB_OTI::execDetails(int p_reqId, const Contract &p_contract, const Execution& p_execution)
{
  //	cout << "------------- >>>>>>>>>>>>> ExecDetais: orderid:" << p_execution.orderId << " -> " << p_contract.symbol << endl;
  ATU_OTI_tradefeed_struct* tf = new ATU_OTI_tradefeed_struct();
  const Order &order = m_orderId2Order[p_execution.orderId];
  const Contract &contract = !m_isMaster ? m_orderId2Contract[p_execution.orderId] : p_contract;
  tf->m_timestamp = ibTradeTime2AtuTime(p_execution.time);
  // Use IB's traded time.  tf->m_timestamp = Toolbox::timeStamp_full();
  tf->m_market = p_contract.exchange;

  long conId = p_contract.conId;
  string productCode = m_contractExtractor->getProductCode(boost::lexical_cast<string>(conId));
  if ( productCode == "" ) {
    tf->m_instrument = !m_isMaster ? m_orderId2feedcode[p_execution.orderId] : (contract.symbol + "-" + contract.localSymbol + "-" + contract.secType);
  } else {
    tf->m_instrument = productCode;
  }

  tf->m_order_id = !m_isMaster ? m_IBOrderId2userOrderId[p_execution.orderId] : (boost::lexical_cast<string>(p_execution.clientId) + ":" + boost::lexical_cast<string>(p_execution.orderId));
  tf->m_price = p_execution.price;
  tf->m_qty = p_execution.shares;
  // When reqExecutions(), order.action is empty
  //	tf->m_buy_or_sell = order.action == "BUY" ? 1 : 2;
  tf->m_buy_or_sell = p_execution.side == "BOT" ? 1 : 2;
  if (!isReady()) {
    tf->m_buy_or_sell = m_tmpOrderId2BuySell[p_execution.orderId] == "1" ? 1 : 2;
  }
  tf->m_trade_id = p_execution.execId;
  tf->m_source = p_reqId == -1 ? 0 : 1;

  m_tradefeedList[p_execution.execId] = tf;

  if (isReady())
    notify_tradefeed(*tf);
}

void ATU_IB_OTI::execDetailsEnd(int p_reqId)
{
  // Weird, this function is called twice. No choice but to comment out it
  //	if (m_isDataReady) {
  //		throw "execDetailsEnd() is called!!";
  //	}
  m_isDataReady = true;
}


void ATU_IB_OTI::readOrderMapping() {
  FILE *file = fopen(m_filenameOrderMapping.c_str(), "r");
  if (file == NULL) {
    throw "The order mapping file doesn't exist.";
  } else {
    char line[501];
    while (fgets(line, 501, file) && !feof(file)) {
      if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = 0;
      }
      if (strlen(line) > 0) {
        // user order id, ib order id, product code, open/close, 1/2 (buy/sell)
        vector<string> parts;
        string row = string(line);
        cout << "Read a order mapping: " << row << endl;
        boost::split(parts, row, boost::is_any_of(","));
        if (parts.size() == 5) {
          string userOrderId = parts[0];
          string IBOrderId = parts[1];
          string productCode = parts[2];
          string openClose = parts[3];
          string buySell = parts[4];

          OrderId longIBOrderId = boost::lexical_cast<OrderId>(IBOrderId);
          m_userOrderId2IBOrderId[userOrderId] = longIBOrderId;
          m_IBOrderId2userOrderId[longIBOrderId] = userOrderId;
          m_orderId2feedcode[longIBOrderId] = productCode;
          m_orderId2filledQty[longIBOrderId] = ORDER_FILLED_QTY_ERROR;
          m_tmpOrderId2OpenClose[longIBOrderId] = openClose;
          m_tmpOrderId2BuySell[longIBOrderId] = buySell;
        } else {
          throw "The format of the order mapping file has problems.";
        }
      }
    }
    fclose(file);
  }

  // Open it again for writing
  m_fileForAppendingOrderMapping.open(m_filenameOrderMapping.c_str(), std::ios::out | std::ios::app);
  if (!m_fileForAppendingOrderMapping.is_open()) {
    throw "The order mapping file is not ready for appending";
  }
}

void ATU_IB_OTI::notify_orderfeed(const ATU_OTI_orderfeed_struct & of)
{
  BYTE * pb = m_ecbOrd->GetWritingPtr();
  strcpy((char*)pb,ATU_OTI_orderfeed_struct::ToString(of).c_str());
  m_ecbOrd->PushBack(strlen((char*)pb));
}

void ATU_IB_OTI::notify_tradefeed(const ATU_OTI_tradefeed_struct & tf)
{
  BYTE * pb = m_ecbOrd->GetWritingPtr();
  strcpy((char*)pb,ATU_OTI_tradefeed_struct::ToString(tf).c_str());
  m_ecbOrd->PushBack(strlen((char*)pb));
}

