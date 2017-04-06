#ifndef STRATEGIES_STRATEGYB1_H_
#define STRATEGIES_STRATEGYB1_H_

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <vector>
#include "Sma.hpp"
#include "SDateTime.h"
#include "SFunctional.h"
#include "../Strategies/StrategyBase.h"

class StrategyB1 : public StrategyBase {
  public:
    enum TRAINMODE {TM_MAXPROFIT=1,TM_MAXSHARPE=2};

    typedef struct {
      double               m_trainingperiod  ;
      int                  m_ma_period       ;
      const vector<double> * m_v_hist_forsma ;
      const vector<double> * m_v_hist_forpnl ;
      vector<int>          * m_v_signal_trail;
      vector<bool>         * m_v_disable_days;
      double *             m_pdAvgPnL        ;
      double *             m_pdSharpe        ;
      WEIGHTING_SCHEME     m_ws              ;
      double               m_curbInSmpReturn ;
    } TestRunParm;

    StrategyB1();
    virtual ~StrategyB1();


    bool TrainUpParam(const YYYYMMDDHHMMSS &, const double, const vector<double> &, const vector<double> &, const TRAINMODE, const WEIGHTING_SCHEME, map<double,int> &, map<double,int> &, map<double,int> &, const double);
    void SetParamRange(const double, const double, const double);
    void SetParamRange(const double, const double, const double, const double, const double, const double);


  protected:

    bool TestRun(const TestRunParm &);

    //--------------------------------------------------
    // Strategy objects
    //--------------------------------------------------
    double m_ma_period_1_low;
    double m_ma_period_1_high;
    double m_ma_period_1_adj;
    double m_ma_period_2_low;
    double m_ma_period_2_high;
    double m_ma_period_2_adj;


};

#endif /* STRATEGIES_STRATEGYB1_H_ */
