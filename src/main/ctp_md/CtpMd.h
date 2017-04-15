#ifndef ATU_CTP_MDI_H_
#define ATU_CTP_MDI_H_
#include "ATU_Abstract_MDI.h"
#include "StdStreamLogger.h"
#include "STool.h"
//#include "ATU_TCP_MDI_string_handler.h"
#include <iostream>
#include <map>
#include <dlfcn.h>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
using namespace boost::filesystem;
using namespace boost::interprocess;
#include "ThostFtdcMdApi.h"

class CtpMd : public CThostFtdcMdSpi {
  public:
    CtpMd();
    virtual void init();
    virtual ~CtpMd(){}

    void notify_marketfeed(const ATU_MDI_marketfeed_struct &);
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
    virtual void setConnectString(const string &);
    virtual void setBrokerID(const int);
    virtual void setInvestorID(const string &);
    virtual void setPassword(const string &);

    int m_iRequestID;

  private:
    boost::scoped_ptr<boost::thread>   m_p_init_and_run_thread;
    CThostFtdcMdApi* m_pMdApi;

    void*  m_p_ctp_lib_handle;

    string                   m_connection_string;
    TThostFtdcBrokerIDType   m_broker_id;
    TThostFtdcInvestorIDType m_investor_id;
    TThostFtdcPasswordType   m_password;
    TThostFtdcFrontIDType    m_front_id;
    TThostFtdcSessionIDType  m_session_id;
    set<string>              m_subscribedSymbols;

    boost::shared_ptr<StdStreamLogger> m_Logger;
};

#endif
