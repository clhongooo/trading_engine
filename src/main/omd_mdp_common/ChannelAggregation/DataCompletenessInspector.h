//**************************************************
//  Author:      Sunny Yan
//  Created On:  Tue Apr 29 10:54:25 HKT 2014
//  Description: Detects gaps in our message circle buffer, then triggers RTS / refresh as needed.
//
//**************************************************

#ifndef DATACOMPLINSPECTOR_H_
#define DATACOMPLINSPECTOR_H_

#include <string>
#include <vector>
#include <deque>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/chrono.hpp>

#include "BinaryTools.h"
#include "McastIdentifier.h"
#include "RTSClient.h"
#include "SDateTime.h"
#include "ExpandableCirBuffer4Msg.h"
#include "Logger.h"
#include "SharedObjects.h"
#include "SystemConfig.h"
#include "ThreadHealthMonitor.h"

using namespace std;
using namespace boost;

class DataCompletenessInspector
{
  public:
    DataCompletenessInspector(const unsigned short);
    DataCompletenessInspector(const unsigned short,const bool);
    void Init();
    virtual ~DataCompletenessInspector() {}
    void Run();
    void NotifyRTSClient(uint32_t,uint32_t,uint32_t,bool);

  private:
    enum { MILLISEC_RTSLOG = 5000 };
    const unsigned short                  m_ChannelID;
    unsigned short                        m_DataComplInspectorSleepMillisec;
    scoped_ptr<boost::posix_time::ptime>  m_TimeSinceEpoch;
    //--------------------------------------------------
    // shared objects 
    //--------------------------------------------------
    boost::shared_ptr<Logger>             m_Logger;
    boost::shared_ptr<SharedObjects>      m_ShrObj;
    boost::shared_ptr<SystemConfig>       m_SysCfg;
    boost::shared_ptr<RTSClient>          m_RTSClt;
    boost::shared_ptr<ThreadHealthMonitor>       m_ThreadHealthMon;
    ExpandableCirBuffer4Msg *             m_MsgCirBuf_RT;

    unsigned long                         m_TriggerRetransmissionSeqNoGap;
    unsigned long                         m_TriggerRetransmissionTimeGapMillisec;
    unsigned long                         m_TriggerRefreshSeqNoGap;
    unsigned long                         m_TriggerRefreshTimeGapMillisec;
    unsigned long                         m_RTSRequestMaxSeqNoRange;
    unsigned long                         m_RTSRequestMaxMsg;
    // unsigned long                         m_RTSRequestBuffer;

    bool                                  m_UnitTestingMode;
    boost::posix_time::ptime              m_LastPrintLogTime;

};

#endif /* DATACOMPLINSPECTOR_H_ */
