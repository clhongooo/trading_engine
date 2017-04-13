//**************************************************
//  Author:      Sunny Yan
//  Created On:  Apr 9, 2014
//
//**************************************************
#include "McastReceiver.h"

McastReceiver::McastReceiver(
  boost::asio::io_service& io_service,
  const string & sName,
  const boost::asio::ip::address& multicast_address,
  const short multicast_port,
  boost::posix_time::ptime progstarttime,
  const char * sDataFolder,
  const short iMode)
: m_Socket(io_service),
  m_Name(sName),
  m_CannedFile(NULL),
  m_ProgramStartTime(progstarttime),
  m_Mode(iMode)
{

  string sFile = string(sDataFolder) + "/fmt" + boost::lexical_cast<string>(m_Mode) + "_" + m_Name + "_" + to_iso_string(m_ProgramStartTime).substr(0,15);
  m_CannedFile = fopen(sFile.c_str(), "w");

  // Create the socket so that multiple may be bound to the same address.
  boost::asio::ip::udp::endpoint listen_endpoint(multicast_address, multicast_port);
  m_Socket.open(listen_endpoint.protocol());
  m_Socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
  m_Socket.bind(listen_endpoint);

  // Join the multicast group.
  m_Socket.set_option(
    boost::asio::ip::multicast::join_group(multicast_address));

  m_Socket.async_receive_from(
    boost::asio::buffer(m_Buffer, BUFFER_SIZE),
    m_SenderEndpoint,
    boost::bind(&McastReceiver::handle_receive_from, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));

}

void McastReceiver::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
{
  if (!error)
  {

    if (m_Mode == 1)
    {
      //--------------------------------------------------
      // HKEx Simulator format
      //--------------------------------------------------
      unsigned short iPktLen = *((uint16_t*)(&m_Buffer[0]));
      fwrite((&m_Buffer[0])+1,1,1,m_CannedFile);
      fwrite(m_Buffer,1,1,m_CannedFile);
      fwrite(m_Buffer,1,iPktLen,m_CannedFile);
      fflush(m_CannedFile);
    }
    else if (m_Mode == 2 || m_Mode == 3)
    {
      //--------------------------------------------------
      // Custom format:
      // 8 bytes - Relative time stamp in millisec
      // n bytes - The actual packet
      //--------------------------------------------------
      uint16_t uiPktSize = *((uint16_t*)&m_Buffer[0]);
      if (uiPktSize <= BUFFER_SIZE)
      {
        if (m_Mode == 2)
        {
          //--------------------------------------------------
          // Relative time in millisec
          //--------------------------------------------------
          boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
          boost::posix_time::time_duration diff = now - m_ProgramStartTime;
          uint64_t uiRelTime = diff.total_milliseconds();
          fwrite(&uiRelTime,8,1,m_CannedFile);
        }
        else if (m_Mode == 3)
        {
          //--------------------------------------------------
          // Unix time in millisec
          //--------------------------------------------------
          struct timeval tp;
          gettimeofday(&tp, NULL);
          uint64_t uiUnixTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
          fwrite(&uiUnixTime,8,1,m_CannedFile);
        }
        //--------------------------------------------------
        // Msg itself
        //--------------------------------------------------
        uint16_t uiPktSize = *((uint16_t*)&m_Buffer[0]);
        fwrite(m_Buffer,1,uiPktSize,m_CannedFile);

        fflush(m_CannedFile);
      }
    }

    m_Socket.async_receive_from(
      boost::asio::buffer(m_Buffer, BUFFER_SIZE),
      m_SenderEndpoint,
      boost::bind(&McastReceiver::handle_receive_from, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
  }
}

McastReceiver::~McastReceiver()
{
}

inline string McastReceiver::PadLeft(const string &sOri, char cPad, unsigned int iLen)
{
  if (iLen < 0 || sOri.length() >= iLen) return sOri;
  else
  {
    string sPad; sPad = cPad;
    string sR(sOri);
    while (sR.length() < iLen) sR = sPad + sR;
    return sR;
  }
}
