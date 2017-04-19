/*
 * ChannelController.h
 *
 *  Created on: Apr 9, 2014
 *      Author: sunny
 */

#ifndef CHANNELCONTROLLER_H_
#define CHANNELCONTROLLER_H_

#include <deque>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "McastIdentifier.h"
#include "McastReceiver.h"
#include "DataCompletenessInspector.h"
#include "../OMD_Global.h"
#include "../SystemConfig.h"
#include "../SharedObjects.h"
#include "../Logger/Logger.h"

using namespace std;

class McastThreadCls
{
  public:
    McastThreadCls(const McastIdentifier &);
    ~McastThreadCls(){}
    void Run();
    void CallIoserviceStop();

  private:
    boost::asio::io_service m_ios;
    McastIdentifier m_McIdentifier;
};

class ChannelController {
  public:
    ChannelController();
    virtual ~ChannelController();
    //--------------------------------------------------
    // multicast receivers and recovery handlers 
    //--------------------------------------------------
    void StartRecvMcast();
    void StartDataCompletenessInspectors();
    //--------------------------------------------------
    // start these when needed by DataCompletenessInspector
    //--------------------------------------------------
    void StartRetransmission();
    void StartRefresh();
    void StopMcastRecvr();

  private:
    //--------------------------------------------------
    // multicast receiver 
    //--------------------------------------------------
    ptr_deque<McastThreadCls>             m_McastThreadCls;
    boost::thread_group                   m_McastRecvrBoostThreadGrp;
    //--------------------------------------------------
    // recovery handler 
    //--------------------------------------------------
    ptr_deque<DataCompletenessInspector>  m_DataComplInspector;
    boost::thread_group                   m_DataComplInspectorBoostThreadGrp;

    //------------------------------
    // System objects
    //------------------------------
    boost::shared_ptr<SystemConfig>    m_SysCfg;
    boost::shared_ptr<Logger>          m_Logger;
    boost::shared_ptr<SharedObjects>   m_ShrObj;
};

#endif
