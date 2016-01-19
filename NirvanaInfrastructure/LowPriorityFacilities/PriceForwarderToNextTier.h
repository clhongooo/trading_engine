#ifndef LOWPRIORITYFACILITIES_PRICEFORWARDERTONEXTTIER_H_
#define LOWPRIORITYFACILITIES_PRICEFORWARDERTONEXTTIER_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include "MarketData.h"
#include "SystemConfig.h"
#include "SystemState.h"
#include "ThreadHealthMonitor.h"
#include <zmq.hpp>

class PriceForwarderToNextTier {
  public:
    PriceForwarderToNextTier();
    virtual ~PriceForwarderToNextTier();
    void Run();

  protected:

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<MarketData>           m_MarketData;
    boost::shared_ptr<SystemState>          m_SystemState;
    boost::shared_ptr<SystemConfig>         m_SysCfg;
    boost::shared_ptr<Logger>               m_Logger;
    boost::shared_ptr<ThreadHealthMonitor>  m_ThrdHlthMon;

    //--------------------------------------------------
    set<string>    m_sAllSym;

    //--------------------------------------------------
    // ZMQ
    //--------------------------------------------------
    boost::scoped_ptr<zmq::context_t> m_zmqcontext;
    boost::scoped_ptr<zmq::socket_t> m_zmqsocket;
    //--------------------------------------------------

};

#endif /* LOWPRIORITYFACILITIES_PRICEFORWARDERTONEXTTIER_H_ */
