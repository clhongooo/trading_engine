#include "RTSClient.h"

boost::weak_ptr<RTSClient> RTSClient::m_pInstance;

boost::shared_ptr<RTSClient> RTSClient::Instance() {
  boost::shared_ptr<RTSClient> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new RTSClient());
    m_pInstance = instance;
  }
  return instance;
}


RTSClient::RTSClient() :
  m_RTS_Username(""),
  m_RTS_NumOfSvr(""),
  m_RTS_SvrIndex(0),
  m_RTS_NoOfTimeout(0),
  m_RTS_ReconnectionWaitSec(5),
  m_BytesReadFrAsyncRead(0),
  m_BytesWrittenFrAsyncWrite(0),
  m_TimerExpired(false),
  m_ReadWriteError(false),
  m_RTSClientSleepMillisec(10),
  m_BufBeginIdx(0),
  m_BufEndIdx(0),
  m_bOutputJson(false),
  m_bExceedMaxReqPerDay(false),
  m_LastPrintLogTime(boost::posix_time::microsec_clock::local_time())
{

  //--------------------------------------------------
  // Initialization
  //--------------------------------------------------
  m_SysCfg  =  SystemConfig::Instance();
  m_ShrObj  =  SharedObjects::Instance();
  m_Logger  =  Logger::Instance();
  m_ThreadHealthMon =  ThreadHealthMonitor::Instance();

  m_Logger->Write(Logger::INFO, "RTSClient: RTSClient()");

  Init();

  m_resolver.reset(new tcp::resolver(m_io_service));
  m_socket.reset(new tcp::socket(m_io_service));
  m_deadlinetimer.reset(new deadline_timer(m_io_service));

  m_Buffer = new char[TCPReadByte];
  memset(m_Buffer,'\0',TCPReadByte);

  m_DataProcFunc.reset(DataProcFuncFactory::GetDataProcFunc(m_SysCfg->GetIdentity()));

}

RTSClient::~RTSClient()
{
  m_deadlinetimer->cancel();
  if (m_Buffer)
  {
    delete [] m_Buffer;
    m_Buffer = NULL;
  }
  return;
}

void RTSClient::Init()
{
  m_RTS_Username            =  m_SysCfg->GetRTSUsername();
  m_RTS_NumOfSvr            =  m_SysCfg->GetRTSNumOfSvr();
  m_RTS_ReconnectionWaitSec =  m_SysCfg->GetRTSReconnectionWaitSec();
  m_BytesReadFrAsyncRead    =  0;
  m_pRTS_ServerIP           =  m_SysCfg->GetRTSServerIP();
  m_pRTS_ServerPort         =  m_SysCfg->GetRTSServerPort();
  m_RTSClientSleepMillisec  =  m_SysCfg->GetRTSClientSleepMillisec();
  m_RTS_SvrIndex            =  0; // Starting from the first RTS server. // Starting from the first RTS server::resolver resolver(m_io_service);
  m_RTS_NoOfTimeout         =  0;

  return;
}

void RTSClient::AddRTSRequest(unsigned short usChannelID ,uint32_t uiBeginUnadjSeqNum, uint32_t uiEndUnadjSeqNum)
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);

  map<unsigned short,SeqNoRange>::iterator it;
  it = m_ChnlSeqNoRange.find(usChannelID);

  if (it != m_ChnlSeqNoRange.end())
  {
    //--------------------------------------------------
    // Just update with the latest figures, since the range of missing seq no is a moving target
    //--------------------------------------------------
    it->second.BeginSeqNum = uiBeginUnadjSeqNum;
    it->second.EndSeqNum = uiEndUnadjSeqNum;
  }
  else
  {
    SeqNoRange snr;
    snr.BeginSeqNum = uiBeginUnadjSeqNum;
    snr.EndSeqNum = uiEndUnadjSeqNum;
    m_ChnlSeqNoRange[usChannelID] = snr;
    m_RequestQueue.push_back(usChannelID);
  }
  return;
}

bool RTSClient::IsNewRTSRequest(unsigned short usChannelID ,uint32_t uiBeginUnadjSeqNum, uint32_t uiEndUnadjSeqNum)
{
  boost::shared_lock<boost::shared_mutex> lock(m_Mutex);

  map<unsigned short,SeqNoRange>::iterator it;
  it = m_ChnlSeqNoRange.find(usChannelID);

  if (it != m_ChnlSeqNoRange.end())
  {
    if (it->second.BeginSeqNum == uiBeginUnadjSeqNum && it->second.EndSeqNum == uiEndUnadjSeqNum) return false;
    else return true;
  }
  else
    return true;
}


void RTSClient::PurgeAllRTSRequests()
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);
  m_RequestQueue.clear();
  m_ChnlSeqNoRange.clear();
  return;
}


void RTSClient::Run()
{
  for (;;)
  {

    try
    {
      //--------------------------------------------------
      // Report Health
      //--------------------------------------------------
      m_ThreadHealthMon->ReportHealthy(ThreadHealthMonitor::RTSCLIENT, 0);

      //--------------------------------------------------
      // System shutdown
      if (m_ShrObj->ThreadShouldExit()) break;
      //--------------------------------------------------

      //--------------------------------------------------
      // Wait for the next request
      //--------------------------------------------------
      if (m_RequestQueue.empty())
      {
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff = now - m_LastPrintLogTime;
        unsigned long ulTD = diff.total_milliseconds();
        if (ulTD > MILLISEC_RTSLOG)
        {
          m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): %u request(s) is/are are in the queue.", m_RequestQueue.size());
          m_LastPrintLogTime = now;
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(m_RTSClientSleepMillisec));

        continue;
      }

      //--------------------------------------------------
      // Get next RTS request in queue
      //--------------------------------------------------
      boost::unique_lock<boost::shared_mutex> lock(m_Mutex);
      m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): %u request(s) is/are are in the queue.", m_RequestQueue.size());
      unsigned short usChannelID = m_RequestQueue[0];
      m_RequestQueue.pop_front();
      uint32_t uiBeginUnadjSeqNum = m_ChnlSeqNoRange[usChannelID].BeginSeqNum;
      uint32_t uiEndUnadjSeqNum   = m_ChnlSeqNoRange[usChannelID].EndSeqNum;
      m_ChnlSeqNoRange.erase(usChannelID);
      m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): Picked up request item: ChannelID:%u. Seq No from %u to %u.", usChannelID,uiBeginUnadjSeqNum,uiEndUnadjSeqNum);
      lock.unlock();

      //--------------------------------------------------
      // If already exceeded max num of requests per day, directly activate refresh on that channel because it's just a waste of time trying RTS again.
      //--------------------------------------------------
      if (m_bExceedMaxReqPerDay)
      {
        m_ShrObj->ActivateRefresh(usChannelID);
        m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): ChannelID:%u. Activated refresh directly because RTS server complained that we have already exceeded the max # of requests per day.", usChannelID);
        continue;
      }

      //--------------------------------------------------
      // Get Msg Circular Buffer
      //--------------------------------------------------
      m_MsgCirBuf =  m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, usChannelID);
      if ((*(m_SysCfg->GetRTSClientJson()))[usChannelID] == 1) m_bOutputJson = true;
      uint32_t uiLatestRTOffset =  m_ShrObj->CheckRTChnlLatestSeqNoOffset(usChannelID);
      {
        //--------------------------------------------------
        // To avoid wasting precious retransmission quota, check if it's really necessary
        // Because it's possible that while we were still receiving and process data in the previous RTS resquest,
        // DataComletenessInspector checked the msg circular buffer again and saw there were still missing seq no / messages, and notified RTSClient
        //--------------------------------------------------
        bool bR1, bR2, bDirtyFlag1, bDirtyFlag2;
        uint64_t ulTStamp;

        bR1 = m_MsgCirBuf->CheckDirtyFlagTStamp(uiBeginUnadjSeqNum+uiLatestRTOffset,bDirtyFlag1,ulTStamp);
        bR2 = m_MsgCirBuf->CheckDirtyFlagTStamp(uiEndUnadjSeqNum+uiLatestRTOffset,bDirtyFlag2,ulTStamp);

        if (!bR1 || bDirtyFlag1 || !bR2 || bDirtyFlag2)
        {
          m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): ChannelID:%u. Some of the seq no have already been recovered. Skipping RTS request to save quota.", usChannelID);
          continue;
        }
        if (m_ShrObj->CheckRefreshActivatnStatus(usChannelID))
        {
          m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): ChannelID:%u. Refresh is already activated on this channel. Skipping RTS request to save quota.", usChannelID);
          continue;
        }
      }




      //--------------------------------------------------
      // Establish connection and Logon if needed
      //--------------------------------------------------
      if (!m_socket->is_open())
      {

        unsigned short usNoOfRounds = 0;

        //--------------------------------------------------
        // If Timeout for too many times (3 times), switch to the next RTS server
        //--------------------------------------------------
        if (m_RTS_NoOfTimeout >= 3)
        {
          char sPort[8];
          sprintf(sPort,"%u",(*m_pRTS_ServerPort)[m_RTS_SvrIndex]);
          m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): Already timeout 3 times connecting to %s : %s. Switch to the next server.", (*m_pRTS_ServerIP)[m_RTS_SvrIndex].c_str(),sPort);

          m_RTS_NoOfTimeout = 0;
          m_RTS_SvrIndex = (m_RTS_SvrIndex+1)%(m_pRTS_ServerIP->size());
        }

        for (;;)
        {
          //--------------------------------------------------
          // Establish TCP connection
          //--------------------------------------------------

          char sPort[8];
          sprintf(sPort,"%u",(*m_pRTS_ServerPort)[m_RTS_SvrIndex]);

          m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): Connecting to %s : %s.", (*m_pRTS_ServerIP)[m_RTS_SvrIndex].c_str(),sPort);

          m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): RTS server has been successfully connected %u time(s).", m_ShrObj->GetCountNumOfRTSConn());

          tcp::resolver::query query((*m_pRTS_ServerIP)[m_RTS_SvrIndex].c_str(),sPort); //TODO
          tcp::resolver::iterator endpoint_iterator = m_resolver->resolve(query);
          tcp::resolver::iterator end;

          boost::system::error_code error = boost::asio::error::host_not_found;

          m_socket->connect(*endpoint_iterator++, error);
          while (error && endpoint_iterator != end)
          {
            m_socket->close();
            m_socket->connect(*endpoint_iterator++, error);
          }

          if (error)
          {
            boost::system::system_error syserr(error);
            m_Logger->Write(Logger::ERROR, "RTSClient: Run(): Trying to request missing seq no from %u to %u in channel %u" , uiBeginUnadjSeqNum, uiEndUnadjSeqNum, usChannelID);
            m_Logger->Write(Logger::ERROR, "RTSClient: Run(): Error occurred while connecting to %s : %s : %s",(*m_pRTS_ServerIP)[m_RTS_SvrIndex].c_str(),sPort,syserr.what());

            //--------------------------------------------------
            // Switch to the next RTS server, or just trigger refresh if RTS servers could not be reached
            //--------------------------------------------------
            m_RTS_SvrIndex = (m_RTS_SvrIndex+1)%(m_pRTS_ServerIP->size());
            if (m_RTS_SvrIndex == 0) ++usNoOfRounds;
            if (usNoOfRounds == 3) //already tried 3 rounds, exhausted patience, activate refresh
            {
              m_socket->close();
              m_ShrObj->ActivateRefresh(usChannelID);
              m_Logger->Write(Logger::NOTICE, "RTSClient: Tried connecting to each RTS servers 2 times but failed. Will stop trying for now. Refresh mode is activated on channel %u.", usChannelID);
              continue;
            }

            boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
          }
          else
          {
            //--------------------------------------------------
            // Connection established
            //--------------------------------------------------
            m_Logger->Write(Logger::NOTICE, "RTSClient: Run(): Connected to %s : %s.",(*m_pRTS_ServerIP)[m_RTS_SvrIndex].c_str(),sPort);

            break;
          }
        } //for(;;)


        //--------------------------------------------------
        // Logon
        //--------------------------------------------------
        {
          m_Logger->Write(Logger::NOTICE, "RTSClient: Attempting Logon...");
          //--------------------------------------------------
          // To prevent older versions of Valgrind from complaining
          memset(m_Buffer,'\0',TCPReadByte);
          //--------------------------------------------------

          OMD_Retransmission_Header* pOmdRTSHdr = (OMD_Retransmission_Header*)(&m_Buffer[0]);
          OMD_Logon* pOmdLogon = (OMD_Logon*)(&m_Buffer[sizeof(OMD_Retransmission_Header)]);

          pOmdRTSHdr->PktSize = sizeof(OMD_Retransmission_Header) + sizeof(OMD_Logon);
          WRITE_UINT8 (&(pOmdRTSHdr->MsgCount), 1); // Must be 1 according to spec
          pOmdRTSHdr->SeqNum = 0;
          pOmdRTSHdr->SendTime = 0;
          //other fields in pOmdRTSHdr are not used, but just set them to 0

          pOmdLogon->MsgSize = sizeof(OMD_Logon);
          pOmdLogon->MsgType = OMD_LOGON;
          memset(pOmdLogon->Username,'\0',12);
          strncpy(pOmdLogon->Username, m_RTS_Username.c_str(), m_RTS_Username.length());

          // m_socket->write_some(boost::asio::buffer(m_Buffer, sizeof(OMD_Retransmission_Header) + sizeof(OMD_Logon)));
          boost::system::error_code error;
          size_t write_len = sizeof(OMD_Retransmission_Header) + sizeof(OMD_Logon);
          if (!write_some_with_timeout(m_socket, boost::asio::buffer(m_Buffer,write_len), write_len, error))
          {
            m_Logger->Write(Logger::WARNING, "RTSClient: Logon request: Timeout waiting for RTS server / Write error.");
            ++m_RTS_NoOfTimeout;
            boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
            continue;
          }
          m_Logger->Write(Logger::NOTICE, "RTSClient: Logon Request has just been sent.");
        }

        //--------------------------------------------------
        // Get Logon response
        //--------------------------------------------------
        {
          m_BufBeginIdx = 0;
          m_BufEndIdx = 0;
          size_t read_len = 0;
          uint16_t uiPktSize = 0;
          boost::system::error_code error;

          memset(m_Buffer,'\0',TCPReadByte);
          for (;;)
          {
            // read_len = m_socket->read_some(boost::asio::buffer(&m_Buffer[m_BufEndIdx],MAX_OMD_PACKET_SIZE-m_BufEndIdx), error);
            if (!read_some_with_timeout(m_socket, boost::asio::buffer(&m_Buffer[m_BufEndIdx],MAX_OMD_PACKET_SIZE-m_BufEndIdx), error, read_len))
            {
              m_Logger->Write(Logger::WARNING, "RTSClient: Timeout waiting for RTS server / Read error.");
              ++m_RTS_NoOfTimeout;
              boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
              break;
            }
            m_BufEndIdx += read_len;

            if (m_BufEndIdx > 2) // we've got the Pkt Size!
              uiPktSize = READ_UINT16(&m_Buffer[0]);

            if (uiPktSize == 0 || uiPktSize > MAX_OMD_PACKET_SIZE)
            {
              m_Logger->Write(Logger::WARNING, "RTSClient: Expected OMD Logon Response but received unknown message instead. Reconnect and try again.");
              m_socket->close();
              break;
            }

            //--------------------------------------------------
            // Finished getting logon response
            //--------------------------------------------------
            if (uiPktSize > 0 && m_BufEndIdx >= uiPktSize)
              break;

            if (error == boost::asio::error::eof)
              break; // Connection closed cleanly by peer.
            else if (error)
              throw boost::system::system_error(error); // Some other error.
          }
        }

        //--------------------------------------------------
        // Safety check
        //--------------------------------------------------
        if (!m_socket->is_open())
        {
          m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
          continue;
        }

        //--------------------------------------------------
        // Check Logon response
        //--------------------------------------------------
        {
          OMD_Logon_Response * pOmdLogonResp = (OMD_Logon_Response*)(&m_Buffer[sizeof(OMD_Packet_Header)]);

          if (pOmdLogonResp->MsgType == OMD_LOGON_RESPONSE)
          {
            m_Logger->Write(Logger::NOTICE, "RTSClient: Logon Response has just been received.");

            switch(pOmdLogonResp->SessionStatus)
            {
              //--------------------------------------------------
              // The only Normal path
              //--------------------------------------------------
              case SessionActive:
                {
                  m_Logger->Write(Logger::NOTICE, "RTSClient: Logon Response: Session Active.");

                  break;
                }
              case InvalidUserOrIP:
                {
                  m_Logger->Write(Logger::ERROR, "RTSClient: Logon Response: Invalid Username or IP. Please check Config.ini.");
                  m_socket->close();
                  boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
                  continue;
                  break;
                }
              case UserAlrConnected:
                {
                  m_Logger->Write(Logger::ERROR, "RTSClient: Logon Response: User already connected. Please check if another OMD instance is running.");
                  m_socket->close();
                  boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
                  continue;
                  break;
                }
                //--------------------------------------------------
                // That's strange, but somehow the RTS simulator has skipped Logon Response
                //--------------------------------------------------
              case OMD_RETRANSMISSION_RESPONSE:
                {
                  m_Logger->Write(Logger::ERROR, "RTSClient: Logon Response has been skipped by the RTS. We got a Retransmission Response right a way?!");
                  break;
                }
              default:
                {
                  m_Logger->Write(Logger::WARNING, "RTSClient: Logon Response: Unknown response SessionStatus = %u.", pOmdLogonResp->SessionStatus);
                  // m_socket->close();
                  // boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
                  continue;
                  break;
                }
            }
          }
          else
          {
            m_Logger->Write(Logger::INFO, "RTSClient: ChannelID:%u. SessionStatus:%u.", usChannelID, pOmdLogonResp->SessionStatus);
            m_Logger->Write(Logger::WARNING, "RTSClient: Logon Response was expected but another MsgType (%u) was received. We must restart from the beginning to avoid messing up the workflow.", pOmdLogonResp->MsgType);
            m_socket->close();
            boost::this_thread::sleep(boost::posix_time::seconds(20));
            continue;
          }
        }

        //--------------------------------------------------
        // Got positive logon response
        //--------------------------------------------------
        m_ShrObj->AddCountNumOfRTSConn();

      } // if (!m_socket->is_open())

      //--------------------------------------------------
      // Safety check
      //--------------------------------------------------
      if (!m_socket->is_open())
      {
        m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
        continue;
      }








      //--------------------------------------------------
      // Send Retransmission request
      //--------------------------------------------------
      {
        OMD_Retransmission_Header* pOmdRTSHdr = (OMD_Retransmission_Header*)(&m_Buffer[0]);
        OMD_Retransmission_Request* pOmdReReq = (OMD_Retransmission_Request*)(&m_Buffer[sizeof(OMD_Retransmission_Header)]);

        pOmdRTSHdr->PktSize = sizeof(OMD_Retransmission_Header) + sizeof(OMD_Retransmission_Request);
        WRITE_UINT8 (&(pOmdRTSHdr->MsgCount), 1); // Must be 1 according to spec
        //other fields in pOmdRTSHdr are not used

        pOmdReReq->MsgSize      =  sizeof(OMD_Retransmission_Request);
        pOmdReReq->MsgType      =  OMD_RETRANSMISSION_REQUEST;

        pOmdReReq->ChannelID    =  usChannelID;
        pOmdReReq->BeginSeqNum  =  uiBeginUnadjSeqNum;
        pOmdReReq->EndSeqNum    =  uiEndUnadjSeqNum;

        m_Logger->Write(Logger::NOTICE, "RTSClient: Sending retransmission request: Channel ID: %u, Begin Seq : %u, End Seq : %u.", usChannelID, uiBeginUnadjSeqNum, uiEndUnadjSeqNum);
        // m_socket->write_some(boost::asio::buffer(m_Buffer, sizeof(OMD_Retransmission_Header) + sizeof(OMD_Retransmission_Request)));
        boost::system::error_code error;
        size_t write_len = sizeof(OMD_Retransmission_Header) + sizeof(OMD_Retransmission_Request);
        if (!write_some_with_timeout(m_socket, boost::asio::buffer(m_Buffer,write_len), write_len, error))
        {
          m_Logger->Write(Logger::WARNING, "RTSClient: Retransmission Request: Timeout waiting for RTS server / Write error.");
          ++m_RTS_NoOfTimeout;
          boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
          continue;
        }


      }

      //--------------------------------------------------
      // Safety check
      //--------------------------------------------------
      if (!m_socket->is_open())
      {
        m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
        continue;
      }

      //--------------------------------------------------
      // Get Retransmission response
      //--------------------------------------------------

      bool bFinishedGettingRetransmissionResp = false;
      do
      {
        {
          //--------------------------------------------------
          // Get for first time
          //--------------------------------------------------
          m_BufBeginIdx = 0;
          m_BufEndIdx = 0;
          size_t read_len = 0;
          uint16_t uiPktSize = 0;
          boost::system::error_code error;
          memset(m_Buffer,'\0',TCPReadByte);
          for (;;)
          {
            // read_len = m_socket->read_some(boost::asio::buffer(&m_Buffer[m_BufEndIdx],MAX_OMD_PACKET_SIZE-m_BufEndIdx), error);
            if (!read_some_with_timeout(m_socket, boost::asio::buffer(&m_Buffer[m_BufEndIdx],MAX_OMD_PACKET_SIZE-m_BufEndIdx), error, read_len))
            {
              m_Logger->Write(Logger::WARNING, "RTSClient: Timeout waiting for RTS server / Read error.");
              ++m_RTS_NoOfTimeout;
              boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
              break;
            }
            m_BufEndIdx += read_len;

            if (m_BufEndIdx > 2) // we've got the Pkt Size!
              uiPktSize = READ_UINT16(&m_Buffer[0]);

            if (uiPktSize == 0 || uiPktSize > MAX_OMD_PACKET_SIZE)
            {
              m_Logger->Write(Logger::WARNING, "RTSClient: Expected OMD Retransmission Response but received unknown message instead. Reconnect and try again.");
              m_socket->close();
              break;
            }

            // finished getting retransmission response
            if (uiPktSize > 0 && m_BufEndIdx >= uiPktSize)
            {
              //--------------------------------------------------
              // Skip the Retransmission response message and go to the others
              //--------------------------------------------------
              m_BufBeginIdx = uiPktSize;

              //--------------------------------------------------
              // The rest are those retransmitted packets
              //--------------------------------------------------
              m_Logger->Write(Logger::DEBUG, "RTSClient: Retransmitted packets: uiPktSize : %u, m_BufBeginIdx : %u, m_BufEndIdx : %u.", uiPktSize, m_BufBeginIdx, m_BufEndIdx);

              break;
            }

            if (error == boost::asio::error::eof)
              break; // Connection closed cleanly by peer.
            else if (error)
              throw boost::system::system_error(error); // Some other error.
          }
        }

        //--------------------------------------------------
        // Safety check
        //--------------------------------------------------
        if (!m_socket->is_open())
        {
          m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
          break;
        }

        {
          //--------------------------------------------------
          // Special lenient treatment as a reaction to OMD testing line behaviour:
          // MsgType == 0: It's seen before in the testing line: RTS server returned the wrong MsgType==0 in the retransmission response but in fact it means ok.
          // This is seen in the OMD testing line. Although strictly speaking, the RTS server is incorrect, but we know from the testing line behaviour that they meant they have accepted our request. So proceed.
          //--------------------------------------------------
          OMD_Retransmission_Response * pOmdRetranResp = (OMD_Retransmission_Response*)(&m_Buffer[sizeof(OMD_Packet_Header)]);

          //--------------------------------------------------
          // Special lenient treatment:
          // MsgType == 0: It's seen before in the testing line: RTS server returned the wrong MsgType==0 in the retransmission response but in fact it means ok.
          // This is seen in the OMD testing line. Although strictly speaking, the RTS server is incorrect, but we know from the testing line behaviour that they meant they have accepted our request. So proceed.
          //--------------------------------------------------
          if (pOmdRetranResp->MsgType == 0)
          {
            m_Logger->Write(Logger::WARNING, "RTSClient: Retransmission Response: MsgType==0 received for Retransmission Response. Lenient treatment.");
            // Deam correct
            if (pOmdRetranResp->ChannelID == usChannelID)
            {
              pOmdRetranResp->MsgType = OMD_RETRANSMISSION_RESPONSE;
              bFinishedGettingRetransmissionResp = true;
            }
            else
            {
              //--------------------------------------------------
              // Read from socket again
              //--------------------------------------------------
              bFinishedGettingRetransmissionResp = false;

            }
          }
          else
          {
            bFinishedGettingRetransmissionResp = true;
          }
        }
      } while (!bFinishedGettingRetransmissionResp);



      //--------------------------------------------------
      // Safety check
      //--------------------------------------------------
      if (!m_socket->is_open())
      {
        m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
        continue;
      }



      //--------------------------------------------------
      // Check Retransmission response
      //--------------------------------------------------
      {
        OMD_Retransmission_Response * pOmdRetranResp = (OMD_Retransmission_Response*)(&m_Buffer[sizeof(OMD_Packet_Header)]);

        if (pOmdRetranResp->MsgType == OMD_RETRANSMISSION_RESPONSE)
        {
          m_Logger->Write(Logger::NOTICE, "RTSClient: Retransmission Response has just been received.");

          switch(pOmdRetranResp->RetransStatus)
          {
            //--------------------------------------------------
            // The only Normal path
            //--------------------------------------------------
            case RequestAccepted:
              m_Logger->Write(Logger::NOTICE, "RTSClient: Retransmission Response: Request Accepted.");

              m_Logger->Write(Logger::NOTICE, "RTSClient: Retransmission Response: Channel ID: %u", pOmdRetranResp->ChannelID);
              m_Logger->Write(Logger::NOTICE, "RTSClient: Retransmission Response: Begin Seq No (unadj): %u", pOmdRetranResp->BeginSeqNum);
              m_Logger->Write(Logger::NOTICE, "RTSClient: Retransmission Response: End Seq No (unadj): %u"  , pOmdRetranResp->EndSeqNum);

              //--------------------------------------------------
              // In case it's less than what we requested
              //--------------------------------------------------
              if (pOmdRetranResp->BeginSeqNum != 0 && uiBeginUnadjSeqNum < pOmdRetranResp->BeginSeqNum && pOmdRetranResp->BeginSeqNum < 90000000) uiBeginUnadjSeqNum = pOmdRetranResp->BeginSeqNum;
              if (pOmdRetranResp->EndSeqNum != 0 && uiEndUnadjSeqNum > pOmdRetranResp->EndSeqNum) uiEndUnadjSeqNum = pOmdRetranResp->EndSeqNum;

              //--------------------------------------------------
              // Check if the channel ID in RTS's response matches our request.
              // This is not supposed to happen, but it's seen on testing line...
              //--------------------------------------------------
              if (pOmdRetranResp->ChannelID != usChannelID)
              {
                m_Logger->Write(Logger::WARNING,
                    "RTSClient: Retransmission Response: Strangely the Channel ID in RTS response (%u) is different from what we requested (%u). Anyway, continue to process, don't waste the data.",
                    pOmdRetranResp->ChannelID, usChannelID);
                usChannelID = pOmdRetranResp->ChannelID;
                m_MsgCirBuf =  m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, usChannelID);
                uiBeginUnadjSeqNum = pOmdRetranResp->BeginSeqNum;
                uiEndUnadjSeqNum = pOmdRetranResp->EndSeqNum;
              }

              break;
            case UnknownChnlID:
              m_Logger->Write(Logger::WARNING, "RTSClient: Retransmission Response: Unknown Channel ID.");
              // m_socket->close();
              m_ShrObj->ActivateRefresh(usChannelID);
              m_Logger->Write(Logger::NOTICE, "RTSClient: Refresh mode activated on channel %u.", usChannelID);
              // boost::this_thread::sleep(boost::posix_time::milliseconds(m_RTSClientSleepMillisec));
              continue;
              break;
            case MsgNotAvg:
              m_Logger->Write(Logger::WARNING, "RTSClient: Retransmission Response: Message Not Available.");
              // m_socket->close();
              m_ShrObj->ActivateRefresh(usChannelID);
              m_Logger->Write(Logger::NOTICE, "RTSClient: Refresh mode activated on channel %u.", usChannelID);
              // boost::this_thread::sleep(boost::posix_time::milliseconds(m_RTSClientSleepMillisec));
              continue;
              break;
            case ExceedMaxSeqRng:
              m_Logger->Write(Logger::ERROR, "RTSClient: Retransmission Response: Exceed Max Seq Range. This shouldn't happen. Check Config.ini.");
              // m_socket->close();
              m_ShrObj->ActivateRefresh(usChannelID);
              m_Logger->Write(Logger::NOTICE, "RTSClient: Refresh mode activated on channel %u.", usChannelID);
              // boost::this_thread::sleep(boost::posix_time::milliseconds(m_RTSClientSleepMillisec));
              continue;
              break;
            case ExceedMaxReqPerDay:
              m_Logger->Write(Logger::WARNING, "RTSClient: Retransmission Response: Exceed Max Req Per Day.");
              m_socket->close();
              m_ShrObj->ActivateRefresh(usChannelID);
              m_Logger->Write(Logger::NOTICE, "RTSClient: Refresh mode activated on channel %u.", usChannelID);
              m_bExceedMaxReqPerDay = true;
              boost::this_thread::sleep(boost::posix_time::milliseconds(m_RTSClientSleepMillisec));
              continue;
              break;
            default:
              m_Logger->Write(Logger::WARNING, "RTSClient: Retransmission Response: Unknown response. RetransStatus = &u.", pOmdRetranResp->RetransStatus);
              // m_socket->close();
              // boost::this_thread::sleep(boost::posix_time::milliseconds(m_RTSClientSleepMillisec));
              continue;
              break;
          }
        }
        else
        {
          m_Logger->Write(Logger::WARNING, "RTSClient: Retransmission Response was expected but another message was received. MsgType = %u", pOmdRetranResp->MsgType);
          //TODO: what to do?
          // m_socket->close();
          // boost::this_thread::sleep(boost::posix_time::milliseconds(m_RTSClientSleepMillisec));
          continue;
        }

      }

      if (!m_socket->is_open())
      {
        m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
        continue;
      }


















      //--------------------------------------------------
      // Get retransmission content
      // We'll just directly put these messages to the message circle buffer. To fill the gaps detected by DataComletenessInspector
      //--------------------------------------------------
      bool bFinishedGettingAll = false;

      for (;;)
      {
        if (bFinishedGettingAll) break;

        size_t read_len = 0;
        uint16_t uiPktSize = 0;
        boost::system::error_code error;

        if (!m_socket->is_open())
        {
          m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
          break;
        }

        // memset(m_Buffer,'\0',TCPReadByte);
        while (m_BufBeginIdx < m_BufEndIdx)
        {
          if (!m_socket->is_open())
          {
            m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
            break;
          }

          //--------------------------------------------------
          // Got less than one packet
          // Finish getting it before processing
          //--------------------------------------------------
          while (m_BufEndIdx - m_BufBeginIdx < 2)
          {
            if (!m_socket->is_open())
            {
              m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
              break;
            }

            // read_len = m_socket->read_some(boost::asio::buffer(&m_Buffer[m_BufEndIdx],2), error);
            if (!read_some_with_timeout(m_socket, boost::asio::buffer(&m_Buffer[m_BufEndIdx],2), error, read_len))
            {
              m_Logger->Write(Logger::WARNING, "RTSClient: Timeout waiting for RTS server / Read error.");
              ++m_RTS_NoOfTimeout;
              boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
              break;
            }
            m_BufEndIdx += read_len;
          }

          if (!m_socket->is_open())
          {
            m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
            break;
          }

          if (m_BufEndIdx - m_BufBeginIdx >= 2) // we've got the Pkt Size!
            uiPktSize = READ_UINT16(&m_Buffer[m_BufBeginIdx]);


          while (m_BufEndIdx - m_BufBeginIdx < uiPktSize)
          {
            if (!m_socket->is_open())
            {
              m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
              break;
            }

            // read_len = m_socket->read_some(boost::asio::buffer(&m_Buffer[m_BufEndIdx],uiPktSize-m_BufEndIdx), error);
            if (!read_some_with_timeout(m_socket, boost::asio::buffer(&m_Buffer[m_BufEndIdx],uiPktSize-m_BufEndIdx), error, read_len))
            {
              m_Logger->Write(Logger::WARNING, "RTSClient: Timeout waiting for RTS server / Read error.");
              ++m_RTS_NoOfTimeout;
              boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
              break;
            }
            m_BufEndIdx += read_len;
          }

          if (!m_socket->is_open())
          {
            m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
            break;
          }

          //--------------------------------------------------
          // Now that we have at least 1 complete packet
          // Cut out the messages from the packet and insert into circular buffer
          //--------------------------------------------------
          OMD_Packet_Header* oph = (OMD_Packet_Header*) &(m_Buffer[m_BufBeginIdx]);

          unsigned short  usPktSize   =  (unsigned short) READ_UINT16(oph);
          unsigned short  usMsgCnt    =  (unsigned short) READ_UINT8 (&(oph->MsgCount));
          unsigned int    uiUnadjSeqNo=  (unsigned int)   READ_UINT32(&(oph->SeqNum));
          unsigned long   ulSendTime  =  (unsigned long)  READ_UINT64(&(oph->SendTime));

          if (usMsgCnt == 0)
          {
            //--------------------------------------------------
            // Got Heartbeat, must ack RTS `server now
            //--------------------------------------------------
            // m_socket->write_some(boost::asio::buffer(&m_Buffer[m_BufBeginIdx],usPktSize), error);
            boost::system::error_code error;
            size_t write_len = usPktSize;
            if (!write_some_with_timeout(m_socket, boost::asio::buffer(&m_Buffer[m_BufBeginIdx],write_len), write_len, error))
            {
              m_Logger->Write(Logger::WARNING, "RTSClient: Ack RTS heartbeat: Timeout waiting for RTS server / Write error.");
              ++m_RTS_NoOfTimeout;
              boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
              break;
            }
            m_BufBeginIdx += usPktSize;
          }
          else
          {
            m_BufBeginIdx += sizeof(OMD_Packet_Header);

            //--------------------------------------------------
            // Regular messages
            //--------------------------------------------------
            for (unsigned short i = 0; i < usMsgCnt; ++i)
            {
              BYTE *pbMsg = (BYTE*)&m_Buffer[m_BufBeginIdx];

              m_Logger->Write(Logger::DEBUG, "RTSClient: Received Message: Seq No (unadj) : %u, Send Time : %s, Msg Size : %u, Msg Type : %u",
                  uiUnadjSeqNo, SDateTime::fromUnixTimeToString(ulSendTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT).c_str(), READ_UINT16(pbMsg), READ_UINT16(pbMsg+2) );

              if (m_bOutputJson)
                m_DataProcFunc->OutputJsonToLog(__FILE__, usChannelID, m_Logger, pbMsg, m_JsonBuffer);

              //--------------------------------------------------
              // Process message for omd_mdi
              //--------------------------------------------------
              m_DataProcFunc->ProcessMessageForMDI(m_ShrObj, pbMsg, READ_UINT16(pbMsg+2));
              //--------------------------------------------------

              m_MsgCirBuf->PushMsg(pbMsg,uiUnadjSeqNo+uiLatestRTOffset,ulSendTime); // Not the current local server time, but the time in the packet header
              uiUnadjSeqNo++;
              m_BufBeginIdx += READ_UINT16(pbMsg);

              //if (m_BufBeginIdx + sizeof(OMD_Packet_Header) < m_BufEndIdx)
              //  m_BufBeginIdx += sizeof(OMD_Packet_Header);

            }

            //--------------------------------------------------
            // Got all messages
            //--------------------------------------------------
            if (uiUnadjSeqNo > uiEndUnadjSeqNum) // greater than but not equal to
            {
              // m_socket->close();  // reuse connection!
              bFinishedGettingAll = true;
              break;
            }
          } // if

        } //while

        if (!m_socket->is_open())
        {
          m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
          break;
        }
        if (bFinishedGettingAll) break;

        //--------------------------------------------------
        // Fresh start
        //--------------------------------------------------
        if (m_BufBeginIdx == m_BufEndIdx)
        {
          if (!m_socket->is_open())
          {
            m_Logger->Write(Logger::NOTICE, "RTSClient: Socket closed. (Line %u).", __LINE__);
            break;
          }

          memset(m_Buffer,'\0',TCPReadByte);
          // read_len      = m_socket->read_some(boost::asio::buffer(&m_Buffer[0],2), error);
          if (!read_some_with_timeout(m_socket, boost::asio::buffer(&m_Buffer[0],2), error, read_len))
          {
            m_Logger->Write(Logger::WARNING, "RTSClient: Timeout waiting for RTS server / Read error.");
            ++m_RTS_NoOfTimeout;
            boost::this_thread::sleep(boost::posix_time::seconds(m_RTS_ReconnectionWaitSec));
            break;
          }
          m_BufBeginIdx = 0;
          m_BufEndIdx   = read_len;

          //--------------------------------------------------
          // There is still more to process
          //--------------------------------------------------
        }

        if (error == boost::asio::error::eof)
          break; // Connection closed cleanly by peer.
        else if (error)
          throw boost::system::system_error(error); // Some other error.

      } // for(;;)

    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }
  } // for(;;)

  return;
}


//--------------------------------------------------
void RTSClient::handle_timerexpiry(const boost::system::error_code& ec)
{
  if (m_BytesReadFrAsyncRead > 0 || m_BytesWrittenFrAsyncWrite > 0)
  {
    m_TimerExpired = false;
    m_ReadWriteError = false;
  }
  else
  {
    m_TimerExpired = true;
    m_ReadWriteError = true;
    m_socket->close();
    m_Logger->Write(Logger::WARNING, "RTSClient: Timeout waiting for RTS server. Closed socket.");
  }
  return;
}

void RTSClient::handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
  if (ec || bytes_transferred == 0)
  {
    m_ReadWriteError = true;
    m_Logger->Write(Logger::WARNING, "RTSClient: Socket error on async_read().");
    m_socket->close();
  }
  else
  {
    m_BytesReadFrAsyncRead = bytes_transferred;
    m_ReadWriteError = false;
  }
  m_deadlinetimer->cancel();
  return;
}

void RTSClient::handle_write(const boost::system::error_code& ec, size_t bytes_transferred)
{
  if (ec)
  {
    m_ReadWriteError = true;
    m_Logger->Write(Logger::WARNING, "RTSClient: Socket error on async_write().");
    m_socket->close();
  }
  else
  {
    m_BytesWrittenFrAsyncWrite = bytes_transferred;
    m_ReadWriteError = false;
  }
  m_deadlinetimer->cancel();
  return;
}


//Ref: http://www.ridgesolutions.ie/index.php/2012/12/13/boost-c-read-from-serial-port-with-timeout-example/
// Returns true if successful - false if a timeout occurs
  template <typename MutableBufferSequence>
bool RTSClient::read_some_with_timeout(boost::shared_ptr<boost::asio::ip::tcp::socket> sock, const MutableBufferSequence& buffers, boost::system::error_code& ec, std::size_t& bytes_transferred)
{
  //--------------------------------------------------
  // Re-Init
  //--------------------------------------------------
  m_deadlinetimer->cancel();
  sock->get_io_service().reset();
  m_BytesReadFrAsyncRead = 0;
  m_BytesWrittenFrAsyncWrite = 0;
  m_TimerExpired = false;
  m_ReadWriteError = false;
  //--------------------------------------------------

  sock->async_read_some(buffers, boost::bind(&RTSClient::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  m_deadlinetimer->expires_from_now(boost::posix_time::seconds(10));
  m_deadlinetimer->async_wait(boost::bind(&RTSClient::handle_timerexpiry, this, boost::asio::placeholders::error));

  sock->get_io_service().run();

  bytes_transferred = m_BytesReadFrAsyncRead;

  if (m_TimerExpired) return false;
  else if (m_ReadWriteError) return false;
  else return true;
}

  template <typename MutableBufferSequence>
bool RTSClient::write_some_with_timeout(boost::shared_ptr<boost::asio::ip::tcp::socket> sock, const MutableBufferSequence& buffers, const size_t write_len, boost::system::error_code& ec)
{
  //--------------------------------------------------
  // Re-Init
  //--------------------------------------------------
  m_deadlinetimer->cancel();
  sock->get_io_service().reset();
  m_BytesReadFrAsyncRead = 0;
  m_BytesWrittenFrAsyncWrite = 0;
  m_TimerExpired = false;
  m_ReadWriteError = false;
  //--------------------------------------------------

  boost::asio::async_write(*m_socket, boost::asio::buffer(buffers, write_len), boost::asio::transfer_at_least(write_len), boost::bind(&RTSClient::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)); 
  m_deadlinetimer->expires_from_now(boost::posix_time::seconds(10));
  m_deadlinetimer->async_wait(boost::bind(&RTSClient::handle_timerexpiry, this, boost::asio::placeholders::error));

  sock->get_io_service().run();

  if (m_TimerExpired) return false;
  else if (m_ReadWriteError) return false;
  else if (m_BytesWrittenFrAsyncWrite < write_len) return false;
  else return true;
}

