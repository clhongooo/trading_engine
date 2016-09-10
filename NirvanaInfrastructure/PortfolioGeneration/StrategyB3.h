#ifndef PORTFOLIOGENERATION_STRATEGYB3_H_
#define PORTFOLIOGENERATION_STRATEGYB3_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include "StrategyBase.h"
#include "Acct.h"
#include "CommissionCalculator.h"
#include "CPnLHist.h"
#include "HKFE.h"
#include "MarketData.h"
#include "MDI_Acknowledgement.h"
#include "NYSE.h"
#include "SystemConfig.h"
#include "SystemState.h"

#define B3_CURBOUTLIERMODE_FIXED 1
#define B3_CURBOUTLIERMODE_PCTL  2
#define B3_CURBOUTLIERMODE       B3_CURBOUTLIERMODE_FIXED

#define B3_MAX_N 100
#define B3_CHOOSE_N 100*100/2/10
#define B3_SHORTWEIGHT 100

class StrategyB3 : public StrategyBase {
  public:
    StrategyB3();
    virtual ~StrategyB3();

    bool TrainUpParam(const int, const YYYYMMDDHHMMSS &, const double, const double, const double, const vector<double> &, const vector<double> &, const vector<double> &, const vector<double> &, const vector<vector<double> > &);
    void SetParamBetaRange(const double, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double);
    void DoTraining(const int);

  protected:

    //--------------------------------------------------
    // Strategy objects
    //--------------------------------------------------

    //--------------------------------------------------
    double m_b1_low;
    double m_b1_high;
    double m_b1_adj;
    double m_b2_low;
    double m_b2_high;
    double m_b2_adj;
    double m_b3_low;
    double m_b3_high;
    double m_b3_adj;
    double m_b4_low;
    double m_b4_high;
    double m_b4_adj;

    //--------------------------------------------------
    virtual void StrategySetup();
    virtual void ReadParam();
    virtual void ParamSanityCheck();
    virtual void StartOfDayInit();
    virtual void EndOfDayCleanUp();
    virtual void UnsetConvenienceVarb();
    virtual void SetConvenienceVarb(const int iTradSym);
    virtual bool SkipSubseqProcessingForSymbol(const int iTradSym,string & sReason);
    virtual void InitialWarmUp(const int iTradSym);
    virtual void UpdateInternalData(const int iTradSym);
    virtual void UpdateInternalDataTrng(const int iTradSym);
    virtual void LoadOptimalParamsFromTrainingResults();
    virtual void EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket);
    virtual void DetermineRegime(const int iTradSym);
    virtual void PreTradePreparation(const int iTradSym);
    virtual void CalcPosSize(const int iTradSym);
    virtual void PerformTrainingJustBeforeTrading(const int iTradSym);
    //--------------------------------------------------

    //--------------------------------------------------
    // Strategy objects
    //--------------------------------------------------
    int              m_TrainingPeriodMin;
    int              m_TrainingPeriodMax;
    int              m_TrainingPeriodStepSize;
    double           m_NotionalAmt;
    double           m_MaxNotionalAmtEach;

    //--------------------------------------------------
    PeriodicTask     m_PTask_PrintStyActionTime;
    PeriodicTask     m_PTask_PrintAllAvbSym;
    map<string,int>  m_PTask_PrintStyActionTimeToken;
    int              m_PTask_PrintAllAvbSymToken;
    PeriodicTask     m_PTask_PrintTrainingResult;
    int              m_PTask_PrintTrainingResultToken;
    //--------------------------------------------------

    boost::scoped_ptr<YYYYMMDDHHMMSS> m_p_ymdhms_SysTime_Local;
    string           m_TheoCPnLHistFolder;
    vector<CPnLHist> m_TheoCPnLHist;
    string           m_TheoPosFolder;
    vector<Acct>     m_TheoAcct;
    bool             m_PersistTheoPosCPnL;
    int              m_RotationMode;
    bool             m_RotationModeTradeHighestReturn;
    bool             m_RotationModeUseVolyAdjdReturn;
    int              m_RotationModeUseNDayReturn;
    HHMMSS           m_hms_OpenStart;
    HHMMSS           m_hms_OpenEnd;
    HHMMSS           m_hms_CloseStart;
    HHMMSS           m_hms_CloseEnd;
    set<string>      m_TradedSymbolsTradeAtOpen;
    int              m_NumOfAvbGrp;
    boost::optional<double>   m_LongOnlyWhenClosePriceBelowAvgPrice;
    boost::optional<double>   m_ShortOnlyWhenClosePriceAboveAvgPrice;
    boost::optional<double>   m_LongOnlyWhenAvgPriceReturnAbove;
    boost::optional<double>   m_LongOnlyWhenGrpAvgReturnAbove;
    boost::optional<double>   m_ShortOnlyWhenAvgPriceReturnBelow;

    //--------------------------------------------------
    // map<string,vector<int> > m_BestParam;
    map<string,vector<Tuple3<double,int,int> > > m_BestParam;
    // map<string,vector<Tuple4<double,int,int,int> > > m_BestParam;

    map<string,vector<double> >            m_map_HistoricalAvgPx;
    map<string,vector<double> >            m_map_HistoricalClose;
    map<string,vector<double> >            m_map_HistoricalOpenAvg;
    map<string,vector<double> >            m_map_HistoricalAvgPxRtn;
    map<string,vector<double> >            m_map_HistoricalCloseRtn;
    map<string,vector<vector<double> > >   m_map_HistoricalFallThenLongCPnL;
    map<string,double>                     m_map_OutlierLowerBound;
    map<string,double>                     m_map_OutlierUpperBound;

    map<string,deque<double> >             m_map_dq_GKYZ;
    map<string,bool>                       m_map_DoneSODTrade;
    map<string,bool>                       m_map_DoneEODTrade;
    map<string,bool>                       m_map_bTrainRetAvgPx;
    map<string,bool>                       m_map_bRisingRegimeAvgPx;
    double                                 m_TotSharpeOfMethod;
    bool                                   m_B3_HasEnabledMinCommissionCheck;
    bool                                   m_B3_WhetherToRetain;
    TradingStrategyConfig::TrainingFreq    m_B3_TrainingFreq;
    int                                    m_B3_ActionTimeBefCloseInSec;
    int                                    m_B3_ActionTimeAftOpenInSec;
    vector<int>                            m_B3_FilterSMAPeriod;
    vector<Sma<double> >                   m_v_SMA_short;
    vector<Sma<double> >                   m_v_SMA_long;
    bool                                   m_MoveNextBestGroupUpIfNoSignal;
    bool                                   m_MoveNextBestStkInGrpUpIfNoSignal;
    map<string,map<double,int> >           m_RiseRegimeAvgSharpeOfBestPropInEachTraingPeriod;
    map<string,map<double,int> >           m_FallRegimeAvgSharpeOfBestPropInEachTraingPeriod;
    map<string,double>                     m_BestAvgSharpeOfBestPropRiseRegime;
    map<string,double>                     m_BestAvgSharpeOfBestPropFallRegime;
    double                                 m_AvgSharpeThresholdRiseRegime;
    double                                 m_AvgSharpeThresholdFallRegime;
    map<string,int>                        m_RiseRegimeBestTrainingPeriod;
    map<string,int>                        m_FallRegimeBestTrainingPeriod;

    //--------------------------------------------------
    // Convenience pointers
    //--------------------------------------------------
    vector<double>             * m_HistoricalAvgPx;
    vector<double>             * m_HistoricalClose;
    vector<double>             * m_HistoricalOpenAvg;
    vector<double>             * m_HistoricalAvgPxRtn;
    vector<double>             * m_HistoricalCloseRtn;
    vector<vector<double> >    * m_HistoricalFallThenLongCPnL;

    bool *                m_DoneSODTrade;
    bool *                m_DoneEODTrade;
    bool *                m_bTrainRetAvgPx;
    bool *                m_bTrainRetClose;
    double *              m_dOutlierLowerBound;
    double *              m_dOutlierUpperBound;

    double                m_dStrengthCountTrdAtClose;
    double                m_dStrengthCountTrdAtOpen;
    long                  m_lNumOfTrngCombn;
    double                m_dAggUnsignedQty;
    double                m_dAggSignedQty;
    double                m_dGKYZVal;

    //--------------------------------------------------

};

#endif /* PORTFOLIOGENERATION_STRATEGYB3_H_ */
