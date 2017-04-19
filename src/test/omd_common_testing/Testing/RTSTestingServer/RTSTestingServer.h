/*
 * RTSTestingSession.h
 *
 *  Created on: May 12, 2014
 *      Author: sunny
 */

#ifndef RTSTESTINGSERVER_H_
#define RTSTESTINGSERVER_H_

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "OMD_Global.h"
#include "OMD_Message_Headers.h"
#include <boost/thread.hpp>
#include "../UTest/UTest.h"

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;


class RTSTestingSession
{
  public:

    RTSTestingSession(boost::asio::io_service& io_service, UTest &ut, unsigned short id, uint32_t begseqno, uint32_t endseqno, bool btestnontrigger, bool breturnincorrectchannelid, uint16_t responsemsg, bool hold240sec) :
      m_socket(io_service),
      m_ut(ut),
      m_ChannelID(id),
      m_BeginSeqNo(begseqno),
      m_EndSeqNo(endseqno),
      m_TestNonTrigger(btestnontrigger),
      m_ReturnIncorrectChannelID(breturnincorrectchannelid),
      m_ResponseMsg(responsemsg),
      m_Hold240sec(hold240sec)
  {
  }

    tcp::socket& socket()
    {
      return m_socket;
    }

    void start()
    {
      try
      {
        cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << "RTSTestingSession begins." << endl;
        //--------------------------------------------------
        // Key parameters 
        //--------------------------------------------------
        unsigned short usChannelID = 0;
        uint32_t uiBeginSeqNum     = 0;
        uint32_t uiEndSeqNum       = 0;

        //--------------------------------------------------
        // Get Logon request 
        //--------------------------------------------------
        {
          boost::system::error_code error;
          size_t read_len = 0;
          uint16_t uiMsgSize = 0;
          size_t uiOffset = 0;
          for (;;)
          {

            read_len = m_socket.read_some(boost::asio::buffer(&m_Buffer[uiOffset],MAX_OMD_PACKET_SIZE-uiOffset), error);
            uiOffset += read_len;

            if (uiOffset > 2) // we've got the Msg Size!
              uiMsgSize = READ_UINT16(&m_Buffer[0]);

            // finished getting logon response
            if (uiMsgSize > 0 && uiOffset >= uiMsgSize)
              break;

            if (error == boost::asio::error::eof)
              break; // Connection closed cleanly by peer.
            else if (error)
              throw boost::system::system_error(error); // Some other error.
          }

        }

        //--------------------------------------------------
        // Testing non-trigger?? 
        // Reaching here means RTS is triggered
        //--------------------------------------------------
        if (m_TestNonTrigger) m_ut.Assert(false, __FILE__, __FUNCTION__, __LINE__);

        //--------------------------------------------------
        // Logon request received
        //--------------------------------------------------
        {
          cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << endl << flush;
          OMD_Packet_Header *oph = (OMD_Packet_Header*)&m_Buffer[0];
          OMD_Logon *ol = (OMD_Logon*)&m_Buffer[sizeof(OMD_Packet_Header)];

          m_ut.Assert(oph->PktSize == sizeof(OMD_Retransmission_Header)+sizeof(OMD_Retransmission_Response), __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(oph->MsgCount == 1, __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(oph->SeqNum == 0, __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(oph->SendTime == 0, __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(ol->MsgSize == 16, __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(ol->MsgType == OMD_LOGON, __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(string(ol->Username) == "test01", __FILE__, __FUNCTION__, __LINE__);
        }


        //--------------------------------------------------
        // Send Logon response 
        //--------------------------------------------------
        {
          cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << endl << flush;
          OMD_Packet_Header *oph = (OMD_Packet_Header*)&m_Buffer[0];
          OMD_Logon_Response* olr = (OMD_Logon_Response*) &m_Buffer[sizeof(OMD_Packet_Header)];


          WRITE_UINT16(&(oph->PktSize )       , sizeof(OMD_Packet_Header) + sizeof(OMD_Logon_Response) );
          WRITE_UINT8 (&(oph->MsgCount)       , 1   );
          WRITE_UINT16(&(oph->SeqNum  )       , 0   ); // I wont care
          WRITE_UINT16(&(oph->SendTime)       , 0   ); // I wont care

          WRITE_UINT16(&(olr->MsgSize )       , sizeof(OMD_Logon_Response) );
          WRITE_UINT16(&(olr->MsgType )       , OMD_LOGON_RESPONSE );
          // 0 Session Active
          // 5 Invalid username or IP Address
          // 100 User already connected
          WRITE_UINT16(&(olr->SessionStatus )       , 0 );

          m_socket.write_some(boost::asio::buffer(m_Buffer, oph->PktSize));

        }
        //--------------------------------------------------
        // Get Retransmission request 
        //--------------------------------------------------
        {
          cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << endl << flush;
          boost::system::error_code error;
          size_t read_len = 0;
          uint16_t uiMsgSize = 0;
          size_t uiOffset = 0;
          for (;;)
          {
            read_len = m_socket.read_some(boost::asio::buffer(&m_Buffer[uiOffset],MAX_OMD_PACKET_SIZE-uiOffset), error);
            uiOffset += read_len;

            if (uiOffset > 2) // we've got the Msg Size!
              uiMsgSize = READ_UINT16(&m_Buffer[0]);

            // finished getting logon response
            if (uiMsgSize > 0 && uiOffset >= uiMsgSize)
              break;

            if (error == boost::asio::error::eof)
              break; // Connection closed cleanly by peer.
            else if (error)
              throw boost::system::system_error(error); // Some other error.
          }
        }
        //--------------------------------------------------
        // Retransmission request received
        //--------------------------------------------------
        {
          cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << endl << flush;
          OMD_Packet_Header *oph = (OMD_Packet_Header*)&m_Buffer[0];
          OMD_Retransmission_Request *orr = (OMD_Retransmission_Request*)&m_Buffer[sizeof(OMD_Packet_Header)];

          usChannelID   = orr->ChannelID;
          uiBeginSeqNum = orr->BeginSeqNum;
          uiEndSeqNum   = orr->EndSeqNum  ;

          m_ut.Assert(oph->MsgCount == 1, __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(oph->PktSize == sizeof(OMD_Retransmission_Header)+sizeof(OMD_Retransmission_Request), __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(oph->SeqNum == 0, __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(oph->SendTime == 0, __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(orr->MsgSize == sizeof(OMD_Retransmission_Request)   , __FILE__, __FUNCTION__, __LINE__);
          m_ut.Assert(orr->MsgType == OMD_RETRANSMISSION_REQUEST, __FILE__, __FUNCTION__, __LINE__);

          m_ut.Assert(orr->ChannelID    ==  m_ChannelID,   __FILE__,  __FUNCTION__,  __LINE__);
          m_ut.Assert(orr->BeginSeqNum  ==  m_BeginSeqNo,  __FILE__,  __FUNCTION__,  __LINE__);
          m_ut.Assert(orr->EndSeqNum    ==  m_EndSeqNo,    __FILE__,  __FUNCTION__,  __LINE__);

          cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << orr->ChannelID   << endl << flush;
          cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << orr->BeginSeqNum << endl << flush;
          cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << orr->EndSeqNum   << endl << flush;
        }

        //--------------------------------------------------
        // Send Retransmission response 
        //--------------------------------------------------
        {
          cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << endl << flush;
          OMD_Packet_Header *oph = (OMD_Packet_Header*)&m_Buffer[0];
          OMD_Retransmission_Response* orr = (OMD_Retransmission_Response*) &m_Buffer[sizeof(OMD_Packet_Header)];

          WRITE_UINT16(&(oph->PktSize       )       , sizeof(OMD_Packet_Header) + sizeof(OMD_Retransmission_Response) );
          WRITE_UINT8 (&(oph->MsgCount      )       , 1   );
          WRITE_UINT16(&(oph->SeqNum        )       , 0   ); // I wont care
          WRITE_UINT16(&(oph->SendTime      )       , 0   ); // I wont care

          WRITE_UINT16(&(orr->MsgSize      )   , sizeof(OMD_Retransmission_Response) );
          WRITE_UINT16(&(orr->MsgType      )   , OMD_RETRANSMISSION_RESPONSE );

          if (m_ReturnIncorrectChannelID)
            WRITE_UINT16(&(orr->ChannelID    )   , usChannelID+1 );
          else
            WRITE_UINT16(&(orr->ChannelID    )   , usChannelID );
          // WRITE_UINT16(&(orr->ChannelID    )   , (m_ReturnIncorrectChannelID ? usChannelID+1 : usChannelID) );

          // 0 Request accepted
          // 1 Unknown/Unauthorized channel ID
          // 2 Messages not available
          // 100 Exceeds maximum sequence range
          // 101 Exceeds maximum requests in a day
          cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_ResponseMsg << endl << flush;
          WRITE_UINT16(&(orr->RetransStatus)   , m_ResponseMsg   );
          WRITE_UINT16(&(orr->BeginSeqNum  )   , uiBeginSeqNum   );
          WRITE_UINT16(&(orr->EndSeqNum    )   , uiEndSeqNum     );

          m_socket.write_some(boost::asio::buffer(m_Buffer, oph->PktSize));
        }

        if (m_ResponseMsg == 0)
        {
          //--------------------------------------------------
          // Send dummy content 
          //--------------------------------------------------
          unsigned short usHeartBeat = 0;
          for (unsigned int i = uiBeginSeqNum; i <= uiEndSeqNum; ++i)
          {
            memset(m_Buffer,'\0',MAX_OMD_PACKET_SIZE);
            OMD_Packet_Header *oph = (OMD_Packet_Header*)&m_Buffer[0];
            OMDC_Trade_Ticker* ott = (OMDC_Trade_Ticker*) &m_Buffer[sizeof(OMD_Packet_Header)];

            WRITE_UINT16(&(oph->PktSize       )       , sizeof(OMD_Packet_Header) + sizeof(OMDC_Trade_Ticker) );
            WRITE_UINT8 (&(oph->MsgCount      )       , 1   );
            WRITE_UINT32(&(oph->SeqNum        )       , i   );
            WRITE_UINT64(&(oph->SendTime      )       , 1399268283   );

            WRITE_UINT16(&(ott->MsgSize           )   ,  sizeof(OMDC_Trade_Ticker)  );
            WRITE_UINT16(&(ott->MsgType           )   ,  OMDC_TRADE_TICKER  );
            WRITE_UINT32(&(ott->SecurityCode      )   ,  9394  );
            WRITE_UINT32(&(ott->TickerID          )   ,  5354  );
            WRITE_INT32 (&(ott->Price             )   ,  9413  );
            WRITE_UINT64(&(ott->AggregateQuantity )   ,  709394  );
            WRITE_UINT64(&(ott->TradeTime         )   ,  1399268283  );
            WRITE_INT16 (&(ott->TrdType           )   ,  0  );
            WRITE_UINT8 (&(ott->TrdCancelFlag     )   ,  (uint8_t)'N'  );

            m_socket.write_some(boost::asio::buffer(m_Buffer, sizeof(OMD_Packet_Header) + sizeof(OMDC_Trade_Ticker)));
            cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << endl;


            {
              //--------------------------------------------------
              // Heartbeat every msg. Just for testing
              //--------------------------------------------------
              WRITE_UINT16(&(oph->PktSize       )       , sizeof(OMD_Packet_Header) );
              WRITE_UINT8 (&(oph->MsgCount      )       , 0   );
              WRITE_UINT32(&(oph->SeqNum        )       , i   );
              WRITE_UINT64(&(oph->SendTime      )       , 0   );

              m_socket.write_some(boost::asio::buffer(m_Buffer, sizeof(OMD_Packet_Header)));

              boost::system::error_code error;
              size_t read_len = 0;
              size_t tot_read_len = 0;
              cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << endl;
              while (tot_read_len < oph->PktSize)
              {
                read_len = m_socket.read_some(boost::asio::buffer(&m_Buffer[tot_read_len],oph->PktSize-tot_read_len), error);
                tot_read_len += read_len;
              }
              cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << endl;

              OMD_Packet_Header *oph_hb = (OMD_Packet_Header*)&m_Buffer[0];

              m_ut.Assert(oph->PktSize  == sizeof(OMD_Packet_Header), __FILE__, __FUNCTION__, __LINE__);
              m_ut.Assert(oph->MsgCount == 0,                         __FILE__, __FUNCTION__, __LINE__);
              m_ut.Assert(oph->SeqNum   == i,                         __FILE__, __FUNCTION__, __LINE__);
              m_ut.Assert(oph->SendTime == 0,                         __FILE__, __FUNCTION__, __LINE__);
              usHeartBeat++;
              cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") # of times Heartbeat: " << usHeartBeat << endl << flush;
              cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << oph->PktSize << endl << flush;
              cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << oph->MsgCount << endl << flush;
              cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << oph->SeqNum << endl << flush;
            }

            if (m_Hold240sec)
            {
              boost::this_thread::sleep(boost::posix_time::seconds(240));
            }

          }
        }

        //--------------------------------------------------
        // Force close
        //--------------------------------------------------
        m_socket.close();

      }
      catch (std::exception& e)
      {
      }

    }

  private:
    tcp::socket m_socket;
    char m_Buffer[MAX_OMD_PACKET_SIZE];
    UTest &m_ut;
    unsigned short m_ChannelID;
    uint32_t m_BeginSeqNo;
    uint32_t m_EndSeqNo;
    bool m_TestNonTrigger;
    bool m_ReturnIncorrectChannelID;
    uint16_t m_ResponseMsg;
    bool m_Hold240sec;
};

class RTSTestingServer
{
  public:

    RTSTestingServer(boost::asio::io_service& io_service,
        short port,
        UTest &ut,
        unsigned short id,
        uint32_t begseqno,
        uint32_t endseqno,
        bool testnontrigger,
        bool returnincorrectchannelid,
        uint16_t responsemsg,
        bool hold240sec)
      : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      m_ut(ut),
      m_ChannelID(id),
      m_BegSeqNo(begseqno),
      m_EndSeqNo(endseqno),
      m_TestNonTrigger(testnontrigger),
      m_ReturnIncorrectChannelID(returnincorrectchannelid),
      m_ResponseMsg(responsemsg),
      m_Hold240sec(hold240sec)
  {
    start_accept();
  }


  private:
    void start_accept()
    {
      cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << ") " << "RTSTestingServer start_accept" << endl;
      RTSTestingSession* new_RTSTestingSession = new RTSTestingSession(io_service_, m_ut, m_ChannelID, m_BegSeqNo, m_EndSeqNo, m_TestNonTrigger, m_ReturnIncorrectChannelID, m_ResponseMsg, m_Hold240sec);
      m_RTSTestingSess.push_back(new_RTSTestingSession);
      acceptor_.async_accept(new_RTSTestingSession->socket(),
          boost::bind(&RTSTestingServer::handle_accept, this, new_RTSTestingSession,
            boost::asio::placeholders::error));
    }

    void handle_accept(RTSTestingSession* new_RTSTestingSession,
        const boost::system::error_code& error)
    {
      if (!error)
      {
        new_RTSTestingSession->start();
      }

      start_accept();
    }

    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    UTest &m_ut;
    unsigned short m_ChannelID;
    uint32_t m_BegSeqNo;
    uint32_t m_EndSeqNo;
    bool m_TestNonTrigger;
    bool m_ReturnIncorrectChannelID;
    uint16_t m_ResponseMsg;
    bool m_Hold240sec;
    ptr_vector<RTSTestingSession> m_RTSTestingSess;
};


class RTSTestingServerThread
{
  public:

    RTSTestingServerThread(unsigned short port,
        UTest &ut,
        unsigned short id,
        uint32_t begseqno,
        uint32_t endseqno,
        bool testnontrigger,
        bool returnincorrectchannelid,
        uint16_t responsemsg,
        bool hold240sec)
      :
        m_Port(port),
        m_ut(ut),
        m_ChannelID(id),
        m_BegSeqNo(begseqno),
        m_EndSeqNo(endseqno),
        m_TestNonTrigger(testnontrigger),
        m_ReturnIncorrectChannelID(returnincorrectchannelid),
        m_ResponseMsg(responsemsg),
        m_Hold240sec(hold240sec)
  {}


    ~RTSTestingServerThread() {}
    void Run() 
    {
      //--------------------------------------------------
      // Startng RTS Testing server
      //--------------------------------------------------
      try
      {
        boost::asio::io_service io_service;
        RTSTestingServer s(io_service,m_Port,m_ut,m_ChannelID,m_BegSeqNo,m_EndSeqNo,m_TestNonTrigger,m_ReturnIncorrectChannelID,m_ResponseMsg,m_Hold240sec);
        io_service.run(); // Blocks here 
      }
      catch (std::exception& e)
      {
        std::cerr << "Exception: " << e.what() << "\n";
      }

    }

  private:
    unsigned short  m_Port;
    UTest           &m_ut;
    unsigned short  m_ChannelID;
    uint32_t        m_BegSeqNo;
    uint32_t        m_EndSeqNo;
    bool            m_TestNonTrigger;
    bool            m_ReturnIncorrectChannelID;
    uint16_t        m_ResponseMsg;
    bool            m_Hold240sec;
};



#endif /* RTSTESTINGSERVER_H_ */

