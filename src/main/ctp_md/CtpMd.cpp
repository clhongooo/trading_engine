#include "CtpMd.h"

CtpMd::CtpMd() : m_pMdApi(NULL),m_p_ctp_lib_handle(NULL),m_DataFolder(""),m_DataMode(DM_CSV),m_WriteDataToFile(false),m_connection_string("")
{
  m_Logger = StdStreamLogger::Instance();
  m_subscribedSymbols.clear();
}
//--------------------------------------------------
// setters
//--------------------------------------------------
void CtpMd::setDataFolder(const string & df)
{
  m_DataFolder = df;
}
void CtpMd::setDataMode(const string & dm)
{
  string dm2 = dm;
  boost::algorithm::to_lower(dm2);
  if (dm2 == "binary") m_DataMode = DM_BINARY;
  else if (dm2 == "csv") m_DataMode = DM_CSV;
  else
  {
    m_Logger->Write(StdStreamLogger::ERROR,"Unknown data mode.");
    exit(0);
  }
}
void CtpMd::setWriteDataToFile(const string & wdtf)
{
  string wdtf2 = wdtf;
  boost::algorithm::to_lower(wdtf2);
  if (wdtf2 == "true" || wdtf2 == "t" || wdtf2 == "yes" || wdtf2 == "y") m_WriteDataToFile = true;
  else m_WriteDataToFile = false;
}
void CtpMd::setConnectString(const string & connStr)
{
  m_connection_string = connStr;
}
void CtpMd::setBrokerID(const string & bid)
{
  strcpy(m_broker_id,(char*)bid.c_str());
}
void CtpMd::setInvestorID(const string & iid)
{
  strcpy(m_investor_id,(char*)iid.c_str());
}
void CtpMd::setPassword(const string & pwd)
{
  strcpy(m_password,(char*)pwd.c_str());
}
//--------------------------------------------------
// thread run
//--------------------------------------------------
void CtpMd::run()
{
  m_p_ctp_lib_handle = dlopen("thostmduserapi.so",RTLD_NOW);
  if (m_p_ctp_lib_handle == NULL)
  {
    m_Logger->Write(StdStreamLogger::ERROR,"CTP libthostmduserapi.so NOT loaded");
    return;
  } else
  {
    m_Logger->Write(StdStreamLogger::INFO,"CTP libthostmduserapi.so loaded");
  }
  typedef CThostFtdcMdApi* (*CreateFtdcMdApiPtr)(const char *, bool, bool);
  CreateFtdcMdApiPtr CreateFtdcMdApi = (CreateFtdcMdApiPtr)dlsym(m_p_ctp_lib_handle,"_ZN15CThostFtdcMdApi15CreateFtdcMdApiEPKcbb");

  m_pMdApi = CreateFtdcMdApi("",true,true);
  m_pMdApi->RegisterSpi((CThostFtdcMdSpi*) this);
  m_pMdApi->RegisterFront((char*)m_connection_string.c_str());
  m_pMdApi->Init();
  if (m_pMdApi!=NULL)
  {
    m_pMdApi->Join();
  }
}

//--------------------------------------------------
// callback
//--------------------------------------------------
void CtpMd::OnFrontConnected()
{
  m_Logger->Write(StdStreamLogger::INFO,"Front is connected");
  ReqUserLogin();
}

void CtpMd::ReqUserLogin()
{
  CThostFtdcReqUserLoginField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, m_broker_id);
  strcpy(req.UserID, m_investor_id);
  strcpy(req.Password, m_password);
  int iResult = m_pMdApi->ReqUserLogin(&req, ++m_iRequestID);
  m_Logger->Write(StdStreamLogger::CRITICAL,"User Login is %s",(iResult == 0) ? "OK" : "Fail");

  for(set<string>::iterator iter = m_subscribedSymbols.begin(); iter != m_subscribedSymbols.end(); iter++)
  {
    char * _ppInstrumentID[2000]; int nCount = 0;
    char _pInst[100];
    strcpy(_pInst, (*iter).c_str());
    _ppInstrumentID[nCount++] = _pInst;
    SubscribeMarketData(_ppInstrumentID,nCount);
  }

}

void CtpMd::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  if ((pRspInfo) && (pRspInfo->ErrorID != 0))
    m_Logger->Write(StdStreamLogger::ERROR,"ErrorID=%s, ErrorMsg=%s",pRspInfo->ErrorID,pRspInfo->ErrorMsg);
}

void CtpMd::OnFrontDisconnected(int nReason)
{
  m_Logger->Write(StdStreamLogger::EMERGENCY,"Front is disconnected!");
}

void CtpMd::OnHeartBeatWarning(int nTimeLapse)
{
}

void CtpMd::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
  if (pRspInfo->ErrorID != 0)
  {
    char _errorID[20];
    sprintf(_errorID,"%d",pRspInfo->ErrorID);
  }

}

bool CtpMd::on_process_subscription(const ATU_MDI_subscription_struct &s)
{
  char * _ppInstrumentID[2000]; int nCount = 0;
  memset(_ppInstrumentID, 0 , sizeof(_ppInstrumentID));
  char _pInst[100];
  strcpy(_pInst, s.m_feedcode.c_str());
  m_subscribedSymbols.insert(s.m_feedcode);
  _ppInstrumentID[nCount++] = _pInst;
  SubscribeMarketData(_ppInstrumentID,nCount);
  return true;
}

void CtpMd::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpMd::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
  if (m_DataMode == DM_CSV)
  {
    ATU_MDI_marketfeed_struct mfs;
    string tradingDay = pDepthMarketData->TradingDay;
    string updateTime = pDepthMarketData->UpdateTime;
    string updateMillisec = boost::lexical_cast<string>(pDepthMarketData->UpdateMillisec);
    boost::replace_all(updateTime, ":", "");
    char tmpstr[10];
    sprintf(tmpstr,"%06d",pDepthMarketData->UpdateMillisec*1000);
    mfs.m_timestamp     = tradingDay+"_"+updateTime+"_"+string(tmpstr);
    mfs.m_feedcode      = pDepthMarketData->InstrumentID;
    mfs.m_traded_price  = (pDepthMarketData->LastPrice == DBL_MAX) ? 999999 : pDepthMarketData->LastPrice;
    mfs.m_traded_volume = (pDepthMarketData->Volume == DBL_MAX) ? 999999 : pDepthMarketData->Volume;

    mfs.m_bid_price_1   = (pDepthMarketData->BidPrice1  == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice1;
    mfs.m_bid_price_2   = (pDepthMarketData->BidPrice2  == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice2;
    mfs.m_bid_price_3   = (pDepthMarketData->BidPrice3  == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice3;
    mfs.m_bid_price_4   = (pDepthMarketData->BidPrice4  == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice4;
    mfs.m_bid_price_5   = (pDepthMarketData->BidPrice5  == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice5;
    mfs.m_bid_volume_1  = (pDepthMarketData->BidVolume1 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume1;
    mfs.m_bid_volume_2  = (pDepthMarketData->BidVolume2 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume2;
    mfs.m_bid_volume_3  = (pDepthMarketData->BidVolume3 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume3;
    mfs.m_bid_volume_4  = (pDepthMarketData->BidVolume4 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume4;
    mfs.m_bid_volume_5  = (pDepthMarketData->BidVolume5 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume5;

    mfs.m_ask_price_1   = (pDepthMarketData->AskPrice1  == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice1;
    mfs.m_ask_price_2   = (pDepthMarketData->AskPrice2  == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice2;
    mfs.m_ask_price_3   = (pDepthMarketData->AskPrice3  == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice3;
    mfs.m_ask_price_4   = (pDepthMarketData->AskPrice4  == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice4;
    mfs.m_ask_price_5   = (pDepthMarketData->AskPrice5  == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice5;
    mfs.m_ask_volume_1  = (pDepthMarketData->AskVolume1 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume1;
    mfs.m_ask_volume_2  = (pDepthMarketData->AskVolume2 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume2;
    mfs.m_ask_volume_3  = (pDepthMarketData->AskVolume3 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume3;
    mfs.m_ask_volume_4  = (pDepthMarketData->AskVolume4 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume4;
    mfs.m_ask_volume_5  = (pDepthMarketData->AskVolume5 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume5;

    if (m_WriteDataToFile)
      m_Logger->Write(StdStreamLogger::DEBUG,"%s.", ATU_MDI_marketfeed_struct::ToString(mfs).c_str());

    notify_marketfeed(mfs);
  }
}

//--------------------------------------------------
// receive from strategy engine
//--------------------------------------------------
int CtpMd::SubscribeMarketData(char *ppInstrumentID[], int nCount)
{
  for(int i = 0 ; i < nCount; i++)
  {
    m_Logger->Write(StdStreamLogger::INFO,"Subscribe %s.",ppInstrumentID[i]);
  }
  int ret = m_pMdApi->SubscribeMarketData(ppInstrumentID, nCount);
  return ret;
}

void CtpMd::notify_marketfeed(const ATU_MDI_marketfeed_struct &s)
{
}

void CtpMd::Md_api_release()
{
  if (m_pMdApi != NULL)
  {
    m_pMdApi->RegisterSpi(NULL);
    m_pMdApi->Release();
    m_pMdApi = NULL;
  }
}
