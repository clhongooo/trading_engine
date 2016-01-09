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
  m_ExecLogPath("nirvana_exec.log"),
  m_MTMHoldingsLogPath("nirvana_mtm_holdings.log"),
  m_SupplementaryBarD1Path(""),
  m_SupplementaryBarM1Path(""),
  m_HKIntradaySeasonalityOnOff(false),
  m_HKIntradaySeasonalityPath(""),
  m_HKIntradaySeasonalityWindow(44),
  m_PositionPersistenceFile(""),
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
  m_ErroneousTickPxChg(NAN),
  m_GKYZ_Window_Size(6000),
  m_MktData_TradeVolumeMode(SystemConfig::ACCUMULATED_VOLUME_MODE),
  m_VolSurfParamFile1FM(""),
  m_VolSurfParamFile2FM(""),
  m_ProbDistrFileFSMC1D(""),
  m_OrderRoutingMode(ORDER_ROUTE_OTI),
  m_TCPOrEmbeddedMode(TCPWITHOUTACK),
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
  m_B2_US_HasEnabledMinCommissionCheck(true),
  m_B2_US_HasEnabledRotationMode(false),
  m_B2_US_TheoCPnLHistFolder(""),
  m_B2_US_TheoPosFolder(""),
  m_B2_US_PersistTheoPosCPnL(false),
  m_B2_HK_HasEnabledMinCommissionCheck(true),
  m_B2_HK_HasEnabledRotationMode(false),
  m_B2_HK_TheoCPnLHistFolder(""),
  m_B2_HK_TheoPosFolder(""),
  m_B2_HK_PersistTheoPosCPnL(false)
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
SystemConfig::OrderRoutingMode         SystemConfig::Get_OrderRoutingMode()                            const {  return m_OrderRoutingMode;                            }
SystemConfig::TCPOrEmbeddedMode        SystemConfig::Get_TCPOrEmbeddedMode()                           const {  return m_TCPOrEmbeddedMode;                           }
string                                 SystemConfig::Get_Main_Log_Path()                               const {  return m_MainLogPath;                                 }
string                                 SystemConfig::Get_MTM_Log_Path()                                const {  return m_MTMLogPath;                                  }
string                                 SystemConfig::Get_Exec_Log_Path()                               const {  return m_ExecLogPath;                                 }
string                                 SystemConfig::Get_Holdings_Log_Path()                           const {  return m_MTMHoldingsLogPath;                          }
string                                 SystemConfig::Get_SupplementaryBarD1Path()                      const {  return m_SupplementaryBarD1Path;                      }
string                                 SystemConfig::Get_SupplementaryBarM1Path()                      const {  return m_SupplementaryBarM1Path;                      }
bool                                   SystemConfig::Get_HKIntradaySeasonalityOnOff()                  const {  return m_HKIntradaySeasonalityOnOff;                  }
string                                 SystemConfig::Get_HKIntradaySeasonalityPath()                   const {  return m_HKIntradaySeasonalityPath;                   }
int                                    SystemConfig::Get_HKIntradaySeasonalityWindow()                 const {  return m_HKIntradaySeasonalityWindow;                 }
string                                 SystemConfig::Get_PositionPersistenceFile()                     const {  return m_PositionPersistenceFile;                     }
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

bool         SystemConfig::B2_HasEnabledMinCommissionCheck(const StrategyID id)  const {  if (id == STY_B2_USSTK) return m_B2_US_HasEnabledMinCommissionCheck; else if (id == STY_B2_HK) return m_B2_HK_HasEnabledMinCommissionCheck; }
bool         SystemConfig::B2_HasEnabledRotationMode      (const StrategyID id)  const {  if (id == STY_B2_USSTK) return m_B2_US_HasEnabledRotationMode;       else if (id == STY_B2_HK) return m_B2_HK_HasEnabledRotationMode;       }
string       SystemConfig::B2_TheoCPnLHistFolder          (const StrategyID id)  const {  if (id == STY_B2_USSTK) return m_B2_US_TheoCPnLHistFolder;           else if (id == STY_B2_HK) return m_B2_HK_TheoCPnLHistFolder;           }
string       SystemConfig::B2_TheoPosFolder               (const StrategyID id)  const {  if (id == STY_B2_USSTK) return m_B2_US_TheoPosFolder;                else if (id == STY_B2_HK) return m_B2_HK_TheoPosFolder;                }
bool         SystemConfig::B2_PersistTheoPosCPnL          (const StrategyID id)  const {  if (id == STY_B2_USSTK) return m_B2_US_PersistTheoPosCPnL;           else if (id == STY_B2_HK) return m_B2_HK_PersistTheoPosCPnL;           }
vector<int>  SystemConfig::Get_B2_RotationGroup           (const StrategyID id)  const {  if (id == STY_B2_USSTK) return m_B2_US_RotationGroup;                else if (id == STY_B2_HK) return m_B2_HK_RotationGroup;                }


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

  string sLogPath = pt.get<std::string>("SystemSettings.LogPath");
  m_Logger->SetPath(sLogPath.c_str());

  string sLogLevel = pt.get<std::string>("SystemSettings.LogLevel");
  if      (sLogLevel == "EMERGENCY") { m_Logger->SetLogLevel(Logger::EMERGENCY); }
  else if (sLogLevel == "ALERT")     { m_Logger->SetLogLevel(Logger::ALERT);     }
  else if (sLogLevel == "CRITICAL")  { m_Logger->SetLogLevel(Logger::CRITICAL);  }
  else if (sLogLevel == "ERROR")     { m_Logger->SetLogLevel(Logger::ERROR);     }
  else if (sLogLevel == "WARNING")   { m_Logger->SetLogLevel(Logger::WARNING);   }
  else if (sLogLevel == "NOTICE")    { m_Logger->SetLogLevel(Logger::NOTICE);    }
  else if (sLogLevel == "INFO")      { m_Logger->SetLogLevel(Logger::INFO);      }
  else if (sLogLevel == "DEBUG")     { m_Logger->SetLogLevel(Logger::DEBUG);     }

  m_MainLogPath                        = pt.get<string>         ("SystemSettings.LogPath");
  m_MTMLogPath                         = pt.get<string>         ("SystemSettings.MTMLogPath");
  m_ExecLogPath                        = pt.get<string>         ("SystemSettings.ExecLogPath");
  m_MTMHoldingsLogPath                 = pt.get<string>         ("SystemSettings.HoldingsLogPath");
  m_SupplementaryBarD1Path             = pt.get<string>         ("SystemSettings.SupplementaryBarD1Path");
  m_SupplementaryBarM1Path             = pt.get<string>         ("SystemSettings.SupplementaryBarM1Path");
  m_PositionPersistenceFile            = pt.get<string>         ("SystemSettings.PositionPersistenceFile");
  m_PositionPersistenceIsEnabled       = pt.get<bool>           ("SystemSettings.EnablePositionPersistence");
  m_SendResetOnConnectionToCAPI        = pt.get<bool>           ("SystemSettings.SendResetOnConnectionToCAPI");

  m_SystemServicePort                  = pt.get<unsigned short> ("SystemSettings.SystemServicePort");

  string sDefaultTimeZone;
  boost::optional<string> o_DefaultTimeZone = pt.get_optional<string>("SystemSettings.DefaultTimeZone");
  if (o_DefaultTimeZone) sDefaultTimeZone = o_DefaultTimeZone.get();
  if (sDefaultTimeZone == "HKT") m_DefaultTimeZone = SDateTime::HKT;
  if (sDefaultTimeZone == "EST") m_DefaultTimeZone = SDateTime::EST;
  if (sDefaultTimeZone == "CST") m_DefaultTimeZone = SDateTime::CST;
  if (sDefaultTimeZone == "GMT") m_DefaultTimeZone = SDateTime::GMT;

  boost::optional<bool>    o_HKIntradaySeasonalityOnOff    = pt.get_optional<bool>    ("HKIntradaySeasonality.OnOff");
  boost::optional<string>  o_HKIntradaySeasonalityPath     = pt.get_optional<string>  ("HKIntradaySeasonality.TrainingResultPath");
  boost::optional<int>     o_HKIntradaySeasonalityWindow   = pt.get_optional<int>     ("HKIntradaySeasonality.WindowLength");

  if(o_HKIntradaySeasonalityOnOff )  m_HKIntradaySeasonalityOnOff   = o_HKIntradaySeasonalityOnOff  .get();
  if(o_HKIntradaySeasonalityPath  )  m_HKIntradaySeasonalityPath    = o_HKIntradaySeasonalityPath   .get();
  if(o_HKIntradaySeasonalityWindow)  m_HKIntradaySeasonalityWindow  = o_HKIntradaySeasonalityWindow .get();

  m_VolSurfCalcIntervalInSec           = pt.get<long>           ("VolatilitySurface.CalcIntervalInSeconds");
  m_ProbDistrnCalcIntervalInSec        = pt.get<long>           ("ProbDistribution.CalcIntervalInSeconds");
  m_TechIndUpdateIntervalInSec         = pt.get<long>           ("TechIndicators.CalcIntervalInSeconds");
  m_TradingHoursPath                   = pt.get<string>         ("StaticData.TradingHoursPath");
  m_HKFE_CalendarPath                  = pt.get<string>         ("StaticData.HKFE_CalendarPath");
  m_HKSE_HSIConstituentsPath           = pt.get<string>         ("StaticData.HKSE_HSIConstituentsPath");
  m_HKMA_ExchgFundBillPath             = pt.get<string>         ("StaticData.HKMA_ExchgFundBillPath");
  m_CorrelMatricesPath                 = pt.get<string>         ("StaticData.CorrelMatricesPath");
  m_GKYZ_Window_Size                   = pt.get<long>           ("TechIndicators.GKYZ_WindowSize");

  m_VolSurfParamFile1FM                = pt.get<string>         ("VolatilitySurface.ParamFile1FM");
  m_VolSurfParamFile2FM                = pt.get<string>         ("VolatilitySurface.ParamFile2FM");
  m_ProbDistrFileFSMC1D                = pt.get<string>         ("ProbDistribution.ProbDistrFileFSMC1D");

  boost::optional<int>    o_B1_HKF_SamplingIntervalInSec       = pt.get_optional<int>   ("Strategy_B1_HKF.SamplingIntervalInSec");
  boost::optional<string> o_S11A_ParamFileHistSDCorrel         = pt.get_optional<string>("Strategy_S11A.ParamFileHistSDCorrel");
  boost::optional<string> o_S11A_ParamFileHSIHHIIV             = pt.get_optional<string>("Strategy_S11A.ParamFileHSIHHIIV");
  boost::optional<string> o_S11A_ParamFileT1GumbelCauchy       = pt.get_optional<string>("Strategy_S11A.ParamFileT1GumbelCauchy");
  boost::optional<string> o_S11A_ParamFileT1GumbelGauss        = pt.get_optional<string>("Strategy_S11A.ParamFileT1GumbelGauss");
  boost::optional<string> o_S11A_ParamFileT2Copula             = pt.get_optional<string>("Strategy_S11A.ParamFileT2Copula");

  boost::optional<bool>   o_B2_US_HasEnabledMinCommissionCheck = pt.get_optional<bool>  ("Strategy_B2_USSTK.EnableMinCommissionCheck");
  boost::optional<bool>   o_B2_US_HasEnabledRotationMode       = pt.get_optional<bool>  ("Strategy_B2_USSTK.EnableRotationMode");
  boost::optional<string> o_B2_US_TheoCPnLHistFolder           = pt.get_optional<string>("Strategy_B2_USSTK.TheoCPnLHistFolder");
  boost::optional<string> o_B2_US_TheoPosFolder                = pt.get_optional<string>("Strategy_B2_USSTK.TheoPosFolder");
  boost::optional<bool>   o_B2_US_PersistTheoPosCPnL           = pt.get_optional<bool>  ("Strategy_B2_USSTK.PersistTheoPosCPnL");

  boost::optional<bool>   o_B2_HK_HasEnabledMinCommissionCheck = pt.get_optional<bool>  ("Strategy_B2_HK.EnableMinCommissionCheck");
  boost::optional<bool>   o_B2_HK_HasEnabledRotationMode       = pt.get_optional<bool>  ("Strategy_B2_HK.EnableRotationMode");
  boost::optional<string> o_B2_HK_TheoCPnLHistFolder           = pt.get_optional<string>("Strategy_B2_HK.TheoCPnLHistFolder");
  boost::optional<string> o_B2_HK_TheoPosFolder                = pt.get_optional<string>("Strategy_B2_HK.TheoPosFolder");
  boost::optional<bool>   o_B2_HK_PersistTheoPosCPnL           = pt.get_optional<bool>  ("Strategy_B2_HK.PersistTheoPosCPnL");


  if(o_B1_HKF_SamplingIntervalInSec   )  m_B1_HKF_SamplingIntervalInSec     = o_B1_HKF_SamplingIntervalInSec    .get();
  if(o_S11A_ParamFileHistSDCorrel     )  m_S11A_ParamFileHistSDCorrel       = o_S11A_ParamFileHistSDCorrel      .get();
  if(o_S11A_ParamFileHSIHHIIV         )  m_S11A_ParamFileHSIHHIIV           = o_S11A_ParamFileHSIHHIIV          .get();
  if(o_S11A_ParamFileT1GumbelCauchy   )  m_S11A_ParamFileT1GumbelCauchy     = o_S11A_ParamFileT1GumbelCauchy    .get();
  if(o_S11A_ParamFileT1GumbelGauss    )  m_S11A_ParamFileT1GumbelGauss      = o_S11A_ParamFileT1GumbelGauss     .get();
  if(o_S11A_ParamFileT2Copula         )  m_S11A_ParamFileT2Copula           = o_S11A_ParamFileT2Copula          .get();

  if(o_B2_US_HasEnabledMinCommissionCheck)  m_B2_US_HasEnabledMinCommissionCheck  = o_B2_US_HasEnabledMinCommissionCheck .get();
  if(o_B2_US_HasEnabledRotationMode      )  m_B2_US_HasEnabledRotationMode        = o_B2_US_HasEnabledRotationMode       .get();
  if(o_B2_US_TheoCPnLHistFolder          )  m_B2_US_TheoCPnLHistFolder            = o_B2_US_TheoCPnLHistFolder           .get();
  if(o_B2_US_TheoPosFolder               )  m_B2_US_TheoPosFolder                 = o_B2_US_TheoPosFolder                .get();
  if(o_B2_US_PersistTheoPosCPnL          )  m_B2_US_PersistTheoPosCPnL            = o_B2_US_PersistTheoPosCPnL           .get();

  if(o_B2_HK_HasEnabledMinCommissionCheck)  m_B2_HK_HasEnabledMinCommissionCheck  = o_B2_HK_HasEnabledMinCommissionCheck .get();
  if(o_B2_HK_HasEnabledRotationMode      )  m_B2_HK_HasEnabledRotationMode        = o_B2_HK_HasEnabledRotationMode       .get();
  if(o_B2_HK_TheoCPnLHistFolder          )  m_B2_HK_TheoCPnLHistFolder            = o_B2_HK_TheoCPnLHistFolder           .get();
  if(o_B2_HK_TheoPosFolder               )  m_B2_HK_TheoPosFolder                 = o_B2_HK_TheoPosFolder                .get();
  if(o_B2_HK_PersistTheoPosCPnL          )  m_B2_HK_PersistTheoPosCPnL            = o_B2_HK_PersistTheoPosCPnL           .get();

  {
    boost::optional<string> o_B2_US_RotationGroup = pt.get_optional<string>("Strategy_B2_USSTK.RotationGroup");

    if (o_B2_US_RotationGroup)
    {
      string s_B2_US_RotationGroup = o_B2_US_RotationGroup.get();
      vector<string> vs;
      boost::split(vs, s_B2_US_RotationGroup, boost::is_any_of(","));
      for_each (vs,[&](const string & sgid) { m_B2_US_RotationGroup.push_back(boost::lexical_cast<int>(sgid)); });
    }
  }
  {
    boost::optional<string> o_B2_HK_RotationGroup = pt.get_optional<string>("Strategy_B2_HK.RotationGroup");

    if (o_B2_HK_RotationGroup)
    {
      string s_B2_HK_RotationGroup = o_B2_HK_RotationGroup.get();
      vector<string> vs;
      boost::split(vs, s_B2_HK_RotationGroup, boost::is_any_of(","));
      for_each (vs,[&](const string & sgid) { m_B2_HK_RotationGroup.push_back(boost::lexical_cast<int>(sgid)); });
    }
  }

  m_HKFEContFutRollFwdTime = HHMMSS(pt.get<int>("Strategy_Base.HKFEContFutRollFwdTime"));
  m_CMEContFutRollFwdTime  = HHMMSS(pt.get<int>("Strategy_Base.CMEContFutRollFwdTime"));

  {
    string s_ProceedStyForceExcnEvenIfNoMD = pt.get<string>("Strategy_Base.ProceedStyForceExcnEvenIfNoMD");

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
        tsc.m_SignalLogPath = pt.get<string>   (sStratSession + ".SignalLogPath");

        tsc.m_StartTimeBufferInSec  = pt.get<int>(sStratSession  + ".StartTimeBufferInSec");
        tsc.m_EndTimeBufferInSec    = pt.get<int>(sStratSession  + ".EndTimeBufferInSec");
        tsc.m_BreakTimeBufferInSec  = pt.get<int>(sStratSession  + ".BreakTimeBufferInSec");
        tsc.m_TreatRegularMiniAsOne = pt.get<bool>(sStratSession + ".TreatRegularMiniAsOne");

        {
          boost::optional<string> o_ParamVec = pt.get_optional<string>(sStratSession + ".ParamVector");
          string s_ParamVec;
          if (o_ParamVec)
          {
            s_ParamVec = o_ParamVec.get();

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
          string s_TradedSym = pt.get<string>(sStratSession + ".TradedSymbols");
          vector<string> vs;
          boost::split(vs, s_TradedSym, boost::is_any_of(","));
          for_each (vs,[&](const string & s) { tsc.m_TradedSymbols.push_back(s); });
        }
        {
          string s_SpecialTime = pt.get<string>(sStratSession + ".SpecialTime");
          vector<string> vs;
          boost::split(vs, s_SpecialTime, boost::is_any_of(","));
          for_each (vs,[&](const string & s) { if (s != "") tsc.m_SpecialTime.insert(HHMMSS(boost::lexical_cast<int>(s))); });
        }

        {
          string s_SyncSym = pt.get<string>(sStratSession + ".SynchronizedSymbols");

          if (STool::Trim(s_SyncSym) != "")
          {
            vector<string> vsGrp;
            boost::split(vsGrp, s_SyncSym, boost::is_any_of(";"));

            for_each(vsGrp,[&](const string sGrp){
              vector<string> vGrp;
              boost::split(vGrp, sGrp, boost::is_any_of(","));

              set<string> setGrp;
              for_each (vGrp,[&](const string sSym) { setGrp.insert(sSym); });
              for_each (vGrp,[&](const string sSym) { tsc.m_SynchronizedSymbols[sSym] = setGrp; });

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
        string sTrainFreq = pt.get<string>(sStratSession + ".TrainingFreq");
        if      (sTrainFreq == "Daily")   tsc.m_TrainingFreq = TradingStrategyConfig::Daily;
        else if (sTrainFreq == "Weekly")  tsc.m_TrainingFreq = TradingStrategyConfig::Weekly;
        else if (sTrainFreq == "Monthly") tsc.m_TrainingFreq = TradingStrategyConfig::Monthly;
        else if (sTrainFreq == "Yearly")  tsc.m_TrainingFreq = TradingStrategyConfig::Yearly;
        else if (sTrainFreq == "Auto")    tsc.m_TrainingFreq = TradingStrategyConfig::Auto;

        tsc.m_CPnLHistPersistenceFile        = pt.get<string> (sStratSession + ".CPnLHistPersistenceFile");
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
        omc.m_MDI_IP   = oMDI_TCP.get();
        omc.m_MDI_Port = pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".ServerPort");

        string s_MDI_SubscriptionSymbols = pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionSymbols");
        vector<string> vsub;
        boost::split(vsub, s_MDI_SubscriptionSymbols, boost::is_any_of(","));
        omc.m_MDISubscriptionSymbols = vsub;

        omc.m_MDISubscriptionStartDate   = pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionStartDate");
        omc.m_MDISubscriptionEndDate     = pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionEndDate");
        omc.m_MDISubscriptionReplaySpeed = pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionReplaySpeed");

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

        string s_MDI_SubscriptionSymbols = pt.get<string>("MDI_" + boost::lexical_cast<string>(otimdi) + ".SubscriptionSymbols");
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

        omc.m_OTI_IP   = pt.get<string>("OTI_" + boost::lexical_cast<string>(otimdi) + ".ServerIP");
        omc.m_OTI_Port = pt.get<string>("OTI_" + boost::lexical_cast<string>(otimdi) + ".ServerPort");

        m_Logger->Write(Logger::INFO,"SystemConfig: Read OTI Config: %d %s", otimdi, omc.m_OTI_IP.c_str());
      }
    }
  }

  {
    m_HaltTradingPath = pt.get<string>("SystemSettings.HaltTradingPath");
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
    m_HalfDayTradingPath = pt.get<string>("SystemSettings.HalfDayTradingPath");
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

  {
    m_CorpActionAdjPath = pt.get<string>("SystemSettings.CorpActionAdjPath");
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

  string sOrderRoutingMode = pt.get<string>("SystemSettings.OrderRoutingMode");
  if      (sOrderRoutingMode == "record") m_OrderRoutingMode = ORDER_ROUTE_RECORD;
  else if (sOrderRoutingMode == "oti")    m_OrderRoutingMode = ORDER_ROUTE_OTI;

  string sTCPOrEmbeddedMode = pt.get<string>("SystemSettings.TCPOrEmbeddedMode");
  if (sTCPOrEmbeddedMode == "embedded")
    m_TCPOrEmbeddedMode = EMBEDDED;
  else if (sTCPOrEmbeddedMode == "tcpwithoutack")
    m_TCPOrEmbeddedMode = TCPWITHOUTACK;
  else if (sTCPOrEmbeddedMode == "tcpwithack")
    m_TCPOrEmbeddedMode = TCPWITHACK;

  switch(pt.get<int> ("MarketData.TradeVolumeMode"))
  {
    case 0:  { m_MktData_TradeVolumeMode = PER_TRADE_VOLUME_MODE;   break;   }
    case 1:  { m_MktData_TradeVolumeMode = ACCUMULATED_VOLUME_MODE; break;   }
    default: { m_MktData_TradeVolumeMode = ACCUMULATED_VOLUME_MODE; break;   }
  }

  {
    string sBarAggregationM1Symbols = pt.get<string>("MarketData.BarAggregationM1Symbols");
    vector<string> vs;
    boost::split(vs, sBarAggregationM1Symbols, boost::is_any_of(","));

    vector<string>::iterator it;
    for (it = vs.begin(); it !=  vs.end(); ++it)
    {
      m_BarAggregationM1Symbols.insert(*it);
    }
  }

  {
    string sBarAggregationD1Symbols = pt.get<string>("MarketData.BarAggregationD1Symbols");
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
    string sSymToBeWarmedUpWithDayBars= pt.get<string>("TechIndicators.SymToBeWarmedUpWithDayBars");
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
