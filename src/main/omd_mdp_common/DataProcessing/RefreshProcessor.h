/*
 * RefreshProcessor.h
 *
 *  Created on: May 19, 2014
 *      Author: sunny
 */

#ifndef REFRESHPROCESSOR_H_
#define REFRESHPROCESSOR_H_

#include "Logger.h"
#include "SharedObjects.h"
#include "SystemConfig.h"
#include "BinaryTools.h"
#include "OMD_Message_Headers.h"
#include "ExpandableCirBuffer4Msg.h"
#include "OrderBook.h"
#include "DataProcFunctions.h"
#include "ThreadHealthMonitor.h"
#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <cstring>

using namespace std;
using namespace boost;

class RefreshProcessor {
  public:
    RefreshProcessor(const unsigned short);
    virtual void Run() = 0;
    virtual ~RefreshProcessor() {}
  protected:
    boost::shared_ptr<DataProcFunctions>      m_DataProcFunc;

    //Output related
    bool                               m_bOutputJson;
    bool                               m_bRunRefreshProcessor;
    bool                               m_PrintRefreshProcSeqNoAsInfo;
    bool                               m_PrintOrderBookAsInfo;

    //Shared objects
    ExpandableCirBuffer4Msg *          m_MsgCirBuf_RT;
    ExpandableCirBuffer4Msg *          m_MsgCirBuf_RF;
    boost::shared_ptr<SharedObjects>          m_ShrObj;
    boost::shared_ptr<SystemConfig>           m_SysCfg;
    boost::shared_ptr<Logger>                 m_Logger;
    boost::shared_ptr<ThreadHealthMonitor>    m_ThreadHealthMon;

    //Others
    unsigned short                     m_ChannelID;
    unsigned short                     m_RefreshProcSleepMillisec;
    char                               m_JsonBuffer[JSON_BUFFER_SIZE];
    uint32_t                           m_LastCheckedAdjSeqNo;
    bool                               m_MaintainRefreshActn;
    unsigned int                       m_RFMsgCirBufProtection;

    //--------------------------------------------------
    // Velocity of Refresh Complete coverage
    //--------------------------------------------------
    uint32_t                           m_RTSeqNo1;
    uint32_t                           m_RTSeqNo2;
};

class RefreshProcessor_OMD : public RefreshProcessor {
  public:
    RefreshProcessor_OMD(const unsigned short us) : RefreshProcessor(us) {}
    virtual ~RefreshProcessor_OMD() {}
    virtual void Run();
};

class RefreshProcessor_MDP : public RefreshProcessor {
  public:
    RefreshProcessor_MDP(const unsigned short us) : RefreshProcessor(us) {}
    virtual ~RefreshProcessor_MDP() {}
    virtual void Run();
};

class RefreshProcessorFactory {
  public:
    static RefreshProcessor* GetRefreshProcessor(SystemConfig::Identity, const unsigned short);
};

#endif /* REFRESHPROCESSOR_H_ */
