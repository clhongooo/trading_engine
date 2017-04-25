//**************************************************
//  Author:      Sunny Yan
//  Created On:  Fri Apr 11 12:04:04 HKT 2014
//
//
//**************************************************
#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_

#include "Logger.h"
#include "SharedObjects.h"
#include "SystemConfig.h"
#include "SDateTime.h"
#include "McastIdentifier.h"
#include "BinaryTools.h"
#include "OMD_Message_Headers.h"
#include "OrderBook.h"
#include "DataProcFunctions.h"
#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <cstring>
#include <set>
#include "ThreadHealthMonitor.h"
#include "MDP_Message_Headers.h"
#include "MessageHeader.h"

using namespace std;
using namespace boost;

class PreProcessor {
  public:
    PreProcessor(const McastIdentifier &);
    virtual ~PreProcessor();
    virtual void Run() = 0;
  protected:
    boost::shared_ptr<DataProcFunctions> m_DataProcFunc;

    //Output related
    BinaryRecorder m_BinaryRecorder;
    bool   m_bRecordMcast;
    bool   m_bOutputJson;

    //--------------------------------------------------
    // Special Debug options, to be turned off in production
    //--------------------------------------------------
    bool   m_PrintPreProcSeqNoAsInfo;

    //Shared objects
    ExpandableCirBuffer *              m_RawPktCirBuf;
    ExpandableCirBuffer4Msg *          m_MsgCirBuf;
    boost::shared_ptr<SharedObjects>        m_ShrObj;
    boost::shared_ptr<SystemConfig>         m_SysCfg;
    boost::shared_ptr<Logger>               m_Logger;
    boost::shared_ptr<ThreadHealthMonitor>  m_ThreadHealthMon;

    //Others
    const McastIdentifier &    m_McastIdentifier;
    unsigned short             m_ChannelID;
    // uint32_t                   m_LastUnadjSeqNo;
    char                       m_JsonBuffer[JSON_BUFFER_SIZE];
    char                       m_NameBuffer[256];
    boost::posix_time::ptime   m_ProgramStartTime;
    // uint64_t                   m_PrevPktHdrTime;
    unsigned long              m_MaxOneTimeAlloc;
    unsigned long              m_TrashSeqNoGapLargerThan;
    uint32_t                   m_LocalLastAdjSeqNo;
};

class PreProcessor_OMD : public PreProcessor {
  public:
    PreProcessor_OMD(const McastIdentifier & m) : m_LocalLastBaseSeqNo(0), PreProcessor(m) {}
    virtual ~PreProcessor_OMD() {}
    void Run();
    bool DealingWithSeqNoGaps(uint32_t);
  private:
    uint32_t m_LocalLastBaseSeqNo;
};

class PreProcessor_MDP : public PreProcessor {
  public:
    PreProcessor_MDP(const McastIdentifier & m) : PreProcessor(m) {}
    virtual ~PreProcessor_MDP() {}
    void Run();
};

class PreProcessorFactory {
  public:
    static PreProcessor* GetPreProcessor(SystemConfig::Identity, const McastIdentifier &);
};

#endif /* PREPROCESSOR_H_ */
