#ifndef DATAAGGREGATOR_H_
#define DATAAGGREGATOR_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <stdarg.h>
#include "Logger.h"
#include "SDateTime.h"
#include "MDI_Acknowledgement.h"
#include "MarketData.h"
#include "SystemState.h"
#include "SystemConfig.h"
#include "ATU_TCPClient.h"
#include "PortfoliosAndOrders.h"
#include "ThreadHealthMonitor.h"

using namespace std;
using namespace boost;

//--------------------------------------------------
// Not singleton, there can be a whole bunch of DataAggregator
//--------------------------------------------------
typedef boost::function<void()> ATU_TCPClientAfterConnectionCallBackFunc;
typedef boost::function<void(string str)> ATU_TCPReadCallBackFunc;

class DataAggregator {
  public:
    DataAggregator(const int);
    virtual ~DataAggregator();
    void SetMDIServer(const string &,const string &);
    void OnNotifyLogfeed(ATU_logfeed_struct *);

    virtual void OnRecvMsgCSV(string);
    virtual void OnRecvMsgMDIStruct(const ATU_MDI_marketfeed_struct &);
    virtual void OnTCPConnect();
    virtual void Run();
    virtual void BatchMktFeedSubscription(const vector<string>&, const string &, const string &, const string &);
    virtual void BatchMktFeedSubscription(const vector<string>&, const string &, const string &, const string &, vector<ATU_MDI_subscription_struct> &);

    //--------------------------------------------------
    // Read Data File
    //--------------------------------------------------
    virtual void ReadDataFile(const string &);

  protected:
    //--------------------------------------------------
    int m_Aggregator_Identity;

    //--------------------------------------------------
    // TCP Client
    //--------------------------------------------------
    boost::scoped_ptr<ATU_TCPClient> m_mdi_server;
    string m_MDI_File;
    string m_MDI_Server_IP;
    string m_MDI_Server_Port;
    //--------------------------------------------------

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<Logger>               m_Logger;
    boost::shared_ptr<MarketData>           m_MarketData;
    boost::shared_ptr<SystemState>          m_SystemState;
    boost::shared_ptr<SystemConfig>         m_SysCfg;
    boost::shared_ptr<MDI_Acknowledgement>  m_MDIAck;
    boost::shared_ptr<ThreadHealthMonitor>  m_ThrdHlthMon;
    //--------------------------------------------------

};

#endif
