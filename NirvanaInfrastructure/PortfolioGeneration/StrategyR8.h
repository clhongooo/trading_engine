#ifndef PORTFOLIOGENERATION_STRATEGYR8_H_
#define PORTFOLIOGENERATION_STRATEGYR8_H_

#include "PCH.h"
#include "Constants.h"
#include "StrategyBase.h"
#include "SystemState.h"
#include "SystemConfig.h"
#include "MDI_Acknowledgement.h"
#include "MarketData.h"
#include "TechIndicators.h"
#include "PortfoliosAndOrders.h"
#include "LinRegr.h"
#include "HighLow.hpp"

class StrategyR8 : public StrategyBase {
  public:
    StrategyR8();
    virtual ~StrategyR8();

  protected:
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
    virtual void AdjustSamplingInterval(const int iTradSym);
    virtual void EndOfDayTrainingRoutine(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket);

    //--------------------------------------------------
    // Strategy param
    //--------------------------------------------------
    vector<double> m_DownTrendWindowInSec;
    vector<double> m_DownTrendMagnitude;
    vector<double> m_DownTrendSlopeThreshold;
    vector<double> m_ReboundWindowInSec;
    vector<double> m_TakeProfitMul;
    vector<double> m_StopLossMul;

    //--------------------------------------------------
    // Strategy objects
    //--------------------------------------------------
    vector<double>           m_EntryPrice;
    vector<double>           m_StopLossPrice;
    vector<double>           m_TakeProfitPrice;
    vector<LinRegr>          m_vLinRegr;
    vector<HighLow<double> > m_vHighLowPrice;
    vector<HighLow<double> > m_vHighLowSlope;


};


#endif /* PORTFOLIOGENERATION_STRATEGYR8_H_ */
