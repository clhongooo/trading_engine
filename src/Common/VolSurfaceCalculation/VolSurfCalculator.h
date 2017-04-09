#ifndef VOLSURFCALCULATOR_H_
#define VOLSURFCALCULATOR_H_

#include "PCH.h"
#include "Constants.h"
#include "SystemState.h"
#include "SystemConfig.h"
#include "MarketData.h"
#include "BlackScholes.h"
#include "Exchange.h"
#include "HKFE.h"
#include "HKMA.h"
#include "TechIndicators.h"
#include "VolSurfaces.h"
#include "ThreadHealthMonitor.h"

class VolSurfCalculator
{
  public:
    VolSurfCalculator();
    virtual ~VolSurfCalculator();
    void SetCalcIntervalInSec(long);
    void Run();

  private:
    long m_CalcIntervalInSeconds;

    //--------------------------------------------------
    //--------------------------------------------------
    YYYYMMDDHHMMSS m_ymdhms_LastCalculated;

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<MarketData>           m_MarketData;
    boost::shared_ptr<SystemState>          m_SystemState;
    boost::shared_ptr<SystemConfig>         m_SysCfg;
    boost::shared_ptr<VolSurfaces>          m_VolSurfaces;
    boost::shared_ptr<Logger>               m_Logger;
    boost::shared_ptr<Exchange>             m_Exchg;
    boost::shared_ptr<HKFE>                 m_HKFE;
    boost::shared_ptr<HKMA>                 m_HKMA;
    boost::shared_ptr<TechIndicators>       m_TechInd;
    boost::shared_ptr<ThreadHealthMonitor>  m_ThrdHlthMon;

};

#endif
