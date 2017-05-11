#ifndef INSTRUMENTREPLAYPROCESSOR_H_
#define INSTRUMENTREPLAYPROCESSOR_H_

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
#include "Constants.h"

#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <cstring>

using namespace std;
using namespace boost;

class InstrumentReplayProcessor {
  public:
    InstrumentReplayProcessor(const unsigned short);
    virtual void Run() = 0;
    virtual ~InstrumentReplayProcessor() {}
  protected:
    boost::shared_ptr<DataProcFunctions>      m_DataProcFunc;

    //Output related
    bool    m_bRunInstrumentReplayProcessor;
    bool    m_PrintInstrumentReplaySeqNoAsInfo;

    //Shared objects
    ExpandableCirBuffer4Msg *                 m_MsgCirBuf_IR;
    boost::shared_ptr<SharedObjects>          m_ShrObj;
    boost::shared_ptr<SystemConfig>           m_SysCfg;
    boost::shared_ptr<Logger>                 m_Logger;
    boost::shared_ptr<ThreadHealthMonitor>    m_ThreadHealthMon;

    //Others
    unsigned short   m_ChannelID;
    char             m_JsonBuffer[JSON_BUFFER_SIZE];
};

class InstrumentReplayProcessor_MDP : public InstrumentReplayProcessor {
  public:
    InstrumentReplayProcessor_MDP(const unsigned short us) : InstrumentReplayProcessor(us) {}
    virtual ~InstrumentReplayProcessor_MDP() {}
    void Run();
};

class InstrumentReplayProcessorFactory {
  public:
    static InstrumentReplayProcessor* GetInstrumentReplayProcessor(dma::Identity, const unsigned short);
};

#endif
