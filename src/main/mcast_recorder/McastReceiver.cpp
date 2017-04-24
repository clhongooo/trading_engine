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
  m_ProgramStartTime(progstarttime),
  m_Mode(_Mode)
{
  string sFile = string(sDataFolder) + "/fmt" + boost::lexical_cast<string>(m_Mode) + "_" + m_Name + "_" + to_iso_string(m_ProgramStartTime).substr(0,15);
  m_BinaryRecorder.SetProgramStartTime(m_ProgramStartTime);
  m_BinaryRecorder.SetOutFilePathAndOpen(sFile);

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
    m_BinaryRecorder.WriteHKExSim(m_Buffer);

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
    m_BinaryRecorder.WriteHKExRelTime(m_Buffer);

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
    m_BinaryRecorder.WriteHKExUnixTime(m_Buffer);

    m_Socket.async_receive_from(
      boost::asio::buffer(m_Buffer, BUFFER_SIZE),
      m_SenderEndpoint,
      boost::bind(&McastReceiver::handle_receive_from, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
  }
}


