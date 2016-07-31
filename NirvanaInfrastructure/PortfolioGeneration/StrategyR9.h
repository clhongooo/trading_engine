#ifndef PORTFOLIOGENERATION_STRATEGYR9_H_
#define PORTFOLIOGENERATION_STRATEGYR9_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include "StrategyBase.h"
#include "NumberPosition.h"

class StrategyR9 : public StrategyBase {
  public:
    enum FixedOrTrailingStop {FOT_FIXED=0,FOT_TRAILING};
    StrategyR9();
    virtual ~StrategyR9();

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
    virtual void AdjustSamplingInterval(const int iTradSym);
    virtual void EndOfDayTrainingRoutine(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket);

  private:
    //--------------------------------------------------
    // Strategy objects
    //--------------------------------------------------
    vector<vector<double> > m_StopLossPrice;
    vector<vector<double> > m_RefPrice;
    vector<vector<double> > m_SeedTradePos;
    vector<NumberPosition> m_vNumPos_RefPx;

    //--------------------------------------------------
    // Strategy Parameters
    //--------------------------------------------------
    vector<double> m_SARThreshold;
    vector<double> m_TargetProfit;
    vector<double> m_NumOfSeeds;
    vector<double> m_SeedsPtsApart;
    vector<double> m_FixedOrTrailingStop;

};

#endif /* PORTFOLIOGENERATION_STRATEGYR9_H_ */
