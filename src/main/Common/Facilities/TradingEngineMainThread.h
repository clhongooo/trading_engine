#ifndef TRADINGENGINEMAINTHREAD_H_
#define TRADINGENGINEMAINTHREAD_H_

#include "PCH.h"
#include "Constants.h"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/property_tree/ptree.hpp"
#include <boost/cstdint.hpp>
#include <boost/thread.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <boost/ptr_container/ptr_deque.hpp>
#include <string>
#include "Logger.h"
#include "SystemConfig.h"
#include "MarketData.h"
#include "MDIDataReceiver.h"
#include "StrategyConstants.h"
#include "OrderExecutor.h"
#include "VolSurfCalculator.h"
#include "VolSurfaces.h"
#include "ProbDistributionGenerator.h"
#include "TechIndUpdater.h"
#include "PriceForwarderToNextTier.h"
#include "Exchange.h"
#include "HKFE.h"
#include "HKSE.h"
#include "HKMA.h"
#include "Terminal.h"
#include "CorrelMatrices.h"
#include "MarkToMarket.h"
#include "ThreadHealthMonitor.h"

#include "StrategyTest.h"
#include "StrategyB2US1.h"
#include "StrategyB2US2.h"
#include "StrategyB2US3.h"
#include "StrategyB2HK.h"
#include "StrategyB3US.h"
#include "StrategyNIR1.h"

class TradingEngineMainThread {
  public:
    TradingEngineMainThread(const char *);
    TradingEngineMainThread(const char *, const char *);
    virtual ~TradingEngineMainThread();
    void RunMainThread();
    bool HasFinishedInit();
    inline static void ShortSleep() {usleep(100000);}

    string m_ConfigPath;
    string m_ItrdHighLowFromIB;

    boost::shared_ptr<SystemConfig>                p_SysCfg;
    boost::shared_ptr<MarketData>                  p_MktData;
    boost::shared_ptr<Logger>                      p_Logger;
    boost::shared_ptr<Exchange>                    p_Exchg;
    boost::shared_ptr<HKFE>                        p_HKFE;
    boost::shared_ptr<HKSE>                        p_HKSE;
    vector<boost::shared_ptr<MDIDataReceiver> >    p_dataagg;
    boost::shared_ptr<OrderExecutor>               p_oe;

  private:
    bool m_FinishedInit;

};

#endif /* TRADINGENGINEMAINTHREAD_H_ */
