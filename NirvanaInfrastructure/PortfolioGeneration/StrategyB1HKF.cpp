#include <StrategyB1HKF.h>

StrategyB1_HKF::StrategyB1_HKF() :
  m_B1_WhetherToRetain(false),
  StrategyB1()
{
  SetStrategyID(STY_B1_HKF);
  SetStyDomicileMkt(SDM_HK);
  UnsetConvenienceVarb();
  SetCloseAllPosIfOutsideTrdgHour(ACP_NO);
  m_iNumOfParam = 11;
}

StrategyB1_HKF::~StrategyB1_HKF()
{
}


void StrategyB1_HKF::ReadParam()
{
  //--------------------------------------------------
  // Parameters
  //--------------------------------------------------
  m_B1_WhetherToRetain = m_SysCfg->GetWhetherToRetrain(m_StyID);
  m_B1_TrainingFreq    = m_SysCfg->GetTrainingFreq(m_StyID);

  m_Logger->Write(Logger::INFO,"Strategy %s: m_B1_WhetherToRetain %s", GetStrategyName(m_StyID).c_str(), (m_B1_WhetherToRetain  ?"true":"false"));
  m_Logger->Write(Logger::INFO,"Strategy %s: m_B1_TrainingFreq    %d", GetStrategyName(m_StyID).c_str(),  m_B1_TrainingFreq                     );


  m_NotionalAmt           .insert(m_NotionalAmt           .begin(), m_TradedSymbols.size(),0);
  m_CurbInSmpRtn          .insert(m_CurbInSmpRtn          .begin(), m_TradedSymbols.size(),0);
  m_TrainingPeriod        .insert(m_TrainingPeriod        .begin(), m_TradedSymbols.size(),0);
  m_MAPeriod_1_start      .insert(m_MAPeriod_1_start      .begin(), m_TradedSymbols.size(),0);
  m_MAPeriod_1_end        .insert(m_MAPeriod_1_end        .begin(), m_TradedSymbols.size(),0);
  m_MAPeriod_1_increment  .insert(m_MAPeriod_1_increment  .begin(), m_TradedSymbols.size(),0);
  m_MAPeriod_2_start      .insert(m_MAPeriod_2_start      .begin(), m_TradedSymbols.size(),0);
  m_MAPeriod_2_end        .insert(m_MAPeriod_2_end        .begin(), m_TradedSymbols.size(),0);
  m_MAPeriod_2_increment  .insert(m_MAPeriod_2_increment  .begin(), m_TradedSymbols.size(),0);
  m_PropOfBestParam       .insert(m_PropOfBestParam       .begin(), m_TradedSymbols.size(),0);
  m_WeightScheme          .insert(m_WeightScheme          .begin(), m_TradedSymbols.size(),WS_UNIFORM);


  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    m_NotionalAmt           [i] =  m_ParamVector[i*m_iNumOfParam  +   0]                               ;
    m_CurbInSmpRtn          [i] =  m_ParamVector[i*m_iNumOfParam  +   1]                               ;
    m_TrainingPeriod        [i] =  m_ParamVector[i*m_iNumOfParam  +   2]                               ;
    m_MAPeriod_1_start      [i] =  m_ParamVector[i*m_iNumOfParam  +   3]                               ;
    m_MAPeriod_1_end        [i] =  m_ParamVector[i*m_iNumOfParam  +   4]                               ;
    m_MAPeriod_1_increment  [i] =  m_ParamVector[i*m_iNumOfParam  +   5]                               ;
    m_MAPeriod_2_start      [i] =  m_ParamVector[i*m_iNumOfParam  +   6]                               ;
    m_MAPeriod_2_end        [i] =  m_ParamVector[i*m_iNumOfParam  +   7]                               ;
    m_MAPeriod_2_increment  [i] =  m_ParamVector[i*m_iNumOfParam  +   8]                               ;
    m_PropOfBestParam       [i] =  m_ParamVector[i*m_iNumOfParam  +   9]                               ;
    m_WeightScheme          [i] = (m_ParamVector[i*m_iNumOfParam  +  10] == 1 ? WS_UNIFORM : WS_LINEAR);


    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_NotionalAmt           %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_NotionalAmt         [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_CurbInSmpRtn          %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_CurbInSmpRtn        [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_TrainingPeriod        %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_TrainingPeriod      [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_MAPeriod_1_start      %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_MAPeriod_1_start    [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_MAPeriod_1_end        %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_MAPeriod_1_end      [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_MAPeriod_1_increment  %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_MAPeriod_1_increment[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_MAPeriod_2_start      %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_MAPeriod_2_start    [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_MAPeriod_2_end        %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_MAPeriod_2_end      [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_MAPeriod_2_increment  %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_MAPeriod_2_increment[i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_PropOfBestParam       %f", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_PropOfBestParam     [i]);
    m_Logger->Write(Logger::INFO,"Strategy %s: %s m_WeightScheme          %d", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[i].c_str(), m_WeightScheme        [i]);

  }

  //--------------------------------------------------
  for (unsigned int iTradSym = 0; iTradSym < m_TradedSymbols.size(); ++iTradSym)
  {
    for (unsigned int iItfOrUndly = 0; iItfOrUndly < 2; ++iItfOrUndly)
    {
      string sSymbol;

      if      (iItfOrUndly == 0) sSymbol = m_TradedSymbols[iTradSym];
      else if (iItfOrUndly == 1) sSymbol = ContFut::GetContFutUndly(m_TradedSymbols[iTradSym]);

      vector<Sma<double> > v;
      v.insert(v.begin(), max(m_MAPeriod_1_end[iTradSym],m_MAPeriod_2_end[iTradSym])+1, Sma<double>());
      m_mv_Sma[sSymbol] = v;

      set<int> setUnion = ComputeUnionOfMAPeriods
        (
          m_MAPeriod_1_start[iTradSym],
          m_MAPeriod_1_end[iTradSym],
          m_MAPeriod_1_increment[iTradSym],
          m_MAPeriod_2_start[iTradSym],
          m_MAPeriod_2_end[iTradSym],
          m_MAPeriod_2_increment[iTradSym]
        );

      m_map_MAPeriodUnion[sSymbol] = setUnion;

      FForEach(setUnion,[&](const int maprd){
        m_mv_Sma[sSymbol][maprd].Reset(maprd,true);
        m_Logger->Write(Logger::INFO,"Strategy %s: %s MA periods for training includes %d", GetStrategyName(m_StyID).c_str(), sSymbol.c_str(), maprd);
      } );
    }
  }



  //--------------------------------------------------

}

void StrategyB1_HKF::StartOfDayInit()
{
  for (unsigned int i = 0; i < m_TradedSymbols.size(); ++i)
  {
    m_map_DoneEODTrade[m_TradedSymbols[i]] = false;
  }
}

void StrategyB1_HKF::EndOfDayCleanUp()
{
}

void StrategyB1_HKF::SetConvenienceVarb(const int iTradSym)
{

  {
    map<string,vector<double> >::iterator it1 = m_map_HistoricalAvgPx.find(m_TradedSymbols[iTradSym]);
    if (it1 == m_map_HistoricalAvgPx.end())
    {
      m_map_HistoricalAvgPx[m_TradedSymbols[iTradSym]] = vector<double>();
      it1 = m_map_HistoricalAvgPx.find(m_TradedSymbols[iTradSym]);
    }
    m_HistoricalAvgPx = &(it1->second);
  }
  {
    map<string,vector<double> >::iterator it1 = m_map_HistoricalClose.find(m_TradedSymbols[iTradSym]);
    if (it1 == m_map_HistoricalClose.end())
    {
      m_map_HistoricalClose[m_TradedSymbols[iTradSym]] = vector<double>();
      it1 = m_map_HistoricalClose.find(m_TradedSymbols[iTradSym]);
    }
    m_HistoricalClose = &(it1->second);
  }

  if (ContFut::IsSymContFut(m_TradedSymbols[iTradSym]))
  {
    string sUndly = ContFut::GetContFutUndly(m_TradedSymbols[iTradSym]);
    //--------------------------------------------------
    {
      map<string,vector<double> >::iterator it1 = m_map_HistoricalAvgPx.find(sUndly);
      if (it1 == m_map_HistoricalAvgPx.end())
      {
        m_map_HistoricalAvgPx[sUndly] = vector<double>();
        it1 = m_map_HistoricalAvgPx.find(sUndly);
      }
      m_HistoricalUndlyAvgPx = &(it1->second);
    }
    //--------------------------------------------------
    {
      map<string,vector<double> >::iterator it1 = m_map_HistoricalClose.find(sUndly);
      if (it1 == m_map_HistoricalClose.end())
      {
        m_map_HistoricalClose[sUndly] = vector<double>();
        it1 = m_map_HistoricalClose.find(sUndly);
      }
      m_HistoricalUndlyClose = &(it1->second);
    }
  }

  {
    map<string,set<YYYYMMDD> >::iterator it3 = m_map_HistoricalPricesDates.find(m_TradedSymbols[iTradSym]);
    if (it3 == m_map_HistoricalPricesDates.end())
    {
      m_map_HistoricalPricesDates[m_TradedSymbols[iTradSym]] = set<YYYYMMDD>();
      it3 = m_map_HistoricalPricesDates.find(m_TradedSymbols[iTradSym]);
    }
    m_HistoricalPricesDates = &(it3->second);
  }

  if (ContFut::IsSymContFut(m_TradedSymbols[iTradSym]))
  {
    string sUndly = ContFut::GetContFutUndly(m_TradedSymbols[iTradSym]);
    map<string,set<YYYYMMDD> >::iterator it3 = m_map_HistoricalPricesDates.find(sUndly);
    if (it3 == m_map_HistoricalPricesDates.end())
    {
      m_map_HistoricalPricesDates[sUndly] = set<YYYYMMDD>();
      it3 = m_map_HistoricalPricesDates.find(sUndly);
    }
    m_HistoricalUndlyPricesDates = &(it3->second);
  }

  //--------------------------------------------------
  {
    map<string,map<double,int> >::iterator it5a = m_map_BestParamSetUndlyDeriv1.find(m_TradedSymbols[iTradSym]);
    if (it5a == m_map_BestParamSetUndlyDeriv1.end())
    {
      m_map_BestParamSetUndlyDeriv1[m_TradedSymbols[iTradSym]] = map<double,int>();
      it5a = m_map_BestParamSetUndlyDeriv1.find(m_TradedSymbols[iTradSym]);
    }
    m_p_map_BestParamSetUndlyDeriv1 = &(it5a->second);
  }
  {
    map<string,map<double,int> >::iterator it5a = m_map_BestParamSetDerivDeriv1.find(m_TradedSymbols[iTradSym]);
    if (it5a == m_map_BestParamSetDerivDeriv1.end())
    {
      m_map_BestParamSetDerivDeriv1[m_TradedSymbols[iTradSym]] = map<double,int>();
      it5a = m_map_BestParamSetDerivDeriv1.find(m_TradedSymbols[iTradSym]);
    }
    m_p_map_BestParamSetDerivDeriv1 = &(it5a->second);
  }
  {
    map<string,map<double,int> >::iterator it5a = m_map_BestParamSetUndlyDeriv2.find(m_TradedSymbols[iTradSym]);
    if (it5a == m_map_BestParamSetUndlyDeriv2.end())
    {
      m_map_BestParamSetUndlyDeriv2[m_TradedSymbols[iTradSym]] = map<double,int>();
      it5a = m_map_BestParamSetUndlyDeriv2.find(m_TradedSymbols[iTradSym]);
    }
    m_p_map_BestParamSetUndlyDeriv2 = &(it5a->second);
  }
  //--------------------------------------------------


  //--------------------------------------------------
  {
    map<string,bool>::iterator it7 = m_map_DoneEODTrade.find(m_TradedSymbols[iTradSym]);
    if (it7 == m_map_DoneEODTrade.end())
    {
      m_map_DoneEODTrade[m_TradedSymbols[iTradSym]] = false;
      it7 = m_map_DoneEODTrade.find(m_TradedSymbols[iTradSym]);
    }
    m_DoneEODTrade = &(it7->second);
  }


  //--------------------------------------------------
  map<string,bool>::iterator it8a = m_map_bTrainRetAvgPx.find(m_TradedSymbols[iTradSym]);
  if (it8a == m_map_bTrainRetAvgPx.end())
  {
    m_map_bTrainRetAvgPx[m_TradedSymbols[iTradSym]] = false;
    it8a = m_map_bTrainRetAvgPx.find(m_TradedSymbols[iTradSym]);
  }
  m_bTrainRetAvgPx = &(it8a->second);

}


bool StrategyB1_HKF::SkipSubseqProcessingForSymbol(const int iTradSym, string & sComment)
{
  return false;
  //--------------------------------------------------
  // Testing
  //--------------------------------------------------
  // sComment = "m_DoneEODTrade = " + string(*m_DoneEODTrade?"true":"false");
  // if (*m_DoneEODTrade) return true;
  //
  // //--------------------------------------------------
  // if (
  //   m_ymdhms_SysTime_HKT.GetHHMMSS() == HHMMSS(100000) ||
  //   m_ymdhms_SysTime_HKT.GetHHMMSS() == HHMMSS(160000)
  //   )
  // {
  //   return false;
  // }
  // //--------------------------------------------------
  //
  // return true;
}

void StrategyB1_HKF::InitialWarmUp(const int iTradSym)
{

  YMD ymdStart(m_ymdhms_SysTime_HKT.GetYYYYMMDD());
  ymdStart.SubtractDay(m_TrainingPeriod[iTradSym]+10);

  for (unsigned int iItfOrUndly = 0; iItfOrUndly < 2; ++iItfOrUndly)
  {
    string sSymbol;

    if (iItfOrUndly == 1 && !ContFut::IsSymContFut(m_TradedSymbols[iTradSym])) break;

    if      (iItfOrUndly == 0) sSymbol = m_TradedSymbols[iTradSym];
    else if (iItfOrUndly == 1) sSymbol = ContFut::GetContFutUndly(m_TradedSymbols[iTradSym]);

    //--------------------------------------------------
    vector<YYYYMMDD> vYMD;
    vector<double>   vOpen;
    vector<double>   vHigh;
    vector<double>   vLow;
    vector<double>   vClose;
    vector<long>     vVol;

    //TODO MarketData::Instance()
    MarketData::Instance()->GetSuppD1BarOHLCVInDateRange(sSymbol, YYYYMMDD(ymdStart), m_ymdhms_SysTime_HKT.GetYYYYMMDD(), vYMD, vOpen, vHigh, vLow, vClose, vVol);

    if (!vYMD.empty() && (m_ymdhms_SysTime_HKT.GetYYYYMMDD() > vYMD.back() && m_ymdhms_SysTime_HKT.GetYYYYMMDD() - vYMD.back() > 5))
    {
      m_Logger->Write(Logger::ERROR,"Strategy %s: %s Supplementary day bar may be out-of-date. Current date: %s. Last supp day bar date: %s.", GetStrategyName(m_StyID).c_str(), sSymbol.c_str(), m_ymdhms_SysTime_HKT.GetYYYYMMDD().ToStr_().c_str(), vYMD.back().ToStr_().c_str());
      // sleep(1);
      // exit(1);
    }

    if (vYMD.empty()) return;
    for (unsigned int i = 0; i < vYMD.size(); ++i)
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: %s Read from day bar file: %s %s High = %f Low = %f Close = %f.", GetStrategyName(m_StyID).c_str(), sSymbol.c_str(), vYMD[i].ToStr_().c_str(),sSymbol.c_str(), vHigh[i],vLow[i],vClose[i]);

      //--------------------------------------------------
      // skip today
      //--------------------------------------------------
      if (m_ymdhms_SysTime_HKT.GetYYYYMMDD() == vYMD[i])
      {
        m_Logger->Write(Logger::INFO,"Strategy %s: %s Skipping day bar %s because it's the same as today.", GetStrategyName(m_StyID).c_str(), sSymbol.c_str(), vYMD[i].ToStr_().c_str());
        continue;
      }

      //--------------------------------------------------
      // Testing
      //--------------------------------------------------
      // double dAvgPx = vClose[i];
      double dAvgPx = (vHigh[i] + vLow[i] + vClose[i]) / 3;

      if (!std::isnan(dAvgPx))
      {
        if (iItfOrUndly == 0)
        {
          m_HistoricalAvgPx->push_back(dAvgPx);
          m_HistoricalClose->push_back(vClose[i]);
          m_HistoricalPricesDates->insert(vYMD[i]);
        }
        else if (iItfOrUndly == 1)
        {
          m_HistoricalUndlyAvgPx->push_back(dAvgPx);
          m_HistoricalUndlyClose->push_back(vClose[i]);
          m_HistoricalUndlyPricesDates->insert(vYMD[i]);
        }

        //--------------------------------------------------
        FForEach(m_map_MAPeriodUnion[m_TradedSymbols[iTradSym]],[&](const int maprd){ m_mv_Sma[sSymbol][maprd].Add(dAvgPx); } );
        //--------------------------------------------------

      }
    }
  }
}

void StrategyB1_HKF::UpdateInternalDataTrng(const int iTradSym)
{
  return;
}

void StrategyB1_HKF::UpdateInternalData(const int iTradSym)
{

  HHMMSS hmsStart = m_Exchg->GetTimeNSecBefClose(m_TradedSymbols[iTradSym],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),300);
  HHMMSS hmsEnd   = m_Exchg->GetTimeNSecBefClose(m_TradedSymbols[iTradSym],m_ymdhms_SysTime_HKT.GetYYYYMMDD(),0);

  m_Logger->Write(Logger::DEBUG,"Strategy %s: %s will only proceed when time (currently %s) is between %s and %s",
                  GetStrategyName(m_StyID).c_str(),
                  m_TradedSymbols[iTradSym].c_str(),
                  m_ymdhms_SysTime_HKT.GetHHMMSS().ToStr_().c_str(),
                  hmsStart.ToStr_().c_str(),
                  hmsEnd.ToStr_().c_str()
                 );

  if (
    m_HistoricalPricesDates->find(m_ymdhms_SysTime_HKT.GetYYYYMMDD()) == m_HistoricalPricesDates->end() &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() >= hmsStart &&
    m_ymdhms_SysTime_HKT.GetHHMMSS() <= hmsEnd
    )
  {

    for (unsigned int iItfOrUndly = 0; iItfOrUndly < 2; ++iItfOrUndly)
    {
      string sSymbol;

      if (iItfOrUndly == 1 && !ContFut::IsSymContFut(m_TradedSymbols[iTradSym])) break;

      if      (iItfOrUndly == 0) sSymbol = m_TradedSymbols[iTradSym];
      else if (iItfOrUndly == 1) sSymbol = ContFut::GetContFutUndly(m_TradedSymbols[iTradSym]);

      double dHigh  = TechIndicators::Instance()->GetDayHigh(sSymbol);
      double dLow   = TechIndicators::Instance()->GetDayLow(sSymbol);
      double dClose = TechIndicators::Instance()->GetDayClose(sSymbol);

      if (!std::isnan(dHigh) && !std::isnan(dLow) && !std::isnan(dClose))
      {

        {
          //--------------------------------------------------
          // Wait for TechIndUpdater to finish
          //--------------------------------------------------
          double dMQ = 0;
          YYYYMMDDHHMMSS ymdhms_mq;
          //--------------------------------------------------
          // TODO: MarketData
          //--------------------------------------------------
          MarketData::Instance()->GetLatestMidQuote(sSymbol,dMQ,ymdhms_mq);
          if (!m_SysCfg->CheckIfHalfDayTrading(sSymbol,m_ymdhms_SysTime_HKT.GetYYYYMMDD()))
          {
            while (abs(dMQ - dClose) > NIR_EPSILON)
            {
              MarketData::Instance()->GetLatestMidQuote(sSymbol,dMQ,ymdhms_mq);

              dHigh  = TechIndicators::Instance()->GetDayHigh(sSymbol);
              dLow   = TechIndicators::Instance()->GetDayLow(sSymbol);
              dClose = TechIndicators::Instance()->GetDayClose(sSymbol);
            }
          }
        }

        if (iItfOrUndly == 0)
        {
          m_Logger->Write(Logger::INFO,"Strategy %s: %s %s Getting aggregated daily high low close: High = %f Low = %f Close = %f",
                          GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(),sSymbol.c_str(), dHigh, dLow, dClose);
          //--------------------------------------------------
          // Testing
          //--------------------------------------------------
          double dAvgPx = (dHigh+dLow+dClose)/(double)3;
          // double dAvgPx = dClose;

          m_HistoricalAvgPx->push_back(dAvgPx);
          m_HistoricalClose->push_back(dClose);
          m_HistoricalPricesDates->insert(m_ymdhms_SysTime_HKT.GetYYYYMMDD());

          if (m_HistoricalAvgPx->size()>1)
          {
            FForEach(m_map_MAPeriodUnion[m_TradedSymbols[iTradSym]],[&](const int maprd) { m_mv_Sma[sSymbol][maprd].Add(m_HistoricalAvgPx->back()); });
          }
        }
        else if (iItfOrUndly == 1)
        {
          m_Logger->Write(Logger::INFO,"Strategy %s: %s %s Getting aggregated daily high low close: High = %f Low = %f Close = %f",
                          GetStrategyName(m_StyID).c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str(),sSymbol.c_str(), dHigh, dLow, dClose);
          //--------------------------------------------------
          // Testing
          //--------------------------------------------------
          double dAvgPx = (dHigh+dLow+dClose)/(double)3;
          // double dAvgPx = dClose;

          m_HistoricalUndlyAvgPx->push_back(dAvgPx);
          m_HistoricalUndlyClose->push_back(dClose);
          m_HistoricalUndlyPricesDates->insert(m_ymdhms_SysTime_HKT.GetYYYYMMDD());

          if (m_HistoricalUndlyAvgPx->size()>1)
          {
            FForEach(m_map_MAPeriodUnion[m_TradedSymbols[iTradSym]],[&](const int maprd){ m_mv_Sma[sSymbol][maprd].Add(m_HistoricalUndlyAvgPx->back());  } );
          }
        }

      }
    }

  }
}



void StrategyB1_HKF::PerformTrainingJustBeforeTrading(const int iTradSym)
{

  // //--------------------------------------------------
  // // Testing
  // //--------------------------------------------------
  return;


  //--------------------------------------------------
  // Should do the training here, otherwise will miss the last data point (which is today) in our training
  //--------------------------------------------------

  //--------------------------------------------------
  // Train up our parameters
  //--------------------------------------------------
  if (
    m_B1_TrainingFreq == TradingStrategyConfig::Daily
    ||
    (
      !m_map_ymdhms_LastTrainTime[B1_METH_AVGPX][m_TradedSymbols[iTradSym]].IsValid()
      ||
      ((*m_bTrainRetAvgPx) && m_ymdhms_SysTime_HKT.GetYYYYMMDD() - m_map_ymdhms_LastTrainTime[B1_METH_AVGPX][m_TradedSymbols[iTradSym]].GetYYYYMMDD() > (int)ceil(m_TrainingPeriod[iTradSym]/(double)10))
      ||
      (!(*m_bTrainRetAvgPx) && m_ymdhms_SysTime_HKT.GetYYYYMMDD() > m_map_ymdhms_LastTrainTime[B1_METH_AVGPX][m_TradedSymbols[iTradSym]].GetYYYYMMDD())
      ||
      (m_p_map_BestParamSetUndlyDeriv1->empty() && m_p_map_BestParamSetDerivDeriv1->empty() && m_p_map_BestParamSetUndlyDeriv2->empty())
    )
    )
  {
    SetParamRange(
      m_MAPeriod_1_start[iTradSym],
      m_MAPeriod_1_end[iTradSym],
      m_MAPeriod_1_increment[iTradSym],
      m_MAPeriod_2_start[iTradSym],
      m_MAPeriod_2_end[iTradSym],
      m_MAPeriod_2_increment[iTradSym]
      );

    *m_bTrainRetAvgPx = TrainUpParam(
      m_ymdhms_SysTime_HKT,
      m_TrainingPeriod[iTradSym],
      (*m_HistoricalAvgPx),
      (*m_HistoricalClose),
      // StrategyB1::TM_MAXSHARPE,
      StrategyB1::TM_MAXPROFIT,
      m_WeightScheme[iTradSym],
      (*m_p_map_BestParamSetUndlyDeriv1),
      (*m_p_map_BestParamSetDerivDeriv1),
      (*m_p_map_BestParamSetUndlyDeriv2),
      m_CurbInSmpRtn[iTradSym]
      );

    m_map_ymdhms_LastTrainTime[B1_METH_AVGPX][m_TradedSymbols[iTradSym]] = m_ymdhms_SysTime_HKT;
  }

  m_Logger->Write(Logger::INFO,"Strategy %s: %s %s (%d) %s AvgPx TrainUpParam() returned %s. m_p_map_BestParamSetUndlyDeriv1.size() = %d m_p_map_BestParamSetDerivDeriv1.size() = %d m_p_map_BestParamSetUndlyDeriv2.size() = %d LastTrainTime = %s",
                  GetStrategyName(m_StyID).c_str(),
                  m_ymdhms_SysTime_HKT.ToStr().c_str(),
                  __FUNCTION__,__LINE__,
                  m_TradedSymbols[iTradSym].c_str(),(*m_bTrainRetAvgPx?"True":"False"),
                  (*m_p_map_BestParamSetUndlyDeriv1).size(),
                  (*m_p_map_BestParamSetDerivDeriv1).size(),
                  (*m_p_map_BestParamSetUndlyDeriv2).size(),
                  m_map_ymdhms_LastTrainTime[B1_METH_AVGPX][m_TradedSymbols[iTradSym]].ToStr().c_str()
                 );

}

void StrategyB1_HKF::EndOfDayTrainingForEachTimeBucket(const int iTradSym,const map<HHMMSS,double> & map_HistDataInTimeBucket)
{
}

void StrategyB1_HKF::DetermineRegime(const int iTradSym)
{
}

void StrategyB1_HKF::PreTradePreparation(const int iTradSym)
{

  long lNumOfTrngCombn1 = (m_MAPeriod_1_end[iTradSym] - m_MAPeriod_1_start[iTradSym] + 1) / m_MAPeriod_1_increment[iTradSym];
  long lNumOfTrngCombn2 = (m_MAPeriod_2_end[iTradSym] - m_MAPeriod_2_start[iTradSym] + 1) / m_MAPeriod_2_increment[iTradSym];
  m_dStrengthCountAvgPx = 0;
  //--------------------------------------------------


  // //--------------------------------------------------
  // // Testing
  // //--------------------------------------------------
  // if (m_mv_Sma[m_TradedSymbols[iTradSym]][5].Ready())
  // {
  //   if (m_SymMidQuote >= m_mv_Sma[m_TradedSymbols[iTradSym]][5].Value())
  //     m_dStrengthCountAvgPx += 1;
  //   else 
  //     m_dStrengthCountAvgPx -= 1;
  // }


  //--------------------------------------------------
  // Testing
  //--------------------------------------------------
  if (m_mv_Sma.find(string("HSI")) != m_mv_Sma.end() && m_mv_Sma[string("HSI")][30].Ready())
  {
    double dMQ = 0;
    YYYYMMDDHHMMSS ymdhms_mq;
    //--------------------------------------------------
    // TODO: MarketData
    //--------------------------------------------------
    MarketData::Instance()->GetLatestMidQuote(string("HSI"),dMQ,ymdhms_mq);

    if (dMQ >= m_mv_Sma[string("HSI")][2].Value())
      m_dStrengthCountAvgPx += 1;
    else 
      m_dStrengthCountAvgPx -= 1;

    if (dMQ >= m_mv_Sma[string("HSI")][3].Value())
      m_dStrengthCountAvgPx += 1;
    else 
      m_dStrengthCountAvgPx -= 1;

    if (dMQ >= m_mv_Sma[string("HSI")][4].Value())
      m_dStrengthCountAvgPx += 1;
    else 
      m_dStrengthCountAvgPx -= 1;

    if (dMQ >= m_mv_Sma[string("HSI")][5].Value())
      m_dStrengthCountAvgPx += 1;
    else 
      m_dStrengthCountAvgPx -= 1;

    if (abs(dMQ - m_mv_Sma[string("HSI")][30].Value())/dMQ > 0.04)
      m_dStrengthCountAvgPx = 0;
  }



  // --------------------------------------------------
  map<double,int>::iterator it_BestParam;
  if (*m_bTrainRetAvgPx)
  {

    if (
      !m_p_map_BestParamSetUndlyDeriv1->empty() &&
      !m_p_map_BestParamSetDerivDeriv1->empty() &&
      !m_p_map_BestParamSetUndlyDeriv2->empty()
      )
    {

      // {
      //   //--------------------------------------------------
      //   // Using stage 1 optimization result : m_p_map_BestParamSetUndlyDeriv1
      //   //--------------------------------------------------
      //   it_BestParam = m_p_map_BestParamSetUndlyDeriv1->end();
      //   it_BestParam--;
      //
      //   int iCnt = 0;
      //
      //   while (it_BestParam != m_p_map_BestParamSetUndlyDeriv1->begin())
      //   {
      //     //--------------------------------------------------
      //     // Testing
      //     //--------------------------------------------------
      //     const int iSMAPeriod = it_BestParam->second;
      //     // const int iSMAPeriod = 1;
      //
      //     const string sUndlySym = ContFut::GetContFutUndly(m_TradedSymbols[iTradSym]);
      //
      //     double dUndlyMQ = 0;
      //     YYYYMMDDHHMMSS ymdhms_undly_mq;
      //     MarketData::Instance()->GetLatestMidQuote(sUndlySym,dUndlyMQ,ymdhms_undly_mq);
      //
      //     if (dUndlyMQ >= m_mv_Sma[sUndlySym][iSMAPeriod].Value())
      //       m_dStrengthCountAvgPx += (double)1/(double)(lNumOfTrngCombn1*2);
      //     else 
      //       m_dStrengthCountAvgPx -= (double)1/(double)(lNumOfTrngCombn1*2);
      //
      //     // //--------------------------------------------------
      //     // // Testing
      //     // //--------------------------------------------------
      //     // if (dUndlyMQ >= m_mv_Sma[sUndlySym][iSMAPeriod].Value())
      //     //   m_dStrengthCountAvgPx += (double)1/(double)(lNumOfTrngCombn1*2);
      //     // else 
      //     //   m_dStrengthCountAvgPx -= (double)1/(double)(lNumOfTrngCombn1*2);
      //     // //--------------------------------------------------
      //
      //     m_Logger->Write(Logger::INFO,"Strategy %s: %s Selected from the set of best parameters: m_p_map_BestParamSetUndlyDeriv1 SMA period = %d %s mid-quote = %f %s mid-quote = %f %s SMA value = %f %s MQ >= SMA = %s",
      //                     GetStrategyName(m_StyID).c_str(),
      //                     m_ymdhms_SysTime_HKT.ToStr().c_str(),
      //                     iSMAPeriod,
      //                     m_TradedSymbols[iTradSym].c_str(),
      //                     m_SymMidQuote,
      //                     sUndlySym.c_str(),
      //                     dUndlyMQ,
      //                     sUndlySym.c_str(),
      //                     m_mv_Sma[sUndlySym][iSMAPeriod].Value(),
      //                     sUndlySym.c_str(),
      //                     (dUndlyMQ >= m_mv_Sma[sUndlySym][iSMAPeriod].Value() ? "true" : "false")
      //                    );
      //
      //     // //--------------------------------------------------
      //     // // Testing
      //     // //--------------------------------------------------
      //     // break;
      //     // //--------------------------------------------------
      //
      //     iCnt++;
      //     if ((iCnt+1) >= m_PropOfBestParam[iTradSym] / (double)100 * (double)lNumOfTrngCombn1) break;
      //     if (iCnt >= m_p_map_BestParamSetUndlyDeriv1->size()) break;
      //     it_BestParam--;
      //   }
      //
      //   m_Logger->Write(Logger::INFO,"Strategy %s: %s %s m_p_map_BestParamSetUndlyDeriv1 Selected a set of %d parameters in total.",
      //                   GetStrategyName(m_StyID).c_str(),
      //                   m_ymdhms_SysTime_HKT.ToStr().c_str(),
      //                   m_TradedSymbols[iTradSym].c_str(),
      //                   iCnt);
      // }

      // {
      //   //--------------------------------------------------
      //   // Using stage 1 optimization result : m_p_map_BestParamSetDerivDeriv1
      //   //--------------------------------------------------
      //   it_BestParam = m_p_map_BestParamSetDerivDeriv1->begin();
      //   map<double,int>::iterator it_BestParamEnd = m_p_map_BestParamSetDerivDeriv1->end();
      //   int iCnt = 0;
      //
      //   while (it_BestParam != it_BestParamEnd)
      //   {
      //     //--------------------------------------------------
      //     // Testing
      //     //--------------------------------------------------
      //     const int iSMAPeriod = it_BestParam->second;
      //     // const int iSMAPeriod = 5;
      //
      //     //--------------------------------------------------
      //     // Anti-strength
      //     //--------------------------------------------------
      //     if (m_SymMidQuote >= m_mv_Sma[m_TradedSymbols[iTradSym]][iSMAPeriod].Value())
      //       m_dStrengthCountAvgPx -= (double)1/(double)(lNumOfTrngCombn1*2);
      //     else 
      //       m_dStrengthCountAvgPx += (double)1/(double)(lNumOfTrngCombn1*2);
      //
      //     // //--------------------------------------------------
      //     // // Testing
      //     // //--------------------------------------------------
      //     // if (m_SymMidQuote >= m_mv_Sma[m_TradedSymbols[iTradSym]][iSMAPeriod].Value())
      //     //   m_dStrengthCountAvgPx -= (double)1/(double)(lNumOfTrngCombn1*2);
      //     // else 
      //     //   m_dStrengthCountAvgPx += (double)1/(double)(lNumOfTrngCombn1*2);
      //     // //--------------------------------------------------
      //
      //     m_Logger->Write(Logger::INFO,"Strategy %s: %s Selected from the set of best parameters: m_p_map_BestParamSetDerivDeriv1 SMA period = %d %s mid-quote = %f %s SMA value = %f %s MQ >= SMA = %s",
      //                     GetStrategyName(m_StyID).c_str(),
      //                     m_ymdhms_SysTime_HKT.ToStr().c_str(),
      //                     iSMAPeriod,
      //                     m_TradedSymbols[iTradSym].c_str(),
      //                     m_SymMidQuote,
      //                     m_TradedSymbols[iTradSym].c_str(),
      //                     m_mv_Sma[m_TradedSymbols[iTradSym]][iSMAPeriod].Value(),
      //                     m_TradedSymbols[iTradSym].c_str(),
      //                     (m_SymMidQuote >= m_mv_Sma[m_TradedSymbols[iTradSym]][iSMAPeriod].Value() ? "true" : "false")
      //                    );
      //
      //     // //--------------------------------------------------
      //     // // Testing
      //     // //--------------------------------------------------
      //     // break;
      //     // //--------------------------------------------------
      //
      //     iCnt++;
      //     if ((iCnt+1) >= m_PropOfBestParam[iTradSym] / (double)100 * (double)lNumOfTrngCombn1) break;
      //     if (iCnt >= m_p_map_BestParamSetDerivDeriv1->size()) break;
      //     it_BestParam++;
      //   }
      //
      //   m_Logger->Write(Logger::INFO,"Strategy %s: %s %s m_p_map_BestParamSetDerivDeriv1 Selected a set of %d parameters in total.",
      //                   GetStrategyName(m_StyID).c_str(),
      //                   m_ymdhms_SysTime_HKT.ToStr().c_str(),
      //                   m_TradedSymbols[iTradSym].c_str(),
      //                   iCnt);
      // }


      // if (abs(m_dStrengthCountAvgPx) < NIR_EPSILON)
      // {
      //   //--------------------------------------------------
      //   // if there is no discrepancy between the futures and the underlying
      //   // Using stage 2 optimization result
      //   //--------------------------------------------------
      //   it_BestParam = m_p_map_BestParamSetUndlyDeriv2->end();
      //   it_BestParam--;
      //
      //   int iCnt = 0;
      //
      //   while (it_BestParam != m_p_map_BestParamSetUndlyDeriv2->begin())
      //   {
      //     //--------------------------------------------------
      //     // Testing
      //     //--------------------------------------------------
      //     const int iSMAPeriod = it_BestParam->second;
      //     // const int iSMAPeriod = 50;
      //
      //     const string sUndlySym = ContFut::GetContFutUndly(m_TradedSymbols[iTradSym]);
      //
      //     double dUndlyMQ = 0;
      //     YYYYMMDDHHMMSS ymdhms_undly_mq;
      //     MarketData::Instance()->GetLatestMidQuote(sUndlySym,dUndlyMQ,ymdhms_undly_mq);
      //
      //     if (dUndlyMQ >= m_mv_Sma[sUndlySym][iSMAPeriod].Value())
      //       m_dStrengthCountAvgPx += (double)1/(double)lNumOfTrngCombn2;
      //     else 
      //       m_dStrengthCountAvgPx -= (double)1/(double)lNumOfTrngCombn2;
      //
      //     // //--------------------------------------------------
      //     // // Testing
      //     // //--------------------------------------------------
      //     // if (dUndlyMQ >= m_mv_Sma[sUndlySym][iSMAPeriod].Value())
      //     //   m_dStrengthCountAvgPx += (double)3/(double)lNumOfTrngCombn2;
      //     // else
      //     //   m_dStrengthCountAvgPx -= (double)3/(double)lNumOfTrngCombn2;
      //     // //--------------------------------------------------
      //
      //     m_Logger->Write(Logger::INFO,"Strategy %s: %s Selected from the set of best parameters: m_p_map_BestParamSetUndlyDeriv2 SMA period = %d %s mid-quote = %f %s mid-quote = %f %s SMA value = %f %s MQ >= SMA = %s",
      //                     GetStrategyName(m_StyID).c_str(),
      //                     m_ymdhms_SysTime_HKT.ToStr().c_str(),
      //                     iSMAPeriod,
      //                     m_TradedSymbols[iTradSym].c_str(),
      //                     m_SymMidQuote,
      //                     sUndlySym.c_str(),
      //                     dUndlyMQ,
      //                     sUndlySym.c_str(),
      //                     m_mv_Sma[sUndlySym][iSMAPeriod].Value(),
      //                     sUndlySym.c_str(),
      //                     (dUndlyMQ >= m_mv_Sma[sUndlySym][iSMAPeriod].Value() ? "true" : "false")
      //                    );
      //
      //     // //--------------------------------------------------
      //     // // Testing
      //     // //--------------------------------------------------
      //     // break;
      //     // //--------------------------------------------------
      //
      //     iCnt++;
      //     if ((iCnt+1) >= m_PropOfBestParam[iTradSym] / (double)100 * (double)lNumOfTrngCombn2) break;
      //     if (iCnt >= m_p_map_BestParamSetUndlyDeriv2->size()) break;
      //     it_BestParam--;
      //   }
      //
      //   m_Logger->Write(Logger::INFO,"Strategy %s: %s %s m_p_map_BestParamSetUndlyDeriv2 Selected a set of %d parameters in total.",
      //                   GetStrategyName(m_StyID).c_str(),
      //                   m_ymdhms_SysTime_HKT.ToStr().c_str(),
      //                   m_TradedSymbols[iTradSym].c_str(),
      //                   iCnt);
      // }
      //
      // // //--------------------------------------------------
      // // // Testing
      // // //--------------------------------------------------
      // // else
      // // {
      // //   m_dStrengthCountAvgPx = 0;
      // // }
      // // //--------------------------------------------------




    }
    else
    {
      m_Logger->Write(Logger::INFO,"Strategy %s: %s %s m_p_map_BestParamSet empty.", GetStrategyName(m_StyID).c_str(), m_TradedSymbols[iTradSym].c_str(), m_ymdhms_SysTime_HKT.ToStr().c_str());
    }

  }
  else
  {
    m_Logger->Write(Logger::INFO,"Strategy %s: %s %s %s (%d) m_bTrainRetAvgPx is false.",
                    GetStrategyName(m_StyID).c_str(),
                    m_ymdhms_SysTime_HKT.ToStr().c_str(),
                    m_TradedSymbols[iTradSym].c_str(),
                    __FUNCTION__,
                    __LINE__);
  }
  //--------------------------------------------------


  m_Logger->Write(Logger::INFO,"Strategy %s: %s %s m_dStrengthCountAvgPx = %f lNumOfTrngCombn1 = %d lNumOfTrngCombn2 = %d",
                  GetStrategyName(m_StyID).c_str(),
                  m_ymdhms_SysTime_HKT.ToStr().c_str(),
                  m_TradedSymbols[iTradSym].c_str(),
                  m_dStrengthCountAvgPx,
                  lNumOfTrngCombn1,
                  lNumOfTrngCombn2
                 );

  //--------------------------------------------------
  // Calculate actual position size
  //--------------------------------------------------
  m_dAggUnsignedQty = 0;
  m_dAggSignedQty = 0;

  //--------------------------------------------------
  // Testing
  //--------------------------------------------------
  // m_dAggUnsignedQty = 10;
  m_dAggUnsignedQty = (m_NotionalAmt[iTradSym] / m_SymFM1MidQuote) / (m_HKFE->GetContractMultiplier(m_TradedSymbols[iTradSym]));


  //--------------------------------------------------
  // remember we have scaled the strength
  //--------------------------------------------------
  // m_dAggSignedQty = round(m_dAggUnsignedQty * m_dStrengthCountAvgPx / (m_PropOfBestParam[iTradSym] / (double)100 * (double)lNumOfTrngCombn1));
  m_dAggSignedQty = round(m_dAggUnsignedQty * m_dStrengthCountAvgPx / (m_PropOfBestParam[iTradSym]/(double)100));

  //--------------------------------------------------
  if (!(*m_DoneEODTrade))
  {
    m_TargetTradeDir [iTradSym] = (m_dAggSignedQty > 0 ? TD_LONG : TD_SHORT);
    m_TargetAbsPos   [iTradSym] = abs(m_dAggSignedQty);
  }
  *m_DoneEODTrade = true;
  //--------------------------------------------------

}

void StrategyB1_HKF::CalcPosSize(const int iTradSym)
{
}

void StrategyB1_HKF::UnsetConvenienceVarb()
{

  m_HistoricalAvgPx               = NULL;
  m_HistoricalClose               = NULL;
  m_HistoricalPricesDates         = NULL;
  m_p_map_BestParamSetUndlyDeriv1 = NULL;
  m_p_map_BestParamSetDerivDeriv1 = NULL;
  m_p_map_BestParamSetUndlyDeriv2 = NULL;
  m_bTrainRetAvgPx                = NULL;
  m_DoneEODTrade                  = NULL;

  return;
}

void StrategyB1_HKF::ParamSanityCheck()
{
  return;
}

void StrategyB1_HKF::LoadOptimalParamsFromTrainingResults()
{
  return;
}

set<int> StrategyB1_HKF::ComputeUnionOfMAPeriods(
  const int ma_period_1_start,
  const int ma_period_1_end,
  const int ma_period_1_increment,
  const int ma_period_2_start,
  const int ma_period_2_end,
  const int ma_period_2_increment)
{
  set<int> setUnion;
  for (int maprd = ma_period_1_start; maprd <= ma_period_1_end; maprd += ma_period_1_increment) setUnion.insert(maprd);
  for (int maprd = ma_period_2_start; maprd <= ma_period_2_end; maprd += ma_period_2_increment) setUnion.insert(maprd);
  return setUnion;
}
