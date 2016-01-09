#ifndef TRADINGENGINEALGOI_H_
#define TRADINGENGINEALGOI_H_
#include "ATU_Abstract_AlgoI.h"
#include <boost/lexical_cast.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread.hpp>
#include "TradingEngineMainThread.h"
#include <boost/shared_ptr.hpp>

using namespace boost;

class TradingEngineAlgoI : public ATU_Abstract_AlgoI {
  public:

    TradingEngineAlgoI();
    ~TradingEngineAlgoI();
    virtual bool on_marketfeed_process(ATU_MDI_marketfeed_struct& s);
    virtual bool on_tradefeed_process(ATU_OTI_tradefeed_struct &sin);
    virtual bool on_riskfeed_process(ATU_OTI_riskfeed_struct &sin);
    virtual bool on_orderfeed_process(ATU_OTI_orderfeed_struct &sin);
    virtual bool on_portfoliofeed_process(ATU_OTI_portfoliofeed_struct &s);
    virtual bool on_accumperffeed_process(ATU_OTI_accumperffeed_struct &s);
    virtual bool on_pnlperffeed_process(ATU_OTI_pnlperffeed_struct &pnlfeed);
    virtual bool on_errorfeedp_process(ATU_ErrorMsgStruct *s);
    virtual bool on_errorfeed_process(ATU_ErrorMsgStruct &s);
    virtual bool on_dailyperffeed_process(ATU_OTI_dailyperffeed_struct &s);
    virtual void detach();
  private:
    shared_ptr<TradingEngineMainThread> m_MainThdCls;
    shared_ptr<boost::thread> m_MainThd;
    shared_ptr<PortfoliosAndOrders> m_PortAndOrders;

};
#endif
