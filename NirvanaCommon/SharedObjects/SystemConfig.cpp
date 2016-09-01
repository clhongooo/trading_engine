#include "SystemConfig.h"

boost::weak_ptr<SystemConfig> SystemConfig::m_pInstance;


boost::shared_ptr<SystemConfig> SystemConfig::Instance() {
  boost::shared_ptr<SystemConfig> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new SystemConfig());
    m_pInstance = instance;
  }
  return instance;
}

//--------------------------------------------------
SystemConfig::SystemConfig() :
  m_DefaultTimeZone(SDateTime::HKT),
  m_ProgramStartTime(boost::posix_time::microsec_clock::local_time()),
  m_SystemServicePort(8000),
  m_ConfigPath("nirvana.ini"),
  m_MainLogPath("nirvana.log"),
  m_MTMLogPath("nirvana_mtm.log"),
  m_MTMBySymLogPath("nirvana_mtm_bysym.log"),
  m_ExecLogPath("nirvana_exec.log"),
  m_MTMHoldingsLogPath("nirvana_mtm_holdings.log"),
  m_SupplementaryBarD1Path(""),
  m_SupplementaryBarM1Path(""),
  m_SupplementaryBarLeadingZeroAdj(false),
  m_HKIntradaySeasonalityOnOff(false),
  m_HKIntradaySeasonalityPath(""),
  m_HKIntradaySeasonalityWindow(44),
  m_PositionPersistenceFile(""),
  m_LoadItrdHighLowFromIB(""),
  m_PositionPersistenceIsEnabled(false),
  m_S11A_ParamFileHistSDCorrel(""),
  m_S11A_ParamFileHSIHHIIV(""),
  m_S11A_ParamFileT1GumbelCauchy(""),
  m_S11A_ParamFileT1GumbelGauss(""),
  m_S11A_ParamFileT2Copula(""),
  m_VolSurfCalcIntervalInSec(60),
  m_ProbDistrnCalcIntervalInSec(60),
  m_TechIndUpdateIntervalInSec(60),
  m_TradingHoursPath(""),
  m_HKFE_CalendarPath(""),
  m_HKSE_HSIConstituentsPath(""),
  m_HKMA_ExchgFundBillPath(""),
  m_CorrelMatricesPath(""),
  m_HaltTradingPath(""),
  m_HalfDayTradingPath(""),
  m_CorpActionAdjPath(""),
  m_LotSizePath(""),
  m_ErroneousTickPxChg(NAN),
  m_GKYZ_Window_Size(6000),
  m_MktData_TradeVolumeMode(SystemConfig::ACCUMULATED_VOLUME_MODE),
  m_VolSurfParamFile1FM(""),
  m_VolSurfParamFile2FM(""),
  m_ProbDistrFileFSMC1D(""),
  m_MDIMode(MDI_READFILE),
  m_OTIMode(OTI_RECORD),
  m_NextTierZMQIsOn(false),
  m_NextTier_ZMQ_MD_IP_Port(""),
  m_NextTier_ZMQ_TF_IP_Port(""),
  m_B1_HKF_SamplingIntervalInSec(1800),
  m_SendResetOnConnectionToCAPI(true),
  m_NumOfMDI(0),
  m_NumOfOTI(0),
  m_HKFEContFutRollFwdTime(HHMMSS(0)),
  m_CMEContFutRollFwdTime(HHMMSS(0)),

  m_On_S11A_T1GBM          (false),
  m_On_S11A_T1GUMBELCAUCHY (false),
  m_On_S11A_T1GUMBELGAUSS  (false),
  m_On_S11A_T2GBM          (false),
  m_On_S11A_T2PCTL         (false),
  m_On_S11A_T2COPU         (false),

  m_S11A_NotionalValuePerLeg(0),
  m_S11A_T1GBM_TriggerThreshold(0),
  m_S11A_T2GBM_TriggerThreshold(0),
  m_S11A_T1GumbelCauchy_TriggerThreshold(0),
  m_S11A_T1GumbelGauss_TriggerThreshold(0),
  m_S11A_T2Copu_TriggerThreshold(0),
  m_S11A_T1GBM_TakeProfitThreshold(0),
  m_S11A_T2GBM_TakeProfitThreshold(0),
  m_S11A_T1GumbelCauchy_TakeProfitThreshold(0),
  m_S11A_T1GumbelGauss_TakeProfitThreshold(0),
  m_S11A_T2Copu_TakeProfitThreshold(0),
  m_S11A_T1GBM_StopLossThreshold(0),
  m_S11A_T2GBM_StopLossThreshold(0),
  m_S11A_T1GumbelCauchy_StopLossThreshold(0),
  m_S11A_T1GumbelGauss_StopLossThreshold(0),
  m_S11A_T1GumbelCauchy_EOLPeriod(0),
  m_S11A_T1GumbelGauss_EOLPeriod(0),
  m_S11A_T2Copu_StopLossThreshold(0),
  m_S11A_T1GBM_DeltaTPeriods(0),
  m_S11A_T2_MAPeriods(0),
  m_S11A_T2GBM_DeltaTPeriods(0),
  m_S11A_T1GumbelCauchyPeriod(0),
  m_S11A_T1GumbelGaussPeriod(0),
  m_S11A_T2CopuPeriod(0),
  m_S11A_T2PctlPeriod(0),
  m_S11A_T1GumbelCauchyThetaMean(0),
  m_S11A_T1GumbelGaussThetaMean(0),
  m_S11A_T1GumbelCauchyThetaMeanReversion(0),
  m_S11A_T1GumbelGaussThetaMeanReversion(0),
  m_S11A_T1GumbelCauchyMarginalSDScaling(0),
  m_S11A_T1GumbelGaussMarginalSDScaling(0),
  m_S11A_MonitoringIntervalInSec(0),
  m_S11A_T1GumbelGaussSigalLimit(0),
  m_B2_US1_HasEnabledMinCommissionCheck(true),
  m_B2_US1_CommissionRateThreshold(0),
  m_B2_US1_HasEnabledRotationMode(0),
  m_B2_US1_RotationModeTradeHighestReturn(0),
  m_B2_US1_RotationModeUseVolyAdjdReturn(true),
  m_B2_US1_RotationModeUseNDayReturn(2),
  m_B2_US1_LongOnlyWhenClosePriceBelowAvgPrice(boost::optional<double>()),
  m_B2_US1_ShortOnlyWhenClosePriceAboveAvgPrice(boost::optional<double>()),
  m_B2_US1_LongOnlyWhenAvgPriceReturnAbove(boost::optional<double>()),
  m_B2_US1_LongOnlyWhenGrpAvgReturnAbove(boost::optional<double>()),
  m_B2_US1_ShortOnlyWhenAvgPriceReturnBelow(boost::optional<double>()),
  m_B2_US1_TheoCPnLHistFolder(""),
  m_B2_US1_TheoPosFolder(""),
  m_B2_US1_PersistTheoPosCPnL(false),
  m_B2_US1_RotationGroupFile(""),
  m_B2_US1_Arima_ZMQ_IP_Port(""),
  m_B2_US1_PerformCppTaylor(true),
  m_B2_US1_SymbolStickiness(false),
  m_B2_US1_TaylorWeight(0.5),
  m_B2_US1_ArimaWeight(0.5),
  m_B2_US1_ChooseBestNRotationGroupProp(0.5),
  m_B2_US1_ActionTimeBefCloseInSec(180),
  m_B2_US1_ActionTimeAftOpenInSec(180),
  m_B2_US1_MoveNextBestGroupUpIfNoSignal(true),
  m_B2_US1_MoveNextBestStkInGrpUpIfNoSignal(true),
  m_B2_US1_AvgAggSgndNotnlThresh(0),
  m_B2_US2_HasEnabledMinCommissionCheck(true),
  m_B2_US2_CommissionRateThreshold(0),
  m_B2_US2_HasEnabledRotationMode(0),
  m_B2_US2_RotationModeTradeHighestReturn(0),
  m_B2_US2_RotationModeUseVolyAdjdReturn(true),
  m_B2_US2_RotationModeUseNDayReturn(2),
  m_B2_US2_LongOnlyWhenClosePriceBelowAvgPrice(boost::optional<double>()),
  m_B2_US2_ShortOnlyWhenClosePriceAboveAvgPrice(boost::optional<double>()),
  m_B2_US2_LongOnlyWhenAvgPriceReturnAbove(boost::optional<double>()),
  m_B2_US2_LongOnlyWhenGrpAvgReturnAbove(boost::optional<double>()),
  m_B2_US2_ShortOnlyWhenAvgPriceReturnBelow(boost::optional<double>()),
  m_B2_US2_TheoCPnLHistFolder(""),
  m_B2_US2_TheoPosFolder(""),
  m_B2_US2_PersistTheoPosCPnL(false),
  m_B2_US2_RotationGroupFile(""),
  m_B2_US2_Arima_ZMQ_IP_Port(""),
  m_B2_US2_PerformCppTaylor(true),
  m_B2_US2_SymbolStickiness(false),
  m_B2_US2_TaylorWeight(0.5),
  m_B2_US2_ArimaWeight(0.5),
  m_B2_US2_ChooseBestNRotationGroupProp(0.5),
  m_B2_US2_ActionTimeBefCloseInSec(180),
  m_B2_US2_ActionTimeAftOpenInSec(180),
  m_B2_US2_MoveNextBestGroupUpIfNoSignal(true),
  m_B2_US2_MoveNextBestStkInGrpUpIfNoSignal(true),
  m_B2_US2_AvgAggSgndNotnlThresh(0),
  m_B2_US3_HasEnabledMinCommissionCheck(true),
  m_B2_US3_CommissionRateThreshold(0),
  m_B2_US3_HasEnabledRotationMode(0),
  m_B2_US3_RotationModeTradeHighestReturn(0),
  m_B2_US3_RotationModeUseVolyAdjdReturn(true),
  m_B2_US3_RotationModeUseNDayReturn(2),
  m_B2_US3_LongOnlyWhenClosePriceBelowAvgPrice(boost::optional<double>()),
  m_B2_US3_ShortOnlyWhenClosePriceAboveAvgPrice(boost::optional<double>()),
  m_B2_US3_LongOnlyWhenAvgPriceReturnAbove(boost::optional<double>()),
  m_B2_US3_LongOnlyWhenGrpAvgReturnAbove(boost::optional<double>()),
  m_B2_US3_ShortOnlyWhenAvgPriceReturnBelow(boost::optional<double>()),
  m_B2_US3_TheoCPnLHistFolder(""),
  m_B2_US3_TheoPosFolder(""),
  m_B2_US3_PersistTheoPosCPnL(false),
  m_B2_US3_RotationGroupFile(""),
  m_B2_US3_Arima_ZMQ_IP_Port(""),
  m_B2_US3_PerformCppTaylor(true),
  m_B2_US3_SymbolStickiness(false),
  m_B2_US3_TaylorWeight(0.5),
  m_B2_US3_ArimaWeight(0.5),
  m_B2_US3_ChooseBestNRotationGroupProp(0.5),
  m_B2_US3_ActionTimeBefCloseInSec(180),
  m_B2_US3_ActionTimeAftOpenInSec(180),
  m_B2_US3_MoveNextBestGroupUpIfNoSignal(true),
  m_B2_US3_MoveNextBestStkInGrpUpIfNoSignal(true),
  m_B2_US3_AvgAggSgndNotnlThresh(0),
  m_B2_HK_HasEnabledMinCommissionCheck(true),
  m_B2_HK_CommissionRateThreshold(0),
  m_B2_HK_HasEnabledRotationMode(0),
  m_B2_HK_RotationModeTradeHighestReturn(0),
  m_B2_HK_RotationModeUseNDayReturn(2),
  m_B2_HK_LongOnlyWhenClosePriceBelowAvgPrice(boost::optional<double>()),
  m_B2_HK_ShortOnlyWhenClosePriceAboveAvgPrice(boost::optional<double>()),
  m_B2_HK_LongOnlyWhenAvgPriceReturnAbove(boost::optional<double>()),
  m_B2_HK_LongOnlyWhenGrpAvgReturnAbove(boost::optional<double>()),
  m_B2_HK_ShortOnlyWhenAvgPriceReturnBelow(boost::optional<double>()),
  m_B2_HK_TheoCPnLHistFolder(""),
  m_B2_HK_TheoPosFolder(""),
  m_B2_HK_PersistTheoPosCPnL(false),
  m_B2_HK_RotationGroupFile(""),
  m_B2_HK_Arima_ZMQ_IP_Port(""),
  m_B2_HK_PerformCppTaylor(true),
  m_B2_HK_SymbolStickiness(false),
  m_B2_HK_TaylorWeight(0.5),
  m_B2_HK_ArimaWeight(0.5),
  m_B2_HK_ChooseBestNRotationGroupProp(0.5),
  m_B2_HK_ActionTimeBefCloseInSec(180),
  m_B2_HK_ActionTimeAftOpenInSec(180),
  m_B2_HK_MoveNextBestGroupUpIfNoSignal(true),
  m_B2_HK_MoveNextBestStkInGrpUpIfNoSignal(true),
  m_B2_HK_AvgAggSgndNotnlThresh(0),
  m_SymbolsToBeUsedInAllSections(""),
  m_MaxAllowedTradeNotional(0),
  m_MaxAllowedTradeQty(0)
{
  m_Logger = Logger::Instance();
  SetProgramStartTime();
  Reset();
}


//--------------------------------------------------
OTIMDIConfig::OTIMDIConfig() :
  m_MDI_File(""),
  m_MDI_IP(""),
  m_MDI_Port(""),
  m_OTI_IP(""),
  m_OTI_Port("")
{
  return;
}


//--------------------------------------------------
TradingStrategyConfig::TradingStrategyConfig() :
  m_OnOff(false),
  m_SignalLogPath(""),
  m_StartTimeBufferInSec(60),
  m_EndTimeBufferInSec(60),
  m_BreakTimeBufferInSec(60),
  m_MTMTime(235959),
  m_MonitoringIntervalInSec(-1),
  m_WhetherToRetrain(true),
  m_TrainingFreq(Auto),
  m_CPnLHistPersistenceIsEnabled(false),
  m_CPnLHistPersistenceFile("")
{
  return;
}


//--------------------------------------------------
void SystemConfig::Reset()
{
}

void SystemConfig::SetConfigPath(const string & sConfigPath)
{
  m_ConfigPath = sConfigPath;
}

void SystemConfig::SetProgramStartTime()
{
  m_ProgramStartTime = boost::posix_time::microsec_clock::local_time();
}

void SystemConfig::SetItrdHighLowFromIB(const string & sItrdHighLowFromIB)
{
  m_LoadItrdHighLowFromIB = sItrdHighLowFromIB;
}

SDateTime::TIMEZONE SystemConfig::GetDefaultTimeZone() const
{
  return  m_DefaultTimeZone;
}

const unsigned short SystemConfig::GetSystemServicePort() const
{
  return  m_SystemServicePort;
}

boost::posix_time::ptime SystemConfig::GetProgramStartTime() const
{
  return m_ProgramStartTime;
}


long                                   SystemConfig::Get_VolSurfCalcIntervalInSec()                    const {  return m_VolSurfCalcIntervalInSec;                    }
long                                   SystemConfig::Get_ProbDistrnCalcIntervalInSec()                 const {  return m_ProbDistrnCalcIntervalInSec;                 }
long                                   SystemConfig::Get_TechIndUpdateIntervalInSec()                  const {  return m_TechIndUpdateIntervalInSec;                  }
string                                 SystemConfig::Get_TradingHoursPath()                            const {  return m_TradingHoursPath;                            }
string                                 SystemConfig::Get_HKFE_CalendarPath()                           const {  return m_HKFE_CalendarPath;                           }
string                                 SystemConfig::Get_HKSE_HSIConstituentsPath()                    const {  return m_HKSE_HSIConstituentsPath;                    }
string                                 SystemConfig::Get_HKMA_ExchgFundBillPath()                      const {  return m_HKMA_ExchgFundBillPath;                      }
string                                 SystemConfig::Get_CorrelMatricesPath()                          const {  return m_CorrelMatricesPath;                          }
long                                   SystemConfig::Get_GKYZ_Window_Size()                            const {  return m_GKYZ_Window_Size;                            }
SystemConfig::MktDataTradeVolumeMode   SystemConfig::Get_MktDataTradeVolumeMode()                      const {  return m_MktData_TradeVolumeMode;                     }
string                                 SystemConfig::Get_VolSurfParamFile1FM()                         const {  return m_VolSurfParamFile1FM;                         }
string                                 SystemConfig::Get_VolSurfParamFile2FM()                         const {  return m_VolSurfParamFile2FM;                         }
string                                 SystemConfig::Get_ProbDistrFileFSMC1D()                         const {  return m_ProbDistrFileFSMC1D;                         }
string                                 SystemConfig::Get_NextTier_ZMQ_MD_IP_Port()                     const {  return m_NextTier_ZMQ_MD_IP_Port;                     }
string                                 SystemConfig::Get_NextTier_ZMQ_TF_IP_Port()                     const {  return m_NextTier_ZMQ_TF_IP_Port;                     }
SystemConfig::MDIMode                  SystemConfig::Get_MDIMode()                                     const {  return m_MDIMode;                                     }
SystemConfig::OTIMode                  SystemConfig::Get_OTIMode()                                     const {  return m_OTIMode;                                     }
bool                                   SystemConfig::NextTierZMQIsOn()                                 const {  return m_NextTierZMQIsOn;                             }
string                                 SystemConfig::Get_Main_Log_Path()                               const {  return m_MainLogPath;                                 }
string                                 SystemConfig::Get_MTM_Log_Path()                                const {  return m_MTMLogPath;                                  }
string                                 SystemConfig::Get_MTMBySym_Log_Path()                           const {  return m_MTMBySymLogPath;                             }
string                                 SystemConfig::Get_Exec_Log_Path()                               const {  return m_ExecLogPath;                                 }
string                                 SystemConfig::Get_Holdings_Log_Path()                           const {  return m_MTMHoldingsLogPath;                          }
string                                 SystemConfig::Get_SupplementaryBarD1Path()                      const {  return m_SupplementaryBarD1Path;                      }
string                                 SystemConfig::Get_SupplementaryBarM1Path()                      const {  return m_SupplementaryBarM1Path;                      }
bool                                   SystemConfig::Get_SupplementaryBarLeadingZeroAdj()              const {  return m_SupplementaryBarLeadingZeroAdj;              }
bool                                   SystemConfig::Get_HKIntradaySeasonalityOnOff()                  const {  return m_HKIntradaySeasonalityOnOff;                  }
string                                 SystemConfig::Get_HKIntradaySeasonalityPath()                   const {  return m_HKIntradaySeasonalityPath;                   }
int                                    SystemConfig::Get_HKIntradaySeasonalityWindow()                 const {  return m_HKIntradaySeasonalityWindow;                 }
string                                 SystemConfig::Get_PositionPersistenceFile()                     const {  return m_PositionPersistenceFile;                     }
string                                 SystemConfig::Get_LoadItrdHighLowFromIB()                       const {  return m_LoadItrdHighLowFromIB;                       }
bool                                   SystemConfig::PositionPersistenceIsEnabled()                    const {  return m_PositionPersistenceIsEnabled;                }
string                                 SystemConfig::Get_S11AParamFileHistSDCorrel()                   const {  return m_S11A_ParamFileHistSDCorrel;                  }
string                                 SystemConfig::Get_S11AParamFileHSIHHIIV()                       const {  return m_S11A_ParamFileHSIHHIIV;                      }
string                                 SystemConfig::Get_S11AParamFileT1GumbelCauchy()                 const {  return m_S11A_ParamFileT1GumbelCauchy;                }
string                                 SystemConfig::Get_S11AParamFileT1GumbelGauss()                  const {  return m_S11A_ParamFileT1GumbelGauss;                 }
string                                 SystemConfig::Get_S11AParamFileT2Copula()                       const {  return m_S11A_ParamFileT2Copula;                      }
int                                    SystemConfig::Get_B1_HKF_SmplgIntvlInSec()                      const {  return m_B1_HKF_SamplingIntervalInSec;                }
bool                                   SystemConfig::SendResetOnConnectionToCAPI()                     const {  return m_SendResetOnConnectionToCAPI;                 }
int                                    SystemConfig::GetNumOfMDI()                                     const {  return m_NumOfMDI;                                    }
int                                    SystemConfig::GetNumOfOTI()                                     const {  return m_NumOfOTI;                                    }

HHMMSS                                 SystemConfig::Get_HKFEContFutRollFwdTime()                      const {  return m_HKFEContFutRollFwdTime;                      }
HHMMSS                                 SystemConfig::Get_CMEContFutRollFwdTime()                       const {  return m_CMEContFutRollFwdTime;                       }

bool                                   SystemConfig::Get_OnOff_S11A_T1GBM()                            const {  return m_On_S11A_T1GBM         ;                      }
bool                                   SystemConfig::Get_OnOff_S11A_T1GUMBELCAUCHY()                   const {  return m_On_S11A_T1GUMBELCAUCHY;                      }
bool                                   SystemConfig::Get_OnOff_S11A_T1GUMBELGAUSS()                    const {  return m_On_S11A_T1GUMBELGAUSS ;                      }
bool                                   SystemConfig::Get_OnOff_S11A_T2GBM()                            const {  return m_On_S11A_T2GBM         ;                      }
bool                                   SystemConfig::Get_OnOff_S11A_T2PCTL()                           const {  return m_On_S11A_T2PCTL        ;                      }
bool                                   SystemConfig::Get_OnOff_S11A_T2COPU()                           const {  return m_On_S11A_T2COPU        ;                      }

vector<long>                           SystemConfig::Get_S11AT1GBMDeltaTPeriods()                      const {  return m_S11A_T1GBM_DeltaTPeriods;                    }
vector<long>                           SystemConfig::Get_S11AT2MAPeriods()                             const {  return m_S11A_T2_MAPeriods;                           }
vector<long>                           SystemConfig::Get_S11AT2GBMDeltaTPeriods()                      const {  return m_S11A_T2GBM_DeltaTPeriods;                    }
double                                 SystemConfig::Get_S11AT1GBMTriggerThreshold()                   const {  return m_S11A_T1GBM_TriggerThreshold;                 }
double                                 SystemConfig::Get_S11AT2GBMTriggerThreshold()                   const {  return m_S11A_T2GBM_TriggerThreshold;                 }
double                                 SystemConfig::Get_S11AT1GumbelCauchyTriggerThreshold()          const {  return m_S11A_T1GumbelCauchy_TriggerThreshold;        }
double                                 SystemConfig::Get_S11AT1GumbelGaussTriggerThreshold()           const {  return m_S11A_T1GumbelGauss_TriggerThreshold;         }
double                                 SystemConfig::Get_S11AT2CopuTriggerThreshold()                  const {  return m_S11A_T2Copu_TriggerThreshold;                }
double                                 SystemConfig::Get_S11AT1GBMTakeProfitThreshold()                const {  return m_S11A_T1GBM_TakeProfitThreshold;              }
double                                 SystemConfig::Get_S11AT2GBMTakeProfitThreshold()                const {  return m_S11A_T2GBM_TakeProfitThreshold;              }
double                                 SystemConfig::Get_S11AT1GumbelCauchyTakeProfitThreshold()       const {  return m_S11A_T1GumbelCauchy_TakeProfitThreshold;     }
double                                 SystemConfig::Get_S11AT1GumbelGaussTakeProfitThreshold()        const {  return m_S11A_T1GumbelGauss_TakeProfitThreshold;      }
double                                 SystemConfig::Get_S11AT2CopuTakeProfitThreshold()               const {  return m_S11A_T2Copu_TakeProfitThreshold;             }
double                                 SystemConfig::Get_S11AT1GBMStopLossThreshold()                  const {  return m_S11A_T1GBM_StopLossThreshold;                }
double                                 SystemConfig::Get_S11AT2GBMStopLossThreshold()                  const {  return m_S11A_T2GBM_StopLossThreshold;                }
double                                 SystemConfig::Get_S11AT1GumbelCauchyStopLossThreshold()         const {  return m_S11A_T1GumbelCauchy_StopLossThreshold;       }
double                                 SystemConfig::Get_S11AT1GumbelGaussStopLossThreshold()          const {  return m_S11A_T1GumbelGauss_StopLossThreshold;        }
double                                 SystemConfig::Get_S11AT2CopuStopLossThreshold()                 const {  return m_S11A_T2Copu_StopLossThreshold;               }
double                                 SystemConfig::Get_S11AT1GumbelCauchyEOLPeriod()                 const {  return m_S11A_T1GumbelCauchy_EOLPeriod;               }
double                                 SystemConfig::Get_S11AT1GumbelGaussEOLPeriod()                  const {  return m_S11A_T1GumbelGauss_EOLPeriod;                }
double                                 SystemConfig::Get_S11ANotionalValuePerLeg()                     const {  return m_S11A_NotionalValuePerLeg;                    }
double                                 SystemConfig::Get_S11AT2PctlPeriod()                            const {  return m_S11A_T2PctlPeriod;                           }
double                                 SystemConfig::Get_S11AT1GumbelCauchyPeriod()                    const {  return m_S11A_T1GumbelCauchyPeriod;                   }
double                                 SystemConfig::Get_S11AT1GumbelGaussPeriod()                     const {  return m_S11A_T1GumbelGaussPeriod;                    }
double                                 SystemConfig::Get_S11AT2CopuPeriod()                            const {  return m_S11A_T2CopuPeriod;                           }
double                                 SystemConfig::Get_S11AT1GumbelCauchyThetaMean()                 const {  return m_S11A_T1GumbelCauchyThetaMean;                }
double                                 SystemConfig::Get_S11AT1GumbelGaussThetaMean()                  const {  return m_S11A_T1GumbelGaussThetaMean;                 }
double                                 SystemConfig::Get_S11AT1GumbelCauchyThetaMeanReversion()        const {  return m_S11A_T1GumbelCauchyThetaMeanReversion;       }
double                                 SystemConfig::Get_S11AT1GumbelGaussThetaMeanReversion()         const {  return m_S11A_T1GumbelGaussThetaMeanReversion;        }
double                                 SystemConfig::Get_S11AT1GumbelCauchyMarginalSDScaling()         const {  return m_S11A_T1GumbelCauchyMarginalSDScaling;        }
double                                 SystemConfig::Get_S11AT1GumbelGaussMarginalSDScaling()          const {  return m_S11A_T1GumbelGaussMarginalSDScaling;         }
double                                 SystemConfig::Get_S11AMonitoringIntervalInSec()                 const {  return m_S11A_MonitoringIntervalInSec;                }
double                                 SystemConfig::Get_S11AT1GumbelGaussSignalLimit()                const {  return m_S11A_T1GumbelGaussSigalLimit;                }

bool                          SystemConfig::B2_HasEnabledMinCommissionCheck        (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_HasEnabledMinCommissionCheck;         else if (id == STY_B2_US2) return m_B2_US2_HasEnabledMinCommissionCheck;            else if (id == STY_B2_US3) return m_B2_US3_HasEnabledMinCommissionCheck;            else if (id == STY_B2_HK) return m_B2_HK_HasEnabledMinCommissionCheck;        }
double                        SystemConfig::B2_CommissionRateThreshold             (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_CommissionRateThreshold;              else if (id == STY_B2_US2) return m_B2_US2_CommissionRateThreshold;                 else if (id == STY_B2_US3) return m_B2_US3_CommissionRateThreshold;                 else if (id == STY_B2_HK) return m_B2_HK_CommissionRateThreshold;             }
int                           SystemConfig::B2_HasEnabledRotationMode              (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_HasEnabledRotationMode;               else if (id == STY_B2_US2) return m_B2_US2_HasEnabledRotationMode;                  else if (id == STY_B2_US3) return m_B2_US3_HasEnabledRotationMode;                  else if (id == STY_B2_HK) return m_B2_HK_HasEnabledRotationMode;              }
bool                          SystemConfig::B2_RotationModeTradeHighestReturn      (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_RotationModeTradeHighestReturn;       else if (id == STY_B2_US2) return m_B2_US2_RotationModeTradeHighestReturn;          else if (id == STY_B2_US3) return m_B2_US3_RotationModeTradeHighestReturn;          else if (id == STY_B2_HK) return m_B2_HK_RotationModeTradeHighestReturn;      }
bool                          SystemConfig::B2_RotationModeUseVolyAdjdReturn       (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_RotationModeUseVolyAdjdReturn;        else if (id == STY_B2_US2) return m_B2_US2_RotationModeUseVolyAdjdReturn;           else if (id == STY_B2_US3) return m_B2_US3_RotationModeUseVolyAdjdReturn;           else if (id == STY_B2_HK) return m_B2_HK_RotationModeUseVolyAdjdReturn;       }
int                           SystemConfig::B2_RotationModeUseNDayReturn           (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_RotationModeUseNDayReturn;            else if (id == STY_B2_US2) return m_B2_US2_RotationModeUseNDayReturn;               else if (id == STY_B2_US3) return m_B2_US3_RotationModeUseNDayReturn;               else if (id == STY_B2_HK) return m_B2_HK_RotationModeUseNDayReturn;           }
boost::optional<double>       SystemConfig::B2_LongOnlyWhenClosePriceBelowAvgPrice (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_LongOnlyWhenClosePriceBelowAvgPrice;  else if (id == STY_B2_US2) return m_B2_US2_LongOnlyWhenClosePriceBelowAvgPrice;     else if (id == STY_B2_US3) return m_B2_US3_LongOnlyWhenClosePriceBelowAvgPrice;     else if (id == STY_B2_HK) return m_B2_HK_LongOnlyWhenClosePriceBelowAvgPrice; }
boost::optional<double>       SystemConfig::B2_ShortOnlyWhenClosePriceAboveAvgPrice(const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_ShortOnlyWhenClosePriceAboveAvgPrice; else if (id == STY_B2_US2) return m_B2_US2_ShortOnlyWhenClosePriceAboveAvgPrice;    else if (id == STY_B2_US3) return m_B2_US3_ShortOnlyWhenClosePriceAboveAvgPrice;    else if (id == STY_B2_HK) return m_B2_HK_ShortOnlyWhenClosePriceAboveAvgPrice;}
boost::optional<double>       SystemConfig::B2_LongOnlyWhenAvgPriceReturnAbove     (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_LongOnlyWhenAvgPriceReturnAbove;      else if (id == STY_B2_US2) return m_B2_US2_LongOnlyWhenAvgPriceReturnAbove;         else if (id == STY_B2_US3) return m_B2_US3_LongOnlyWhenAvgPriceReturnAbove;         else if (id == STY_B2_HK) return m_B2_HK_LongOnlyWhenAvgPriceReturnAbove;     }
boost::optional<double>       SystemConfig::B2_LongOnlyWhenGrpAvgReturnAbove       (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_LongOnlyWhenGrpAvgReturnAbove;        else if (id == STY_B2_US2) return m_B2_US2_LongOnlyWhenGrpAvgReturnAbove;           else if (id == STY_B2_US3) return m_B2_US3_LongOnlyWhenGrpAvgReturnAbove;           else if (id == STY_B2_HK) return m_B2_HK_LongOnlyWhenGrpAvgReturnAbove;       }
boost::optional<double>       SystemConfig::B2_ShortOnlyWhenAvgPriceReturnBelow    (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_ShortOnlyWhenAvgPriceReturnBelow;     else if (id == STY_B2_US2) return m_B2_US2_ShortOnlyWhenAvgPriceReturnBelow;        else if (id == STY_B2_US3) return m_B2_US3_ShortOnlyWhenAvgPriceReturnBelow;        else if (id == STY_B2_HK) return m_B2_HK_ShortOnlyWhenAvgPriceReturnBelow;    }
string                        SystemConfig::B2_TheoCPnLHistFolder                  (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_TheoCPnLHistFolder;                   else if (id == STY_B2_US2) return m_B2_US2_TheoCPnLHistFolder;                      else if (id == STY_B2_US3) return m_B2_US3_TheoCPnLHistFolder;                      else if (id == STY_B2_HK) return m_B2_HK_TheoCPnLHistFolder;                  }
string                        SystemConfig::B2_TheoPosFolder                       (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_TheoPosFolder;                        else if (id == STY_B2_US2) return m_B2_US2_TheoPosFolder;                           else if (id == STY_B2_US3) return m_B2_US3_TheoPosFolder;                           else if (id == STY_B2_HK) return m_B2_HK_TheoPosFolder;                       }
bool                          SystemConfig::B2_PersistTheoPosCPnL                  (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_PersistTheoPosCPnL;                   else if (id == STY_B2_US2) return m_B2_US2_PersistTheoPosCPnL;                      else if (id == STY_B2_US3) return m_B2_US3_PersistTheoPosCPnL;                      else if (id == STY_B2_HK) return m_B2_HK_PersistTheoPosCPnL;                  }
string                        SystemConfig::Get_B2_RotationGroupFile               (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_RotationGroupFile;                    else if (id == STY_B2_US2) return m_B2_US2_RotationGroupFile;                       else if (id == STY_B2_US3) return m_B2_US3_RotationGroupFile;                       else if (id == STY_B2_HK) return m_B2_HK_RotationGroupFile;                   }
string                        SystemConfig::Get_B2_Arima_ZMQ_IP_Port               (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_Arima_ZMQ_IP_Port;                    else if (id == STY_B2_US2) return m_B2_US2_Arima_ZMQ_IP_Port;                       else if (id == STY_B2_US3) return m_B2_US3_Arima_ZMQ_IP_Port;                       else if (id == STY_B2_HK) return m_B2_HK_Arima_ZMQ_IP_Port;                   }
bool                          SystemConfig::Get_B2_PerformCppTaylor                (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_PerformCppTaylor;                     else if (id == STY_B2_US2) return m_B2_US2_PerformCppTaylor;                        else if (id == STY_B2_US3) return m_B2_US3_PerformCppTaylor;                        else if (id == STY_B2_HK) return m_B2_HK_PerformCppTaylor;                    }
bool                          SystemConfig::Get_B2_SymbolStickiness                (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_SymbolStickiness;                     else if (id == STY_B2_US2) return m_B2_US2_SymbolStickiness;                        else if (id == STY_B2_US3) return m_B2_US3_SymbolStickiness;                        else if (id == STY_B2_HK) return m_B2_HK_SymbolStickiness;                    }
double                        SystemConfig::Get_B2_TaylorWeight                    (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_TaylorWeight;                         else if (id == STY_B2_US2) return m_B2_US2_TaylorWeight;                            else if (id == STY_B2_US3) return m_B2_US3_TaylorWeight;                            else if (id == STY_B2_HK) return m_B2_HK_TaylorWeight;                        }
double                        SystemConfig::Get_B2_ArimaWeight                     (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_ArimaWeight;                          else if (id == STY_B2_US2) return m_B2_US2_ArimaWeight;                             else if (id == STY_B2_US3) return m_B2_US3_ArimaWeight;                             else if (id == STY_B2_HK) return m_B2_HK_ArimaWeight;                         }
vector<int>                   SystemConfig::Get_B2_RoleOfSym                       (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_RoleOfSym;                            else if (id == STY_B2_US2) return m_B2_US2_RoleOfSym;                               else if (id == STY_B2_US3) return m_B2_US3_RoleOfSym;                               else if (id == STY_B2_HK) return m_B2_HK_RoleOfSym;                           }
double                        SystemConfig::Get_B2_ChooseBestNRotationGroupProp    (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_ChooseBestNRotationGroupProp;         else if (id == STY_B2_US2) return m_B2_US2_ChooseBestNRotationGroupProp;            else if (id == STY_B2_US3) return m_B2_US3_ChooseBestNRotationGroupProp;            else if (id == STY_B2_HK) return m_B2_HK_ChooseBestNRotationGroupProp;        }
int                           SystemConfig::Get_B2_ActionTimeBefCloseInSec         (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_ActionTimeBefCloseInSec;              else if (id == STY_B2_US2) return m_B2_US2_ActionTimeBefCloseInSec;                 else if (id == STY_B2_US3) return m_B2_US3_ActionTimeBefCloseInSec;                 else if (id == STY_B2_HK) return m_B2_HK_ActionTimeBefCloseInSec;             }
int                           SystemConfig::Get_B2_ActionTimeAftOpenInSec          (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_ActionTimeAftOpenInSec;               else if (id == STY_B2_US2) return m_B2_US2_ActionTimeAftOpenInSec;                  else if (id == STY_B2_US3) return m_B2_US3_ActionTimeAftOpenInSec;                  else if (id == STY_B2_HK) return m_B2_HK_ActionTimeAftOpenInSec;              }
vector<int>                   SystemConfig::Get_B2_FilterSMAPeriod                 (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_FilterSMAPeriod;                      else if (id == STY_B2_US2) return m_B2_US2_FilterSMAPeriod;                         else if (id == STY_B2_US3) return m_B2_US3_FilterSMAPeriod;                         else if (id == STY_B2_HK) return m_B2_HK_FilterSMAPeriod;                     }
bool                          SystemConfig::Get_B2_MoveNextBestGroupUpIfNoSignal   (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_MoveNextBestGroupUpIfNoSignal;        else if (id == STY_B2_US2) return m_B2_US2_MoveNextBestGroupUpIfNoSignal;           else if (id == STY_B2_US3) return m_B2_US3_MoveNextBestGroupUpIfNoSignal;           else if (id == STY_B2_HK) return m_B2_HK_MoveNextBestGroupUpIfNoSignal;       }
bool                          SystemConfig::Get_B2_MoveNextBestStkInGrpUpIfNoSignal(const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_MoveNextBestStkInGrpUpIfNoSignal;     else if (id == STY_B2_US2) return m_B2_US2_MoveNextBestStkInGrpUpIfNoSignal;        else if (id == STY_B2_US3) return m_B2_US3_MoveNextBestStkInGrpUpIfNoSignal;        else if (id == STY_B2_HK) return m_B2_HK_MoveNextBestStkInGrpUpIfNoSignal;    }
double                        SystemConfig::Get_B2_AvgAggSgndNotnlThresh           (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_AvgAggSgndNotnlThresh;                else if (id == STY_B2_US2) return m_B2_US2_AvgAggSgndNotnlThresh;                   else if (id == STY_B2_US3) return m_B2_US3_AvgAggSgndNotnlThresh;                   else if (id == STY_B2_HK) return m_B2_HK_AvgAggSgndNotnlThresh;               }
set<string>                   SystemConfig::Get_B2_TradedSymTradeAtOpen            (const StrategyID id)  const {  if (id == STY_B2_US1) return m_B2_US1_TradedSymbolsTradeAtOpen;             else if (id == STY_B2_US2) return m_B2_US2_TradedSymbolsTradeAtOpen;                else if (id == STY_B2_US3) return m_B2_US3_TradedSymbolsTradeAtOpen;                else if (id == STY_B2_HK) return m_B2_HK_TradedSymbolsTradeAtOpen;            }


bool SystemConfig::ChkIfProceedStyForceExcnEvenIfNoMD(const string & symbol) const
{
  set<string>::const_iterator it = m_ProceedStyForceExcnEvenIfNoMD.find(symbol);
  if (it == m_ProceedStyForceExcnEvenIfNoMD.end()) return false;
  else return true;
}

bool SystemConfig::CheckIfHaltTrading(const string & symbol, const YYYYMMDD & ymd) const
{
  map<string,set<YYYYMMDD> >::const_iterator it = m_HaltTradingSymDate.find(symbol);

  if (it == m_HaltTradingSymDate.end()) return false;

  set<YYYYMMDD>::const_iterator it2 = it->second.find(ymd);

  if (it2 == it->second.end()) return false;

  return true;
}

bool SystemConfig::CheckIfHalfDayTrading(const string & symbol, const YYYYMMDD & ymd) const
{
  map<string,set<YYYYMMDD> >::const_iterator it = m_HalfDayTradingSymDate.find(symbol);

  if (it == m_HalfDayTradingSymDate.end()) return false;

  set<YYYYMMDD>::const_iterator it2 = it->second.find(ymd);

  if (it2 == it->second.end()) return false;

  return true;
}

bool SystemConfig::GetCorpActionAdj(const string & symbol, const YYYYMMDD & ymd, double & price_delta, double & price_ratio) const
{
  map<string,map<YYYYMMDD,tuple_corp_action_adj> >::const_iterator it = m_CorpActionAdjDetails.find(symbol);
  if (it == m_CorpActionAdjDetails.end()) return false;

  const map<YYYYMMDD,tuple_corp_action_adj> & mapTmp = it->second;

  map<YYYYMMDD,tuple_corp_action_adj>::const_iterator it2 = mapTmp.find(ymd);
  if (it2 == mapTmp.end()) return false;

  price_delta = it2->second.m_price_delta;
  price_ratio = it2->second.m_price_ratio;
  return true;
}

int SystemConfig::GetLotSize(const string & symbol) const
{
  map<string,int>::const_iterator it = m_map_LotSize.find(symbol);
  if (it == m_map_LotSize.end()) return 1;
  return it->second;
}

long SystemConfig::RoundDownLotSize(const string & symbol, const long volume) const
{
  const int iLotSize = GetLotSize(symbol);
  return STool::Sign(volume) * (abs(volume) / iLotSize) * iLotSize;
}

bool SystemConfig::IsPriceFwdrToNextTierOn() const
{
  return m_PriceFwdrIsOn;
}

int SystemConfig::GetPriceFwdrToNextTierIntervalInSec() const
{
  return m_PriceFwdrIntervalInSec;
}

double SystemConfig::GetMaxAllowedTradeNotional() const
{
  return m_MaxAllowedTradeNotional;
}
long SystemConfig::GetMaxAllowedTradeQty() const
{
  return m_MaxAllowedTradeQty;
}

bool SystemConfig::CheckIfBarAggregationM1Symbol(const string & symbol) const
{
  if (m_BarAggregationM1Symbols.find(symbol) == m_BarAggregationM1Symbols.end()) return false;
  else return true;
}

bool SystemConfig::CheckIfBarAggregationD1Symbol(const string & symbol) const
{
  if (m_BarAggregationD1Symbols.find(symbol) == m_BarAggregationD1Symbols.end()) return false;
  else return true;
}

double SystemConfig::GetErroneousTickPxChg() const
{
  return m_ErroneousTickPxChg;
}

bool SystemConfig::CheckIfSymToBeWarmedUpWithDayBars(const string & symbol) const
{
  if (m_SymToBeWarmedUpWithDayBars.find(symbol) == m_SymToBeWarmedUpWithDayBars.end()) return false;
  else return true;
}

void SystemConfig::ReadConfig(const string & sConfigPath)
{
  m_Logger->Write(Logger::INFO,"SystemConfig: Reading Config file: %s", sConfigPath.c_str());

  // Read config file
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(sConfigPath, pt);

  string sLogPath = STool::Trim(pt.get<std::string>("SystemSettings.LogPath"));
  m_Logger->SetPath(sLogPath.c_str());

  string sLogLevel = STool::Trim(pt.get<std::string>("SystemSettings.LogLevel"));
  if      (sLogLevel == "EMERGENCY") { m_Logger->SetLogLevel(Logger::EMERGENCY); }
  else if (sLogLevel == "ALERT")     { m_Logger->SetLogLevel(Logger::ALERT);     }
  else if (sLogLevel == "CRITICAL")  { m_Logger->SetLogLevel(Logger::CRITICAL);  }
  else if (sLogLevel == "ERROR")     { m_Logger->SetLogLevel(Logger::ERROR);     }
  else if (sLogLevel == "WARNING")   { m_Logger->SetLogLevel(Logger::WARNING);   }
  else if (sLogLevel == "NOTICE")    { m_Logger->SetLogLevel(Logger::NOTICE);    }
  else if (sLogLevel == "INFO")      { m_Logger->SetLogLevel(Logger::INFO);      }
  else if (sLogLevel == "DEBUG")     { m_Logger->SetLogLevel(Logger::DEBUG);     }

  m_SymbolsToBeUsedInAllSections       = STool::Trim(pt.get<string>   ("SystemSettings.SymbolsToBeUsedInAllSections"));
  m_MainLogPath                        = STool::Trim(pt.get<string>   ("SystemSettings.LogPath"));
  m_MTMLogPath                         = STool::Trim(pt.get<string>   ("SystemSettings.MTMLogPath"));
  m_MTMBySymLogPath                    = STool::Trim(pt.get<string>   ("SystemSettings.MTMBySymLogPath"));
  m_ExecLogPath                        = STool::Trim(pt.get<string>   ("SystemSettings.ExecLogPath"));
  m_MTMHoldingsLogPath                 = STool::Trim(pt.get<string>   ("SystemSettings.HoldingsLogPath"));
  m_SupplementaryBarD1Path             = STool::Trim(pt.get<string>   ("SystemSettings.SupplementaryBarD1Path"));
  m_SupplementaryBarM1Path             = STool::Trim(pt.get<string>   ("SystemSettings.SupplementaryBarM1Path"));
  m_PositionPersistenceFile            = STool::Trim(pt.get<string>   ("SystemSettings.PositionPersistenceFile"));
  m_LoadItrdHighLowFromIB              = STool::Trim(pt.get<string>   ("SystemSettings.LoadItrdHighLowFromIB"));

  m_MaxAllowedTradeNotional            = pt.get<double>   ("SystemSettings.MaxAllowedTradeNotional");
  m_MaxAllowedTradeQty                 = pt.get<long>     ("SystemSettings.MaxAllowedTradeQty");

  if (m_SymbolsToBeUsedInAllSections != "") m_Logger->Write(Logger::INFO,"SystemConfig: Read SymbolsToBeUsedInAllSections: (truncated to 1024) %s", m_SymbolsToBeUsedInAllSections.substr(0,1024).c_str());
  else m_Logger->Write(Logger::INFO,"SystemConfig: Read SymbolsToBeUsedInAllSections: Nil");

  m_Logger->Write(Logger::INFO,"SystemConfig: MaxAllowedTradeNotional %f", m_MaxAllowedTradeNotional);
  m_Logger->Write(Logger::INFO,"SystemConfig: MaxAllowedTradeQty %d", m_MaxAllowedTradeQty);

  m_SupplementaryBarLeadingZeroAdj     = pt.get<bool>           ("SystemSettings.SupplementaryBarFileLeadingZeroAdj");
  m_PositionPersistenceIsEnabled       = pt.get<bool>           ("SystemSettings.EnablePositionPersistence");
  m_SendResetOnConnectionToCAPI        = pt.get<bool>           ("SystemSettings.SendResetOnConnectionToCAPI");

  m_SystemServicePort                  = pt.get<unsigned short> ("SystemSettings.SystemServicePort");

  string sDefaultTimeZone;
  boost::optional<string> o_DefaultTimeZone = pt.get_optional<string>("SystemSettings.DefaultTimeZone");
  if (o_DefaultTimeZone) sDefaultTimeZone = STool::Trim(o_DefaultTimeZone.get());
  if (sDefaultTimeZone == "HKT") m_DefaultTimeZone = SDateTime::HKT;
  if (sDefaultTimeZone == "EST") m_DefaultTimeZone = SDateTime::EST;
  if (sDefaultTimeZone == "CST") m_DefaultTimeZone = SDateTime::CST;
  if (sDefaultTimeZone == "GMT") m_DefaultTimeZone = SDateTime::GMT;

  boost::optional<string>  o_HKIntradaySeasonalityPath     = pt.get_optional<string>("HKIntradaySeasonality.TrainingResultPath");
  boost::optional<bool>    o_HKIntradaySeasonalityOnOff    = pt.get_optional<bool>    ("HKIntradaySeasonality.OnOff");
  boost::optional<int>     o_HKIntradaySeasonalityWindow   = pt.get_optional<int>     ("HKIntradaySeasonality.WindowLength");

  if(o_HKIntradaySeasonalityPath  )  m_HKIntradaySeasonalityPath    = STool::Trim(o_HKIntradaySeasonalityPath.get());
  if(o_HKIntradaySeasonalityOnOff )  m_HKIntradaySeasonalityOnOff   = o_HKIntradaySeasonalityOnOff  .get();
  if(o_HKIntradaySeasonalityWindow)  m_HKIntradaySeasonalityWindow  = o_HKIntradaySeasonalityWindow .get();

  m_TradingHoursPath                   = STool::Trim(pt.get<string>   ("StaticData.TradingHoursPath"));
  m_HKFE_CalendarPath                  = STool::Trim(pt.get<string>   ("StaticData.HKFE_CalendarPath"));
  m_HKSE_HSIConstituentsPath           = STool::Trim(pt.get<string>   ("StaticData.HKSE_HSIConstituentsPath"));
  m_HKMA_ExchgFundBillPath             = STool::Trim(pt.get<string>   ("StaticData.HKMA_ExchgFundBillPath"));
  m_CorrelMatricesPath                 = STool::Trim(pt.get<string>   ("StaticData.CorrelMatricesPath"));
  m_VolSurfParamFile1FM                = STool::Trim(pt.get<string>   ("VolatilitySurface.ParamFile1FM"));
  m_VolSurfParamFile2FM                = STool::Trim(pt.get<string>   ("VolatilitySurface.ParamFile2FM"));
  m_ProbDistrFileFSMC1D                = STool::Trim(pt.get<string>   ("ProbDistribution.ProbDistrFileFSMC1D"));
  m_VolSurfCalcIntervalInSec           = pt.get<long>           ("VolatilitySurface.CalcIntervalInSeconds");
  m_ProbDistrnCalcIntervalInSec        = pt.get<long>           ("ProbDistribution.CalcIntervalInSeconds");
  m_TechIndUpdateIntervalInSec         = pt.get<long>           ("TechIndicators.CalcIntervalInSeconds");
  m_GKYZ_Window_Size                   = pt.get<long>           ("TechIndicators.GKYZ_WindowSize");


  boost::optional<int>    o_B1_HKF_SamplingIntervalInSec       = pt.get_optional<int>   ("Strategy_B1_HKF.SamplingIntervalInSec");
  boost::optional<string> o_S11A_ParamFileHistSDCorrel         = pt.get_optional<string>("Strategy_S11A.ParamFileHistSDCorrel");
  boost::optional<string> o_S11A_ParamFileHSIHHIIV             = pt.get_optional<string>("Strategy_S11A.ParamFileHSIHHIIV");
  boost::optional<string> o_S11A_ParamFileT1GumbelCauchy       = pt.get_optional<string>("Strategy_S11A.ParamFileT1GumbelCauchy");
  boost::optional<string> o_S11A_ParamFileT1GumbelGauss        = pt.get_optional<string>("Strategy_S11A.ParamFileT1GumbelGauss");
  boost::optional<string> o_S11A_ParamFileT2Copula             = pt.get_optional<string>("Strategy_S11A.ParamFileT2Copula");

  if(o_B1_HKF_SamplingIntervalInSec   )  m_B1_HKF_SamplingIntervalInSec     = o_B1_HKF_SamplingIntervalInSec                .get();
  if(o_S11A_ParamFileHistSDCorrel     )  m_S11A_ParamFileHistSDCorrel       = STool::Trim(o_S11A_ParamFileHistSDCorrel      .get());
  if(o_S11A_ParamFileHSIHHIIV         )  m_S11A_ParamFileHSIHHIIV           = STool::Trim(o_S11A_ParamFileHSIHHIIV          .get());
  if(o_S11A_ParamFileT1GumbelCauchy   )  m_S11A_ParamFileT1GumbelCauchy     = STool::Trim(o_S11A_ParamFileT1GumbelCauchy    .get());
  if(o_S11A_ParamFileT1GumbelGauss    )  m_S11A_ParamFileT1GumbelGauss      = STool::Trim(o_S11A_ParamFileT1GumbelGauss     .get());
  if(o_S11A_ParamFileT2Copula         )  m_S11A_ParamFileT2Copula           = STool::Trim(o_S11A_ParamFileT2Copula          .get());

  boost::optional<bool>   o_B2_US1_HasEnabledMinCommissionCheck        = pt.get_optional<bool>    ("Strategy_B2_US1.EnableMinCommissionCheck");
  boost::optional<double> o_B2_US1_CommissionRateThreshold             = pt.get_optional<double>  ("Strategy_B2_US1.CommissionRateThreshold");
  boost::optional<int>    o_B2_US1_HasEnabledRotationMode              = pt.get_optional<int>     ("Strategy_B2_US1.EnableRotationMode");
  boost::optional<bool>   o_B2_US1_RotationModeTradeHighestReturn      = pt.get_optional<bool>    ("Strategy_B2_US1.RotationModeTradeHighestReturn");
  boost::optional<bool>   o_B2_US1_RotationModeUseVolyAdjdReturn       = pt.get_optional<bool>    ("Strategy_B2_US1.RotationModeUseVolyAdjdReturn");
  boost::optional<int>    o_B2_US1_RotationModeUseNDayReturn           = pt.get_optional<int>     ("Strategy_B2_US1.RotationModeUseNDayReturn");
  boost::optional<bool>   o_B2_US1_PersistTheoPosCPnL                  = pt.get_optional<bool>    ("Strategy_B2_US1.PersistTheoPosCPnL");
  boost::optional<double> o_B2_US1_ChooseBestNRotationGroupProp        = pt.get_optional<double>  ("Strategy_B2_US1.ChooseBestNRotationGroupProp");
  boost::optional<int>    o_B2_US1_ActionTimeBefCloseInSec             = pt.get_optional<int>     ("Strategy_B2_US1.ActionTimeBefCloseInSec");
  boost::optional<int>    o_B2_US1_ActionTimeAftOpenInSec              = pt.get_optional<int>     ("Strategy_B2_US1.ActionTimeAftOpenInSec");
  boost::optional<bool>   o_B2_US1_MoveNextBestGroupUpIfNoSignal       = pt.get_optional<bool>    ("Strategy_B2_US1.MoveNextBestGroupUpIfNoSignal");
  boost::optional<bool>   o_B2_US1_MoveNextBestStkInGrpUpIfNoSignal    = pt.get_optional<bool>    ("Strategy_B2_US1.MoveNextBestStkInGrpUpIfNoSignal");
  boost::optional<double> o_B2_US1_AvgAggSgndNotnlThresh               = pt.get_optional<double>  ("Strategy_B2_US1.AvgAggSgndNotnlThresh");
  boost::optional<string> o_B2_US1_TheoCPnLHistFolder                  = pt.get_optional<string>  ("Strategy_B2_US1.TheoCPnLHistFolder");
  boost::optional<string> o_B2_US1_TheoPosFolder                       = pt.get_optional<string>  ("Strategy_B2_US1.TheoPosFolder");
  boost::optional<string> o_B2_US1_RotationGroupFile                   = pt.get_optional<string>  ("Strategy_B2_US1.RotationGroupFile");
  boost::optional<string> o_B2_US1_Arima_ZMQ_IP_Port                   = pt.get_optional<string>  ("Strategy_B2_US1.ArimaZMQIPPort");
  boost::optional<bool>   o_B2_US1_PerformCppTaylor                    = pt.get_optional<bool>    ("Strategy_B2_US1.PerformCppTaylor");
  boost::optional<bool>   o_B2_US1_SymbolStickiness                    = pt.get_optional<bool>    ("Strategy_B2_US1.SymbolStickiness");
  boost::optional<double> o_B2_US1_TaylorWeight                        = pt.get_optional<double>  ("Strategy_B2_US1.TaylorWeight");
  boost::optional<double> o_B2_US1_ArimaWeight                         = pt.get_optional<double>  ("Strategy_B2_US1.ArimaWeight");

  if(o_B2_US1_HasEnabledMinCommissionCheck        )  m_B2_US1_HasEnabledMinCommissionCheck         = o_B2_US1_HasEnabledMinCommissionCheck        .get();
  if(o_B2_US1_CommissionRateThreshold             )  m_B2_US1_CommissionRateThreshold              = o_B2_US1_CommissionRateThreshold             .get();
  if(o_B2_US1_HasEnabledRotationMode              )  m_B2_US1_HasEnabledRotationMode               = o_B2_US1_HasEnabledRotationMode              .get();
  if(o_B2_US1_RotationModeTradeHighestReturn      )  m_B2_US1_RotationModeTradeHighestReturn       = o_B2_US1_RotationModeTradeHighestReturn      .get();
  if(o_B2_US1_RotationModeUseVolyAdjdReturn       )  m_B2_US1_RotationModeUseVolyAdjdReturn        = o_B2_US1_RotationModeUseVolyAdjdReturn       .get();
  if(o_B2_US1_RotationModeUseNDayReturn           )  m_B2_US1_RotationModeUseNDayReturn            = o_B2_US1_RotationModeUseNDayReturn           .get();
  if(o_B2_US1_PersistTheoPosCPnL                  )  m_B2_US1_PersistTheoPosCPnL                   = o_B2_US1_PersistTheoPosCPnL                  .get();
  if(o_B2_US1_ChooseBestNRotationGroupProp        )  m_B2_US1_ChooseBestNRotationGroupProp         = o_B2_US1_ChooseBestNRotationGroupProp        .get();
  if(o_B2_US1_ActionTimeBefCloseInSec             )  m_B2_US1_ActionTimeBefCloseInSec              = o_B2_US1_ActionTimeBefCloseInSec             .get();
  if(o_B2_US1_ActionTimeAftOpenInSec              )  m_B2_US1_ActionTimeAftOpenInSec               = o_B2_US1_ActionTimeAftOpenInSec              .get();
  if(o_B2_US1_MoveNextBestGroupUpIfNoSignal       )  m_B2_US1_MoveNextBestGroupUpIfNoSignal        = o_B2_US1_MoveNextBestGroupUpIfNoSignal       .get();
  if(o_B2_US1_MoveNextBestStkInGrpUpIfNoSignal    )  m_B2_US1_MoveNextBestStkInGrpUpIfNoSignal     = o_B2_US1_MoveNextBestStkInGrpUpIfNoSignal    .get();
  if(o_B2_US1_AvgAggSgndNotnlThresh               )  m_B2_US1_AvgAggSgndNotnlThresh                = o_B2_US1_AvgAggSgndNotnlThresh               .get();
  if(o_B2_US1_TheoCPnLHistFolder                  )  m_B2_US1_TheoCPnLHistFolder                   = STool::Trim(o_B2_US1_TheoCPnLHistFolder      .get());
  if(o_B2_US1_TheoPosFolder                       )  m_B2_US1_TheoPosFolder                        = STool::Trim(o_B2_US1_TheoPosFolder           .get());
  if(o_B2_US1_RotationGroupFile                   )  m_B2_US1_RotationGroupFile                    = STool::Trim(o_B2_US1_RotationGroupFile       .get());
  if(o_B2_US1_Arima_ZMQ_IP_Port                   )  m_B2_US1_Arima_ZMQ_IP_Port                    = STool::Trim(o_B2_US1_Arima_ZMQ_IP_Port       .get());
  if(o_B2_US1_PerformCppTaylor                    )  m_B2_US1_PerformCppTaylor                     = o_B2_US1_PerformCppTaylor                    .get();
  if(o_B2_US1_SymbolStickiness                    )  m_B2_US1_SymbolStickiness                     = o_B2_US1_SymbolStickiness                    .get();
  if(o_B2_US1_TaylorWeight                        )  m_B2_US1_TaylorWeight                         = o_B2_US1_TaylorWeight                        .get();
  if(o_B2_US1_ArimaWeight                         )  m_B2_US1_ArimaWeight                          = o_B2_US1_ArimaWeight                         .get();

  boost::optional<bool>   o_B2_US2_HasEnabledMinCommissionCheck        = pt.get_optional<bool>    ("Strategy_B2_US2.EnableMinCommissionCheck");
  boost::optional<double> o_B2_US2_CommissionRateThreshold             = pt.get_optional<double>  ("Strategy_B2_US2.CommissionRateThreshold");
  boost::optional<int>    o_B2_US2_HasEnabledRotationMode              = pt.get_optional<int>     ("Strategy_B2_US2.EnableRotationMode");
  boost::optional<bool>   o_B2_US2_RotationModeTradeHighestReturn      = pt.get_optional<bool>    ("Strategy_B2_US2.RotationModeTradeHighestReturn");
  boost::optional<bool>   o_B2_US2_RotationModeUseVolyAdjdReturn       = pt.get_optional<bool>    ("Strategy_B2_US2.RotationModeUseVolyAdjdReturn");
  boost::optional<int>    o_B2_US2_RotationModeUseNDayReturn           = pt.get_optional<int>     ("Strategy_B2_US2.RotationModeUseNDayReturn");
  boost::optional<bool>   o_B2_US2_PersistTheoPosCPnL                  = pt.get_optional<bool>    ("Strategy_B2_US2.PersistTheoPosCPnL");
  boost::optional<double> o_B2_US2_ChooseBestNRotationGroupProp        = pt.get_optional<double>  ("Strategy_B2_US2.ChooseBestNRotationGroupProp");
  boost::optional<int>    o_B2_US2_ActionTimeBefCloseInSec             = pt.get_optional<int>     ("Strategy_B2_US2.ActionTimeBefCloseInSec");
  boost::optional<int>    o_B2_US2_ActionTimeAftOpenInSec              = pt.get_optional<int>     ("Strategy_B2_US2.ActionTimeAftOpenInSec");
  boost::optional<bool>   o_B2_US2_MoveNextBestGroupUpIfNoSignal       = pt.get_optional<bool>    ("Strategy_B2_US2.MoveNextBestGroupUpIfNoSignal");
  boost::optional<bool>   o_B2_US2_MoveNextBestStkInGrpUpIfNoSignal    = pt.get_optional<bool>    ("Strategy_B2_US2.MoveNextBestStkInGrpUpIfNoSignal");
  boost::optional<double> o_B2_US2_AvgAggSgndNotnlThresh               = pt.get_optional<double>  ("Strategy_B2_US2.AvgAggSgndNotnlThresh");
  boost::optional<string> o_B2_US2_TheoCPnLHistFolder                  = pt.get_optional<string>  ("Strategy_B2_US2.TheoCPnLHistFolder");
  boost::optional<string> o_B2_US2_TheoPosFolder                       = pt.get_optional<string>  ("Strategy_B2_US2.TheoPosFolder");
  boost::optional<string> o_B2_US2_RotationGroupFile                   = pt.get_optional<string>  ("Strategy_B2_US2.RotationGroupFile");
  boost::optional<string> o_B2_US2_Arima_ZMQ_IP_Port                   = pt.get_optional<string>  ("Strategy_B2_US2.ArimaZMQIPPort");
  boost::optional<bool>   o_B2_US2_PerformCppTaylor                    = pt.get_optional<bool>    ("Strategy_B2_US2.PerformCppTaylor");
  boost::optional<bool>   o_B2_US2_SymbolStickiness                    = pt.get_optional<bool>    ("Strategy_B2_US2.SymbolStickiness");
  boost::optional<double> o_B2_US2_TaylorWeight                        = pt.get_optional<double>  ("Strategy_B2_US2.TaylorWeight");
  boost::optional<double> o_B2_US2_ArimaWeight                         = pt.get_optional<double>  ("Strategy_B2_US2.ArimaWeight");

  if(o_B2_US2_HasEnabledMinCommissionCheck        )  m_B2_US2_HasEnabledMinCommissionCheck         = o_B2_US2_HasEnabledMinCommissionCheck        .get();
  if(o_B2_US2_CommissionRateThreshold             )  m_B2_US2_CommissionRateThreshold              = o_B2_US2_CommissionRateThreshold             .get();
  if(o_B2_US2_HasEnabledRotationMode              )  m_B2_US2_HasEnabledRotationMode               = o_B2_US2_HasEnabledRotationMode              .get();
  if(o_B2_US2_RotationModeTradeHighestReturn      )  m_B2_US2_RotationModeTradeHighestReturn       = o_B2_US2_RotationModeTradeHighestReturn      .get();
  if(o_B2_US2_RotationModeUseVolyAdjdReturn       )  m_B2_US2_RotationModeUseVolyAdjdReturn        = o_B2_US2_RotationModeUseVolyAdjdReturn       .get();
  if(o_B2_US2_RotationModeUseNDayReturn           )  m_B2_US2_RotationModeUseNDayReturn            = o_B2_US2_RotationModeUseNDayReturn           .get();
  if(o_B2_US2_PersistTheoPosCPnL                  )  m_B2_US2_PersistTheoPosCPnL                   = o_B2_US2_PersistTheoPosCPnL                  .get();
  if(o_B2_US2_ChooseBestNRotationGroupProp        )  m_B2_US2_ChooseBestNRotationGroupProp         = o_B2_US2_ChooseBestNRotationGroupProp        .get();
  if(o_B2_US2_ActionTimeBefCloseInSec             )  m_B2_US2_ActionTimeBefCloseInSec              = o_B2_US2_ActionTimeBefCloseInSec             .get();
  if(o_B2_US2_ActionTimeAftOpenInSec              )  m_B2_US2_ActionTimeAftOpenInSec               = o_B2_US2_ActionTimeAftOpenInSec              .get();
  if(o_B2_US2_MoveNextBestGroupUpIfNoSignal       )  m_B2_US2_MoveNextBestGroupUpIfNoSignal        = o_B2_US2_MoveNextBestGroupUpIfNoSignal       .get();
  if(o_B2_US2_MoveNextBestStkInGrpUpIfNoSignal    )  m_B2_US2_MoveNextBestStkInGrpUpIfNoSignal     = o_B2_US2_MoveNextBestStkInGrpUpIfNoSignal    .get();
  if(o_B2_US2_AvgAggSgndNotnlThresh               )  m_B2_US2_AvgAggSgndNotnlThresh                = o_B2_US2_AvgAggSgndNotnlThresh               .get();
  if(o_B2_US2_TheoCPnLHistFolder                  )  m_B2_US2_TheoCPnLHistFolder                   = STool::Trim(o_B2_US2_TheoCPnLHistFolder      .get());
  if(o_B2_US2_TheoPosFolder                       )  m_B2_US2_TheoPosFolder                        = STool::Trim(o_B2_US2_TheoPosFolder           .get());
  if(o_B2_US2_RotationGroupFile                   )  m_B2_US2_RotationGroupFile                    = STool::Trim(o_B2_US2_RotationGroupFile       .get());
  if(o_B2_US2_Arima_ZMQ_IP_Port                   )  m_B2_US2_Arima_ZMQ_IP_Port                    = STool::Trim(o_B2_US2_Arima_ZMQ_IP_Port       .get());
  if(o_B2_US2_PerformCppTaylor                    )  m_B2_US2_PerformCppTaylor                     = o_B2_US2_PerformCppTaylor                    .get();
  if(o_B2_US2_SymbolStickiness                    )  m_B2_US2_SymbolStickiness                     = o_B2_US2_SymbolStickiness                    .get();
  if(o_B2_US2_TaylorWeight                        )  m_B2_US2_TaylorWeight                         = o_B2_US2_TaylorWeight                        .get();
  if(o_B2_US2_ArimaWeight                         )  m_B2_US2_ArimaWeight                          = o_B2_US2_ArimaWeight                         .get();

  boost::optional<bool>   o_B2_US3_HasEnabledMinCommissionCheck        = pt.get_optional<bool>    ("Strategy_B2_US3.EnableMinCommissionCheck");
  boost::optional<double> o_B2_US3_CommissionRateThreshold             = pt.get_optional<double>  ("Strategy_B2_US3.CommissionRateThreshold");
  boost::optional<int>    o_B2_US3_HasEnabledRotationMode              = pt.get_optional<int>     ("Strategy_B2_US3.EnableRotationMode");
  boost::optional<bool>   o_B2_US3_RotationModeTradeHighestReturn      = pt.get_optional<bool>    ("Strategy_B2_US3.RotationModeTradeHighestReturn");
  boost::optional<bool>   o_B2_US3_RotationModeUseVolyAdjdReturn       = pt.get_optional<bool>    ("Strategy_B2_US3.RotationModeUseVolyAdjdReturn");
  boost::optional<int>    o_B2_US3_RotationModeUseNDayReturn           = pt.get_optional<int>     ("Strategy_B2_US3.RotationModeUseNDayReturn");
  boost::optional<bool>   o_B2_US3_PersistTheoPosCPnL                  = pt.get_optional<bool>    ("Strategy_B2_US3.PersistTheoPosCPnL");
  boost::optional<double> o_B2_US3_ChooseBestNRotationGroupProp        = pt.get_optional<double>  ("Strategy_B2_US3.ChooseBestNRotationGroupProp");
  boost::optional<int>    o_B2_US3_ActionTimeBefCloseInSec             = pt.get_optional<int>     ("Strategy_B2_US3.ActionTimeBefCloseInSec");
  boost::optional<int>    o_B2_US3_ActionTimeAftOpenInSec              = pt.get_optional<int>     ("Strategy_B2_US3.ActionTimeAftOpenInSec");
  boost::optional<bool>   o_B2_US3_MoveNextBestGroupUpIfNoSignal       = pt.get_optional<bool>    ("Strategy_B2_US3.MoveNextBestGroupUpIfNoSignal");
  boost::optional<bool>   o_B2_US3_MoveNextBestStkInGrpUpIfNoSignal    = pt.get_optional<bool>    ("Strategy_B2_US3.MoveNextBestStkInGrpUpIfNoSignal");
  boost::optional<double> o_B2_US3_AvgAggSgndNotnlThresh               = pt.get_optional<double>  ("Strategy_B2_US3.AvgAggSgndNotnlThresh");
  boost::optional<string> o_B2_US3_TheoCPnLHistFolder                  = pt.get_optional<string>  ("Strategy_B2_US3.TheoCPnLHistFolder");
  boost::optional<string> o_B2_US3_TheoPosFolder                       = pt.get_optional<string>  ("Strategy_B2_US3.TheoPosFolder");
  boost::optional<string> o_B2_US3_RotationGroupFile                   = pt.get_optional<string>  ("Strategy_B2_US3.RotationGroupFile");
  boost::optional<string> o_B2_US3_Arima_ZMQ_IP_Port                   = pt.get_optional<string>  ("Strategy_B2_US3.ArimaZMQIPPort");
  boost::optional<bool>   o_B2_US3_PerformCppTaylor                    = pt.get_optional<bool>    ("Strategy_B2_US3.PerformCppTaylor");
  boost::optional<bool>   o_B2_US3_SymbolStickiness                    = pt.get_optional<bool>    ("Strategy_B2_US3.SymbolStickiness");
  boost::optional<double> o_B2_US3_TaylorWeight                        = pt.get_optional<double>  ("Strategy_B2_US3.TaylorWeight");
  boost::optional<double> o_B2_US3_ArimaWeight                         = pt.get_optional<double>  ("Strategy_B2_US3.ArimaWeight");

  if(o_B2_US3_HasEnabledMinCommissionCheck        )  m_B2_US3_HasEnabledMinCommissionCheck         = o_B2_US3_HasEnabledMinCommissionCheck        .get();
  if(o_B2_US3_CommissionRateThreshold             )  m_B2_US3_CommissionRateThreshold              = o_B2_US3_CommissionRateThreshold             .get();
  if(o_B2_US3_HasEnabledRotationMode              )  m_B2_US3_HasEnabledRotationMode               = o_B2_US3_HasEnabledRotationMode              .get();
  if(o_B2_US3_RotationModeTradeHighestReturn      )  m_B2_US3_RotationModeTradeHighestReturn       = o_B2_US3_RotationModeTradeHighestReturn      .get();
  if(o_B2_US3_RotationModeUseVolyAdjdReturn       )  m_B2_US3_RotationModeUseVolyAdjdReturn        = o_B2_US3_RotationModeUseVolyAdjdReturn       .get();
  if(o_B2_US3_RotationModeUseNDayReturn           )  m_B2_US3_RotationModeUseNDayReturn            = o_B2_US3_RotationModeUseNDayReturn           .get();
  if(o_B2_US3_PersistTheoPosCPnL                  )  m_B2_US3_PersistTheoPosCPnL                   = o_B2_US3_PersistTheoPosCPnL                  .get();
  if(o_B2_US3_ChooseBestNRotationGroupProp        )  m_B2_US3_ChooseBestNRotationGroupProp         = o_B2_US3_ChooseBestNRotationGroupProp        .get();
  if(o_B2_US3_ActionTimeBefCloseInSec             )  m_B2_US3_ActionTimeBefCloseInSec              = o_B2_US3_ActionTimeBefCloseInSec             .get();
  if(o_B2_US3_ActionTimeAftOpenInSec              )  m_B2_US3_ActionTimeAftOpenInSec               = o_B2_US3_ActionTimeAftOpenInSec              .get();
  if(o_B2_US3_MoveNextBestGroupUpIfNoSignal       )  m_B2_US3_MoveNextBestGroupUpIfNoSignal        = o_B2_US3_MoveNextBestGroupUpIfNoSignal       .get();
  if(o_B2_US3_MoveNextBestStkInGrpUpIfNoSignal    )  m_B2_US3_MoveNextBestStkInGrpUpIfNoSignal     = o_B2_US3_MoveNextBestStkInGrpUpIfNoSignal    .get();
  if(o_B2_US3_AvgAggSgndNotnlThresh               )  m_B2_US3_AvgAggSgndNotnlThresh                = o_B2_US3_AvgAggSgndNotnlThresh               .get();
  if(o_B2_US3_TheoCPnLHistFolder                  )  m_B2_US3_TheoCPnLHistFolder                   = STool::Trim(o_B2_US3_TheoCPnLHistFolder      .get());
  if(o_B2_US3_TheoPosFolder                       )  m_B2_US3_TheoPosFolder                        = STool::Trim(o_B2_US3_TheoPosFolder           .get());
  if(o_B2_US3_RotationGroupFile                   )  m_B2_US3_RotationGroupFile                    = STool::Trim(o_B2_US3_RotationGroupFile       .get());
  if(o_B2_US3_Arima_ZMQ_IP_Port                   )  m_B2_US3_Arima_ZMQ_IP_Port                    = STool::Trim(o_B2_US3_Arima_ZMQ_IP_Port       .get());
  if(o_B2_US3_PerformCppTaylor                    )  m_B2_US3_PerformCppTaylor                     = o_B2_US3_PerformCppTaylor                    .get();
  if(o_B2_US3_SymbolStickiness                    )  m_B2_US3_SymbolStickiness                     = o_B2_US3_SymbolStickiness                    .get();
  if(o_B2_US3_TaylorWeight                        )  m_B2_US3_TaylorWeight                         = o_B2_US3_TaylorWeight                        .get();
  if(o_B2_US3_ArimaWeight                         )  m_B2_US3_ArimaWeight                          = o_B2_US3_ArimaWeight                         .get();

  boost::optional<bool>   o_B2_HK_HasEnabledMinCommissionCheck        = pt.get_optional<bool>    ("Strategy_B2_HK.EnableMinCommissionCheck");
  boost::optional<double> o_B2_HK_CommissionRateThreshold             = pt.get_optional<double>  ("Strategy_B2_HK.CommissionRateThreshold");
  boost::optional<int>    o_B2_HK_HasEnabledRotationMode              = pt.get_optional<int>     ("Strategy_B2_HK.EnableRotationMode");
  boost::optional<bool>   o_B2_HK_RotationModeTradeHighestReturn      = pt.get_optional<bool>    ("Strategy_B2_HK.RotationModeTradeHighestReturn");
  boost::optional<bool>   o_B2_HK_RotationModeUseVolyAdjdReturn       = pt.get_optional<bool>    ("Strategy_B2_HK.RotationModeUseVolyAdjdReturn");
  boost::optional<int>    o_B2_HK_RotationModeUseNDayReturn           = pt.get_optional<int>     ("Strategy_B2_HK.RotationModeUseNDayReturn");
  boost::optional<bool>   o_B2_HK_PersistTheoPosCPnL                  = pt.get_optional<bool>    ("Strategy_B2_HK.PersistTheoPosCPnL");
  boost::optional<double> o_B2_HK_ChooseBestNRotationGroupProp        = pt.get_optional<double>  ("Strategy_B2_HK.ChooseBestNRotationGroupProp");
  boost::optional<int>    o_B2_HK_ActionTimeBefCloseInSec             = pt.get_optional<int>     ("Strategy_B2_HK.ActionTimeBefCloseInSec");
  boost::optional<int>    o_B2_HK_ActionTimeAftOpenInSec              = pt.get_optional<int>     ("Strategy_B2_HK.ActionTimeAftOpenInSec");
  boost::optional<bool>   o_B2_HK_MoveNextBestGroupUpIfNoSignal       = pt.get_optional<bool>    ("Strategy_B2_HK.MoveNextBestGroupUpIfNoSignal");
  boost::optional<bool>   o_B2_HK_MoveNextBestStkInGrpUpIfNoSignal    = pt.get_optional<bool>    ("Strategy_B2_HK.MoveNextBestStkInGrpUpIfNoSignal");
  boost::optional<double> o_B2_HK_AvgAggSgndNotnlThresh               = pt.get_optional<double>  ("Strategy_B2_HK.AvgAggSgndNotnlThresh");
  boost::optional<string> o_B2_HK_TheoCPnLHistFolder                  = pt.get_optional<string>  ("Strategy_B2_HK.TheoCPnLHistFolder");
  boost::optional<string> o_B2_HK_TheoPosFolder                       = pt.get_optional<string>  ("Strategy_B2_HK.TheoPosFolder");
  boost::optional<string> o_B2_HK_RotationGroupFile                   = pt.get_optional<string>  ("Strategy_B2_HK.RotationGroupFile");
  boost::optional<string> o_B2_HK_Arima_ZMQ_IP_Port                   = pt.get_optional<string>  ("Strategy_B2_HK.ArimaZMQIPPort");
  boost::optional<bool>   o_B2_HK_PerformCppTaylor                    = pt.get_optional<bool>    ("Strategy_B2_HK.PerformCppTaylor");
  boost::optional<bool>   o_B2_HK_SymbolStickiness                    = pt.get_optional<bool>    ("Strategy_B2_HK.SymbolStickiness");
  boost::optional<double> o_B2_HK_TaylorWeight                        = pt.get_optional<double>  ("Strategy_B2_HK.TaylorWeight");
  boost::optional<double> o_B2_HK_ArimaWeight                         = pt.get_optional<double>  ("Strategy_B2_HK.ArimaWeight");

  if(o_B2_HK_HasEnabledMinCommissionCheck        )  m_B2_HK_HasEnabledMinCommissionCheck        = o_B2_HK_HasEnabledMinCommissionCheck        .get();
  if(o_B2_HK_CommissionRateThreshold             )  m_B2_HK_CommissionRateThreshold             = o_B2_HK_CommissionRateThreshold             .get();
  if(o_B2_HK_HasEnabledRotationMode              )  m_B2_HK_HasEnabledRotationMode              = o_B2_HK_HasEnabledRotationMode              .get();
  if(o_B2_HK_RotationModeTradeHighestReturn      )  m_B2_HK_RotationModeTradeHighestReturn      = o_B2_HK_RotationModeTradeHighestReturn      .get();
  if(o_B2_HK_RotationModeUseVolyAdjdReturn       )  m_B2_HK_RotationModeUseVolyAdjdReturn       = o_B2_HK_RotationModeUseVolyAdjdReturn       .get();
  if(o_B2_HK_RotationModeUseNDayReturn           )  m_B2_HK_RotationModeUseNDayReturn           = o_B2_HK_RotationModeUseNDayReturn           .get();
  if(o_B2_HK_PersistTheoPosCPnL                  )  m_B2_HK_PersistTheoPosCPnL                  = o_B2_HK_PersistTheoPosCPnL                  .get();
  if(o_B2_HK_ChooseBestNRotationGroupProp        )  m_B2_HK_ChooseBestNRotationGroupProp        = o_B2_HK_ChooseBestNRotationGroupProp        .get();
  if(o_B2_HK_ActionTimeBefCloseInSec             )  m_B2_HK_ActionTimeBefCloseInSec             = o_B2_HK_ActionTimeBefCloseInSec             .get();
  if(o_B2_HK_ActionTimeAftOpenInSec              )  m_B2_HK_ActionTimeAftOpenInSec              = o_B2_HK_ActionTimeAftOpenInSec              .get();
  if(o_B2_HK_MoveNextBestGroupUpIfNoSignal       )  m_B2_HK_MoveNextBestGroupUpIfNoSignal       = o_B2_HK_MoveNextBestGroupUpIfNoSignal       .get();
  if(o_B2_HK_MoveNextBestStkInGrpUpIfNoSignal    )  m_B2_HK_MoveNextBestStkInGrpUpIfNoSignal    = o_B2_HK_MoveNextBestStkInGrpUpIfNoSignal    .get();
  if(o_B2_HK_AvgAggSgndNotnlThresh               )  m_B2_HK_AvgAggSgndNotnlThresh               = o_B2_HK_AvgAggSgndNotnlThresh               .get();
  if(o_B2_HK_TheoCPnLHistFolder                  )  m_B2_HK_TheoCPnLHistFolder                  = STool::Trim(o_B2_HK_TheoCPnLHistFolder      .get());
  if(o_B2_HK_TheoPosFolder                       )  m_B2_HK_TheoPosFolder                       = STool::Trim(o_B2_HK_TheoPosFolder           .get());
  if(o_B2_HK_RotationGroupFile                   )  m_B2_HK_RotationGroupFile                   = STool::Trim(o_B2_HK_RotationGroupFile       .get());
  if(o_B2_HK_Arima_ZMQ_IP_Port                   )  m_B2_HK_Arima_ZMQ_IP_Port                   = STool::Trim(o_B2_HK_Arima_ZMQ_IP_Port       .get());
  if(o_B2_HK_PerformCppTaylor                    )  m_B2_HK_PerformCppTaylor                    = o_B2_HK_PerformCppTaylor                    .get();
  if(o_B2_HK_SymbolStickiness                    )  m_B2_HK_SymbolStickiness                    = o_B2_HK_SymbolStickiness                    .get();
  if(o_B2_HK_TaylorWeight                        )  m_B2_HK_TaylorWeight                        = o_B2_HK_TaylorWeight                        .get();
  if(o_B2_HK_ArimaWeight                         )  m_B2_HK_ArimaWeight                         = o_B2_HK_ArimaWeight                         .get();

  boost::optional<string> o_B2_US1_LongOnlyWhenClosePriceBelowAvgPrice   = pt.get_optional<string>  ("Strategy_B2_US1.LongOnlyWhenClosePriceBelowAvgPrice");
  boost::optional<string> o_B2_US1_ShortOnlyWhenClosePriceAboveAvgPrice  = pt.get_optional<string>  ("Strategy_B2_US1.ShortOnlyWhenClosePriceAboveAvgPrice");
  boost::optional<string> o_B2_US2_LongOnlyWhenClosePriceBelowAvgPrice   = pt.get_optional<string>  ("Strategy_B2_US2.LongOnlyWhenClosePriceBelowAvgPrice");
  boost::optional<string> o_B2_US2_ShortOnlyWhenClosePriceAboveAvgPrice  = pt.get_optional<string>  ("Strategy_B2_US2.ShortOnlyWhenClosePriceAboveAvgPrice");
  boost::optional<string> o_B2_US3_LongOnlyWhenClosePriceBelowAvgPrice   = pt.get_optional<string>  ("Strategy_B2_US3.LongOnlyWhenClosePriceBelowAvgPrice");
  boost::optional<string> o_B2_US3_ShortOnlyWhenClosePriceAboveAvgPrice  = pt.get_optional<string>  ("Strategy_B2_US3.ShortOnlyWhenClosePriceAboveAvgPrice");
  boost::optional<string> o_B2_HK_LongOnlyWhenClosePriceBelowAvgPrice    = pt.get_optional<string>  ("Strategy_B2_HK.LongOnlyWhenClosePriceBelowAvgPrice");
  boost::optional<string> o_B2_HK_ShortOnlyWhenClosePriceAboveAvgPrice   = pt.get_optional<string>  ("Strategy_B2_HK.ShortOnlyWhenClosePriceAboveAvgPrice");
  if(o_B2_US1_LongOnlyWhenClosePriceBelowAvgPrice ) { string s = STool::Trim(o_B2_US1_LongOnlyWhenClosePriceBelowAvgPrice  .get()); if (s != "") m_B2_US1_LongOnlyWhenClosePriceBelowAvgPrice  = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US1_ShortOnlyWhenClosePriceAboveAvgPrice) { string s = STool::Trim(o_B2_US1_ShortOnlyWhenClosePriceAboveAvgPrice .get()); if (s != "") m_B2_US1_ShortOnlyWhenClosePriceAboveAvgPrice = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US2_LongOnlyWhenClosePriceBelowAvgPrice ) { string s = STool::Trim(o_B2_US2_LongOnlyWhenClosePriceBelowAvgPrice  .get()); if (s != "") m_B2_US2_LongOnlyWhenClosePriceBelowAvgPrice  = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US2_ShortOnlyWhenClosePriceAboveAvgPrice) { string s = STool::Trim(o_B2_US2_ShortOnlyWhenClosePriceAboveAvgPrice .get()); if (s != "") m_B2_US2_ShortOnlyWhenClosePriceAboveAvgPrice = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US3_LongOnlyWhenClosePriceBelowAvgPrice ) { string s = STool::Trim(o_B2_US3_LongOnlyWhenClosePriceBelowAvgPrice  .get()); if (s != "") m_B2_US3_LongOnlyWhenClosePriceBelowAvgPrice  = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US3_ShortOnlyWhenClosePriceAboveAvgPrice) { string s = STool::Trim(o_B2_US3_ShortOnlyWhenClosePriceAboveAvgPrice .get()); if (s != "") m_B2_US3_ShortOnlyWhenClosePriceAboveAvgPrice = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_HK_LongOnlyWhenClosePriceBelowAvgPrice )  { string s = STool::Trim(o_B2_HK_LongOnlyWhenClosePriceBelowAvgPrice   .get()); if (s != "") m_B2_HK_LongOnlyWhenClosePriceBelowAvgPrice   = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_HK_ShortOnlyWhenClosePriceAboveAvgPrice)  { string s = STool::Trim(o_B2_HK_ShortOnlyWhenClosePriceAboveAvgPrice  .get()); if (s != "") m_B2_HK_ShortOnlyWhenClosePriceAboveAvgPrice  = boost::optional<double>(boost::lexical_cast<double>(s));  }

  boost::optional<string> o_B2_US1_LongOnlyWhenAvgPriceReturnAbove   = pt.get_optional<string>  ("Strategy_B2_US1.LongOnlyWhenAvgPriceReturnAbove");
  boost::optional<string> o_B2_US1_ShortOnlyWhenAvgPriceReturnBelow  = pt.get_optional<string>  ("Strategy_B2_US1.ShortOnlyWhenAvgPriceReturnBelow");
  boost::optional<string> o_B2_US2_LongOnlyWhenAvgPriceReturnAbove   = pt.get_optional<string>  ("Strategy_B2_US2.LongOnlyWhenAvgPriceReturnAbove");
  boost::optional<string> o_B2_US2_ShortOnlyWhenAvgPriceReturnBelow  = pt.get_optional<string>  ("Strategy_B2_US2.ShortOnlyWhenAvgPriceReturnBelow");
  boost::optional<string> o_B2_US3_LongOnlyWhenAvgPriceReturnAbove   = pt.get_optional<string>  ("Strategy_B2_US3.LongOnlyWhenAvgPriceReturnAbove");
  boost::optional<string> o_B2_US3_ShortOnlyWhenAvgPriceReturnBelow  = pt.get_optional<string>  ("Strategy_B2_US3.ShortOnlyWhenAvgPriceReturnBelow");
  boost::optional<string> o_B2_HK_LongOnlyWhenAvgPriceReturnAbove    = pt.get_optional<string>  ("Strategy_B2_HK.LongOnlyWhenAvgPriceReturnAbove");
  boost::optional<string> o_B2_HK_ShortOnlyWhenAvgPriceReturnBelow   = pt.get_optional<string>  ("Strategy_B2_HK.ShortOnlyWhenAvgPriceReturnBelow");
  if(o_B2_US1_LongOnlyWhenAvgPriceReturnAbove ) { string s = STool::Trim(o_B2_US1_LongOnlyWhenAvgPriceReturnAbove  .get()); if (s != "") m_B2_US1_LongOnlyWhenAvgPriceReturnAbove  = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US1_ShortOnlyWhenAvgPriceReturnBelow) { string s = STool::Trim(o_B2_US1_ShortOnlyWhenAvgPriceReturnBelow .get()); if (s != "") m_B2_US1_ShortOnlyWhenAvgPriceReturnBelow = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US2_LongOnlyWhenAvgPriceReturnAbove ) { string s = STool::Trim(o_B2_US2_LongOnlyWhenAvgPriceReturnAbove  .get()); if (s != "") m_B2_US2_LongOnlyWhenAvgPriceReturnAbove  = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US2_ShortOnlyWhenAvgPriceReturnBelow) { string s = STool::Trim(o_B2_US2_ShortOnlyWhenAvgPriceReturnBelow .get()); if (s != "") m_B2_US2_ShortOnlyWhenAvgPriceReturnBelow = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US3_LongOnlyWhenAvgPriceReturnAbove ) { string s = STool::Trim(o_B2_US3_LongOnlyWhenAvgPriceReturnAbove  .get()); if (s != "") m_B2_US3_LongOnlyWhenAvgPriceReturnAbove  = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US3_ShortOnlyWhenAvgPriceReturnBelow) { string s = STool::Trim(o_B2_US3_ShortOnlyWhenAvgPriceReturnBelow .get()); if (s != "") m_B2_US3_ShortOnlyWhenAvgPriceReturnBelow = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_HK_LongOnlyWhenAvgPriceReturnAbove )  { string s = STool::Trim(o_B2_HK_LongOnlyWhenAvgPriceReturnAbove   .get()); if (s != "") m_B2_HK_LongOnlyWhenAvgPriceReturnAbove   = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_HK_ShortOnlyWhenAvgPriceReturnBelow)  { string s = STool::Trim(o_B2_HK_ShortOnlyWhenAvgPriceReturnBelow  .get()); if (s != "") m_B2_HK_ShortOnlyWhenAvgPriceReturnBelow  = boost::optional<double>(boost::lexical_cast<double>(s));  }

  boost::optional<string> o_B2_US1_LongOnlyWhenGrpAvgReturnAbove   = pt.get_optional<string>  ("Strategy_B2_US1.LongOnlyWhenGrpAvgReturnAbove");
  boost::optional<string> o_B2_US2_LongOnlyWhenGrpAvgReturnAbove   = pt.get_optional<string>  ("Strategy_B2_US2.LongOnlyWhenGrpAvgReturnAbove");
  boost::optional<string> o_B2_US3_LongOnlyWhenGrpAvgReturnAbove   = pt.get_optional<string>  ("Strategy_B2_US3.LongOnlyWhenGrpAvgReturnAbove");
  boost::optional<string> o_B2_HK_LongOnlyWhenGrpAvgReturnAbove    = pt.get_optional<string>  ("Strategy_B2_HK.LongOnlyWhenGrpAvgReturnAbove");
  if(o_B2_US1_LongOnlyWhenGrpAvgReturnAbove ) { string s = STool::Trim(o_B2_US1_LongOnlyWhenGrpAvgReturnAbove  .get()); if (s != "") m_B2_US1_LongOnlyWhenGrpAvgReturnAbove  = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US2_LongOnlyWhenGrpAvgReturnAbove ) { string s = STool::Trim(o_B2_US2_LongOnlyWhenGrpAvgReturnAbove  .get()); if (s != "") m_B2_US2_LongOnlyWhenGrpAvgReturnAbove  = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_US3_LongOnlyWhenGrpAvgReturnAbove ) { string s = STool::Trim(o_B2_US3_LongOnlyWhenGrpAvgReturnAbove  .get()); if (s != "") m_B2_US3_LongOnlyWhenGrpAvgReturnAbove  = boost::optional<double>(boost::lexical_cast<double>(s));  }
  if(o_B2_HK_LongOnlyWhenGrpAvgReturnAbove )  { string s = STool::Trim(o_B2_HK_LongOnlyWhenGrpAvgReturnAbove   .get()); if (s != "") m_B2_HK_LongOnlyWhenGrpAvgReturnAbove   = boost::optional<double>(boost::lexical_cast<double>(s));  }

  {
    boost::optional<string> o_B2_US1_FilterSMAPeriod = pt.get_optional<string>("Strategy_B2_US1.FilterSMAPeriod");
    if(o_B2_US1_FilterSMAPeriod)
    {
      vector<string> vs;
      boost::split(vs, o_B2_US1_FilterSMAPeriod.get(), boost::is_any_of(","));
      if (!vs.empty() && vs[0] != "") FForEach (vs,[&](const string & sma) { m_B2_US1_FilterSMAPeriod.push_back(boost::lexical_cast<int>(sma)); });
    }
  }

  {
    boost::optional<string> o_B2_US2_FilterSMAPeriod = pt.get_optional<string>("Strategy_B2_US2.FilterSMAPeriod");
    if(o_B2_US2_FilterSMAPeriod)
    {
      vector<string> vs;
      boost::split(vs, o_B2_US2_FilterSMAPeriod.get(), boost::is_any_of(","));
      if (!vs.empty() && vs[0] != "") FForEach (vs,[&](const string & sma) { m_B2_US2_FilterSMAPeriod.push_back(boost::lexical_cast<int>(sma)); });
    }
  }

  {
    boost::optional<string> o_B2_US3_FilterSMAPeriod = pt.get_optional<string>("Strategy_B2_US3.FilterSMAPeriod");
    if(o_B2_US3_FilterSMAPeriod)
    {
      vector<string> vs;
      boost::split(vs, o_B2_US3_FilterSMAPeriod.get(), boost::is_any_of(","));
      if (!vs.empty() && vs[0] != "") FForEach (vs,[&](const string & sma) { m_B2_US3_FilterSMAPeriod.push_back(boost::lexical_cast<int>(sma)); });
    }
  }

  {
    boost::optional<string> o_B2_HK_FilterSMAPeriod = pt.get_optional<string>("Strategy_B2_HK.FilterSMAPeriod");
    if(o_B2_HK_FilterSMAPeriod)
    {
      vector<string> vs;
      boost::split(vs, o_B2_HK_FilterSMAPeriod.get(), boost::is_any_of(","));
      if (!vs.empty() && vs[0] != "") FForEach (vs,[&](const string & sma) { m_B2_HK_FilterSMAPeriod.push_back(boost::lexical_cast<int>(sma)); });
    }
  }

  //--------------------------------------------------

  {
    boost::optional<string> o_B2_US1_RoleOfSym = pt.get_optional<string>("Strategy_B2_US1.RoleOfSym");

    if (o_B2_US1_RoleOfSym)
    {
      string s_B2_US1_RoleOfSym = o_B2_US1_RoleOfSym.get();
      vector<string> vs;
      boost::split(vs, s_B2_US1_RoleOfSym, boost::is_any_of(","));
      if (!vs.empty() && vs[0] != "") FForEach (vs,[&](const string & irole) { m_B2_US1_RoleOfSym.push_back(boost::lexical_cast<int>(irole)); });
    }
  }

  {
    boost::optional<string> o_B2_US2_RoleOfSym = pt.get_optional<string>("Strategy_B2_US2.RoleOfSym");

    if (o_B2_US2_RoleOfSym)
    {
      string s_B2_US2_RoleOfSym = o_B2_US2_RoleOfSym.get();
      vector<string> vs;
      boost::split(vs, s_B2_US2_RoleOfSym, boost::is_any_of(","));
      if (!vs.empty() && vs[0] != "") FForEach (vs,[&](const string & irole) { m_B2_US2_RoleOfSym.push_back(boost::lexical_cast<int>(irole)); });
    }
  }

  {
    boost::optional<string> o_B2_US3_RoleOfSym = pt.get_optional<string>("Strategy_B2_US3.RoleOfSym");

    if (o_B2_US3_RoleOfSym)
    {
      string s_B2_US3_RoleOfSym = o_B2_US3_RoleOfSym.get();
      vector<string> vs;
      boost::split(vs, s_B2_US3_RoleOfSym, boost::is_any_of(","));
      if (!vs.empty() && vs[0] != "") FForEach (vs,[&](const string & irole) { m_B2_US3_RoleOfSym.push_back(boost::lexical_cast<int>(irole)); });
    }
  }

  //--------------------------------------------------
  {
    boost::optional<string> o_B2_HK_RoleOfSym = pt.get_optional<string>("Strategy_B2_HK.RoleOfSym");

    if (o_B2_HK_RoleOfSym)
    {
      string s_B2_HK_RoleOfSym = o_B2_HK_RoleOfSym.get();
      vector<string> vs;
      boost::split(vs, s_B2_HK_RoleOfSym, boost::is_any_of(","));
      if (!vs.empty() && vs[0] != "") FForEach (vs,[&](const string & irole) { m_B2_HK_RoleOfSym.push_back(boost::lexical_cast<int>(irole)); });
    }
  }

  {
    boost::optional<string> o_TradedSym = pt.get_optional<string>("Strategy_B2_US1.TradedSymbolsTradeAtOpen");
    if (o_TradedSym)
    {
      string s_TradedSym = STool::Trim(o_TradedSym.get());
      m_Logger->Write(Logger::INFO,"SystemConfig: m_B2_US1_TradedSymbolsTradeAtOpen (truncated to 1024) %s", s_TradedSym.substr(0,1024).c_str());
      vector<string> vs;
      boost::split(vs, s_TradedSym, boost::is_any_of(","));
      FForEach (vs,[&](const string & s) { m_B2_US1_TradedSymbolsTradeAtOpen.insert(s); });
    }
  }
  {
    boost::optional<string> o_TradedSym = pt.get_optional<string>("Strategy_B2_US2.TradedSymbolsTradeAtOpen");
    if (o_TradedSym)
    {
      string s_TradedSym = STool::Trim(o_TradedSym.get());
      m_Logger->Write(Logger::INFO,"SystemConfig: m_B2_US2_TradedSymbolsTradeAtOpen (truncated to 1024) %s", s_TradedSym.substr(0,1024).c_str());
      vector<string> vs;
      boost::split(vs, s_TradedSym, boost::is_any_of(","));
      FForEach (vs,[&](const string & s) { m_B2_US2_TradedSymbolsTradeAtOpen.insert(s); });
    }
  }
  {
    boost::optional<string> o_TradedSym = pt.get_optional<string>("Strategy_B2_US3.TradedSymbolsTradeAtOpen");
    if (o_TradedSym)
    {
      string s_TradedSym = STool::Trim(o_TradedSym.get());
      m_Logger->Write(Logger::INFO,"SystemConfig: m_B2_US3_TradedSymbolsTradeAtOpen (truncated to 1024) %s", s_TradedSym.substr(0,1024).c_str());
      vector<string> vs;
      boost::split(vs, s_TradedSym, boost::is_any_of(","));
      FForEach (vs,[&](const string & s) { m_B2_US3_TradedSymbolsTradeAtOpen.insert(s); });
    }
  }
  {
    boost::optional<string> o_TradedSym = pt.get_optional<string>("Strategy_B2_HK.TradedSymbolsTradeAtOpen");
    if (o_TradedSym)
    {
      string s_TradedSym = STool::Trim(o_TradedSym.get());
      m_Logger->Write(Logger::INFO,"SystemConfig: m_B2_HK_TradedSymbolsTradeAtOpen (truncated to 1024) %s", s_TradedSym.substr(0,1024).c_str());
      vector<string> vs;
      boost::split(vs, s_TradedSym, boost::is_any_of(","));
      FForEach (vs,[&](const string & s) { m_B2_HK_TradedSymbolsTradeAtOpen.insert(s); });
    }
  }
  //--------------------------------------------------


  m_HKFEContFutRollFwdTime = HHMMSS(pt.get<int>("Strategy_Base.HKFEContFutRollFwdTime"));
  m_CMEContFutRollFwdTime  = HHMMSS(pt.get<int>("Strategy_Base.CMEContFutRollFwdTime"));

  {
    string s_ProceedStyForceExcnEvenIfNoMD = STool::Trim(pt.get<string>("Strategy_Base.ProceedStyForceExcnEvenIfNoMD"));

    vector<string> vs;
    boost::split(vs, s_ProceedStyForceExcnEvenIfNoMD, boost::is_any_of(","));
    for (unsigned int i = 0; i < vs.size(); ++i)
    {
      m_ProceedStyForceExcnEvenIfNoMD.insert(vs[i]);
    }
  }


  {
    for (int sty = STY_NIR; sty != STY_LAST; sty++)
    {
      StrategyID sid = static_cast<StrategyID>(sty);

      map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(sid);
      if (it == m_map_TradingStrategyConfig.end())
      {
        m_map_TradingStrategyConfig[sid] = TradingStrategyConfig();
        it = m_map_TradingStrategyConfig.find(sid);
      }

      TradingStrategyConfig & tsc = it->second;
      string sStratSession = "Strategy_" + GetStrategyName(sid);
      boost::optional<bool> o_OnOff = pt.get_optional<bool>(sStratSession + ".OnOff");
      if (o_OnOff)
      {
        tsc.m_OnOff         = pt.get<bool>     (sStratSession + ".OnOff");
        tsc.m_MTMTime       = pt.get<int>      (sStratSession + ".MTMTime");
        tsc.m_SignalLogPath = STool::Trim(pt.get<string>(sStratSession + ".SignalLogPath"));

        tsc.m_StartTimeBufferInSec  = pt.get<int>(sStratSession  + ".StartTimeBufferInSec");
        tsc.m_EndTimeBufferInSec    = pt.get<int>(sStratSession  + ".EndTimeBufferInSec");
        tsc.m_BreakTimeBufferInSec  = pt.get<int>(sStratSession  + ".BreakTimeBufferInSec");
        tsc.m_TreatRegularMiniAsOne = pt.get<bool>(sStratSession + ".TreatRegularMiniAsOne");

        {
          boost::optional<string> o_ParamVec = pt.get_optional<string>(sStratSession + ".ParamVector");
          string s_ParamVec;
          if (o_ParamVec)
          {
            s_ParamVec = STool::Trim(o_ParamVec.get());

            //--------------------------------------------------
            // New format
            //--------------------------------------------------
            vector<string> vs;
            boost::split(vs, s_ParamVec, boost::is_any_of(","));
            for (unsigned int i = 0; i < vs.size(); ++i)
            {
              if (vs[i] != "|")
              {
                tsc.m_ParamVector.push_back(boost::lexical_cast<double>(vs[i]));
              }
            }

          }
        }
        {
          string s_TradedSym = m_SymbolsToBeUsedInAllSections;
          if (s_TradedSym == "") s_TradedSym = STool::Trim(pt.get<string>(sStratSession + ".TradedSymbols"));
          if (s_TradedSym != "") m_Logger->Write(Logger::INFO,"SystemConfig: %s m_TradedSymbols (truncated to 1024) %s", sStratSession.c_str(), s_TradedSym.substr(0,1024).c_str());
          else m_Logger->Write(Logger::INFO,"SystemConfig: m_TradedSymbols %s Nil", sStratSession.c_str());

          vector<string> vs;
          boost::split(vs, s_TradedSym, boost::is_any_of(","));
          FForEach (vs,[&](const string & s) { tsc.m_TradedSymbols.push_back(s); });
        }
        {
          string s_SpecialTime = STool::Trim(pt.get<string>(sStratSession + ".SpecialTime"));
          vector<string> vs;
          boost::split(vs, s_SpecialTime, boost::is_any_of(","));
          FForEach (vs,[&](const string & s) { if (s != "") tsc.m_SpecialTime.insert(HHMMSS(boost::lexical_cast<int>(s))); });
        }

        {
          string s_SyncSym = STool::Trim(pt.get<string>(sStratSession + ".SynchronizedSymbols"));

          if (STool::Trim(s_SyncSym) != "")
          {
            vector<string> vsGrp;
            boost::split(vsGrp, s_SyncSym, boost::is_any_of(";"));

            FForEach(vsGrp,[&](const string sGrp){
              vector<string> vGrp;
              boost::split(vGrp, sGrp, boost::is_any_of(","));

              set<string> setGrp;
              FForEach (vGrp,[&](const string sSym) { setGrp.insert(sSym); });
              FForEach (vGrp,[&](const string sSym) { tsc.m_SynchronizedSymbols[sSym] = setGrp; });

            });
          }
          tsc.m_SynchronizedSymbolsSec = pt.get<int>(sStratSession + ".SynchronizedSymbolsSec");

        }
        {
          boost::optional<int> o_MonitoringIntvl = pt.get_optional<int>(sStratSession + ".MonitoringIntervalInSec");
          if (o_MonitoringIntvl)
          {
            tsc.m_MonitoringIntervalInSec = o_MonitoringIntvl.get();
          }
        }
        tsc.m_WhetherToRetrain = pt.get<bool>(sStratSession + ".WhetherToRetrain");
        string sTrainFreq = STool::Trim(pt.get<string>(sStratSession + ".TrainingFreq"));
        if      (sTrainFreq == "Daily")            tsc.m_TrainingFreq = TradingStrategyConfig::Daily;
        else if (sTrainFreq == "Weekly")           tsc.m_TrainingFreq = TradingStrategyConfig::Weekly;
        else if (sTrainFreq == "Monthly")          tsc.m_TrainingFreq = TradingStrategyConfig::Monthly;
        else if (sTrainFreq == "Yearly")           tsc.m_TrainingFreq = TradingStrategyConfig::Yearly;
        else if (sTrainFreq == "Auto")             tsc.m_TrainingFreq = TradingStrategyConfig::Auto;
        else if (sTrainFreq == "OnceAtInitWarmup") tsc.m_TrainingFreq = TradingStrategyConfig::OnceAtInitWarmup;

        tsc.m_CPnLHistPersistenceFile        = STool::Trim(pt.get<string>(sStratSession + ".CPnLHistPersistenceFile"));
        tsc.m_CPnLHistPersistenceIsEnabled   = pt.get<bool>   (sStratSession + ".EnableCPnLHistPersistence");


        if (sty == STY_S11A)
        {
          deque<double> dqParamVect;

          //--------------------------------------------------
          for (unsigned int i = 0; i < tsc.m_ParamVector.size(); ++i)
          {
            dqParamVect.push_back(tsc.m_ParamVector[i]);
          }
          //--------------------------------------------------

          m_S11A_NotionalValuePerLeg = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_MonitoringIntervalInSec = dqParamVect[0];
          dqParamVect.pop_front();


          if      (dqParamVect[0] == 1) m_On_S11A_T1GBM = true;
          else if (dqParamVect[0] == 0) m_On_S11A_T1GBM = false;
          dqParamVect.pop_front();


          m_S11A_T1GBM_TriggerThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GBM_TakeProfitThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GBM_StopLossThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          int iT1DeltaTNum = dqParamVect[0];
          dqParamVect.pop_front();

          for (unsigned int i = 0; i < iT1DeltaTNum; ++i)
          {
            m_S11A_T1GBM_DeltaTPeriods.push_back(dqParamVect[0]);
            dqParamVect.pop_front();
          }






          if      (dqParamVect[0] == 1) m_On_S11A_T1GUMBELCAUCHY = true;
          else if (dqParamVect[0] == 0) m_On_S11A_T1GUMBELCAUCHY = false;
          dqParamVect.pop_front();

          m_S11A_T1GumbelCauchyPeriod = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelCauchy_TriggerThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelCauchy_TakeProfitThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelCauchy_StopLossThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelCauchy_EOLPeriod = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelCauchyThetaMean = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelCauchyThetaMeanReversion = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelCauchyMarginalSDScaling = dqParamVect[0];
          dqParamVect.pop_front();




          if      (dqParamVect[0] == 1) m_On_S11A_T1GUMBELGAUSS = true;
          else if (dqParamVect[0] == 0) m_On_S11A_T1GUMBELGAUSS = false;
          dqParamVect.pop_front();

          m_S11A_T1GumbelGaussPeriod = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelGauss_TriggerThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelGauss_TakeProfitThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelGauss_StopLossThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelGauss_EOLPeriod = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelGaussThetaMean = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelGaussThetaMeanReversion = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelGaussMarginalSDScaling = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T1GumbelGaussSigalLimit = dqParamVect[0];
          dqParamVect.pop_front();





          if      (dqParamVect[0] == 1) m_On_S11A_T2GBM = true;
          else if (dqParamVect[0] == 0) m_On_S11A_T2GBM = false;
          dqParamVect.pop_front();


          m_S11A_T2GBM_TriggerThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T2GBM_TakeProfitThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T2GBM_StopLossThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          int iT2MANum = dqParamVect[0];
          dqParamVect.pop_front();

          for (unsigned int i = 0; i < iT2MANum; ++i)
          {
            m_S11A_T2_MAPeriods.push_back(dqParamVect[0]);
            dqParamVect.pop_front();
          }

          int iT2DeltaTNum = dqParamVect[0];
          dqParamVect.pop_front();

          for (unsigned int i = 0; i < iT2DeltaTNum; ++i)
          {
            m_S11A_T2GBM_DeltaTPeriods.push_back(dqParamVect[0]);
            dqParamVect.pop_front();
          }



          if      (dqParamVect[0] == 1) m_On_S11A_T2PCTL = true;
          else if (dqParamVect[0] == 0) m_On_S11A_T2PCTL = false;
          dqParamVect.pop_front();


          m_S11A_T2PctlPeriod = dqParamVect[0];
          dqParamVect.pop_front();



          if      (dqParamVect[0] == 1) m_On_S11A_T2COPU = true;
          else if (dqParamVect[0] == 0) m_On_S11A_T2COPU = false;
          dqParamVect.pop_front();

          m_S11A_T2CopuPeriod = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T2Copu_TriggerThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T2Copu_TakeProfitThreshold = dqParamVect[0];
          dqParamVect.pop_front();

          m_S11A_T2Copu_StopLossThreshold = dqParamVect[0];
          dqParamVect.pop_front();

        }

      }
    }
  }

  {
    m_NumOfMDI = 0;
    m_NumOfOTI = 0;

    for (int om = 0; om < MAX_OTIMDI; om++)
    {
      int otimdi = om;

      boost::optional<string> oMDI_TCP  = pt.get_optional<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".ServerIP");
      boost::optional<string> oMDI_File = pt.get_optional<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".DataFile");

      if (oMDI_TCP && !oMDI_File)
      {
        //--------------------------------------------------
        // MDI TCP server
        //--------------------------------------------------
        m_NumOfMDI++;

        map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(otimdi);
        if (it == m_map_OTIMDIConfig.end())
        {
          m_map_OTIMDIConfig[otimdi] = OTIMDIConfig();
          it = m_map_OTIMDIConfig.find(otimdi);
        }
        OTIMDIConfig & omc = it->second;

        omc.m_MDI_File = "";
        omc.m_MDI_IP   = STool::Trim(oMDI_TCP.get());
        omc.m_MDI_Port = STool::Trim(pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".ServerPort"));

        string s_MDI_SubscriptionSymbols = m_SymbolsToBeUsedInAllSections;
        if (s_MDI_SubscriptionSymbols == "") s_MDI_SubscriptionSymbols = STool::Trim(pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionSymbols"));
        if (s_MDI_SubscriptionSymbols != "") m_Logger->Write(Logger::INFO,"SystemConfig: otimid %d s_MDI_SubscriptionSymbols (truncated to 1024) %s", otimdi, s_MDI_SubscriptionSymbols.substr(0,1024).c_str());
        else m_Logger->Write(Logger::INFO,"SystemConfig: s_MDI_SubscriptionSymbols %d Nil.", otimdi);

        vector<string> vsub;
        boost::split(vsub, s_MDI_SubscriptionSymbols, boost::is_any_of(","));
        omc.m_MDISubscriptionSymbols = vsub;

        omc.m_MDISubscriptionStartDate   = STool::Trim(pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionStartDate"));
        omc.m_MDISubscriptionEndDate     = STool::Trim(pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionEndDate"));
        omc.m_MDISubscriptionReplaySpeed = STool::Trim(pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionReplaySpeed"));

        m_Logger->Write(Logger::INFO,"SystemConfig: Read MDI Config: %d %s", otimdi, omc.m_MDI_IP.c_str());
      }
      else if (!oMDI_TCP && oMDI_File)
      {
        //--------------------------------------------------
        // MDI Data file
        //--------------------------------------------------
        m_NumOfMDI++;

        map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(otimdi);
        if (it == m_map_OTIMDIConfig.end())
        {
          m_map_OTIMDIConfig[otimdi] = OTIMDIConfig();
          it = m_map_OTIMDIConfig.find(otimdi);
        }
        OTIMDIConfig & omc = it->second;

        omc.m_MDI_File = oMDI_File.get();
        omc.m_MDI_IP   = "";
        omc.m_MDI_Port = "";

        string s_MDI_SubscriptionSymbols = m_SymbolsToBeUsedInAllSections;
        if (s_MDI_SubscriptionSymbols == "") s_MDI_SubscriptionSymbols = STool::Trim(pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionSymbols"));
        if (s_MDI_SubscriptionSymbols != "") m_Logger->Write(Logger::INFO,"SystemConfig: s_MDI_SubscriptionSymbols %d (truncated to 1024) %s", otimdi, s_MDI_SubscriptionSymbols.substr(0,1024).c_str());
        else m_Logger->Write(Logger::INFO,"SystemConfig: s_MDI_SubscriptionSymbols %d Nil", otimdi);

        vector<string> vsub;
        boost::split(vsub, s_MDI_SubscriptionSymbols, boost::is_any_of(","));
        omc.m_MDISubscriptionSymbols = vsub;

        omc.m_MDISubscriptionStartDate   = "";
        omc.m_MDISubscriptionEndDate     = "";
        omc.m_MDISubscriptionReplaySpeed = "";

        m_Logger->Write(Logger::INFO,"SystemConfig: Read MDI Config: %d %s", otimdi, omc.m_MDI_IP.c_str());
      }


      boost::optional<string> oOTI = pt.get_optional<string>("OTI_" + boost::lexical_cast<string>(otimdi) + ".ServerIP");
      if (oOTI)
      {
        m_NumOfOTI++;

        map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(otimdi);
        if (it == m_map_OTIMDIConfig.end())
        {
          m_map_OTIMDIConfig[otimdi] = OTIMDIConfig();
          it = m_map_OTIMDIConfig.find(otimdi);
        }
        OTIMDIConfig & omc = it->second;

        omc.m_OTI_IP   = STool::Trim(pt.get<string>("OTI_" + boost::lexical_cast<string>(otimdi) + ".ServerIP"));
        omc.m_OTI_Port = STool::Trim(pt.get<string>("OTI_" + boost::lexical_cast<string>(otimdi) + ".ServerPort"));

        m_Logger->Write(Logger::INFO,"SystemConfig: Read OTI Config: %d %s", otimdi, omc.m_OTI_IP.c_str());
      }
    }
  }

  {
    m_HaltTradingPath = STool::Trim(pt.get<string>("SystemSettings.HaltTradingPath"));
    deque<string> dqFile;
    STool::ReadFile(m_HaltTradingPath,dqFile);
    for (deque<string>::iterator it = dqFile.begin(); it != dqFile.end(); ++it)
    {
      vector<string> vs;
      boost::split(vs, *it, boost::is_any_of(","));

      const string & sym = vs[0];
      const YYYYMMDD & ymd = vs[1];
      map<string,set<YYYYMMDD> >::iterator it2 = m_HaltTradingSymDate.find(sym);
      if (it2 == m_HaltTradingSymDate.end())
      {
        m_HaltTradingSymDate[sym] = set<YYYYMMDD>();
        it2 = m_HaltTradingSymDate.find(sym);
      }
      it2->second.insert(ymd);
    }

    //--------------------------------------------------
    // print log
    //--------------------------------------------------
    for (map<string,set<YYYYMMDD> >::iterator it = m_HaltTradingSymDate.begin(); it != m_HaltTradingSymDate.end(); ++it)
    {
      const string & symbol = it->first;
      set<YYYYMMDD> & setYMD = it->second;
      for (set<YYYYMMDD>::iterator it2 = setYMD.begin(); it2 != setYMD.end(); ++it2)
      {
        m_Logger->Write(Logger::INFO,"SystemConfig: HaltTrading: %s %s", symbol.c_str(), (*it2).ToStr_().c_str());
      }
    }
  }
  {
    m_HalfDayTradingPath = STool::Trim(pt.get<string>("SystemSettings.HalfDayTradingPath"));
    deque<string> dqFile;
    STool::ReadFile(m_HalfDayTradingPath,dqFile);
    for (deque<string>::iterator it = dqFile.begin(); it != dqFile.end(); ++it)
    {
      vector<string> vs;
      boost::split(vs, *it, boost::is_any_of(","));

      const string & sym = vs[0];
      const YYYYMMDD & ymd = vs[1];
      map<string,set<YYYYMMDD> >::iterator it2 = m_HalfDayTradingSymDate.find(sym);
      if (it2 == m_HalfDayTradingSymDate.end())
      {
        m_HalfDayTradingSymDate[sym] = set<YYYYMMDD>();
        it2 = m_HalfDayTradingSymDate.find(sym);
      }
      it2->second.insert(ymd);
    }

    //--------------------------------------------------
    // print log
    //--------------------------------------------------
    for (map<string,set<YYYYMMDD> >::iterator it = m_HalfDayTradingSymDate.begin(); it != m_HalfDayTradingSymDate.end(); ++it)
    {
      const string & symbol = it->first;
      set<YYYYMMDD> & setYMD = it->second;
      for (set<YYYYMMDD>::iterator it2 = setYMD.begin(); it2 != setYMD.end(); ++it2)
      {
        m_Logger->Write(Logger::INFO,"SystemConfig: HalfDayTrading: %s %s", symbol.c_str(), (*it2).ToStr_().c_str());
      }
    }
  }
  //--------------------------------------------------
  {
    m_CorpActionAdjPath = STool::Trim(pt.get<string>("SystemSettings.CorpActionAdjPath"));
    deque<string> dqFile;
    STool::ReadFile(m_CorpActionAdjPath,dqFile);
    for (deque<string>::iterator it = dqFile.begin(); it != dqFile.end(); ++it)
    {
      vector<string> vs;
      boost::split(vs, *it, boost::is_any_of(","));

      const YYYYMMDD & ymd     = vs[0];
      const string & sym       = vs[1];
      const double price_delta = boost::lexical_cast<double>(vs[2]);
      const double price_ratio = boost::lexical_cast<double>(vs[3]);
      map<string,map<YYYYMMDD,tuple_corp_action_adj> >::iterator it2 = m_CorpActionAdjDetails.find(sym);
      if (it2 == m_CorpActionAdjDetails.end())
      {
        m_CorpActionAdjDetails[sym] = map<YYYYMMDD,tuple_corp_action_adj>();
        it2 = m_CorpActionAdjDetails.find(sym);
      }
      map<YYYYMMDD,tuple_corp_action_adj>::iterator it3 = it2->second.find(ymd);
      if (it3 == it2->second.end())
      {
        it2->second[ymd] = tuple_corp_action_adj();
        it3 = it2->second.find(ymd);
      }
      tuple_corp_action_adj tcaa;
      tcaa.m_price_delta = price_delta;
      tcaa.m_price_ratio = price_ratio;
      it3->second = tcaa;
    }

    //--------------------------------------------------
    // print log
    //--------------------------------------------------
    for (map<string,map<YYYYMMDD,tuple_corp_action_adj> >::iterator it = m_CorpActionAdjDetails.begin(); it != m_CorpActionAdjDetails.end(); ++it)
    {
      const string & symbol = it->first;
      map<YYYYMMDD,tuple_corp_action_adj> & mapTmp = it->second;
      for (map<YYYYMMDD,tuple_corp_action_adj>::iterator it2 = mapTmp.begin(); it2 != mapTmp.end(); ++it2)
      {
        m_Logger->Write(Logger::INFO,"SystemConfig: CorpActionAdj: %s %s price delta = %f price ratio = %f",
                        symbol.c_str(),
                        it2->first.ToStr_().c_str(),
                        it2->second.m_price_delta,
                        it2->second.m_price_ratio
                       );
      }
    }
    //--------------------------------------------------
  }

  //--------------------------------------------------
  {
    m_LotSizePath = STool::Trim(pt.get<string>("SystemSettings.LotSizePath"));
    deque<string> dqFile;
    STool::ReadFile(m_LotSizePath,dqFile);
    for (deque<string>::iterator it = dqFile.begin(); it != dqFile.end(); ++it)
    {
      vector<string> vs;
      boost::split(vs, *it, boost::is_any_of(","));

      const string & sym      = vs[0];
      const int      lot_size = boost::lexical_cast<int>(vs[1]);
      m_map_LotSize[sym] = lot_size;
    }

    //--------------------------------------------------
    // print log
    //--------------------------------------------------
    FForEach (m_map_LotSize,[&](const std::pair<string,int> & ls) {
      const string & symbol   = ls.first;
      const int      lot_size = ls.second;
      m_Logger->Write(Logger::INFO,"SystemConfig: Read config file: %s Lot size: %d", symbol.c_str(), lot_size);
    });
    //--------------------------------------------------
  }

  m_PriceFwdrIsOn = pt.get<bool>("SystemSettings.PriceForwarderToNextTierOnOff");
  m_PriceFwdrIntervalInSec = pt.get<int>("SystemSettings.PriceForwarderToNextTierIntervalInSec");


  string sMDIMode = STool::Trim(pt.get<string>("SystemSettings.MDIMode"));
  if      (sMDIMode == "readfile")        m_MDIMode = MDI_READFILE;
  else if (sMDIMode == "tcpwithack")      m_MDIMode = MDI_TCPWITHACK;
  else if (sMDIMode == "tcpwithoutack")   m_MDIMode = MDI_TCPWITHOUTACK;

  string sOTIMode = STool::Trim(pt.get<string>("SystemSettings.OTIMode"));
  if      (sOTIMode == "record")          m_OTIMode = OTI_RECORD;
  else if (sOTIMode == "tcp")             m_OTIMode = OTI_TCP;

  m_NextTierZMQIsOn = pt.get<bool>("SystemSettings.NextTierZMQIsOn");

  boost::optional<string> o_NextTier_ZMQ_MD_IP_Port = pt.get_optional<string>("SystemSettings.ZMQMDIPPort");
  boost::optional<string> o_NextTier_ZMQ_TF_IP_Port = pt.get_optional<string>("SystemSettings.ZMQTFIPPort");
  if (o_NextTier_ZMQ_MD_IP_Port) m_NextTier_ZMQ_MD_IP_Port = STool::Trim(o_NextTier_ZMQ_MD_IP_Port.get());
  if (o_NextTier_ZMQ_TF_IP_Port) m_NextTier_ZMQ_TF_IP_Port = STool::Trim(o_NextTier_ZMQ_TF_IP_Port.get());

  switch(pt.get<int> ("MarketData.TradeVolumeMode"))
  {
    case 0:  { m_MktData_TradeVolumeMode = PER_TRADE_VOLUME_MODE;   break;   }
    case 1:  { m_MktData_TradeVolumeMode = ACCUMULATED_VOLUME_MODE; break;   }
    default: { m_MktData_TradeVolumeMode = ACCUMULATED_VOLUME_MODE; break;   }
  }

  {
    string sBarAggregationM1Symbols = m_SymbolsToBeUsedInAllSections;
    if (sBarAggregationM1Symbols == "") sBarAggregationM1Symbols = STool::Trim(pt.get<string>("MarketData.BarAggregationM1Symbols"));
    if (sBarAggregationM1Symbols != "") m_Logger->Write(Logger::INFO,"SystemConfig: sBarAggregationM1Symbols (truncated to 1024) %s", sBarAggregationM1Symbols.substr(0,1024).c_str());
    else m_Logger->Write(Logger::INFO,"SystemConfig: sBarAggregationM1Symbols Nil");

    vector<string> vs;
    boost::split(vs, sBarAggregationM1Symbols, boost::is_any_of(","));

    vector<string>::iterator it;
    for (it = vs.begin(); it !=  vs.end(); ++it)
    {
      m_BarAggregationM1Symbols.insert(*it);
    }
  }

  {
    string sBarAggregationD1Symbols = m_SymbolsToBeUsedInAllSections;
    if (sBarAggregationD1Symbols == "") sBarAggregationD1Symbols = STool::Trim(pt.get<string>("MarketData.BarAggregationD1Symbols"));
    if (sBarAggregationD1Symbols != "") m_Logger->Write(Logger::INFO,"SystemConfig: sBarAggregationD1Symbols (truncated to 1024) %s", sBarAggregationD1Symbols.substr(0,1024).c_str());
    else m_Logger->Write(Logger::INFO,"SystemConfig: sBarAggregationD1Symbols Nil");

    vector<string> vs;
    boost::split(vs, sBarAggregationD1Symbols, boost::is_any_of(","));

    vector<string>::iterator it;
    for (it = vs.begin(); it !=  vs.end(); ++it)
    {
      m_BarAggregationD1Symbols.insert(*it);
    }
  }

  m_ErroneousTickPxChg = pt.get<double>("MarketData.ErroneousTickPxChg");

  {
    string sSymToBeWarmedUpWithDayBars = m_SymbolsToBeUsedInAllSections;
    if (sSymToBeWarmedUpWithDayBars == "") sSymToBeWarmedUpWithDayBars = STool::Trim(pt.get<string>("TechIndicators.SymToBeWarmedUpWithDayBars"));
    if (sSymToBeWarmedUpWithDayBars != "") m_Logger->Write(Logger::INFO,"SystemConfig: sSymToBeWarmedUpWithDayBars (truncated to 1024) %s", sSymToBeWarmedUpWithDayBars.substr(0,1024).c_str());
    else m_Logger->Write(Logger::INFO,"SystemConfig: sSymToBeWarmedUpWithDayBars Nil");

    vector<string> vs;
    boost::split(vs, sSymToBeWarmedUpWithDayBars, boost::is_any_of(","));

    vector<string>::iterator it;
    for (it = vs.begin(); it !=  vs.end(); ++it)
    {
      m_SymToBeWarmedUpWithDayBars.insert(*it);
    }
  }

  m_Logger->Write(Logger::INFO,"SystemConfig: Finished Reading Config file: %s", sConfigPath.c_str());

  return;
}

string SystemConfig::GetSignalLogPath(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_SignalLogPath;
  }
  else return "";
}
HHMMSS SystemConfig::GetStrategyMTMTime(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_MTMTime;
  }
  else return HHMMSS();
}
vector<double> SystemConfig::GetParamVector(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_ParamVector;
  }
  else return vector<double>();
}
vector<string> SystemConfig::GetTradedSymbols(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_TradedSymbols;
  }
  else return vector<string>();
}
set<string> SystemConfig::GetSynchronizedSymbols(const StrategyID id, const string & symbol)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    map<string,set<string> >::iterator it2 = it->second.m_SynchronizedSymbols.find(symbol);
    if (it2 == it->second.m_SynchronizedSymbols.end()) return set<string>();
    else return it2->second;
  }
  else return set<string>();
}
int SystemConfig::GetSynchronizedSymbolsSec(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_SynchronizedSymbolsSec;
  }
  else return -1;
}
int SystemConfig::GetMonitoringIntervalInSec(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_MonitoringIntervalInSec;
  }
  else return -1;
}
bool SystemConfig::GetWhetherToRetrain(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_WhetherToRetrain;
  }
  else return false;
}
bool SystemConfig::ChkIfTreatRegularMiniAsOne(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_TreatRegularMiniAsOne;
  }
  else return false;
}
TradingStrategyConfig::TrainingFreq SystemConfig::GetTrainingFreq(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_TrainingFreq;
  }
  else return TradingStrategyConfig::Auto;
}
int SystemConfig::GetStrategyStartTimeBufferInSec(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_StartTimeBufferInSec;
  }
  else return -1;
}
int SystemConfig::GetStrategyEndTimeBufferInSec(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_EndTimeBufferInSec;
  }
  else return -1;
}
int SystemConfig::GetStrategyBreakTimeBufferInSec(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_BreakTimeBufferInSec;
  }
  else return -1;
}
bool SystemConfig::IsStrategyOn(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_OnOff;
  }
  else return false;
}
string SystemConfig::Get_CPnLHistPersistenceFile(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_CPnLHistPersistenceFile;
  }
  else return "";
}

bool SystemConfig::IsCPnLHistPersistenceEnabled(const StrategyID id)
{
  map<StrategyID,TradingStrategyConfig>::iterator it = m_map_TradingStrategyConfig.find(id);
  if (it != m_map_TradingStrategyConfig.end())
  {
    return it->second.m_CPnLHistPersistenceIsEnabled;
  }
  else return false;
}

string SystemConfig::Get_MDI_File(const int s)
{
  map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(s);
  if (it != m_map_OTIMDIConfig.end())
  {
    return it->second.m_MDI_File;
  }
  else return "";
}
string SystemConfig::Get_MDI_IP(const int s)
{
  map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(s);
  if (it != m_map_OTIMDIConfig.end())
  {
    return it->second.m_MDI_IP;
  }
  else return "";
}
string SystemConfig::Get_OTI_IP(const int s)
{
  map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(s);
  if (it != m_map_OTIMDIConfig.end())
  {
    return it->second.m_OTI_IP;
  }
  else return "";
}
string SystemConfig::Get_MDI_Port(const int s)
{
  map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(s);
  if (it != m_map_OTIMDIConfig.end())
  {
    return it->second.m_MDI_Port;
  }
  else return "";
}
string SystemConfig::Get_OTI_Port(const int s)
{
  map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(s);
  if (it != m_map_OTIMDIConfig.end())
  {
    return it->second.m_OTI_Port;
  }
  else return "";
}
const vector<string> SystemConfig::GetMDISubscriptionSymbols(const int s)
{
  map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(s);
  if (it != m_map_OTIMDIConfig.end())
  {
    return it->second.m_MDISubscriptionSymbols;
  }
  else return vector<string>();
}
const string SystemConfig::GetMDISubscriptionStartDate(const int s)
{
  map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(s);
  if (it != m_map_OTIMDIConfig.end())
  {
    return it->second.m_MDISubscriptionStartDate;
  }
  else return "";
}
const string SystemConfig::GetMDISubscriptionEndDate(const int s)
{
  map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(s);
  if (it != m_map_OTIMDIConfig.end())
  {
    return it->second.m_MDISubscriptionEndDate;
  }
  else return "";
}
const string SystemConfig::GetMDISubscriptionReplaySpeed(const int s)
{
  map<int,OTIMDIConfig>::iterator it = m_map_OTIMDIConfig.find(s);
  if (it != m_map_OTIMDIConfig.end())
  {
    return it->second.m_MDISubscriptionReplaySpeed;
  }
  else return "";
}
