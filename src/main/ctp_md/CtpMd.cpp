#include "CtpMd.h"

CtpMd::CtpMd() : m_connection_string("") {
  m_Logger = StdStreamLogger::Instance();

  //--------------------------------------------------
  // from config
  //--------------------------------------------------
  //	  strcpy(m_connection_string,"tcp://180.166.165.179:41213");
  //		strcpy(m_broker_id,"1013");
  //		strcpy(m_investor_id,"00000062");
  //		strcpy(m_password,"834591");
  m_subscribedSymbols.clear();
}
void CtpMd::detach(){
  m_p_init_and_run_thread.reset(new boost::thread(boost::bind(&CtpMd::init,this)));
}
void CtpMd::setConnectString(const string & connStr) {
  m_connection_string = connStr;
}
void CtpMd::init(){
  m_p_ctp_lib_handle = dlopen("libthostmduserapi.so",RTLD_NOW);
  if (m_p_ctp_lib_handle == NULL) {
    m_Logger->Write(StdStreamLogger::INFO,"CTP libthostmduserapi.so loaded");
  } else {
    m_Logger->Write(StdStreamLogger::ERROR,"CTP libthostmduserapi.so NOT loaded");
    return;
  }
  typedef CThostFtdcMdApi* (*CreateFtdcMdApiPtr)(const char *, bool, bool);
  CreateFtdcMdApiPtr CreateFtdcMdApi = (CreateFtdcMdApiPtr)dlsym(m_p_ctp_lib_handle,"_ZN15CThostFtdcMdApi15CreateFtdcMdApiEPKcbb");

  m_pMdApi = CreateFtdcMdApi("",true,true);
  m_pMdApi->RegisterSpi((CThostFtdcMdSpi*) this);

  char m_connection_string_2[64];
  strcpy(m_connection_string_2, m_connection_string.c_str());

  m_pMdApi->RegisterFront(m_connection_string_2);
  m_pMdApi->Init();
  if (m_pMdApi!=NULL) {
    m_pMdApi->Join();
  }
}

void CtpMd::OnFrontConnected() {
  m_Logger->Write(StdStreamLogger::INFO,"Front is connected");
  ReqUserLogin();
}

void CtpMd::ReqUserLogin(){
  CThostFtdcReqUserLoginField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, m_broker_id);
  strcpy(req.UserID, m_investor_id);
  strcpy(req.Password, m_password);
  int iResult = m_pMdApi->ReqUserLogin(&req, ++m_iRequestID);
  m_Logger->Write(StdStreamLogger::CRITICAL,"User Login is %s",(iResult == 0) ? "OK" : "Fail");

  for(set<string>::iterator iter = m_subscribedSymbols.begin(); iter != m_subscribedSymbols.end(); iter++){
    char * _ppInstrumentID[2000]; int nCount = 0;
    char _pInst[100];
    strcpy(_pInst, (*iter).c_str());
    _ppInstrumentID[nCount++] = _pInst;
    SubscribeMarketData(_ppInstrumentID,nCount);
  }

}

void CtpMd::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
  if ((pRspInfo) && (pRspInfo->ErrorID != 0))
    m_Logger->Write(StdStreamLogger::ERROR,"ErrorID=%s, ErrorMsg=%s",pRspInfo->ErrorID,pRspInfo->ErrorMsg);
}

void CtpMd::OnFrontDisconnected(int nReason){
  m_Logger->Write(StdStreamLogger::EMERGENCY,"Front is disconnected!");
}

void CtpMd::OnHeartBeatWarning(int nTimeLapse){
}

void CtpMd::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
  if (pRspInfo->ErrorID != 0) {
    char _errorID[20];
    sprintf(_errorID,"%d",pRspInfo->ErrorID);
  }

}

bool CtpMd::on_process_subscription(const ATU_MDI_subscription_struct &s) {
  char * _ppInstrumentID[2000]; int nCount = 0;
  memset(_ppInstrumentID, 0 , sizeof(_ppInstrumentID));
  char _pInst[100];
  strcpy(_pInst, s.m_feedcode.c_str());
  m_subscribedSymbols.insert(s.m_feedcode);
  _ppInstrumentID[nCount++] = _pInst;
  SubscribeMarketData(_ppInstrumentID,nCount);
  return true;
}

int CtpMd::SubscribeMarketData(char *ppInstrumentID[], int nCount){
  for(int i = 0 ; i < nCount; i++) {
    m_Logger->Write(StdStreamLogger::INFO,"Subscribe %s.",ppInstrumentID[i]);
  }
  int ret = m_pMdApi->SubscribeMarketData(ppInstrumentID, nCount);
  return ret;
}

void CtpMd::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast){

}

void CtpMd::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
  ATU_MDI_marketfeed_struct mf;
  string tradingDay = pDepthMarketData->TradingDay;
  string updateTime = pDepthMarketData->UpdateTime;
  string updateMillisec = boost::lexical_cast<string>(pDepthMarketData->UpdateMillisec);
  boost::replace_all(updateTime, ":", "");
  char tmpstr[10];
  sprintf(tmpstr,"%06d",pDepthMarketData->UpdateMillisec*1000);
  mf.m_timestamp = tradingDay+"_"+updateTime+"_"+string(tmpstr);
  mf.m_feedcode = pDepthMarketData->InstrumentID;
  mf.m_traded_price = (pDepthMarketData->LastPrice == DBL_MAX) ? 999999 : pDepthMarketData->LastPrice;
  mf.m_traded_volume = (pDepthMarketData->Volume == DBL_MAX) ? 999999 : pDepthMarketData->Volume;
  mf.m_ask_price_1 = (pDepthMarketData->AskPrice1 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice1;
  mf.m_ask_volume_1 = (pDepthMarketData->AskVolume1 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume1;
  mf.m_ask_price_2 = (pDepthMarketData->AskPrice2 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice2;
  mf.m_ask_volume_2 = (pDepthMarketData->AskVolume2 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume2;
  mf.m_ask_price_3 = (pDepthMarketData->AskPrice3 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice3;
  mf.m_ask_volume_3 = (pDepthMarketData->AskVolume3 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume3;
  mf.m_ask_price_4 = (pDepthMarketData->AskPrice4 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice4;
  mf.m_ask_volume_4 = (pDepthMarketData->AskVolume4 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume4;
  mf.m_ask_price_5 = (pDepthMarketData->AskPrice5 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice5;
  mf.m_ask_volume_5 = (pDepthMarketData->AskVolume5 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume5;

  mf.m_bid_price_1 = (pDepthMarketData->BidPrice1 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice1;
  mf.m_bid_volume_1 = (pDepthMarketData->BidVolume1 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume1;
  mf.m_bid_price_2 = (pDepthMarketData->BidPrice2 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice2;
  mf.m_bid_volume_2 = (pDepthMarketData->BidVolume2 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume2;
  mf.m_bid_price_3 = (pDepthMarketData->BidPrice3 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice3;
  mf.m_bid_volume_3 = (pDepthMarketData->BidVolume3 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume3;
  mf.m_bid_price_4 = (pDepthMarketData->BidPrice4 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice4;
  mf.m_bid_volume_4 = (pDepthMarketData->BidVolume4 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume4;
  mf.m_bid_price_5 = (pDepthMarketData->BidPrice5 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice5;
  mf.m_bid_volume_5 = (pDepthMarketData->BidVolume5 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume5;
  notify_marketfeed(mf);
}

void CtpMd::notify_marketfeed(const ATU_MDI_marketfeed_struct &s){
}
void CtpMd::Md_api_release(){
  if (m_pMdApi != NULL) {
    m_pMdApi->RegisterSpi(NULL);
    m_pMdApi->Release();
    m_pMdApi = NULL;
  }
}
