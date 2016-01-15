#ifndef PORTFOLIOS_H_
#define PORTFOLIOS_H_

#include "PCH.h"
#include "Constants.h"
#include "ATU_Abstract_OTI.h"
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <map>
#include <ctime>
#include <iomanip>
#include "MarketData.h"
#include "SystemConfig.h"
#include "Acct.h"
#include "STool.h"
#include "StyCPnLHist.h"
#include <zmq.hpp>

#define NUMOFDECIMALPLACEINPRICE 5

using namespace std;
using namespace boost;


typedef struct {
  enum OrderExecState
  {
    PENDING_ADD_BEF_SF            =0,
    PENDING_ADD_AFT_SF            =1,
    ADDED                         =2,
    PENDING_CANCEL_BEF_SF         =3,
    PENDING_CANCEL_AFT_SF         =4,
    CANCELLED                     =5,
    FILLED                        =6,
    ERROR                         =7
  };
  enum ExecStrategy
  {
    MARKET                        =0,
    LIMIT_AT_MIDQUOTE_THEN_MARKET =1,
    LIMIT_AT_BBO_THEN_MARKET      =2,
    PROGRESSIVE_LIMIT             =3,
    ALWAYS_LIMIT_AT_MID_QUOTE     =4,
    SIMPLE_LIMIT                  =5
  };
  string              m_order_id;
  string              m_time_stamp;
  string              m_symbol;
  long                m_signed_qty;
  int                 m_portfolio_id;
  OrderExecState      m_order_exec_state;
  ExecStrategy        m_exec_strategy;
  bool                m_resend;
  HHMMSS              m_attention_time;
  HHMMSS              m_create_time;
  vector<double>      m_exec_strategy_param;
} OrderInstruction;



class AttentionTime {
  public:
    int  GetWorkingOrdersPastAttentionTime(const HHMMSS &,deque<HHMMSS> &,deque<string> &);
    bool RemoveOrderIDFromAttentionTimeMap(const HHMMSS &, const string &);
    void CleanAttentionTimeMap(const HHMMSS &);
    void Add(const HHMMSS &,const string &);

  private:
    map<HHMMSS,set<string> > m_map_attention_time;
    shared_mutex             m_attention_time_mutex;
};

class PortfoliosAndOrders {
  public:
    enum Mode {SINGLE_ORDER, PORTFOLIO};
    enum OrderSet {WORKING_ORDER=0, COMPLETED_ORDER=1};

    static boost::shared_ptr<PortfoliosAndOrders> Instance();
    virtual ~PortfoliosAndOrders();

    void SwitchMode(const Mode m);
    bool Trade(const int,const string &,const long);
    bool Trade(const int,const string &,const long,const bool,const string &);

    bool Get_MTM_CPnL(const int,double &);
    double GetCPnL(const int);
    void ProvidePriceForMTM(const int,const string &,const double);
    set<string> GetSymbolsWithPos(const int);

    // XN
    void ConstructSignalFeedsAndChgOrderState(vector<string>&);
    void ConstructSignalFeedsAndChgOrderState(vector<ATU_OTI_signalfeed_struct>&);
    void MarketOrderConstructor(const OrderInstruction oi, ATU_OTI_signalfeed_struct&);
    void LimitOrderConstructor(const OrderInstruction, ATU_OTI_signalfeed_struct&);
    void CancelOrderConstructor(const OrderInstruction, ATU_OTI_signalfeed_struct&);

    bool UpdateOrderState(const string orderId, const OrderInstruction::OrderExecState state);
    bool BookTradeFromOTI(const string & order_id, const string & symbol, const double price, const long signed_qty);

    //--------------------------------------------------
    // Get / set portfolios
    //--------------------------------------------------
    void GetTargetPortfolio(const int, map<string,long> &);
    void SetTargetPortfolio(const int, const map<string,long> &);
    bool GetActualPortfolio(const int, map<string,long> &);
    void ConstructActualPortfolio(const int portId);
    double GetActualSignedPosition(const int portid, const string & symbol);

    void CancelAllWorkingOrdersInPortfolio(const int);

    bool IsOrderPendingCancelAftSF(const string orderId);
    bool IsOrderPendingAddAftSF(const string orderId);
    int  GetUnprocessedWorkingOrdersCount();
    bool IsPortfolioWorkingOrderEmpty(const long portId);

    void CheckAndReplaceOrders(const HHMMSS &);

    //--------------------------------------------------
    void WriteActualPortToPersistentPosFile();
    void WriteActualPortToPersistentPosToLog();
    //--------------------------------------------------
    // Observer
    //--------------------------------------------------
    void WaitForData();
    void NotifyConsumers();
    //--------------------------------------------------

  private:
    void LoadPersistedPositionFromFile();
    bool CancelWorkingOrder(const string orderId, const bool isResend);
    bool GetAndEraseWorkingOrder(OrderInstruction &);

    bool DetermineExecStrategy(const string &,OrderInstruction::ExecStrategy &,vector<double> &);
    bool TradeUltimate(const int, const string &, const long, const OrderInstruction::ExecStrategy, const vector<double>&, const bool, const string &);
    void UpdateTargetPortfolio(const int, const string &, const long, const char);
    void AcctTrade(const int,const string &,const long,const double);
    void AcctSetRecord(const int,const string &,const long,const double);
    void AcctClearPosChgFlagNoLock();
    double AcctCheckPos(const int,const string &);
    bool CheckIfSomeAcctPosChgd();

    //--------------------------------------------------
    void OutputActualPortToPersistentPos(const char);
    //--------------------------------------------------

    // XN
    Mode m_CurrentMode;
    AttentionTime m_attn_time;
    long m_UniqID;

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    PortfoliosAndOrders();
    PortfoliosAndOrders(PortfoliosAndOrders const&);
    PortfoliosAndOrders& operator=(PortfoliosAndOrders const&){};
    static boost::weak_ptr<PortfoliosAndOrders> m_pInstance;

    //--------------------------------------------------
    // System Objects
    //--------------------------------------------------
    boost::shared_ptr<MarketData>     m_MarketData;
    boost::shared_ptr<Logger>         m_Logger;
    boost::shared_ptr<SystemConfig>   m_SysCfg;
    boost::shared_ptr<StyCPnLHist>    m_StyCPnLHist;

    //--------------------------------------------------
    // m_target_portfolios:
    // Key   - portfolio number
    // Value - map containing the portfolio composition
    //         Key   - instrument symbol
    //         Value - signed qty. e.g. +2 means long 2 contracts
    //--------------------------------------------------
    map<int, map<string,long> >              m_target_portfolios;
    shared_mutex                             m_target_portfolios_mutex;

    //--------------------------------------------------
    // A working order refers to any uncancelled and unfilled orders
    // A completed order refers to any cancelled and filled orders
    // Key   - order id
    // Value - OrderInstruction
    //--------------------------------------------------
    map<string,OrderInstruction>             m_working_orders;
    map<string,OrderInstruction>             m_completed_orders;
    shared_mutex                             m_orders_mutex;

    //--------------------------------------------------
    // For internal book keeping, calculation of PnL
    //--------------------------------------------------
    map<int,Acct>     m_acct;
    shared_mutex      m_acct_mutex;
    //--------------------------------------------------

    //--------------------------------------------------
    // Observer
    //--------------------------------------------------
    boost::mutex m_ObserverMutex;
    boost::condition_variable m_cvOrdersAvb;
    //--------------------------------------------------

    //--------------------------------------------------
    // ZMQ
    //--------------------------------------------------
    boost::scoped_ptr<zmq::context_t> m_zmqcontext;
    boost::scoped_ptr<zmq::socket_t> m_zmqsocket;
    //--------------------------------------------------

};

#endif /* PORTFOLIOS_H_ */
