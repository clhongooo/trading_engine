#include "CtpMd.h"

CtpMd::CtpMd() : m_pCThostFtdcMdApi(NULL),m_p_ctp_lib_handle(NULL),m_DataFolder(""),m_WriteDataToFile(false),m_connection_string(""),m_WriteDataToFile_CallBackFunc(NULL)
{
  m_Logger = StdStreamLogger::Instance();
  m_subscribedSymbols.clear();
  m_WriteDataToFile_CallBackFunc = new WriteDataToFile_CallBackFunc(boost::bind(&CtpMd::DoNotWriteDataToFile,this,_1));
}

void CtpMd::ReadConfig(const string & sConfigPath)
{
  m_Logger = StdStreamLogger::Instance();
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(sConfigPath, pt);

  string sLogLevel = STool::Trim(pt.get<std::string>("General.LogLevel"));
  if      (sLogLevel == "EMERGENCY") { m_Logger->SetLogLevel(StdStreamLogger::EMERGENCY); }
  else if (sLogLevel == "ALERT")     { m_Logger->SetLogLevel(StdStreamLogger::ALERT);     }
  else if (sLogLevel == "CRITICAL")  { m_Logger->SetLogLevel(StdStreamLogger::CRITICAL);  }
  else if (sLogLevel == "ERROR")     { m_Logger->SetLogLevel(StdStreamLogger::ERROR);     }
  else if (sLogLevel == "WARNING")   { m_Logger->SetLogLevel(StdStreamLogger::WARNING);   }
  else if (sLogLevel == "NOTICE")    { m_Logger->SetLogLevel(StdStreamLogger::NOTICE);    }
  else if (sLogLevel == "INFO")      { m_Logger->SetLogLevel(StdStreamLogger::INFO);      }
  else if (sLogLevel == "DEBUG")     { m_Logger->SetLogLevel(StdStreamLogger::DEBUG);     }

  m_Logger->Write(StdStreamLogger::INFO,"Reading Config file: %s", sConfigPath.c_str());
  m_Logger->Write(StdStreamLogger::INFO,"LogLevel: %s", sLogLevel.c_str());

  //--------------------------------------------------
  // CtpMd
  //--------------------------------------------------
  setDataFolder     (STool::Trim(pt.get<std::string>("General.DataFolder")));
  setWriteDataToFile(STool::Trim(pt.get<std::string>("General.WriteDataToFile")));
  setConnectString  (STool::Trim(pt.get<std::string>("Account.ConnectionString")));
  setBrokerID       (STool::Trim(pt.get<std::string>("Account.BrokerID")));
  setInvestorID     (STool::Trim(pt.get<std::string>("Account.UserID")));
  setPassword       (            pt.get<std::string>("Account.Password"));

  vector<string> vSym;
  const string sSyms = pt.get<std::string>("General.SubscribeSymbols");
  boost::split(vSym,sSyms,boost::is_any_of(","));
  setSubscribeSymbols<vector<string> >(vSym);
}


//--------------------------------------------------
// setters
//--------------------------------------------------
void CtpMd::setDataFolder(const string & df)
{
  m_DataFolder = df;
}
void CtpMd::setWriteDataToFile(const string & wdtf)
{
  string wdtf2 = wdtf;
  boost::algorithm::to_lower(wdtf2);
  if (wdtf2 == "true" || wdtf2 == "t" || wdtf2 == "yes" || wdtf2 == "y")
  {
    m_WriteDataToFile = true;
    m_WriteDataToFile_CallBackFunc = new WriteDataToFile_CallBackFunc(boost::bind(&CtpMd::WriteDataToFile,this,_1));
  }
  else
  {
    m_WriteDataToFile = false;
    m_WriteDataToFile_CallBackFunc = new WriteDataToFile_CallBackFunc(boost::bind(&CtpMd::DoNotWriteDataToFile,this,_1));
  }
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
  strcpy(m_user_id,(char*)iid.c_str());
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

  m_pCThostFtdcMdApi = CreateFtdcMdApi("",true,true);
  m_pCThostFtdcMdApi->RegisterSpi((CThostFtdcMdSpi*) this);
  m_pCThostFtdcMdApi->RegisterFront((char*)m_connection_string.c_str());
  m_pCThostFtdcMdApi->Init();
  if (m_pCThostFtdcMdApi!=NULL)
  {
    m_pCThostFtdcMdApi->Join();
  }
}

//--------------------------------------------------
// callback
//--------------------------------------------------
void CtpMd::OnFrontConnected()
{
  m_Logger->Write(StdStreamLogger::INFO,"Front is connected");
  CThostFtdcReqUserLoginField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, m_broker_id);
  strcpy(req.UserID, m_user_id);
  strcpy(req.Password, m_password);
  int iResult = m_pCThostFtdcMdApi->ReqUserLogin(&req, ++m_iRequestID);
  m_Logger->Write(StdStreamLogger::CRITICAL,"User Login is %s",(iResult == 0) ? "OK" : "Fail");
  SubscribeSymbols(m_subscribedSymbols);
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

void CtpMd::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpMd::WriteDataToFile(const ATU_MDI_binary_marketfeed_struct & bmfs)
{
  // const string sFile(m_DataFolder+"/"+SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS()+".csv");
  // if (!m_data_outfile) m_data_outfile.reset(fopen(sFile.c_str(),"w"));
  //
  // fwrite(&bmfs,sizeof(ATU_MDI_binary_marketfeed_struct),1,m_data_outfile.get());
  // // fflush(m_data_outfile.get());

  // m_Logger->Write(StdStreamLogger::DEBUG,"%s.", ATU_MDI_marketfeed_struct::ToString(bmfs).c_str());
  const string sFile(m_DataFolder+"/"+SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS()+".csv");
  if (!m_data_outfile) m_data_outfile.reset(fopen(sFile.c_str(),"w"));

  const int iSize = 1024;
  char cStr[iSize];
  strcpy(cStr,ATU_MDI_binary_marketfeed_struct::ToString(bmfs).c_str());
  strcat(cStr,"\n");
  fwrite(cStr,sizeof(char),strlen(cStr),m_data_outfile.get());
  // fflush(m_data_outfile.get());
}
void CtpMd::DoNotWriteDataToFile(const ATU_MDI_binary_marketfeed_struct & bmfs)
{
  return;
}

void CtpMd::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
  ATU_MDI_binary_marketfeed_struct bmfs;

  //--------------------------------------------------
  // string tradingDay = pDepthMarketData->TradingDay;
  // string updateTime = pDepthMarketData->UpdateTime;
  // string updateMillisec = boost::lexical_cast<string>(pDepthMarketData->UpdateMillisec);
  // boost::replace_all(updateTime, ":", "");
  // char sMicrosec[10];
  // sprintf(sMicrosec,"%06d",pDepthMarketData->UpdateMillisec*1000);
  // mfs.m_timestamp     = tradingDay+"_"+updateTime+"_"+string(sMicrosec);
  //--------------------------------------------------
  bmfs.m_microsec_since_epoch_gmt = SDateTime::GetCurrentTimeInMicrosecSinceEpochGMT();
  memset(bmfs.m_instrument,'\0',strlen(bmfs.m_instrument)); // just to enable better compression ratio in the output binary data file.
  strcpy(bmfs.m_instrument,pDepthMarketData->InstrumentID);

  bmfs.m_traded_price  = (pDepthMarketData->LastPrice  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->LastPrice;
  bmfs.m_bid_price_1   = (pDepthMarketData->BidPrice1  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidPrice1;
  bmfs.m_bid_price_2   = (pDepthMarketData->BidPrice2  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidPrice2;
  bmfs.m_bid_price_3   = (pDepthMarketData->BidPrice3  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidPrice3;
  bmfs.m_bid_price_4   = (pDepthMarketData->BidPrice4  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidPrice4;
  bmfs.m_bid_price_5   = (pDepthMarketData->BidPrice5  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidPrice5;
  bmfs.m_ask_price_1   = (pDepthMarketData->AskPrice1  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskPrice1;
  bmfs.m_ask_price_2   = (pDepthMarketData->AskPrice2  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskPrice2;
  bmfs.m_ask_price_3   = (pDepthMarketData->AskPrice3  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskPrice3;
  bmfs.m_ask_price_4   = (pDepthMarketData->AskPrice4  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskPrice4;
  bmfs.m_ask_price_5   = (pDepthMarketData->AskPrice5  == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskPrice5;
  bmfs.m_traded_volume = (pDepthMarketData->Volume     == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->Volume;
  bmfs.m_bid_volume_1  = (pDepthMarketData->BidVolume1 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidVolume1;
  bmfs.m_bid_volume_2  = (pDepthMarketData->BidVolume2 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidVolume2;
  bmfs.m_bid_volume_3  = (pDepthMarketData->BidVolume3 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidVolume3;
  bmfs.m_bid_volume_4  = (pDepthMarketData->BidVolume4 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidVolume4;
  bmfs.m_bid_volume_5  = (pDepthMarketData->BidVolume5 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->BidVolume5;
  bmfs.m_ask_volume_1  = (pDepthMarketData->AskVolume1 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskVolume1;
  bmfs.m_ask_volume_2  = (pDepthMarketData->AskVolume2 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskVolume2;
  bmfs.m_ask_volume_3  = (pDepthMarketData->AskVolume3 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskVolume3;
  bmfs.m_ask_volume_4  = (pDepthMarketData->AskVolume4 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskVolume4;
  bmfs.m_ask_volume_5  = (pDepthMarketData->AskVolume5 == DBL_MAX) ? ATU_INVALID_PRICE : pDepthMarketData->AskVolume5;

  (*m_WriteDataToFile_CallBackFunc)(bmfs);
  // notify_marketfeed(bmfs);
}

//--------------------------------------------------
// receive from strategy engine
//--------------------------------------------------
void CtpMd::on_process_subscription(const ATU_MDI_subscription_struct & ssub)
{
  SubscribeSymbol(ssub.m_instrument);
  m_subscribedSymbols.insert(ssub.m_instrument);
}

void CtpMd::SubscribeSymbol(const string & sym)
{
  char * _ppInstrumentID[1];
  char csSym[64];
  strcpy(csSym, sym.c_str());
  _ppInstrumentID[0] = csSym;
  m_pCThostFtdcMdApi->SubscribeMarketData(_ppInstrumentID,1);
  m_Logger->Write(StdStreamLogger::INFO,"Subscribed %s.",sym.c_str());
}

void CtpMd::notify_marketfeed(const ATU_MDI_marketfeed_struct &s)
{
}

void CtpMd::release_CThostFtdcMdApi()
{
  if (m_pCThostFtdcMdApi != NULL)
  {
    m_pCThostFtdcMdApi->RegisterSpi(NULL);
    m_pCThostFtdcMdApi->Release();
    m_pCThostFtdcMdApi = NULL;
  }
}
