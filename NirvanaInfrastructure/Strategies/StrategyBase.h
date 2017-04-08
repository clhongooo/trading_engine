#ifndef STRATEGIES_STRATEGYBASE_H_
#define STRATEGIES_STRATEGYBASE_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include <map>
#include <vector>
#include "SystemState.h"
#include "SystemConfig.h"
#include "MDI_Acknowledgement.h"
#include "MarketData.h"
#include "TechIndicators.h"
#include "PortfoliosAndOrders.h"
#include "VolSurfaces.h"
// #include "HKIntradaySeasonality.h"
#include "Acct.h"
#include "SDateTime.h"
#include "ThreadHealthMonitor.h"
#include "StyCPnLHist.h"
#include "Exchange.h"
#include "RegularMiniCtrtManager.h"
#include "PeriodicTask.h"

#define NUMOFMETHODS 10

class StrategyBase {
  public:
    enum WEIGHTING_SCHEME {WS_UNDEFINED=0,WS_UNIFORM=1,WS_LINEAR=2};
    enum TRADEDIR {TD_NODIR=0,TD_LONG=1,TD_SHORT=2};
    enum AUTOCLOSEPOS {ACP_UNSET=0,ACP_YES=1,ACP_NO=2,ACP_YESBUTNOTOVERLUNCH};
    enum STY_RUN_MODE {SRM_UNSET=0,SRM_TRNG=1,SRM_BKT_OR_LIVE=2};
    enum STY_DOMICILE_MKT {SDM_UNSET=0,SDM_HK=1,SDM_US=2};

    StrategyBase();
    virtual ~StrategyBase();
    virtual void Run();

  protected:
    void SetStrategyID(const StrategyID);
    void SetStyDomicileMkt(const STY_DOMICILE_MKT);
    StrategyID GetStrategyID();
    STY_DOMICILE_MKT GetStyDomicileMkt();
    void SetCloseAllPosIfOutsideTrdgHour(const AUTOCLOSEPOS);
    void SetStyRunMode(const STY_RUN_MODE);
    void SetTradeRelatedParam(const int iTradSym, const TRADEDIR td, const double abspos);
    double GetPrevTheoSgndPos(const string &);

    virtual void StrategySetup() = 0;
    virtual void ReadParam() = 0;
    virtual void ParamSanityCheck() = 0;
    virtual void StartOfDayInit() = 0;
    virtual void EndOfDayCleanUp() = 0;
    virtual void UnsetConvenienceVarb() = 0;
    virtual void SetConvenienceVarb(const int iTradSym) = 0;
    virtual bool SkipSubseqProcessingForSymbol(const int iTradSym,string & sReason) = 0; // Special function to reduce redundant processing, if any
    virtual void InitialWarmUp(const int iTradSym) = 0;
    virtual void UpdateInternalData(const int iTradSym) = 0;                             // The only place where we should get market prices from the shared objects
    virtual void UpdateInternalDataTrng(const int iTradSym) = 0;
    virtual void LoadOptimalParamsFromTrainingResults() = 0;
    virtual void EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket) = 0;
    virtual void DetermineRegime(const int iTradSym) = 0;
    virtual void PreTradePreparation(const int iTradSym) = 0;
    virtual void CalcPosSize(const int iTradSym) = 0;
    virtual void AdjustSamplingInterval(const int iTradSym);
    virtual void PerformTrainingJustBeforeTrading(const int iTradSym);
    virtual void EndOfDayTrainingRoutine(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket);

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<SystemConfig>           m_SysCfg;
    boost::shared_ptr<Logger>                 m_Logger;
    boost::shared_ptr<Exchange>               m_Exchg;
    boost::shared_ptr<HKFE>                   m_HKFE;
    boost::shared_ptr<HKSE>                   m_HKSE;
    boost::shared_ptr<NYSE>                   m_NYSE;
    boost::shared_ptr<CME>                    m_CME;
    // boost::shared_ptr<HKIntradaySeasonality>  m_HKItrdSea;
    boost::shared_ptr<StyCPnLHist>            m_StyCPnLHist;

    //--------------------------------------------------
    // Common strategy objects
    //--------------------------------------------------

    StrategyID       m_StyID;
    int              m_MonitoringIntervalInSec;
    Acct             m_TrainingAcct;
    bool             m_WhetherToRetrain;
    vector<double>   m_ParamVector;
    vector<string>   m_TradedSymbols;
    vector<double>   m_TargetAbsPos;
    YYYYMMDDHHMMSS   m_ymdhms_SysTime_HKT;
    YYYYMMDDHHMMSS   m_ymdhms_SysTime_EST;
    YYYYMMDDHHMMSS   m_ymdhms_SysTime_Domicile;
    HHMMSS           m_hmsStyGlobalMTMTime;
    HMS              m_hms_StyMrngOpen;
    HMS              m_hms_StyMrngClose;
    HMS              m_hms_StyAftnOpen;
    HMS              m_hms_StyAftnClose;
    vector<string>   m_PreviousTradedSym;
    vector<TRADEDIR> m_TargetTradeDir;
    int              m_iNumOfParam;
    int              m_StartTimeBufferInSec;
    int              m_EndTimeBufferInSec;
    int              m_BreakTimeBufferInSec;

    double           m_SymMidQuote;
    double           m_SymFM1MidQuote;
    YYYYMMDDHHMMSS   m_ymdhms_midquote;

    map<int, map<string,YYYYMMDDHHMMSS> > m_map_ymdhms_LastTrainTime;
    boost::scoped_ptr<ofstream> m_fsSignalLog;

  private:
    void ResetTradeRelatedParam(const int);
    void ResumeOrInitTradeTargets(const int);
    void TradeRelatedProcedure(const int);
    void GetFutFrontMonthSymbols();
    // void GetHKFETimeTable(const YYYYMMDD &);
    void ReportAckIfNeeded();
    void RollForward(const int);
    bool ShouldRollForward(const int);
    void PlaceOrder(const int);
    void PerformEndOfDayTraining(const int iTradSym);
    void SetPrevTheoSgndPos(const string &, const double);
    void StyInitAndChecking();
    void StyEODSODRoutine();
    void StyForceRollContFutIfNec(const int);

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<MarketData>           m_MarketData;
    boost::shared_ptr<TechIndicators>       m_TechInd;
    boost::shared_ptr<PortfoliosAndOrders>  m_PortAndOrders;
    boost::shared_ptr<VolSurfaces>          m_VolSurf;
    boost::shared_ptr<SystemState>          m_SystemState;
    boost::shared_ptr<MDI_Acknowledgement>  m_MDIAck;
    boost::shared_ptr<ThreadHealthMonitor>  m_ThrdHlthMon;

    //--------------------------------------------------
    PeriodicTask   m_PTask_PrintPrevTheoSgndPos;
    map<string,int> m_PTask_PrintPrevTheoSgndPosToken;

    AUTOCLOSEPOS     m_CloseAllPosIfOutsideTrdgHour;
    STY_RUN_MODE     m_StyRunMode;
    STY_DOMICILE_MKT m_StyDomicileMkt;

    YYYYMMDDHHMMSS   m_ymdhms_LastMonitorTime;
    vector<bool>     m_HasWarmedUpInternalData;
    vector<string>   m_FutFrontMthSym;
    vector<string>   m_TargetTradedSym;
    bool             m_AlreadyPerformedEODRoutines;
    vector<bool>     m_AlreadyPerformedJustBefTrdgTraining;

    char m_caStyName[HEALTHKEYLEN+1];
    bool m_TreatRegularMiniAsOne;
    RegularMiniCtrtManager m_RegularMiniCtrtMgr;
    bool m_FirstNewDay;
    map<string,double> m_PrevTheoSgndPos;
};

#endif /* STRATEGIES_STRATEGYBASE_H_ */
