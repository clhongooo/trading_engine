//--------------------------------------------------
// modified from jasonlin's code written near Oct 8, 2014
//--------------------------------------------------
#ifndef ATU_IB_MDI_H_
#define ATU_IB_MDI_H_

#include "ATU_IB_Client_MDI.h"
#include "ATU_Abstract_MDI.h"
#include "ATU_IB_ClientSocket_MDI.h"
#include "MarketDepthData.h"
#include "Contract.h"
#include "ContractManager.h"
#include "StdStreamLogger.h"
#include "STool.h"
#include "SDateTime.h"
#include "Constants.h"
#include "ExpandableCirBuffer.h"

#include "Utility.h"
#include "ConfigMgr.h"
#include <string>
#include <boost/lexical_cast.hpp>
#include "IniContractReader.h"
#include <boost/thread.hpp>
#include <map>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

using std::tr1::unordered_map;
using std::tr1::unordered_set;
using std::map;
using namespace boost;
using namespace atu;

class ATU_IB_MDI : public ATU_IB_Client_MDI
{
  public:
    ATU_IB_MDI(boost::shared_ptr<ExpandableCirBuffer>, const string &p_host, unsigned int p_port = 7496, int p_clientId = 0);
    ATU_IB_MDI(boost::shared_ptr<ExpandableCirBuffer>);
    virtual ~ATU_IB_MDI();
    virtual void run();

    virtual bool isReady() const;

    //--------------------------------------------------
    // Interfaces in ATU_IB_Client_MDI
    //--------------------------------------------------
    virtual void tickPrice(TickerId p_tickerId, TickType p_field, double p_price, int p_canAutoExecute);
    virtual void tickSize(TickerId p_tickerId, TickType p_field, int p_size);
    virtual void tickOptionComputation(TickerId p_tickerId, TickType p_tickType, double p_impliedVok, double p_delta,
                                       double p_optPrice, double p_pvDividend, double p_gamma, double p_vega, double p_theta, double p_undPrice);
    virtual void tickGeneric(TickerId p_tickerId, TickType p_tickType, double p_value);
    virtual void tickString(TickerId p_tickerId, TickType p_tickType, const IBString &p_value);
    virtual void tickEFP(TickerId p_tickerId, TickType p_tickType, double p_basisPoints, const IBString &p_formattedBasisPoints,
                         double p_totalDividends, int p_holdDays, const IBString &p_futureExpiry, double p_dividendImpact, double p_dividendsToExpiry);
    virtual void tickSnapshotEnd(int p_reqId);
    virtual void marketDataType(TickerId p_reqId, int p_marketDataType);
    virtual void updateMktDepth(TickerId p_id, int p_position, int p_operation, int p_side, double p_price, int p_size);
    virtual void error(const int p_id, const int p_errorCode, const IBString p_errorString);

    //--------------------------------------------------
    virtual bool on_process_subscription(const string &);
    virtual void notify_marketfeed(const MarketDepthData &);
    virtual void notify_marketfeed(const ATU_MDI_marketfeed_struct &);

    //--------------------------------------------------
    virtual void setIP(const string &);
    virtual void setPort(const unsigned int);
    virtual void setClientID(const int);
    virtual void setAccount(const string &);

    void readContracts(string p_filepath);

  private:

    string        m_host;
    unsigned int  m_port;
    int           m_clientId;
    IBString      m_account;
    bool          m_isReady;
    string        m_connection_string;
    set<string>   m_subscribedSymbols;

    //--------------------------------------------------
    // Returning level II (market depth) data will trigger updateMktDepth() and tickPrice() and tickSize(),
    // while level I data (best bid and best ask) trigger only tickPrice() and tickSize()
    //
    // When fetching level II data and use updateMktDepth() to update data, better not to use tickPrice() and tickSize()
    // to update the best bid and best ask in the meanwhile, I found some data is not consistent
    //
    // To fix this, use a member vairable to distinguish which mode the data plan is. If m_usingLevel2 is true,
    // we will not use tickPrice and tickSize to update data.
    //--------------------------------------------------

    unordered_map<TickerId, bool> m_usingLevel2;

    unordered_map<TickerId, MarketDepthData> m_marketDepthData;

    ATU_IB_ClientSocket_MDI m_clientSocket;

    // TODO: send all when connecting?
    map<TickerId, Contract> m_subscribeList;
    unordered_set<string> m_feedcodeSet;
    TickerId m_nextTickerId;

    boost::mutex m_updateTickerIdMutex;

    ContractManager* m_contractExtractor;


    boost::shared_ptr<FILE>  m_data_outfile;
    boost::shared_ptr<ExpandableCirBuffer> m_ecbMD;

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<StdStreamLogger> m_Logger;

};

#endif
