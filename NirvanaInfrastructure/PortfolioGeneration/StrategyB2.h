#ifndef PORTFOLIOGENERATION_STRATEGYB2_H_
#define PORTFOLIOGENERATION_STRATEGYB2_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include "StrategyBase.h"
#include "Acct.h"
#include "CommissionCalculator.h"
#include "CountingFunction.h"
#include "CountingFunction.h"
#include "CPnLHist.h"
#include "Ema.hpp"
#include "HKFE.h"
#include "KolmogorovSmirnov.h"
#include "MarketData.h"
#include "MDI_Acknowledgement.h"
#include "NYSE.h"
#include "SystemConfig.h"
#include "SystemState.h"
#include "Sma.hpp"

#define B2_ROTATION_PICKTOPSYM 1

#define B2_KS_PRICE_FILTER_NDAYS 100
#define B2_KS_PRICE_FILTER_RISE_ADJFACTOR 0.5
#define B2_KS_PRICE_FILTER_FALL_ADJFACTOR 1

#define B2_SKIPMACHLEARNING false
#define B2_STICINESS_ON     true
#define B2_MIN_INSMP_SHARPEREQ   -0.03

#define B2_HYPOTHESIS_TAYLOR               1
#define B2_HYPOTHESIS_TAYLOR1_TRDATCLOSE   11
#define B2_HYPOTHESIS_TAYLOR2_TRDATCLOSE   12
#define B2_HYPOTHESIS_TAYLOR1_TRDATOPEN    13
#define B2_HYPOTHESIS_SMA                  3
#define B2_HYPOTHESIS                      B2_HYPOTHESIS_TAYLOR

#define B2_SETPOSSIZETOFULLORZERO 0.5
#define B2_TRADEATOPEN_ON  false
#define B2_TRADEATCLOSE_ON true

#define B2_CURBOUTLIERMODE_FIXED 1
#define B2_CURBOUTLIERMODE_PCTL  2
#define B2_CURBOUTLIERMODE       B2_CURBOUTLIERMODE_FIXED

class StrategyB2 : public StrategyBase {
  public:
    typedef struct {
      double trainingperiod;
      double beta_1;
      double beta_2;
      double beta_3;
      double beta_4;
      const vector<double> * v_hist_trfpx;
      const vector<double> * v_hist_close;
      const vector<double> * v_hist_openavg;
      double * pdAvgSquaredError;
      double * pdAvgPnL;
      double * pdSharpe;
      double * pdFinalEstimate;
      WEIGHTING_SCHEME ws;
      double outlierLowerBound;
      double outlierUpperBound;
      int hypothesis;
    } GetEstParam;

    //--------------------------------------------------
    // BestParam
    // because the objective (e.g. Sharpe ratio) of 2 different hypotheses can be exactly the same
    // we cannot simply use a map for sorting them
    //--------------------------------------------------
    typedef struct TupObjParamVec {
      double m_obj;
      int m_paramvecidx;
      TupObjParamVec(double o, const int i)
      {
        m_obj = o;
        m_paramvecidx = i;
      }
      bool operator<(const TupObjParamVec & rhs) const { return this->m_obj < rhs.m_obj; }
      TupObjParamVec & operator=(const TupObjParamVec & tup)
      { if (this == &tup) return *this;
        m_obj = tup.m_obj;
        m_paramvecidx = tup.m_paramvecidx;
        return *this;
      }
    } TupObjParamVec;

    enum TRAINMODE {TM_MINSQERR=0,TM_MAXPROFIT=1,TM_MAXSHARPE=2};
    StrategyB2();
    virtual ~StrategyB2();

    bool GetEstimate(const double, const double, const double, const double, const double, const vector<double> &, const vector<double> &, const vector<double> &, double *, double *, double *, double *, WEIGHTING_SCHEME ws, const double, const double, const int);
    bool GetEstimate(const GetEstParam &);
    double CalcPredictionTaylorTrdAtOpen(const vector<double> &,const vector<double> &,const int,const double,const double,const double,const double,const int);
    double CalcPredictionTaylorTrdAtClose(const vector<double> &,const int,const double,const double,const double,const double,const int);
    double CalcPredictionSMA(const vector<double> &,const int,const double,const double,const double,const double);
    bool TrainUpParamTaylor( const string &, const YYYYMMDDHHMMSS &, const double, const double, const vector<double> &, const vector<double> &, const vector<double> &, const vector<double> &, const CountingFunction &, const TRAINMODE, const WEIGHTING_SCHEME, vector<TupObjParamVec> &,  vector<TupObjParamVec> &,  vector<TupObjParamVec> &,  vector<TupObjParamVec> &,  vector<TupObjParamVec> &,  vector<TupObjParamVec> &, map<int,vector<double> > &, map<int,vector<double> > &, map<int,vector<double> > &, map<int,vector<double> > &, map<int,vector<double> > &, map<int,vector<double> > &, const double);
    // bool TrainUpParamSMA(const string &,const YYYYMMDDHHMMSS &, const double, const double, const double, const double, const vector<double> &, const vector<double> &, const CountingFunction &, const TRAINMODE, const WEIGHTING_SCHEME, map<double,vector<double> > &, map<double,vector<double> > &, const double);
    void SetParamBetaRange(const double, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double);
    void DoTraining(const int);
    bool GetOutliers(const vector<double> &, const int, const double, double &, double &);

  protected:

    //--------------------------------------------------
    // Strategy objects
    //--------------------------------------------------
    // double SML_ADJ_BETA12;
    // double BIG_ADJ_BETA12;
    // double SML_ADJ_BETA34;
    // double BIG_ADJ_BETA34;

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
    vector<double>           m_NotionalAmt;
    vector<int>              m_PriceMode;
    vector<double>           m_CurbInSmpOutlier;
    vector<int>              m_AllowTrdDir;
    vector<double>           m_KSBound;
    vector<double>           m_KSRange;
    vector<double>           m_MinAnnVolPsnSz;
    vector<double>           m_TrainingPeriod1;
    vector<double>           m_TrainingPeriod2;
    // vector<double>           m_TrainingPeriod3;
    // vector<double>           m_TrainingPeriod4;
    vector<double>           m_TrainingPeriodMax;
    vector<double>           m_beta_1_start;
    vector<double>           m_beta_1_end;
    vector<double>           m_beta_1_incremt;
    vector<double>           m_beta_2_start;
    vector<double>           m_beta_2_end;
    vector<double>           m_beta_2_incremt;
    vector<double>           m_beta_3_start;
    vector<double>           m_beta_3_end;
    vector<double>           m_beta_3_incremt;
    vector<double>           m_beta_4_start;
    vector<double>           m_beta_4_end;
    vector<double>           m_beta_4_incremt;
    vector<double>           m_PropOfBestParam;
    vector<WEIGHTING_SCHEME> m_WeightScheme;


    //--------------------------------------------------
    PeriodicTask    m_PTask_PrintStyActionTime;
    PeriodicTask    m_PTask_PrintAllAvbSym;
    map<string,int> m_PTask_PrintStyActionTimeToken;
    int             m_PTask_PrintAllAvbSymToken;
    PeriodicTask    m_PTask_PrintTrainingResult;
    int             m_PTask_PrintTrainingResultToken;
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
    Option<double>   m_LongOnlyWhenClosePriceBelowAvgPrice;
    Option<double>   m_ShortOnlyWhenClosePriceAboveAvgPrice;
    Option<double>   m_LongOnlyWhenAvgPriceReturnAbove;
    Option<double>   m_LongOnlyWhenGrpAvgReturnAbove;
    Option<double>   m_ShortOnlyWhenAvgPriceReturnBelow;
    int              m_ChooseBestNRotationGroup;
    double           m_AvgAggSgndNotnlThresh;
    HHMMSS           m_hms_OpenStart;
    HHMMSS           m_hms_OpenEnd;
    HHMMSS           m_hms_CloseStart;
    HHMMSS           m_hms_CloseEnd;
    set<string>      m_TradedSymbolsTradeAtOpen;

    //--------------------------------------------------
    map<string,vector<double> >               m_map_HistoricalAvgPx;
    map<string,vector<double> >               m_map_HistoricalClose;
    map<string,vector<double> >               m_map_HistoricalOpenAvg;
    map<string,vector<double> >               m_map_HistoricalAvgPxRtn;
    map<string,vector<double> >               m_map_HistoricalCloseRtn;
    map<string,CountingFunction>              m_map_HistoricalNumOfRisingDaysCountAvgPx;
    map<string,double>                        m_map_OutlierLowerBound;
    map<string,double>                        m_map_OutlierUpperBound;

    map<string,vector<TupObjParamVec> >  m_map_vec_BestParamSetBeta1Beta3AvgPx_trdatopen_hypo1;
    map<string,vector<TupObjParamVec> >  m_map_vec_BestParamSetBeta2Beta4AvgPx_trdatopen_hypo1;
    map<string,vector<TupObjParamVec> >  m_map_vec_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1;
    map<string,vector<TupObjParamVec> >  m_map_vec_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1;
    map<string,vector<TupObjParamVec> >  m_map_vec_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2;
    map<string,vector<TupObjParamVec> >  m_map_vec_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2;

    map<string,map<int,vector<double> > >  m_map_map_BestParamSetBeta1Beta3AvgPx_trdatopen_hypo1;
    map<string,map<int,vector<double> > >  m_map_map_BestParamSetBeta2Beta4AvgPx_trdatopen_hypo1;
    map<string,map<int,vector<double> > >  m_map_map_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1;
    map<string,map<int,vector<double> > >  m_map_map_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1;
    map<string,map<int,vector<double> > >  m_map_map_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2;
    map<string,map<int,vector<double> > >  m_map_map_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2;

    map<string,deque<double> >                m_map_dq_GKYZ;
    map<string,bool>                          m_map_DoneSODTrade;
    map<string,bool>                          m_map_DoneEODTrade;
    map<string,bool>                          m_map_bTrainRetAvgPx;
    map<string,bool>                          m_map_bRisingRegimeAvgPx;
    double                                    m_TotSharpeOfMethod;
    bool                                      m_B2_HasEnabledMinCommissionCheck;
    bool                                      m_B2_WhetherToRetain;
    TradingStrategyConfig::TrainingFreq       m_B2_TrainingFreq;
    int                                       m_B2_ActionTimeBefCloseInSec;
    int                                       m_B2_ActionTimeAftOpenInSec;
    vector<int>                               m_B2_FilterSMAPeriod;
    vector<Sma<double> >                      m_v_SMA_short;
    vector<Sma<double> >                      m_v_SMA_long;
    bool                                      m_MoveNextBestGroupUpIfNoSignal;
    bool                                      m_MoveNextBestStkInGrpUpIfNoSignal;

    //--------------------------------------------------
    // Convenience pointers
    //--------------------------------------------------
    vector<double>   *            m_HistoricalAvgPx;
    vector<double>   *            m_HistoricalClose;
    vector<double>   *            m_HistoricalOpenAvg;
    vector<double>   *            m_HistoricalAvgPxRtn;
    vector<double>   *            m_HistoricalCloseRtn;
    CountingFunction *            m_HistoricalNumOfRisingDaysCountAvgPx;

    vector<TupObjParamVec> * m_p_vec_BestParamSetBeta1Beta3AvgPx_trdatopen_hypo1;
    vector<TupObjParamVec> * m_p_vec_BestParamSetBeta2Beta4AvgPx_trdatopen_hypo1;
    vector<TupObjParamVec> * m_p_vec_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1;
    vector<TupObjParamVec> * m_p_vec_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1;
    vector<TupObjParamVec> * m_p_vec_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2;
    vector<TupObjParamVec> * m_p_vec_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2;

    map<int,vector<double> > * m_p_map_BestParamSetBeta1Beta3AvgPx_trdatopen_hypo1;
    map<int,vector<double> > * m_p_map_BestParamSetBeta2Beta4AvgPx_trdatopen_hypo1;
    map<int,vector<double> > * m_p_map_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1;
    map<int,vector<double> > * m_p_map_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1;
    map<int,vector<double> > * m_p_map_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2;
    map<int,vector<double> > * m_p_map_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2;

    bool *                        m_DoneSODTrade;
    bool *                        m_DoneEODTrade;
    bool *                        m_bTrainRetAvgPx;
    bool *                        m_bTrainRetClose;
    bool *                        m_bRisingRegimeAvgPx;
    double *                      m_dOutlierLowerBound;
    double *                      m_dOutlierUpperBound;

    double                        m_dStrengthCountTrdAtClose;
    double                        m_dStrengthCountTrdAtOpen;
    long                          m_lNumOfTrngCombnAvgPx;
    double                        m_dAggUnsignedQty;
    double                        m_dAggSignedQty;
    double                        m_dGKYZVal;

    // //--------------------------------------------------
    // // Special ES + SPY mode
    // //--------------------------------------------------
    // int    m_ES_SPY_Together_SPYidx;
    // double m_SPY_Px;
    // int    m_NoOfSgndESCtrtReqd;
    // //--------------------------------------------------

    //--------------------------------------------------
    // Rotation
    //--------------------------------------------------
    typedef struct TupRetSym {
      double m_return;
      char _m_symbol[16];
      int m_rttngrp;
      TupRetSym(double r, const string s, const int g)
      {
        m_return = r;
        // memset (_m_symbol,'\0',16);
        strcpy(_m_symbol,s.c_str());
        _m_symbol[s.length()] = '\0';
        m_rttngrp = g;
      }
      string m_symbol() const
      {
        return string(_m_symbol);
      }
      bool operator<(const TupRetSym & rhs) const { return this->m_return < rhs.m_return; }
      TupRetSym& operator=(const TupRetSym& tup)
      { if (this == &tup) return *this;
        m_return = tup.m_return;
        strcpy(_m_symbol,tup._m_symbol);
        m_rttngrp = tup.m_rttngrp;
        return *this;
      }
    } TupRetSym;

    enum ASC_DESC {AD_ASCENDING=0,AD_DESCENDING};
    bool NDayRollingReturnReadyForAllSym(const int,const YYYYMMDD &);
    bool AggSgndQtyReadyForAllSym(const YYYYMMDD &);
    void AddNDayRollingReturn(const int,const YYYYMMDD &,const string &,const double);
    void AddAggSgndQty(const YYYYMMDD &,const string &,const double);
    void AddAggSgndNotnl(const YYYYMMDD &,const int,const string &,const double);
    Option<string> GetSymInGrpRankByRet(const int,const YYYYMMDD &,const int,const ASC_DESC);
    double GetAvgRollgRetOfGrp(const int,const YYYYMMDD &);
    double GetAvgAggSgndNotnlOfGrp(const int,const YYYYMMDD &);
    vector<int>                                 m_RotationGroup;
    vector<int>                                 m_vRoleOfSym;
    map<string,int>                             m_mapRoleOfSym;
    vector<Option<string> >                     m_vGroupRepresentative;
    vector<map<YYYYMMDD,set<string> > >         m_AllAvbSymForRollingBasket;
    map<YYYYMMDD,set<string> >                  m_AllAvbSym;
    map<YYYYMMDD,set<string> >                  m_AllSymWithUpdateDataAtClose;
    map<YYYYMMDD,set<string> >                  m_AllSymWithUpdateDataAtOpen;
    vector<map<YYYYMMDD,vector<TupRetSym> > >   m_SymRankedByRollingReturn;
    vector<map<YYYYMMDD,map<string,double> > >  m_SymAndRollingReturn;
    vector<map<YYYYMMDD,set<string> > >         m_MaintainPosWithinGrp;
    map<YYYYMMDD,set<int> >                     m_MaintainGrp;
    map<YYYYMMDD,vector<double> >               m_AvgRtnOfGrp;
    map<YYYYMMDD,vector<Option<string> > >      m_RttnGrpWithSgnl;
    map<YYYYMMDD,vector<TupRetSym> >            m_GrpRtnAndLeadingSym;
    map<YYYYMMDD,map<string,double> >           m_SymAggSgndQty;
    map<YYYYMMDD,map<int,map<string,double> > > m_AggSgndNotnlOfSym;
    set<string>                                 m_StkPicks;
    SSet<YYYYMMDD>                              m_HasPerformedRotationLogic;
    //--------------------------------------------------

};

#endif
