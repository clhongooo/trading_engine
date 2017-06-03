/*
 * ATU_IB_OTI.h
 *
 *  Created on: Oct 13, 2014
 *      Author: jasonlin
 */

#ifndef ATU_IB_OTI_H_
#define ATU_IB_OTI_H_

#include <boost/thread.hpp>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "ATU_Abstract_OTI.h"
#include "ATU_IB_Client_OTI.h"
#include "ATU_IB_ClientSocket_OTI.h"
#include "ConfigMgr.h"
#include "Contract.h"
#include "ContractManager.h"
#include "Execution.h"
#include "IniContractReader.h"
#include "JsonContractReader.h"
#include "Order.h"
#include "Utility.h"
#include "ExpandableCirBuffer.h"
#include "StdStreamLogger.h"

using std::tr1::unordered_map;
using std::tr1::unordered_set;
using std::stringstream;
using namespace atu;

#define ORDER_FILLED_QTY_INIT -1
#define ORDER_FILLED_QTY_ERROR -2
#define ORDER_FILLED_QTY_COMPLETED -3 // Including deleted

class ATU_IB_OTI : public ATU_IB_Client_OTI
{
  public:

    ATU_IB_OTI(boost::shared_ptr<ExpandableCirBuffer>, const string &p_host, unsigned int p_port = 7496, int p_clientId = 0);
    ATU_IB_OTI(boost::shared_ptr<ExpandableCirBuffer>);
    virtual ~ATU_IB_OTI();
    virtual void run();

    virtual bool isReady() const;

    //--------------------------------------------------
    virtual bool on_process_signalfeed(ATU_OTI_signalfeed_struct &p_s);
    virtual bool on_process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &p_s);
    virtual bool on_process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &p_s);

    // Interfaces in ATU_IB_Client_OTI
    virtual void orderStatus(OrderId p_orderId, const IBString &p_status, int p_filled, int p_remaining, double p_avgFillPrice, int p_permId,
                             int p_parentId, double p_lastFillPrice, int p_clientId, const IBString &p_whyHeld);
    virtual void openOrder(OrderId p_orderId, const Contract &p_contract, const Order &p_order, const OrderState &p_orderState);
    virtual void openOrderEnd();
    virtual void nextValidId(OrderId p_orderId);
    virtual void deltaNeutralValidation(int p_reqId, const UnderComp &p_underComp);
    virtual void execDetails(int p_reqId, const Contract &p_contract, const Execution& p_execution);
    virtual void execDetailsEnd(int p_reqId);
    virtual void error(const int p_id, const int p_errorCode, const IBString p_errorString);

    //--------------------------------------------------
    virtual void setIP(const string &);
    virtual void setPort(const unsigned int);
    virtual void setClientID(const int);
    virtual void setAccount(const string &);
    virtual void setOrderMappingFile(const string &);

    void readContracts(string p_filepath);
    void readOrderMapping();

  private:
    inline void notify_tradefeed(const ATU_OTI_tradefeed_struct &);
    inline void notify_orderfeed(const ATU_OTI_orderfeed_struct &);

    string m_host;
    unsigned int m_port;
    int m_clientId;
    IBString m_account;
    bool m_isMaster;
    int m_nextReqId;
    bool m_isDataReady;
    bool m_hasReqOpenOrders;
    string m_filenameOrderMapping;
    OrderId m_nextValidOrderId;
    ofstream m_fileForAppendingOrderMapping;

    ATU_IB_ClientSocket_OTI m_clientSocket;

    // Map free-text styled order id to long type IB OrderId
    map<string, OrderId> m_userOrderId2IBOrderId;
    map<OrderId, string> m_IBOrderId2userOrderId;
    unordered_map<OrderId, Contract> m_orderId2Contract;
    unordered_map<OrderId, string> m_orderId2feedcode;
    unordered_map<OrderId, Order> m_orderId2Order;
    map<OrderId, int> m_orderId2filledQty;
    map<OrderId, IBString> m_orderId2errMsg;
    map<IBString, ATU_OTI_tradefeed_struct*> m_tradefeedList;

    boost::mutex m_orderId2filledQtyMutex;
    boost::mutex m_checkOrderIdMutex;
    boost::shared_mutex m_getTradeHistoryMutex;
    boost::condition_variable m_orderId2filledQtyCond;

    unordered_set<int> filteredErrorCodes;
    unordered_map<OrderId, string> m_tmpOrderId2OpenClose;
    unordered_map<OrderId, string> m_tmpOrderId2BuySell;

    ContractManager* m_contractExtractor;
    boost::shared_ptr<ExpandableCirBuffer> m_ecbOrd;
    boost::shared_ptr<StdStreamLogger> m_Logger;
};


#endif /* ATU_IB_OTI_H_ */
