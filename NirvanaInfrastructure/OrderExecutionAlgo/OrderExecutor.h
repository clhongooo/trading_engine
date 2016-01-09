#ifndef ORDEREXECUTOR_H_
#define ORDEREXECUTOR_H_

#include "PCH.h"
#include "Constants.h"
#include <memory>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <stdarg.h>
#include "Logger.h"
#include "MarketData.h"
#include "SDateTime.h"
#include "SystemState.h"
#include "PortfoliosAndOrders.h"
#include "ATU_TCPClient.h"
#include "ATU_Abstract_OTI.h"
#include "ThreadHealthMonitor.h"

using namespace std;
using namespace boost;

typedef boost::function<void()> ATU_TCPClientAfterConnectionCallBackFunc;
typedef boost::function<void(string str)> ATU_TCPReadCallBackFunc;

//--------------------------------------------------
// Not singleton, there can be different order executors for different OTI
//--------------------------------------------------
class OrderExecutor {
  public:
    OrderExecutor();
    virtual ~OrderExecutor();
    void SetOTIServer(const string &,const string &);
    void OnNotifyLogfeed(ATU_logfeed_struct *);
    // Xiao Nan
    void HandleOrderFeedRecv(const vector<string> & feedMsg);
    void HandleTradeFeedRecv(const vector<string> & feedMsg);
    void HandleOrderFeedRecv(const ATU_OTI_orderfeed_struct &);
    void HandleTradeFeedRecv(const ATU_OTI_tradefeed_struct &);
    // Xiao Nan

    virtual void OnRecvMsg(string);
    virtual void OnTCPConnect();
    virtual void Run();
    virtual void RunChkOrd();
    virtual void RunPersistPos();

  protected:
    //--------------------------------------------------
    // TCP Client
    //--------------------------------------------------
    boost::scoped_ptr<ATU_TCPClient> m_oti_server;
    string m_OTI_Server_IP;
    string m_OTI_Server_Port;
    //--------------------------------------------------

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<Logger>              m_Logger;
    boost::shared_ptr<MarketData>          m_MarketData;
    boost::shared_ptr<SystemState>         m_SystemState;
    boost::shared_ptr<PortfoliosAndOrders> m_PortAndOrders;
    boost::shared_ptr<SystemConfig>        m_SysCfg;
    boost::shared_ptr<ThreadHealthMonitor> m_ThrdHlthMon;
    //--------------------------------------------------
};

#endif /* ORDEREXECUTOR_H_ */
