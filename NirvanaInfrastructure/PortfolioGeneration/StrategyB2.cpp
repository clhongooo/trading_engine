#include <StrategyB2.h>

StrategyB2::StrategyB2() :
  m_TheoCPnLHistFolder(""),
  m_TheoPosFolder(""),
  m_ChooseBestRotationGroupNum(1),
  m_NumOfAvbGrp(0),
  m_B2_HasEnabledMinCommissionCheck(true),
  m_B2_WhetherToRetain(false),
  m_B2_ActionTimeBefCloseInSec(180),
  m_B2_ActionTimeAftOpenInSec(180),
  m_MoveNextBestGroupUpIfNoSignal(true),
  m_MoveNextBestStkInGrpUpIfNoSignal(true),
  StrategyBase()
{
  UnsetConvenienceVarb();
  m_iNumOfParam = 25;
}

StrategyB2::~StrategyB2()
{
}


bool StrategyB2::GetEstimate(const GetEstParam & gep)
{

  return GetEstimate(
    gep.trainingperiod,
    gep.beta_1,
    gep.beta_2,
    gep.beta_3,
    gep.beta_4,
    *gep.v_hist_trfpx,
    *gep.v_hist_close,
    *gep.v_hist_openavg,
    gep.pdAvgSquaredError,
    gep.pdAvgPnL,
    gep.pdSharpe,
    gep.pdFinalEstimate,
    gep.ws,
    gep.outlierLowerBound,
    gep.outlierUpperBound,
    gep.hypothesis
    );
}


double StrategyB2::CalcPredictionTaylorTrdAtClose(const vector<double> & v_hist_trfpx, const int idx, const double beta_1, const double beta_2, const double beta_3, const double beta_4, const int hypothesis)
{
  double dFirBeta = (v_hist_trfpx[idx] > v_hist_trfpx[idx-1] ? beta_1 : beta_2);
  double dSecBeta = (v_hist_trfpx[idx] > v_hist_trfpx[idx-1] ? beta_3 : beta_4);

  double dTaylor1 = 0;
  double dTaylor2 = 0;

  if (hypothesis == B2_HYPOTHESIS_TAYLOR1_TRDATCLOSE)
  {
    if (v_hist_trfpx.size()-1 < idx) return NAN;

    //--------------------------------------------------
    // avoid multiplying zero betas
    //--------------------------------------------------
    if (abs(dFirBeta) > NIR_EPSILON) dTaylor1 = dFirBeta * (v_hist_trfpx[idx] - v_hist_trfpx[idx-1]);
    if (abs(dSecBeta) > NIR_EPSILON) dTaylor2 = dSecBeta * (v_hist_trfpx[idx] - 2 * v_hist_trfpx[idx-1] + v_hist_trfpx[idx-2]);

    return v_hist_trfpx[idx] + dTaylor1 + dTaylor2;
  }
  else if (hypothesis == B2_HYPOTHESIS_TAYLOR2_TRDATCLOSE)
  {
    if (v_hist_trfpx.size()-1 < idx) return NAN;

    //--------------------------------------------------
    // avoid multiplying zero betas
    //--------------------------------------------------
    if (abs(dFirBeta) > NIR_EPSILON) dTaylor1 = dFirBeta * (v_hist_trfpx[idx] - v_hist_trfpx[idx-2]);
    if (abs(dSecBeta) > NIR_EPSILON) dTaylor2 = dSecBeta * (v_hist_trfpx[idx] - 2 * v_hist_trfpx[idx-1] + v_hist_trfpx[idx-2]);

    return v_hist_trfpx[idx] + dTaylor1 + dTaylor2;
  }
}

bool StrategyB2::GetEstimate(
  const double trainingperiod,
  const double beta_1,
  const double beta_2,
  const double beta_3,
  const double beta_4,
  const vector<double> & v_hist_trfpx,
  const vector<double> & v_hist_close,
  const vector<double> & v_hist_openavg,
  double * pdAvgSquaredError,
  double * pdAvgPnL,
  double * pdSharpe,
  double * pdFinalEstimate,
  WEIGHTING_SCHEME ws,
  const double outlierLowerBound,
  const double outlierUpperBound,
  const int hypothesis
  )
{
  if (v_hist_trfpx.size() != v_hist_close.size())
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB2: %s::%s (%d) GetEstimate returns false.",__FILE__,__FUNCTION__,__LINE__);
    return false;
  }
  if (v_hist_trfpx.size() < trainingperiod)
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB2: %s::%s (%d) GetEstimate returns false.",__FILE__,__FUNCTION__,__LINE__);
    return false;
  }
  if (v_hist_trfpx.size() < 2)
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB2: %s::%s (%d) GetEstimate returns false.",__FILE__,__FUNCTION__,__LINE__);
    return false;
  }

  boost::accumulators::accumulator_set<double, boost::accumulators::features<boost::accumulators::tag::mean, boost::accumulators::tag::variance>, double> acc;
  double dPnL = 0;
  double dTotalWeight = 0;
  double dAvgSquaredError = 0;

  int iEndingIdx = 0;
  iEndingIdx = v_hist_trfpx.size()-1;

  for (unsigned int i = 2; i < iEndingIdx; ++i)
  {
    double dWeight = 0;
    if (ws == WS_LINEAR)
    {
      dWeight = max((double)i-(double)(iEndingIdx+1)+(double)trainingperiod,(double)0);
      if (dWeight <= NIR_EPSILON) continue;
    }
    else if (ws == WS_UNIFORM)
    {
      if (i < (double)(iEndingIdx+1)-(double)trainingperiod) continue;
      dWeight = 1;
    }

    dTotalWeight += dWeight;

    double dPrediction = NAN;
    dPrediction = CalcPredictionTaylorTrdAtClose(v_hist_trfpx, i, beta_1, beta_2, beta_3, beta_4, hypothesis);

    if (STool::IsValidPriceOrVol(dPrediction))
    {
      if (pdAvgSquaredError) dAvgSquaredError += dWeight * pow(dPrediction-v_hist_trfpx[i+1],2);
      if (pdAvgPnL || pdSharpe)
      {
        if (!std::isnan(outlierUpperBound) && !std::isnan(outlierLowerBound))
          dPnL = (dPrediction > v_hist_trfpx[i] ? 1 : -1) * max(min((v_hist_close[i+1] / v_hist_close[i]) -1, outlierUpperBound), outlierLowerBound);
        else
          dPnL = (dPrediction > v_hist_trfpx[i] ? 1 : -1) * ((v_hist_close[i+1] / v_hist_close[i]) -1);

        acc(dPnL, boost::accumulators::weight = dWeight);
      }
    }
  }

  if (pdAvgSquaredError) *pdAvgSquaredError = dAvgSquaredError / dTotalWeight;

  //--------------------------------------------------
  // Output PnL Sharpe
  //--------------------------------------------------
  if (pdAvgPnL)
  {
    *pdAvgPnL = boost::accumulators::mean(acc);
  }
  if (pdSharpe)
  {
    *pdSharpe = boost::accumulators::mean(acc) / sqrt(boost::accumulators::variance(acc));
  }
  //--------------------------------------------------
  // Final estimate
  //--------------------------------------------------
  if (pdFinalEstimate)
  {
    *pdFinalEstimate = CalcPredictionTaylorTrdAtClose(v_hist_trfpx, iEndingIdx, beta_1, beta_2, beta_3, beta_4, hypothesis) - v_hist_trfpx[iEndingIdx];
  }

  return true;
}


bool StrategyB2::TrainUpParamTaylor(
  const int iTradSym,
  const YYYYMMDDHHMMSS & ymdhms,
  const double trainingperiodmin,
  const double trainingperiodmax,
  const double trainingperiodstepsize,
  const vector<double> & v_hist_trfpx,
  const vector<double> & v_hist_close,
  const vector<double> & v_hist_openavg,
  const vector<double> & v_hist_avgpxrtn,
  const CountingFunction & countingfunc,
  const TRAINMODE tm,
  const WEIGHTING_SCHEME ws,
  const double curbInSmpOutlier
  )
{
  const string symbol = m_TradedSymbols[iTradSym];
  int iBestParamIdx = 0;

  if (
    trainingperiodmin == 0 ||
    trainingperiodmax == 0 ||
    trainingperiodstepsize == 0
    )
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB2: %s::%s (%d) %s TrainUpParamTaylor exits: Training periods are 0.",__FILE__,__FUNCTION__,__LINE__,symbol.c_str());
    return true;
  }

  if (
    trainingperiodmin > v_hist_trfpx.size() ||
    trainingperiodmax > v_hist_trfpx.size() ||
    trainingperiodmin > countingfunc.Size() ||
    trainingperiodmax > countingfunc.Size()
    )
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB1: %s::%s (%d) %s TrainUpParamTaylor exits: Not enough historical data for the training periods. v_hist_trfpx.size() %d",
                    __FILE__,__FUNCTION__,__LINE__, symbol.c_str(), v_hist_trfpx.size());
    return false;
  }

  if (
    trainingperiodmin      <= 0 ||
    trainingperiodmax      <= 0 ||
    trainingperiodstepsize <= 0
    )
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB2: %s::%s (%d) %s TrainUpParamTaylor exits: Not all training periods are > 0.",__FILE__,__FUNCTION__,__LINE__, symbol.c_str());
    return false;
  }

  m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1[symbol].clear();
  m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1[symbol].clear();
  m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2[symbol].clear();
  m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2[symbol].clear();
  m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1[symbol].clear();
  m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1[symbol].clear();
  m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2[symbol].clear();
  m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2[symbol].clear();
  m_RiseRegimeAvgSharpeOfEachTraingPeriod[symbol].clear();
  m_FallRegimeAvgSharpeOfEachTraingPeriod[symbol].clear();

  int iTaylorIdx = B2_HYPOTHESIS_TAYLOR1_TRDATCLOSE;
  while (iTaylorIdx <= B2_HYPOTHESIS_TAYLOR2_TRDATCLOSE)
  {
    {
      //--------------------------------------------------
      // initialization for that symbol
      //--------------------------------------------------
      for (int iLoop = 0; iLoop < 4; ++iLoop)
      {
        map<string,map<int,vector<TupObjParamVec> > >   * tmp_tup = NULL;
        map<string,map<int,map<int,vector<double> > > > * tmp_dbl = NULL;

        if (iTaylorIdx == B2_HYPOTHESIS_TAYLOR1_TRDATCLOSE && iLoop == 0)
        {
          tmp_tup = & m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1;
          tmp_dbl = & m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1;
        }
        else if (iTaylorIdx == B2_HYPOTHESIS_TAYLOR1_TRDATCLOSE && iLoop == 1)
        {
          tmp_tup = & m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1;
          tmp_dbl = & m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1;
        }
        else if (iTaylorIdx == B2_HYPOTHESIS_TAYLOR2_TRDATCLOSE && iLoop == 2)
        {
          tmp_tup = & m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2;
          tmp_dbl = & m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2;
        }
        else if (iTaylorIdx == B2_HYPOTHESIS_TAYLOR2_TRDATCLOSE && iLoop == 3)
        {
          tmp_tup = & m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2;
          tmp_dbl = & m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2;
        }

        if (tmp_tup) (*tmp_tup)[symbol] = map<int,vector<TupObjParamVec> >();
        if (tmp_dbl) (*tmp_dbl)[symbol] = map<int,map<int,vector<double> > >();

      }
    }

    //--------------------------------------------------
    // loop through different training periods
    //--------------------------------------------------
    for (int iTrngPrd = m_TrainingPeriodMin; iTrngPrd <= STool::Min(m_TrainingPeriodMax, m_HistoricalAvgPx->size()); iTrngPrd += m_TrainingPeriodStepSize)
    {

      vector<TupObjParamVec>   * vectupBestParamBeta1Beta3       = NULL;
      vector<TupObjParamVec>   * vectupBestParamBeta2Beta4       = NULL;
      map<int,vector<double> > * mapvecdblBestParamSetBeta1Beta3 = NULL;
      map<int,vector<double> > * mapvecdblBestParamSetBeta2Beta4 = NULL;

      if (iTaylorIdx == B2_HYPOTHESIS_TAYLOR1_TRDATCLOSE)
      {
        m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1[symbol][iTrngPrd] = vector<TupObjParamVec>();
        m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1[symbol][iTrngPrd] = vector<TupObjParamVec>();
        m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1[symbol][iTrngPrd] = map<int,vector<double> >();
        m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1[symbol][iTrngPrd] = map<int,vector<double> >();

        vectupBestParamBeta1Beta3       = & m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1[symbol][iTrngPrd];
        vectupBestParamBeta2Beta4       = & m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1[symbol][iTrngPrd];
        mapvecdblBestParamSetBeta1Beta3 = & m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1[symbol][iTrngPrd];
        mapvecdblBestParamSetBeta2Beta4 = & m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1[symbol][iTrngPrd];
      }
      else if (iTaylorIdx == B2_HYPOTHESIS_TAYLOR2_TRDATCLOSE)
      {
        m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2[symbol][iTrngPrd] = vector<TupObjParamVec>();
        m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2[symbol][iTrngPrd] = vector<TupObjParamVec>();
        m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2[symbol][iTrngPrd] = map<int,vector<double> >();
        m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2[symbol][iTrngPrd] = map<int,vector<double> >();

        vectupBestParamBeta1Beta3       = & m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2[symbol][iTrngPrd];
        vectupBestParamBeta2Beta4       = & m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2[symbol][iTrngPrd];
        mapvecdblBestParamSetBeta1Beta3 = & m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2[symbol][iTrngPrd];
        mapvecdblBestParamSetBeta2Beta4 = & m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2[symbol][iTrngPrd];
      }

      //--------------------------------------------------
      // 0 = rising, 1 = falling
      //--------------------------------------------------
      for (unsigned int rising_or_falling = 0; rising_or_falling < 2; ++rising_or_falling)
      {
        vector<double> dPrelimAvgPnL1;
        vector<double> dPrelimSharpe1;
        vector<double> dPrelimSquaredError1;

        if (rising_or_falling == 0)
        {
          vectupBestParamBeta1Beta3->clear();
          mapvecdblBestParamSetBeta1Beta3->clear();
        }
        else if (rising_or_falling == 1)
        {
          vectupBestParamBeta2Beta4->clear();
          mapvecdblBestParamSetBeta2Beta4->clear();
        }

        //--------------------------------------------------
        double dTrainingPeriodToUse = 0;
        if      (rising_or_falling == 0) dTrainingPeriodToUse = countingfunc.HowManyElemFromTailToAccum(iTrngPrd);
        else if (rising_or_falling == 1) dTrainingPeriodToUse = countingfunc.HowManyElemFromTailToAccumOpp(iTrngPrd);

        m_Logger->Write(Logger::INFO,"StrategyB2: %s %s Looping rising / falling regime = %d. Using training period %f.",
                        ymdhms.ToStr().c_str(),
                        symbol.c_str(),
                        rising_or_falling,
                        dTrainingPeriodToUse);


        //--------------------------------------------------
        double dFirTermLowerBound = 0;
        double dFirTermUpperBound = 0;
        double dSecTermLowerBound = 0;
        double dSecTermUpperBound = 0;
        double dFirTermAdj        = 0;
        double dSecTermAdj        = 0;

        if (rising_or_falling == 0)
        {
          dFirTermLowerBound = m_b1_low;
          dFirTermUpperBound = m_b1_high;
          dFirTermAdj        = m_b1_adj;
          dSecTermLowerBound = m_b3_low;
          dSecTermUpperBound = m_b3_high;
          dSecTermAdj        = m_b3_adj;
        }
        else if (rising_or_falling == 1)
        {
          dFirTermLowerBound = m_b2_low;
          dFirTermUpperBound = m_b2_high;
          dFirTermAdj        = m_b2_adj;
          dSecTermLowerBound = m_b4_low;
          dSecTermUpperBound = m_b4_high;
          dSecTermAdj        = m_b4_adj;
        }

        //--------------------------------------------------
        dPrelimAvgPnL1      .clear();
        dPrelimSharpe1      .clear();
        dPrelimSquaredError1.clear();

        for (double bFir = dFirTermLowerBound; bFir <= dFirTermUpperBound; bFir += dFirTermAdj)
        {
          for (double bSec = dSecTermLowerBound; bSec <= dSecTermUpperBound; bSec += dSecTermAdj)
          {
            dPrelimAvgPnL1      .push_back(NAN);
            dPrelimSharpe1      .push_back(NAN);
            dPrelimSquaredError1.push_back(NAN);
          }
        }

        //--------------------------------------------------
        int iCnt = 0;
        boost::thread_group thdgrp;
        //--------------------------------------------------

        for (double bFir = dFirTermLowerBound; bFir <= dFirTermUpperBound; bFir += dFirTermAdj)
        {
          for (double bSec = dSecTermLowerBound; bSec <= dSecTermUpperBound; bSec += dSecTermAdj)
          {

            GetEstParam gep1;

            if (rising_or_falling == 0)
            {
              gep1.beta_1          =   bFir;
              gep1.beta_3          =   bSec;
              gep1.beta_2          =   0;
              gep1.beta_4          =   0;
            }
            else if (rising_or_falling == 1)
            {
              gep1.beta_1          =   0;
              gep1.beta_3          =   0;
              gep1.beta_2          =   bFir;
              gep1.beta_4          =   bSec;
            }

            gep1.trainingperiod    =   dTrainingPeriodToUse;
            gep1.v_hist_trfpx      =   &v_hist_trfpx;
            gep1.v_hist_close      =   &v_hist_close;
            gep1.v_hist_openavg    =   &v_hist_openavg;
            gep1.pdAvgSquaredError =   &dPrelimSquaredError1[iCnt];
            gep1.pdAvgPnL          =   &dPrelimAvgPnL1      [iCnt];
            gep1.pdSharpe          =   &dPrelimSharpe1      [iCnt];
            gep1.pdFinalEstimate   =   NULL;
            gep1.ws                =   ws;
            gep1.outlierLowerBound =   *m_dOutlierLowerBound;
            gep1.outlierUpperBound =   *m_dOutlierUpperBound;
            gep1.hypothesis        =   iTaylorIdx;

            iCnt++;

            thdgrp.add_thread(new boost::thread(boost::bind(&StrategyB2::GetEstimate, this, gep1)));
          }
        }

        thdgrp.join_all();

        //--------------------------------------------------
        iCnt = 0;
        double dAvgSharpeOfAllParamCombination = 0;
        for (double bFir = dFirTermLowerBound; bFir <= dFirTermUpperBound; bFir += dFirTermAdj)
        {
          for (double bSec = dSecTermLowerBound; bSec <= dSecTermUpperBound; bSec += dSecTermAdj)
          {
            int iBestParamIdxToUse = iBestParamIdx++;
            double dObj = NAN;

            vector<double> vTmp;
            vTmp.push_back(bFir);
            vTmp.push_back(bSec);
            if (rising_or_falling == 0)
            {
              if      (tm == TM_MINSQERR)  dObj = dPrelimSquaredError1[iCnt];
              else if (tm == TM_MAXPROFIT) dObj = dPrelimAvgPnL1      [iCnt];
              else if (tm == TM_MAXSHARPE) dObj = dPrelimSharpe1      [iCnt];

              TupObjParamVec tup(dObj,iBestParamIdxToUse);
              (*vectupBestParamBeta1Beta3).push_back(tup);
              (*mapvecdblBestParamSetBeta1Beta3)[iBestParamIdxToUse] = vTmp;
            }
            else if (rising_or_falling == 1)
            {
              if      (tm == TM_MINSQERR)  dObj = dPrelimSquaredError1[iCnt];
              else if (tm == TM_MAXPROFIT) dObj = dPrelimAvgPnL1      [iCnt];
              else if (tm == TM_MAXSHARPE) dObj = dPrelimSharpe1      [iCnt];

              TupObjParamVec tup(dObj,iBestParamIdxToUse);
              (*vectupBestParamBeta2Beta4).push_back(tup);
              (*mapvecdblBestParamSetBeta2Beta4)[iBestParamIdxToUse] = vTmp;
            }

            dAvgSharpeOfAllParamCombination += dPrelimSharpe1[iCnt];

            m_Logger->Write(Logger::DEBUG,"StrategyB2: %s %s Looped parameters: rising_or_falling %d vectupBestParamBeta1Beta3.size() %d mapvecdblBestParamSetBeta1Beta3.size() %d vectupBestParamBeta2Beta4.size() %d mapvecdblBestParamSetBeta2Beta4.size() %d beta_1 = %f, beta_2 = %f, beta_3 = %f, beta_4 = %f, dPrelimSquaredError = %f. dPrelimAvgPnL = %f. dPrelimSharpe = %f. iTaylorIdx = %d",
                            ymdhms.ToStr().c_str(),
                            symbol.c_str(),
                            rising_or_falling,
                            vectupBestParamBeta1Beta3->size(),
                            mapvecdblBestParamSetBeta1Beta3->size(),
                            vectupBestParamBeta2Beta4->size(),
                            mapvecdblBestParamSetBeta2Beta4->size(),
                            bFir,(double)0,bSec,(double)0,dPrelimSquaredError1[iCnt],dPrelimAvgPnL1[iCnt],dPrelimSharpe1[iCnt],
                            iTaylorIdx);

            iCnt++;
          }
        }

        //--------------------------------------------------
        // calculate the average Sharpe
        //--------------------------------------------------
        dAvgSharpeOfAllParamCombination = dAvgSharpeOfAllParamCombination / (double)iCnt;
        if (rising_or_falling == 0)
        {
          if (m_RiseRegimeAvgSharpeOfEachTraingPeriod.find(symbol) == m_RiseRegimeAvgSharpeOfEachTraingPeriod.end())
            m_RiseRegimeAvgSharpeOfEachTraingPeriod[symbol] = map<double,int>();
          m_RiseRegimeAvgSharpeOfEachTraingPeriod[symbol][dAvgSharpeOfAllParamCombination] = iTrngPrd;
        }
        else if (rising_or_falling == 1)
        {
          if (m_FallRegimeAvgSharpeOfEachTraingPeriod.find(symbol) == m_FallRegimeAvgSharpeOfEachTraingPeriod.end())
            m_FallRegimeAvgSharpeOfEachTraingPeriod[symbol] = map<double,int>();
          m_FallRegimeAvgSharpeOfEachTraingPeriod[symbol][dAvgSharpeOfAllParamCombination] = iTrngPrd;
        }
        //--------------------------------------------------

      }

    } // for loop through training periods

    iTaylorIdx++;

  } // while (iTaylorIdx)

  //--------------------------------------------------
  // what is the best training period?
  //--------------------------------------------------
  if (!m_RiseRegimeAvgSharpeOfEachTraingPeriod[symbol].empty())
  {
    map<double,int>::iterator it = m_RiseRegimeAvgSharpeOfEachTraingPeriod[symbol].end();
    it--;
    m_RiseRegimeBestTrainingPeriod[symbol] = it->second;
    AddAvgSharpeOfSym(iTradSym,
                      it->first,
                      B2_RISEREGIME);
  }

  if (!m_FallRegimeAvgSharpeOfEachTraingPeriod[symbol].empty())
  {
    map<double,int>::iterator it = m_FallRegimeAvgSharpeOfEachTraingPeriod[symbol].end();
    it--;
    m_FallRegimeBestTrainingPeriod[symbol] = it->second;
    AddAvgSharpeOfSym(iTradSym,
                      it->first,
                      B2_FALLREGIME);
  }

  FForEach(m_RiseRegimeAvgSharpeOfEachTraingPeriod[symbol],[&](const std::pair<double,int> kv) {
    m_Logger->Write(Logger::INFO,"StrategyB2: %s %s m_RiseRegimeAvgSharpeOfEachTraingPeriod %f %d",
                    ymdhms.ToStr().c_str(),
                    symbol.c_str(),
                    kv.first,
                    kv.second
                   );
  });
  FForEach(m_FallRegimeAvgSharpeOfEachTraingPeriod[symbol],[&](const std::pair<double,int> kv) {
    m_Logger->Write(Logger::INFO,"StrategyB2: %s %s m_FallRegimeAvgSharpeOfEachTraingPeriod %f %d",
                    ymdhms.ToStr().c_str(),
                    symbol.c_str(),
                    kv.first,
                    kv.second
                   );
  });
  m_Logger->Write(Logger::INFO,"StrategyB2: %s %s m_RiseRegimeBestTrainingPeriod %d m_FallRegimeBestTrainingPeriod %d",
                  ymdhms.ToStr().c_str(),
                  symbol.c_str(),
                  m_RiseRegimeBestTrainingPeriod[symbol],
                  m_FallRegimeBestTrainingPeriod[symbol]
                 );
  //--------------------------------------------------


  return true;
}

void StrategyB2::SetParamBetaRange(
  const double b1_low,
  const double b1_high,
  const double b1_adj,
  const double b2_low,
  const double b2_high,
  const double b2_adj,
  const double b3_low,
  const double b3_high,
  const double b3_adj,
  const double b4_low,
  const double b4_high,
  const double b4_adj
  )
{
  m_b1_low  = b1_low;
  m_b1_high = b1_high;
  m_b1_adj  = b1_adj;
  m_b2_low  = b2_low;
  m_b2_high = b2_high;
  m_b2_adj  = b2_adj;
  m_b3_low  = b3_low;
  m_b3_high = b3_high;
  m_b3_adj  = b3_adj;
  m_b4_low  = b4_low;
  m_b4_high = b4_high;
  m_b4_adj  = b4_adj;
  return;
}

void StrategyB2::ReadRotationGroupFile(const string & file)
{
  deque<string> dqFile;
  STool::ReadFile(file,dqFile);
  for (deque<string>::iterator it = dqFile.begin(); it != dqFile.end(); ++it)
  {
    vector<string> vs;
    boost::split(vs, *it, boost::is_any_of(","));

    const YYYYMMDD ymd(boost::lexical_cast<int>(vs[0]));
    vector<int> vgrp;
    for (int i = 1; i < vs.size(); ++i) vgrp.push_back(boost::lexical_cast<int>(vs[i]));

    m_RotationGroupFromFile[ymd] = vgrp;
  }

  FForEach(m_RotationGroupFromFile,[&](std::pair<YYYYMMDD,vector<int> > tup) {
    m_Logger->Write(Logger::INFO,"Strategy %s: m_RotationGroupFromFile %s %s",
                    GetStrategyName(m_StyID).c_str(),
                    tup.first.ToStr_().c_str(),
                    STool::MkString<int>(tup.second).c_str()
                   );
  });

  return;
}

void StrategyB2::ReadParam()
{
  //--------------------------------------------------
  // Parameters
  //--------------------------------------------------
  m_TheoCPnLHistFolder = m_SysCfg->B2_TheoCPnLHistFolder(m_StyID);
  m_TheoPosFolder      = m_SysCfg->B2_TheoPosFolder     (m_StyID);
  m_PersistTheoPosCPnL = m_SysCfg->B2_PersistTheoPosCPnL(m_StyID);

  //--------------------------------------------------
  m_RotationMode = m_SysCfg->B2_HasEnabledRotationMode(m_StyID);
  m_Logger->Write(Logger::INFO,"Strategy %s: m_RotationMode %d", GetStrategyName(m_StyID).c_str(),  m_RotationMode);
  m_RotationModeTradeHighestReturn = m_SysCfg->B2_RotationModeTradeHighestReturn(m_StyID);
  m_Logger->Write(Logger::INFO,"Strategy %s: m_RotationModeTradeHighestReturn %s", GetStrategyName(m_StyID).c_str(), (m_RotationModeTradeHighestReturn?"true":"false"));
  m_RotationModeUseVolyAdjdReturn = m_SysCfg->B2_RotationModeUseVolyAdjdReturn(m_StyID);
  m_Logger->Write(Logger::INFO,"Strategy %s: m_RotationModeUseVolyAdjdReturn %s", GetStrategyName(m_StyID).c_str(), (m_RotationModeUseVolyAdjdReturn?"true":"false"));
  m_RotationModeUseNDayReturn = m_SysCfg->B2_RotationModeUseNDayReturn(m_StyID);
  m_Logger->Write(Logger::INFO,"Strategy %s: m_RotationModeUseNDayReturn %d", GetStrategyName(m_StyID).c_str(), m_RotationModeUseNDayReturn);

  m_LongOnlyWhenClosePriceBelowAvgPrice = m_SysCfg->B2_LongOnlyWhenClosePriceBelowAvgPrice(m_StyID);
  if (m_LongOnlyWhenClosePriceBelowAvgPrice.IsSome())
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenClosePriceBelowAvgPrice %f", GetStrategyName(m_StyID).c_str(), m_LongOnlyWhenClosePriceBelowAvgPrice.Get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenClosePriceBelowAvgPrice: Nil", GetStrategyName(m_StyID).c_str());

  m_ShortOnlyWhenClosePriceAboveAvgPrice = m_SysCfg->B2_ShortOnlyWhenClosePriceAboveAvgPrice(m_StyID);
  if (m_ShortOnlyWhenClosePriceAboveAvgPrice.IsSome())
    m_Logger->Write(Logger::INFO,"Strategy %s: m_ShortOnlyWhenClosePriceAboveAvgPrice %f", GetStrategyName(m_StyID).c_str(), m_ShortOnlyWhenClosePriceAboveAvgPrice.Get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_ShortOnlyWhenClosePriceAboveAvgPrice: Nil", GetStrategyName(m_StyID).c_str());

  m_LongOnlyWhenAvgPriceReturnAbove = m_SysCfg->B2_LongOnlyWhenAvgPriceReturnAbove(m_StyID);
  if (m_LongOnlyWhenAvgPriceReturnAbove.IsSome())
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenAvgPriceReturnAbove %f", GetStrategyName(m_StyID).c_str(), m_LongOnlyWhenAvgPriceReturnAbove.Get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenAvgPriceReturnAbove: Nil", GetStrategyName(m_StyID).c_str());

  m_LongOnlyWhenGrpAvgReturnAbove = m_SysCfg->B2_LongOnlyWhenGrpAvgReturnAbove(m_StyID);
  if (m_LongOnlyWhenGrpAvgReturnAbove.IsSome())
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenGrpAvgReturnAbove %f", GetStrategyName(m_StyID).c_str(), m_LongOnlyWhenGrpAvgReturnAbove.Get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenGrpAvgReturnAbove: Nil", GetStrategyName(m_StyID).c_str());

  m_ShortOnlyWhenAvgPriceReturnBelow = m_SysCfg->B2_ShortOnlyWhenAvgPriceReturnBelow(m_StyID);
  if (m_ShortOnlyWhenAvgPriceReturnBelow.IsSome())
    m_Logger->Write(Logger::INFO,"Strategy %s: m_ShortOnlyWhenAvgPriceReturnBelow %f", GetStrategyName(m_StyID).c_str(), m_ShortOnlyWhenAvgPriceReturnBelow.Get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_ShortOnlyWhenAvgPriceReturnBelow: Nil", GetStrategyName(m_StyID).c_str());

  m_Logger->Write(Logger::INFO,"Strategy %s: B2_CommissionRateThreshold %f", GetStrategyName(m_StyID).c_str(), m_SysCfg->B2_CommissionRateThreshold(m_StyID));

  if (m_RotationMode != 0)
  {
    m_ChooseBestRotationGroupProp = m_SysCfg->Get_B2_ChooseBestNRotationGroupProp(m_StyID);
    m_Logger->Write(Logger::INFO,"Strategy %s: m_ChooseBestNRotationGroupProp %f", GetStrategyName(m_StyID).c_str(),  m_ChooseBestRotationGroupProp);

    m_AvgAggSgndNotnlThresh = m_SysCfg->Get_B2_AvgAggSgndNotnlThresh(m_StyID);
    m_Logger->Write(Logger::INFO,"Strategy %s: m_AvgAggSgndNotnlThresh %f", GetStrategyName(m_StyID).c_str(),  m_AvgAggSgndNotnlThresh);

    m_MoveNextBestGroupUpIfNoSignal = m_SysCfg->Get_B2_MoveNextBestGroupUpIfNoSignal(m_StyID);
    m_Logger->Write(Logger::INFO,"Strategy %s: m_MoveNextBestGroupUpIfNoSignal %s", GetStrategyName(m_StyID).c_str(),  (m_MoveNextBestGroupUpIfNoSignal?"true":"false"));

    m_MoveNextBestStkInGrpUpIfNoSignal = m_SysCfg->Get_B2_MoveNextBestStkInGrpUpIfNoSignal(m_StyID);
    m_Logger->Write(Logger::INFO,"Strategy %s: m_MoveNextBestStkInGrpUpIfNoSignal %s", GetStrategyName(m_StyID).c_str(),  (m_MoveNextBestStkInGrpUpIfNoSignal?"true":"false"));

    m_RotationGroupFile = m_SysCfg->Get_B2_RotationGroupFile(m_StyID);
    m_Logger->Write(Logger::INFO,"Strategy %s: m_RotationGroupFile %s", GetStrategyName(m_StyID).c_str(), m_RotationGroupFile.c_str());

    ReadRotationGroupFile(m_RotationGroupFile);

    m_vRoleOfSym = m_SysCfg->Get_B2_RoleOfSym(m_StyID);
    FForEach (m_vRoleOfSym,[&](const int i) { m_Logger->Write(Logger::INFO,"Strategy %s: m_vRoleOfSym %d", GetStrategyName(m_StyID).c_str(), i); });
    for (int i = 0; i < m_TradedSymbols.size(); ++i)
    {
      m_mapRoleOfSym[m_TradedSymbols[i]] = m_vRoleOfSym[i];
    }

    for(map<string,int>::iterator it = m_mapRoleOfSym.begin(); it != m_mapRoleOfSym.end(); ++it)
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: m_mapRoleOfSym %s %d",
                      GetStrategyName(m_StyID).c_str(),
                      it->first.c_str(),
                      it->second);
    }
  }
  else
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: Any m_ChooseBestNRotationGroupProp settings are ignored", GetStrategyName(m_StyID).c_str());
    m_Logger->Write(Logger::INFO,"Strategy %s: Any m_AvgAggSgndNotnlThresh settings are ignored", GetStrategyName(m_StyID).c_str());
    m_Logger->Write(Logger::INFO,"Strategy %s: Any m_RotationGroup settings are ignored", GetStrategyName(m_StyID).c_str());
    m_Logger->Write(Logger::INFO,"Strategy %s: Any m_vRoleOfSym settings are ignored", GetStrategyName(m_StyID).c_str());
    m_Logger->Write(Logger::INFO,"Strategy %s: Any m_MoveNextBestGroupUpIfNoSignal settings are ignored", GetStrategyName(m_StyID).c_str());
    m_Logger->Write(Logger::INFO,"Strategy %s: Any m_MoveNextBestStkInGrpUpIfNoSignal settings are ignored", GetStrategyName(m_StyID).c_str());
  }


  if (m_RotationMode != 0)
  {
    m_AllAvbSymForRollingBasket  .insert(m_AllAvbSymForRollingBasket  .begin(), B2_ROTATION_GROUP_MAX, map<YYYYMMDD,set<string> >        () );
    m_SymRankedByRollingReturn   .insert(m_SymRankedByRollingReturn   .begin(), B2_ROTATION_GROUP_MAX, map<YYYYMMDD,vector<TupRetSym> >  () );
    m_SymAndRollingReturn        .insert(m_SymAndRollingReturn        .begin(), B2_ROTATION_GROUP_MAX, map<YYYYMMDD,map<string,double> > () );
    m_MaintainPosWithinGrp       .insert(m_MaintainPosWithinGrp       .begin(), B2_ROTATION_GROUP_MAX, map<YYYYMMDD,set<string> >        () );
  }
  //--------------------------------------------------

  m_B2_HasEnabledMinCommissionCheck = m_SysCfg->B2_HasEnabledMinCommissionCheck(m_StyID);
  m_B2_WhetherToRetain              = m_SysCfg->GetWhetherToRetrain(m_StyID);
  m_B2_TrainingFreq                 = m_SysCfg->GetTrainingFreq(m_StyID);
  m_B2_ActionTimeBefCloseInSec      = m_SysCfg->Get_B2_ActionTimeBefCloseInSec(m_StyID);
  m_B2_ActionTimeAftOpenInSec       = m_SysCfg->Get_B2_ActionTimeAftOpenInSec(m_StyID);
  m_B2_FilterSMAPeriod              = m_SysCfg->Get_B2_FilterSMAPeriod(m_StyID);

  m_Logger->Write(Logger::INFO,"Strategy %s: m_B2_HasEnabledMinCommissionCheck %s", GetStrategyName(m_StyID).c_str(), (m_B2_HasEnabledMinCommissionCheck?"true":"false"));
  m_Logger->Write(Logger::INFO,"Strategy %s: m_B2_WhetherToRetain              %s", GetStrategyName(m_StyID).c_str(), (m_B2_WhetherToRetain             ?"true":"false"));
  m_Logger->Write(Logger::INFO,"Strategy %s: m_B2_TrainingFreq                 %d", GetStrategyName(m_StyID).c_str(),  m_B2_TrainingFreq                                );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_B2_ActionTimeBefCloseInSec      %d", GetStrategyName(m_StyID).c_str(),  m_B2_ActionTimeBefCloseInSec                     );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_B2_ActionTimeAftOpenInSec       %d", GetStrategyName(m_StyID).c_str(),  m_B2_ActionTimeAftOpenInSec                      );

  FForEach(m_B2_FilterSMAPeriod,[&](const int iSma) { m_Logger->Write(Logger::INFO,"Strategy %s: m_B2_FilterSMAPeriod %d", GetStrategyName(m_StyID).c_str(), iSma); });

  m_TradedSymbolsTradeAtOpen = m_SysCfg->Get_B2_TradedSymTradeAtOpen(m_StyID);
  FForEach(m_TradedSymbolsTradeAtOpen,[&](const string & sym) { m_Logger->Write(Logger::INFO,"Strategy %s: m_TradedSymbolsTradeAtOpen %s", GetStrategyName(m_StyID).c_str(), sym.c_str()); });

  //--------------------------------------------------
  m_TrainingPeriodMin      = m_ParamVector[0];
  m_TrainingPeriodMax      = m_ParamVector[1];
  m_TrainingPeriodStepSize = m_ParamVector[2];

  m_Logger->Write(Logger::INFO,"Strategy %s: m_TrainingPeriodMin      %f", GetStrategyName(m_StyID).c_str(), m_TrainingPeriodMin     );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_TrainingPeriodMax      %f", GetStrategyName(m_StyID).c_str(), m_TrainingPeriodMax     );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_TrainingPeriodStepSize %f", GetStrategyName(m_StyID).c_str(), m_TrainingPeriodStepSize);
  //--------------------------------------------------

  m_NotionalAmt        = m_ParamVector[3 +  0];
  m_MaxNotionalAmtEach = m_ParamVector[3 +  1];
  m_PriceMode          = m_ParamVector[3 +  2];
  m_CurbInSmpOutlier   = m_ParamVector[3 +  3];
  m_AllowTrdDir        = m_ParamVector[3 +  4];
  m_KSBound            = m_ParamVector[3 +  5];
  m_KSRange            = m_ParamVector[3 +  6];
  m_MinAnnVolPsnSz     = m_ParamVector[3 +  7];
  m_beta_1_start       = m_ParamVector[3 +  8];
  m_beta_1_end         = m_ParamVector[3 +  9];
  m_beta_1_incremt     = m_ParamVector[3 + 10];
  m_beta_2_start       = m_ParamVector[3 + 11];
  m_beta_2_end         = m_ParamVector[3 + 12];
  m_beta_2_incremt     = m_ParamVector[3 + 13];
  m_beta_3_start       = m_ParamVector[3 + 14];
  m_beta_3_end         = m_ParamVector[3 + 15];
  m_beta_3_incremt     = m_ParamVector[3 + 16];
  m_beta_4_start       = m_ParamVector[3 + 17];
  m_beta_4_end         = m_ParamVector[3 + 18];
  m_beta_4_incremt     = m_ParamVector[3 + 19];
  m_PropOfBestParam    = m_ParamVector[3 + 20];
  m_WeightScheme       =(m_ParamVector[3 + 21] == 1 ? WS_UNIFORM : WS_LINEAR);

  m_Logger->Write(Logger::INFO,"Strategy %s: m_NotionalAmt         %f", GetStrategyName(m_StyID).c_str(), m_NotionalAmt       );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_MaxNotionalAmtEach  %f", GetStrategyName(m_StyID).c_str(), m_MaxNotionalAmtEach);
  m_Logger->Write(Logger::INFO,"Strategy %s: m_PriceMode           %d", GetStrategyName(m_StyID).c_str(), m_PriceMode         );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_CurbInSmpOutlier    %f", GetStrategyName(m_StyID).c_str(), m_CurbInSmpOutlier  );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_AllowTrdDir         %d", GetStrategyName(m_StyID).c_str(), m_AllowTrdDir       );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_KSBound             %f", GetStrategyName(m_StyID).c_str(), m_KSBound           );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_KSRange             %f", GetStrategyName(m_StyID).c_str(), m_KSRange           );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_MinAnnVolPsnSz      %f", GetStrategyName(m_StyID).c_str(), m_MinAnnVolPsnSz    );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_1_start        %f", GetStrategyName(m_StyID).c_str(), m_beta_1_start      );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_1_end          %f", GetStrategyName(m_StyID).c_str(), m_beta_1_end        );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_1_incremt      %f", GetStrategyName(m_StyID).c_str(), m_beta_1_incremt    );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_2_start        %f", GetStrategyName(m_StyID).c_str(), m_beta_2_start      );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_2_end          %f", GetStrategyName(m_StyID).c_str(), m_beta_2_end        );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_2_incremt      %f", GetStrategyName(m_StyID).c_str(), m_beta_2_incremt    );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_3_start        %f", GetStrategyName(m_StyID).c_str(), m_beta_3_start      );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_3_end          %f", GetStrategyName(m_StyID).c_str(), m_beta_3_end        );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_3_incremt      %f", GetStrategyName(m_StyID).c_str(), m_beta_3_incremt    );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_4_start        %f", GetStrategyName(m_StyID).c_str(), m_beta_4_start      );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_4_end          %f", GetStrategyName(m_StyID).c_str(), m_beta_4_end        );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_beta_4_incremt      %f", GetStrategyName(m_StyID).c_str(), m_beta_4_incremt    );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_PropOfBestParam     %f", GetStrategyName(m_StyID).c_str(), m_PropOfBestParam   );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_WeightScheme        %d", GetStrategyName(m_StyID).c_str(), m_WeightScheme      );

  //--------------------------------------------------

  FForEach (m_TradedSymbols,[&](const string & sym) {
    m_PTask_PrintStyActionTimeToken[sym] = m_PTask_PrintStyActionTime.GetNewTokenAndSetIntervalInSec(60);
  });
  m_PTask_PrintAllAvbSymToken = m_PTask_PrintAllAvbSym.GetNewTokenAndSetIntervalInSec(300);

  if (!m_B2_FilterSMAPeriod.empty())
  {
    if (m_B2_FilterSMAPeriod.size() == 2)
    {
      if (m_B2_FilterSMAPeriod[0] > 0)
      {
        for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
          m_v_SMA_short.push_back(Sma<double>(m_B2_FilterSMAPeriod[0],true));
      }
      if (m_B2_FilterSMAPeriod[1] > 0)
      {
        for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
          m_v_SMA_long.push_back(Sma<double>(m_B2_FilterSMAPeriod[1],true));
      }
    }
    else if (m_B2_FilterSMAPeriod.size() == 1)
    {
      if (m_B2_FilterSMAPeriod[0] > 0)
      {
        for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
          m_v_SMA_long.push_back(Sma<double>(m_B2_FilterSMAPeriod[0],true));
      }
    }
  }

  m_PTask_PrintTrainingResultToken = m_PTask_PrintTrainingResult.GetNewTokenAndSetIntervalInSec(60);

}

void StrategyB2::StartOfDayInit()
{
  //--------------------------------------------------
  // Rotation group update
  //--------------------------------------------------
  for (map<YYYYMMDD,vector<int> >::iterator it = m_RotationGroupFromFile.begin(); it != m_RotationGroupFromFile.end(); ++it)
  {
    if (m_ymdhms_SysTime_HKT.GetYYYYMMDD() >= it->first)
    {
      m_RotationGroup.clear();
      m_RotationGroup.insert(m_RotationGroup.begin(),it->second.begin(),it->second.end());
    }
  }
  //--------------------------------------------------

  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    m_map_DoneSODTrade[m_TradedSymbols[i]] = false;
    m_map_DoneEODTrade[m_TradedSymbols[i]] = false;
  }

  m_StkPicks.clear();

  //--------------------------------------------------
  // they give HKT
  //--------------------------------------------------
  m_hms_OpenStart.Set(m_Exchg->GetTimeNSecAftOpen (m_TradedSymbols[0],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),0));
  m_hms_OpenEnd.Set(m_Exchg->GetTimeNSecAftOpen (m_TradedSymbols[0],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),m_B2_ActionTimeAftOpenInSec));
  if (m_hms_OpenStart > m_hms_OpenEnd) m_hms_OpenStart.Set(0,0,0);
  m_hms_CloseStart.Set(m_Exchg->GetTimeNSecBefClose(m_TradedSymbols[0],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),m_B2_ActionTimeBefCloseInSec));
  m_hms_CloseEnd.Set(m_Exchg->GetTimeNSecBefClose(m_TradedSymbols[0],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),0));

}

void StrategyB2::EndOfDayCleanUp()
{
  StartOfDayInit();
}

void StrategyB2::SetConvenienceVarb(const int iTradSym)
{

  {
    //--------------------------------------------------
    map<string,vector<double> >::iterator it1 = m_map_HistoricalAvgPx.find(m_TradedSymbols[iTradSym]);
    if (it1 == m_map_HistoricalAvgPx.end())
    {
      m_map_HistoricalAvgPx[m_TradedSymbols[iTradSym]] = vector<double>();
      it1 = m_map_HistoricalAvgPx.find(m_TradedSymbols[iTradSym]);
    }
    m_HistoricalAvgPx = &(it1->second);

    map<string,vector<double> >::iterator it2 = m_map_HistoricalClose.find(m_TradedSymbols[iTradSym]);
    if (it2 == m_map_HistoricalClose.end())
    {
      m_map_HistoricalClose[m_TradedSymbols[iTradSym]] = vector<double>();
      it2 = m_map_HistoricalClose.find(m_TradedSymbols[iTradSym]);
    }
    m_HistoricalClose = &(it2->second);

    map<string,vector<double> >::iterator it3 = m_map_HistoricalOpenAvg.find(m_TradedSymbols[iTradSym]);
    if (it3 == m_map_HistoricalOpenAvg.end())
    {
      m_map_HistoricalOpenAvg[m_TradedSymbols[iTradSym]] = vector<double>();
      it3 = m_map_HistoricalOpenAvg.find(m_TradedSymbols[iTradSym]);
    }
    m_HistoricalOpenAvg = &(it3->second);
    //--------------------------------------------------
  }
  {
    //--------------------------------------------------
    map<string,vector<double> >::iterator it1 = m_map_HistoricalAvgPxRtn.find(m_TradedSymbols[iTradSym]);
    if (it1 == m_map_HistoricalAvgPxRtn.end())
    {
      m_map_HistoricalAvgPxRtn[m_TradedSymbols[iTradSym]] = vector<double>();
      it1 = m_map_HistoricalAvgPxRtn.find(m_TradedSymbols[iTradSym]);
    }
    m_HistoricalAvgPxRtn = &(it1->second);

    map<string,vector<double> >::iterator it2 = m_map_HistoricalCloseRtn.find(m_TradedSymbols[iTradSym]);
    if (it2 == m_map_HistoricalCloseRtn.end())
    {
      m_map_HistoricalCloseRtn[m_TradedSymbols[iTradSym]] = vector<double>();
      it2 = m_map_HistoricalCloseRtn.find(m_TradedSymbols[iTradSym]);
    }
    m_HistoricalCloseRtn = &(it2->second);
    //--------------------------------------------------
  }

  map<string,CountingFunction>::iterator it4a = m_map_HistoricalNumOfRisingDaysCountAvgPx.find(m_TradedSymbols[iTradSym]);
  if (it4a == m_map_HistoricalNumOfRisingDaysCountAvgPx.end())
  {
    m_map_HistoricalNumOfRisingDaysCountAvgPx[m_TradedSymbols[iTradSym]] = CountingFunction();
    it4a = m_map_HistoricalNumOfRisingDaysCountAvgPx.find(m_TradedSymbols[iTradSym]);
  }
  m_HistoricalNumOfRisingDaysCountAvgPx = &(it4a->second);

  //--------------------------------------------------
  map<string,double>::iterator it4c = m_map_OutlierLowerBound.find(m_TradedSymbols[iTradSym]);
  if (it4c == m_map_OutlierLowerBound.end())
  {
    m_map_OutlierLowerBound[m_TradedSymbols[iTradSym]] = NAN;
    it4c = m_map_OutlierLowerBound.find(m_TradedSymbols[iTradSym]);
  }
  m_dOutlierLowerBound = &(it4c->second);

  //--------------------------------------------------
  map<string,double>::iterator it4d = m_map_OutlierUpperBound.find(m_TradedSymbols[iTradSym]);
  if (it4d == m_map_OutlierUpperBound.end())
  {
    m_map_OutlierUpperBound[m_TradedSymbols[iTradSym]] = NAN;
    it4d = m_map_OutlierUpperBound.find(m_TradedSymbols[iTradSym]);
  }
  m_dOutlierUpperBound = &(it4d->second);


  map<string,bool>::iterator it7a = m_map_DoneSODTrade.find(m_TradedSymbols[iTradSym]);
  if (it7a == m_map_DoneSODTrade.end())
  {
    m_map_DoneSODTrade[m_TradedSymbols[iTradSym]] = false;
    it7a = m_map_DoneSODTrade.find(m_TradedSymbols[iTradSym]);
  }
  m_DoneSODTrade = &(it7a->second);

  map<string,bool>::iterator it7b = m_map_DoneEODTrade.find(m_TradedSymbols[iTradSym]);
  if (it7b == m_map_DoneEODTrade.end())
  {
    m_map_DoneEODTrade[m_TradedSymbols[iTradSym]] = false;
    it7b = m_map_DoneEODTrade.find(m_TradedSymbols[iTradSym]);
  }
  m_DoneEODTrade = &(it7b->second);

  map<string,bool>::iterator it8a = m_map_bTrainRetAvgPx.find(m_TradedSymbols[iTradSym]);
  if (it8a == m_map_bTrainRetAvgPx.end())
  {
    m_map_bTrainRetAvgPx[m_TradedSymbols[iTradSym]] = false;
    it8a = m_map_bTrainRetAvgPx.find(m_TradedSymbols[iTradSym]);
  }
  m_bTrainRetAvgPx = &(it8a->second);

  map<string,bool>::iterator it9a = m_map_bRisingRegimeAvgPx.find(m_TradedSymbols[iTradSym]);
  if (it9a == m_map_bRisingRegimeAvgPx.end())
  {
    m_map_bRisingRegimeAvgPx[m_TradedSymbols[iTradSym]] = false;
    it9a = m_map_bRisingRegimeAvgPx.find(m_TradedSymbols[iTradSym]);
  }
  m_bRisingRegime = &(it9a->second);

}


bool StrategyB2::SkipSubseqProcessingForSymbol(const int iTradSym, string & sComment)
{
  sComment = "m_DoneSODTrade = " + string(*m_DoneSODTrade?"true":"false");
  sComment = "m_DoneEODTrade = " + string(*m_DoneEODTrade?"true":"false");

  if (m_PTask_PrintStyActionTime.CheckIfItIsTimeToWakeUp(m_PTask_PrintStyActionTimeToken[m_TradedSymbols[iTradSym]],m_ymdhms_SysTime_HKT))
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: Sym=%s Current time %s Action time between (%s and %s) and (%s and %s)",
                    GetStrategyName(m_StyID).c_str(),
                    m_TradedSymbols[iTradSym].c_str(),
                    m_ymdhms_SysTime_HKT.GetHHMMSS().ToStr_().c_str(),
                    m_hms_OpenStart.ToStr_().c_str(),
                    m_hms_OpenEnd.ToStr_().c_str(),
                    m_hms_CloseStart.ToStr_().c_str(),
                    m_hms_CloseEnd.ToStr_().c_str()
                   );
  }

  if (
    !*m_DoneSODTrade &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_hms_OpenStart &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() <= m_hms_OpenEnd
    ) return false;
  else if
    (
      //--------------------------------------------------
      // note that we need to trade EOD even for trade at open, because need to close
      //--------------------------------------------------
      !*m_DoneEODTrade &&
      m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_hms_CloseStart &&
      m_ymdhms_SysTime_HKT.GetHHMMSS() <= m_hms_CloseEnd
    ) return false;
  else return true;
}



void StrategyB2::InitialWarmUp(const int iTradSym)
{
  //--------------------------------------------------
  // TheoCPnLHist and TheoPos
  //--------------------------------------------------
  m_TheoCPnLHist.clear();
  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    m_TheoCPnLHist.push_back(CPnLHist());
    m_TheoAcct.push_back(Acct());

    if (m_PersistTheoPosCPnL)
    {
      if (!m_TheoCPnLHist.back().LoadFromFile(m_TheoCPnLHistFolder+"/"+m_TradedSymbols[iTradSym]))
      {
        sleep(1);
        exit(1);
      }

      if (!m_TheoAcct.back().LoadFromFile(m_TheoPosFolder+"/"+m_TradedSymbols[iTradSym]))
      {
        sleep(1);
        exit(1);
      }
    }
  }
  //--------------------------------------------------

  vector<YYYYMMDD> vYMD;
  vector<double>   vOpen;
  vector<double>   vHigh;
  vector<double>   vLow;
  vector<double>   vClose;
  vector<long>     vVol;

  YMD ymdStart(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
  ymdStart.SubtractDay(m_TrainingPeriodMax*2); // times 2 since here we are subtracting calendar days

  //TODO MarketData::Instance()
  m_Logger->Write(Logger::INFO,"Strategy %s: Sym=%s GetSuppD1BarOHLCVInDateRange before %s %d", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[iTradSym].c_str(), __FILE__, __LINE__);
  MarketData::Instance()->GetSuppD1BarOHLCVInDateRange(m_TradedSymbols[iTradSym], YYYYMMDD(ymdStart), m_p_ymdhms_SysTime_Local->GetYYYYMMDD(), vYMD, vOpen, vHigh, vLow, vClose, vVol);
  m_Logger->Write(Logger::INFO,"Strategy %s: Sym=%s GetSuppD1BarOHLCVInDateRange after %s %d", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[iTradSym].c_str(), __FILE__, __LINE__);

  if (!vYMD.empty() && (m_p_ymdhms_SysTime_Local->GetYYYYMMDD() > vYMD.back() && m_p_ymdhms_SysTime_Local->GetYYYYMMDD() - vYMD.back() > 5))
  {
    m_Logger->Write(Logger::ERROR,"Strategy %s: Sym=%s Supplementary day bar may be out-of-date. Current date: %s. Last supp day bar date: %s.",
                    GetStrategyName(m_StyID).c_str(), m_TradedSymbols[iTradSym].c_str(), m_p_ymdhms_SysTime_Local->GetYYYYMMDD().ToStr_().c_str(), vYMD.back().ToStr_().c_str());
    // sleep(1);
    // exit(1);
  }

  if (vYMD.empty()) return;
  for (unsigned int i = 0; i < vYMD.size(); ++i)
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: Sym=%s Read from day bar file: %s %s High = %f Low = %f Close = %f.",
                    GetStrategyName(m_StyID).c_str(), m_TradedSymbols[iTradSym].c_str(), vYMD[i].ToStr_().c_str(),m_TradedSymbols[iTradSym].c_str(), vHigh[i],vLow[i],vClose[i]);

    //--------------------------------------------------
    // skip today
    //--------------------------------------------------
    if (m_p_ymdhms_SysTime_Local->GetYYYYMMDD() == vYMD[i])
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: Sym=%s Skipping day bar %s because it's the same as today.", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[iTradSym].c_str(), vYMD[i].ToStr_().c_str());
      continue;
    }

    double dAvgPx = (vHigh[i] + vLow[i] + vClose[i]) / 3;
    if (!std::isnan(dAvgPx))
    {
      if (!m_HistoricalAvgPx->empty())   m_HistoricalNumOfRisingDaysCountAvgPx->Add(dAvgPx >= m_HistoricalAvgPx->back());

      m_HistoricalAvgPx->push_back(dAvgPx);
      m_HistoricalClose->push_back(vClose[i]);
      m_HistoricalOpenAvg->push_back(vOpen[i]);
      m_HistoricalOpenAvg->push_back(dAvgPx);

      //--------------------------------------------------
      {
        YYYYMMDD ymd = vYMD[i];

        map<YYYYMMDD,set<string> >::iterator it = m_AllSymWithUpdateDataAtOpen.find(ymd);
        if (it == m_AllSymWithUpdateDataAtOpen.end())
        {
          m_AllSymWithUpdateDataAtOpen[ymd] = set<string>();
          it = m_AllSymWithUpdateDataAtOpen.find(ymd);
        }
        it->second.insert(m_TradedSymbols[iTradSym]);
      }

      {
        YYYYMMDD ymd = vYMD[i];

        map<YYYYMMDD,set<string> >::iterator it = m_AllSymWithUpdateDataAtClose.find(ymd);
        if (it == m_AllSymWithUpdateDataAtClose.end())
        {
          m_AllSymWithUpdateDataAtClose[ymd] = set<string>();
          it = m_AllSymWithUpdateDataAtClose.find(ymd);
        }
        it->second.insert(m_TradedSymbols[iTradSym]);
      }
      //--------------------------------------------------

      if (!m_B2_FilterSMAPeriod.empty())
      {
        if (m_B2_FilterSMAPeriod.size() == 2)
        {
          if (m_B2_FilterSMAPeriod[0] > 0) m_v_SMA_short[iTradSym].Add(dAvgPx);
          if (m_B2_FilterSMAPeriod[1] > 0) m_v_SMA_long[iTradSym].Add(dAvgPx);
        }
        else if (m_B2_FilterSMAPeriod.size() == 1)
        {
          if (m_B2_FilterSMAPeriod[0] > 0) m_v_SMA_long[iTradSym].Add(dAvgPx);
        }
      }

      if (m_HistoricalAvgPx->size()>1)
      {
        double d = m_HistoricalAvgPx->back() / *(m_HistoricalAvgPx->end()-2);
        if (!std::isnan(d)) m_HistoricalAvgPxRtn->push_back(d);
      }
      if (m_HistoricalClose->size()>1)
      {
        double d = m_HistoricalClose->back() / *(m_HistoricalClose->end()-2);
        if (!std::isnan(d)) m_HistoricalCloseRtn->push_back(d);
      }

    }
  }

  if (m_B2_TrainingFreq == TradingStrategyConfig::OnceAtInitWarmup)
  {
    DoTraining(iTradSym);
  }

}

void StrategyB2::UpdateInternalDataTrng(const int iTradSym)
{
  return;
}

void StrategyB2::UpdateInternalData(const int iTradSym)
{

  //--------------------------------------------------
  // For rotation
  //--------------------------------------------------
  if (m_RotationMode != 0 && m_RotationGroup[iTradSym] >= 0)
  {
    //--------------------------------------------------
    if (m_AllAvbSymForRollingBasket[m_RotationGroup[iTradSym]].find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD()) == m_AllAvbSymForRollingBasket[m_RotationGroup[iTradSym]].end())
    {
      m_AllAvbSymForRollingBasket[m_RotationGroup[iTradSym]][m_p_ymdhms_SysTime_Local->GetYYYYMMDD()] = set<string>();
    }
    //--------------------------------------------------
    // Only add those symbols with enough backtest data
    //--------------------------------------------------
    if (m_map_HistoricalAvgPx[m_TradedSymbols[iTradSym]].size() >= m_TrainingPeriodMin)
    {
      m_AllAvbSymForRollingBasket[m_RotationGroup[iTradSym]][m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].insert(m_TradedSymbols[iTradSym]);
    }
    //--------------------------------------------------

    {
      YYYYMMDD ymd = m_p_ymdhms_SysTime_Local->GetYYYYMMDD();
      map<YYYYMMDD,set<string> >::iterator it = m_AllAvbSym.find(ymd);
      if (it == m_AllAvbSym.end())
      {
        m_AllAvbSym[ymd] = set<string>();
        it = m_AllAvbSym.find(ymd);
      }
      //--------------------------------------------------
      // strategy base will only call UpdateInternalData() for that symbol only if there is price update for that symbol
      //--------------------------------------------------
      if (m_HistoricalAvgPx->size() >= m_TrainingPeriodMin)
        it->second.insert(m_TradedSymbols[iTradSym]);

      if (m_PTask_PrintAllAvbSym.CheckIfItIsTimeToWakeUp(m_PTask_PrintAllAvbSymToken,m_ymdhms_SysTime_HKT))
        FForEach(it->second,[&](const string & sym) { m_Logger->Write(Logger::INFO,"Strategy %s: m_AllAvbSym %s", GetStrategyName(m_StyID).c_str(), sym.c_str()); });
    }
  }
  //--------------------------------------------------


  if (
    m_AllSymWithUpdateDataAtOpen[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].find(m_TradedSymbols[iTradSym]) ==
    m_AllSymWithUpdateDataAtOpen[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].end() &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_hms_OpenStart &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() <= m_hms_OpenEnd
    )
  {
    if (abs(m_ymdhms_SysTime_HKT - m_ymdhms_midquote) < 10)
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s Getting opening price: MidQuote = %f.",
                      GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(), m_SymMidQuote);

      m_HistoricalOpenAvg->push_back(m_SymMidQuote);

      //--------------------------------------------------
      {
        YYYYMMDD ymd = m_p_ymdhms_SysTime_Local->GetYYYYMMDD();
        map<YYYYMMDD,set<string> >::iterator it = m_AllSymWithUpdateDataAtOpen.find(ymd);
        if (it == m_AllSymWithUpdateDataAtOpen.end())
        {
          m_AllSymWithUpdateDataAtOpen[ymd] = set<string>();
          it = m_AllSymWithUpdateDataAtOpen.find(ymd);
        }
        it->second.insert(m_TradedSymbols[iTradSym]);
      }
      //--------------------------------------------------

    }
  }

  if (
    //--------------------------------------------------
    // has to be done whether we are trading at open
    //--------------------------------------------------
    m_AllSymWithUpdateDataAtClose[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].find(m_TradedSymbols[iTradSym]) ==
    m_AllSymWithUpdateDataAtClose[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].end() &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_hms_CloseStart &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() <= m_hms_CloseEnd
    )
  {
    double dHigh  = TechIndicators::Instance()->GetDayHigh(m_TradedSymbols[iTradSym]);
    double dLow   = TechIndicators::Instance()->GetDayLow(m_TradedSymbols[iTradSym]);
    double dClose = TechIndicators::Instance()->GetDayClose(m_TradedSymbols[iTradSym]);
    if (!std::isnan(dHigh) && !std::isnan(dLow) && !std::isnan(dClose))
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s Getting aggregated daily high low close: High = %f Low = %f Close = %f MidQuote = %f.",
                      GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(), dHigh, dLow, dClose, m_SymMidQuote);
      double dAvgPx = (dHigh+dLow+m_SymMidQuote)/(double)3;

      if (!m_HistoricalAvgPx->empty()) m_HistoricalNumOfRisingDaysCountAvgPx->Add(dAvgPx >= m_HistoricalAvgPx->back());

      m_HistoricalAvgPx->push_back(dAvgPx);
      m_HistoricalClose->push_back(m_SymMidQuote);
      m_HistoricalOpenAvg->push_back(dAvgPx);

      //--------------------------------------------------
      {
        YYYYMMDD ymd = m_p_ymdhms_SysTime_Local->GetYYYYMMDD();
        map<YYYYMMDD,set<string> >::iterator it = m_AllSymWithUpdateDataAtClose.find(ymd);
        if (it == m_AllSymWithUpdateDataAtClose.end())
        {
          m_AllSymWithUpdateDataAtClose[ymd] = set<string>();
          it = m_AllSymWithUpdateDataAtClose.find(ymd);
        }
        it->second.insert(m_TradedSymbols[iTradSym]);
      }
      //--------------------------------------------------

      if (!m_B2_FilterSMAPeriod.empty())
      {
        if (m_B2_FilterSMAPeriod.size() == 2)
        {
          if (m_B2_FilterSMAPeriod[0] > 0) m_v_SMA_short[iTradSym].Add(dAvgPx);
          if (m_B2_FilterSMAPeriod[1] > 0) m_v_SMA_long[iTradSym].Add(dAvgPx);
        }
        else if (m_B2_FilterSMAPeriod.size() == 1)
        {
          if (m_B2_FilterSMAPeriod[0] > 0) m_v_SMA_long[iTradSym].Add(dAvgPx);

        }
      }

      if (m_HistoricalAvgPx->size()>1)
      {
        double d = m_HistoricalAvgPx->back() / *(m_HistoricalAvgPx->end()-2);
        m_HistoricalAvgPxRtn->push_back(d);
      }
      if (m_HistoricalClose->size()>1)
      {
        double d = m_HistoricalClose->back() / *(m_HistoricalClose->end()-2);
        m_HistoricalCloseRtn->push_back(d);
      }

      GetOutliers(
        *m_HistoricalAvgPxRtn,
        STool::Min(m_TrainingPeriodMax,m_HistoricalAvgPx->size())-1,
        m_CurbInSmpOutlier,
        *m_dOutlierLowerBound,
        *m_dOutlierUpperBound
        );

      m_Logger->Write(Logger::INFO,"Strategy %s: %s %s (%d) Sym=%s OutlierLowerBound = %f OutlierUpperBound = %f ",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      __FUNCTION__,__LINE__,
                      m_TradedSymbols[iTradSym].c_str(),
                      *m_dOutlierLowerBound,
                      *m_dOutlierUpperBound
                     );

    }
  }

  m_dGKYZVal = TechIndicators::Instance()->GetGKYZValue(m_TradedSymbols[iTradSym]);
}


bool StrategyB2::GetOutliers(const vector<double> & v_hist_avgpxrtn, const int trainingperiod, const double curbInSmpOutlier, double & dOutlierLowerBound, double & dOutlierUpperBound)
{
  if (v_hist_avgpxrtn.size() < trainingperiod) return false;
  if (curbInSmpOutlier <= 0) return false;

  dOutlierLowerBound = NAN;
  dOutlierUpperBound = NAN;

  if (B2_CURBOUTLIERMODE == B2_CURBOUTLIERMODE_FIXED)
  {
    dOutlierLowerBound = -curbInSmpOutlier;
    dOutlierUpperBound = curbInSmpOutlier;
    return true;
  }
  else if (B2_CURBOUTLIERMODE == B2_CURBOUTLIERMODE_PCTL)
  {
    vector<double> vTmp;
    vTmp.insert(vTmp.begin(), v_hist_avgpxrtn.end()-trainingperiod, v_hist_avgpxrtn.end());
    std::sort(vTmp.begin(),vTmp.end());
    const int iLBidx = vTmp.size() * curbInSmpOutlier;
    const int iUBidx = vTmp.size() - iLBidx - 1;
    dOutlierLowerBound = v_hist_avgpxrtn[iLBidx];
    dOutlierUpperBound = v_hist_avgpxrtn[iUBidx];
    return true;
  }
  //--------------------------------------------------

  return true;
}



void StrategyB2::DoTraining(const int iTradSym)
{
  if (B2_SKIPMACHLEARNING) return;

  if (m_RotationMode != 0 && m_mapRoleOfSym[m_TradedSymbols[iTradSym]] == 0)
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: %s %s (%d) Sym=%s Just an indicator symbol, will not do training.",
                    GetStrategyName(m_StyID).c_str(),
                    m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                    __FUNCTION__,__LINE__,
                    m_TradedSymbols[iTradSym].c_str()
                   );
    return;
  }

  // SetParamAdjAmt(SML_ADJ_BETA12, BIG_ADJ_BETA12, SML_ADJ_BETA34, BIG_ADJ_BETA34);
  SetParamBetaRange(
    m_beta_1_start,
    m_beta_1_end,
    m_beta_1_incremt,
    m_beta_2_start,
    m_beta_2_end,
    m_beta_2_incremt,
    m_beta_3_start,
    m_beta_3_end,
    m_beta_3_incremt,
    m_beta_4_start,
    m_beta_4_end,
    m_beta_4_incremt);

  *m_bTrainRetAvgPx = TrainUpParamTaylor(
    iTradSym,
    *m_p_ymdhms_SysTime_Local,
    m_TrainingPeriodMin,
    m_TrainingPeriodMax,
    m_TrainingPeriodStepSize,
    (*m_HistoricalAvgPx),
    (*m_HistoricalClose),
    (*m_HistoricalOpenAvg),
    (*m_HistoricalAvgPxRtn),
    (*m_HistoricalNumOfRisingDaysCountAvgPx),
    StrategyB2::TM_MAXSHARPE,
    m_WeightScheme,
    m_CurbInSmpOutlier
    );

  m_map_ymdhms_LastTrainTime[0][m_TradedSymbols[iTradSym]] = *m_p_ymdhms_SysTime_Local;

  m_Logger->Write(Logger::INFO,"Strategy %s: %s %s (%d) Sym=%s TrainUpParam() returned %s",
                  GetStrategyName(m_StyID).c_str(),
                  m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                  __FUNCTION__,__LINE__,
                  m_TradedSymbols[iTradSym].c_str(),(*m_bTrainRetAvgPx?"True":"False")
                 );

}


void StrategyB2::PerformTrainingJustBeforeTrading(const int iTradSym)
{
  if (B2_SKIPMACHLEARNING) return;

  //--------------------------------------------------
  // if don't want to miss the last data point (which is today) in our training, should do the training here.
  // if want to leave enough time for many symbols to train, train after initial warmup
  //--------------------------------------------------
  if (m_B2_TrainingFreq == TradingStrategyConfig::OnceAtInitWarmup)
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: %s %s (%d) Sym=%s m_B2_TrainingFreq = OnceAtInitWarmup",
                    GetStrategyName(m_StyID).c_str(),
                    m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                    __FUNCTION__,__LINE__,
                    m_TradedSymbols[iTradSym].c_str());
    m_Logger->Write(Logger::INFO,"Strategy %s: %s %s (%d) Sym=%s TrainUpParam() returned %s. LastTrainTime = %s",
                    GetStrategyName(m_StyID).c_str(),
                    m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                    __FUNCTION__,__LINE__,
                    m_TradedSymbols[iTradSym].c_str(),
                    (*m_bTrainRetAvgPx?"True":"False"),
                    m_map_ymdhms_LastTrainTime[0][m_TradedSymbols[iTradSym]].ToStr().c_str()
                   );
    return;
  }

  //--------------------------------------------------
  // Train up our parameters
  //--------------------------------------------------
  if (
    m_B2_TrainingFreq == TradingStrategyConfig::Daily
    ||
    (
      !m_map_ymdhms_LastTrainTime[0][m_TradedSymbols[iTradSym]].IsValid()
      ||
      ((*m_bTrainRetAvgPx) && m_p_ymdhms_SysTime_Local->GetYYYYMMDD() - m_map_ymdhms_LastTrainTime[0][m_TradedSymbols[iTradSym]].GetYYYYMMDD() > 10)
      ||
      (!(*m_bTrainRetAvgPx) && m_p_ymdhms_SysTime_Local->GetYYYYMMDD() > m_map_ymdhms_LastTrainTime[0][m_TradedSymbols[iTradSym]].GetYYYYMMDD())
    )
    )
  {
    DoTraining(iTradSym);
  }
  else
  {
    if (m_PTask_PrintTrainingResult.CheckIfItIsTimeToWakeUp(m_PTask_PrintTrainingResultToken,m_ymdhms_SysTime_HKT))
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: %s %s (%d) Sym=%s Training not redone. TrainUpParam() returned %s. LastTrainTime = %s",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      __FUNCTION__,__LINE__,
                      m_TradedSymbols[iTradSym].c_str(),
                      (*m_bTrainRetAvgPx?"True":"False"),
                      m_map_ymdhms_LastTrainTime[0][m_TradedSymbols[iTradSym]].ToStr().c_str()
                     );
    }
  }

}


void StrategyB2::EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}


void StrategyB2::DetermineRegime(const int iTradSym)
{
  *m_bRisingRegime = false;

  if (m_HistoricalAvgPx->size() < 2) return;
  if (m_HistoricalClose->size() < 2) return;
  if (m_HistoricalAvgPx->back() >= *(m_HistoricalAvgPx->end()-2)) *m_bRisingRegime = true;

  m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_bRisingRegime = %s",
                  GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),(*m_bRisingRegime?"True":"False"));                            

}


void StrategyB2::PreTradePreparation(const int iTradSym)
{

  //--------------------------------------------------
  // Training combinations
  //--------------------------------------------------
  if (*m_bRisingRegime)
  {
    m_lNumOfTrngCombnAvgPx =
      ((m_beta_1_end - m_beta_1_start) / m_beta_1_incremt + 1) *
      ((m_beta_3_end - m_beta_3_start) / m_beta_3_incremt + 1);
  }
  else
  {
    m_lNumOfTrngCombnAvgPx =
      ((m_beta_2_end - m_beta_2_start) / m_beta_2_incremt + 1) *
      ((m_beta_4_end - m_beta_4_start) / m_beta_4_incremt + 1);
  }
  //--------------------------------------------------

  //--------------------------------------------------
  // Day End logic
  //--------------------------------------------------
  if (
    !*m_DoneEODTrade &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_hms_CloseStart &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() <= m_hms_CloseEnd
    )
  {

    if (m_AllSymWithUpdateDataAtClose[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].find(m_TradedSymbols[iTradSym]) ==
        m_AllSymWithUpdateDataAtClose[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].end())
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s Guard in PreTradePreparation(): Trade at Close: Internal data not updated yet.",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str()
                     );
      return;
    }

    //--------------------------------------------------
    // Get our predictor - Trade at Close
    //--------------------------------------------------
    m_dStrengthCountTrdAtClose = 0;
    m_TotSharpeOfMethod = 0;

    if (!B2_SKIPMACHLEARNING)
    {
      int iTaylorIdx = B2_HYPOTHESIS_TAYLOR1_TRDATCLOSE;

      while (iTaylorIdx <= B2_HYPOTHESIS_TAYLOR2_TRDATCLOSE)
      {
        const int iBestTrainingPeriod = (*m_bRisingRegime ? m_RiseRegimeBestTrainingPeriod[m_TradedSymbols[iTradSym]] : m_FallRegimeBestTrainingPeriod[m_TradedSymbols[iTradSym]]);
        if (iBestTrainingPeriod == 0)
        {
          m_Logger->Write(Logger::DEBUG,"Strategy %s: %s Sym=%s Rising regime = %s No need to do day end logic. iBestTrainingPeriod = %d",
                          GetStrategyName(m_StyID).c_str(),
                          m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                          m_TradedSymbols[iTradSym].c_str(),
                          (*m_bRisingRegime ? "true" : "false"),
                          iBestTrainingPeriod);
          break;
        }
        else
        {
          m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s Rising regime = %s iBestTrainingPeriod = %d",
                          GetStrategyName(m_StyID).c_str(),
                          m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                          m_TradedSymbols[iTradSym].c_str(),
                          (*m_bRisingRegime ? "true" : "false"),
                          iBestTrainingPeriod);
        }

        vector<TupObjParamVec>   * vec_tup_BestParamSetBeta1Beta3 = NULL;
        vector<TupObjParamVec>   * vec_tup_BestParamSetBeta2Beta4 = NULL;
        map<int,vector<double> > * map_dbl_BestParamSetBeta1Beta3 = NULL;
        map<int,vector<double> > * map_dbl_BestParamSetBeta2Beta4 = NULL;

        if (iTaylorIdx == B2_HYPOTHESIS_TAYLOR1_TRDATCLOSE)
        {
          vec_tup_BestParamSetBeta1Beta3 = & m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1[m_TradedSymbols[iTradSym]][iBestTrainingPeriod];
          vec_tup_BestParamSetBeta2Beta4 = & m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1[m_TradedSymbols[iTradSym]][iBestTrainingPeriod];
          map_dbl_BestParamSetBeta1Beta3 = & m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo1[m_TradedSymbols[iTradSym]][iBestTrainingPeriod];
          map_dbl_BestParamSetBeta2Beta4 = & m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo1[m_TradedSymbols[iTradSym]][iBestTrainingPeriod];
        }
        else if (iTaylorIdx == B2_HYPOTHESIS_TAYLOR2_TRDATCLOSE)
        {
          vec_tup_BestParamSetBeta1Beta3 = & m_tup_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2[m_TradedSymbols[iTradSym]][iBestTrainingPeriod];
          vec_tup_BestParamSetBeta2Beta4 = & m_tup_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2[m_TradedSymbols[iTradSym]][iBestTrainingPeriod];
          map_dbl_BestParamSetBeta1Beta3 = & m_dbl_BestParamSetBeta1Beta3AvgPx_trdatclose_hypo2[m_TradedSymbols[iTradSym]][iBestTrainingPeriod];
          map_dbl_BestParamSetBeta2Beta4 = & m_dbl_BestParamSetBeta2Beta4AvgPx_trdatclose_hypo2[m_TradedSymbols[iTradSym]][iBestTrainingPeriod];
        }

        double & dStrengthCount = m_dStrengthCountTrdAtClose;
        bool & m_bTrainRet = *m_bTrainRetAvgPx;
        long & lNumOfTrngCombn = m_lNumOfTrngCombnAvgPx;
        const long lMaxStren = m_PropOfBestParam / (double)100 * (double)lNumOfTrngCombn;

        //--------------------------------------------------
        m_TotSharpeOfMethod = 0;
        //--------------------------------------------------

        //--------------------------------------------------
        if (m_bTrainRet)
        {
          double b1 = 0;
          double b2 = 0;
          double b3 = 0;
          double b4 = 0;

          if ( (*m_bRisingRegime && !(vec_tup_BestParamSetBeta1Beta3->empty())) ||
              (!*m_bRisingRegime && !(vec_tup_BestParamSetBeta2Beta4->empty()))   )
          {
            int iCnt = 0;
            std::sort(vec_tup_BestParamSetBeta1Beta3->begin(), vec_tup_BestParamSetBeta1Beta3->end());
            std::sort(vec_tup_BestParamSetBeta2Beta4->begin(), vec_tup_BestParamSetBeta2Beta4->end());

            vector<TupObjParamVec>::iterator it_rising  = vec_tup_BestParamSetBeta1Beta3->end();
            vector<TupObjParamVec>::iterator it_falling = vec_tup_BestParamSetBeta2Beta4->end();

            if (*m_bRisingRegime)
              it_rising--;
            else
              it_falling--;


            //--------------------------------------------------
            vector<double> vdEstimate;
            vector<double> vdAvgPnL;
            vector<double> vdSharpe;
            vector<double> vdSquaredError;
            vdEstimate     .insert(vdEstimate    .begin(), lNumOfTrngCombn, 0 );
            vdAvgPnL       .insert(vdAvgPnL      .begin(), lNumOfTrngCombn, 0 );
            vdSharpe       .insert(vdSharpe      .begin(), lNumOfTrngCombn, 0 );
            vdSquaredError .insert(vdSquaredError.begin(), lNumOfTrngCombn, 0 );

            //--------------------------------------------------
            boost::thread_group thdgrp;

            while (
              (*m_bRisingRegime  && it_rising  != vec_tup_BestParamSetBeta1Beta3->begin()) ||
              (!*m_bRisingRegime && it_falling != vec_tup_BestParamSetBeta2Beta4->begin())
              )
            {

              double dScale = 10000;
              if (*m_bRisingRegime)
              {
                b1 = round((*map_dbl_BestParamSetBeta1Beta3)[it_rising->m_paramvecidx][0]*dScale)/dScale;
                b3 = round((*map_dbl_BestParamSetBeta1Beta3)[it_rising->m_paramvecidx][1]*dScale)/dScale;
              }
              else
              {
                b2 = round((*map_dbl_BestParamSetBeta2Beta4)[it_falling->m_paramvecidx][0]*dScale)/dScale;
                b4 = round((*map_dbl_BestParamSetBeta2Beta4)[it_falling->m_paramvecidx][1]*dScale)/dScale;
              }

              GetEstParam gep;
              gep.trainingperiod       =  iBestTrainingPeriod;
              gep.beta_1               =  b1;
              gep.beta_2               =  b2;
              gep.beta_3               =  b3;
              gep.beta_4               =  b4;
              gep.v_hist_trfpx         =  m_HistoricalAvgPx;
              gep.v_hist_close         =  m_HistoricalClose;
              gep.v_hist_openavg       =  m_HistoricalOpenAvg;
              gep.pdAvgSquaredError    =  &(vdSquaredError[iCnt]);
              gep.pdAvgPnL             =  &(vdAvgPnL[iCnt]);
              gep.pdSharpe             =  &(vdSharpe[iCnt]);
              gep.pdFinalEstimate      =  &(vdEstimate[iCnt]);
              gep.ws                   =  m_WeightScheme;
              gep.outlierLowerBound    =  *m_dOutlierLowerBound;
              gep.outlierUpperBound    =  *m_dOutlierUpperBound;
              gep.hypothesis           =  iTaylorIdx;


              thdgrp.add_thread(new boost::thread(boost::bind(&StrategyB2::GetEstimate, this, gep)));

              m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s Selected from the set of best parameters: m_beta_1 = %f, m_beta_2 = %f, m_beta_3 = %f, m_beta_4 = %f",
                              GetStrategyName(m_StyID).c_str(),
                              m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                              m_TradedSymbols[iTradSym].c_str(),
                              b1,b2,b3,b4);
              iCnt++;
              if ((iCnt+1) >= lMaxStren) break;
              if ( *m_bRisingRegime && iCnt >= map_dbl_BestParamSetBeta1Beta3->size()) break;
              if (!*m_bRisingRegime && iCnt >= map_dbl_BestParamSetBeta2Beta4->size()) break;
              if (*m_bRisingRegime)
                it_rising--;
              else
                it_falling--;
            }

            thdgrp.join_all();

            //--------------------------------------------------
            // checking result
            //--------------------------------------------------
            const int iTotCnt = iCnt;
            for (int i = 0; i < iTotCnt; ++i)
            {
              if (vdEstimate[i] > NIR_EPSILON)
              {
                dStrengthCount += 1;
              }
              else if (vdEstimate[i] < -NIR_EPSILON)
              {
                dStrengthCount -= 1;
              }
              m_TotSharpeOfMethod += vdSharpe[i];
              m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s vdSquaredError %f vdAvgPnL %f vdSharpe %f",
                              GetStrategyName(m_StyID).c_str(),
                              m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                              m_TradedSymbols[iTradSym].c_str(),
                              vdSquaredError[i],vdAvgPnL[i],vdSharpe[i]);
            }

            //--------------------------------------------------

            m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s Selected a set of %d parameters in total.",
                            GetStrategyName(m_StyID).c_str(),
                            m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                            m_TradedSymbols[iTradSym].c_str(),
                            iCnt);
          }
          else
          {
            m_Logger->Write(Logger::INFO,"Strategy %s: Sym=%s %s m_p_map_BestParamSet empty.",
                            GetStrategyName(m_StyID).c_str(),
                            m_TradedSymbols[iTradSym].c_str(),
                            m_p_ymdhms_SysTime_Local->ToStr().c_str());
          }

        }
        else
        {
          m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s %s (%d) m_bTrainRet is false.",
                          GetStrategyName(m_StyID).c_str(),
                          m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                          m_TradedSymbols[iTradSym].c_str(),
                          __FUNCTION__,
                          __LINE__);
        }
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s dStrengthCount (x2) %f Max Stren (x1) %d Max Stren (x2) %d",
                        GetStrategyName(m_StyID).c_str(),
                        m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                        m_TradedSymbols[iTradSym].c_str(),
                        dStrengthCount,
                        lMaxStren,
                        lMaxStren*2
                       );

        iTaylorIdx++;
      }

    }

    //--------------------------------------------------
    // Calculate actual position size
    //--------------------------------------------------
    m_dAggUnsignedQty = 0;
    m_dAggSignedQty = 0;
    if (m_map_dq_GKYZ.find(m_TradedSymbols[iTradSym]) == m_map_dq_GKYZ.end())
    {
      m_map_dq_GKYZ[m_TradedSymbols[iTradSym]] = deque<double>();
    }

    if (!std::isnan(m_dGKYZVal))
    {
      m_map_dq_GKYZ[m_TradedSymbols[iTradSym]].push_back(m_dGKYZVal);
    }

    m_dAggUnsignedQty = min(m_NotionalAmt / m_ChooseBestRotationGroupNum, m_MaxNotionalAmtEach) / m_SymMidQuote;
    m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s GKYZ.size() %d GKYZ %f m_dAggUnsignedQty (if not perform any volatility adj) %f.",
                    GetStrategyName(m_StyID).c_str(),
                    m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                    m_TradedSymbols[iTradSym].c_str(),
                    m_map_dq_GKYZ[m_TradedSymbols[iTradSym]].size(),
                    m_dGKYZVal,
                    m_dAggUnsignedQty);

    if (!(m_map_dq_GKYZ[m_TradedSymbols[iTradSym]].empty() || std::isnan(m_dGKYZVal) || m_MinAnnVolPsnSz <= 0))
    {
      m_dAggUnsignedQty = min(m_NotionalAmt / m_ChooseBestRotationGroupNum, m_MaxNotionalAmtEach) / m_SymMidQuote * min(m_MinAnnVolPsnSz / m_dGKYZVal,(double)1);
      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggUnsignedQty (after volatility adj) %f GKYZ %f.", GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),m_dAggUnsignedQty,m_dGKYZVal);
    }

    //--------------------------------------------------
    m_dAggSignedQty =
      round(m_dAggUnsignedQty * m_dStrengthCountTrdAtClose / (m_PropOfBestParam / (double)100 * (double)m_lNumOfTrngCombnAvgPx));

    //--------------------------------------------------
    // Taylor has 2 hypothesis
    //--------------------------------------------------
    if (!B2_SKIPMACHLEARNING
        &&
        B2_HYPOTHESIS == B2_HYPOTHESIS_TAYLOR)
    {
      m_dAggSignedQty = m_dAggSignedQty / 2.0;
      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty (after div by 2 because of 2 Taylor hypotheses) %f",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      m_dAggSignedQty);

      //--------------------------------------------------
      // no reason to hold position if position is too small, let others use the place
      //--------------------------------------------------
      double dAbsFullQty = round(min(m_NotionalAmt / m_ChooseBestRotationGroupNum, m_MaxNotionalAmtEach) / m_SymMidQuote);
      if (abs(m_dAggSignedQty) < B2_SETPOSSIZETOFULLORZERO * dAbsFullQty)
        m_dAggSignedQty = 0;
      else
        m_dAggSignedQty = (m_dAggSignedQty >= 0 ? 1 : -1) * dAbsFullQty;

      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty (after adj by B2_SETPOSSIZETOFULLORZERO) %f",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      m_dAggSignedQty);
      //--------------------------------------------------

    }
    //--------------------------------------------------

    //--------------------------------------------------
    if (B2_SKIPMACHLEARNING) m_dAggSignedQty = round(min(m_NotionalAmt / m_ChooseBestRotationGroupNum, m_MaxNotionalAmtEach) / m_SymMidQuote);
    //--------------------------------------------------

    //--------------------------------------------------
    // KolmogorovSmirnov
    //--------------------------------------------------
    const int iBestTrainingPeriod = (*m_bRisingRegime ? m_RiseRegimeBestTrainingPeriod[m_TradedSymbols[iTradSym]] : m_FallRegimeBestTrainingPeriod[m_TradedSymbols[iTradSym]]);
    if (!B2_SKIPMACHLEARNING &&
        m_KSBound > 0 &&
        m_KSRange > 0 &&
        m_HistoricalAvgPxRtn->size() >= iBestTrainingPeriod &&
        //--------------------------------------------------
        // only adjust size when price has gone down
        //--------------------------------------------------
        m_HistoricalAvgPx->size() > B2_KS_PRICE_FILTER_NDAYS+2
        //--------------------------------------------------
       )
    {
      vector<double> vdPeriod1;
      vector<double> vdPeriod2;
      vdPeriod1.insert(vdPeriod1.begin(),m_HistoricalAvgPxRtn->end()-iBestTrainingPeriod,m_HistoricalAvgPxRtn->end()-iBestTrainingPeriod/2);
      vdPeriod2.insert(vdPeriod2.begin(),m_HistoricalAvgPxRtn->end()-iBestTrainingPeriod/2,m_HistoricalAvgPxRtn->end());

      double ks = KolmogorovSmirnov::TwoSmpKSStat(vdPeriod1,vdPeriod2,20);
      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s vdPeriod1.size() = %d vdPeriod2.size() = %d KolmogorovSmirnov = %f",
                      GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),
                      vdPeriod1.size(), vdPeriod2.size(), ks);

      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty (before KolmogorovSmirnov adjustment)  %f",
                      GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(), m_dAggSignedQty);


      if (!std::isnan(ks))
      {
        if (m_HistoricalAvgPx->back() < *(m_HistoricalAvgPx->end() - B2_KS_PRICE_FILTER_NDAYS - 1))
          m_dAggSignedQty = m_dAggSignedQty * (1 - B2_KS_PRICE_FILTER_FALL_ADJFACTOR) + m_dAggSignedQty * (B2_KS_PRICE_FILTER_FALL_ADJFACTOR) * (min(max(m_KSBound-ks,(double)0),m_KSRange)) / m_KSRange;
        else
          m_dAggSignedQty = m_dAggSignedQty * (1 - B2_KS_PRICE_FILTER_RISE_ADJFACTOR) + m_dAggSignedQty * (B2_KS_PRICE_FILTER_RISE_ADJFACTOR) * (min(max(m_KSBound-ks,(double)0),m_KSRange)) / m_KSRange;
      }

      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty (after KolmogorovSmirnov adjustment)  %f",
                      GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(), m_dAggSignedQty);
    }
    //--------------------------------------------------

    //--------------------------------------------------
    // Adhoc filter: Filter away stocks with a declining trend
    //--------------------------------------------------
    if (!m_B2_FilterSMAPeriod.empty())
    {
      //--------------------------------------------------
      // short SMA long SMA
      //      1       -1      trade
      //     -1       -1      not trade
      //     -1        1      trade
      //      1        1      trade
      //--------------------------------------------------
      if (m_RotationMode == 1
          &&
          m_B2_FilterSMAPeriod.size() == 2
          &&
          m_v_SMA_short[iTradSym].Ready()
          &&
          m_v_SMA_long[iTradSym].Ready()
          &&
          m_HistoricalAvgPx->back() < m_v_SMA_short[iTradSym].Value()
          &&
          m_HistoricalAvgPx->back() < m_v_SMA_long[iTradSym].Value())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to zero because of the SMA filters. m_dAggSignedQty (bef adj) = %f Avg Px = %f SMA = %f SMA = %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, m_HistoricalAvgPx->back(), m_v_SMA_short[iTradSym].Value(), m_v_SMA_long[iTradSym].Value());
        m_dAggSignedQty = 0;
      }
      else if (m_RotationMode == 1
               &&
               m_B2_FilterSMAPeriod.size() == 1
               &&
               m_v_SMA_short[iTradSym].Ready()
               &&
               m_v_SMA_long[iTradSym].Ready()
               &&
               m_HistoricalAvgPx->back() < m_v_SMA_long[iTradSym].Value())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to zero because of the SMA filters. m_dAggSignedQty (bef adj) = %f Avg Px = %f SMA = %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, m_HistoricalAvgPx->back(), m_v_SMA_long[iTradSym].Value());
        m_dAggSignedQty = 0;
      }
      else if (m_RotationMode == -1
               &&
               m_B2_FilterSMAPeriod.size() == 2
               &&
               m_v_SMA_short[iTradSym].Ready()
               &&
               m_v_SMA_long[iTradSym].Ready()
               &&
               m_HistoricalAvgPx->back() > m_v_SMA_short[iTradSym].Value()
               &&
               m_HistoricalAvgPx->back() > m_v_SMA_long[iTradSym].Value())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to zero because of the SMA filters. m_dAggSignedQty (bef adj) = %f Avg Px = %f SMA = %f SMA = %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, m_HistoricalAvgPx->back(), m_v_SMA_short[iTradSym].Value(), m_v_SMA_long[iTradSym].Value());
        m_dAggSignedQty = 0;
      }
      else if (m_RotationMode == -1
               &&
               m_B2_FilterSMAPeriod.size() == 1
               &&
               m_v_SMA_long[iTradSym].Ready()
               &&
               m_HistoricalAvgPx->back() > m_v_SMA_long[iTradSym].Value())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to zero because of the SMA filters. m_dAggSignedQty (bef adj) = %f Avg Px = %f SMA = %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, m_HistoricalAvgPx->back(), m_v_SMA_long[iTradSym].Value());
        m_dAggSignedQty = 0;
      }
      else if (m_B2_FilterSMAPeriod.size() == 2 && m_v_SMA_long[iTradSym].Ready())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty = %f Avg Px = %f SMA = %f SMA = %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, m_HistoricalAvgPx->back(), m_v_SMA_short[iTradSym].Value(), m_v_SMA_long[iTradSym].Value());
      }
      else if (m_B2_FilterSMAPeriod.size() == 1 && m_v_SMA_long[iTradSym].Ready())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty = %f Avg Px = %f SMA = %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(),m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, m_HistoricalAvgPx->back(), m_v_SMA_long[iTradSym].Value());
      }
    }
    //--------------------------------------------------

    //--------------------------------------------------
    // Allow Trade Direction
    //--------------------------------------------------
    if      (m_AllowTrdDir ==  1) m_dAggSignedQty = max(m_dAggSignedQty,(double)0);
    else if (m_AllowTrdDir == -1) m_dAggSignedQty = min(m_dAggSignedQty,(double)0);


    //--------------------------------------------------
    // Close price relative to average price
    //--------------------------------------------------
    if (m_LongOnlyWhenClosePriceBelowAvgPrice.IsSome()
        &&
        STool::IsValidPriceOrVol(m_SymMidQuote) && !m_HistoricalAvgPx->empty())
    {
      double dClosePxAgstAvgPx = m_SymMidQuote / m_HistoricalAvgPx->back() - 1;
      if (dClosePxAgstAvgPx > m_LongOnlyWhenClosePriceBelowAvgPrice.Get())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to min(%f, 0). dClosePxAgstAvgPx %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, dClosePxAgstAvgPx);
        m_dAggSignedQty = min(m_dAggSignedQty,0.0);
      }
    }
    if (m_ShortOnlyWhenClosePriceAboveAvgPrice.IsSome()
        &&
        STool::IsValidPriceOrVol(m_SymMidQuote) && !m_HistoricalAvgPx->empty())
    {
      double dClosePxAgstAvgPx = m_SymMidQuote / m_HistoricalAvgPx->back() - 1;
      if (dClosePxAgstAvgPx < m_ShortOnlyWhenClosePriceAboveAvgPrice.Get())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to max(%f, 0). dClosePxAgstAvgPx %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, dClosePxAgstAvgPx);
        m_dAggSignedQty = max(m_dAggSignedQty,0.0);
      }
    }

    //--------------------------------------------------
    // Average price return
    //--------------------------------------------------
    if (m_LongOnlyWhenAvgPriceReturnAbove.IsSome()
        &&
        m_HistoricalAvgPx->size() >= 2)
    {
      double dAvgPxRtn = m_HistoricalAvgPx->back() / *(m_HistoricalAvgPx->end()-2) - 1;
      if (dAvgPxRtn < m_LongOnlyWhenAvgPriceReturnAbove.Get())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to min(%f, 0). dAvgPxRtn %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, dAvgPxRtn);
        m_dAggSignedQty = min(m_dAggSignedQty,0.0);
      }
    }
    if (m_ShortOnlyWhenAvgPriceReturnBelow.IsSome()
        &&
        m_HistoricalAvgPx->size() >= 2)
    {
      double dAvgPxRtn = m_HistoricalAvgPx->back() / *(m_HistoricalAvgPx->end()-2) - 1;
      if (dAvgPxRtn > m_ShortOnlyWhenAvgPriceReturnBelow.Get())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to max(%f, 0). dAvgPxRtn %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, dAvgPxRtn);
        m_dAggSignedQty = max(m_dAggSignedQty,0.0);
      }
    }

    //--------------------------------------------------
    // Rotation
    //--------------------------------------------------
    if (m_RotationMode != 0 && m_RotationGroup[iTradSym] >= 0)
    {
      //--------------------------------------------------
      // check whether there is enough historical data
      //--------------------------------------------------
      if (m_HistoricalAvgPx->size() < m_RotationModeUseNDayReturn)
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s Not enough data points to calculate %d-day return. %d",
                        GetStrategyName(m_StyID).c_str(),
                        m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                        m_TradedSymbols[iTradSym].c_str(),
                        m_RotationModeUseNDayReturn,
                        m_HistoricalAvgPx->size());

        m_TargetTradeDir [iTradSym] = TD_NODIR;
        m_TargetAbsPos   [iTradSym] = 0;
        *m_DoneEODTrade = true;

        return;
      }

      //--------------------------------------------------
      // calculate rolling return
      //--------------------------------------------------
      double dRollingReturn = (m_HistoricalAvgPx->back() / *(m_HistoricalAvgPx->end()-1-m_RotationModeUseNDayReturn)) - 1;
      m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s dRollingReturn (before voly adj) = %f m_HistoricalAvgPx %f %f",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      dRollingReturn,
                      m_HistoricalAvgPx->back(),
                      *(m_HistoricalAvgPx->end()-1-m_RotationModeUseVolyAdjdReturn));
      if (m_RotationModeUseVolyAdjdReturn)
      {
        dRollingReturn = dRollingReturn / TechIndicators::Instance()->GetGKYZValue(m_TradedSymbols[iTradSym]) * 100.0;
      }
      m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s dRollingReturn (after voly adj) = %f",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      dRollingReturn);

      //--------------------------------------------------
      // sticky group: get the groups whose position we will maintain
      // sticky stock: get the set of symbol whose position we will maintain (within the groups)
      //--------------------------------------------------

      if (B2_STICINESS_ON)
      {
        if (
          (
            m_RotationMode == 1
            &&
            GetPrevTheoSgndPos(m_TradedSymbols[iTradSym]) > 0
            &&
            m_dAggSignedQty > 0
          )
          || 
          (
            m_RotationMode == -1
            &&
            GetPrevTheoSgndPos(m_TradedSymbols[iTradSym]) < 0
            &&
            m_dAggSignedQty < 0
          )
          )
        {
          {
            //--------------------------------------------------
            // sticky stock within group
            //--------------------------------------------------
            map<YYYYMMDD,set<string> >::iterator it = m_MaintainPosWithinGrp[m_RotationGroup[iTradSym]].find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
            if (it == m_MaintainPosWithinGrp[m_RotationGroup[iTradSym]].end())
            {
              m_MaintainPosWithinGrp[m_RotationGroup[iTradSym]][m_p_ymdhms_SysTime_Local->GetYYYYMMDD()] = set<string>();
              it = m_MaintainPosWithinGrp[m_RotationGroup[iTradSym]].find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
            }
            it->second.insert(m_TradedSymbols[iTradSym]);
            m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s has a position that we will maintain. Previous pos = %f Current (raw) m_dAggSignedQty = %f",
                            GetStrategyName(m_StyID).c_str(),
                            m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                            m_TradedSymbols[iTradSym].c_str(),
                            GetPrevTheoSgndPos(m_TradedSymbols[iTradSym]),
                            m_dAggSignedQty);
          }

          {
            //--------------------------------------------------
            // sticky group
            //--------------------------------------------------
            map<YYYYMMDD,set<int> >::iterator it = m_MaintainGrp.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
            if (it == m_MaintainGrp.end())
            {
              m_MaintainGrp[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()] = set<int>();
              it = m_MaintainGrp.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
            }
            set<int> & setMtnGrp = it->second;
            setMtnGrp.insert(m_RotationGroup[iTradSym]);

            FForEach(setMtnGrp,[&](const int grp) {
              m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s Sticky Group %d",
                              GetStrategyName(m_StyID).c_str(),
                              m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                              m_TradedSymbols[iTradSym].c_str(),
                              grp);
            });
          }

        }
      }

      //--------------------------------------------------
      // save rolling return and m_dAggSignedQty
      //--------------------------------------------------
      AddNDayRollingReturn(m_RotationGroup[iTradSym],
                           m_p_ymdhms_SysTime_Local->GetYYYYMMDD(),
                           m_TradedSymbols[iTradSym],
                           dRollingReturn);
      AddAggSgndQty(m_p_ymdhms_SysTime_Local->GetYYYYMMDD(),
                    m_TradedSymbols[iTradSym],
                    m_dAggSignedQty);
      AddAggSgndNotnl(m_p_ymdhms_SysTime_Local->GetYYYYMMDD(),
                      m_RotationGroup[iTradSym],
                      m_TradedSymbols[iTradSym],
                      m_SymMidQuote * m_dAggSignedQty);

      //--------------------------------------------------
      // check if all rolling returns are available
      //--------------------------------------------------
      if (!NDayRollingReturnReadyForAllSym(m_RotationGroup[iTradSym],m_p_ymdhms_SysTime_Local->GetYYYYMMDD()))
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s Not all %d-day returns are ready.",
                        GetStrategyName(m_StyID).c_str(),
                        m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                        m_TradedSymbols[iTradSym].c_str(),
                        m_RotationModeUseNDayReturn);
        return;
      }
      m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s All %d-day returns are ready.",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      m_RotationModeUseNDayReturn);

      //--------------------------------------------------
      // check if all aggregate signed quantity is available
      //--------------------------------------------------
      if (!AggSgndQtyReadyForAllSym(m_p_ymdhms_SysTime_Local->GetYYYYMMDD()))
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s Not all aggregate signed qty are ready.",
                        GetStrategyName(m_StyID).c_str(),
                        m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                        m_TradedSymbols[iTradSym].c_str());
        return;
      }
      m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s All aggregate signed qty are ready.",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str());

      //--------------------------------------------------
      // check if we have updated internal data FOR ALL symbols before generating signals
      //--------------------------------------------------
      if (m_AllSymWithUpdateDataAtClose[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].size() <
          m_AllAvbSym[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].size())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s Not all symbols have up-to-date internal data",
                        GetStrategyName(m_StyID).c_str(),
                        m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                        m_TradedSymbols[iTradSym].c_str());
        FForEach(m_AllSymWithUpdateDataAtClose[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()],[&](const string & sym) {
          m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s With update internal data",
                          GetStrategyName(m_StyID).c_str(),
                          m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                          sym.c_str());
        });
        return;
      }

      //--------------------------------------------------
      // if not performed the rotation logic, do it now, but do it once only
      //--------------------------------------------------
      if (!m_HasPerformedRotationLogic.Contains(m_p_ymdhms_SysTime_Local->GetYYYYMMDD()))
      {
        //--------------------------------------------------
        // check the average return of the groups
        //--------------------------------------------------
        {
          map<YYYYMMDD,vector<double> >::iterator it = m_AvgRtnOfGrp.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
          if (it == m_AvgRtnOfGrp.end())
          {
            vector<double> vGrpRtn;
            vGrpRtn.insert(vGrpRtn.begin(), B2_ROTATION_GROUP_MAX, NAN);
            m_AvgRtnOfGrp[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()] = vGrpRtn;
            it = m_AvgRtnOfGrp.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
          }

          vector<double> & vGrpRtn = it->second;

          FForEach(m_RotationGroup,[&](const int grp){
            if (std::isnan(vGrpRtn[grp])) vGrpRtn[grp] = GetAvgRollgRetOfGrp(grp, m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
          });

          m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- Group Avg Return ---", GetStrategyName(m_StyID).c_str());
          m_NumOfAvbGrp = 0;
          for (int grp = 0; grp < B2_ROTATION_GROUP_MAX; ++grp)
          {
            if (!std::isnan(vGrpRtn[grp]))
            {
              m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Group %d Avg Return %f",
                              GetStrategyName(m_StyID).c_str(),
                              m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                              grp, vGrpRtn[grp]);
              m_NumOfAvbGrp++;
            }
          }
          m_ChooseBestRotationGroupNum = (int)ceil(m_ChooseBestRotationGroupProp * m_NumOfAvbGrp);
          m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- Group Avg Return ---", GetStrategyName(m_StyID).c_str());
          m_Logger->Write(Logger::INFO,"Strategy %s: %s Rotation mode: m_NumOfAvbGrp %d m_ChooseBestRotationGroupNum %d",
                          GetStrategyName(m_StyID).c_str(),
                          m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                          m_NumOfAvbGrp,
                          m_ChooseBestRotationGroupNum
                         );
        }


        {
          map<YYYYMMDD,vector<Option<string> > >::iterator it = m_RttnGrpWithSgnl.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
          if (it == m_RttnGrpWithSgnl.end())
          {
            vector<Option<string> > vSymWithSgnl;
            vSymWithSgnl.insert(vSymWithSgnl.begin(), B2_ROTATION_GROUP_MAX, Option<string>());
            m_RttnGrpWithSgnl[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()] = vSymWithSgnl;
            it = m_RttnGrpWithSgnl.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
          }

          vector<Option<string> > & vSymWithSgnl = it->second;

          for (int iTradSymRttnGrp = 0; iTradSymRttnGrp < m_TradedSymbols.size(); ++iTradSymRttnGrp)
          {
            if (m_RotationGroup[iTradSymRttnGrp] < 0) continue;
            //--------------------------------------------------
            // pick stocks for trading in each group
            //--------------------------------------------------
            const set<string> & setMtnPsn = m_MaintainPosWithinGrp[m_RotationGroup[iTradSymRttnGrp]][m_p_ymdhms_SysTime_Local->GetYYYYMMDD()];

            //--------------------------------------------------
            // those maintained stocks have precedence over others
            //--------------------------------------------------
            if (setMtnPsn.find(m_TradedSymbols[iTradSymRttnGrp]) != setMtnPsn.end())
            {
              vSymWithSgnl[m_RotationGroup[iTradSymRttnGrp]] = Option<string>(m_TradedSymbols[iTradSymRttnGrp]);
            }
          }

          //--------------------------------------------------
          FReverse(m_SymRankedByAvgSharpeRiseRgm);
          std::sort(m_SymRankedByAvgSharpeRiseRgm.begin(), m_SymRankedByAvgSharpeRiseRgm.end());
          FReverse(m_SymRankedByAvgSharpeRiseRgm);
          FReverse(m_SymRankedByAvgSharpeFallRgm);
          std::sort(m_SymRankedByAvgSharpeFallRgm.begin(), m_SymRankedByAvgSharpeFallRgm.end());
          FReverse(m_SymRankedByAvgSharpeFallRgm);
          m_Logger->Write(Logger::INFO,"Strategy %s: %s Rotation mode: --- m_SymRankedByAvgSharpe ---",
                          GetStrategyName(m_StyID).c_str(),
                          m_p_ymdhms_SysTime_Local->ToStr().c_str());
          FForEach(m_SymRankedByAvgSharpeRiseRgm,[&](const TupAvgSharpeSymIdx & tup) {
            m_Logger->Write(Logger::INFO,"Strategy %s: %s Rotation mode: m_SymRankedByAvgSharpeRiseRgm [Group %d] %s %f",
                            GetStrategyName(m_StyID).c_str(),
                            m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                            m_RotationGroup[tup.m_symidx],
                            m_TradedSymbols[tup.m_symidx].c_str(),
                            tup.m_avgsharpe);
          });
          FForEach(m_SymRankedByAvgSharpeFallRgm,[&](const TupAvgSharpeSymIdx & tup) {
            m_Logger->Write(Logger::INFO,"Strategy %s: %s Rotation mode: m_SymRankedByAvgSharpeFallRgm [Group %d] %s %f",
                            GetStrategyName(m_StyID).c_str(),
                            m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                            m_RotationGroup[tup.m_symidx],
                            m_TradedSymbols[tup.m_symidx].c_str(),
                            tup.m_avgsharpe);
          });
          m_Logger->Write(Logger::INFO,"Strategy %s: %s Rotation mode: --- m_SymRankedByAvgSharpe ---",
                          GetStrategyName(m_StyID).c_str(),
                          m_p_ymdhms_SysTime_Local->ToStr().c_str());
          //--------------------------------------------------


          for (int iRttnGrp = 0; iRttnGrp < B2_ROTATION_GROUP_MAX; ++iRttnGrp)
          {
            int iRank = 1;
            while (
              vSymWithSgnl[iRttnGrp].IsNone()
              &&
              iRank <=
              m_AllAvbSymForRollingBasket[iRttnGrp][m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].size()
              )
            {
              ASC_DESC ad;
              if (m_RotationMode == 1)
                ad = AD_DESCENDING;
              else if (m_RotationMode == -1)
                ad = AD_ASCENDING;

              // Option<string> o_sym = GetSymInGrpWithRank(m_SymRankedByRollingReturn,
              //                                             iRttnGrp,
              //                                             m_p_ymdhms_SysTime_Local->GetYYYYMMDD(),
              //                                             iRank,
              //                                             ad);

              Option<string> o_sym;
              if (*m_bRisingRegime)
              {
                o_sym = GetSymInGrpWithRank(m_SymRankedByAvgSharpeRiseRgm,
                                            iRttnGrp,
                                            iRank);
              }
              else
              {
                o_sym = GetSymInGrpWithRank(m_SymRankedByAvgSharpeFallRgm,
                                            iRttnGrp,
                                            iRank);
              }

              if (o_sym.IsSome())
              {
                double dAggSgndQty = m_SymAggSgndQty[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()][o_sym.GetOrElse("")];

                if (
                  (
                    (m_RotationMode == 1 && dAggSgndQty > 0)
                    ||
                    (m_RotationMode == -1 && dAggSgndQty < 0)
                  )
                  &&
                  (m_mapRoleOfSym[o_sym.Get()] > 0) // not indicator symbol
                  )
                {
                  vSymWithSgnl[iRttnGrp] = o_sym;
                }
              }

              m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: Rotation group %d Rank %d m_AllAvbSymForRollingBasket.size() %d vSymWithSgnl %s",
                              GetStrategyName(m_StyID).c_str(),
                              iRttnGrp,
                              iRank,
                              m_AllAvbSymForRollingBasket[iRttnGrp][m_p_ymdhms_SysTime_Local->GetYYYYMMDD()].size(),
                              vSymWithSgnl[iRttnGrp].GetOrElse("___").c_str()
                             );
              iRank++;

              //--------------------------------------------------
              // if the option isn't set, we don't move the next best stock forward for trading
              //--------------------------------------------------
              if (!m_MoveNextBestStkInGrpUpIfNoSignal && iRank > B2_ROTATION_PICKTOPSYM) break;
              //--------------------------------------------------

            }

            FForEach(m_AllAvbSymForRollingBasket[iRttnGrp][m_p_ymdhms_SysTime_Local->GetYYYYMMDD()], [&](const string & s) {
              m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: Available symbol for the group %d: %s", GetStrategyName(m_StyID).c_str(), iRttnGrp, s.c_str());
            });


          }

          m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- Symbol with signal ---", GetStrategyName(m_StyID).c_str());
          for (int grp = 0; grp < vSymWithSgnl.size(); ++grp)
          {
            m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Group %d Symbol with signal %s",
                            GetStrategyName(m_StyID).c_str(),
                            m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                            grp, vSymWithSgnl[grp].GetOrElse("___").c_str());
          }
          m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- Symbol with signal ---", GetStrategyName(m_StyID).c_str());

          for (int iRttnGrp = 0; iRttnGrp < B2_ROTATION_GROUP_MAX; ++iRttnGrp)
          {
            double dAvgSgndNotnlOfRttnGrp = GetAvgAggSgndNotnlOfGrp(iRttnGrp,m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
            m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Group %d Avg Sgnd Notional of Grp %f",
                            GetStrategyName(m_StyID).c_str(),
                            m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                            iRttnGrp, dAvgSgndNotnlOfRttnGrp);

            if (
              (m_RotationMode == 1 && dAvgSgndNotnlOfRttnGrp < m_AvgAggSgndNotnlThresh)
              ||
              (m_RotationMode == -1 && dAvgSgndNotnlOfRttnGrp > m_AvgAggSgndNotnlThresh)
              )
            {
              //--------------------------------------------------
              // check the average signed notional of each group,
              // if it is smaller than the threshold, then remove all the symbols from the group
              //--------------------------------------------------
              vSymWithSgnl[iRttnGrp] = Option<string>();
            }

          }
          //--------------------------------------------------

        }

        //--------------------------------------------------
        // sort the groups (with stock positions) by their returns
        //--------------------------------------------------
        {
          map<YYYYMMDD,vector<Option<string> > >::iterator it1 = m_RttnGrpWithSgnl.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
          map<YYYYMMDD,vector<double> >::iterator it2 = m_AvgRtnOfGrp.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());

          vector<Option<string> > & vSymWithSgnl = it1->second;
          vector<double> & vGrpRtn = it2->second;

          map<YYYYMMDD,vector<TupRetSym> >::iterator it = m_GrpRtnAndLeadingSym.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
          if (it == m_GrpRtnAndLeadingSym.end())
          {
            vector<TupRetSym> mGrpRtnAndLeadSym;
            m_GrpRtnAndLeadingSym[m_p_ymdhms_SysTime_Local->GetYYYYMMDD()] = mGrpRtnAndLeadSym;
            it  = m_GrpRtnAndLeadingSym.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
          }

          vector<TupRetSym> & mGrpRtnAndLeadSym = it->second;

          for (int grp = 0; grp < B2_ROTATION_GROUP_MAX; ++grp)
          {
            // if (vSymWithSgnl[grp].IsNone()) continue;
            if (std::isnan(vGrpRtn[grp])) continue;
            mGrpRtnAndLeadSym.push_back(TupRetSym(vGrpRtn[grp],vSymWithSgnl[grp].GetOrElse(""),grp));
          }

          //--------------------------------------------------
          // pick only the top groups with signals
          //--------------------------------------------------
          if (!mGrpRtnAndLeadSym.empty())
          {

            m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- mGrpRtnAndLeadSym (before sorting) ---", GetStrategyName(m_StyID).c_str());

            int grp = 0;
            FForEach(mGrpRtnAndLeadSym,[&](const TupRetSym & tup) {
              m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: mGrpRtnAndLeadSym: Group %d %f %s (Grp %d)",
                              GetStrategyName(m_StyID).c_str(),
                              grp,
                              tup.m_return,
                              tup.m_symbol().c_str(),
                              tup.m_rttngrp);
              grp++;
            });

            m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- mGrpRtnAndLeadSym (sorted) ---", GetStrategyName(m_StyID).c_str());

            std::sort(mGrpRtnAndLeadSym.begin(), mGrpRtnAndLeadSym.end());

            FForEach(mGrpRtnAndLeadSym,[&](const TupRetSym & tup) {
              m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: (ascending order) mGrpRtnAndLeadSym: %f %s (Grp %d)",
                              GetStrategyName(m_StyID).c_str(),
                              tup.m_return,
                              tup.m_symbol().c_str(),
                              tup.m_rttngrp);
            });

            if (m_RotationModeTradeHighestReturn) FReverse(mGrpRtnAndLeadSym);

            FForEach(mGrpRtnAndLeadSym,[&](const TupRetSym & tup) {
              m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: (according to rotation scheme) mGrpRtnAndLeadSym: %f %s (Grp %d)",
                              GetStrategyName(m_StyID).c_str(),
                              tup.m_return,
                              tup.m_symbol().c_str(),
                              tup.m_rttngrp);
            });
            m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- mGrpRtnAndLeadSym ---", GetStrategyName(m_StyID).c_str());


            {
              //--------------------------------------------------
              // sticky group: here we move sticky groups to the front
              //--------------------------------------------------
              vector<TupRetSym> newGrpRtnAndLeadSym;
              map<YYYYMMDD,set<int> >::iterator it = m_MaintainGrp.find(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());
              if (it != m_MaintainGrp.end())
              {
                const set<int> & setMtnGrp = it->second;

                //--------------------------------------------------
                // filter those sticky groups
                //--------------------------------------------------
                FForEach(mGrpRtnAndLeadSym,[&](const TupRetSym & tup) {
                  if (setMtnGrp.find(tup.m_rttngrp) != setMtnGrp.end())
                    newGrpRtnAndLeadSym.push_back(tup);
                });
                //--------------------------------------------------
                // filter those non-sticky groups
                //--------------------------------------------------
                FForEach(mGrpRtnAndLeadSym,[&](const TupRetSym & tup) {
                  if (setMtnGrp.find(tup.m_rttngrp) == setMtnGrp.end())
                    newGrpRtnAndLeadSym.push_back(tup);
                });

                mGrpRtnAndLeadSym = newGrpRtnAndLeadSym;
              }

              FForEach(mGrpRtnAndLeadSym,[&](const TupRetSym & tup) {
                m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: (after moving sticky group to front) mGrpRtnAndLeadSym: %f %s (Grp %d)",
                                GetStrategyName(m_StyID).c_str(),
                                tup.m_return,
                                tup.m_symbol().c_str(),
                                tup.m_rttngrp);
              });
              m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- mGrpRtnAndLeadSym ---", GetStrategyName(m_StyID).c_str());

            }
            //--------------------------------------------------


            int iCnt = 0;
            while (true)
            {
              if (!m_MoveNextBestGroupUpIfNoSignal)
              {
                if (iCnt >= m_ChooseBestRotationGroupNum) break;
              }
              else
              {
                if (m_StkPicks.size() >= m_ChooseBestRotationGroupNum) break;
              }

              //--------------------------------------------------
              // if there is a choosen symbol in the group
              // and
              // if the group return is at least positive
              // then pick the stock
              //--------------------------------------------------
              if (mGrpRtnAndLeadSym[iCnt].m_symbol() != "")
              {
                if (m_LongOnlyWhenGrpAvgReturnAbove.IsNone())
                {
                  m_StkPicks.insert(mGrpRtnAndLeadSym[iCnt].m_symbol());
                }
                else
                {
                  if (mGrpRtnAndLeadSym[iCnt].m_return > m_LongOnlyWhenGrpAvgReturnAbove.Get())
                  {
                    m_StkPicks.insert(mGrpRtnAndLeadSym[iCnt].m_symbol());
                  }
                  else
                  {
                    m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Stock %s is not picked because group return is smaller than threshold %f",
                                    GetStrategyName(m_StyID).c_str(),
                                    m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                                    mGrpRtnAndLeadSym[iCnt].m_symbol().c_str(),
                                    m_LongOnlyWhenGrpAvgReturnAbove.Get());
                  }
                }
              }

              iCnt++;
              if (iCnt >= mGrpRtnAndLeadSym.size()) break;
            }
          }

          m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- Stock pick ---", GetStrategyName(m_StyID).c_str());
          for (set<string>::iterator it = m_StkPicks.begin(); it != m_StkPicks.end(); ++it)
          {
            m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Stock pick %s",
                            GetStrategyName(m_StyID).c_str(),
                            m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                            it->c_str());
          }
          m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: --- Stock pick ---", GetStrategyName(m_StyID).c_str());
        }

        //--------------------------------------------------
        // mark done rotation logic
        //--------------------------------------------------
        m_HasPerformedRotationLogic.Add(m_p_ymdhms_SysTime_Local->GetYYYYMMDD());

      }

      //--------------------------------------------------
      // if not in the correct direction, set quantity to zero
      //--------------------------------------------------
      if (m_RotationMode == 1
          &&
          m_dAggSignedQty < 0)
      {
        m_dAggSignedQty = 0;
      }
      else if (m_RotationMode == -1
               &&
               m_dAggSignedQty > 0)
      {
        m_dAggSignedQty = 0;
      }

      //--------------------------------------------------
      // set 0 to trade position if this is not the stock that we should keep
      //--------------------------------------------------
      if (m_StkPicks.find(m_TradedSymbols[iTradSym]) == m_StkPicks.end())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s is not the chosen one. Set position to 0.",
                        GetStrategyName(m_StyID).c_str(),
                        m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                        m_TradedSymbols[iTradSym].c_str());

        m_TargetTradeDir [iTradSym] = TD_NODIR;
        m_TargetAbsPos   [iTradSym] = 0;
        *m_DoneEODTrade = true;

        return;
      }

    }

    //--------------------------------------------------
    // Round down to lot size if HK stocks
    //--------------------------------------------------
    if (m_StyID == STY_B2_HK && GetStyDomicileMkt() == SDM_HK)
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s Before lot size round down: m_dAggSignedQty = %f",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      m_dAggSignedQty);
      m_dAggSignedQty = m_SysCfg->RoundDownLotSize(m_TradedSymbols[iTradSym],(long)m_dAggSignedQty);
      m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s After lot size round down: m_dAggSignedQty = %f",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      m_dAggSignedQty);
    }

    //--------------------------------------------------
    // Position size after considering the effect of commission
    //--------------------------------------------------
    if (m_B2_HasEnabledMinCommissionCheck)
    {

      if (
        //--------------------------------------------------
        // if position is to be set to zero, do it! ignore commission impact
        //--------------------------------------------------
        abs(m_dAggSignedQty) > NIR_EPSILON
        &&
        CommissionCalculator::CalcCommission(
          CommissionCalculator::IB,
          CommissionCalculator::USSTK,
          CommissionCalculator::SPOT,
          m_SymMidQuote,
          m_dAggSignedQty) / (m_SymMidQuote * abs(m_dAggSignedQty)) > m_SysCfg->B2_CommissionRateThreshold(m_StyID)
        )
      {
        //--------------------------------------------------
        // looking one step ahead - when we actually have to close the remaining position
        // the target quantity is too small, just make the quantity zero.
        // not worth keeping a tiny position here
        //--------------------------------------------------
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty will be adjusted to zero after considering commission impact. m_SymMidQuote = %f m_dAggSignedQty = %f",
                        GetStrategyName(m_StyID).c_str(),
                        m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                        m_TradedSymbols[iTradSym].c_str(),
                        m_SymMidQuote,
                        m_dAggSignedQty);
        m_dAggSignedQty = 0;
      }

    }

    double dAbsDeltaQty = abs(m_dAggSignedQty - GetPrevTheoSgndPos(m_TradedSymbols[iTradSym]));
    if (
      !m_B2_HasEnabledMinCommissionCheck
      ||
      //--------------------------------------------------
      // if position is to be set to zero, do it! ignore commission impact
      //--------------------------------------------------
      abs(m_dAggSignedQty) < NIR_EPSILON
      ||
      (
        m_B2_HasEnabledMinCommissionCheck
        &&
        CommissionCalculator::CalcCommission(
          CommissionCalculator::IB,
          CommissionCalculator::USSTK,
          CommissionCalculator::SPOT,
          m_SymMidQuote,
          dAbsDeltaQty) / (m_SymMidQuote * dAbsDeltaQty) <= m_SysCfg->B2_CommissionRateThreshold(m_StyID)
      )
      )
    {
      if (!(*m_DoneEODTrade))
      {
        m_TargetTradeDir [iTradSym] = (m_dAggSignedQty > 0 ? TD_LONG : TD_SHORT);
        m_TargetAbsPos   [iTradSym] = round(abs(m_dAggSignedQty));
      }
    }
    else
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: Rotation mode: %s Sym=%s Trade wasn't initiated because the commission expense rate is higher than threshold. m_SymMidQuote = %f m_dAggSignedQty = %f",
                      GetStrategyName(m_StyID).c_str(),
                      m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                      m_TradedSymbols[iTradSym].c_str(),
                      m_SymMidQuote,
                      m_dAggSignedQty);
    }


    *m_DoneEODTrade = true;
    //--------------------------------------------------

    return;
  }

}

void StrategyB2::CalcPosSize(const int iTradSym)
{

}

void StrategyB2::UnsetConvenienceVarb()
{
  m_HistoricalAvgPx                      = NULL;
  m_HistoricalClose                      = NULL;
  m_HistoricalNumOfRisingDaysCountAvgPx  = NULL;
  m_DoneSODTrade                         = NULL;
  m_DoneEODTrade                         = NULL;
  m_bTrainRetAvgPx                       = NULL;
  m_bTrainRetClose                       = NULL;
  m_bRisingRegime                        = NULL;

  return;
}

void StrategyB2::ParamSanityCheck()
{
  if (m_RotationMode != 0)
  {
    if (
      m_vRoleOfSym.size() != m_TradedSymbols.size()
      ||
      m_ParamVector.size() != m_iNumOfParam
      )
    {
      m_Logger->Write(Logger::ERROR,"Strategy %s: Failed sanity test", GetStrategyName(m_StyID).c_str());
      sleep(2);
      exit(1);
    }
  }
  return;
}

void StrategyB2::LoadOptimalParamsFromTrainingResults()
{
  return;
}

bool StrategyB2::NDayRollingReturnReadyForAllSym(const int gid, const YYYYMMDD & ymd)
{
  map<YYYYMMDD,map<string,double> >::iterator it = m_SymAndRollingReturn[gid].find(ymd);
  if (it == m_SymAndRollingReturn[gid].end())
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: Date %s not found in m_SymAndRollingReturn. gid = %d",
                    GetStrategyName(m_StyID).c_str(), ymd.ToStr().c_str(),
                    gid);
    return false;
  }

  map<string,double> & mapTmp = it->second;

  if (mapTmp.size() < m_AllAvbSymForRollingBasket[gid][ymd].size())
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: gid = %d No of rolling return obtained = %d. Num of available symbols = %d",
                    GetStrategyName(m_StyID).c_str(), gid,
                    mapTmp.size(),
                    m_AllAvbSymForRollingBasket[gid][ymd].size());

    FForEach(mapTmp,[&](const std::pair<string,double> tup) {
      m_Logger->Write(Logger::INFO,"Strategy %s: gid = %d Rolling return obtained for %s",
                      GetStrategyName(m_StyID).c_str(), gid,
                      tup.first.c_str());
    });

    FForEach(m_AllAvbSymForRollingBasket[gid][ymd],[&](const string & s) {
      m_Logger->Write(Logger::INFO,"Strategy %s: gid = %d Available symbols %s",
                      GetStrategyName(m_StyID).c_str(), gid,
                      s.c_str());
    });

    return false;
  }

  return true;
}

bool StrategyB2::AggSgndQtyReadyForAllSym(const YYYYMMDD & ymd)
{
  map<YYYYMMDD,map<string,double> >::iterator it = m_SymAggSgndQty.find(ymd);
  if (it == m_SymAggSgndQty.end())
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: Date %s not found in m_SymAggSgndQty.",
                    GetStrategyName(m_StyID).c_str(), ymd.ToStr().c_str());
    return false;
  }

  map<string,double> & mapTmp = it->second;

  if (mapTmp.size() < m_AllAvbSym[ymd].size())
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: No of aggregate signed qty obtained = %d. Num of available symbols = %d",
                    GetStrategyName(m_StyID).c_str(),
                    mapTmp.size(), m_AllAvbSym[ymd].size());

    FForEach(mapTmp,[&](const std::pair<string,double> tup) {
      m_Logger->Write(Logger::INFO,"Strategy %s: Aggregate signed qty obtained for %s",
                      GetStrategyName(m_StyID).c_str(), tup.first.c_str());
    });

    FForEach(m_AllAvbSym[ymd],[&](const string & s) {
      m_Logger->Write(Logger::INFO,"Strategy %s: Available symbols %s",
                      GetStrategyName(m_StyID).c_str(), s.c_str());
    });

    return false;
  }

  return true;
}

void StrategyB2::AddNDayRollingReturn(const int gid, const YYYYMMDD & ymd, const string & sym, const double ret)
{
  //--------------------------------------------------
  // don't add if NAN
  //--------------------------------------------------
  if (std::isnan(ret)) return;

  if (m_AllAvbSymForRollingBasket[gid][ymd].find(sym) == m_AllAvbSymForRollingBasket[gid][ymd].end()) return;

  map<YYYYMMDD,map<string,double> >::iterator it1 = m_SymAndRollingReturn[gid].find(ymd);
  map<YYYYMMDD,vector<TupRetSym> >::iterator it2 = m_SymRankedByRollingReturn[gid].find(ymd);
  if (it1 == m_SymAndRollingReturn[gid].end())
  {
    m_SymAndRollingReturn[gid][ymd] = map<string,double>();
    it1 = m_SymAndRollingReturn[gid].find(ymd);
    m_SymRankedByRollingReturn[gid][ymd] = vector<TupRetSym>();
    it2 = m_SymRankedByRollingReturn[gid].find(ymd);
  }
  else
  {
    //--------------------------------------------------
    // added already, won't add second time
    //--------------------------------------------------
    if (it1->second.find(sym) != it1->second.end()) return;
  }

  //--------------------------------------------------
  // add now
  //--------------------------------------------------
  it1->second[sym] = ret;
  it2->second.push_back(TupRetSym(ret, sym, gid));

}

void StrategyB2::AddAvgSharpeOfSym(const int symidx, const double avgsharpe, const int regime)
{
  //--------------------------------------------------
  // don't add if NAN
  //--------------------------------------------------
  if (std::isnan(avgsharpe)) return;

  if (regime == B2_RISEREGIME)
  {
    for (vector<TupAvgSharpeSymIdx>::iterator it = m_SymRankedByAvgSharpeRiseRgm.begin(); it != m_SymRankedByAvgSharpeRiseRgm.end(); ++it)
    {
      if (it->m_symidx == symidx)
      {
        m_SymRankedByAvgSharpeRiseRgm.erase(it);
        break;
      }
    }
    m_SymRankedByAvgSharpeRiseRgm.push_back(TupAvgSharpeSymIdx(avgsharpe, symidx));
  }
  else if (regime == B2_FALLREGIME)
  {
    for (vector<TupAvgSharpeSymIdx>::iterator it = m_SymRankedByAvgSharpeFallRgm.begin(); it != m_SymRankedByAvgSharpeFallRgm.end(); ++it)
    {
      if (it->m_symidx == symidx)
      {
        m_SymRankedByAvgSharpeFallRgm.erase(it);
        break;
      }
    }
    m_SymRankedByAvgSharpeFallRgm.push_back(TupAvgSharpeSymIdx(avgsharpe, symidx));
  }
}

void StrategyB2::AddAggSgndQty(const YYYYMMDD & ymd, const string & sym, const double dAggSgndQty)
{
  map<YYYYMMDD,map<string,double> >::iterator it = m_SymAggSgndQty.find(ymd);

  if (it == m_SymAggSgndQty.end())
  {
    m_SymAggSgndQty[ymd] = map<string,double>();
    it = m_SymAggSgndQty.find(ymd);
  }

  (it->second)[sym] = dAggSgndQty;
}

void StrategyB2::AddAggSgndNotnl(const YYYYMMDD & ymd, const int gid, const string & sym, const double dStrengthIndi)
{
  map<YYYYMMDD,map<int,map<string,double> > >::iterator it = m_AggSgndNotnlOfSym.find(ymd);

  if (it == m_AggSgndNotnlOfSym.end())
  {
    m_AggSgndNotnlOfSym[ymd] = map<int,map<string,double> >();
    it = m_AggSgndNotnlOfSym.find(ymd);
  }

  map<int,map<string,double> > & mapTmp = it->second;

  map<int,map<string,double> >::iterator it2 = mapTmp.find(gid);
  if (it2 == mapTmp.end())
  {
    mapTmp[gid] = map<string,double>();
    it2 = mapTmp.find(gid);
  }

  (it2->second)[sym] = dStrengthIndi;
}


Option<string> StrategyB2::GetSymInGrpWithRank(const vector<map<YYYYMMDD,vector<TupRetSym> > > & vecmapYMD, const int gid, const YYYYMMDD & ymd, const int rank, const ASC_DESC ad)
{
  map<YYYYMMDD,vector<TupRetSym> >::const_iterator it2 = vecmapYMD[gid].find(ymd);
  if (it2 == vecmapYMD[gid].end()) return Option<string>();

  vector<TupRetSym> vecTmp = it2->second;
  if (vecTmp.empty()) return Option<string>();

  std::sort(vecTmp.begin(), vecTmp.end());

  if (ad == AD_ASCENDING && vecTmp.size() >= rank) return Option<string>(vecTmp[rank-1].m_symbol());
  if (ad == AD_DESCENDING && vecTmp.size() >= rank) return Option<string>(vecTmp[vecTmp.size()-rank].m_symbol());

  return Option<string>();
}

Option<string> StrategyB2::GetSymInGrpWithRank(const vector<TupAvgSharpeSymIdx> & sortedRevVecTup, const int gid, const int rank)
{
  int iRank = 1;
  for (vector<TupAvgSharpeSymIdx>::const_iterator it = sortedRevVecTup.begin(); it != sortedRevVecTup.end(); ++it)
  {
    if (m_RotationGroup[it->m_symidx] == gid)
    {
      if (iRank == rank) return Option<string>(m_TradedSymbols[it->m_symidx]);
      iRank++;
    }
  };

  return Option<string>();
}

double StrategyB2::GetAvgRollgRetOfGrp(const int gid,const YYYYMMDD & ymd)
{
  map<YYYYMMDD,vector<TupRetSym> >::iterator it2 = m_SymRankedByRollingReturn[gid].find(ymd);
  if (it2 == m_SymRankedByRollingReturn[gid].end()) return NAN;

  const vector<TupRetSym> vt = FFilterNot(it2->second,[](const TupRetSym &tup){ return std::isnan(tup.m_return); });

  int iCnt = vt.size();
  double dTot = FFold(vt,(double)0,[](const double &a, const TupRetSym &b) { return a+b.m_return; } );

  if (iCnt > 0) return dTot / (double)iCnt;
  else return NAN;
}

double StrategyB2::GetAvgAggSgndNotnlOfGrp(const int gid,const YYYYMMDD & ymd)
{
  map<YYYYMMDD,map<int,map<string,double> > >::iterator it = m_AggSgndNotnlOfSym.find(ymd);
  if (it == m_AggSgndNotnlOfSym.end()) return NAN;

  map<int,map<string,double> > & mapTmp = it->second;
  map<int,map<string,double> >::iterator it2 = mapTmp.find(gid);
  if (it2 == mapTmp.end()) return NAN;

  map<string,double> & mapTmp2 = it2->second;

  vector<double> vt;
  FForEach(mapTmp2,[&](const std::pair<string,double> symStren) {
    if (!std::isnan(symStren.second)) vt.push_back(symStren.second);
  });

  int iCnt = vt.size();
  double dTot = FFold(vt,(double)0,[](const double c, const double e) { return c+e; } );

  if (iCnt > 0) return dTot / (double)iCnt;
  else return NAN;
}

