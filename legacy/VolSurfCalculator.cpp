#include <VolSurfCalculator.h>

VolSurfCalculator::VolSurfCalculator() :
  m_CalcIntervalInSeconds(60)
{
  m_MarketData   = MarketData::Instance();
  m_SystemState  = SystemState::Instance();
  m_SysCfg       = SystemConfig::Instance();
  m_VolSurfaces  = VolSurfaces::Instance();
  m_Logger       = Logger::Instance();
  m_Exchg        = Exchange::Instance();
  m_HKFE         = HKFE::Instance();
  m_HKMA         = HKMA::Instance();
  m_TechInd      = TechIndicators::Instance();
  m_ThrdHlthMon  = ThreadHealthMonitor::Instance();
}

VolSurfCalculator::~VolSurfCalculator()
{
}

void VolSurfCalculator::SetCalcIntervalInSec(long calcint)
{
  m_CalcIntervalInSeconds = calcint;
}

void VolSurfCalculator::Run()
{
  for (;;)
  {
    m_MarketData->WaitForData();

    if (m_SystemState->ChkIfThreadShouldStop()) break;

    m_ThrdHlthMon->ReportThatIAmHealthy(ID_VOLSURFCALC);

    //--------------------------------------------------
    // Figure out system time and which volatility surfaces should be calculated
    //--------------------------------------------------
    YYYYMMDDHHMMSS ymdhms_MDITime = m_MarketData->GetSystemTimeHKT();

    if (m_ymdhms_LastCalculated.GetYYYYMMDD() == YYYYMMDD::BAD_DATE)
    {
      m_ymdhms_LastCalculated.Set(ymdhms_MDITime);
    }

    long lTimeDiff = ymdhms_MDITime - m_ymdhms_LastCalculated;
    if (lTimeDiff < m_CalcIntervalInSeconds) continue;

    //--------------------------------------------------
    // More than 2 hours since last calc, assume new day, reset surfaces
    //--------------------------------------------------
    if (lTimeDiff > 2*60*60)
    {
      m_VolSurfaces->ResetSurfaces(UNDERLYING_HSI);
      m_Logger->Write(Logger::NOTICE,"VolSurfCalculator: System time: %s. VolSurface for %s has been reset.", ymdhms_MDITime.ToStr().c_str(), UNDERLYING_HSI);
      m_ymdhms_LastCalculated.Set(ymdhms_MDITime);
    }

    if (!m_Exchg->IsTradingHour(HSI_CONT_FUT_1,ymdhms_MDITime))
    {
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time: %s. Non-trading hour.", ymdhms_MDITime.ToStr().c_str());
      continue;
    }

    //--------------------------------------------------
    // It's time to recalculate VolSurfaces!
    //--------------------------------------------------
    string s1FM_Fut  = m_HKFE->GetCtrtCode1(HKFE::FUTURES, ymdhms_MDITime.GetYYYYMMDD(),1);
    string s1FM_Call = m_HKFE->GetCtrtCode1(HKFE::CALL,    ymdhms_MDITime.GetYYYYMMDD(),1);
    string s1FM_Put  = m_HKFE->GetCtrtCode1(HKFE::PUT,     ymdhms_MDITime.GetYYYYMMDD(),1);
    string s2FM_Fut  = m_HKFE->GetCtrtCode1(HKFE::FUTURES, ymdhms_MDITime.GetYYYYMMDD(),2);
    string s2FM_Call = m_HKFE->GetCtrtCode1(HKFE::CALL,    ymdhms_MDITime.GetYYYYMMDD(),2);
    string s2FM_Put  = m_HKFE->GetCtrtCode1(HKFE::PUT,     ymdhms_MDITime.GetYYYYMMDD(),2);

    //--------------------------------------------------
    // Check the latest prices
    //--------------------------------------------------
    string sHSI_Fut_1FM = UNDERLYING_HSI+s1FM_Fut;
    double dHSI_Fut_1FM_BestBid = 0;
    double dHSI_Fut_1FM_BestAsk = 0;
    double dHSI_Fut_1FM_MidQuote = 0;
    YYYYMMDDHHMMSS ymdhms_HSI_Fut_1FM_UpdateTime;

    string sHSI_Fut_2FM = UNDERLYING_HSI+s2FM_Fut;
    double dHSI_Fut_2FM_BestBid = 0;
    double dHSI_Fut_2FM_BestAsk = 0;
    double dHSI_Fut_2FM_MidQuote = 0;
    YYYYMMDDHHMMSS ymdhms_HSI_Fut_2FM_UpdateTime;

    if (!m_MarketData->GetLatestBestBidAskMid(sHSI_Fut_1FM,dHSI_Fut_1FM_BestBid,dHSI_Fut_1FM_BestAsk,dHSI_Fut_1FM_MidQuote,ymdhms_HSI_Fut_1FM_UpdateTime))
    {
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time: %s. Latest price snapshot for %s not available.", ymdhms_MDITime.ToStr().c_str(), sHSI_Fut_1FM.c_str());
      continue;
    }

    if (!m_MarketData->GetLatestBestBidAskMid(sHSI_Fut_2FM,dHSI_Fut_2FM_BestBid,dHSI_Fut_2FM_BestAsk,dHSI_Fut_2FM_MidQuote,ymdhms_HSI_Fut_2FM_UpdateTime))
    {
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time: %s. Latest price snapshot for %s not available.", ymdhms_MDITime.ToStr().c_str(), sHSI_Fut_2FM.c_str());
      continue;
    }

    if (abs(ymdhms_HSI_Fut_1FM_UpdateTime - ymdhms_MDITime) > 10)
    {
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time: %s. Futures price not up-to-date.", ymdhms_MDITime.ToStr().c_str());
      continue;
    }

    if (abs(ymdhms_HSI_Fut_2FM_UpdateTime - ymdhms_MDITime) > 10)
    {
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time: %s. Futures price not up-to-date.", ymdhms_MDITime.ToStr().c_str());
      continue;
    }

    //--------------------------------------------------
    // Last update time of Futures is recent enough, proceed
    //--------------------------------------------------

    //--------------------------------------------------
    // HKMA risk free rate
    //--------------------------------------------------
    double dRiskFreeRate = 0;
    if (!m_HKMA->GetExchgFundBillAnnlzd(ymdhms_MDITime.GetYYYYMMDD(),dRiskFreeRate))
    {
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time: %s. HKMA exchange fund bills yield not available.", ymdhms_MDITime.ToStr().c_str());
      continue;
    }

    //--------------------------------------------------
    // Time to maturity
    //--------------------------------------------------
    double d_1FM_TTM = m_HKFE->GetTTM(ymdhms_MDITime,1);
    double d_2FM_TTM = m_HKFE->GetTTM(ymdhms_MDITime,2);
    int i_1FM_Expy_YYYYMMDD = m_HKFE->GetExpy(ymdhms_MDITime.GetYYYYMMDD(),1).ToInt();
    int i_2FM_Expy_YYYYMMDD = m_HKFE->GetExpy(ymdhms_MDITime.GetYYYYMMDD(),2).ToInt();

    //--------------------------------------------------
    // Get option prices from strike price 0.75 to 1.2
    //--------------------------------------------------
    long lStrikeLowerBound = floor(dHSI_Fut_1FM_MidQuote * 0.75 / 200) * 200;
    long lStrikeUpperBound = (long) (dHSI_Fut_1FM_MidQuote * 1.2);
    for (long lStrike = lStrikeLowerBound; lStrike <= lStrikeUpperBound; lStrike += 200)
    {

      string sHSI_Call_1FM = UNDERLYING_HSI+boost::lexical_cast<string>(lStrike)+s1FM_Call;
      double dHSI_Call_1FM_BestBid = 0;
      double dHSI_Call_1FM_BestAsk = 0;
      double dHSI_Call_1FM_MidQuote = 0;
      YYYYMMDDHHMMSS ymdhms_HSI_Call_1FM_UpdateTime;

      string sHSI_Put_1FM  = UNDERLYING_HSI+boost::lexical_cast<string>(lStrike)+s1FM_Put;
      double dHSI_Put_1FM_BestBid = 0;
      double dHSI_Put_1FM_BestAsk = 0;
      double dHSI_Put_1FM_MidQuote = 0;
      YYYYMMDDHHMMSS ymdhms_HSI_Put_1FM_UpdateTime;

      string sHSI_Call_2FM = UNDERLYING_HSI+boost::lexical_cast<string>(lStrike)+s2FM_Call;
      double dHSI_Call_2FM_BestBid = 0;
      double dHSI_Call_2FM_BestAsk = 0;
      double dHSI_Call_2FM_MidQuote = 0;
      YYYYMMDDHHMMSS ymdhms_HSI_Call_2FM_UpdateTime;

      string sHSI_Put_2FM  = UNDERLYING_HSI+boost::lexical_cast<string>(lStrike)+s2FM_Put;
      double dHSI_Put_2FM_BestBid = 0;
      double dHSI_Put_2FM_BestAsk = 0;
      double dHSI_Put_2FM_MidQuote = 0;
      YYYYMMDDHHMMSS ymdhms_HSI_Put_2FM_UpdateTime;

      bool bGetLatestCall1 = m_MarketData->GetLatestBestBidAskMid(sHSI_Call_1FM,dHSI_Call_1FM_BestBid,dHSI_Call_1FM_BestAsk,dHSI_Call_1FM_MidQuote,ymdhms_HSI_Call_1FM_UpdateTime);
      bool bGetLatestPut1  = m_MarketData->GetLatestBestBidAskMid(sHSI_Put_1FM,dHSI_Put_1FM_BestBid,dHSI_Put_1FM_BestAsk,dHSI_Put_1FM_MidQuote,ymdhms_HSI_Put_1FM_UpdateTime);
      bool bGetLatestCall2 = m_MarketData->GetLatestBestBidAskMid(sHSI_Call_2FM,dHSI_Call_2FM_BestBid,dHSI_Call_2FM_BestAsk,dHSI_Call_2FM_MidQuote,ymdhms_HSI_Call_2FM_UpdateTime);
      bool bGetLatestPut2  = m_MarketData->GetLatestBestBidAskMid(sHSI_Put_2FM,dHSI_Put_2FM_BestBid,dHSI_Put_2FM_BestAsk,dHSI_Put_2FM_MidQuote,ymdhms_HSI_Put_2FM_UpdateTime);

      //--------------------------------------------------
      // Can't get price even for 1FM
      //--------------------------------------------------
      if (!bGetLatestCall1 && !bGetLatestPut1)
      {
        m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: Strike = %d Futures = %d | Price not available.", ymdhms_MDITime.ToStr().c_str(), lStrike, (long)dHSI_Fut_1FM_MidQuote);
        continue;
      }

      //--------------------------------------------------
      // BASpread
      //--------------------------------------------------
      double dHSI_Call_1FM_BASpread = (dHSI_Call_1FM_BestAsk - dHSI_Call_1FM_BestBid);
      double dHSI_Put_1FM_BASpread  = (dHSI_Put_1FM_BestAsk  - dHSI_Put_1FM_BestBid);
      double dHSI_Call_2FM_BASpread = (dHSI_Call_2FM_BestAsk - dHSI_Call_2FM_BestBid);
      double dHSI_Put_2FM_BASpread  = (dHSI_Put_2FM_BestAsk  - dHSI_Put_2FM_BestBid);

      //--------------------------------------------------
      // BASpread: Come on. Can't be real
      //--------------------------------------------------
      if (dHSI_Call_1FM_BASpread < 0 || dHSI_Put_1FM_BASpread < 0)
      {
        m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: Strike = %d Futures = %d | Call [%d / %d] Put [%d / %d] Spread negative.",
            ymdhms_MDITime.ToStr().c_str(), lStrike, (long)dHSI_Fut_1FM_MidQuote, (int)dHSI_Call_1FM_BestBid, (int)dHSI_Call_1FM_BestAsk,
            (int)dHSI_Put_1FM_BestBid, (int)dHSI_Put_1FM_BestAsk);
        continue;
      }
      if (dHSI_Call_2FM_BASpread < 0 || dHSI_Put_2FM_BASpread < 0)
      {
        m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: Strike = %d Futures = %d | Call [%d / %d] Put [%d / %d] Spread negative.",
            ymdhms_MDITime.ToStr().c_str(), lStrike, (long)dHSI_Fut_2FM_MidQuote, (int)dHSI_Call_2FM_BestBid, (int)dHSI_Call_2FM_BestAsk,
            (int)dHSI_Put_2FM_BestBid, (int)dHSI_Put_2FM_BestAsk);
        continue;
      }

      //--------------------------------------------------
      // Choose valid option price
      // Choose OTM
      // Choose smaller BASpread
      //--------------------------------------------------
      bool b_1FM_UseCall = false;
      bool b_2FM_UseCall = false;
      if (bGetLatestCall1 && lStrike > dHSI_Fut_1FM_MidQuote) b_1FM_UseCall = true;
      if (bGetLatestCall2 && lStrike > dHSI_Fut_2FM_MidQuote) b_2FM_UseCall = true;

      //--------------------------------------------------
      // Is 2FM ATM?
      //--------------------------------------------------
      bool b_Is_2FM_ATM = abs(dHSI_Fut_2FM_MidQuote - lStrike) < 200;

      //--------------------------------------------------
      // BASpread too large (consider absolute no of points)
      //--------------------------------------------------
      if ((b_1FM_UseCall && dHSI_Call_1FM_BASpread > 5) ||
          (!b_1FM_UseCall && dHSI_Put_1FM_BASpread > 5))
      {
        m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: Strike = %d Futures = %d | Call [%d / %d] Put [%d / %d] Spread too large.",
            ymdhms_MDITime.ToStr().c_str(), lStrike, (long)dHSI_Fut_1FM_MidQuote, (int)dHSI_Call_1FM_BestBid, (int)dHSI_Call_1FM_BestAsk,
            (int)dHSI_Put_1FM_BestBid, (int)dHSI_Put_1FM_BestAsk);
        continue;
      }
      if (b_Is_2FM_ATM) //ATM
      {
        if ((b_2FM_UseCall && dHSI_Call_2FM_BASpread > 10) ||
            (!b_2FM_UseCall && dHSI_Put_2FM_BASpread > 10))
        {
          m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: Strike = %d Futures = %d | Call [%d / %d] Put [%d / %d] Spread too large.",
              ymdhms_MDITime.ToStr().c_str(), lStrike, (long)dHSI_Fut_2FM_MidQuote, (int)dHSI_Call_2FM_BestBid, (int)dHSI_Call_2FM_BestAsk,
              (int)dHSI_Put_2FM_BestBid, (int)dHSI_Put_2FM_BestAsk);
          continue;
        }
      }

      //--------------------------------------------------
      // Ignore 0 prices
      //--------------------------------------------------
      if ((b_1FM_UseCall && dHSI_Call_1FM_MidQuote < 1) ||
          (!b_1FM_UseCall && dHSI_Put_1FM_MidQuote < 1))
      {
        m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: Strike = %d Futures = %d | Call [%d / %d] Put [%d / %d] Ignore 0 prices.",
            ymdhms_MDITime.ToStr().c_str(), lStrike, (long)dHSI_Fut_1FM_MidQuote, (int)dHSI_Call_1FM_BestBid, (int)dHSI_Call_1FM_BestAsk,
            (int)dHSI_Put_1FM_BestBid, (int)dHSI_Put_1FM_BestAsk);
        continue;
      }
      if (b_Is_2FM_ATM) // ATM
      {
        if ((b_2FM_UseCall && dHSI_Call_2FM_MidQuote < 1) ||
            (!b_2FM_UseCall && dHSI_Put_2FM_MidQuote < 1))
        {
          m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: Strike = %d Futures = %d | Call [%d / %d] Put [%d / %d] Ignore 0 prices.",
              ymdhms_MDITime.ToStr().c_str(), lStrike, (long)dHSI_Fut_2FM_MidQuote, (int)dHSI_Call_2FM_BestBid, (int)dHSI_Call_2FM_BestAsk,
              (int)dHSI_Put_2FM_BestBid, (int)dHSI_Put_2FM_BestAsk);
          continue;
        }
      }

      //--------------------------------------------------
      // Get all parameters needed by Black Scholes
      //--------------------------------------------------
      double dPrevIV_1FM = m_VolSurfaces->GetIVByStrike(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, lStrike);
      double dPrevIV_2FM = m_VolSurfaces->GetIVByStrike(UNDERLYING_HSI, i_2FM_Expy_YYYYMMDD, lStrike);

      double d_1FM_IV = -1;
      double d_2FM_IV = -1;
      if (dPrevIV_1FM <= 0)
      {
        if (b_1FM_UseCall) d_1FM_IV = BlackScholes::ImpldVolatilityFut(true,  dHSI_Fut_1FM_MidQuote, lStrike, dRiskFreeRate, d_1FM_TTM, dHSI_Call_1FM_MidQuote);
        else               d_1FM_IV = BlackScholes::ImpldVolatilityFut(false, dHSI_Fut_1FM_MidQuote, lStrike, dRiskFreeRate, d_1FM_TTM, dHSI_Put_1FM_MidQuote);
      }
      else
      {
        if (b_1FM_UseCall) d_1FM_IV = BlackScholes::ImpldVolatilityFut(true,  dHSI_Fut_1FM_MidQuote, lStrike, dRiskFreeRate, d_1FM_TTM, dHSI_Call_1FM_MidQuote, dPrevIV_1FM);
        else               d_1FM_IV = BlackScholes::ImpldVolatilityFut(false, dHSI_Fut_1FM_MidQuote, lStrike, dRiskFreeRate, d_1FM_TTM, dHSI_Put_1FM_MidQuote,  dPrevIV_1FM);
      }
      if (b_Is_2FM_ATM) // ATM
      {
        if (dPrevIV_2FM <= 0)
        {
          if (b_2FM_UseCall) d_2FM_IV = BlackScholes::ImpldVolatilityFut(true,  dHSI_Fut_2FM_MidQuote, lStrike, dRiskFreeRate, d_2FM_TTM, dHSI_Call_2FM_MidQuote);
          else               d_2FM_IV = BlackScholes::ImpldVolatilityFut(false, dHSI_Fut_2FM_MidQuote, lStrike, dRiskFreeRate, d_2FM_TTM, dHSI_Put_2FM_MidQuote);
        }
        else
        {
          if (b_2FM_UseCall) d_2FM_IV = BlackScholes::ImpldVolatilityFut(true,  dHSI_Fut_2FM_MidQuote, lStrike, dRiskFreeRate, d_2FM_TTM, dHSI_Call_2FM_MidQuote, dPrevIV_2FM);
          else               d_2FM_IV = BlackScholes::ImpldVolatilityFut(false, dHSI_Fut_2FM_MidQuote, lStrike, dRiskFreeRate, d_2FM_TTM, dHSI_Put_2FM_MidQuote,  dPrevIV_2FM);
        }
      }

      m_VolSurfaces->UpdateUndlIVByStrike(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, lStrike, dHSI_Fut_1FM_MidQuote, d_1FM_IV);
      m_VolSurfaces->UpdateUndlIVByStrike(UNDERLYING_HSI, i_2FM_Expy_YYYYMMDD, lStrike, dHSI_Fut_2FM_MidQuote, d_2FM_IV);

      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: Strike = %d Futures = %d | Call [%d / %d] Put [%d / %d] Using %s. PrevIV = %f, IV = %f.",
          ymdhms_MDITime.ToStr().c_str(), lStrike, (long)dHSI_Fut_1FM_MidQuote, (int)dHSI_Call_1FM_BestBid, (int)dHSI_Call_1FM_BestAsk,
          (int)dHSI_Put_1FM_BestBid, (int)dHSI_Put_1FM_BestAsk, (b_1FM_UseCall ? "call" : "put"), dPrevIV_1FM, d_1FM_IV);

      if (b_Is_2FM_ATM)
      {
        m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: Strike = %d Futures = %d | Call [%d / %d] Put [%d / %d] Using %s. PrevIV = %f, IV = %f.",
            ymdhms_MDITime.ToStr().c_str(), lStrike, (long)dHSI_Fut_2FM_MidQuote, (int)dHSI_Call_2FM_BestBid, (int)dHSI_Call_2FM_BestAsk,
            (int)dHSI_Put_2FM_BestBid, (int)dHSI_Put_2FM_BestAsk, (b_2FM_UseCall ? "call" : "put"), dPrevIV_2FM, d_2FM_IV);
      }

    }

    //--------------------------------------------------
    // Consider volatility smile calculated
    //--------------------------------------------------
    m_ymdhms_LastCalculated.Set(ymdhms_MDITime);

    //--------------------------------------------------
    // Calculate our own VHSI
    //--------------------------------------------------
    double d_1FM_ATM_Strike = STool::Round(dHSI_Fut_1FM_MidQuote/200,0)*200;
    double d_2FM_ATM_Strike = STool::Round(dHSI_Fut_2FM_MidQuote/200,0)*200;

    string sHSI_Call_1FM = UNDERLYING_HSI+boost::lexical_cast<string>(d_1FM_ATM_Strike)+s1FM_Call;
    string sHSI_Call_2FM = UNDERLYING_HSI+boost::lexical_cast<string>(d_2FM_ATM_Strike)+s2FM_Call;
    string sHSI_Put_1FM  = UNDERLYING_HSI+boost::lexical_cast<string>(d_1FM_ATM_Strike)+s1FM_Put;
    string sHSI_Put_2FM  = UNDERLYING_HSI+boost::lexical_cast<string>(d_2FM_ATM_Strike)+s2FM_Put;

    double dHSI_Call_1FM_MidQuote = 0;
    double dHSI_Call_2FM_MidQuote = 0;
    double dHSI_Put_1FM_MidQuote  = 0;
    double dHSI_Put_2FM_MidQuote  = 0;

    m_MarketData->GetLatestMidQuote(sHSI_Call_1FM,dHSI_Call_1FM_MidQuote);
    m_MarketData->GetLatestMidQuote(sHSI_Put_1FM,dHSI_Put_1FM_MidQuote);
    m_MarketData->GetLatestMidQuote(sHSI_Call_2FM,dHSI_Call_2FM_MidQuote);
    m_MarketData->GetLatestMidQuote(sHSI_Put_2FM,dHSI_Put_2FM_MidQuote);

    double dIV_1FM = m_VolSurfaces->GetIVByStrike(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, d_1FM_ATM_Strike);
    double dIV_2FM = m_VolSurfaces->GetIVByStrike(UNDERLYING_HSI, i_2FM_Expy_YYYYMMDD, d_2FM_ATM_Strike);
    if (dIV_1FM > 0 && dIV_2FM > 0)
    {
      double dVHSI =
        sqrt(
            12 *
            (
             d_1FM_TTM * pow(dIV_1FM,2) * (d_2FM_TTM - (double)22/HKFE::_TRDG_DAY_PER_YR) / (d_2FM_TTM - d_1FM_TTM) +
             d_2FM_TTM * pow(dIV_2FM,2) * ((double)22/HKFE::_TRDG_DAY_PER_YR - d_1FM_TTM) / (d_2FM_TTM - d_1FM_TTM)
            )
            );

      m_VolSurfaces->UpdateNirVHSI(ymdhms_MDITime,dVHSI);

      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: ATM K = %f\t%f\tTTM =\t%f\t%f\tExp Date =\t%d\t%d\tCall Price =\t%f\t%f\tPut Price =\t%f\t%f\tATM IV =\t%f\t%f\tVHSI =\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f",
          ymdhms_MDITime.ToStr().c_str(),
          d_1FM_ATM_Strike, d_2FM_ATM_Strike,
          d_1FM_TTM, d_2FM_TTM,
          i_1FM_Expy_YYYYMMDD, i_2FM_Expy_YYYYMMDD,
          dHSI_Call_1FM_MidQuote, dHSI_Call_2FM_MidQuote,
          dHSI_Put_1FM_MidQuote, dHSI_Put_2FM_MidQuote,
          dIV_1FM, dIV_2FM,
          dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());

      //--------------------------------------------------
      // Get realized volatility from GKYZ
      //--------------------------------------------------
      GKYZ gkyz = m_TechInd->GetGKYZClone(HSI_CONT_FUT_1);

      YYYYMMDD yyyymmdd;
      HHMMSS   hhmmss;
      double   dOpen;
      double   dHigh;
      double   dLow;
      double   dClose;
      long     lVolume;
      m_MarketData->Peek1DayOHLCBar(HSI_CONT_FUT_1,yyyymmdd,hhmmss,dOpen,dHigh,dLow,dClose,lVolume);
      gkyz.Add(dOpen,dHigh,dLow,dClose,lVolume);
      double dGKYZ = gkyz.Value();

      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. GKYZ not ready yet.", ymdhms_MDITime.ToStr().c_str());
      if (dGKYZ <= 0) continue;

      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: GKYZ = %f", ymdhms_MDITime.ToStr().c_str(), dGKYZ);
      //--------------------------------------------------

      //--------------------------------------------------
      // If too close to maturity, no need to calibrate
      //--------------------------------------------------
      int iDTM = m_HKFE->GetDTM(ymdhms_MDITime.GetYYYYMMDD(),1);
      if (iDTM <= 5)
      {
        m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: DTM = %d. Too close to maturity. No need to calibrate", ymdhms_MDITime.ToStr().c_str(), iDTM);
        continue;
      }

      //--------------------------------------------------
      // 1Day Hist Return of VHSI
      //--------------------------------------------------
      if (m_VolSurfaces->GetNirVHSI1DayHistReturn() < -1)
      {
        m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: %s: VHSI = %f. 1 Day VHSI return not ready yet.", ymdhms_MDITime.ToStr().c_str(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
        continue;
      }

      //--------------------------------------------------
      // For the calibration of volatility smile prediction formula
      //--------------------------------------------------
      m_VolSurfaces->FitVolSmile(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD);
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 1.3 ,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 1.3 , m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 1.3 ), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 1.25,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 1.25, m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 1.25), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 1.2 ,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 1.2 , m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 1.2 ), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 1.15,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 1.15, m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 1.15), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 1.1 ,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 1.1 , m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 1.1 ), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 1.05,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 1.05, m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 1.05), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 1.0 ,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 1.0 , m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 1.0 ), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 0.95,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 0.95, m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 0.95), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 0.9 ,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 0.9 , m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 0.9 ), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());
      m_Logger->Write(Logger::DEBUG,"VolSurfCalculator: System time = %s. Moneyness=\t%f\tEquiv Strike =\t%f\tIV =\t%f\tGKYZ =\t%f\tr =\t%f\ttau =\t%f\tFut_1FM =\t%f\tVHSI=\t%f\t3D_SMA_VHSI =\t%f\t1D_Hist_Rtn_VHSI =\t%f", 0.85,  ymdhms_MDITime.ToStr().c_str(), dHSI_Fut_1FM_MidQuote / 0.85, m_VolSurfaces->GetIVByMoneyness(UNDERLYING_HSI, i_1FM_Expy_YYYYMMDD, 0.85), dGKYZ, dRiskFreeRate, d_1FM_TTM, dHSI_Fut_1FM_MidQuote, dVHSI, m_VolSurfaces->GetNirVHSI3DaySma(), m_VolSurfaces->GetNirVHSI1DayHistReturn());

    }

    m_VolSurfaces->NotifyConsumers();

  }

  m_Logger->Write(Logger::NOTICE,"VolSurfCalculator has ended.");
  sleep(2);

  return;
}

