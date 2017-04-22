/*
 * RTSClient.h
 *
 *  Created on: Apr 30, 2014
 *      Author: sunny
 */

#ifndef RTSCLIENT_H_
#define RTSCLIENT_H_

#include "BinaryTools.h"
#include "OMD_Message_Headers.h"
#include "SystemConfig.h"
#include "SharedObjects.h"
#include "SDateTime.h"
#include "Logger.h"
#include "DataProcFunctions.h"
#include "ThreadHealthMonitor.h"
#include <deque>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "DataProcFunctions.h"
#include <boost/optional.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio/write.hpp>

using namespace std;
using boost::asio::ip::tcp;
using boost::asio::deadline_timer;

class RTSClient
{
  public:
    enum enum_RTS_Logon_Resp
    {
      SessionActive     =  0,
      InvalidUserOrIP   =  5,
      UserAlrConnected  =  100
    };
    enum enum_RTS_Retran_Resp
    {
      RequestAccepted   =  0,
      UnknownChnlID     =  1,
      MsgNotAvg         =  2,
      ExceedMaxSeqRng   =  100,
      ExceedMaxReqPerDay=  101,
    };

    typedef struct {
      uint32_t  BeginSeqNum;
      uint32_t  EndSeqNum;
    } SeqNoRange;

    RTSClient();
    static boost::shared_ptr<RTSClient> Instance();
    ~RTSClient();
    void Run();
    void Init();
    void AddRTSRequest(unsigned short,uint32_t,uint32_t);
    bool IsNewRTSRequest(unsigned short,uint32_t,uint32_t);
    void PurgeAllRTSRequests();


  private:
    enum {TCPReadByte = 150000};
    enum {MILLISEC_RTSLOG = 10000 };
    static boost::weak_ptr<RTSClient> m_pInstance;
    boost::shared_ptr<DataProcFunctions> m_DataProcFunc;

    template <typename MutableBufferSequence>
      bool read_some_with_timeout(boost::shared_ptr<boost::asio::ip::tcp::socket> sock, const MutableBufferSequence& buffers, boost::system::error_code& ec, std::size_t& bytes_transferred);

    template <typename MutableBufferSequence>
      bool write_some_with_timeout(boost::shared_ptr<boost::asio::ip::tcp::socket> sock, const MutableBufferSequence& buffers, const size_t write_len, boost::system::error_code& ec);

    void handle_read(const boost::system::error_code&, size_t);
    void handle_write(const boost::system::error_code&, size_t);
    void handle_timerexpiry(const boost::system::error_code&);

    //--------------------------------------------------
    // Socket related
    //--------------------------------------------------
    boost::asio::io_service    m_io_service;
    boost::scoped_ptr<tcp::resolver>  m_resolver;
    boost::shared_ptr<tcp::socket>    m_socket;
    boost::scoped_ptr<deadline_timer> m_deadlinetimer;

    //--------------------------------------------------
    // Shared objects
    //--------------------------------------------------
    boost::shared_ptr<SystemConfig>   m_SysCfg;
    boost::shared_ptr<Logger>         m_Logger;
    boost::shared_ptr<SharedObjects>  m_ShrObj;
    boost::shared_ptr<ThreadHealthMonitor>  m_ThreadHealthMon;
    ExpandableCirBuffer4Msg*   m_MsgCirBuf;

    //--------------------------------------------------
    // RTS Server config
    //--------------------------------------------------
    string                              m_RTS_Username;
    string                              m_RTS_NumOfSvr;
    boost::shared_ptr<vector<string> >         m_pRTS_ServerIP;
    boost::shared_ptr<vector<unsigned short> > m_pRTS_ServerPort;
    unsigned short                      m_RTS_SvrIndex;
    unsigned short                      m_RTS_NoOfTimeout;
    unsigned short                      m_RTS_ReconnectionWaitSec;

    //--------------------------------------------------
    // Async Read / Write
    //--------------------------------------------------
    size_t                              m_BytesReadFrAsyncRead;
    size_t                              m_BytesWrittenFrAsyncWrite;
    bool                                m_TimerExpired;
    bool                                m_ReadWriteError;


    //--------------------------------------------------
    // Others
    //--------------------------------------------------
    unsigned long                       m_RTSClientSleepMillisec;
    char*                               m_Buffer;
    unsigned int                        m_BufBeginIdx;
    unsigned int                        m_BufEndIdx;
    bool                                m_bOutputJson;
    char                                m_JsonBuffer[JSON_BUFFER_SIZE];
    deque<unsigned short>               m_RequestQueue;
    map<unsigned short,SeqNoRange>      m_ChnlSeqNoRange;
    boost::shared_mutex                 m_Mutex;
    bool                                m_bExceedMaxReqPerDay;
    boost::posix_time::ptime            m_LastPrintLogTime;
};

#endif /* RTSCLIENT_H_ */
