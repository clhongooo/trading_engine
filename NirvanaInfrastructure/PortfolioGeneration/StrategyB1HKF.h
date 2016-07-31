#ifndef PORTFOLIOGENERATION_STRATEGYB1HKF_H_
#define PORTFOLIOGENERATION_STRATEGYB1HKF_H_

#include "PCH.h"
#include "Constants.h"
#include "StrategyB1.h"
#include "CPnLHist.h"
#include "Acct.h"

#define B1_METH_AVGPX 0
#define B1_METH_CLOSE 1

#define B1_DOUBLESIZE_NUMOFDAYS 4

class StrategyB1_HKF : public StrategyB1 {
  public:
    StrategyB1_HKF();
    virtual ~StrategyB1_HKF();

  protected:
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

  private:
    set<int> ComputeUnionOfMAPeriods( const int, const int, const int, const int, const int, const int);

    //--------------------------------------------------
    // Strategy objects
    //--------------------------------------------------
    vector<double>           m_NotionalAmt;
    vector<double>           m_CurbInSmpRtn;
    vector<double>           m_TrainingPeriod;
    vector<double>           m_MAPeriod_1_start;
    vector<double>           m_MAPeriod_1_end;
    vector<double>           m_MAPeriod_1_increment;
    vector<double>           m_MAPeriod_2_start;
    vector<double>           m_MAPeriod_2_end;
    vector<double>           m_MAPeriod_2_increment;
    vector<double>           m_PropOfBestParam;
    vector<WEIGHTING_SCHEME> m_WeightScheme;

    //--------------------------------------------------
    map<string,vector<double> >          m_map_HistoricalAvgPx;
    map<string,vector<double> >          m_map_HistoricalClose;
    map<string,set<YYYYMMDD>  >          m_map_HistoricalPricesDates;

    map<string,map<double,int> >         m_map_BestParamSetUndlyDeriv1;
    map<string,map<double,int> >         m_map_BestParamSetDerivDeriv1;
    map<string,map<double,int> >         m_map_BestParamSetUndlyDeriv2;
    map<string,bool>                     m_map_bTrainRetAvgPx;
    map<int,double>                      m_map_TotSharpeOfMethod;
    bool                                 m_B1_WhetherToRetain;
    TradingStrategyConfig::TrainingFreq  m_B1_TrainingFreq;
    map<string,vector<Sma<double> > >    m_mv_Sma;
    map<string,set<int> >                m_map_MAPeriodUnion;
    map<string,bool>                     m_map_DoneEODTrade;


    //--------------------------------------------------
    // Convenience pointers
    //--------------------------------------------------
    vector<double>   *     m_HistoricalUndlyAvgPx;
    vector<double>   *     m_HistoricalUndlyClose;
    set<YYYYMMDD>    *     m_HistoricalUndlyPricesDates;
    vector<double>   *     m_HistoricalAvgPx;
    vector<double>   *     m_HistoricalClose;
    set<YYYYMMDD>    *     m_HistoricalPricesDates;
    map<double,int>  *     m_p_map_BestParamSetUndlyDeriv1;
    map<double,int>  *     m_p_map_BestParamSetDerivDeriv1;
    map<double,int>  *     m_p_map_BestParamSetUndlyDeriv2;
    bool *                 m_bTrainRetAvgPx;
    double                 m_dStrengthCountAvgPx;
    double                 m_dAggUnsignedQty;
    double                 m_dAggSignedQty;
    bool *                 m_DoneEODTrade;

};

#endif
