#ifndef PROBDISTRIBUTIONGENERATOR_H_
#define PROBDISTRIBUTIONGENERATOR_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include "MarketData.h"
#include "SystemState.h"
#include "SystemConfig.h"
#include "VolSurfaces.h"
#include "Exchange.h"
#include "HKFE.h"
#include "HKSE.h"
#include "HKMA.h"
#include "TechIndicators.h"
#include "ProbMassFunc.h"
#include "ProbDistributions.h"
#include "CorrelMatrices.h"
#include "ThreadHealthMonitor.h"

#define FSMC_DEFAULT_YMDHMS 19000101000000

class ProbDistributionGenerator {
public:
	ProbDistributionGenerator();
	virtual ~ProbDistributionGenerator();
    void SetCalcIntervalInSec(long);
    void LoadTrainedFSMCData(const string &);
    void Run();

  private:
    long m_CalcIntervalInSeconds;
    long HashKey(const int,const int);

    //--------------------------------------------------
    YYYYMMDDHHMMSS m_ymdhms_LastCalculated;

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<MarketData>          m_MarketData;
    boost::shared_ptr<SystemState>         m_SystemState;
    boost::shared_ptr<SystemConfig>        m_SysCfg;
    boost::shared_ptr<VolSurfaces>         m_VolSurfaces;
    boost::shared_ptr<Logger>              m_Logger;
    boost::shared_ptr<Exchange>            m_Exchg;
    boost::shared_ptr<HKFE>                m_HKFE;
    boost::shared_ptr<HKSE>                m_HKSE;
    boost::shared_ptr<HKMA>                m_HKMA;
    boost::shared_ptr<TechIndicators>      m_TechInd;
    boost::shared_ptr<ProbDistributions>   m_ProbDistrn;
    boost::shared_ptr<CorrelMatrices>      m_CorrelMatrices;
    boost::shared_ptr<ThreadHealthMonitor> m_ThrdHlthMon;

    //--------------------------------------------------
    // Trained data for FSMC
    // Key - Hash of YYYYMM and FSMC state number
    //--------------------------------------------------
    map<long,ProbMassFunc*> m_fsmc_pmf;
};

#endif
