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


#define B2_METH_AVGPX 0
#define B2_METH_CLOSE 1

#define B2_PRICEMETH_AVGPX      1
#define B2_PRICEMETH_AVGPXCLOSE 2

#define B2_ROTATION_PICKTOPSYM 1

#define B2_KS_PRICE_FILTER_NDAYS 100
#define B2_KS_PRICE_FILTER_RISE_ADJFACTOR 0.5
#define B2_KS_PRICE_FILTER_FALL_ADJFACTOR 1

#define B2_SKIPMACHLEARNING false

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
      double * pdAvgSquaredError;
      double * pdAvgPnL;
      double * pdSharpe;
      double * pdFinalEstimate;
      WEIGHTING_SCHEME ws;
      double curbInSmpReturn;
    } GetEstParam;

    enum TRAINMODE {TM_MINSQERR=0,TM_MAXPROFIT=1,TM_MAXSHARPE=2};
    StrategyB2();
    virtual ~StrategyB2();

    bool GetEstimate(const double, const double, const double, const double, const double, const vector<double> &, const vector<double> &, double *, double *, double *, double *, WEIGHTING_SCHEME ws, const double);
    bool GetEstimate(const GetEstParam &);
    bool TrainUpParam(const YYYYMMDDHHMMSS &, const double, const double, const double, const double, double &, double &, double &, double &, const vector<double> &, const vector<double> &, const CountingFunction &, const TRAINMODE, const WEIGHTING_SCHEME, map<double,vector<double> > &, map<double,vector<double> > &, const double);
    void SetParamBetaRange(const double, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double);
    void DoTraining(const int);

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
    vector<double>           m_beta_1;
    vector<double>           m_beta_2;
    vector<double>           m_beta_3;
    vector<double>           m_beta_4;

    vector<double>           m_NotionalAmt;
    vector<int>              m_PriceMode;
    vector<double>           m_CurbInSmpRtn;
    vector<int>              m_AllowTrdDir;
    vector<double>           m_KSBound;
    vector<double>           m_KSRange;
    vector<double>           m_MinAnnVolPsnSz;
    vector<double>           m_TrainingPeriod1;
    vector<double>           m_TrainingPeriod2;
    vector<double>           m_TrainingPeriod3;
    vector<double>           m_TrainingPeriod4;
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

    //--------------------------------------------------
    map<string,vector<double> >               m_map_HistoricalAvgPx;
    map<string,vector<double> >               m_map_HistoricalClose;
    map<string,vector<double> >               m_map_HistoricalAvgPxRtn;
    map<string,vector<double> >               m_map_HistoricalCloseRtn;
    map<string,CountingFunction>              m_map_HistoricalNumOfRisingDaysCountAvgPx;
    map<string,CountingFunction>              m_map_HistoricalNumOfRisingDaysCountClose;

    map<string,map<double,vector<double> > >  m_map_BestParamSetBeta1Beta3AvgPx;
    map<string,map<double,vector<double> > >  m_map_BestParamSetBeta2Beta4AvgPx;
    map<string,map<double,vector<double> > >  m_map_BestParamSetBeta1Beta3Close;
    map<string,map<double,vector<double> > >  m_map_BestParamSetBeta2Beta4Close;
    map<string,deque<double> >                m_map_dq_GKYZ;
    map<string,bool>                          m_map_DoneEODTrade;
    map<string,bool>                          m_map_bTrainRetAvgPx;
    map<string,bool>                          m_map_bTrainRetClose;
    map<string,bool>                          m_map_bRisingRegimeAvgPx;
    map<string,bool>                          m_map_bRisingRegimeClose;
    map<int,double>                           m_map_TotSharpeOfMethod;
    bool                                      m_B2_HasEnabledMinCommissionCheck;
    bool                                      m_B2_WhetherToRetain;
    TradingStrategyConfig::TrainingFreq       m_B2_TrainingFreq;
    int                                       m_B2_ActionTimeBefCloseInSec;
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
    vector<double>   *            m_HistoricalAvgPxRtn;
    vector<double>   *            m_HistoricalCloseRtn;
    CountingFunction *            m_HistoricalNumNumOfRisingDaysCountAvgPx;
    CountingFunction *            m_HistoricalNumNumOfRisingDaysCountClose;
    map<double,vector<double> > * m_p_map_BestParamSetBeta1Beta3AvgPx;
    map<double,vector<double> > * m_p_map_BestParamSetBeta2Beta4AvgPx;
    map<double,vector<double> > * m_p_map_BestParamSetBeta1Beta3Close;
    map<double,vector<double> > * m_p_map_BestParamSetBeta2Beta4Close;
    bool *                        m_DoneEODTrade;
    bool *                        m_bTrainRetAvgPx;
    bool *                        m_bTrainRetClose;
    bool *                        m_bRisingRegimeAvgPx;
    bool *                        m_bRisingRegimeClose;
    double                        m_dStrengthCountAvgPx;
    double                        m_dStrengthCountClose;
    long                          m_lNumOfTrngCombnAvgPx;
    long                          m_lNumOfTrngCombnClose;
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
    map<YYYYMMDD,set<string> >                  m_AllSymWithUpdateData;
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
