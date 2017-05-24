#include "CtpOrd.h"

CtpOrd::CtpOrd(boost::shared_ptr<ExpandableCirBuffer> p) : m_pCThostFtdcTraderApi(NULL), m_iRequestID(0), m_orderRefCount(0), m_lastOrderTimestamp(""),m_ecbOrd(p)
{
  m_Logger = StdStreamLogger::Instance();
}
//--------------------------------------------------
// setters
//--------------------------------------------------
void CtpOrd::setFlowDataFolder(const string & fdf)
{
  m_FlowDataFolder = fdf;
}
void CtpOrd::setConnectString(const string & connStr)
{
  m_connection_string = connStr;
}
void CtpOrd::setBrokerID(const string & bid)
{
  strcpy(m_broker_id,(char*)bid.c_str());
}
void CtpOrd::setInvestorID(const string & iid)
{
  strcpy(m_user_id,(char*)iid.c_str());
}
void CtpOrd::setPassword(const string & pwd)
{
  strcpy(m_password,(char*)pwd.c_str());
}

void CtpOrd::run()
{
  m_p_ctp_lib_handle=dlopen("thosttraderapi.so",RTLD_NOW);
  if (m_p_ctp_lib_handle == NULL)
  {
    m_Logger->Write(StdStreamLogger::ERROR,"%s: CTP libthosttraderapi.so NOT loaded. %s", __FILENAME__, dlerror());
    return;
  }
  else
  {
    m_Logger->Write(StdStreamLogger::INFO,"%s: CTP libthosttraderapi.so loaded", __FILENAME__);
  }
  typedef CThostFtdcTraderApi* (*CreateFtdcTradeApiPtr)(char const*);
  CreateFtdcTradeApiPtr CreateFtdcTradeApi= (CreateFtdcTradeApiPtr)dlsym(m_p_ctp_lib_handle,"_ZN19CThostFtdcTraderApi19CreateFtdcTraderApiEPKc");

  m_pCThostFtdcTraderApi=CreateFtdcTradeApi(m_FlowDataFolder.c_str());
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called CreateFtdcTradeApi", __FILENAME__);
  m_pCThostFtdcTraderApi->RegisterSpi((CThostFtdcTraderSpi*) this);
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called RegisterSpi()", __FILENAME__);
  m_pCThostFtdcTraderApi->SubscribePublicTopic(THOST_TERT_QUICK);
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called SubscribePublicTopic()", __FILENAME__);
  m_pCThostFtdcTraderApi->SubscribePrivateTopic(THOST_TERT_QUICK);
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called SubscribePrivateTopic()", __FILENAME__);
  m_pCThostFtdcTraderApi->RegisterFront((char*)m_connection_string.c_str());
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called RegisterFront()", __FILENAME__);
  m_pCThostFtdcTraderApi->Init();
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called Init()", __FILENAME__);
  if (m_pCThostFtdcTraderApi!=NULL) m_pCThostFtdcTraderApi->Join();
  m_Logger->Write(StdStreamLogger::INFO,"%s: After Join()", __FILENAME__);
}

CtpOrd::~CtpOrd() {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Trader API is released", __FILENAME__);
  if (m_pCThostFtdcTraderApi!=NULL) {
    m_pCThostFtdcTraderApi->RegisterSpi(NULL);
    m_pCThostFtdcTraderApi->Release();
    m_pCThostFtdcTraderApi = NULL;
  }

  if (m_p_ctp_lib_handle!=NULL) {
    dlclose(m_p_ctp_lib_handle);
  }
}
bool CtpOrd::on_process_signalfeed(ATU_OTI_signalfeed_struct &s) {
  if (s.m_order_action.compare("insert")==0)
  {
    if (s.m_order_id != "" &&
        s.m_instrument != "")
    {
      map<string,string>::iterator it = m_orderid_instrument.find(s.m_order_id);

      if (it == m_orderid_instrument.end())
      {
        m_orderid_instrument[s.m_order_id] = s.m_instrument;
      }
    }
    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, m_broker_id);
    strcpy(req.InvestorID, m_user_id);
    strcpy(req.InstrumentID, s.m_instrument.c_str());
    string orderref=createOrderRef(s.m_order_id);
    strcpy(req.OrderRef, orderref.c_str());

    req.ContingentCondition = THOST_FTDC_CC_Immediately;
    //Limit order or Market order or stop order
    if(s.m_order_type.compare("limit_order")==0){
      req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
      req.LimitPrice = s.m_price;
      //TODO: limit order with expiration time
    }
    else if(s.m_order_type.compare("market_order")==0){
      req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
      req.LimitPrice = 0;
    }
    else if(s.m_order_type.compare("stop_order")==0){
      char _attr[100],_temp[20];
      strcpy(_attr,s.m_order_attributes.c_str());
      double _stop_price; int _stop_condition; char _orderType[15];
      for(int i = 0 ; i < strlen(_attr) ; i++) if (_attr[i] == ';' || _attr[i] == '=') _attr[i] = ' ';
      sscanf(_attr,"%s%lf%s%d%s%s",_temp,&_stop_price,_temp,&_stop_condition,_temp,_orderType);
      if(strcmp(_orderType,"market_order") == 0){
        req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
        req.LimitPrice = 0;
      }
      else {
        req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
        req.LimitPrice = s.m_price;
      }
      req.StopPrice = _stop_price;
      m_Logger->Write(StdStreamLogger::INFO,"%s: Stop condition: %f %d %s", __FILENAME__, _stop_price,_stop_condition,_orderType);
      switch(_stop_condition){
        case 1:	{req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterEqualStopPrice; break;}
        case 2:	{req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserEqualStopPrice; break;}
        case 3:	{req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterEqualStopPrice; break;}
        case 4:	{req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserEqualStopPrice; break;}
        case 5:	{req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice; break;}
        case 6:	{req.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice; break;}
      }
    }

    //Buy or Sell
    if (s.m_buy_or_sell==1) {
      req.Direction = THOST_FTDC_D_Buy;
    } else {
      req.Direction = THOST_FTDC_D_Sell;
    }
    req.UserForceClose = 0;

    //Open or Close
    if (s.m_open_or_close.compare("open") == 0){
      req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    }
    else if(s.m_open_or_close.compare("close") == 0){
      //			req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
      req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
      //some exchange use CloseToday some use Close
      //never use Force Close or it cant be cancel
      //			req.UserForceClose = 1;

    }

    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;

    req.VolumeTotalOriginal = s.m_qty;

    //Order validity
    if(s.m_order_validity.compare("today") == 0){
      req.TimeCondition = THOST_FTDC_TC_GFD;//today
      req.VolumeCondition = THOST_FTDC_VC_AV;//any volume
    }
    else if(s.m_order_validity.compare("fill_and_kill") == 0){
      req.TimeCondition = THOST_FTDC_TC_IOC;//immediately
      req.VolumeCondition = THOST_FTDC_VC_AV;//any volume
    }
    else if(s.m_order_validity.compare("fill_or_kill") == 0){
      req.TimeCondition = THOST_FTDC_TC_IOC;//immediately
      req.VolumeCondition = THOST_FTDC_VC_CV;//all volume
    }

    //MinVoume is valid when req.VolumeCondition = THOST_FTDC_VC_MV //minimum volume
    req.MinVolume = 1;

    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    req.IsAutoSuspend = 0;
    boost::unique_lock<boost::recursive_mutex> lock(m_requestIDMutex);
    int iResult = m_pCThostFtdcTraderApi->ReqOrderInsert(&req, ++m_iRequestID);

    m_Logger->Write(StdStreamLogger::INFO,"%s: Order Insert to CTP order id=%s", __FILENAME__, s.m_order_id.c_str());
    m_Logger->Write(StdStreamLogger::INFO,"%s: ReqOrderInsert result: %s", __FILENAME__, ((iResult == 0) ? "OK" : "Fail"));

    //this should generate errorfeed or orderfeed as notification of error
  }
  else if (s.m_order_action.compare("delete")==0) {
    CThostFtdcInputOrderActionField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, m_broker_id);
    strcpy(req.InvestorID, m_user_id);
    strcpy(req.OrderRef, getOrderRef(s.m_order_id).c_str());
    req.FrontID = m_front_id;
    req.SessionID = m_session_id;
    req.ActionFlag = THOST_FTDC_AF_Delete;
    if (s.m_instrument == "" &&
        m_orderid_instrument.find(s.m_order_id) != m_orderid_instrument.end())
      s.m_instrument = m_orderid_instrument[s.m_order_id];
    strcpy(req.InstrumentID, s.m_instrument.c_str());
    m_Logger->Write(StdStreamLogger::INFO,"%s: Order Deleted from CTP order id=%s", __FILENAME__, s.m_order_id.c_str());
    boost::unique_lock<boost::recursive_mutex> lock(m_requestIDMutex);
    int iResult = m_pCThostFtdcTraderApi->ReqOrderAction(&req, ++m_iRequestID);
    m_Logger->Write(StdStreamLogger::INFO,"%s: ReqOrderAction result: %s", __FILENAME__, ((iResult == 0) ? "OK" : "Fail"));
    //this should generate errorfeed or orderfeed as notification of error
  }

  return true;
}

bool CtpOrd::on_process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &s) {
  CThostFtdcQryOrderField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, m_broker_id);
  strcpy(req.InvestorID, m_user_id);
  boost::unique_lock<boost::recursive_mutex> lock(m_requestIDMutex);
  if (m_pCThostFtdcTraderApi == NULL) sleep(2); // The purpose of adding sleep is that the CTP initialization may take more time to finish.
  int iResult = m_pCThostFtdcTraderApi->ReqQryOrder(&req, ++m_iRequestID);
  m_Logger->Write(StdStreamLogger::INFO,"%s: ReqQryOrder result: %s", __FILENAME__, ((iResult == 0) ? "OK" : "Fail"));

  return true;
}
bool CtpOrd::process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &s) {
  return true;
}
bool CtpOrd::process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &s)
{
  return true;
}
bool CtpOrd::on_process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &s)
{
  CThostFtdcQryTradeField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, m_broker_id);
  strcpy(req.InvestorID, m_user_id);
  boost::unique_lock<boost::recursive_mutex> lock(m_requestIDMutex);
  if (m_pCThostFtdcTraderApi == NULL) sleep(2); // The purpose of adding sleep is that the CTP initialization may take more time to finish.
  int iResult = m_pCThostFtdcTraderApi->ReqQryTrade(&req, ++m_iRequestID);
  m_Logger->Write(StdStreamLogger::INFO,"%s: ReqQryTrade result: %s", __FILENAME__, ((iResult == 0) ? "OK" : "Fail"));
  return true;
}


void CtpOrd::OnFrontConnected() {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Front is connected", __FILENAME__);
  ReqUserLogin();
}

void CtpOrd::ReqUserLogin() {
  CThostFtdcReqUserLoginField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, m_broker_id);
  strcpy(req.UserID, m_user_id);
  strcpy(req.Password, m_password);
  boost::unique_lock<boost::recursive_mutex> lock(m_requestIDMutex);
  int iResult = m_pCThostFtdcTraderApi->ReqUserLogin(&req, ++m_iRequestID);
  m_Logger->Write(StdStreamLogger::INFO,"%s: UserLogin result: %s", __FILENAME__, ((iResult == 0) ? "OK" : "Fail"));
}

void CtpOrd::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRspUserLogin()", __FILENAME__);
  if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
    m_front_id = pRspUserLogin->FrontID;
    m_session_id = pRspUserLogin->SessionID;
    //		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
    //		iNextOrderRef++;
    //		sprintf(ORDER_REF, "%d", iNextOrderRef); // overflow if > 13 char
    m_Logger->Write(StdStreamLogger::INFO, "%s: pRspUserLogin->MaxOrderRef = %s", __FILENAME__,  pRspUserLogin->MaxOrderRef);
    m_Logger->Write(StdStreamLogger::INFO, "%s: --->>> GetTradingDay() = %s", __FILENAME__,  m_pCThostFtdcTraderApi->GetTradingDay());
    //ReqSettlementInfoConfirm();
  }
}


void CtpOrd::OnRspSettlementInfoConfirm(
  CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
  CThostFtdcRspInfoField *pRspInfo,
  int nRequestID,
  bool bIsLast) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRspSettlementInfoConfirm()", __FILENAME__);
  if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
    //ReqQryInstrument();
  }
}


void CtpOrd::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument,
                                CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRspQryInstrument()", __FILENAME__);
  if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
    //		ReqQryTradingAccount();
  }
}


void CtpOrd::OnRspQryTradingAccount(
  CThostFtdcTradingAccountField *pTradingAccount,
  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Balance=%f, Available=%f", __FILENAME__, pTradingAccount->Balance ,pTradingAccount->Available);
  if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
    //ReqQryInvestorPosition();
  }
}


void CtpOrd::OnRspQryInvestorPosition(
  CThostFtdcInvestorPositionField *pInvestorPosition,
  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRspQryInvestorPosition()", __FILENAME__);
  m_Logger->Write(StdStreamLogger::INFO,"%s: %s %s %s today pos=%d pos=%d open vol=%d openAmt=%f", __FILENAME__, pInvestorPosition->TradingDay,pInvestorPosition->PosiDirection,pInvestorPosition->InstrumentID,pInvestorPosition->TodayPosition, pInvestorPosition->Position, pInvestorPosition->OpenVolume,pInvestorPosition->OpenAmount);
  if (!IsErrorRspInfo(pRspInfo)) {		//ReqOrderInsert();

  }
}
void CtpOrd::OnRspQryInvestorPositionDetail(
  CThostFtdcInvestorPositionDetailField *pInvestorPosition,
  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRspQryInvestorPositionDetail()", __FILENAME__);
  m_Logger->Write(StdStreamLogger::INFO,"%s: %s %s %s %s TradeID=%s Volume=%d OpenPrice=%f", __FILENAME__, pInvestorPosition->TradingDay,pInvestorPosition->OpenDate,pInvestorPosition->Direction,pInvestorPosition->InstrumentID, pInvestorPosition->TradeID, pInvestorPosition->Volume, pInvestorPosition->OpenPrice);
  if (!IsErrorRspInfo(pRspInfo)) {		//ReqOrderInsert();

  }
}

void CtpOrd::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,
                              CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRspOrderInsert()", __FILENAME__);
  IsErrorRspInfo(pRspInfo);
  ATU_OTI_orderfeed_struct of;
  of.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  of.m_market = "CFFEX";
  of.m_instrument = string(pInputOrder->InstrumentID);
  of.m_order_id = getOrderID(string(pInputOrder->OrderRef));
  of.m_price = pInputOrder->LimitPrice;
  of.m_qty = pInputOrder->VolumeTotalOriginal;
  of.m_islast=0;
  //TODO: need to determine if it is open or close
  if(pInputOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Open){
    of.m_open_or_close = "open";
  }
  else if(pInputOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Close){
    of.m_open_or_close = "close";
  }
  else if(pInputOrder->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday){
    of.m_open_or_close = "close";
  }

  if (pInputOrder->Direction == THOST_FTDC_D_Buy) {
    of.m_buy_or_sell = 1;
  }
  if (pInputOrder->Direction == THOST_FTDC_D_Sell) {
    of.m_buy_or_sell = 2;
  }

  //Wrong order with qty_fileed = 0
  of.m_qty_filled = 0;

  //Wrong order will be deleted immediately
  of.m_deleted = 1;

  //OnRtnOder receive correct order message, OnRspOrderInsert receive wrong order message
  of.m_order_status = 1;
  of.m_error_description = string(pRspInfo->ErrorMsg);

  //real-time update
  of.m_source = 0;

  //TODO:stop order
  if(pInputOrder->OrderPriceType == THOST_FTDC_OPT_AnyPrice) {
    of.m_order_type = "market_order";
  }
  else if(pInputOrder->OrderPriceType == THOST_FTDC_OPT_LimitPrice){
    of.m_order_type = "limit_order";
  }

  of.m_order_validity = "N/A";

  notify_orderfeed(of);
}


void CtpOrd::OnRspOrderAction(
  CThostFtdcInputOrderActionField *pInputOrderAction,
  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

  //Do not believe order information attached in the response with Error.
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRspOrderAction()", __FILENAME__);
  IsErrorRspInfo(pRspInfo);
  ATU_OTI_orderfeed_struct of;
  of.m_islast=0;
  of.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  of.m_market = "CFFEX";
  of.m_instrument = string(pInputOrderAction->InstrumentID);
  of.m_order_id = getOrderID(string(pInputOrderAction->OrderRef));
  of.m_price = pInputOrderAction->LimitPrice;
  //cancel order failed, pInputOrderAction don't provide quantity
  of.m_qty = 0;
  //cancel order failed, pInputOrderAction don't provide open or close information
  of.m_open_or_close = "open";
  //cancel order failed, pInputOrderAction don't provide buy or sell information
  of.m_buy_or_sell = 1;

  //Wrong order with qty_fileed = 0
  of.m_qty_filled = 0;

  //cancel order got error
  of.m_deleted = 1; //changed to mark as deleted

  //OnRtnOder receive correct order message, OnRspOrderAction receive wrong order message
  of.m_order_status = 1;
  of.m_error_description = string(pRspInfo->ErrorMsg);

  //real-time update
  of.m_source = 0;

  //cancel order failed, pInputOrderAction don't provide order type information
  of.m_order_type = "N/A";

  of.m_order_validity = "N/A";
  notify_orderfeed(of);
}

void CtpOrd::OnRtnOrder(CThostFtdcOrderField *pOrder) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRtnOrder()", __FILENAME__);

  printCThostFtdcOrderField(pOrder);
  ATU_OTI_orderfeed_struct of;

  of.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  of.m_market = "CFFEX";
  of.m_instrument = string(pOrder->InstrumentID);
  of.m_order_id = getOrderID(string(pOrder->OrderRef));
  of.m_price = pOrder->LimitPrice;
  of.m_qty = pOrder->VolumeTotalOriginal;
  //TODO: need to determine if it is open or close
  of.m_open_or_close="NA";
  if(pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Open){
    of.m_open_or_close = "open";
  }
  else if(pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Close){
    of.m_open_or_close = "close";
  }
  else if(pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday){
    of.m_open_or_close = "close";
  }

  if (pOrder->Direction == THOST_FTDC_D_Buy) {
    of.m_buy_or_sell = 1;
  }
  if (pOrder->Direction == THOST_FTDC_D_Sell) {
    of.m_buy_or_sell = 2;
  }

  of.m_qty_filled = pOrder->VolumeTraded;

  if(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) of.m_deleted = 0;
  else  of.m_deleted = 1;
  if (of.m_qty==of.m_qty_filled) {
    of.m_deleted=1;
  }
  //OnRtnOder receive correct order message, OnRspOrderInsert receive wrong order message
  of.m_order_status = 0;
  of.m_error_description = "";
  if(pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected ) {
    of.m_order_status = 1;
    of.m_error_description = "Insert Order Rejected";
  }
  if(pOrder->OrderSubmitStatus == THOST_FTDC_OSS_CancelRejected ) {
    of.m_order_status = 1;
    of.m_error_description = "Cancel Order Rejected";
  }
  if(pOrder->OrderSubmitStatus == THOST_FTDC_OSS_ModifyRejected ) {
    of.m_order_status = 1;
    of.m_error_description = "Modify Order Rejected";
  }


  of.m_islast=0;
  //real-time update
  of.m_source = 0;

  //TODO:stop order
  if(pOrder->OrderPriceType == THOST_FTDC_OPT_AnyPrice) {
    of.m_order_type = "market_order";
  }
  else if(pOrder->OrderPriceType == THOST_FTDC_OPT_LimitPrice){
    of.m_order_type = "limit_order";
  }

  of.m_order_validity = "N/A";

  m_Logger->Write(StdStreamLogger::INFO,"%s: StatusMsg:%s", __FILENAME__,pOrder->StatusMsg);

  notify_orderfeed(of);

}

void CtpOrd::OnRtnTrade(CThostFtdcTradeField *pTrade) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRtnTrade()", __FILENAME__);
  //	cout << pTrade->InstrumentID << "," << pTrade->Direction << "," << pTrade->Price << "," << pTrade->Volume << "," << pTrade->TraderID << "," << pTrade->OrderLocalID << "," << pTrade->OrderRef << endl;

  ATU_OTI_tradefeed_struct tf;

  tf.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  tf.m_market="CFFEX";
  tf.m_instrument=string(pTrade->InstrumentID);
  tf.m_order_id=getOrderID(string(pTrade->OrderRef));
  tf.m_price=pTrade->Price;
  tf.m_qty=pTrade->Volume;
  tf.m_islast=0;
  if(pTrade->OffsetFlag == THOST_FTDC_OF_Open){
    tf.m_open_or_close="open";
  }
  else if(pTrade->OffsetFlag == THOST_FTDC_OF_Close){
    tf.m_open_or_close="close";
  }
  else if(pTrade->OffsetFlag == THOST_FTDC_OF_CloseToday){
    tf.m_open_or_close="close";
  }

  if(pTrade->Direction == THOST_FTDC_D_Buy){
    tf.m_buy_or_sell=1;
  }
  else if(pTrade->Direction == THOST_FTDC_D_Sell){
    tf.m_buy_or_sell=2;
  }

  if (pTrade->Direction==THOST_FTDC_D_Sell) {
    tf.m_buy_or_sell=2;
  }
  tf.m_trade_id=pTrade->TradeID;
  boost::trim(tf.m_trade_id);
  tf.m_portfolio_name="N/A";

  char _tradeTime_c[9],_tradeTime_l[9];
  strcpy(_tradeTime_c, pTrade->TradeTime);
  _tradeTime_l[0] = _tradeTime_c[0],_tradeTime_l[1] = _tradeTime_c[1],_tradeTime_l[2] = _tradeTime_c[3];
  _tradeTime_l[3] = _tradeTime_c[5],_tradeTime_l[4] = _tradeTime_c[7],_tradeTime_l[5] = _tradeTime_c[8];
  _tradeTime_l[6] = 0;
  string _tradeDate = pTrade->TradeDate, _tradeTime = _tradeTime_l;
  tf.m_trade_timestamp=_tradeDate+"_"+_tradeTime_l;
  tf.m_source=0;
  notify_tradefeed(tf);
}

void CtpOrd::OnFrontDisconnected(int nReason) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Call OnFrontDisconnected: Reason = %d", __FILENAME__,nReason);
}

void CtpOrd::OnHeartBeatWarning(int nTimeLapse) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Call OnHeartBeatWarning. nTimerLapse=%d", __FILENAME__,nTimeLapse);
}

void CtpOrd::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  m_Logger->Write(StdStreamLogger::INFO,"%s: Called OnRspError()", __FILENAME__);
  IsErrorRspInfo(pRspInfo);
  //TODO:: should call notify_errorfeed
  //TODO:: or should call orderfeed with error_status marked..
}

bool CtpOrd::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo) {
  bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (bResult)
    m_Logger->Write(StdStreamLogger::INFO,"%s: ErrorID=%d, ErrorMsg=%s", __FILENAME__,pRspInfo->ErrorID,pRspInfo->ErrorMsg);
  return bResult;
}

void CtpOrd::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){

  //executed or canceled

  ATU_OTI_orderfeed_struct of;
  of.m_source = 1;
  if (bIsLast || pOrder==NULL) {
    //			cout << "**************************************************************************************" << endl;
  }
  if (pOrder==NULL) {
    if (bIsLast) {
      of.m_islast=1;
      notify_orderfeed(of);
    }
    return;
  }

  m_Logger->Write(StdStreamLogger::INFO,"%s: OrderRef=%s", __FILENAME__,pOrder->OrderRef);

  if(pOrder->OrderStatus == THOST_FTDC_OST_AllTraded || pOrder->OrderStatus == THOST_FTDC_OST_Canceled || pOrder->OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing || pOrder->OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing){
    m_Logger->Write(StdStreamLogger::INFO,"%s: Order is executed or canceled", __FILENAME__);

    if (bIsLast) {
      of.m_islast=1;
      notify_orderfeed(of);
    }
    return ;
  }

  of.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  of.m_market = "CFFEX";
  of.m_instrument = string(pOrder->InstrumentID);
  of.m_order_id = getOrderID(string(pOrder->OrderRef));
  of.m_price = pOrder->LimitPrice;
  of.m_qty = pOrder->VolumeTotalOriginal;
  of.m_open_or_close="N/A";
  if(pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Open){
    of.m_open_or_close = "open";
  }
  else if(pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Close){
    of.m_open_or_close = "close";
  }
  else if(pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday){
    of.m_open_or_close = "close";
  }

  if (pOrder->Direction == THOST_FTDC_D_Buy) {
    of.m_buy_or_sell = 1;
  }
  if (pOrder->Direction == THOST_FTDC_D_Sell) {
    of.m_buy_or_sell = 2;
  }

  of.m_qty_filled = pOrder->VolumeTraded;

  //working order should not be labeled as "deleted"
  of.m_deleted = 0;
  of.m_islast = 0;
  //TODO: Checking Status
  of.m_order_status = 0;
  of.m_error_description = "";

  //message reply
  of.m_source = 1;

  //TODO:stop order
  if(pOrder->OrderPriceType == THOST_FTDC_OPT_AnyPrice) {
    of.m_order_type = "market_order";
  }
  else if(pOrder->OrderPriceType == THOST_FTDC_OPT_LimitPrice){
    of.m_order_type = "limit_order";
  }

  of.m_order_validity = "N/A";
  notify_orderfeed(of);
  if (bIsLast) {
    of.m_islast=1;
    notify_orderfeed(of);
  }

}
void CtpOrd::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
  ATU_OTI_tradefeed_struct tf;
  if (pTrade==NULL) {
    tf.m_source=1;
    tf.m_islast=1;
    notify_tradefeed(tf);
    return;
  }
  tf.m_timestamp = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  tf.m_market="CFFEX";
  tf.m_instrument=string(pTrade->InstrumentID);
  tf.m_order_id=getOrderID(string(pTrade->OrderRef));
  tf.m_price=pTrade->Price;
  tf.m_qty=pTrade->Volume;
  tf.m_islast=0;
  tf.m_open_or_close="NA";
  if(pTrade->OffsetFlag == THOST_FTDC_OF_Open){
    tf.m_open_or_close="open";
  }
  else if(pTrade->OffsetFlag == THOST_FTDC_OF_Close){
    tf.m_open_or_close="close";
  }
  else if(pTrade->OffsetFlag == THOST_FTDC_OF_CloseToday){
    tf.m_open_or_close="close";
  }

  if(pTrade->Direction == THOST_FTDC_D_Buy){
    tf.m_buy_or_sell=1;
  }
  else if(pTrade->Direction == THOST_FTDC_D_Sell){
    tf.m_buy_or_sell=2;
  }

  if (pTrade->Direction==THOST_FTDC_D_Sell) {
    tf.m_buy_or_sell=2;
  }
  tf.m_trade_id=pTrade->TradeID;
  boost::trim(tf.m_trade_id);
  tf.m_portfolio_name="N/A";

  char _tradeTime_c[9],_tradeTime_l[9];
  strcpy(_tradeTime_c, pTrade->TradeTime);
  _tradeTime_l[0] = _tradeTime_c[0],_tradeTime_l[1] = _tradeTime_c[1],_tradeTime_l[2] = _tradeTime_c[3];
  _tradeTime_l[3] = _tradeTime_c[5],_tradeTime_l[4] = _tradeTime_c[7],_tradeTime_l[5] = _tradeTime_c[8];
  _tradeTime_l[6] = 0;
  string _tradeDate = pTrade->TradeDate, _tradeTime = _tradeTime_l;
  tf.m_trade_timestamp=_tradeDate+"_"+_tradeTime_l;
  tf.m_source=1;

  notify_tradefeed(tf);
  if (bIsLast) {
    tf.m_islast=1;
    notify_tradefeed(tf);
  }
}

string CtpOrd::createOrderRef(string orderid) {
  const string tmpstr = SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff();
  vector<string> fieldvec;
  boost::split(fieldvec, tmpstr, boost::is_any_of("_"));
  string currentOrderTimestamp=fieldvec[1];
  boost::unique_lock<boost::recursive_mutex> lock(m_orderRefMutex);
  if (m_lastOrderTimestamp.compare(currentOrderTimestamp)==0) {
    m_orderRefCount++;
  } else {
    m_orderRefCount=0;
    m_lastOrderTimestamp=currentOrderTimestamp;
  }
  char temp_orderId[15];
  sprintf(temp_orderId,"%s%06d",currentOrderTimestamp.c_str(),m_orderRefCount);
  //	string orderref=currentOrderTimestamp+"_"+boost::lexical_cast<string>(m_orderRefCount);
  string orderref = string(temp_orderId);
  m_order_idToOrderRef[orderid]=orderref;
  m_OrderRefToorder_id[orderref]=orderid;
  return orderref;
}
string CtpOrd::getOrderRef(string orderid) {
  boost::unique_lock<boost::recursive_mutex> lock(m_orderRefMutex);
  if (m_order_idToOrderRef.find(orderid)==m_order_idToOrderRef.end()) {
    return orderid;
  } else {
    return m_order_idToOrderRef[orderid];
  }
}
string CtpOrd::getOrderID(string orderref) {
  boost::unique_lock<boost::recursive_mutex> lock(m_orderRefMutex);
  if (m_OrderRefToorder_id.find(orderref)==m_OrderRefToorder_id.end()) {
    return orderref;
  } else {
    return m_OrderRefToorder_id[orderref];
  }
}

void CtpOrd::printCThostFtdcOrderField(CThostFtdcOrderField *pOrder)
{
  ostringstream oo;

  oo<<"BrokerID("<<pOrder->BrokerID<<"); ";
  oo<<"InvestorID("<<pOrder->InvestorID<<"); ";
  oo<<"InstrumentID("<<pOrder->InstrumentID<<"); ";
  oo<<"OrderRef("<<pOrder->OrderRef<<"); ";
  oo<<"UserID("<<pOrder->UserID<<"); ";
  oo<<"OrderPriceType("<<pOrder->OrderPriceType<<"); ";
  oo<<"Direction("<<pOrder->Direction<<"); ";
  oo<<"CombOffsetFlag("<<pOrder->CombOffsetFlag<<"); ";
  oo<<"CombHedgeFlag("<<pOrder->CombHedgeFlag<<"); ";
  oo<<"LimitPrice("<<pOrder->LimitPrice<<"); ";
  oo<<"VolumeTotalOriginal("<<pOrder->VolumeTotalOriginal<<"); ";
  oo<<"TimeCondition("<<pOrder->TimeCondition<<"); ";
  oo<<"GTDDate("<<pOrder->GTDDate<<"); ";
  oo<<"VolumeCondition("<<pOrder->VolumeCondition<<"); ";
  oo<<"MinVolume("<<pOrder->MinVolume<<"); ";
  oo<<"ContingentCondition("<<pOrder->ContingentCondition<<"); ";
  oo<<"StopPrice("<<pOrder->StopPrice<<"); ";
  oo<<"ForceCloseReason("<<pOrder->ForceCloseReason<<"); ";
  oo<<"IsAutoSuspend("<<pOrder->IsAutoSuspend<<"); ";
  oo<<"BusinessUnit("<<pOrder->BusinessUnit<<"); ";
  oo<<"RequestID("<<pOrder->RequestID<<"); ";
  oo<<"OrderLocalID("<<pOrder->OrderLocalID<<"); ";
  oo<<"ExchangeID("<<pOrder->ExchangeID<<"); ";
  oo<<"ParticipantID("<<pOrder->ParticipantID<<"); ";
  oo<<"ClientID("<<pOrder->ClientID<<"); ";
  oo<<"ExchangeInstID("<<pOrder->ExchangeInstID<<"); ";
  oo<<"TraderID("<<pOrder->TraderID<<"); ";
  oo<<"InstallID("<<pOrder->InstallID<<"); ";
  oo<<"OrderSubmitStatus("<<pOrder->OrderSubmitStatus<<"); ";
  oo<<"NotifySequence("<<pOrder->NotifySequence<<"); ";
  oo<<"TradingDay("<<pOrder->TradingDay<<"); ";
  oo<<"SettlementID("<<pOrder->SettlementID<<"); ";
  oo<<"OrderSysID("<<pOrder->OrderSysID<<"); ";
  oo<<"OrderSource("<<pOrder->OrderSource<<"); ";
  oo<<"OrderStatus("<<pOrder->OrderStatus<<"); ";
  oo<<"OrderType("<<pOrder->OrderType<<"); ";
  oo<<"VolumeTraded("<<pOrder->VolumeTraded<<"); ";
  oo<<"VolumeTotal("<<pOrder->VolumeTotal<<"); ";
  oo<<"InsertDate("<<pOrder->InsertDate<<"); ";
  oo<<"InsertTime("<<pOrder->InsertTime<<"); ";
  oo<<"ActiveTime("<<pOrder->ActiveTime<<"); ";
  oo<<"SuspendTime("<<pOrder->SuspendTime<<"); ";
  oo<<"UpdateTime("<<pOrder->UpdateTime<<"); ";
  oo<<"CancelTime("<<pOrder->CancelTime<<"); ";
  oo<<"ActiveTraderID("<<pOrder->ActiveTraderID<<"); ";
  oo<<"ClearingPartID("<<pOrder->ClearingPartID<<"); ";
  oo<<"SequenceNo("<<pOrder->SequenceNo<<"); ";
  oo<<"FrontID("<<pOrder->FrontID<<"); ";
  oo<<"SessionID("<<pOrder->SessionID<<"); ";
  oo<<"UserProductInfo("<<pOrder->UserProductInfo<<"); ";
  oo<<"StatusMsg("<<pOrder->StatusMsg<<"); ";
  oo<<"UserForceClose("<<pOrder->UserForceClose<<"); ";
  oo<<"ActiveUserID("<<pOrder->ActiveUserID<<"); ";
  oo<<"BrokerOrderSeq("<<pOrder->BrokerOrderSeq<<"); ";
  oo<<"RelativeOrderSysID("<<pOrder->RelativeOrderSysID<<"); ";
  oo<<"ZCETotalTradedVolume("<<pOrder->ZCETotalTradedVolume<<"); ";
  oo<<"IsSwapOrder("<<pOrder->IsSwapOrder<<"); ";
  oo<<endl;
  oo<<endl;

  m_Logger->Write(StdStreamLogger::INFO,"%s: printCThostFtdcOrderField: %s", __FILENAME__, oo.str().c_str());
}

void CtpOrd::notify_tradefeed(const ATU_OTI_tradefeed_struct & tf)
{
  BYTE * pb = m_ecbOrd->GetWritingPtr();
  strcpy((char*)pb,ATU_OTI_tradefeed_struct::ToString(tf).c_str());
  m_ecbOrd->PushBack(strlen((char*)pb));
}

void CtpOrd::notify_orderfeed(const ATU_OTI_orderfeed_struct & of)
{
  BYTE * pb = m_ecbOrd->GetWritingPtr();
  strcpy((char*)pb,ATU_OTI_orderfeed_struct::ToString(of).c_str());
  m_ecbOrd->PushBack(strlen((char*)pb));
}

