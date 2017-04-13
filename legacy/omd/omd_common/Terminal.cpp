/*
 * Terminal.cpp
 *
 *  Created on: May 16, 2014
 *      Author: sunny
 */

#include "Terminal.h"


  TerminalSession::TerminalSession(boost::asio::io_service& io_service, ChannelController *cc)
: m_socket(io_service),
  m_ios(io_service),
  m_pChnlCtrlr(cc)
{
  m_Logger = Logger::Instance();
  m_SysCfg = SystemConfig::Instance();
  m_ShrObj = SharedObjects::Instance();
  memset(m_LastCmd,'\0',SIZE);
}

void TerminalSession::start()
{
  try
  {
    strcpy(m_Buffer,CMD_PS1);

    boost::asio::async_write(m_socket,
        boost::asio::buffer(m_Buffer, strlen(m_Buffer)),
        boost::bind(&TerminalSession::handle_write, this,
          boost::asio::placeholders::error));

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return;
}



void TerminalSession::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{

  if (!error)
  {

    m_Logger->Write(Logger::NOTICE, "TerminalSession: Received : %s", m_Buffer);

    //--------------------------------------------------
    // Run last commnad if needed
    //--------------------------------------------------
    if (strncmp(m_Buffer,CMD_L,strlen(CMD_L))==0 && strncmp(m_Buffer,CMD_LASTSEQNO,strlen(CMD_LASTSEQNO))!=0 && strncmp(m_Buffer,CMD_LOGLEVEL,strlen(CMD_LOGLEVEL))!=0)
    {
      if (strlen(m_LastCmd) > 0)
        strncpy(m_Buffer,m_LastCmd,SIZE);
      else
        strncpy(m_Buffer,CMD_H,SIZE);
    }
    else
    {
      strncpy(m_LastCmd,m_Buffer,SIZE);
    }

    //--------------------------------------------------
    // Print help
    //--------------------------------------------------
    if (strncmp(m_Buffer,CMD_H,strlen(CMD_H))==0 || strncmp(m_Buffer,CMD_HELP,strlen(CMD_HELP))==0)
    {

      sprintf(m_Buffer,"0       Emergency       System is unusable               "); output_tml_log();
      sprintf(m_Buffer,"1       Alert           Action must be taken immediately "); output_tml_log();
      sprintf(m_Buffer,"2       Critical        Critical conditions              "); output_tml_log();
      sprintf(m_Buffer,"3       Error           Error conditions                 "); output_tml_log();
      sprintf(m_Buffer,"4       Warning         Warning conditions               "); output_tml_log();
      sprintf(m_Buffer,"5       Notice          Normal but significant condition "); output_tml_log();
      sprintf(m_Buffer,"6       Informational   Informational messages           "); output_tml_log();
      sprintf(m_Buffer,"7       Debug           Debug-level messages             "); output_tml_log();

      sprintf(m_Buffer,"Supported commands:\nl (repeat last command)\nh (help)\nq (quit)\nlog[0-7]\nrs (reset)\nrfa (refresh activate all)\nrfc (refresh check)\nrf[x,x,x] (refresh channels)\nrfd (refresh deactivate all)\nsd (shutdown)\nlsn (last seq num)\ncbd (circular buffer detail)\ncte (enable cap test mode)\nctd (disable cap test mode)\n");
      output_tml_log_prompt();

      return;
    }

    //--------------------------------------------------
    // Quit
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_Q,strlen(CMD_Q))==0)
    {
      m_socket.close();
      return;
    }

    //--------------------------------------------------
    // Reset
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_RESET,strlen(CMD_RESET))==0)
    {
      sprintf(m_Buffer,"TerminalSession: The OMD program will now reset its internal data and states.");
      output_tml_log();
      m_ShrObj->ResetObjectsPurgeRawCirBuf();
      sprintf(m_Buffer,"TerminalSession: Internal data and states successfully reset.");
      output_tml_log_prompt();
      return;
    }

    //--------------------------------------------------
    // Shutdown
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_SHUTDOWN,strlen(CMD_SHUTDOWN))==0)
    {
      sprintf(m_Buffer, "TerminalSession: The OMD program will now shutdown. Stopping threads.");
      output_tml_log_prompt();
      m_ShrObj->SetSystemState(SharedObjects::DOWN);
      m_pChnlCtrlr->StopMcastRecvr();
      m_ios.stop();

      {
        // Insert dummy data into the circular buffers to wake threads up
        const shared_ptr<vector<unsigned short> > pvActiveMcastChnl = m_SysCfg->GetActiveMcastChnl();

        // Raw Cir Buf
        for (int i = 0; i < pvActiveMcastChnl->size(); ++i)
        {
          unsigned short usChannelID = (*pvActiveMcastChnl)[i];

          McastIdentifier mcRTA(usChannelID, McastIdentifier::REALTIME, McastIdentifier::A);
          McastIdentifier mcRTB(usChannelID, McastIdentifier::REALTIME, McastIdentifier::B);
          McastIdentifier mcRFA(usChannelID, McastIdentifier::REFRESH,  McastIdentifier::A);
          McastIdentifier mcRFB(usChannelID, McastIdentifier::REFRESH,  McastIdentifier::B);

          ExpandableCirBuffer* m_RawPktCirBuf_RTA = m_ShrObj->GetRawPktCirBufPtr(mcRTA);
          ExpandableCirBuffer* m_RawPktCirBuf_RTB = m_ShrObj->GetRawPktCirBufPtr(mcRTB);
          ExpandableCirBuffer* m_RawPktCirBuf_RFA = m_ShrObj->GetRawPktCirBufPtr(mcRFA);
          ExpandableCirBuffer* m_RawPktCirBuf_RFB = m_ShrObj->GetRawPktCirBufPtr(mcRFB);

          m_RawPktCirBuf_RTA->PushBack();
          m_RawPktCirBuf_RTB->PushBack();
          m_RawPktCirBuf_RFA->PushBack();
          m_RawPktCirBuf_RFB->PushBack();
        }

        // Msg Cir Buf
        char byteDummyMsg[MAX_OMD_PACKET_SIZE];
        memset(byteDummyMsg,'\0',MAX_OMD_PACKET_SIZE);
        OMD_Sequence_Reset * dummyStrct = (OMD_Sequence_Reset *) (&byteDummyMsg[0]);
        dummyStrct->MsgSize = sizeof(OMD_Sequence_Reset);
        dummyStrct->MsgType = OMD_SEQUENCE_RESET;
        dummyStrct->NewSeqNo = 1;

        for (int i = 0; i < pvActiveMcastChnl->size(); ++i)
        {
          unsigned short usChannelID = (*pvActiveMcastChnl)[i];
          ExpandableCirBuffer4Msg* m_MsgCirBuf_RT = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, usChannelID);
          uint32_t uiSeqNo;
          if (!m_MsgCirBuf_RT->GetLatestSeqNo(uiSeqNo)) uiSeqNo = m_MsgCirBuf_RT->GetStartSeqNo();
          m_MsgCirBuf_RT->PushMsg((BYTE *)byteDummyMsg, ++uiSeqNo, 1);
        }
      }
      return;
    }

    //--------------------------------------------------
    // Refresh
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_REFRESH,strlen(CMD_REFRESH))==0)
    {
      const shared_ptr<vector<unsigned short> > pvActiveMcastChnl = m_SysCfg->GetActiveMcastChnl();

      if (m_Buffer[strlen(CMD_REFRESH)] == 'a')
      {
        for (int i = 0; i < pvActiveMcastChnl->size(); ++i)
        {
          unsigned short usChannelID = (*pvActiveMcastChnl)[i];
          m_ShrObj->ActivateRefresh(usChannelID);

          sprintf(m_Buffer, "TerminalSession: %s: Manually activated refresh mode on Channel %u.", CMD_REFRESH, usChannelID);
          output_tml_log();
        }
      }
      else if (m_Buffer[strlen(CMD_REFRESH)] == 'd')
      {
        for (int i = 0; i < pvActiveMcastChnl->size(); ++i)
        {
          unsigned short usChannelID = (*pvActiveMcastChnl)[i];
          m_ShrObj->DeactivateRefresh(usChannelID);

          sprintf(m_Buffer, "TerminalSession: %s: Manually deactivated refresh mode on Channel %u.", CMD_REFRESH, usChannelID);
          output_tml_log();
        }
      }
      else if (m_Buffer[strlen(CMD_REFRESH)] == 'c')
      {
        for (int i = 0; i < pvActiveMcastChnl->size(); ++i)
        {
          unsigned short usChannelID = (*pvActiveMcastChnl)[i];
          sprintf(m_Buffer, "TerminalSession: %s: Refresh mode on Channel %u is activated: %c", CMD_REFRESH, usChannelID, (m_ShrObj->CheckRefreshActivatnStatus(usChannelID) ? 'Y':'N'));
          output_tml_log();
        }
      }
      else
      {
        string inputchnl(&m_Buffer[strlen(CMD_REFRESH)]);

        char_separator<char> sep(",");
        tokenizer< char_separator<char> > tokens(inputchnl, sep);

        BOOST_FOREACH (const string& t, tokens)
        {
          unsigned short usChannelID;
          try
          {
            usChannelID = lexical_cast<unsigned short>(Util::Trim(t));

            //--------------------------------------------------
            // Search if exist in Active Channels
            //--------------------------------------------------
            bool bFound = false;
            for (int i = 0; i < pvActiveMcastChnl->size(); ++i)
              if ((*pvActiveMcastChnl)[i] == usChannelID)
                bFound = true;
            //--------------------------------------------------

            //--------------------------------------------------
            if (bFound)
            {
              m_ShrObj->ActivateRefresh(usChannelID);
              sprintf(m_Buffer,"TerminalSession: %s: Manually activated refresh mode on Channel %u.", CMD_REFRESH, usChannelID);
              output_tml_log();
            }
            else
            {
              sprintf(m_Buffer,"TerminalSession: %s: Channel isn't active: %u.", CMD_REFRESH, usChannelID);
              output_tml_log();
            }
            //--------------------------------------------------

          }
          catch (bad_lexical_cast &)
          {
            sprintf(m_Buffer, "TerminalSession: Unknown channel id. %u.", usChannelID);
            output_tml_log_prompt();

            return;
          }
        }
      }


      //--------------------------------------------------
      // Prompt
      //--------------------------------------------------
      strcpy(m_Buffer,CMD_PS1);

      boost::asio::async_write(m_socket,
          boost::asio::buffer(m_Buffer, strlen(m_Buffer)),
          boost::bind(&TerminalSession::handle_write, this,
            boost::asio::placeholders::error));

      memset(m_Buffer,'\0',SIZE);

      return;

    }

    //--------------------------------------------------
    // Log Level
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_LOGLEVEL,strlen(CMD_LOGLEVEL))==0 && strncmp(m_Buffer,CMD_LASTSEQNO,strlen(CMD_LASTSEQNO))!=0 )
    {

      switch (m_Buffer[strlen(CMD_LOGLEVEL)])
      {
        case  '0'  :  { sprintf(m_Buffer, "TerminalSession: Changing log level to  EMERGENCY    ");  m_Logger->SetLogLevel(  Logger::EMERGENCY );   break; }
        case  '1'  :  { sprintf(m_Buffer, "TerminalSession: Changing log level to  ALERT        ");  m_Logger->SetLogLevel(  Logger::ALERT     );   break; }
        case  '2'  :  { sprintf(m_Buffer, "TerminalSession: Changing log level to  CRITICAL     ");  m_Logger->SetLogLevel(  Logger::CRITICAL  );   break; }
        case  '3'  :  { sprintf(m_Buffer, "TerminalSession: Changing log level to  ERROR        ");  m_Logger->SetLogLevel(  Logger::ERROR     );   break; }
        case  '4'  :  { sprintf(m_Buffer, "TerminalSession: Changing log level to  WARNING      ");  m_Logger->SetLogLevel(  Logger::WARNING   );   break; }
        case  '5'  :  { sprintf(m_Buffer, "TerminalSession: Changing log level to  NOTICE       ");  m_Logger->SetLogLevel(  Logger::NOTICE    );   break; }
        case  '6'  :  { sprintf(m_Buffer, "TerminalSession: Changing log level to  INFORMATIONAL");  m_Logger->SetLogLevel(  Logger::INFO      );   break; }
        case  '7'  :  { sprintf(m_Buffer, "TerminalSession: Changing log level to  DEBUG        ");  m_Logger->SetLogLevel(  Logger::DEBUG     );   break; }

        default    :  { sprintf(m_Buffer, "TerminalSession: Unrecognized log level. The log level must range from 0 to 7.");                          break; }
      }

      output_tml_log_prompt();

      return;

    }
    //--------------------------------------------------
    // Dump last seq num in all channels
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_LASTSEQNO,strlen(CMD_LASTSEQNO))==0 && strncmp(m_Buffer,CMD_LOGLEVEL,strlen(CMD_LOGLEVEL))!=0)
    {
      unsigned short usChannelID = 0;
      uint32_t uiRTLastSeqNo = 0;
      uint32_t uiRFLastSeqNo = 0;

      const shared_ptr<vector<unsigned short> > pvActiveMcastChnl = m_SysCfg->GetActiveMcastChnl();
      for (int i = 0; i < pvActiveMcastChnl->size(); ++i)
      {

        usChannelID = (*pvActiveMcastChnl)[i];

        //--------------------------------------------------
        // Raw Packet Cir Buf
        //--------------------------------------------------
        {
          McastIdentifier mci_rta(usChannelID, McastIdentifier::REALTIME, McastIdentifier::A);
          McastIdentifier mci_rtb(usChannelID, McastIdentifier::REALTIME, McastIdentifier::B);
          McastIdentifier mci_rfa(usChannelID, McastIdentifier::REFRESH,  McastIdentifier::A);
          McastIdentifier mci_rfb(usChannelID, McastIdentifier::REFRESH,  McastIdentifier::B);

          ExpandableCirBuffer* m_RawPktCirBuf_RT_A = m_ShrObj->GetRawPktCirBufPtr(mci_rta);
          ExpandableCirBuffer* m_RawPktCirBuf_RT_B = m_ShrObj->GetRawPktCirBufPtr(mci_rtb);
          ExpandableCirBuffer* m_RawPktCirBuf_RF_A = m_ShrObj->GetRawPktCirBufPtr(mci_rfa);
          ExpandableCirBuffer* m_RawPktCirBuf_RF_B = m_ShrObj->GetRawPktCirBufPtr(mci_rfb);

          ExpandableCirBuffer4Msg* m_MsgCirBuf_RT = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, usChannelID);
          ExpandableCirBuffer4Msg* m_MsgCirBuf_RF = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REFRESH,  usChannelID);

          sprintf(m_Buffer, "TerminalSession: ChannelID:%u. RT A RawPktCirBuf Size: %u.",          usChannelID, m_RawPktCirBuf_RT_A->Size());          output_tml_log();
          sprintf(m_Buffer, "TerminalSession: ChannelID:%u. RT B RawPktCirBuf Size: %u.",          usChannelID, m_RawPktCirBuf_RT_B->Size());          output_tml_log();
          sprintf(m_Buffer, "TerminalSession: ChannelID:%u. RF A RawPktCirBuf Size: %u.",          usChannelID, m_RawPktCirBuf_RF_A->Size());          output_tml_log();
          sprintf(m_Buffer, "TerminalSession: ChannelID:%u. RF B RawPktCirBuf Size: %u.",          usChannelID, m_RawPktCirBuf_RF_B->Size());          output_tml_log();

          sprintf(m_Buffer, "TerminalSession: ChannelID:%u. RT   MsgCirBuf    Size: %u.",          usChannelID, m_MsgCirBuf_RT->Size());               output_tml_log();
          sprintf(m_Buffer, "TerminalSession: ChannelID:%u. RF   MsgCirBuf    Size: %u.",          usChannelID, m_MsgCirBuf_RF->Size());               output_tml_log();
        }

        //--------------------------------------------------
        // Msg Cir Buf
        //--------------------------------------------------
        {
          ExpandableCirBuffer4Msg *ecbRT = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, usChannelID);
          ExpandableCirBuffer4Msg *ecbRF = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REFRESH,  usChannelID);

          if (!ecbRT->GetLatestSeqNo(uiRTLastSeqNo))
          {
            if (ecbRT->GetStartSeqNo() > 0)
              uiRTLastSeqNo = ecbRT->GetStartSeqNo()-1;
            else
              uiRTLastSeqNo = 0;
          }

          if (!ecbRF->GetLatestSeqNo(uiRFLastSeqNo))
          {
            if (ecbRF->GetStartSeqNo() > 0)
              uiRFLastSeqNo = ecbRF->GetStartSeqNo()-1;
            else
              uiRFLastSeqNo = 0;
          }


          //--------------------------------------------------
          // RT
          //--------------------------------------------------
          {
            ostringstream ooAdjLSN;
            uint32_t uiAdjLSN = uiRTLastSeqNo;
            if (uiAdjLSN == 0) ooAdjLSN << "N/A";
            else ooAdjLSN << uiAdjLSN;

            ostringstream ooRawLSN;
            long lRawLSN = (uiRTLastSeqNo-m_ShrObj->CheckRTChnlLatestSeqNoOffset(usChannelID));
            if (lRawLSN <= 0) ooRawLSN << "N/A";
            else ooRawLSN << lRawLSN;

            sprintf(m_Buffer, "TerminalSession: ChannelID:%u. [RT] Last Seq No (adj): %s. (raw): %s.", usChannelID, ooAdjLSN.str().c_str(), ooRawLSN.str().c_str());
            output_tml_log();
          }

          //--------------------------------------------------
          // RF
          //--------------------------------------------------
          {
            ostringstream ooAdjLSN;
            uint32_t uiAdjLSN = uiRFLastSeqNo;
            if (uiAdjLSN == 0) ooAdjLSN << "N/A";
            else ooAdjLSN << uiAdjLSN;

            ostringstream ooRawLSN;
            long lRawLSN = (uiRFLastSeqNo-m_ShrObj->CheckRFChnlLatestSeqNoOffset(usChannelID));
            if (lRawLSN <= 0) ooRawLSN << "N/A";
            else ooRawLSN << lRawLSN;

            sprintf(m_Buffer, "TerminalSession: ChannelID:%u. [RF] Last Seq No (adj): %s. (raw): %s.", usChannelID, ooAdjLSN.str().c_str(), ooRawLSN.str().c_str());
            output_tml_log();
          }
        }
      }

      sprintf(m_Buffer, "\n");
      output_tml_log_prompt();

      return;

    }
    //--------------------------------------------------
    // Circular buffer detail
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_CIRCUFDTL,strlen(CMD_CIRCUFDTL))==0)
    {
      unsigned short usChannelID = 0;
      uint32_t uiRTLastSeqNo = 0;
      uint32_t uiRFLastSeqNo = 0;

      const shared_ptr<vector<unsigned short> > pvActiveMcastChnl = m_SysCfg->GetActiveMcastChnl();
      for (int i = 0; i < pvActiveMcastChnl->size(); ++i)
      {
        usChannelID = (*pvActiveMcastChnl)[i];

        ExpandableCirBuffer4Msg *ecbRT = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REALTIME, usChannelID);
        ExpandableCirBuffer4Msg *ecbRF = m_ShrObj->GetMsgCirBufPtr(McastIdentifier::REFRESH,  usChannelID);

        sprintf(m_Buffer, "TerminalSession: ChannelID:%u. [RT] Seq No offset: %u, Missing Seq No (adj): %s.", usChannelID, m_ShrObj->CheckRTChnlLatestSeqNoOffset(usChannelID), ecbRT->GetAllMissingSeqNo().substr(0,200).c_str()); //200 is the length of an output of reasonable length
        output_tml_log();
        sprintf(m_Buffer, "TerminalSession: ChannelID:%u. [RF] Seq No offset: %u, Missing Seq No (adj): %s.", usChannelID, m_ShrObj->CheckRFChnlLatestSeqNoOffset(usChannelID), ecbRF->GetAllMissingSeqNo().substr(0,200).c_str()); //200 is the length of an output of reasonable length
        output_tml_log();
      }

      sprintf(m_Buffer, "\n");
      output_tml_log_prompt();

      return;

    }

    //--------------------------------------------------
    // OMD Readiness Test: Session 4: Capacity test
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_CAPTEST,strlen(CMD_CAPTEST))==0)
    {
      if (m_Buffer[strlen(CMD_CAPTEST)] == 'e')
      {
        m_ShrObj->TurnOnCapTest();
        sprintf(m_Buffer,"TerminalSession: Capacity Test mode: ENABLED. Entering special mode to cater for capacity test. The circular buffers will continue to be processed even if refresh mode is activated.", m_Buffer);
        output_tml_log_prompt();
      } 
      else if (m_Buffer[strlen(CMD_CAPTEST)] == 'd')
      {
        m_ShrObj->TurnOffCapTest();
        sprintf(m_Buffer,"TerminalSession: Capacity Test mode: DISABLED.", m_Buffer);
        output_tml_log_prompt();
      }
      return;
    }

    //--------------------------------------------------
    // Unknown command
    //--------------------------------------------------
    else
    {
      sprintf(m_Buffer, "TerminalSession: Unknown command.");
      output_tml_log_prompt();

      return;
    }

  }
  else
  {
    //delete this; // don't delete
    if (error.message().substr(0,11) != "End of file")
      m_Logger->Write(Logger::ERROR, "TerminalSession: Error: %s", error.message().c_str());

  }

  return;
}


void TerminalSession::handle_write(const boost::system::error_code& error)
{

  if (!error)
  {
    //--------------------------------------------------
    // Read some more
    //--------------------------------------------------
    m_socket.async_read_some(boost::asio::buffer(&m_Buffer[0], SIZE),
        boost::bind(
          &TerminalSession::handle_read,
          this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    //delete this; // don't delete
    m_Logger->Write(Logger::ERROR, "TerminalSession: Error: %s", error.message().c_str());
  }
}

void TerminalSession::handle_write_donthg(const boost::system::error_code& error)
{

  if (!error)
  {
    // do nothing
  }
  else
  {
    //delete this; // don't delete
    m_Logger->Write(Logger::ERROR, "TerminalSession: Error: %s", error.message().c_str());
  }
}

void TerminalSession::output_tml_log_prompt()
{
  m_Logger->Write(Logger::NOTICE, m_Buffer);

  strcat(m_Buffer,"\n");
  strcat(m_Buffer,CMD_PS1);

  boost::asio::async_write(m_socket,
      boost::asio::buffer(m_Buffer, strlen(m_Buffer)),
      boost::bind(&TerminalSession::handle_write, this,
        boost::asio::placeholders::error));

  memset(m_Buffer,'\0',SIZE);
  return;
}

void TerminalSession::output_tml_log()
{
  m_Logger->Write(Logger::NOTICE, m_Buffer);

  strcat(m_Buffer,"\n");

  boost::asio::async_write(m_socket,
      boost::asio::buffer(m_Buffer, strlen(m_Buffer)),
      boost::bind(&TerminalSession::handle_write_donthg, this,
        boost::asio::placeholders::error));

  memset(m_Buffer,'\0',SIZE);
  return;
}


  TerminalServer::TerminalServer(boost::asio::io_service& io_service, short port, ChannelController *cc)
: io_service_(io_service),
  acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
  m_pChnlCtrlr(cc)
{
  m_Logger = Logger::Instance();
  m_ShrObj = SharedObjects::Instance();
  start_accept();
}

void TerminalServer::start_accept()
{
  TerminalSession* newTmlSess = new TerminalSession(io_service_, m_pChnlCtrlr);
  m_vTmlSess.push_back(newTmlSess);

  if (!m_ShrObj->ThreadShouldExit())
  {
    acceptor_.async_accept(newTmlSess->socket(),
        boost::bind(
          &TerminalServer::handle_accept,
          this,
          newTmlSess,
          boost::asio::placeholders::error));
  }
}

void TerminalServer::handle_accept(TerminalSession* newTmlSess, const boost::system::error_code& error)
{
  m_Logger->Write(Logger::NOTICE, "TerminalServer: Accepted TCP connection.");

  if (!error)
  {
    newTmlSess->start();
    newTmlSess = new TerminalSession(io_service_, m_pChnlCtrlr);
    m_vTmlSess.push_back(newTmlSess);
    if (!m_ShrObj->ThreadShouldExit())
    {
      acceptor_.async_accept(newTmlSess->socket(),
          boost::bind(
            &TerminalServer::handle_accept,
            this,
            newTmlSess,
            boost::asio::placeholders::error));
    }
  }
  else
  {
    delete newTmlSess;
  }
}


TerminalThread::TerminalThread(ChannelController *cc) :
  m_SystemServicePort(8000), //default
  m_pChnlCtrlr(cc)
{
  m_SysCfg            = SystemConfig::Instance();
  m_SystemServicePort = m_SysCfg->GetSystemServicePort();
}

TerminalThread::~TerminalThread() {
}

void TerminalThread::Run()
{
  //--------------------------------------------------
  // Startng RTS Testing server
  //--------------------------------------------------
  try
  {
    boost::asio::io_service io_service;
    TerminalServer s(io_service,m_SystemServicePort,m_pChnlCtrlr);
    io_service.run(); // Blocks here
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

}
