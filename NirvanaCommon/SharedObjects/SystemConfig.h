#ifndef SYSTEMCONFIG_HPP_
#define SYSTEMCONFIG_HPP_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/algorithm/string.hpp>

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "Logger.h"
#include "SDateTime.h"

using namespace std;
using namespace boost;

class OTIMDIConfig
{
  public:
    OTIMDIConfig();

    string                 m_MDI_File;
    string                 m_MDI_IP;
    string                 m_MDI_Port;
    string                 m_OTI_IP;
    string                 m_OTI_Port;
    vector<string>         m_MDISubscriptionSymbols;
    string                 m_MDISubscriptionStartDate;
    string                 m_MDISubscriptionEndDate;
    string                 m_MDISubscriptionReplaySpeed;
};

class TradingStrategyConfig
{
  public:
    enum TrainingFreq {Auto=0,Daily,Weekly,Monthly,Yearly,OnceAtInitWarmup};
    TradingStrategyConfig();

    bool                      m_OnOff;
    vector<double>            m_ParamVector;
    string                    m_SignalLogPath;
    int                       m_StartTimeBufferInSec;
    int                       m_EndTimeBufferInSec;
    int                       m_BreakTimeBufferInSec;
    set<HHMMSS>               m_SpecialTime;
    HHMMSS                    m_MTMTime;
    vector<string>            m_TradedSymbols;
    map<string,set<string> >  m_SynchronizedSymbols;
    int                       m_SynchronizedSymbolsSec;
    bool                      m_TreatRegularMiniAsOne;
    int                       m_MonitoringIntervalInSec;
    bool                      m_WhetherToRetrain;
    TrainingFreq              m_TrainingFreq;
    bool                      m_CPnLHistPersistenceIsEnabled;
    string                    m_CPnLHistPersistenceFile;
};

class SystemConfig : boost::noncopyable
{
  public:
    enum MktDataTradeVolumeMode {PER_TRADE_VOLUME_MODE=0, ACCUMULATED_VOLUME_MODE=1};
    enum OrderRoutingMode {ORDER_ROUTE_RECORD=0, ORDER_ROUTE_OTI=1, ORDER_ROUTE_NEXTTIERZMQ=2, ORDER_ROUTE_OTINXTIERZMQ=3};
    enum TCPOrEmbeddedMode {TCPWITHOUTACK=0, TCPWITHACK=1, EMBEDDED=2};

    static boost::shared_ptr<SystemConfig> Instance();
    void SetConfigPath(const string &);
    ~SystemConfig(){};
    void Reset();
    void ReadConfig(const string &);
    void SetProgramStartTime();
    boost::posix_time::ptime GetProgramStartTime() const;

    SDateTime::TIMEZONE    GetDefaultTimeZone()                  const;
    const unsigned short   GetSystemServicePort()                const;
    string                 Get_Main_Log_Path()                   const;
    string                 Get_MTM_Log_Path()                    const;
    string                 Get_Exec_Log_Path()                   const;
    string                 Get_Holdings_Log_Path()               const;
    string                 Get_SupplementaryBarD1Path()          const;
    string                 Get_SupplementaryBarM1Path()          const;
    bool                   Get_SupplementaryBarLeadingZeroAdj()  const;
    bool                   Get_HKIntradaySeasonalityOnOff()      const;
    string                 Get_HKIntradaySeasonalityPath()       const;
    int                    Get_HKIntradaySeasonalityWindow()     const;
    string                 Get_PositionPersistenceFile()         const;
    bool                   PositionPersistenceIsEnabled()        const;
    string                 Get_S11AParamFileHistSDCorrel()       const;
    string                 Get_S11AParamFileHSIHHIIV()           const;
    string                 Get_S11AParamFileT1GumbelCauchy()     const;
    string                 Get_S11AParamFileT1GumbelGauss()      const;
    string                 Get_S11AParamFileT2Copula()           const;
    bool                   SendResetOnConnectionToCAPI()         const;
    int                    GetNumOfMDI()                         const;
    int                    GetNumOfOTI()                         const;

    HHMMSS                 Get_HKFEContFutRollFwdTime()       const;
    HHMMSS                 Get_CMEContFutRollFwdTime()        const;

    int                    Get_B1_HKF_SmplgIntvlInSec()       const;
    long                   Get_VolSurfCalcIntervalInSec()     const;
    long                   Get_ProbDistrnCalcIntervalInSec()  const;
    long                   Get_TechIndUpdateIntervalInSec()   const;
    string                 Get_TradingHoursPath()             const;
    string                 Get_HKFE_CalendarPath()            const;
    string                 Get_HKMA_ExchgFundBillPath()       const;
    string                 Get_HKSE_HSIConstituentsPath()     const;
    string                 Get_CorrelMatricesPath()           const;
    long                   Get_GKYZ_Window_Size()             const;
    MktDataTradeVolumeMode Get_MktDataTradeVolumeMode()       const;
    string                 Get_VolSurfParamFile1FM()          const;
    string                 Get_VolSurfParamFile2FM()          const;
    string                 Get_ProbDistrFileFSMC1D()          const;

    OrderRoutingMode       Get_OrderRoutingMode()             const;
    string                 Get_NextTier_ZMQ_MD_IP_Port()      const;
    string                 Get_NextTier_ZMQ_TF_IP_Port()      const;
    TCPOrEmbeddedMode      Get_TCPOrEmbeddedMode()            const;

    bool                   Get_OnOff_S11A_T1GBM()             const;
    bool                   Get_OnOff_S11A_T1GUMBELCAUCHY()    const;
    bool                   Get_OnOff_S11A_T1GUMBELGAUSS()     const;
    bool                   Get_OnOff_S11A_T2GBM()             const;
    bool                   Get_OnOff_S11A_T2PCTL()            const;
    bool                   Get_OnOff_S11A_T2COPU()            const;

    vector<long>           Get_S11AT1GBMDeltaTPeriods()                                        const;
    vector<long>           Get_S11AT2MAPeriods()                                               const;
    vector<long>           Get_S11AT2GBMDeltaTPeriods()                                        const;
    double                 Get_S11AT1GBMTriggerThreshold()                                     const;
    double                 Get_S11AT2GBMTriggerThreshold()                                     const;
    double                 Get_S11AT1GumbelCauchyTriggerThreshold()                            const;
    double                 Get_S11AT1GumbelGaussTriggerThreshold()                             const;
    double                 Get_S11AT2CopuTriggerThreshold()                                    const;
    double                 Get_S11AT1GBMTakeProfitThreshold()                                  const;
    double                 Get_S11AT2GBMTakeProfitThreshold()                                  const;
    double                 Get_S11AT1GumbelCauchyTakeProfitThreshold()                         const;
    double                 Get_S11AT1GumbelGaussTakeProfitThreshold()                          const;
    double                 Get_S11AT2CopuTakeProfitThreshold()                                 const;
    double                 Get_S11AT1GBMStopLossThreshold()                                    const;
    double                 Get_S11AT2GBMStopLossThreshold()                                    const;
    double                 Get_S11AT1GumbelCauchyStopLossThreshold()                           const;
    double                 Get_S11AT1GumbelGaussStopLossThreshold()                            const;
    double                 Get_S11AT2CopuStopLossThreshold()                                   const;
    double                 Get_S11AT1GumbelCauchyEOLPeriod()                                   const;
    double                 Get_S11AT1GumbelGaussEOLPeriod()                                    const;
    double                 Get_S11ANotionalValuePerLeg()                                       const;
    double                 Get_S11AT1GumbelCauchyPeriod()                                      const;
    double                 Get_S11AT1GumbelGaussPeriod()                                       const;
    double                 Get_S11AT2PctlPeriod()                                              const;
    double                 Get_S11AT2CopuPeriod()                                              const;
    double                 Get_S11AT1GumbelCauchyThetaMean()                                   const;
    double                 Get_S11AT1GumbelGaussThetaMean()                                    const;
    double                 Get_S11AT1GumbelCauchyThetaMeanReversion()                          const;
    double                 Get_S11AT1GumbelGaussThetaMeanReversion()                           const;
    double                 Get_S11AT1GumbelCauchyMarginalSDScaling()                           const;
    double                 Get_S11AT1GumbelGaussMarginalSDScaling()                            const;
    double                 Get_S11AMonitoringIntervalInSec()                                   const;
    double                 Get_S11AT1GumbelGaussSignalLimit()                                  const;
    bool                   B2_HasEnabledMinCommissionCheck(const StrategyID)                   const;
    int                    B2_HasEnabledRotationMode(const StrategyID)                         const;
    string                 B2_TheoCPnLHistFolder(const StrategyID)                             const;
    string                 B2_TheoPosFolder(const StrategyID)                                  const;
    bool                   B2_PersistTheoPosCPnL(const StrategyID)                             const;
    vector<int>            Get_B2_RotationGroup(const StrategyID)                              const;
    int                    Get_B2_ChooseBestNRotationGroup(const StrategyID)                   const;
    int                    Get_B2_ActionTimeBefCloseInSec(const StrategyID)                    const;
    vector<int>            Get_B2_FilterSMAPeriod(const StrategyID)                            const;
    bool                   Get_B2_MoveNextBestUpIfNoSignal(const StrategyID)                   const;
    bool                   CheckIfHaltTrading(const string &,const YYYYMMDD &)                 const;
    bool                   CheckIfHalfDayTrading(const string &,const YYYYMMDD &)              const;
    bool                   GetCorpActionAdj(const string &,const YYYYMMDD &,double &,double &) const;
    int                    GetLotSize(const string &)                                          const;
    long                   RoundDownLotSize(const string &,const long)                         const;
    bool                   IsPriceFwdrToNextTierOn()                                           const;
    int                    GetPriceFwdrToNextTierIntervalInSec()                               const;


    //--------------------------------------------------
    // Convenience for Trading System config
    //--------------------------------------------------
    string                 GetSignalLogPath(const StrategyID);
    HHMMSS                 GetStrategyMTMTime(const StrategyID);
    vector<double>         GetParamVector(const StrategyID);
    vector<string>         GetTradedSymbols(const StrategyID);
    set<string>            GetSynchronizedSymbols(const StrategyID,const string &);
    int                    GetSynchronizedSymbolsSec(const StrategyID);
    bool                   ChkIfTreatRegularMiniAsOne(const StrategyID);
    int                    GetStrategyStartTimeBufferInSec(const StrategyID);
    int                    GetStrategyEndTimeBufferInSec(const StrategyID);
    int                    GetStrategyBreakTimeBufferInSec(const StrategyID);
    set<HHMMSS>            GetStrategySpecialTime(const StrategyID);
    bool                   IsStrategyOn(const StrategyID);
    int                    GetMonitoringIntervalInSec(const StrategyID);
    bool                   GetWhetherToRetrain(const StrategyID);
    TradingStrategyConfig::TrainingFreq GetTrainingFreq(const StrategyID);
    string                 Get_CPnLHistPersistenceFile(const StrategyID);
    bool                   IsCPnLHistPersistenceEnabled(const StrategyID);

    //--------------------------------------------------
    // Convenience for Trading System config
    //--------------------------------------------------
    string                 Get_MDI_File(const int);
    string                 Get_MDI_IP(const int);
    string                 Get_MDI_Port(const int);
    string                 Get_OTI_IP(const int);
    string                 Get_OTI_Port(const int);
    const vector<string>   GetMDISubscriptionSymbols(const int);
    const string           GetMDISubscriptionStartDate(const int);
    const string           GetMDISubscriptionEndDate(const int);
    const string           GetMDISubscriptionReplaySpeed(const int);

    bool CheckIfBarAggregationM1Symbol(const string &) const;
    bool CheckIfBarAggregationD1Symbol(const string &) const;
    double GetErroneousTickPxChg() const;
    bool CheckIfSymToBeWarmedUpWithDayBars(const string &) const;
    bool ChkIfProceedStyForceExcnEvenIfNoMD(const string &) const;

  private:
    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    SystemConfig();
    SystemConfig(SystemConfig const&);
    SystemConfig& operator=(SystemConfig const&){};
    static boost::weak_ptr<SystemConfig> m_pInstance;

    //--------------------------------------------------
    boost::shared_ptr<Logger> m_Logger;
    string m_ConfigPath;

    //--------------------------------------------------
    typedef struct {
      double    m_price_delta;
      double    m_price_ratio;
    } tuple_corp_action_adj;

    //--------------------------------------------------
    // Config.ini fields
    //--------------------------------------------------
    SDateTime::TIMEZONE                                m_DefaultTimeZone;
    boost::posix_time::ptime                           m_ProgramStartTime;
    unsigned short                                     m_SystemServicePort;
    string                                             m_MainLogPath;
    string                                             m_MTMLogPath;
    string                                             m_ExecLogPath;
    string                                             m_MTMHoldingsLogPath;
    string                                             m_SupplementaryBarD1Path;
    string                                             m_SupplementaryBarM1Path;
    bool                                               m_SupplementaryBarLeadingZeroAdj;
    bool                                               m_HKIntradaySeasonalityOnOff;
    string                                             m_HKIntradaySeasonalityPath;
    int                                                m_HKIntradaySeasonalityWindow;
    string                                             m_PositionPersistenceFile;
    bool                                               m_PositionPersistenceIsEnabled;
    string                                             m_S11A_ParamFileHistSDCorrel;
    string                                             m_S11A_ParamFileHSIHHIIV;
    string                                             m_S11A_ParamFileT1GumbelCauchy;
    string                                             m_S11A_ParamFileT1GumbelGauss;
    string                                             m_S11A_ParamFileT2Copula;
    long                                               m_VolSurfCalcIntervalInSec;
    long                                               m_ProbDistrnCalcIntervalInSec;
    long                                               m_TechIndUpdateIntervalInSec;
    string                                             m_TradingHoursPath;
    string                                             m_HKFE_CalendarPath;
    string                                             m_HKSE_HSIConstituentsPath;
    string                                             m_HKMA_ExchgFundBillPath;
    string                                             m_CorrelMatricesPath;
    string                                             m_HaltTradingPath;
    map<string,set<YYYYMMDD> >                         m_HaltTradingSymDate;
    string                                             m_HalfDayTradingPath;
    map<string,set<YYYYMMDD> >                         m_HalfDayTradingSymDate;
    string                                             m_CorpActionAdjPath;
    map<string,map<YYYYMMDD,tuple_corp_action_adj> >   m_CorpActionAdjDetails;
    string                                             m_LotSizePath;
    map<string,int>                                    m_map_LotSize;
    bool                                               m_PriceFwdrIsOn;
    int                                                m_PriceFwdrIntervalInSec;
    set<string>                                        m_BarAggregationM1Symbols;
    set<string>                                        m_BarAggregationD1Symbols;
    double                                             m_ErroneousTickPxChg;
    set<string>                                        m_SymToBeWarmedUpWithDayBars;
    long                                               m_GKYZ_Window_Size;
    MktDataTradeVolumeMode                             m_MktData_TradeVolumeMode;
    string                                             m_VolSurfParamFile1FM;
    string                                             m_VolSurfParamFile2FM;
    string                                             m_ProbDistrFileFSMC1D;
    OrderRoutingMode                                   m_OrderRoutingMode;
    string                                             m_NextTier_ZMQ_MD_IP_Port;
    string                                             m_NextTier_ZMQ_TF_IP_Port;
    TCPOrEmbeddedMode                                  m_TCPOrEmbeddedMode;
    int                                                m_B1_HKF_SamplingIntervalInSec;
    map<int,OTIMDIConfig>                              m_map_OTIMDIConfig;
    map<StrategyID,TradingStrategyConfig>              m_map_TradingStrategyConfig;
    bool                                               m_SendResetOnConnectionToCAPI;
    int                                                m_NumOfMDI;
    int                                                m_NumOfOTI;
    HHMMSS                                             m_HKFEContFutRollFwdTime;
    HHMMSS                                             m_CMEContFutRollFwdTime;
    set<string>                                        m_ProceedStyForceExcnEvenIfNoMD;

    bool                                               m_On_S11A_T1GBM          ;
    bool                                               m_On_S11A_T1GUMBELCAUCHY ;
    bool                                               m_On_S11A_T1GUMBELGAUSS  ;
    bool                                               m_On_S11A_T2GBM          ;
    bool                                               m_On_S11A_T2PCTL         ;
    bool                                               m_On_S11A_T2COPU         ;

    double                                             m_S11A_NotionalValuePerLeg;
    double                                             m_S11A_T1GBM_TriggerThreshold;
    double                                             m_S11A_T2GBM_TriggerThreshold;
    double                                             m_S11A_T1GumbelCauchy_TriggerThreshold;
    double                                             m_S11A_T1GumbelGauss_TriggerThreshold;
    double                                             m_S11A_T2Copu_TriggerThreshold;
    double                                             m_S11A_T1GBM_TakeProfitThreshold;
    double                                             m_S11A_T2GBM_TakeProfitThreshold;
    double                                             m_S11A_T1GumbelCauchy_TakeProfitThreshold;
    double                                             m_S11A_T1GumbelGauss_TakeProfitThreshold;
    double                                             m_S11A_T2Copu_TakeProfitThreshold;
    double                                             m_S11A_T1GBM_StopLossThreshold;
    double                                             m_S11A_T2GBM_StopLossThreshold;
    double                                             m_S11A_T1GumbelCauchy_StopLossThreshold;
    double                                             m_S11A_T1GumbelGauss_StopLossThreshold;
    double                                             m_S11A_T1GumbelCauchy_EOLPeriod;
    double                                             m_S11A_T1GumbelGauss_EOLPeriod;
    double                                             m_S11A_T2Copu_StopLossThreshold;
    vector<long>                                       m_S11A_T1GBM_DeltaTPeriods;
    vector<long>                                       m_S11A_T2_MAPeriods;
    vector<long>                                       m_S11A_T2GBM_DeltaTPeriods;
    double                                             m_S11A_T1GumbelCauchyPeriod;
    double                                             m_S11A_T1GumbelGaussPeriod;
    double                                             m_S11A_T2CopuPeriod;
    double                                             m_S11A_T2PctlPeriod;
    double                                             m_S11A_T1GumbelCauchyThetaMean;
    double                                             m_S11A_T1GumbelGaussThetaMean;
    double                                             m_S11A_T1GumbelCauchyThetaMeanReversion;
    double                                             m_S11A_T1GumbelGaussThetaMeanReversion;
    double                                             m_S11A_T1GumbelCauchyMarginalSDScaling;
    double                                             m_S11A_T1GumbelGaussMarginalSDScaling;
    double                                             m_S11A_MonitoringIntervalInSec;
    double                                             m_S11A_T1GumbelGaussSigalLimit;
    bool                                               m_B2_US1_HasEnabledMinCommissionCheck;
    int                                                m_B2_US1_HasEnabledRotationMode;
    string                                             m_B2_US1_TheoCPnLHistFolder;
    string                                             m_B2_US1_TheoPosFolder;
    bool                                               m_B2_US1_PersistTheoPosCPnL;
    vector<int>                                        m_B2_US1_RotationGroup;
    int                                                m_B2_US1_ChooseBestNRotationGroup;
    int                                                m_B2_US1_ActionTimeBefCloseInSec;
    vector<int>                                        m_B2_US1_FilterSMAPeriod;
    bool                                               m_B2_US1_MoveNextBestUpIfNoSignal;
    bool                                               m_B2_US2_HasEnabledMinCommissionCheck;
    int                                                m_B2_US2_HasEnabledRotationMode;
    string                                             m_B2_US2_TheoCPnLHistFolder;
    string                                             m_B2_US2_TheoPosFolder;
    bool                                               m_B2_US2_PersistTheoPosCPnL;
    vector<int>                                        m_B2_US2_RotationGroup;
    int                                                m_B2_US2_ChooseBestNRotationGroup;
    int                                                m_B2_US2_ActionTimeBefCloseInSec;
    vector<int>                                        m_B2_US2_FilterSMAPeriod;
    bool                                               m_B2_US2_MoveNextBestUpIfNoSignal;
    bool                                               m_B2_HK_HasEnabledMinCommissionCheck;
    int                                                m_B2_HK_HasEnabledRotationMode;
    string                                             m_B2_HK_TheoCPnLHistFolder;
    string                                             m_B2_HK_TheoPosFolder;
    bool                                               m_B2_HK_PersistTheoPosCPnL;
    vector<int>                                        m_B2_HK_RotationGroup;
    int                                                m_B2_HK_ChooseBestNRotationGroup;
    int                                                m_B2_HK_ActionTimeBefCloseInSec;
    vector<int>                                        m_B2_HK_FilterSMAPeriod;
    bool                                               m_B2_HK_MoveNextBestUpIfNoSignal;
    string                                             m_SymbolsToBeUsedInAllSections;
};

#endif
