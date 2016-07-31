#ifndef PORTFOLIOGENERATION_STRATEGYTEST_H_
#define PORTFOLIOGENERATION_STRATEGYTEST_H_

#include "PCH.h"
#include "Constants.h"
#include "StrategyBase.h"

class StrategyTest : public StrategyBase {
  public:
    StrategyTest();
    virtual ~StrategyTest();

  protected:
    virtual void StrategySetup() {}
    virtual void ReadParam() {}
    virtual void ParamSanityCheck() {}
    virtual void StartOfDayInit() {}
    virtual void EndOfDayCleanUp() {}
    virtual void UnsetConvenienceVarb() {}
    virtual void SetConvenienceVarb(const int iTradSym) {}
    virtual bool SkipSubseqProcessingForSymbol(const int iTradSym,string & sReason);
    virtual void InitialWarmUp(const int iTradSym) {}
    virtual void UpdateInternalData(const int iTradSym) {}
    virtual void UpdateInternalDataTrng(const int iTradSym) {}
    virtual void LoadOptimalParamsFromTrainingResults() {}
    virtual void EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket) {}
    virtual void DetermineRegime(const int iTradSym) {}
    virtual void PreTradePreparation(const int iTradSym);
    virtual void CalcPosSize(const int iTradSym) {}
    virtual void AdjustSamplingInterval(const int iTradSym) {}
    virtual void EndOfDayTrainingRoutine(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket) {}

};

#endif /* PORTFOLIOGENERATION_STRATEGYTEST_H_ */
