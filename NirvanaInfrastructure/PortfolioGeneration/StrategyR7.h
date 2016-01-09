#ifndef PORTFOLIOGENERATION_STRATEGYR7_H_
#define PORTFOLIOGENERATION_STRATEGYR7_H_

#include "PCH.h"
#include "Constants.h"
#include "StrategyBase.h"
#include "NumberPosition.h"

class StrategyR7 : public StrategyBase {
  public:
    // enum R7_RefPxMode {
    //   R7REFPX_OPEN=0,
    //   R7REFPX_DAYHIGH,
    //   R7REFPX_DAYLOW,
    //   // R7REFPX_DAYAVGPX,
    //   // R7REFPX_MVGAVGPX,
    //   R7REFPX_LAST};

    StrategyR7();
    virtual ~StrategyR7();

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
    vector<vector<double> > m_RefPrice;
    vector<vector<double> > m_TimeStopPrice;
    vector<vector<double> > m_TheoPosSize;

    vector<vector<double> > m_SeedTradePos;
    vector<NumberPosition> m_vNumPos_RefPx;

    //--------------------------------------------------
    // Strategy Parameters
    //--------------------------------------------------
    vector<double> m_EntryThreshold;
    vector<double> m_MaxSize;
    vector<double> m_RateOfTimeStopAdj;

    vector<double> m_NumOfSeeds;
    vector<double> m_SeedsPtsApart;

};

#endif /* PORTFOLIOGENERATION_STRATEGYR7_H_ */
