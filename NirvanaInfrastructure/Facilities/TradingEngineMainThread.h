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
#include "DataAggregator.h"
#include "OrderExecutor.h"
#include "VolSurfCalculator.h"
#include "VolSurfaces.h"
#include "PortfolioGenerator.h"
#include "StrategyTest.h"
#include "StrategyB1HKF.h"
#include "StrategyB2HK.h"
#include "StrategyB2USSTK.h"
#include "StrategyS11A.h"
#include "StrategyA1.h"
#include "StrategyA6.h"
#include "StrategyR1.h"
#include "StrategyR3.h"
#include "StrategyR7.h"
#include "StrategyR8.h"
#include "StrategyR9.h"
#include "ProbDistributionGenerator.h"
#include "TechIndUpdater.h"
#include "Exchange.h"
#include "HKFE.h"
#include "HKSE.h"
#include "HKMA.h"
#include "Terminal.h"
#include "CorrelMatrices.h"
#include "MarkToMarket.h"
#include "ThreadHealthMonitor.h"

class TradingEngineMainThread {
  public:
    TradingEngineMainThread(const string &);
    virtual ~TradingEngineMainThread();
    void RunMainThread();
    bool HasFinishedInit();

    string m_ConfigPath;

    boost::shared_ptr<SystemConfig>                p_SysCfg;
    boost::shared_ptr<Logger>                      p_Logger;
    boost::shared_ptr<Exchange>                    p_Exchg;
    boost::shared_ptr<HKFE>                        p_HKFE;
    boost::shared_ptr<HKSE>                        p_HKSE;
    boost::shared_ptr<HKMA>                        p_HKMA;
    boost::shared_ptr<CorrelMatrices>              p_CorrelMatrices;
    vector<boost::shared_ptr<DataAggregator> >     p_dataagg;
    boost::shared_ptr<OrderExecutor>               p_oe;

  private:
    bool m_FinishedInit;

};

#endif /* TRADINGENGINEMAINTHREAD_H_ */
