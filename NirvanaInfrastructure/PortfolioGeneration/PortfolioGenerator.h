#ifndef PORTFOLIOGENERATOR_H_
#define PORTFOLIOGENERATOR_H_

#include "PCH.h"
#include "Constants.h"
#include "SystemState.h"
#include "SystemConfig.h"
#include "MDI_Acknowledgement.h"
#include "MarketData.h"
#include "TechIndicators.h"
#include "PortfoliosAndOrders.h"
#include "VolSurfaces.h"

class PortfolioGenerator {
public:
	PortfolioGenerator();
	virtual ~PortfolioGenerator();
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
    boost::shared_ptr<Logger>               m_Logger;
    boost::shared_ptr<TechIndicators>       m_TechInd;
    boost::shared_ptr<PortfoliosAndOrders>  m_PortAndOrders;
    boost::shared_ptr<VolSurfaces>          m_VolSurfaces;
    boost::shared_ptr<MDI_Acknowledgement>  m_MDIAck;
};

#endif
