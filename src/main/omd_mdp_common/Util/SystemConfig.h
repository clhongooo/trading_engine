//**************************************************
//  Author:      Sunny Yan
//  Created On:  Tue Apr 8 16:02:24 HKT 2014
//  Description: System-wide control, like reading config
//
//**************************************************
#ifndef SYSTEMCONFIG_HPP_
#define SYSTEMCONFIG_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

#include "McastIdentifier.h"
#include "Logger.h"
#include "BinaryTools.h"
#include "SharedObjects.h"
#include <vector>
#include <string>
#include <map>

using namespace std;
using namespace boost;

class SharedObjects;

class SystemConfig : boost::noncopyable
{
  public:
    enum Identity { UNKNOWN, OMDC, OMDD, MDP };
    static boost::shared_ptr<SystemConfig> Instance();
    void SetConfigPath(const string &);
    ~SystemConfig(){};
    void Reset();
    void ReadConfig(const string &);
    void ReadConfigOptional(const string &);
    void SetProgramStartTime();
    boost::posix_time::ptime GetProgramStartTime() const;

    const Identity                                    GetIdentity()                                      const;
    const boost::shared_ptr<vector<McastIdentifier> > GetMcastIdentifiers()                              const;
    const boost::shared_ptr<vector<unsigned short> >  GetActiveMcastChnl()                               const;
    const boost::shared_ptr<vector<unsigned short> >  GetPreProcessorRTCannedChnl()                      const;
    const boost::shared_ptr<vector<unsigned short> >  GetPreProcessorRFCannedChnl()                      const;
    const boost::shared_ptr<vector<unsigned short> >  GetRealTimeProcCannedChnl()                        const;
    const boost::shared_ptr<vector<unsigned short> >  GetMcastRecvrPrintPktHdr()                         const;
    const boost::shared_ptr<vector<unsigned short> >  GetPreProcessorRTJson()                            const;
    const boost::shared_ptr<vector<unsigned short> >  GetPreProcessorRFJson()                            const;
    const boost::shared_ptr<vector<unsigned short> >  GetRealTimeProcessorJson()                         const;
    const boost::shared_ptr<vector<unsigned short> >  GetRefreshProcessorJson()                          const;
    const boost::shared_ptr<vector<unsigned short> >  GetRTSClientJson()                                 const;
    const unsigned int                                GetMemoryBlockSize()                               const;
    const unsigned long                               GetMaxOneTimeAlloc()                               const;
    const unsigned long                               GetTrashSeqNoGapLargerThan()                       const;
    const unsigned long                               GetRefreshProcSleepMillisec()                      const;
    const unsigned long                               GetDataComplInspectorSleepMillisec()               const;
    const unsigned long                               GetRTSClientSleepMillisec()                        const;
    const string &                                    GetCannedMcastFilePath()                           const;
    const string &                                    GetCannedProcessedDataFilePath()                   const;
    const string &                                    GetCannedRefreshProcFilePath()                     const;
    const string &                                    GetCannedMcastFopenFlag()                          const;
    const string &                                    GetCannedProcessedDataFopenFlag()                  const;
    const unsigned long                               GetTriggerRetransmissionSeqNoGap()                 const;
    const unsigned long                               GetTriggerRetransmissionTimeGapMillisec()          const;
    const unsigned long                               GetTriggerRefreshSeqNoGap()                        const;
    const unsigned long                               GetTriggerRefreshTimeGapMillisec()                 const;
    const unsigned long                               GetRTSRequestMaxSeqNoRange()                       const;
    const unsigned long                               GetRTSRequestMaxMsg()                              const;



    const string                                      GetRTSUsername()                           const;
    const unsigned short                              GetRTSNumOfSvr()                           const;
    const boost::shared_ptr<vector<string> >          GetRTSServerIP()                           const;
    const boost::shared_ptr<vector<unsigned short> >  GetRTSServerPort()                         const;
    const unsigned short                              GetRTSReconnectionWaitSec()                const;
    const unsigned short                              GetSystemServicePort()                     const;
    const unsigned int                                GetRFMsgCirBufProtection()                 const;



    bool                                       CheckIfRunRealTimeProcessor()                  const;
    bool                                       CheckIfRunRefreshProcessor()                   const;
    bool                                       CheckIfPrintPreProcSeqNoAsInfo()               const;
    bool                                       CheckIfPrintRealTimeProcSeqNoAsInfo()          const;
    bool                                       CheckIfPrintRefreshProcSeqNoAsInfo()           const;
    bool                                       CheckIfPrintOrderBookAsInfo()                  const;

  private:
    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    SystemConfig();
    SystemConfig(SystemConfig const&);
    SystemConfig& operator=(SystemConfig const&){};
    static boost::weak_ptr<SystemConfig> m_pInstance;

    //--------------------------------------------------
    string m_ConfigPath;

    //--------------------------------------------------
    boost::shared_ptr<Logger> m_Logger;
    //--------------------------------------------------
    // Config.ini fields
    //--------------------------------------------------
    Identity                                       m_Identity;
    boost::posix_time::ptime                       m_ProgramStartTime;
    string                                         m_CannedMcastFilePath;
    string                                         m_CannedProcessedDataFilePath;
    string                                         m_CannedMcastFopenFlag;
    string                                         m_CannedProcessedDataFopenFlag;
    unsigned long                                  m_RefreshProcSleepMillisec;
    unsigned long                                  m_DataComplInspectorSleepMillisec;
    unsigned long                                  m_RTSClientSleepMillisec;
    bool                                           m_RunRealTimeProcessor;
    bool                                           m_RunRefreshProcessor;
    unsigned int                                   m_MemoryBlockSize;
    unsigned long                                  m_MaxOneTimeAlloc;
    unsigned long                                  m_TrashSeqNoGapLargerThan;
    unsigned long                                  m_TriggerRetransmissionSeqNoGap;
    unsigned long                                  m_TriggerRetransmissionTimeGapMillisec;
    unsigned long                                  m_TriggerRefreshSeqNoGap;
    unsigned long                                  m_TriggerRefreshTimeGapMillisec;
    unsigned long                                  m_RTSRequestMaxSeqNoRange;
    unsigned long                                  m_RTSRequestMaxMsg;
    boost::shared_ptr<vector<unsigned short> >     m_pActiveMcastChannels;
    boost::shared_ptr<vector<unsigned short> >     m_pPreProcessorRTCannedChnl;
    boost::shared_ptr<vector<unsigned short> >     m_pPreProcessorRFCannedChnl;
    boost::shared_ptr<vector<unsigned short> >     m_pRealTimeProcCannedChnl;
    boost::shared_ptr<vector<unsigned short> >     m_pMcastRecvrPrintPktHdr;
    boost::shared_ptr<vector<unsigned short> >     m_pPreProcessorRTJson;
    boost::shared_ptr<vector<unsigned short> >     m_pPreProcessorRFJson;
    boost::shared_ptr<vector<unsigned short> >     m_pRealTimeProcessorJson;
    boost::shared_ptr<vector<unsigned short> >     m_pRefreshProcessorJson;
    boost::shared_ptr<vector<unsigned short> >     m_pRTSClientJson;
    boost::shared_ptr<vector<McastIdentifier> >    m_pMcastIdentifiers;
    bool                                           m_PrintPreProcSeqNoAsInfo;
    bool                                           m_PrintRealTimeProcSeqNoAsInfo;
    bool                                           m_PrintRefreshProcSeqNoAsInfo;
    bool                                           m_PrintOrderBookAsInfo;
    string                                         m_RTS_Username;
    unsigned short                                 m_RTS_NumOfSvr;
    boost::shared_ptr<vector<string> >             m_pRTS_ServerIP;
    boost::shared_ptr<vector<unsigned short> >     m_pRTS_ServerPort;
    unsigned short                                 m_RTS_ReconnectionWaitSec;
    unsigned short                                 m_SystemServicePort;
    unsigned int                                   m_RFMsgCirBufProtection;
};

#endif
