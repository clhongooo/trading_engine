//**************************************************
//  Author:      Sunny Yan
//  Created On:  Tue Apr 29 14:53:25 HKT 2014
//  Description:
//
//**************************************************

#ifndef FULLFUNCTIONPROCESSOR_H_
#define FULLFUNCTIONPROCESSOR_H_

#include "Logger.h"
#include "SharedObjects.h"
#include "SystemConfig.h"
#include "BinaryTools.h"
#include "OMD_Message_Headers.h"
#include "MDP_Message_Headers.h"
#include "ExpandableCirBuffer4Msg.h"
#include "OrderBook.h"
#include "DataProcFunctions.h"
#include "ThreadHealthMonitor.h"

#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <cstring>

using namespace std;
using namespace boost;

class RealTimeProcessor {
  public:
    RealTimeProcessor(const unsigned short);
    virtual void Run() = 0;
    virtual ~RealTimeProcessor() {}
  protected:
    boost::shared_ptr<DataProcFunctions>      m_DataProcFunc;

    //Output related
    BinaryRecorder m_BinaryRecorder;
    bool    m_bRecordProcessedData;
    bool    m_bOutputJson;
    bool    m_bRunRealTimeProcessor;
    bool    m_PrintRealTimeSeqNoAsInfo;
    bool    m_PrintOrderBookAsInfo;

    //Shared objects
    ExpandableCirBuffer4Msg *                 m_MsgCirBuf;
    boost::shared_ptr<SharedObjects>          m_ShrObj;
    boost::shared_ptr<SystemConfig>           m_SysCfg;
    boost::shared_ptr<Logger>                 m_Logger;
    boost::shared_ptr<ThreadHealthMonitor>    m_ThreadHealthMon;

    //Others
    unsigned short   m_ChannelID;
    char             m_JsonBuffer[JSON_BUFFER_SIZE];
};

class RealTimeProcessor_OMD : public RealTimeProcessor {
  public:
    RealTimeProcessor_OMD(const unsigned short us) : RealTimeProcessor(us) {}
    virtual ~RealTimeProcessor_OMD() {}
    void Run();
};

class RealTimeProcessor_MDP : public RealTimeProcessor {
  public:
    RealTimeProcessor_MDP(const unsigned short us) : RealTimeProcessor(us) {}
    virtual ~RealTimeProcessor_MDP() {}
    void Run();
};

class RealTimeProcessorFactory {
  public:
    static RealTimeProcessor* GetRealTimeProcessor(SystemConfig::Identity, const unsigned short);
};

#endif
