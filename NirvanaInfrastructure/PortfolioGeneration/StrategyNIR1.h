#ifndef PORTFOLIOGENERATION_STRATEGYNIR1_H_
#define PORTFOLIOGENERATION_STRATEGYNIR1_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include "StrategyBase.h"
#include "NumberPosition.h"
#include "BlackScholes.h"
#include "MarketData.h"

class StrategyNIR1 : public StrategyBase {
  public:
    StrategyNIR1();
    virtual ~StrategyNIR1();

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

  private:
    //--------------------------------------------------
    // Strategy objects
    //--------------------------------------------------
    SMap<string,int>    m_map_SgndPos;
    int                 m_MaturityYYYYMMDD;
    double              m_UnderlyingMQ;


};

#endif /* PORTFOLIOGENERATION_STRATEGYNIR1_H_ */
