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

SystemConfig::SystemConfig() :
  m_Identity(SystemConfig::UNKNOWN),
  m_ProgramStartTime(boost::posix_time::microsec_clock::local_time()),
  m_ConfigPath("Config.ini"),
  m_CannedMcastFilePath(""),
  m_CannedProcessedDataFilePath("wb+"),
  m_CannedMcastFopenFlag("wb+"),
  m_CannedProcessedDataFopenFlag("wb+"),
  m_RefreshProcSleepMillisec(10),
  m_DataComplInspectorSleepMillisec(15),
  m_RTSClientSleepMillisec(1000),
  m_RunRealTimeProcessor(true),
  m_RunRefreshProcessor(true),
  m_MemoryBlockSize(4096),
  m_MaxOneTimeAlloc(409600),
  m_TrashSeqNoGapLargerThan(819200),
  m_TriggerRetransmissionSeqNoGap(100),
  m_TriggerRetransmissionTimeGapMillisec(60000),
  m_TriggerRefreshSeqNoGap(10000),
  m_TriggerRefreshTimeGapMillisec(180000),
  m_RTSRequestMaxSeqNoRange(10000),
  m_RTSRequestMaxMsg(50000),
  m_PrintPreProcSeqNoAsInfo(false),
  m_PrintRealTimeProcSeqNoAsInfo(false),
  m_PrintRefreshProcSeqNoAsInfo(false),
  m_PrintOrderBookAsInfo(true),
  m_RTS_Username("T-CELESTIAL3"),
  m_RTS_NumOfSvr(1),
  m_RTS_ReconnectionWaitSec(5),
  m_SystemServicePort(8000),
  m_RFMsgCirBufProtection(30000)
{
  SetProgramStartTime();
  Reset();
}

void SystemConfig::Reset()
{
  m_Logger = Logger::Instance();
  m_pActiveMcastChannels.reset(new vector<unsigned short>());
  m_pPreProcessorRTCannedChnl.reset(new vector<unsigned short>());
  m_pPreProcessorRFCannedChnl.reset(new vector<unsigned short>());
  m_pRealTimeProcCannedChnl.reset(new vector<unsigned short>());
  m_pMcastRecvrPrintPktHdr.reset(new vector<unsigned short>());
  m_pPreProcessorRTJson.reset(new vector<unsigned short>());
  m_pPreProcessorRFJson.reset(new vector<unsigned short>());
  m_pRealTimeProcessorJson.reset(new vector<unsigned short>());
  m_pRefreshProcessorJson.reset(new vector<unsigned short>());
  m_pRTSClientJson.reset(new vector<unsigned short>());
  m_pMcastIdentifiers.reset(new vector<McastIdentifier>());

  m_pRTS_ServerIP.reset(new vector<string>());
  m_pRTS_ServerPort.reset(new vector<unsigned short>());

  m_pMcastRecvrPrintPktHdr->insert(  m_pMcastRecvrPrintPktHdr->begin(),  OMD_MAXIMUM_MESSAGE_TYPE,  0);
  m_pPreProcessorRTJson->insert(     m_pPreProcessorRTJson->begin(),     OMD_MAXIMUM_MESSAGE_TYPE,  0);
  m_pPreProcessorRFJson->insert(     m_pPreProcessorRFJson->begin(),     OMD_MAXIMUM_MESSAGE_TYPE,  0);
  m_pRealTimeProcessorJson->insert(  m_pRealTimeProcessorJson->begin(),  OMD_MAXIMUM_MESSAGE_TYPE,  0);
  m_pRefreshProcessorJson->insert(   m_pRefreshProcessorJson->begin(),   OMD_MAXIMUM_MESSAGE_TYPE,  0);
  m_pRTSClientJson->insert(          m_pRTSClientJson->begin(),          OMD_MAXIMUM_MESSAGE_TYPE,  0);
}

void SystemConfig::SetConfigPath(const string & sConfigPath)
{
  m_ConfigPath = sConfigPath;
}

void SystemConfig::SetProgramStartTime()
{
  m_ProgramStartTime = boost::posix_time::microsec_clock::local_time();
}

boost::posix_time::ptime SystemConfig::GetProgramStartTime() const
{
  return m_ProgramStartTime;
}

const  boost::shared_ptr<vector<McastIdentifier> >   SystemConfig::GetMcastIdentifiers()                 const     {  return  m_pMcastIdentifiers;            }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetActiveMcastChnl()                  const     {  return  m_pActiveMcastChannels;         }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetPreProcessorRTCannedChnl()         const     {  return  m_pPreProcessorRTCannedChnl;    }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetPreProcessorRFCannedChnl()         const     {  return  m_pPreProcessorRFCannedChnl;    }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetRealTimeProcCannedChnl()           const     {  return  m_pRealTimeProcCannedChnl;      }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetMcastRecvrPrintPktHdr()            const     {  return  m_pMcastRecvrPrintPktHdr;       }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetPreProcessorRTJson()               const     {  return  m_pPreProcessorRTJson;          }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetPreProcessorRFJson()               const     {  return  m_pPreProcessorRFJson;          }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetRealTimeProcessorJson()            const     {  return  m_pRealTimeProcessorJson;       }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetRefreshProcessorJson()             const     {  return  m_pRefreshProcessorJson;        }
const  boost::shared_ptr<vector<unsigned  short>  >  SystemConfig::GetRTSClientJson()                    const     {  return  m_pRTSClientJson;               }

const  SystemConfig::Identity SystemConfig::GetIdentity()                           const  {  return  m_Identity;                                      }

const  unsigned  long  SystemConfig::GetTriggerRetransmissionSeqNoGap()             const  {  return  m_TriggerRetransmissionSeqNoGap;                 }
const  unsigned  long  SystemConfig::GetTriggerRetransmissionTimeGapMillisec()      const  {  return  m_TriggerRetransmissionTimeGapMillisec;          }
const  unsigned  long  SystemConfig::GetTriggerRefreshSeqNoGap()                    const  {  return  m_TriggerRefreshSeqNoGap;                        }
const  unsigned  long  SystemConfig::GetTriggerRefreshTimeGapMillisec()             const  {  return  m_TriggerRefreshTimeGapMillisec;                 }
const  unsigned  long  SystemConfig::GetRTSRequestMaxSeqNoRange()                   const  {  return  m_RTSRequestMaxSeqNoRange;                       }
const  unsigned  long  SystemConfig::GetRTSRequestMaxMsg()                          const  {  return  m_RTSRequestMaxMsg;                              }
const  string    &     SystemConfig::GetCannedMcastFilePath()                       const  {  return  m_CannedMcastFilePath;                           }
const  string    &     SystemConfig::GetCannedProcessedDataFilePath()               const  {  return  m_CannedProcessedDataFilePath;                   }
const  string    &     SystemConfig::GetCannedMcastFopenFlag()                      const  {  return  m_CannedMcastFopenFlag;                          }
const  string    &     SystemConfig::GetCannedProcessedDataFopenFlag()              const  {  return  m_CannedProcessedDataFopenFlag;                  }
const  unsigned  int   SystemConfig::GetMemoryBlockSize()                           const  {  return  m_MemoryBlockSize;                               }
const  unsigned  long  SystemConfig::GetMaxOneTimeAlloc()                           const  {  return  m_MaxOneTimeAlloc;                               }
const  unsigned  long  SystemConfig::GetTrashSeqNoGapLargerThan()                   const  {  return  m_TrashSeqNoGapLargerThan;                       }
const  unsigned  long  SystemConfig::GetRefreshProcSleepMillisec()                  const  {  return  m_RefreshProcSleepMillisec;                      }
const  unsigned  long  SystemConfig::GetDataComplInspectorSleepMillisec()           const  {  return  m_DataComplInspectorSleepMillisec;               }
const  unsigned  long  SystemConfig::GetRTSClientSleepMillisec()                    const  {  return  m_RTSClientSleepMillisec;                        }

const string                              SystemConfig::GetRTSUsername()            const  {  return  m_RTS_Username;                   }
const unsigned short                      SystemConfig::GetRTSNumOfSvr()            const  {  return  m_RTS_NumOfSvr;                   }
const boost::shared_ptr<vector<string> >         SystemConfig::GetRTSServerIP()            const  {  return  m_pRTS_ServerIP;                  }
const boost::shared_ptr<vector<unsigned short> > SystemConfig::GetRTSServerPort()          const  {  return  m_pRTS_ServerPort;                }
const unsigned short                      SystemConfig::GetRTSReconnectionWaitSec() const  {  return  m_RTS_ReconnectionWaitSec;        }
const unsigned short                      SystemConfig::GetSystemServicePort()      const  {  return  m_SystemServicePort;              }
const unsigned int                        SystemConfig::GetRFMsgCirBufProtection()  const  {  return  m_RFMsgCirBufProtection;          }

bool  SystemConfig::CheckIfRunRealTimeProcessor()                   const  {  return  m_RunRealTimeProcessor;                   }
bool  SystemConfig::CheckIfRunRefreshProcessor()                    const  {  return  m_RunRefreshProcessor;                    }
bool  SystemConfig::CheckIfPrintPreProcSeqNoAsInfo()                const  {  return  m_PrintPreProcSeqNoAsInfo;                }
bool  SystemConfig::CheckIfPrintRealTimeProcSeqNoAsInfo()           const  {  return  m_PrintRealTimeProcSeqNoAsInfo;           }
bool  SystemConfig::CheckIfPrintRefreshProcSeqNoAsInfo()            const  {  return  m_PrintRefreshProcSeqNoAsInfo;            }
bool  SystemConfig::CheckIfPrintOrderBookAsInfo()                   const  {  return  m_PrintOrderBookAsInfo;                   }


void SystemConfig::ReadConfig(const string & sConfigPath)
{
  m_Logger->Write(Logger::INFO,"Reading Config file: %s", sConfigPath.c_str());

  // Read config file
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(sConfigPath, pt);

  string sLogLevel = pt.get<std::string>("SystemSettings.LogLevel");
  if (sLogLevel == "EMERGENCY")      { m_Logger->SetLogLevel(Logger::EMERGENCY); }
  else if (sLogLevel == "ALERT")     { m_Logger->SetLogLevel(Logger::ALERT);     }
  else if (sLogLevel == "CRITICAL")  { m_Logger->SetLogLevel(Logger::CRITICAL);  }
  else if (sLogLevel == "ERROR")     { m_Logger->SetLogLevel(Logger::ERROR);     }
  else if (sLogLevel == "WARNING")   { m_Logger->SetLogLevel(Logger::WARNING);   }
  else if (sLogLevel == "NOTICE")    { m_Logger->SetLogLevel(Logger::NOTICE);    }
  else if (sLogLevel == "INFO")      { m_Logger->SetLogLevel(Logger::INFO);      }
  else if (sLogLevel == "DEBUG")     { m_Logger->SetLogLevel(Logger::DEBUG);     }

  string sIdentity = pt.get<std::string>("SystemSettings.Identity");
  if      (sIdentity == "OMDC") m_Identity = SystemConfig::OMDC;
  else if (sIdentity == "OMDD") m_Identity = SystemConfig::OMDD;
  else if (sIdentity == "MDP" ) m_Identity = SystemConfig::MDP;

  m_CannedMcastFilePath                               = pt.get<std::string>   ("SystemSettings.CannedMcastFilePath");
  m_CannedMcastFopenFlag                              = pt.get<std::string>   ("SystemSettings.CannedMcastFopenFlag");
  m_CannedProcessedDataFilePath                       = pt.get<std::string>   ("SystemSettings.CannedProcessedDataFilePath");
  m_CannedProcessedDataFopenFlag                      = pt.get<std::string>   ("SystemSettings.CannedProcessedDataFopenFlag");
  m_DataComplInspectorSleepMillisec                   = pt.get<unsigned short>("SystemSettings.DataCompletenessInspectorSleepMillisec");
  m_RefreshProcSleepMillisec                          = pt.get<unsigned short>("SystemSettings.RefreshProcSleepMillisec");
  m_MemoryBlockSize                                   = pt.get<unsigned int>  ("SystemSettings.MemoryBlockSize");
  m_MaxOneTimeAlloc                                   = pt.get<unsigned long> ("SystemSettings.MaxOneTimeAlloc");
  m_TrashSeqNoGapLargerThan                           = pt.get<unsigned long> ("SystemSettings.TrashSeqNoGapLargerThan");
  m_PrintRealTimeProcSeqNoAsInfo                      = pt.get<bool>          ("SystemSettings.PrintRealTimeProcSeqNoAsInfo");
  m_PrintRefreshProcSeqNoAsInfo                       = pt.get<bool>          ("SystemSettings.PrintRefreshProcSeqNoAsInfo");
  m_PrintOrderBookAsInfo                              = pt.get<bool>          ("SystemSettings.PrintOrderBookAsInfo");
  m_PrintPreProcSeqNoAsInfo                           = pt.get<bool>          ("SystemSettings.PrintPreProcSeqNoAsInfo");
  m_RTSClientSleepMillisec                            = pt.get<unsigned short>("SystemSettings.RTSClientSleepMillisec");
  m_RTSRequestMaxMsg                                  = pt.get<unsigned long> ("Retransmission.RTSRequestMaxMsg");
  m_RTSRequestMaxSeqNoRange                           = pt.get<unsigned long> ("Retransmission.RTSRequestMaxSeqNoRange");
  m_RTS_NumOfSvr                                      = pt.get<unsigned short>("Retransmission.NumOfServers");
  m_RTS_ReconnectionWaitSec                           = pt.get<unsigned short>("Retransmission.ReconnectionWaitSec");
  m_RTS_Username                                      = pt.get<string>        ("Retransmission.Username");
  m_RunRealTimeProcessor                              = pt.get<bool>          ("RealTimeProcessor.RunRealTimeProcessor");
  m_RunRefreshProcessor                               = pt.get<bool>          ("RefreshProcessor.RunRefreshProcessor");
  m_TriggerRefreshSeqNoGap                            = pt.get<unsigned long> ("RefreshProcessor.TriggerRefreshSeqNoGap");
  m_TriggerRefreshTimeGapMillisec                     = pt.get<unsigned long> ("RefreshProcessor.TriggerRefreshTimeGapMillisec");
  m_TriggerRetransmissionSeqNoGap                     = pt.get<unsigned long> ("Retransmission.TriggerRetransmissionSeqNoGap");
  m_TriggerRetransmissionTimeGapMillisec              = pt.get<unsigned long> ("Retransmission.TriggerRetransmissionTimeGapMillisec");
  m_SystemServicePort                                 = pt.get<unsigned short>("SystemSettings.SystemServicePort");
  m_RFMsgCirBufProtection                             = pt.get<unsigned int>  ("RefreshProcessor.RFMsgCirBufProtection");

  //--------------------------------------------------

  m_pRTS_ServerIP.reset(new vector<string>());
  m_pRTS_ServerPort.reset(new vector<unsigned short>());
  for (unsigned int i = 1; i < m_RTS_NumOfSvr+1; ++i)
  {
    m_pRTS_ServerIP->push_back(pt.get<string>("Retransmission.ServerIP"+lexical_cast<string>(i)));
    m_pRTS_ServerPort->push_back(pt.get<unsigned short>("Retransmission.ServerPort"+lexical_cast<string>(i)));
  }

  //--------------------------------------------------


  string sLogPath = pt.get<std::string>("SystemSettings.LogPath");
  m_Logger->SetPath(sLogPath.c_str());

  {
    string sPreProcessorRTCannedChnl = pt.get<std::string>("PreProcessor.Realtime_OutputCanned");
    char_separator<char> sep1(",");
    tokenizer<char_separator<char> > tokens1(sPreProcessorRTCannedChnl, sep1);
    for (tokenizer<char_separator<char> >::iterator it = tokens1.begin(); it != tokens1.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      m_pPreProcessorRTCannedChnl->push_back(i);
    }
  }

  {
    string sPreProcessorRFCannedChnl = pt.get<std::string>("PreProcessor.Refresh_OutputCanned");
    char_separator<char> sep2(",");
    tokenizer<char_separator<char> > tokens2(sPreProcessorRFCannedChnl, sep2);
    for (tokenizer<char_separator<char> >::iterator it = tokens2.begin(); it != tokens2.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      m_pPreProcessorRFCannedChnl->push_back(i);
    }
  }

  {
    string sRealTimeProcCannedChnl = pt.get<std::string>("RealTimeProcessor.OutputCanned");
    char_separator<char> sep3(",");
    tokenizer<char_separator<char> > tokens3(sRealTimeProcCannedChnl, sep3);
    for (tokenizer<char_separator<char> >::iterator it = tokens3.begin(); it != tokens3.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      m_pRealTimeProcCannedChnl->push_back(i);
    }
  }


  string sActiveMcastChannels = pt.get<std::string>("SystemSettings.ActiveMulticastChannels");
  char_separator<char> sep(",");
  tokenizer<char_separator<char> > tokens(sActiveMcastChannels, sep);
  for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
  {
    int i = lexical_cast<int>(*it);
    m_pActiveMcastChannels->push_back(i);
  }

  for (vector<unsigned short>::iterator it = m_pActiveMcastChannels->begin();
      it != m_pActiveMcastChannels->end();
      ++it)
  {
    string sRT_IP_A = pt.get<std::string>("MulticastChannel_"+lexical_cast<string>(*it)+".RealtimeServerIP_A");
    string sRT_IP_B = pt.get<std::string>("MulticastChannel_"+lexical_cast<string>(*it)+".RealtimeServerIP_B");
    string sRF_IP_A = pt.get<std::string>("MulticastChannel_"+lexical_cast<string>(*it)+".RefreshServerIP_A");
    string sRF_IP_B = pt.get<std::string>("MulticastChannel_"+lexical_cast<string>(*it)+".RefreshServerIP_B");

    unsigned short usRT_Port_A = pt.get<unsigned short>("MulticastChannel_"+lexical_cast<string>(*it)+".RealtimeServerPort_A");
    unsigned short usRT_Port_B = pt.get<unsigned short>("MulticastChannel_"+lexical_cast<string>(*it)+".RealtimeServerPort_B");
    unsigned short usRF_Port_A = pt.get<unsigned short>("MulticastChannel_"+lexical_cast<string>(*it)+".RefreshServerPort_A");
    unsigned short usRF_Port_B = pt.get<unsigned short>("MulticastChannel_"+lexical_cast<string>(*it)+".RefreshServerPort_B");

    m_pMcastIdentifiers->push_back(McastIdentifier(
          *it, sRT_IP_A, usRT_Port_A, McastIdentifier::REALTIME, McastIdentifier::A));
    m_pMcastIdentifiers->push_back(McastIdentifier(
          *it, sRT_IP_B, usRT_Port_B, McastIdentifier::REALTIME, McastIdentifier::B));
    m_pMcastIdentifiers->push_back(McastIdentifier(
          *it, sRF_IP_A, usRF_Port_A, McastIdentifier::REFRESH, McastIdentifier::A));
    m_pMcastIdentifiers->push_back(McastIdentifier(
          *it, sRF_IP_B, usRF_Port_B, McastIdentifier::REFRESH, McastIdentifier::B));

  }

  {
    string sMcastRecvrPrintPktHdr = pt.get<std::string>("MulticastReceiver.PrintPacketHeader");
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tokens(sMcastRecvrPrintPktHdr, sep);
    for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      (*m_pMcastRecvrPrintPktHdr)[i] = 1;
    }
  }

  {
    string sPreProcessorRTJson = pt.get<std::string>("PreProcessor.Realtime_OutputJson");
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tokens(sPreProcessorRTJson, sep);
    for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      (*m_pPreProcessorRTJson)[i] = 1;
    }
  }

  {
    string sPreProcessorRFJson = pt.get<std::string>("PreProcessor.Refresh_OutputJson");
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tokens(sPreProcessorRFJson, sep);
    for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      (*m_pPreProcessorRFJson)[i] = 1;
    }
  }

  {
    string sRealTimeProcessorJson = pt.get<std::string>("RealTimeProcessor.OutputJson");
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tokens(sRealTimeProcessorJson, sep);
    for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      (*m_pRealTimeProcessorJson)[i] = 1;
    }
  }

  {
    string sRefreshProcessorJson = pt.get<std::string>("RefreshProcessor.OutputJson");
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tokens(sRefreshProcessorJson, sep);
    for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      (*m_pRefreshProcessorJson)[i] = 1;
    }
  }

  {
    string sRTSClientJson = pt.get<std::string>("Retransmission.OutputJson");
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tokens(sRTSClientJson, sep);
    for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      (*m_pRTSClientJson)[i] = 1;
    }
  }

  //--------------------------------------------------
  // At the end of each ReadConfig or ReadConfigOptional, our SharedObjects need to be reinit
  //--------------------------------------------------
  boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
  pShrObj->InitializeSharedObjects(false);
  pShrObj->ResetObjects();


  return;
}
void SystemConfig::ReadConfigOptional(const string & sConfigPath)
{
  m_Logger->Write(Logger::INFO,"Reading Config file: %s", sConfigPath.c_str());

  // Read config file
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(sConfigPath, pt);

  string sLogLevel;
  boost::optional<string> oLogLevel = pt.get_optional<std::string>("SystemSettings.LogLevel");

  if (oLogLevel)
  {
    sLogLevel = oLogLevel.get();

    if (sLogLevel == "EMERGENCY")      { m_Logger->SetLogLevel(Logger::EMERGENCY); }
    else if (sLogLevel == "ALERT")     { m_Logger->SetLogLevel(Logger::ALERT); }
    else if (sLogLevel == "CRITICAL")  { m_Logger->SetLogLevel(Logger::CRITICAL); }
    else if (sLogLevel == "ERROR")     { m_Logger->SetLogLevel(Logger::ERROR); }
    else if (sLogLevel == "WARNING")   { m_Logger->SetLogLevel(Logger::WARNING); }
    else if (sLogLevel == "NOTICE")    { m_Logger->SetLogLevel(Logger::NOTICE); }
    else if (sLogLevel == "INFO")      { m_Logger->SetLogLevel(Logger::INFO); }
    else if (sLogLevel == "DEBUG")     { m_Logger->SetLogLevel(Logger::DEBUG); }
  }


  boost::optional<std::string>     oIdentity                                               =  pt.get_optional<std::string>     ("SystemSettings.Identity");
  boost::optional<bool>            oPrintRealTimeProcSeqNoAsInfo                           =  pt.get_optional<bool>            ("SystemSettings.PrintRealTimeProcSeqNoAsInfo");
  boost::optional<bool>            oPrintRefreshProcSeqNoAsInfo                            =  pt.get_optional<bool>            ("SystemSettings.PrintRefreshProcSeqNoAsInfo");
  boost::optional<bool>            oPrintOrderBookAsInfo                                   =  pt.get_optional<bool>            ("SystemSettings.PrintOrderBookAsInfo");
  boost::optional<bool>            oPrintPreProcSeqNoAsInfo                                =  pt.get_optional<bool>            ("SystemSettings.PrintPreProcSeqNoAsInfo");
  boost::optional<bool>            oRunRealTimeProcessor                                   =  pt.get_optional<bool>            ("RealTimeProcessor.RunRealTimeProcessor");
  boost::optional<bool>            oRunRefreshProcessor                                    =  pt.get_optional<bool>            ("RefreshProcessor.RunRefreshProcessor");
  boost::optional<std::string>     oCannedMcastFilePath                                    =  pt.get_optional<std::string>     ("SystemSettings.CannedMcastFilePath");
  boost::optional<std::string>     oCannedMcastFopenFlag                                   =  pt.get_optional<std::string>     ("SystemSettings.CannedMcastFopenFlag");
  boost::optional<std::string>     oCannedProcessedDataFilePath                            =  pt.get_optional<std::string>     ("SystemSettings.CannedProcessedDataFilePath");
  boost::optional<std::string>     oCannedProcessedDataFopenFlag                           =  pt.get_optional<std::string>     ("SystemSettings.CannedProcessedDataFopenFlag");
  boost::optional<string>          oRTS_Username                                           =  pt.get_optional<string>          ("Retransmission.Username");
  boost::optional<unsigned int>    oMemoryBlockSize                                        =  pt.get_optional<unsigned int>    ("SystemSettings.MemoryBlockSize");
  boost::optional<unsigned long>   oMaxOneTimeAlloc                                        =  pt.get_optional<unsigned long>   ("SystemSettings.MaxOneTimeAlloc");
  boost::optional<unsigned long>   oTrashSeqNoGapLargerThan                                =  pt.get_optional<unsigned long>   ("SystemSettings.TrashSeqNoGapLargerThan");
  boost::optional<unsigned long>   oRTSRequestMaxMsg                                       =  pt.get_optional<unsigned long>   ("Retransmission.RTSRequestMaxMsg");
  boost::optional<unsigned long>   oRTSRequestMaxSeqNoRange                                =  pt.get_optional<unsigned long>   ("Retransmission.RTSRequestMaxSeqNoRange");
  boost::optional<unsigned long>   oTriggerRefreshSeqNoGap                                 =  pt.get_optional<unsigned long>   ("RefreshProcessor.TriggerRefreshSeqNoGap");
  boost::optional<unsigned long>   oTriggerRefreshTimeGapMillisec                          =  pt.get_optional<unsigned long>   ("RefreshProcessor.TriggerRefreshTimeGapMillisec");
  boost::optional<unsigned long>   oTriggerRetransmissionSeqNoGap                          =  pt.get_optional<unsigned long>   ("Retransmission.TriggerRetransmissionSeqNoGap");
  boost::optional<unsigned long>   oTriggerRetransmissionTimeGapMillisec                   =  pt.get_optional<unsigned long>   ("Retransmission.TriggerRetransmissionTimeGapMillisec");
  boost::optional<unsigned short>  oDataComplInspectorSleepMillisec                        =  pt.get_optional<unsigned short>  ("SystemSettings.DataCompletenessInspectorSleepMillisec");
  boost::optional<unsigned short>  oRealTimeProcSleepMillisec                              =  pt.get_optional<unsigned short>  ("SystemSettings.RealTimeProcSleepMillisec");
  boost::optional<unsigned short>  oRefreshProcSleepMillisec                               =  pt.get_optional<unsigned short>  ("SystemSettings.RefreshProcSleepMillisec");
  boost::optional<unsigned short>  oPreProcessorSleepMillisec                              =  pt.get_optional<unsigned short>  ("SystemSettings.PreProcessorSleepMillisec");
  boost::optional<unsigned short>  oRTSClientSleepMillisec                                 =  pt.get_optional<unsigned short>  ("SystemSettings.RTSClientSleepMillisec");
  boost::optional<unsigned short>  oRTS_NumOfSvr                                           =  pt.get_optional<unsigned short>  ("Retransmission.NumOfServers");
  boost::optional<unsigned short>  oRTS_ReconnectionWaitSec                                =  pt.get_optional<unsigned short>  ("Retransmission.ReconnectionWaitSec");
  boost::optional<unsigned short>  oSystemServicePort                                      =  pt.get_optional<unsigned short>  ("SystemSettings.SystemServicePort");
  boost::optional<unsigned int>    oRFMsgCirBufProtection                                  =  pt.get_optional<unsigned int>    ("RefreshProcessor.RFMsgCirBufProtection");


  if (oIdentity)  {
    string sIdentity = oIdentity.get();
    if      (sIdentity == "OMDC") m_Identity = SystemConfig::OMDC;
    else if (sIdentity == "OMDD") m_Identity = SystemConfig::OMDD;
  }

  if  (oDataComplInspectorSleepMillisec              )  {  m_DataComplInspectorSleepMillisec           =  oDataComplInspectorSleepMillisec     .           get();  }
  if  (oRefreshProcSleepMillisec                     )  {  m_RefreshProcSleepMillisec                  =  oRefreshProcSleepMillisec            .           get();  }
  if  (oMemoryBlockSize                              )  {  m_MemoryBlockSize                           =  oMemoryBlockSize                     .           get();  }
  if  (oMaxOneTimeAlloc                              )  {  m_MaxOneTimeAlloc                           =  oMaxOneTimeAlloc                     .           get();  }
  if  (oTrashSeqNoGapLargerThan                      )  {  m_TrashSeqNoGapLargerThan                   =  oTrashSeqNoGapLargerThan             .           get();  }
  if  (oRTSClientSleepMillisec                       )  {  m_RTSClientSleepMillisec                    =  oRTSClientSleepMillisec              .           get();  }
  if  (oTriggerRetransmissionSeqNoGap                )  {  m_TriggerRetransmissionSeqNoGap             =  oTriggerRetransmissionSeqNoGap       .           get();  }
  if  (oTriggerRetransmissionTimeGapMillisec         )  {  m_TriggerRetransmissionTimeGapMillisec      =  oTriggerRetransmissionTimeGapMillisec.           get();  }
  if  (oRTSRequestMaxSeqNoRange                      )  {  m_RTSRequestMaxSeqNoRange                   =  oRTSRequestMaxSeqNoRange             .           get();  }
  if  (oRTSRequestMaxMsg                             )  {  m_RTSRequestMaxMsg                          =  oRTSRequestMaxMsg                    .           get();  }
  if  (oTriggerRefreshSeqNoGap                       )  {  m_TriggerRefreshSeqNoGap                    =  oTriggerRefreshSeqNoGap              .           get();  }
  if  (oTriggerRefreshTimeGapMillisec                )  {  m_TriggerRefreshTimeGapMillisec             =  oTriggerRefreshTimeGapMillisec       .           get();  }
  if  (oRunRealTimeProcessor                         )  {  m_RunRealTimeProcessor                      =  oRunRealTimeProcessor                .           get();  }
  if  (oRunRefreshProcessor                          )  {  m_RunRefreshProcessor                       =  oRunRefreshProcessor                 .           get();  }
  if  (oCannedMcastFilePath                          )  {  m_CannedMcastFilePath                       =  oCannedMcastFilePath                 .           get();  }
  if  (oCannedProcessedDataFilePath                  )  {  m_CannedProcessedDataFilePath               =  oCannedProcessedDataFilePath         .           get();  }
  if  (oCannedMcastFopenFlag                         )  {  m_CannedMcastFopenFlag                      =  oCannedMcastFopenFlag                .           get();  }
  if  (oCannedProcessedDataFopenFlag                 )  {  m_CannedProcessedDataFopenFlag              =  oCannedProcessedDataFopenFlag        .           get();  }
  if  (oPrintPreProcSeqNoAsInfo                      )  {  m_PrintPreProcSeqNoAsInfo                   =  oPrintPreProcSeqNoAsInfo             .           get();  }
  if  (oPrintRealTimeProcSeqNoAsInfo                 )  {  m_PrintRealTimeProcSeqNoAsInfo              =  oPrintRealTimeProcSeqNoAsInfo        .           get();  }
  if  (oPrintRefreshProcSeqNoAsInfo                  )  {  m_PrintRefreshProcSeqNoAsInfo               =  oPrintRefreshProcSeqNoAsInfo         .           get();  }
  if  (oPrintOrderBookAsInfo                         )  {  m_PrintOrderBookAsInfo                      =  oPrintOrderBookAsInfo                .           get();  }
  if  (oRTS_Username                                 )  {  m_RTS_Username                              =  oRTS_Username                        .           get();  }
  if  (oRTS_NumOfSvr                                 )  {  m_RTS_NumOfSvr                              =  oRTS_NumOfSvr                        .           get();  }
  if  (oRTS_ReconnectionWaitSec                      )  {  m_RTS_ReconnectionWaitSec                   =  oRTS_ReconnectionWaitSec             .           get();  }
  if  (oRFMsgCirBufProtection                        )  {  m_RFMsgCirBufProtection                     =  oRFMsgCirBufProtection               .           get();  }

  //--------------------------------------------------

  if (oRTS_NumOfSvr)
  {
    m_pRTS_ServerIP.reset(new vector<string>());
    m_pRTS_ServerPort.reset(new vector<unsigned short>());

    m_pRTS_ServerIP->clear();
    m_pRTS_ServerPort->clear();

    for (unsigned int i = 1; i < m_RTS_NumOfSvr+1; ++i)
    {
      m_pRTS_ServerIP->push_back(pt.get<string>("Retransmission.ServerIP"+lexical_cast<string>(i)));
      m_pRTS_ServerPort->push_back(pt.get<unsigned short>("Retransmission.ServerPort"+lexical_cast<string>(i)));
    }
  }

  //--------------------------------------------------

  boost::optional<string> oActiveMcastChannels = pt.get_optional<std::string>("SystemSettings.ActiveMulticastChannels");
  if (oActiveMcastChannels)
  {
    string sActiveMcastChannels = oActiveMcastChannels.get();

    m_pActiveMcastChannels->clear();
    m_pMcastIdentifiers->clear();

    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tokens(sActiveMcastChannels, sep);
    for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      m_pActiveMcastChannels->push_back(i);
      m_Logger->Write(Logger::INFO,"Active multicast channel: %d",i);
    }

    for (vector<unsigned short>::iterator it = m_pActiveMcastChannels->begin();
        it != m_pActiveMcastChannels->end();
        ++it)
    {
      string sRT_IP_A = pt.get<std::string>("MulticastChannel_"+lexical_cast<string>(*it)+".RealtimeServerIP_A");
      string sRT_IP_B = pt.get<std::string>("MulticastChannel_"+lexical_cast<string>(*it)+".RealtimeServerIP_B");
      string sRF_IP_A = pt.get<std::string>("MulticastChannel_"+lexical_cast<string>(*it)+".RefreshServerIP_A");
      string sRF_IP_B = pt.get<std::string>("MulticastChannel_"+lexical_cast<string>(*it)+".RefreshServerIP_B");

      unsigned short usRT_Port_A = pt.get<unsigned short>("MulticastChannel_"+lexical_cast<string>(*it)+".RealtimeServerPort_A");
      unsigned short usRT_Port_B = pt.get<unsigned short>("MulticastChannel_"+lexical_cast<string>(*it)+".RealtimeServerPort_B");
      unsigned short usRF_Port_A = pt.get<unsigned short>("MulticastChannel_"+lexical_cast<string>(*it)+".RefreshServerPort_A");
      unsigned short usRF_Port_B = pt.get<unsigned short>("MulticastChannel_"+lexical_cast<string>(*it)+".RefreshServerPort_B");

      m_pMcastIdentifiers->push_back(McastIdentifier(
            *it, sRT_IP_A, usRT_Port_A, McastIdentifier::REALTIME, McastIdentifier::A));
      m_pMcastIdentifiers->push_back(McastIdentifier(
            *it, sRT_IP_B, usRT_Port_B, McastIdentifier::REALTIME, McastIdentifier::B));
      m_pMcastIdentifiers->push_back(McastIdentifier(
            *it, sRF_IP_A, usRF_Port_A, McastIdentifier::REFRESH, McastIdentifier::A));
      m_pMcastIdentifiers->push_back(McastIdentifier(
            *it, sRF_IP_B, usRF_Port_B, McastIdentifier::REFRESH, McastIdentifier::B));

    }
  }

  //--------------------------------------------------
  // Pantheois has problems has problems setting the log path the second time
  //--------------------------------------------------

  // boost::optional<string> oLogPath = pt.get_optional<std::string>("SystemSettings.LogPath");
  // if (oLogPath)
  // {
  //   string sLogPath = oLogLevel.get();
  //   m_Logger->SetPath(sLogPath.c_str());
  // }

  boost::optional<string> oPreProcessorRTCannedChnl = pt.get_optional<std::string>("PreProcessor.Realtime_OutputCanned");
  if (oPreProcessorRTCannedChnl)
  {
    m_pPreProcessorRTCannedChnl->clear();
    string sPreProcessorRTCannedChnl = oPreProcessorRTCannedChnl.get();
    char_separator<char> sep1(",");
    tokenizer<char_separator<char> > tokens1(sPreProcessorRTCannedChnl, sep1);
    for (tokenizer<char_separator<char> >::iterator it = tokens1.begin(); it != tokens1.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      m_pPreProcessorRTCannedChnl->push_back(i);
    }
  }

  boost::optional<string> oPreProcessorRFCannedChnl = pt.get_optional<std::string>("PreProcessor.Refresh_OutputCanned");
  if (oPreProcessorRFCannedChnl)
  {
    m_pPreProcessorRFCannedChnl->clear();
    string sPreProcessorRFCannedChnl = oPreProcessorRFCannedChnl.get();
    char_separator<char> sep2(",");
    tokenizer<char_separator<char> > tokens2(sPreProcessorRFCannedChnl, sep2);
    for (tokenizer<char_separator<char> >::iterator it = tokens2.begin(); it != tokens2.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      m_pPreProcessorRFCannedChnl->push_back(i);
    }
  }


  boost::optional<string> oRealTimeProcCannedChnl = pt.get_optional<std::string>("RealTimeProcessor.OutputCanned");
  if (oRealTimeProcCannedChnl)
  {
    m_pRealTimeProcCannedChnl->clear();
    string sRealTimeProcCannedChnl = oRealTimeProcCannedChnl.get();
    char_separator<char> sep3(",");
    tokenizer<char_separator<char> > tokens3(sRealTimeProcCannedChnl, sep3);
    for (tokenizer<char_separator<char> >::iterator it = tokens3.begin(); it != tokens3.end(); ++it)
    {
      int i = lexical_cast<int>(*it);
      m_pRealTimeProcCannedChnl->push_back(i);
    }
  }

  {
    boost::optional<string> oMcastRecvrPrintPktHdr = pt.get_optional<std::string>("MulticastReceiver.PrintPacketHeader");
    if (oMcastRecvrPrintPktHdr)
    {
      for (unsigned int i = 0; i < OMD_MAXIMUM_MESSAGE_TYPE; ++i)
        (*m_pMcastRecvrPrintPktHdr)[i] = 0;

      string sMcastRecvrPrintPktHdr = oMcastRecvrPrintPktHdr.get();
      char_separator<char> sep(",");
      tokenizer<char_separator<char> > tokens(sMcastRecvrPrintPktHdr, sep);
      for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
      {
        int i = lexical_cast<int>(*it);
        (*m_pMcastRecvrPrintPktHdr)[i] = 1;
      }
    }
  }

  {
    boost::optional<string> oPreProcessorRTJson = pt.get_optional<std::string>("PreProcessor.Realtime_OutputJson");
    if (oPreProcessorRTJson)
    {
      for (unsigned int i = 0; i < OMD_MAXIMUM_MESSAGE_TYPE; ++i)
        (*m_pPreProcessorRTJson)[i] = 0;

      string sPreProcessorRTJson = oPreProcessorRTJson.get();
      char_separator<char> sep(",");
      tokenizer<char_separator<char> > tokens(sPreProcessorRTJson, sep);
      for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
      {
        int i = lexical_cast<int>(*it);
        (*m_pPreProcessorRTJson)[i] = 1;
      }
    }
  }

  {
    boost::optional<string> oPreProcessorRFJson = pt.get_optional<std::string>("PreProcessor.Refresh_OutputJson");
    if (oPreProcessorRFJson)
    {
      for (unsigned int i = 0; i < OMD_MAXIMUM_MESSAGE_TYPE; ++i)
        (*m_pPreProcessorRFJson)[i] = 0;

      string sPreProcessorRFJson = oPreProcessorRFJson.get();
      char_separator<char> sep(",");
      tokenizer<char_separator<char> > tokens(sPreProcessorRFJson, sep);
      for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
      {
        int i = lexical_cast<int>(*it);
        (*m_pPreProcessorRFJson)[i] = 1;
      }
    }
  }

  {
    boost::optional<string> oRealTimeProcessorJson = pt.get_optional<std::string>("RealTimeProcessor.OutputJson");
    if (oRealTimeProcessorJson)
    {
      for (unsigned int i = 0; i < OMD_MAXIMUM_MESSAGE_TYPE; ++i)
        (*m_pRealTimeProcessorJson)[i] = 0;

      string sRealTimeProcessorJson = oRealTimeProcessorJson.get();
      char_separator<char> sep(",");
      tokenizer<char_separator<char> > tokens(sRealTimeProcessorJson, sep);
      for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
      {
        int i = lexical_cast<int>(*it);
        (*m_pRealTimeProcessorJson)[i] = 1;
      }
    }
  }


  {
    boost::optional<string> oRefreshProcessorJson = pt.get_optional<std::string>("RefreshProcessor.OutputJson");
    if (oRefreshProcessorJson)
    {
      for (unsigned int i = 0; i < OMD_MAXIMUM_MESSAGE_TYPE; ++i)
        (*m_pRefreshProcessorJson)[i] = 0;

      string sRefreshProcessorJson = oRefreshProcessorJson.get();
      char_separator<char> sep(",");
      tokenizer<char_separator<char> > tokens(sRefreshProcessorJson, sep);
      for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
      {
        int i = lexical_cast<int>(*it);
        (*m_pRefreshProcessorJson)[i] = 1;
      }
    }
  }

  {
    boost::optional<string> oRTSClientJson = pt.get_optional<std::string>("Retransmission.OutputJson");
    if (oRTSClientJson)
    {
      for (unsigned int i = 0; i < OMD_MAXIMUM_MESSAGE_TYPE; ++i)
        (*m_pRTSClientJson)[i] = 0;

      string sRTSClientJson = oRTSClientJson.get();
      char_separator<char> sep(",");
      tokenizer<char_separator<char> > tokens(sRTSClientJson, sep);
      for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it)
      {
        int i = lexical_cast<int>(*it);
        (*m_pRTSClientJson)[i] = 1;
      }
    }
  }

  //--------------------------------------------------
  // At the end of each ReadConfig or ReadConfigOptional, our SharedObjects need to be reinit
  //--------------------------------------------------
  boost::shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
  pShrObj->InitializeSharedObjects(false);
  pShrObj->ResetObjects();


  return;
}
