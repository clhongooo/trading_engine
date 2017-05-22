#ifndef _ATU_CTP_OTI_H_
#define _ATU_CTP_OTI_H_
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>
#include <dlfcn.h>
#include <boost/thread.hpp>
#include <stdio.h>
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
#include <boost/lexical_cast.hpp>

#include "ATU_Abstract_MDI.h"
#include "ATU_Abstract_OTI.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcTraderApi.h"
#include "StdStreamLogger.h"
#include "STool.h"
#include "SDateTime.h"
#include "Constants.h"

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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
using namespace boost::filesystem;
using namespace boost::interprocess;

class CThostFtdcTraderApi;
class CTraderSpi;

class CtpOrd : public CThostFtdcTraderSpi
{
  public:
    CtpOrd();
    virtual void run();
    virtual ~CtpOrd();

    //--------------------------------------------------
    virtual bool on_notify_orderfeed(ATU_OTI_orderfeed_struct &s);
    virtual bool on_notify_tradefeed(ATU_OTI_tradefeed_struct &s);
    virtual bool on_process_signalfeed(ATU_OTI_signalfeed_struct &s);

    //--------------------------------------------------
    virtual bool process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &s);
    virtual bool on_process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &s);
    virtual bool process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &s);
    virtual bool on_process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &s);

    //--------------------------------------------------
    // CTP callback
    //--------------------------------------------------
    virtual void OnFrontConnected();
    virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnFrontDisconnected(int nReason);
    virtual void OnHeartBeatWarning(int nTimeLapse);
    virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
    virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
    void ReqUserLogin();

    //--------------------------------------------------
    // 
    //--------------------------------------------------
    virtual void setConnectString(const string &);
    virtual void setBrokerID(const string &);
    virtual void setInvestorID(const string &);
    virtual void setPassword(const string &);

    void printCThostFtdcOrderField(CThostFtdcOrderField *);

  private:
    CThostFtdcTraderApi*     m_pCThostFtdcTraderApi;
    void*                    m_p_ctp_lib_handle;
    string                   m_connection_string;
    TThostFtdcBrokerIDType   m_broker_id;
    TThostFtdcInvestorIDType m_user_id;
    TThostFtdcPasswordType   m_password;
    TThostFtdcFrontIDType    m_front_id;
    TThostFtdcSessionIDType  m_session_id;
    int m_iRequestID;

    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
    bool IsMyOrder(CThostFtdcOrderField *pOrder);
    bool IsTradingOrder(CThostFtdcOrderField *pOrder);
    string createOrderRef(string orderid);
    string getOrderRef(string orderid);
    string getOrderID(string orderref);
    map<string,string> m_order_idToOrderRef;
    map<string,string> m_OrderRefToorder_id;
    int m_orderRefCount;
    string m_lastOrderTimestamp;
    map<string,string> m_orderid_instrument;

    //--------------------------------------------------
    boost::recursive_mutex m_requestIDMutex;
    boost::recursive_mutex m_orderRefMutex;

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<StdStreamLogger> m_Logger;
};

#endif
