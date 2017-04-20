#ifndef ATU_CTP_MDI_H_
#define ATU_CTP_MDI_H_
#include "ATU_Abstract_MDI.h"
#include "StdStreamLogger.h"
#include "STool.h"
#include "SDateTime.h"
#include "Constants.h"
#include <dlfcn.h>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;
#include <boost/algorithm/string.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/numeric/conversion/cast.hpp>
using namespace boost::filesystem;
using namespace boost::interprocess;
#include "ThostFtdcMdApi.h"

class CtpMd : public CThostFtdcMdSpi {
  public:
    enum DataMode {DM_BINARY=0,DM_CSV};
    CtpMd();
    virtual void run();
    virtual ~CtpMd(){}

    //--------------------------------------------------
    void notify_marketfeed(const ATU_MDI_marketfeed_struct &);
    virtual void on_process_subscription(const ATU_MDI_subscription_struct &s);

    //--------------------------------------------------
    // CTP callback
    //--------------------------------------------------
    virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo,int nRequestID, bool bIsLast);
    virtual void OnFrontDisconnected(int nReason);
    virtual void OnHeartBeatWarning(int nTimeLapse);
    virtual void OnFrontConnected();
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) = 0;
    //--------------------------------------------------

    void release_CThostFtdcMdApi();
    virtual void setDataFolder(const string &);
    static DataMode detDataMode(const string &);
    virtual void setDataMode(const string &);
    virtual DataMode GetDataMode();
    virtual void setWriteDataToFile(const string &);
    virtual void setConnectString(const string &);
    virtual void setBrokerID(const string &);
    virtual void setInvestorID(const string &);
    virtual void setPassword(const string &);

    //--------------------------------------------------
    void SubscribeSymbol(const string &);
    template <typename CollectionType>
      void setSubscribeSymbols(const CollectionType & colSym)
      {
        m_subscribedSymbols.insert(colSym.begin(),colSym.end());
      }
    template <typename CollectionType>
      void SubscribeSymbols(const CollectionType & colSym)
      {
        for(typename CollectionType::iterator it = colSym.begin(); it != colSym.end(); it++)
          SubscribeSymbol(*it);
      }
    //--------------------------------------------------

  protected:
    CThostFtdcMdApi* m_pCThostFtdcMdApi;

    void* m_p_ctp_lib_handle;

    int                      m_iRequestID;

    string                   m_DataFolder;
    DataMode                 m_DataMode;
    string                   m_sDataMode;
    bool                     m_WriteDataToFile;
    string                   m_connection_string;
    TThostFtdcBrokerIDType   m_broker_id;
    TThostFtdcInvestorIDType m_investor_id;
    TThostFtdcPasswordType   m_password;
    TThostFtdcSessionIDType  m_session_id;
    // TThostFtdcFrontIDType    m_front_id;
    set<string>              m_subscribedSymbols;

    boost::shared_ptr<FILE>  m_data_outfile;

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<StdStreamLogger> m_Logger;
};

class CtpMdBin : public CtpMd {
  public:
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData);
};

class CtpMdCsv : public CtpMd {
  public:
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData);
};



#endif
