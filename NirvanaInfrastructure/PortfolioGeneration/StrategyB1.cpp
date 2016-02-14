#include <StrategyB1.h>

StrategyB1::StrategyB1()
{
}

StrategyB1::~StrategyB1()
{
}

bool StrategyB1::TestRun(const TestRunParm & gep)
{

  const double trainingperiod           =   gep.m_trainingperiod  ;
  const double ma_period                =   gep.m_ma_period       ;
  const vector<double> & v_hist_forsma  = * gep.m_v_hist_forsma   ;
  const vector<double> & v_hist_forpnl  = * gep.m_v_hist_forpnl   ;
  vector<int> & v_signal_trail          = * gep.m_v_signal_trail  ;
  const vector<bool> & v_disable_days   = * gep.m_v_disable_days  ;
  double * pdAvgPnL                     =   gep.m_pdAvgPnL        ;
  double * pdSharpe                     =   gep.m_pdSharpe        ;
  WEIGHTING_SCHEME ws                   =   gep.m_ws              ;
  const double curbInSmpReturn          =   gep.m_curbInSmpReturn ;

  //--------------------------------------------------
  if (v_hist_forsma.size() != v_hist_forpnl.size())
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB1: %s::%s (%d) TestRun: v_hist_forsma.size() != v_hist_forpnl.size().",__FILE__,__FUNCTION__,__LINE__);
    return false;
  }
  if (v_disable_days.size() != v_hist_forpnl.size())
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB1: %s::%s (%d) TestRun: v_disable_days.size() != v_hist_forpnl.size().",__FILE__,__FUNCTION__,__LINE__);
    return false;
  }
  if (v_hist_forsma.size() < trainingperiod)
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB1: %s::%s (%d) TestRun: v_hist_forsma.size() < trainingperiod.",__FILE__,__FUNCTION__,__LINE__);
    return false;
  }
  if (v_hist_forsma.size() < 2)
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB1: %s::%s (%d) TestRun: v_hist_forsma.size() < 2.",__FILE__,__FUNCTION__,__LINE__);
    return false;
  }

  //--------------------------------------------------
  v_signal_trail.clear();

  boost::accumulators::accumulator_set<double, boost::accumulators::features<boost::accumulators::tag::mean, boost::accumulators::tag::variance>, double> acc;
  double dPnL = 0;
  double dTotalWeight = 0;


  Sma<double> sma(ma_period,true);

  for (unsigned int i = 1; i < v_hist_forsma.size()-1; ++i)
  {
    sma.Add(v_hist_forsma[i]);

    double dWeight = 0;
    if (ws == WS_LINEAR)
    {
      dWeight = max((double)i-(double)v_hist_forsma.size()+(double)trainingperiod,(double)0);
      if (dWeight <= NIR_EPSILON) continue;
    }
    else if (ws == WS_UNIFORM)
    {
      if (i < (double)v_hist_forsma.size()-(double)trainingperiod) continue;
      dWeight = 1;
    }

    dTotalWeight += dWeight;

    if (!sma.Ready()) continue;
    double dMAValue = sma.Value();

    if (pdAvgPnL || pdSharpe)
    {
      double dSignalVarb = 0;

      if (v_hist_forsma[i] >= dMAValue) dSignalVarb =  1;
      else                              dSignalVarb = -1;

      v_signal_trail.push_back((int)dSignalVarb);

      if (!v_disable_days[i])
      {
        if (curbInSmpReturn>0)  dPnL = dSignalVarb * max(min((v_hist_forpnl[i+1] / v_hist_forpnl[i]) -1, curbInSmpReturn),-curbInSmpReturn);
        else                    dPnL = dSignalVarb * ((v_hist_forpnl[i+1] / v_hist_forpnl[i]) -1);
      }

      if (!std::isnan(dPnL)) acc(dPnL, boost::accumulators::weight = dWeight);
    }
  }

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

  return true;
}


bool StrategyB1::TrainUpParam(
  const YYYYMMDDHHMMSS & ymdhms,
  const double trainingperiod,
  const vector<double> & v_hist_undly,
  const vector<double> & v_hist_deriv,
  const TRAINMODE tm,
  const WEIGHTING_SCHEME ws,
  map<double,int> & mapBestParamSetUndlyDeriv1,
  map<double,int> & mapBestParamSetDerivDeriv1,
  map<double,int> & mapBestParamSetUndlyDeriv2,
  const double curbInSmpReturn
  )
{

  if (trainingperiod <= 0)
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB1: %s::%s (%d) TrainUpParam exits: Training periods cannot be <= 0.",__FILE__,__FUNCTION__,__LINE__);
    return false;
  }

  if (
    trainingperiod > v_hist_undly.size() ||
    trainingperiod > v_hist_deriv.size()
    )
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB1: %s::%s (%d) TrainUpParam exits: Not enough historical data for the training periods. trainingperiod = %f v_hist_undly.size() = %d v_hist_deriv.size() = %d.",
                    __FILE__,__FUNCTION__,__LINE__,
                    trainingperiod,
                    v_hist_undly.size(),
                    v_hist_deriv.size()
                   );
    return false;
  }


  {
    mapBestParamSetUndlyDeriv1.clear();
    mapBestParamSetDerivDeriv1.clear();
    mapBestParamSetUndlyDeriv2.clear();


    vector<bool> vdNoDisable;
    vdNoDisable.insert(vdNoDisable.begin(),v_hist_undly.size(),false);

    vector<int> viDefaultSignalTrail;
    viDefaultSignalTrail.insert(viDefaultSignalTrail.begin(),v_hist_undly.size(),0);

    for (unsigned int iOptmznStep = 0; iOptmznStep < 3; ++iOptmznStep)
    {

      if (iOptmznStep == 2 &&
          (
            mapBestParamSetUndlyDeriv1.empty() ||
            mapBestParamSetDerivDeriv1.empty()
          )
         )
        break;

      //--------------------------------------------------
      double dMAPeriodLowerBound = 0;
      double dMAPeriodUpperBound = 0;
      double dMAPeriodAdj        = 0;
      //--------------------------------------------------

      if (iOptmznStep == 0 || iOptmznStep == 1)
      {
        dMAPeriodLowerBound = m_ma_period_1_low;
        dMAPeriodUpperBound = m_ma_period_1_high;
        dMAPeriodAdj        = m_ma_period_1_adj;
      }
      else if (iOptmznStep == 2)
      {
        dMAPeriodLowerBound = m_ma_period_2_low;
        dMAPeriodUpperBound = m_ma_period_2_high;
        dMAPeriodAdj        = m_ma_period_2_adj;
      }
      //--------------------------------------------------


      //--------------------------------------------------
      vector<double>        vdPrelimAvgPnL;
      vector<double>        vdPrelimSharpe;
      vector<vector<int> >  vviSignalTrail;
      vector<bool>          vdDisableStep3;
      vector<TestRunParm>   vTRP;
      //--------------------------------------------------

      for (double maprd = dMAPeriodLowerBound; maprd <= dMAPeriodUpperBound; maprd += dMAPeriodAdj)
      {
        vdPrelimAvgPnL.push_back(NAN);
        vdPrelimSharpe.push_back(NAN);
        vviSignalTrail.push_back(viDefaultSignalTrail);
        vTRP.push_back(TestRunParm());
      }

      if (iOptmznStep == 2)
      {
        vdDisableStep3.insert(vdDisableStep3.begin(),v_hist_undly.size(),false);

        //--------------------------------------------------
        // construct the disable days list
        // just pick the best and worse parameter and assume to use them
        //--------------------------------------------------
        map<double,int>::iterator it;
        it = mapBestParamSetUndlyDeriv1.end(); it--;
        const int iBestUndlyDerivPrd = it->second;
        it = mapBestParamSetDerivDeriv1.begin();
        const int iWorstDerivDerivPrd = it->second;

        Sma<double> smaUndlyDeriv1(iBestUndlyDerivPrd,true);
        Sma<double> smaDerivDeriv1(iWorstDerivDerivPrd,true);

        for (unsigned int i = 0; i < v_hist_deriv.size(); ++i)
        {
          smaUndlyDeriv1.Add(v_hist_undly[i]);
          smaDerivDeriv1.Add(v_hist_deriv[i]);

          int iSmaUndlyDeriv1 = 0;
          int iSmaDerivDeriv1 = 0;
          if (smaUndlyDeriv1.Ready()) iSmaUndlyDeriv1 = (v_hist_undly[i] >= smaUndlyDeriv1.Value() ?  1 : -1);
          if (smaDerivDeriv1.Ready()) iSmaDerivDeriv1 = (v_hist_deriv[i] >= smaDerivDeriv1.Value() ?  1 : -1);

          int iTrueSignal = iSmaUndlyDeriv1 - iSmaDerivDeriv1;

          if (iTrueSignal > 0) vdDisableStep3[i] = true;
        }
        FForEach(vdDisableStep3,[&](const bool b){
          m_Logger->Write(Logger::DEBUG,"StrategyB1: vdDisableStep3 %s",(b ? "true":"false"));
        } );
      }

      //--------------------------------------------------

      boost::thread_group thdgrp;

      int iCnt = 0;
      for (double maprd = dMAPeriodLowerBound; maprd <= dMAPeriodUpperBound; maprd += dMAPeriodAdj)
      {
        TestRunParm & trp = vTRP[iCnt];

        trp.m_trainingperiod  = trainingperiod       ;
        trp.m_ma_period       = maprd                ;
        trp.m_pdAvgPnL        = &vdPrelimAvgPnL[iCnt];
        trp.m_pdSharpe        = &vdPrelimSharpe[iCnt];
        trp.m_v_signal_trail  = &vviSignalTrail[iCnt];
        trp.m_ws              = ws                   ;
        trp.m_curbInSmpReturn = curbInSmpReturn      ;

        if (iOptmznStep == 0)
        {
          trp.m_v_hist_forsma  = &v_hist_undly;
          trp.m_v_hist_forpnl  = &v_hist_deriv;
          trp.m_v_disable_days = &vdNoDisable ;
        }
        else if (iOptmznStep == 1)
        {
          trp.m_v_hist_forsma  = &v_hist_deriv;
          trp.m_v_hist_forpnl  = &v_hist_deriv;
          trp.m_v_disable_days = &vdNoDisable ;
        }
        else if (iOptmznStep == 2)
        {
          trp.m_v_hist_forsma  = &v_hist_undly  ;
          trp.m_v_hist_forpnl  = &v_hist_deriv  ;
          trp.m_v_disable_days = &vdDisableStep3;
        }

        iCnt++;

        thdgrp.add_thread(new boost::thread(boost::bind(&StrategyB1::TestRun, this, trp)));
      }

      thdgrp.join_all();

      //--------------------------------------------------
      iCnt = 0;
      for (double maprd = dMAPeriodLowerBound; maprd <= dMAPeriodUpperBound; maprd += dMAPeriodAdj)
      {
        if (!std::isnan(vdPrelimAvgPnL[iCnt]))
        {
          if (iOptmznStep == 0)
          {
            if      (tm == TM_MAXPROFIT) mapBestParamSetUndlyDeriv1[vdPrelimAvgPnL[iCnt]] = maprd;
            else if (tm == TM_MAXSHARPE) mapBestParamSetUndlyDeriv1[vdPrelimSharpe[iCnt]] = maprd;
          }
          else if (iOptmznStep == 1)
          {
            if      (tm == TM_MAXPROFIT) mapBestParamSetDerivDeriv1[vdPrelimAvgPnL[iCnt]] = maprd;
            else if (tm == TM_MAXSHARPE) mapBestParamSetDerivDeriv1[vdPrelimSharpe[iCnt]] = maprd;
          }
          else if (iOptmznStep == 2)
          {
            if      (tm == TM_MAXPROFIT) mapBestParamSetUndlyDeriv2[vdPrelimAvgPnL[iCnt]] = maprd;
            else if (tm == TM_MAXSHARPE) mapBestParamSetUndlyDeriv2[vdPrelimSharpe[iCnt]] = maprd;
          }
          m_Logger->Write(Logger::DEBUG,"StrategyB1: %s Looped parameters: Training period = %f Optimization step = %d MA period = %f, vdPrelimAvgPnL = %f. vdPrelimSharpe = %f.",
                          ymdhms.ToStr().c_str(),trainingperiod,iOptmznStep,maprd,vdPrelimAvgPnL[iCnt],vdPrelimSharpe[iCnt]);
        }

        iCnt++;
      }

    }


  }

  m_Logger->Write(Logger::INFO,"StrategyB1: %s Num of best parameters in mapBestParamSetUndlyDeriv1: %d",ymdhms.ToStr().c_str(),mapBestParamSetUndlyDeriv1.size());

  return true;
}

void StrategyB1::SetParamRange(
  const double ma_period_1_low,
  const double ma_period_1_high,
  const double ma_period_1_adj,
  const double ma_period_2_low,
  const double ma_period_2_high,
  const double ma_period_2_adj
  )
{
  m_ma_period_1_low  = ma_period_1_low;
  m_ma_period_1_high = ma_period_1_high;
  m_ma_period_1_adj  = ma_period_1_adj;
  m_ma_period_2_low  = ma_period_2_low;
  m_ma_period_2_high = ma_period_2_high;
  m_ma_period_2_adj  = ma_period_2_adj;
}

