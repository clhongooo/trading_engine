#ifndef ATU_CTP_MDI_H_
#define ATU_CTP_MDI_H_
#include "ATU_Abstract_MDI.h"
//#include "ATU_TCP_MDI_string_handler.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include "ATU_ErrorMsgStruct.h"
#include <map>
#include <dlfcn.h>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;
#include "ThostFtdcMdApi.h"

class CtpMd : public CThostFtdcMdSpi{
  public:
    CtpMd();
    virtual void init();
    virtual ~CtpMd();


    virtual bool notify_marketfeed(const ATU_MDI_marketfeed_struct &s);
    virtual bool on_process_subscription(const ATU_MDI_subscription_struct &s);
    virtual void detach();

    //--------------------------------------------------
    // CTP
    //--------------------------------------------------
    virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo,int nRequestID, bool bIsLast);
    virtual void OnFrontDisconnected(int nReason);
    virtual void OnHeartBeatWarning(int nTimeLapse);
    virtual void OnFrontConnected();
    virtual void ReqUserLogin();
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount);
    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData);
    //--------------------------------------------------

    void Md_api_release();

    virtual void setConnectString(string address);

    int m_iRequestID;

  private:
    void *m_ctp_lib_handle;
    boost::thread *m_init_and_run_thread;

    boost::scoped_ptr<CThostFtdcMdApi> m_pMdApi;
    string m_server_address;
    TThostFtdcBrokerIDType m_broker_id;
    TThostFtdcInvestorIDType m_investor_id;
    TThostFtdcPasswordType m_password;
    TThostFtdcFrontIDType m_front_id;
    TThostFtdcSessionIDType m_session_id;
    bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);

    //--------------------------------------------------
    set<string> m_subscribedSymbols;

};

#endif



#include "CtpMd.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
using namespace boost::filesystem;
using namespace boost::interprocess;


CtpMd::CtpMd() : m_server_address("") {
  m_subscribedSymbols.clear();
}
void CtpMd::detach(){
  m_init_and_run_thread=new boost::thread(boost::bind(&CtpMd::init,this));
}
void CtpMd::setConnectString(string address) {
  // tcp://222.66.166.147:41214
  strcpy(m_server_address,address.c_str());
}
void CtpMd::init(){
  m_ctp_lib_handle=dlopen("libthostmduserapi.so",RTLD_NOW);
  if (m_ctp_lib_handle!=NULL) {
    addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"CTP mdapi loaded","");
  } else {
    addLog(__LOGSOURCE__,ATU_logfeed_struct::ERROR,"CTP mdapi NOT loaded","");
  }
  typedef CThostFtdcMdApi* (*CreateFtdcMdApiPtr)(const char *, bool, bool);
  CreateFtdcMdApiPtr CreateFtdcMdApi= (CreateFtdcMdApiPtr)dlsym(m_ctp_lib_handle,"_ZN15CThostFtdcMdApi15CreateFtdcMdApiEPKcbb");

  m_pMdApi=CreateFtdcMdApi("",true,true);

  //	    strcpy(m_server_address,"tcp://180.166.165.179:41213");
  //		strcpy(m_broker_id,"1013");
  //		strcpy(m_investor_id,"00000062");
  //		strcpy(m_password,"834591");
  m_pMdApi->RegisterSpi((CThostFtdcMdSpi*) this);
  m_pMdApi->RegisterFront(m_server_address);
  m_pMdApi->Init();
  if (m_pMdApi!=NULL) {
    m_pMdApi->Join();
  }
}

CtpMd::~CtpMd(){

}

void CtpMd::OnFrontConnected() {
  addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Front is connected","");
  ReqUserLogin();
}

void CtpMd::ReqUserLogin(){
  CThostFtdcReqUserLoginField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, m_broker_id);
  strcpy(req.UserID, m_investor_id);
  strcpy(req.Password, m_password);
  int iResult = m_pMdApi->ReqUserLogin(&req, ++m_iRequestID);
  addLog(__LOGSOURCE__,ATU_logfeed_struct::CRITICAL,"User Login is %s","s",(iResult == 0) ? "OK" : "Fail");

  for(set<string>::iterator iter = m_subscribedSymbols.begin(); iter != m_subscribedSymbols.end(); iter++){
    char * _ppInstrumentID[2000]; int nCount = 0;
    char _pInst[100];
    strcpy(_pInst, (*iter).c_str());
    _ppInstrumentID[nCount++] = _pInst;
    SubscribeMarketData(_ppInstrumentID,nCount);
  }

}

bool CtpMd::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
  bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if (bResult)
    addLog(__LOGSOURCE__,ATU_logfeed_struct::ERROR,"ErrorID=%s, ErrorMsg=%s","ss",pRspInfo->ErrorID,pRspInfo->ErrorMsg);
  return bResult;
}

void CtpMd::OnRspError(CThostFtdcRspInfoField* pRspInfo,
                       int nRequestID, bool bIsLast){
  addLog(__LOGSOURCE__,ATU_logfeed_struct::ALERT,"Error is Found!","");
  IsErrorRspInfo(pRspInfo);
}

void CtpMd::OnFrontDisconnected(int nReason){
  addLog(__LOGSOURCE__,ATU_logfeed_struct::EMERGENCY,"Front is disconnected!","");
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
    addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Subscribe %s.","s",ppInstrumentID[i]);
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

bool CtpMd::notify_marketfeed(const ATU_MDI_marketfeed_struct &s){
  if (m_marketfeed_call_back_func!=NULL) {
    return (*m_marketfeed_call_back_func)(s);
  } else {
    return false;
  }
}

void CtpMd::Md_api_release(){
  if (m_pMdApi != NULL) {
    m_pMdApi->RegisterSpi(NULL);
    m_pMdApi->Release();
    m_pMdApi.reset(NULL);
  }
}

