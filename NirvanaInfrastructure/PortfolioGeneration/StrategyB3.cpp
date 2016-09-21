#include <StrategyB3.h>

StrategyB3::StrategyB3() :
  m_TheoCPnLHistFolder(""),
  m_TheoPosFolder(""),
  m_NumOfAvbGrp(0),
  m_B3_HasEnabledMinCommissionCheck(true),
  m_B3_WhetherToRetain(false),
  m_B3_ActionTimeBefCloseInSec(180),
  m_B3_ActionTimeAftOpenInSec(180),
  StrategyBase()
{
  UnsetConvenienceVarb();
  m_iNumOfParam = 5;
}

StrategyB3::~StrategyB3()
{
}


bool StrategyB3::TrainUpParam(
  const int iTradSym,
  const YYYYMMDDHHMMSS & ymdhms,
  const double trainingperiodmin,
  const double trainingperiodmax,
  const double trainingperiodstepsize,
  const vector<double> & v_hist_trfpx,
  const vector<double> & v_hist_close,
  const vector<double> & v_hist_openavg,
  const vector<double> & v_hist_avgpxrtn,
  const vector<vector<double> > & m_fall_then_long_cpnl
  )
{
  const string symbol = m_TradedSymbols[iTradSym];

  if (
    trainingperiodmin == 0 ||
    trainingperiodmax == 0 ||
    trainingperiodstepsize == 0
    )
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB3: %s::%s (%d) %s TrainUpParam exits: Training periods are 0.",__FILE__,__FUNCTION__,__LINE__,symbol.c_str());
    return true;
  }

  if (trainingperiodmin > v_hist_trfpx.size())
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB1: %s::%s (%d) %s TrainUpParam exits: Not enough historical data for the training periods. v_hist_trfpx.size() %d",
                    __FILE__,__FUNCTION__,__LINE__, symbol.c_str(), v_hist_trfpx.size());
    return false;
  }

  if (
    trainingperiodmin      <= 0 ||
    trainingperiodmax      <= 0 ||
    trainingperiodstepsize <= 0
    )
  {
    m_Logger->Write(Logger::DEBUG,"StrategyB3: %s::%s (%d) %s TrainUpParam exits: Not all training periods are > 0.",__FILE__,__FUNCTION__,__LINE__, symbol.c_str());
    return false;
  }

  //--------------------------------------------------
  m_BestParam[symbol].clear();
  //--------------------------------------------------

  // vector<Tuple2<double,int> > m_pnl_period;
  // for (int i = 0; i < B3_MAX_N; ++i)
  // {
  //   const vector<double> & v_fall_then_long_cpnl = m_fall_then_long_cpnl[i];
  //   if (v_fall_then_long_cpnl.size() < trainingperiodmin) continue;
  //   const double pnl = (v_fall_then_long_cpnl.back() - v_fall_then_long_cpnl.rbegin()[trainingperiodmin]);
  //   m_pnl_period.push_back(Tuple2<double,int>(pnl,i));
  // }
  //
  // FForEach(m_pnl_period,[&](const Tuple2<double,int> t) {
  //   m_Logger->Write(Logger::INFO,"StrategyB3: %s m_pnl_period %f %d",
  //                   m_p_ymdhms_SysTime_Local->ToStr().c_str(),
  //                   t._1(), t._2());
  // });
  //
  // std::sort(m_pnl_period.begin(),m_pnl_period.end());
  // FReverse(m_pnl_period);
  //
  // int iCnt = 0;
  //
  // for (int i = 0; i < m_pnl_period.size(); ++i)
  // {
  //   m_BestParam[symbol].push_back(m_pnl_period[i]._2());
  //   iCnt++;
  //   if (iCnt > B3_MAX_N) break;
  // }
  //
  // FForEach(m_BestParam[symbol],[&](const int t) {
  //   m_Logger->Write(Logger::INFO,"StrategyB3: %s m_BestParam[%s] %d",
  //                   m_p_ymdhms_SysTime_Local->ToStr().c_str(),
  //                   symbol.c_str(), t);
  // });

  vector<Tuple3<double,int,int> > m_pnl_period;
  for (int p = trainingperiodmin; p < trainingperiodmax; p+=trainingperiodstepsize)
  {
    for (int i = 0; i < B3_MAX_N; ++i)
    {
      const vector<double> & v_fall_then_long_cpnl = m_fall_then_long_cpnl[i];
      if (v_fall_then_long_cpnl.size() < p) continue;
      const double cpnl = (v_fall_then_long_cpnl.back() - v_fall_then_long_cpnl.rbegin()[p]);
      m_pnl_period.push_back(Tuple3<double,int,int>(cpnl,i,p));
    }
  }

  FForEach(m_pnl_period,[&](const Tuple3<double,int,int> t) {
    m_Logger->Write(Logger::INFO,"StrategyB3: %s m_pnl_period %s",
                    m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                    t.ToString().c_str());
  });

  std::sort(m_pnl_period.begin(),m_pnl_period.end());
  FReverse(m_pnl_period);

  m_BestParam[symbol].insert(m_BestParam[symbol].begin(),m_pnl_period.begin(),m_pnl_period.end());

  FForEach(m_BestParam[symbol],[&](const Tuple3<double,int,int> t) {
    m_Logger->Write(Logger::INFO,"StrategyB3: %s m_BestParam[%s] %s",
                    m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                    symbol.c_str(), t.ToString().c_str());
  });

  // vector<Tuple4<double,int,int,int> > m_pnl_bound_weight_pve;
  // vector<Tuple4<double,int,int,int> > m_pnl_bound_weight_nve;
  //
  // for (int maxn = 2; maxn < B3_MAX_N; ++maxn)
  // {
  //   for (int bound = 2; bound < maxn; ++bound)
  //   {
  //     double totpnl = 0;
  //
  //     for (int i = 1; i < maxn; ++i)
  //     {
  //       const vector<double> & v_fall_then_long_cpnl = m_fall_then_long_cpnl[i];
  //       if (v_fall_then_long_cpnl.size() < trainingperiodmin) continue;
  //       const double pnl = (v_fall_then_long_cpnl.back() - v_fall_then_long_cpnl.rbegin()[trainingperiodmin]);
  //       if (i <= bound) totpnl += pnl / (float)bound;
  //       else totpnl -= pnl / ((float)maxn-(float)bound);
  //     }
  //
  //     m_pnl_bound_weight_pve.push_back(Tuple4<double,int,int,int>(totpnl,bound,maxn,1));
  //     // m_pnl_bound_weight_nve.push_back(Tuple4<double,int,int,int>(-totpnl,bound,maxn,-1));
  //   }
  // }
  //
  // FForEach(m_pnl_bound_weight_pve,[&](const Tuple4<double,int,int,int> t) {
  //   m_Logger->Write(Logger::INFO,"StrategyB3: %s m_pnl_bound_weight_pve %f %d %f",
  //                   m_p_ymdhms_SysTime_Local->ToStr().c_str(),
  //                   t._1(), t._2(), t._3());
  // });
  //
  // FForEach(m_pnl_bound_weight_nve,[&](const Tuple4<double,int,int,int> t) {
  //   m_Logger->Write(Logger::INFO,"StrategyB3: %s m_pnl_bound_weight_nve %f %d %f",
  //                   m_p_ymdhms_SysTime_Local->ToStr().c_str(),
  //                   t._1(), t._2(), t._3());
  // });
  //
  // std::sort(m_pnl_bound_weight_pve.begin(),m_pnl_bound_weight_pve.end());
  // FReverse(m_pnl_bound_weight_pve);
  //
  // std::sort(m_pnl_bound_weight_nve.begin(),m_pnl_bound_weight_nve.end());
  // FReverse(m_pnl_bound_weight_nve);
  //
  // //--------------------------------------------------
  // // pick one of pve and nve
  // //--------------------------------------------------
  // double dTotPnLpve = 0;
  // double dTotPnLnve = 0;
  // FForEach(m_pnl_bound_weight_pve,[&](const Tuple4<double,int,int,int> t) { dTotPnLpve += t._1(); });
  // FForEach(m_pnl_bound_weight_nve,[&](const Tuple4<double,int,int,int> t) { dTotPnLnve += t._1(); });
  //
  // // if (dTotPnLpve >= dTotPnLnve)
  //   m_BestParam[symbol].insert(m_BestParam[symbol].begin(), m_pnl_bound_weight_pve.begin(), m_pnl_bound_weight_pve.begin()+B3_CHOOSE_N);
  // // else
  // //   m_BestParam[symbol].insert(m_BestParam[symbol].begin(), m_pnl_bound_weight_nve.begin(), m_pnl_bound_weight_nve.begin()+B3_CHOOSE_N);
  //
  // FForEach(m_BestParam[symbol],[&](const Tuple4<double,int,int,int> t) {
  //   m_Logger->Write(Logger::INFO,"StrategyB3: %s m_BestParam[%s] %s",
  //                   m_p_ymdhms_SysTime_Local->ToStr().c_str(),
  //                   symbol.c_str(), t.ToString().c_str());
  // });

  return true;
}


void StrategyB3::StrategySetup()
{
}

void StrategyB3::ReadParam()
{
  //--------------------------------------------------
  // Parameters
  //--------------------------------------------------
  m_TheoCPnLHistFolder = m_SysCfg->B3_TheoCPnLHistFolder(m_StyID);
  m_TheoPosFolder      = m_SysCfg->B3_TheoPosFolder     (m_StyID);
  m_PersistTheoPosCPnL = m_SysCfg->B3_PersistTheoPosCPnL(m_StyID);

  //--------------------------------------------------

  m_LongOnlyWhenClosePriceBelowAvgPrice = m_SysCfg->B3_LongOnlyWhenClosePriceBelowAvgPrice(m_StyID);
  if (m_LongOnlyWhenClosePriceBelowAvgPrice)
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenClosePriceBelowAvgPrice %f", GetStrategyName(m_StyID).c_str(), m_LongOnlyWhenClosePriceBelowAvgPrice.get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenClosePriceBelowAvgPrice: Nil", GetStrategyName(m_StyID).c_str());

  m_ShortOnlyWhenClosePriceAboveAvgPrice = m_SysCfg->B3_ShortOnlyWhenClosePriceAboveAvgPrice(m_StyID);
  if (m_ShortOnlyWhenClosePriceAboveAvgPrice)
    m_Logger->Write(Logger::INFO,"Strategy %s: m_ShortOnlyWhenClosePriceAboveAvgPrice %f", GetStrategyName(m_StyID).c_str(), m_ShortOnlyWhenClosePriceAboveAvgPrice.get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_ShortOnlyWhenClosePriceAboveAvgPrice: Nil", GetStrategyName(m_StyID).c_str());

  m_LongOnlyWhenAvgPriceReturnAbove = m_SysCfg->B3_LongOnlyWhenAvgPriceReturnAbove(m_StyID);
  if (m_LongOnlyWhenAvgPriceReturnAbove)
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenAvgPriceReturnAbove %f", GetStrategyName(m_StyID).c_str(), m_LongOnlyWhenAvgPriceReturnAbove.get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenAvgPriceReturnAbove: Nil", GetStrategyName(m_StyID).c_str());

  m_LongOnlyWhenGrpAvgReturnAbove = m_SysCfg->B3_LongOnlyWhenGrpAvgReturnAbove(m_StyID);
  if (m_LongOnlyWhenGrpAvgReturnAbove)
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenGrpAvgReturnAbove %f", GetStrategyName(m_StyID).c_str(), m_LongOnlyWhenGrpAvgReturnAbove.get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_LongOnlyWhenGrpAvgReturnAbove: Nil", GetStrategyName(m_StyID).c_str());

  m_ShortOnlyWhenAvgPriceReturnBelow = m_SysCfg->B3_ShortOnlyWhenAvgPriceReturnBelow(m_StyID);
  if (m_ShortOnlyWhenAvgPriceReturnBelow)
    m_Logger->Write(Logger::INFO,"Strategy %s: m_ShortOnlyWhenAvgPriceReturnBelow %f", GetStrategyName(m_StyID).c_str(), m_ShortOnlyWhenAvgPriceReturnBelow.get());
  else
    m_Logger->Write(Logger::INFO,"Strategy %s: m_ShortOnlyWhenAvgPriceReturnBelow: Nil", GetStrategyName(m_StyID).c_str());

  m_Logger->Write(Logger::INFO,"Strategy %s: B3_CommissionRateThreshold %f", GetStrategyName(m_StyID).c_str(), m_SysCfg->B3_CommissionRateThreshold(m_StyID));

  m_B3_HasEnabledMinCommissionCheck = m_SysCfg->B3_HasEnabledMinCommissionCheck(m_StyID);
  m_B3_WhetherToRetain              = m_SysCfg->GetWhetherToRetrain(m_StyID);
  m_B3_TrainingFreq                 = m_SysCfg->GetTrainingFreq(m_StyID);
  m_B3_ActionTimeBefCloseInSec      = m_SysCfg->Get_B3_ActionTimeBefCloseInSec(m_StyID);
  m_B3_ActionTimeAftOpenInSec       = m_SysCfg->Get_B3_ActionTimeAftOpenInSec(m_StyID);

  m_Logger->Write(Logger::INFO,"Strategy %s: m_B3_HasEnabledMinCommissionCheck %s", GetStrategyName(m_StyID).c_str(), (m_B3_HasEnabledMinCommissionCheck?"true":"false"));
  m_Logger->Write(Logger::INFO,"Strategy %s: m_B3_WhetherToRetain              %s", GetStrategyName(m_StyID).c_str(), (m_B3_WhetherToRetain             ?"true":"false"));
  m_Logger->Write(Logger::INFO,"Strategy %s: m_B3_TrainingFreq                 %d", GetStrategyName(m_StyID).c_str(),  m_B3_TrainingFreq                                );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_B3_ActionTimeBefCloseInSec      %d", GetStrategyName(m_StyID).c_str(),  m_B3_ActionTimeBefCloseInSec                     );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_B3_ActionTimeAftOpenInSec       %d", GetStrategyName(m_StyID).c_str(),  m_B3_ActionTimeAftOpenInSec                      );

  m_TradedSymbolsTradeAtOpen = m_SysCfg->Get_B3_TradedSymTradeAtOpen(m_StyID);
  FForEach(m_TradedSymbolsTradeAtOpen,[&](const string & sym) { m_Logger->Write(Logger::INFO,"Strategy %s: m_TradedSymbolsTradeAtOpen %s", GetStrategyName(m_StyID).c_str(), sym.c_str()); });

  //--------------------------------------------------
  m_TrainingPeriodMin      = m_ParamVector[0];
  m_TrainingPeriodMax      = m_ParamVector[1];
  m_TrainingPeriodStepSize = m_ParamVector[2];
  m_NotionalAmt            = m_ParamVector[3];
  m_MaxNotionalAmtEach     = m_ParamVector[4];

  m_Logger->Write(Logger::INFO,"Strategy %s: m_TrainingPeriodMin      %f", GetStrategyName(m_StyID).c_str(), m_TrainingPeriodMin     );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_TrainingPeriodMax      %f", GetStrategyName(m_StyID).c_str(), m_TrainingPeriodMax     );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_TrainingPeriodStepSize %f", GetStrategyName(m_StyID).c_str(), m_TrainingPeriodStepSize);
  m_Logger->Write(Logger::INFO,"Strategy %s: m_NotionalAmt            %f", GetStrategyName(m_StyID).c_str(), m_NotionalAmt       );
  m_Logger->Write(Logger::INFO,"Strategy %s: m_MaxNotionalAmtEach     %f", GetStrategyName(m_StyID).c_str(), m_MaxNotionalAmtEach);

  //--------------------------------------------------

  FForEach (m_TradedSymbols,[&](const string & sym) {
    m_PTask_PrintStyActionTimeToken[sym] = m_PTask_PrintStyActionTime.GetNewTokenAndSetIntervalInSec(60);
  });
  m_PTask_PrintAllAvbSymToken = m_PTask_PrintAllAvbSym.GetNewTokenAndSetIntervalInSec(300);

  m_PTask_PrintTrainingResultToken = m_PTask_PrintTrainingResult.GetNewTokenAndSetIntervalInSec(60);

}

void StrategyB3::StartOfDayInit()
{

  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    m_map_DoneSODTrade[m_TradedSymbols[i]] = false;
    m_map_DoneEODTrade[m_TradedSymbols[i]] = false;
  }

  //--------------------------------------------------
  // HKT entered in config file
  //--------------------------------------------------
  if (m_B3_ActionTimeAftOpenInSec > 0)
  {
    //--------------------------------------------------
    // SOD trading enabled
    //--------------------------------------------------
    m_hms_OpenStart.Set(m_Exchg->GetTimeNSecAftOpen (m_TradedSymbols[0],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),0));
    m_hms_OpenEnd.Set(m_Exchg->GetTimeNSecAftOpen (m_TradedSymbols[0],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),m_B3_ActionTimeAftOpenInSec));
    if (m_hms_OpenStart > m_hms_OpenEnd) m_hms_OpenStart.Set(0,0,0);
  }
  else
  {
    //--------------------------------------------------
    // SOD trading disabled
    //--------------------------------------------------
    m_hms_OpenStart.Set(0,0,0);
    m_hms_OpenEnd.Set(0,0,0);
  }
  m_hms_CloseStart.Set(m_Exchg->GetTimeNSecBefClose(m_TradedSymbols[0],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),m_B3_ActionTimeBefCloseInSec));
  m_hms_CloseEnd.Set(m_Exchg->GetTimeNSecBefClose(m_TradedSymbols[0],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),0));

}

void StrategyB3::EndOfDayCleanUp()
{
  StartOfDayInit();
}

void StrategyB3::SetConvenienceVarb(const int iTradSym)
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

    map<string,vector<vector<double> >  >::iterator it4 = m_map_HistoricalFallThenLongCPnL.find(m_TradedSymbols[iTradSym]);
    if (it4 == m_map_HistoricalFallThenLongCPnL.end())
    {
      m_map_HistoricalFallThenLongCPnL[m_TradedSymbols[iTradSym]] = vector<vector<double> >();

      for (int i = 0; i < B3_MAX_N; ++i)
        m_map_HistoricalFallThenLongCPnL[m_TradedSymbols[iTradSym]].push_back(vector<double>());

      it4 = m_map_HistoricalFallThenLongCPnL.find(m_TradedSymbols[iTradSym]);
    }
    m_HistoricalFallThenLongCPnL = &(it4->second);
    //--------------------------------------------------
  }

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

}


bool StrategyB3::SkipSubseqProcessingForSymbol(const int iTradSym, string & sComment)
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



void StrategyB3::InitialWarmUp(const int iTradSym)
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
      m_HistoricalAvgPx->push_back(dAvgPx);
      m_HistoricalClose->push_back(vClose[i]);
      m_HistoricalOpenAvg->push_back(vOpen[i]);
      m_HistoricalOpenAvg->push_back(dAvgPx);

      //--------------------------------------------------

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
      for (int n = 1; n < B3_MAX_N; ++n)
      {
        // if (m_HistoricalAvgPx->size() > n)
        if (m_HistoricalClose->size() > n)
        {
          if (m_HistoricalClose->rbegin()[1] < m_HistoricalClose->rbegin()[n+1])
            (*m_HistoricalFallThenLongCPnL)[n].push_back(m_HistoricalCloseRtn->back());
          else
            (*m_HistoricalFallThenLongCPnL)[n].push_back(0.0);
        }
      }

    }
  }

  if (m_B3_TrainingFreq == TradingStrategyConfig::OnceAtInitWarmup)
  {
    DoTraining(iTradSym);
  }

}

void StrategyB3::UpdateInternalDataTrng(const int iTradSym)
{
  return;
}

void StrategyB3::UpdateInternalData(const int iTradSym)
{

  if (
    //--------------------------------------------------
    // has to be done whether we are trading at open
    //--------------------------------------------------
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

      m_HistoricalAvgPx->push_back(dAvgPx);
      m_HistoricalClose->push_back(m_SymMidQuote);
      m_HistoricalOpenAvg->push_back(dAvgPx);

      //--------------------------------------------------
      if (m_HistoricalAvgPx->size()>1)
      {
        double d = m_HistoricalAvgPx->back() / (m_HistoricalAvgPx->rbegin()[1]);
        m_HistoricalAvgPxRtn->push_back(d);
      }
      if (m_HistoricalClose->size()>1)
      {
        double d = m_HistoricalClose->back() / (m_HistoricalClose->rbegin()[1]);
        m_HistoricalCloseRtn->push_back(d);
      }
      for (int n = 1; n < B3_MAX_N; ++n)
      {
        if (m_HistoricalAvgPx->size() > n)
        {
          if (m_HistoricalClose->rbegin()[1] < m_HistoricalClose->rbegin()[n+1])
            (*m_HistoricalFallThenLongCPnL)[n].push_back(m_HistoricalCloseRtn->back());
          else
            (*m_HistoricalFallThenLongCPnL)[n].push_back(0.0);
        }
      }

    }
  }

  m_dGKYZVal = TechIndicators::Instance()->GetGKYZValue(m_TradedSymbols[iTradSym]);
}

void StrategyB3::DoTraining(const int iTradSym)
{

  *m_bTrainRetAvgPx = TrainUpParam(
    iTradSym,
    *m_p_ymdhms_SysTime_Local,
    m_TrainingPeriodMin,
    m_TrainingPeriodMax,
    m_TrainingPeriodStepSize,
    (*m_HistoricalAvgPx),
    (*m_HistoricalClose),
    (*m_HistoricalOpenAvg),
    (*m_HistoricalAvgPxRtn),
    (*m_HistoricalFallThenLongCPnL)
    );

  m_map_ymdhms_LastTrainTime[0][m_TradedSymbols[iTradSym]] = *m_p_ymdhms_SysTime_Local;

  m_Logger->Write(Logger::INFO,"Strategy %s: %s %s (%d) Sym=%s TrainUpParam() returned %s",
                  GetStrategyName(m_StyID).c_str(),
                  m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                  __FUNCTION__,__LINE__,
                  m_TradedSymbols[iTradSym].c_str(),(*m_bTrainRetAvgPx?"True":"False")
                 );

}


void StrategyB3::PerformTrainingJustBeforeTrading(const int iTradSym)
{
  //--------------------------------------------------
  // if don't want to miss the last data point (which is today) in our training, should do the training here.
  // if want to leave enough time for many symbols to train, train after initial warmup
  //--------------------------------------------------
  if (m_B3_TrainingFreq == TradingStrategyConfig::OnceAtInitWarmup)
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: %s %s (%d) Sym=%s m_B3_TrainingFreq = OnceAtInitWarmup",
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
    m_B3_TrainingFreq == TradingStrategyConfig::Daily
    ||
    (
      !m_map_ymdhms_LastTrainTime[0][m_TradedSymbols[iTradSym]].IsValid()
      ||
      ((*m_bTrainRetAvgPx) && m_p_ymdhms_SysTime_Local->GetYYYYMMDD() - m_map_ymdhms_LastTrainTime[0][m_TradedSymbols[iTradSym]].GetYYYYMMDD() > 5)
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


void StrategyB3::EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}


void StrategyB3::DetermineRegime(const int iTradSym)
{
}


void StrategyB3::PreTradePreparation(const int iTradSym)
{

  // for (int i = 0; i < (*m_HistoricalFallThenLongCPnL)[1].size(); ++i)
  // {
  //   m_Logger->Write(Logger::DEBUG,"Strategy %s: %s %s (%d) m_HistoricalAvgPxRtn[%d] size %d %f m_HistoricalCloseRtn[%d] size %d %f m_HistoricalFallThenLongCPnL[%d] size %d %f m_HistoricalFallThenLongCPnL size %d",
  //                   GetStrategyName(m_StyID).c_str(),
  //                   m_p_ymdhms_SysTime_Local->ToStr().c_str(),
  //                   __FUNCTION__,__LINE__,
  //                   i, m_HistoricalAvgPxRtn->size(),(*m_HistoricalAvgPxRtn)[i],
  //                   i, m_HistoricalCloseRtn->size(),(*m_HistoricalCloseRtn)[i],
  //                   i, (*m_HistoricalFallThenLongCPnL)[1].size(),(*m_HistoricalFallThenLongCPnL)[1][i],
  //                   (*m_HistoricalFallThenLongCPnL)[50].size()
  //                  );
  // }

  //--------------------------------------------------
  // Day End logic
  //--------------------------------------------------
  if (
    !*m_DoneEODTrade &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() >= m_hms_CloseStart &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() <= m_hms_CloseEnd
    )
  {

    //--------------------------------------------------
    // Get our predictor - Trade at Close
    //--------------------------------------------------

    // const vector<int> & viBestParam = m_BestParam[m_TradedSymbols[iTradSym]];
    // const int iTotWeight = viBestParam.size() * (viBestParam.size()+1) / 2;
    //
    // double dTotStren = 0;
    // for (int i = 0; i < viBestParam.size(); ++i)
    // {
    //   const int iWeight = viBestParam.size() - i; 
    //   const int sign = (viBestParam[i] >= 0 ? 1 : -1);
    //   const int signal = (m_HistoricalAvgPx->back() < m_HistoricalAvgPx->rbegin()[abs(viBestParam[i])] ? 1 : 0);
    //   dTotStren += (double)iWeight / iTotWeight * sign * signal;
    // }

    const vector<Tuple3<double,int,int> > & viBestParam = m_BestParam[m_TradedSymbols[iTradSym]];
    const int iTotWeight = viBestParam.size() * (viBestParam.size()+1) / 2;

    double dTotStren = 0;
    for (int i = 0; i < viBestParam.size(); ++i)
    {
      const int iWeight = viBestParam.size() - i; 
      const int sign = (viBestParam[i]._2() >= 0 ? 1 : -1);
      const int signal = (m_HistoricalAvgPx->back() < m_HistoricalAvgPx->rbegin()[abs(viBestParam[i]._2())] ? 1 : 0);
      dTotStren += (double)iWeight / iTotWeight * sign * signal;
    }

    // const vector<Tuple4<double,int,int,int> > & vdBestParam = m_BestParam[m_TradedSymbols[iTradSym]];
    // double dTotStren = 0;
    // for (int n = 0; n < B3_CHOOSE_N; ++n)
    // {
    //   const int maxn = vdBestParam[n]._3();
    //   const int iBound = (int)vdBestParam[n]._2();
    //   const int direction = (vdBestParam[n]._4() >= 0 ? 1 : -1);
    //   if (iBound == 0 || iBound == maxn) continue;
    //   for (int i = 0; i < maxn; ++i)
    //   {
    //     const int signal = (m_HistoricalAvgPx->back() < m_HistoricalAvgPx->rbegin()[i] ? 1 : 0) * direction;
    //     if (i <= iBound) dTotStren += (double)signal / (double)iBound;
    //     else dTotStren -= (double)signal / ((double)maxn-(double)iBound);
    //   }
    // }
    // dTotStren = dTotStren / B3_CHOOSE_N;

    m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s dTotStren = %f",
                    GetStrategyName(m_StyID).c_str(),
                    m_p_ymdhms_SysTime_Local->ToStr().c_str(),
                    m_TradedSymbols[iTradSym].c_str(),
                    dTotStren
                   );

    //--------------------------------------------------
    // Calculate actual position size
    //--------------------------------------------------
    m_dAggUnsignedQty = min(m_NotionalAmt, m_MaxNotionalAmtEach) / m_SymMidQuote;
    m_dAggSignedQty = round(m_dAggUnsignedQty * dTotStren);

    //--------------------------------------------------
    // Close price relative to average price
    //--------------------------------------------------
    if (m_LongOnlyWhenClosePriceBelowAvgPrice
        &&
        STool::IsValidPriceOrVol(m_SymMidQuote) && !m_HistoricalAvgPx->empty())
    {
      double dClosePxAgstAvgPx = m_SymMidQuote / m_HistoricalAvgPx->back() - 1;
      if (dClosePxAgstAvgPx > m_LongOnlyWhenClosePriceBelowAvgPrice.get())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to min(%f, 0). dClosePxAgstAvgPx %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, dClosePxAgstAvgPx);
        m_dAggSignedQty = min(m_dAggSignedQty,0.0);
      }
    }
    if (m_ShortOnlyWhenClosePriceAboveAvgPrice
        &&
        STool::IsValidPriceOrVol(m_SymMidQuote) && !m_HistoricalAvgPx->empty())
    {
      double dClosePxAgstAvgPx = m_SymMidQuote / m_HistoricalAvgPx->back() - 1;
      if (dClosePxAgstAvgPx < m_ShortOnlyWhenClosePriceAboveAvgPrice.get())
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
    if (m_LongOnlyWhenAvgPriceReturnAbove
        &&
        m_HistoricalAvgPx->size() >= 2)
    {
      double dAvgPxRtn = m_HistoricalAvgPx->back() / (m_HistoricalAvgPx->rbegin()[1]) - 1;
      if (dAvgPxRtn < m_LongOnlyWhenAvgPriceReturnAbove.get())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to min(%f, 0). dAvgPxRtn %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, dAvgPxRtn);
        m_dAggSignedQty = min(m_dAggSignedQty,0.0);
      }
    }
    if (m_ShortOnlyWhenAvgPriceReturnBelow
        &&
        m_HistoricalAvgPx->size() >= 2)
    {
      double dAvgPxRtn = m_HistoricalAvgPx->back() / (m_HistoricalAvgPx->rbegin()[1]) - 1;
      if (dAvgPxRtn > m_ShortOnlyWhenAvgPriceReturnBelow.get())
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Sym=%s m_dAggSignedQty is set to max(%f, 0). dAvgPxRtn %f",
                        GetStrategyName(m_StyID).c_str(), m_p_ymdhms_SysTime_Local->ToStr().c_str(), m_TradedSymbols[iTradSym].c_str(),
                        m_dAggSignedQty, dAvgPxRtn);
        m_dAggSignedQty = max(m_dAggSignedQty,0.0);
      }
    }

    //--------------------------------------------------
    // Round down to lot size if HK stocks
    //--------------------------------------------------
    if (m_StyID == STY_B3_US && GetStyDomicileMkt() == SDM_HK)
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
    if (m_B3_HasEnabledMinCommissionCheck)
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
          m_dAggSignedQty) / (m_SymMidQuote * abs(m_dAggSignedQty)) > m_SysCfg->B3_CommissionRateThreshold(m_StyID)
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
      !m_B3_HasEnabledMinCommissionCheck
      ||
      //--------------------------------------------------
      // if position is to be set to zero, do it! ignore commission impact
      //--------------------------------------------------
      abs(m_dAggSignedQty) < NIR_EPSILON
      ||
      (
        m_B3_HasEnabledMinCommissionCheck
        &&
        CommissionCalculator::CalcCommission(
          CommissionCalculator::IB,
          CommissionCalculator::USSTK,
          CommissionCalculator::SPOT,
          m_SymMidQuote,
          dAbsDeltaQty) / (m_SymMidQuote * dAbsDeltaQty) <= m_SysCfg->B3_CommissionRateThreshold(m_StyID)
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

void StrategyB3::CalcPosSize(const int iTradSym)
{
}

void StrategyB3::UnsetConvenienceVarb()
{
  m_HistoricalAvgPx  = NULL;
  m_HistoricalClose  = NULL;
  m_DoneSODTrade     = NULL;
  m_DoneEODTrade     = NULL;
  m_bTrainRetAvgPx   = NULL;
  m_bTrainRetClose   = NULL;

  return;
}

void StrategyB3::ParamSanityCheck()
{
  if (m_ParamVector.size() != m_iNumOfParam)
  {
    m_Logger->Write(Logger::ERROR,"Strategy %s: Failed sanity test", GetStrategyName(m_StyID).c_str());
    sleep(2);
    exit(1);
  }
  return;
}

void StrategyB3::LoadOptimalParamsFromTrainingResults()
{
  return;
}
