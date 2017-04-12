#ifndef PORTFOLIOSTRATEGYS11A_H_
#define PORTFOLIOSTRATEGYS11A_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/ptr_container/ptr_deque.hpp>
#include <deque>
#include "SystemState.h"
#include "SystemConfig.h"
#include "MDI_Acknowledgement.h"
#include "MarketData.h"
#include "HKFE.h"
#include "TechIndicators.h"
#include "PortfoliosAndOrders.h"
#include "TradeWarehouse.h"
#include "GumbelCopula.h"
#include "CauchyDistribution.h"
#include "GaussianDistribution.h"

#define MIN_HOLDINGPERIOD_SEC 60
#define AVOID_TRADING_NEAR_MKTOPEN_SEC  240
#define AVOID_TRADING_NEAR_MKTCLOSE_SEC 120
#define EXTRABUFFER_OPENTRADE_SEC 120


class StrategyS11A {
  public:
    enum TRADEDIR {NODIR=0,LONG=1,SHORT=2};
    enum S11A_SubSystem
    {
      S11A_T1GBM          =0,
      S11A_T1GUMBELCAUCHY =1,
      S11A_T1GUMBELGAUSS  =2,
      S11A_T2GBM          =3,
      S11A_T2PCTL         =4,
      S11A_T2COPU         =5,
      S11A_LAST           =6
    };
    StrategyS11A();
    virtual ~StrategyS11A();
    // void Run();

  private:
    long m_CalcIntervalInSeconds;
    string GetSubSystemName(const S11A_SubSystem);
    void ReportAckIfNeeded();
    void InitFromSysConfig();
    bool GetLegPrices();
    void GetSDCorrel();
    void OutputStratParam(const S11A_SubSystem, const int);
    TRADEDIR OpenTrade(const S11A_SubSystem, const int);
    void CloseTrade(const S11A_SubSystem, const TRADEDIR, const int);
    void TradeSynchronizedOrders();
    void CalcNumCtrt();
    void CalcLnRtnGBMParam(const int);

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<MarketData>           m_MarketData;
    boost::shared_ptr<SystemState>          m_SystemState;
    boost::shared_ptr<SystemConfig>         m_SysCfg;
    boost::shared_ptr<Logger>               m_Logger;
    boost::shared_ptr<TechIndicators>       m_TechInd;
    boost::shared_ptr<PortfoliosAndOrders>  m_PortAndOrders;
    boost::shared_ptr<HKFE>                 m_HKFE;
    boost::shared_ptr<MDI_Acknowledgement>  m_MDIAck;

    //--------------------------------------------------
    // Store param file data
    //--------------------------------------------------
    map<YYYYMMDD,double>    m_map_HSI_IV;
    map<YYYYMMDD,double>    m_map_HHI_IV;
    map<YYYYMMDD,double>    m_map_HSI_HistSD;
    map<YYYYMMDD,double>    m_map_HHI_HistSD;
    map<YYYYMMDD,double>    m_map_HSI_HHI_Correl;
    // map<YYYYMMDD,double>    m_map_T2_05Pctl;
    // map<YYYYMMDD,double>    m_map_T2_50Pctl;
    // map<YYYYMMDD,double>    m_map_T2_95Pctl;

    map<YYYYMMDD,double>    m_map_T1_GumbelCauchyTheta;
    map<YYYYMMDD,double>    m_map_T1_HSI_CauchyLocation;
    map<YYYYMMDD,double>    m_map_T1_HSI_CauchyScale;
    map<YYYYMMDD,double>    m_map_T1_HHI_CauchyLocation;
    map<YYYYMMDD,double>    m_map_T1_HHI_CauchyScale;

    map<YYYYMMDD,double>    m_map_T1_GumbelGaussTheta;
    map<YYYYMMDD,double>    m_map_T1_HSI_LnRtnGaussMean;
    map<YYYYMMDD,double>    m_map_T1_HSI_LnRtnGaussSD;
    map<YYYYMMDD,double>    m_map_T1_HHI_LnRtnGaussMean;
    map<YYYYMMDD,double>    m_map_T1_HHI_LnRtnGaussSD;

    string                  m_ParamFileHistSDCorrel;
    string                  m_ParamFileHSIHHIIV;
    string                  m_ParamFileT1GumbelCauchy;
    string                  m_ParamFileT1GumbelGauss;
    string                  m_ParamFileT2Copula;

    //--------------------------------------------------

    bool m_On_S11A_T1GBM;
    bool m_On_S11A_T1GUMBELCAUCHY;
    bool m_On_S11A_T1GUMBELGAUSS;
    bool m_On_S11A_T2GBM;
    bool m_On_S11A_T2PCTL;
    bool m_On_S11A_T2COPU;


    double dNotionalValuePerLeg;
    double dT1GBMTriggerThreshold;
    double dT1GBMTakeProfitThreshold;
    double dT1GBMStopLossThreshold;
    double dT2GBMTriggerThreshold;
    double dT2GBMTakeProfitThreshold;
    double dT2GBMStopLossThreshold;
    double dT1GumbelCauchyTriggerThreshold;
    double dT1GumbelCauchyTakeProfitThreshold;
    double dT1GumbelCauchyStopLossThreshold;
    double dT1GumbelCauchyPeriod;
    double dT1GumbelCauchyEOLPeriod;
    double dT1GumbelCauchyThetaMean;
    double dT1GumbelCauchyThetaMeanReversion;
    double dT1GumbelCauchyMarginalSDScaling;

    double dT1GumbelGaussTriggerThreshold;
    double dT1GumbelGaussTakeProfitThreshold;
    double dT1GumbelGaussStopLossThreshold;
    double dT1GumbelGaussPeriod;
    double dT1GumbelGaussEOLPeriod;
    double dT1GumbelGaussThetaMean;
    double dT1GumbelGaussThetaMeanReversion;
    double dT1GumbelGaussMarginalSDScaling;
    double dT1GumbelGaussSigalLimit;

    double dT2CopuTriggerThreshold;
    double dT2CopuTakeProfitThreshold;
    double dT2CopuStopLossThreshold;
    vector<long> m_T2_MAPeriods;

    double dT2PctlPeriod;
    double dT2CopuPeriod;

    vector<long> m_T1GBM_DeltaTPeriods;
    vector<long> m_T2GBM_DeltaTPeriods;

    YYYYMMDDHHMMSS m_ymdhms_LastMonitorTime;
    map<string,double> m_SynchronizedTrades;

    double dHSIPrice;
    double dHHIPrice;
    double dHSIHistSD;
    double dHHIHistSD;
    double dHSIIV;
    double dHHIIV;
    double dHSIHistVar;
    double dHHIHistVar;
    double dHSIIVar;
    double dHHIIVar;
    double dHSIHHICorrel;

    int iLastSysYYYYMMDD;
    string sHSI_Fut;
    string sHHI_Fut;
    string sMHI_Fut;
    string sMCH_Fut;
    string sHSI_Fut_DTM_GT_1;
    string sHHI_Fut_DTM_GT_1;
    string sMHI_Fut_DTM_GT_1;
    string sMCH_Fut_DTM_GT_1;
    string sHSI_Fut_DTM_GT_10;
    string sHHI_Fut_DTM_GT_10;
    string sMHI_Fut_DTM_GT_10;
    string sMCH_Fut_DTM_GT_10;
    YYYYMMDD ymdExpDate;
    YYYYMMDD ymdExpDate_DTM_GT_1;
    YYYYMMDD ymdExpDate_DTM_GT_10;

    TradeWarehouse   m_TradeWarehouse[(int)S11A_LAST];
    YYYYMMDDHHMMSS   ymdhms_SysTime;
    YYYYMMDDHHMMSS   ymdhms_TmpOpenTradeTime;
    HHMMSS           m_StartTime;
    HHMMSS           m_EndTime;
    bool             m_bFirstTrade;


    //S11A_T1GBM
    //S11A_T1GUMBELCAUCHY / S11A_T1GUMBELGAUSS / S11A_T2COPU
    double dEDeltaGamma,dSDDeltaGamma,dHSILnRtn,dHHILnRtn,dLnRtnDiff;
    double dPrevLnHSIHHIRatio;

    //S11A_T2GBM / S11A_T2PCTL
    double dDeltaT,dEGamma,dSDGamma,dCurLnHSIHHIRatio,dLongerTermLnHSIHHIRatio;

    //--------------------------------------------------
    double dHHILegSize;
    double dHSILegSize_1;
    int iNoOfCtrtHSI_1;
    int iNoOfCtrtMHI_1;
    int iNoOfCtrtHHI_1;
    int iNoOfCtrtMCH_1;

    double dHSILegSize_2;
    int iNoOfCtrtHSI_2;
    int iNoOfCtrtMHI_2;
    int iNoOfCtrtHHI_2;
    int iNoOfCtrtMCH_2;

    int iNoOfCtrtHSI;
    int iNoOfCtrtMHI;
    int iNoOfCtrtHHI;
    int iNoOfCtrtMCH;

};

#endif
