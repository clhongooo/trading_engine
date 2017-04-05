#ifndef PORTFOLIOSTRATEGYCMA_H_
#define PORTFOLIOSTRATEGYCMA_H_

#include "PCH.h"
#include "Constants.h"
#include "SystemState.h"
#include "SystemConfig.h"
#include "StrategyBase.h"
#include "MDI_Acknowledgement.h"
#include "MarketData.h"
#include "TechIndicators.h"

class StrategyCMA : public StrategyBase {
  public:
    StrategyCMA();
    virtual ~StrategyCMA();
    void Run();

  private:
    void ReportAckIfNeeded();
    long m_CalcIntervalInSeconds;

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
    boost::shared_ptr<MDI_Acknowledgement>  m_MDIAck;

    //--------------------------------------------------
    // Strategy objects
    //--------------------------------------------------
    // <SMA period><weight>
    //--------------------------------------------------
    vector<double>   m_ParamVector;
    long             m_SamplingIntervalInSec;
    YMDHMS           m_LastMonitoredTime;
};

#endif
