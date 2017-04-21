//**************************************************
//  Author:      Sunny Yan
//  Created On:  Apr 9, 2014
//**************************************************
#include "McastReceiver.h"

McastReceiver::McastReceiver(
  boost::asio::io_service& io_service,
  const string & sName,
  const boost::asio::ip::address& multicast_address,
  const short multicast_port,
  boost::posix_time::ptime progstarttime,
  const char * sDataFolder,
  const short _Mode)
  : m_Socket(io_service),
  m_Name(sName),
  m_CannedFile(NULL),
  m_ProgramStartTime(progstarttime),
  m_Mode(_Mode)
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

McastReceiver::~McastReceiver()
{
}


McastReceiverHKExSim::McastReceiverHKExSim(
  boost::asio::io_service& io_service,
  const string & sName,
  const boost::asio::ip::address& multicast_address,
  const short multicast_port,
  boost::posix_time::ptime progstarttime,
  const char * sDataFolder,
  const short _Mode)
  : McastReceiver(io_service, sName, multicast_address, multicast_port, progstarttime, sDataFolder, _Mode) {}

McastReceiverRelTime::McastReceiverRelTime(
  boost::asio::io_service& io_service,
  const string & sName,
  const boost::asio::ip::address& multicast_address,
  const short multicast_port,
  boost::posix_time::ptime progstarttime,
  const char * sDataFolder,
  const short _Mode)
  : McastReceiver(io_service, sName, multicast_address, multicast_port, progstarttime, sDataFolder, _Mode) {}

McastReceiverUnixTime::McastReceiverUnixTime(
  boost::asio::io_service& io_service,
  const string & sName,
  const boost::asio::ip::address& multicast_address,
  const short multicast_port,
  boost::posix_time::ptime progstarttime,
  const char * sDataFolder,
  const short _Mode)
  : McastReceiver(io_service, sName, multicast_address, multicast_port, progstarttime, sDataFolder, _Mode) {}



void McastReceiverHKExSim::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
{
  if (!error)
  {
    unsigned short iPktLen = *((uint16_t*)(&m_Buffer[0]));
    fwrite((&m_Buffer[0])+1,1,1,m_CannedFile);
    fwrite(m_Buffer,1,1,m_CannedFile);
    fwrite(m_Buffer,1,iPktLen,m_CannedFile);
    fflush(m_CannedFile);

    m_Socket.async_receive_from(
      boost::asio::buffer(m_Buffer, BUFFER_SIZE),
      m_SenderEndpoint,
      boost::bind(&McastReceiver::handle_receive_from, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
  }
}

void McastReceiverRelTime::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
{
  if (!error)
  {
    //--------------------------------------------------
    // 8 bytes - Relative time stamp in millisec
    // n bytes - The actual packet
    //--------------------------------------------------
    uint16_t uiPktSize = *((uint16_t*)&m_Buffer[0]);
    if (uiPktSize <= BUFFER_SIZE)
    {
      boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
      boost::posix_time::time_duration diff = now - m_ProgramStartTime;
      uint64_t uiRelTime = diff.total_milliseconds();
      fwrite(&uiRelTime,8,1,m_CannedFile);

      //--------------------------------------------------
      // The msg itself
      //--------------------------------------------------
      uint16_t uiPktSize = *((uint16_t*)&m_Buffer[0]);
      fwrite(m_Buffer,1,uiPktSize,m_CannedFile);
      fflush(m_CannedFile);
    }

    m_Socket.async_receive_from(
      boost::asio::buffer(m_Buffer, BUFFER_SIZE),
      m_SenderEndpoint,
      boost::bind(&McastReceiver::handle_receive_from, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
  }
}

void McastReceiverUnixTime::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
{
  if (!error)
  {
    //--------------------------------------------------
    // 8 bytes - Unix time stamp in millisec
    // n bytes - The actual packet
    //--------------------------------------------------
    uint16_t uiPktSize = *((uint16_t*)&m_Buffer[0]);
    if (uiPktSize <= BUFFER_SIZE)
    {
      unsigned long uiUnixTime = SDateTime::GetCurrentUnixTimeInMillsecGMT();
      fwrite(&uiUnixTime,8,1,m_CannedFile);

      //--------------------------------------------------
      // The msg itself
      //--------------------------------------------------
      uint16_t uiPktSize = *((uint16_t*)&m_Buffer[0]);
      fwrite(m_Buffer,1,uiPktSize,m_CannedFile);
      fflush(m_CannedFile);
    }

    m_Socket.async_receive_from(
      boost::asio::buffer(m_Buffer, BUFFER_SIZE),
      m_SenderEndpoint,
      boost::bind(&McastReceiver::handle_receive_from, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
  }
}


