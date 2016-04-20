#ifndef TECHINDUPDATER_H_
#define TECHINDUPDATER_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include "FSMC.h"
#include "HKFE.h"
#include "HKSE.h"
#include "MarketData.h"
#include "SystemConfig.h"
#include "SystemState.h"
#include "TechIndicators.h"
#include "TechIndUpdater.h"
#include "VolSurfaces.h"
#include "HKIntradaySeasonality.h"
#include "ThreadHealthMonitor.h"
#include "Exchange.h"
#include "PeriodicTask.h"
#include "STool.h"

class TechIndUpdater {
  public:
    TechIndUpdater();
    virtual ~TechIndUpdater();
    void Run();

  protected:
    void ReportAckIfNeeded();

    //--------------------------------------------------
    //--------------------------------------------------
    YYYYMMDDHHMMSS m_ymdhms_LastCalculated;
    YYYYMMDDHHMMSS m_ymdhms_LastMonitorTime;

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<MarketData>            m_MarketData;
    boost::shared_ptr<SystemState>           m_SystemState;
    boost::shared_ptr<SystemConfig>          m_SysCfg;
    boost::shared_ptr<Logger>                m_Logger;
    boost::shared_ptr<TechIndicators>        m_TechInd;
    boost::shared_ptr<VolSurfaces>           m_VolSurfaces;
    boost::shared_ptr<Exchange>              m_Exchg;
    boost::shared_ptr<HKFE>                  m_HKFE;
    boost::shared_ptr<HKSE>                  m_HKSE;
    boost::shared_ptr<NYSE>                  m_NYSE;
    boost::shared_ptr<MDI_Acknowledgement>   m_MDIAck;
    boost::shared_ptr<HKIntradaySeasonality> m_HKItrdSea;
    boost::shared_ptr<ThreadHealthMonitor>   m_ThrdHlthMon;


    //--------------------------------------------------
    // Commmonly used temp variable to avoid frequent memory allocation
    //--------------------------------------------------
    YYYYMMDD m_temp_yyyymmdd;
    HHMMSS   m_temp_hhmmss;
    double   m_temp_open;
    double   m_temp_high;
    double   m_temp_low;
    double   m_temp_close;
    long     m_temp_volume;


    //--------------------------------------------------
    YYYYMMDDHHMMSS m_ymdhms_SysTimeHKT;
    set<string>    m_sHKc1Sym;
    set<string>    m_sHKIdx;
    set<string>    m_sOtherSym;
    set<string>    m_sAllSym;
    bool           m_WarmedUp;

    PeriodicTask   m_PTask_PrintDayHighLow;
    map<string,int> m_PTask_PrintDayHighLowToken;
};
#endif
