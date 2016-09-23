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
#include <zmq.hpp>

#define B2_ROTATION_GROUP_MAX  38
#define B2_ROTATION_PICKTOPSYM 1

#define B2_KS_PRICE_FILTER_NDAYS          100
#define B2_KS_PRICE_FILTER_RISE_ADJFACTOR 0.5
#define B2_KS_PRICE_FILTER_FALL_ADJFACTOR 1

#define B2_HYPOTHESIS_TAYLOR              1
#define B2_HYPOTHESIS_TAYLOR1_TRDATCLOSE  11
#define B2_HYPOTHESIS_TAYLOR2_TRDATCLOSE  12
#define B2_HYPOTHESIS                     B2_HYPOTHESIS_TAYLOR

#define B2_SETPOSSIZETOFULLORZERO 0.2

#define B2_CURBOUTLIERMODE_FIXED 1
#define B2_CURBOUTLIERMODE_PCTL  2
#define B2_CURBOUTLIERMODE       B2_CURBOUTLIERMODE_FIXED

#define B2_RISEREGIME  0
#define B2_FALLREGIME  1

#define B2_FAVOUR_LONG_TRNG_PRD true
//--------------------------------------------------
// FIXME Testing
//--------------------------------------------------
#define B2_PURE_ROTATN_FOLW_LEADER true

#define B2_RPY_ARIMA  1
#define B2_RPY_TAYLOR 2

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
    double CalcPredictionTaylorTrdAtClose(const vector<double> &,const int,const double,const double,const double,const double,const int);
    bool TrainUpParamTaylor(const int, const YYYYMMDDHHMMSS &, const double, const double, const double, const vector<double> &, const vector<double> &, const vector<double> &, const vector<double> &, const CountingFunction &, const TRAINMODE, const WEIGHTING_SCHEME, const double);
    void SetParamBetaRange(const double, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double);
    void DoTraining(const int);
    bool GetOutliers(const vector<double> &, const int, const double, double &, double &);

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
    // ZMQ
    //--------------------------------------------------
    boost::scoped_ptr<zmq::context_t> m_zmqcontext;
    boost::scoped_ptr<zmq::socket_t> m_zmqsocket;
    //--------------------------------------------------

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
    int              m_PriceMode;
    double           m_CurbInSmpOutlier;
    int              m_AllowTrdDir;
    double           m_KSBound;
    double           m_KSRange;
    double           m_MinAnnVolPsnSz;
    double           m_beta_1_start;
    double           m_beta_1_end;
    double           m_beta_1_incremt;
    double           m_beta_2_start;
    double           m_beta_2_end;
    double           m_beta_2_incremt;
    double           m_beta_3_start;
    double           m_beta_3_end;
    double           m_beta_3_incremt;
    double           m_beta_4_start;
    double           m_beta_4_end;
    double           m_beta_4_incremt;
    double           m_PropOfBestParam;
    WEIGHTING_SCHEME m_WeightScheme;

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
    int              m_ChooseBestRotationGroupNum;
    double           m_ChooseBestRotationGroupProp;
    double           m_AvgAggSgndNotnlThresh;
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
    map<string,map<int,vector<TupObjParamVec> > >    m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1;
    map<string,map<int,vector<TupObjParamVec> > >    m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1;
    map<string,map<int,vector<TupObjParamVec> > >    m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2;
    map<string,map<int,vector<TupObjParamVec> > >    m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2;
    map<string,map<int,map<int,vector<double> > > >  m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1;
    map<string,map<int,map<int,vector<double> > > >  m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1;
    map<string,map<int,map<int,vector<double> > > >  m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2;
    map<string,map<int,map<int,vector<double> > > >  m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2;

    map<string,vector<double> >          m_map_HistoricalAvgPx;
    map<string,vector<double> >          m_map_HistoricalClose;
    map<string,vector<double> >          m_map_HistoricalOpenAvg;
    map<string,vector<double> >          m_map_HistoricalAvgPxRtn;
    map<string,vector<double> >          m_map_HistoricalCloseRtn;
    map<string,CountingFunction>         m_map_HistoricalNumOfRisingDaysCountAvgPx;
    map<string,double>                   m_map_OutlierLowerBound;
    map<string,double>                   m_map_OutlierUpperBound;

    map<string,deque<double> >           m_map_dq_GKYZ;
    map<string,bool>                     m_map_DoneSODTrade;
    map<string,bool>                     m_map_DoneEODTrade;
    map<string,bool>                     m_map_bTrainRetAvgPx;
    map<string,bool>                     m_map_bRisingRegimeAvgPx;
    double                               m_TotSharpeOfMethod;
    bool                                 m_B2_HasEnabledMinCommissionCheck;
    bool                                 m_B2_WhetherToRetain;
    TradingStrategyConfig::TrainingFreq  m_B2_TrainingFreq;
    int                                  m_B2_ActionTimeBefCloseInSec;
    int                                  m_B2_ActionTimeAftOpenInSec;
    vector<int>                          m_B2_FilterSMAPeriod;
    vector<Sma<double> >                 m_v_SMA_short;
    vector<Sma<double> >                 m_v_SMA_long;
    bool                                 m_MoveNextBestGroupUpIfNoSignal;
    bool                                 m_MoveNextBestStkInGrpUpIfNoSignal;
    map<string,map<double,int> >         m_RiseRegimeAvgSharpeOfBestPropInEachTraingPeriod;
    map<string,map<double,int> >         m_FallRegimeAvgSharpeOfBestPropInEachTraingPeriod;
    map<string,double>                   m_BestAvgSharpeOfBestPropRiseRegime;
    map<string,double>                   m_BestAvgSharpeOfBestPropFallRegime;
    double                               m_AvgSharpeThresholdRiseRegime;
    double                               m_AvgSharpeThresholdFallRegime;
    map<string,int>                      m_RiseRegimeBestTrainingPeriod;
    map<string,int>                      m_FallRegimeBestTrainingPeriod;

    //--------------------------------------------------
    // Convenience pointers
    //--------------------------------------------------
    vector<double>   *    m_HistoricalAvgPx;
    vector<double>   *    m_HistoricalClose;
    vector<double>   *    m_HistoricalOpenAvg;
    vector<double>   *    m_HistoricalAvgPxRtn;
    vector<double>   *    m_HistoricalCloseRtn;
    CountingFunction *    m_HistoricalNumOfRisingDaysCountAvgPx;

    bool *                m_DoneSODTrade;
    bool *                m_DoneEODTrade;
    bool *                m_bTrainRetAvgPx;
    bool *                m_bTrainRetClose;
    bool *                m_bRisingRegime;
    double *              m_dOutlierLowerBound;
    double *              m_dOutlierUpperBound;

    double                m_dStrengthCountTrdAtClose;
    double                m_dStrengthCountTrdAtOpen;
    long                  m_lNumOfTrngCombn;
    double                m_dAggUnsignedQty;
    double                m_dAggSignedQty;
    double                m_dGKYZVal;

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
    typedef struct TupAvgSharpeSymIdx {
      double m_avgsharpe;
      int m_symidx;
      TupAvgSharpeSymIdx(double a, const int si)
      {
        m_avgsharpe = a;
        m_symidx = si;
      }
      bool operator<(const TupAvgSharpeSymIdx & rhs) const { return this->m_avgsharpe < rhs.m_avgsharpe; }
      TupAvgSharpeSymIdx& operator=(const TupAvgSharpeSymIdx& tup)
      { if (this == &tup) return *this;
        m_avgsharpe = tup.m_avgsharpe;
        m_symidx = tup.m_symidx;
        return *this;
      }
    } TupAvgSharpeSymIdx;


    enum ASC_DESC {AD_ASCENDING=0,AD_DESCENDING};
    bool NDayRollingReturnReadyForAllSym(const int,const YYYYMMDD &);
    bool ArimaReturnReadyForAllSym(const int,const YYYYMMDD &);
    bool AggSgndQtyReadyForAllSym(const YYYYMMDD &);
    void AddNDayRollingReturn(const int,const YYYYMMDD &,const string &,const double);
    void AddArimaReturn(const int,const YYYYMMDD &,const string &,const double);
    void AddAvgSharpeOfSym(const int, const double, const int);
    void AddAggSgndQty(const YYYYMMDD &,const string &,const double);
    void AddAggSgndNotnl(const YYYYMMDD &,const int,const string &,const double);
    boost::optional<string> GetSymInGrpWithRank(const vector<map<YYYYMMDD,vector<TupRetSym> > > &, const int,const YYYYMMDD &,const int,const ASC_DESC);
    boost::optional<string> GetSymInGrpWithRank(const vector<TupAvgSharpeSymIdx> &, const int,const int);
    double GetAvgRollgRetOfGrp(const int,const YYYYMMDD &);
    double GetAvgArimaRetOfGrp(const int,const YYYYMMDD &);
    double GetAvgAggSgndNotnlOfGrp(const int,const YYYYMMDD &);
    double GetB2ForecastFromRPy(const YYYYMMDD &,const string &,const int,const double,const double,const double,const double);
    void ReadRotationGroupFile(const string &);
    string                                      m_RotationGroupFile;
    map<YYYYMMDD,vector<int> >                  m_RotationGroupFromFile;
    vector<int>                                 m_RotationGroup;
    map<string,int>                             m_SymRotationGroup;
    vector<int>                                 m_vRoleOfSym;
    map<string,int>                             m_mapRoleOfSym;
    vector<map<YYYYMMDD,set<string> > >         m_AllAvbSymForRollingBasket;
    map<YYYYMMDD,set<string> >                  m_AllAvbSym;
    map<YYYYMMDD,set<string> >                  m_AllSymWithUpdateDataAtClose;
    map<YYYYMMDD,set<string> >                  m_AllSymWithUpdateDataAtOpen;
    vector<map<YYYYMMDD,vector<TupRetSym> > >   m_SymRankedByRollingReturn;
    vector<map<YYYYMMDD,vector<TupRetSym> > >   m_SymRankedByArimaReturn;
    vector<TupAvgSharpeSymIdx>                  m_SymRankedByAvgSharpeRiseRgm;
    vector<TupAvgSharpeSymIdx>                  m_SymRankedByAvgSharpeFallRgm;
    vector<map<YYYYMMDD,map<string,double> > >  m_SymAndRollingReturn;
    vector<map<YYYYMMDD,map<string,double> > >  m_SymAndArimaReturn;
    vector<map<YYYYMMDD,set<string> > >         m_MaintainPosWithinGrp;
    map<YYYYMMDD,set<int> >                     m_MaintainGrp;
    map<YYYYMMDD,vector<double> >               m_AvgRtnOfGrp;
    map<YYYYMMDD,vector<boost::optional<string> > >      m_RttnGrpWithSgnl;
    map<YYYYMMDD,vector<TupRetSym> >            m_GrpRtnAndLeadingSym;
    map<YYYYMMDD,map<string,double> >           m_SymAggSgndQty;
    map<YYYYMMDD,map<int,map<string,double> > > m_AggSgndNotnlOfSym;
    set<string>                                 m_StkPicks;
    SSet<YYYYMMDD>                              m_HasPerformedRotationLogic;
    bool                                        m_PerformCppTaylor;
    bool                                        m_SymbolStickiness;
    double                                      m_MinSharpeThreshFilter;
    double                                      m_TaylorWeight;
    double                                      m_ArimaWeight;
    //--------------------------------------------------

};

#endif
