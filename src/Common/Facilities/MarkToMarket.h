#ifndef MARKTOMARKET_H_
#define MARKTOMARKET_H_

#include "PCH.h"
#include "Constants.h"
#include <set>
#include <fstream>                                                                                                                     
#include "SystemState.h"
#include "SystemConfig.h"
#include "MarketData.h"
#include "TechIndicators.h"
#include "PortfoliosAndOrders.h"
#include "ThreadHealthMonitor.h"
#include "StyCPnLHist.h"

class MarkToMarket {
  public:
    MarkToMarket();
    virtual ~MarkToMarket();
    void Run();

  private:
    void ReportAckIfNeeded();

    //--------------------------------------------------
    YYYYMMDDHHMMSS m_ymdhms_LastCalculated;

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<MarketData>           m_MarketData;
    boost::shared_ptr<SystemState>          m_SystemState;
    boost::shared_ptr<SystemConfig>         m_SysCfg;
    boost::shared_ptr<Logger>               m_Logger;
    boost::shared_ptr<TechIndicators>       m_TechInd;
    boost::shared_ptr<PortfoliosAndOrders>  m_PortAndOrders;
    boost::shared_ptr<ThreadHealthMonitor>  m_ThrdHlthMon;
    boost::shared_ptr<StyCPnLHist>          m_StyCPnLHist;
    boost::shared_ptr<MDI_Acknowledgement>  m_MDIAck;
    //--------------------------------------------------

    //--------------------------------------------------
    map<StrategyID,set<int> > m_Sty_MTM_Done;
    set<int> m_Overall_MTM_Done;

};

#endif
