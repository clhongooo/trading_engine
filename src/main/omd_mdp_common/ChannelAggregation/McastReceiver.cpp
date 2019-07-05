/*
 * McastReceiver.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: sunny
 */

#include "McastReceiver.h"

McastReceiver::McastReceiver(boost::asio::io_context& io_context,
    const boost::asio::ip::address& listen_address,
    const boost::asio::ip::address& multicast_address,
    const short multicast_port,
    const short max_size,
    const McastIdentifier & id)
: m_Socket(io_context),
  m_MaxSize(max_size),
  m_Identifier(id),
  m_PrintPktHdr(false)
{
  //Initialization
  m_Logger = Logger::Instance();
  m_ShrObj = SharedObjects::Instance();
  m_SysCfg = SystemConfig::Instance();

  m_RawPktCirBuf = m_ShrObj->GetRawPktCirBufPtr(m_Identifier);

  //--------------------------------------------------
  // Whether we should print packet headers
  //--------------------------------------------------
  boost::shared_ptr<vector<unsigned short> > pvMCPrintHdrChnl = m_SysCfg->GetMcastRecvrPrintPktHdr();
  if ((*pvMCPrintHdrChnl)[m_Identifier.Channel_ID()] == 1) m_PrintPktHdr = true;

  // Create the socket so that multiple may be bound to the same address.
  boost::asio::ip::udp::endpoint listen_endpoint(listen_address, multicast_port);
  m_Socket.open(listen_endpoint.protocol());
  m_Socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
  m_Socket.bind(listen_endpoint);

  // Join the multicast group.
  m_Socket.set_option(
      boost::asio::ip::multicast::join_group(multicast_address.to_v4(), boost::asio::ip::address::from_string("10.55.16.29").to_v4()));

  m_WritePtr = m_RawPktCirBuf->GetWritingPtr();
  m_Socket.async_receive_from(
      boost::asio::buffer(m_WritePtr, m_MaxSize),
      m_SenderEndpoint,
      boost::bind(&McastReceiver::handle_receive_from, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void McastReceiver::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
{
  if (!error)
  {
    if (m_PrintPktHdr)
    {
      m_Logger->Write(Logger::DEBUG, "MulticastReceiver: Packet size:  %u", (unsigned short)(READ_UINT16(m_WritePtr  )));
      m_Logger->Write(Logger::DEBUG, "MulticastReceiver: Msg count:    %u", (unsigned short)(READ_UINT8 (m_WritePtr+2)));
      m_Logger->Write(Logger::DEBUG, "MulticastReceiver: Seq No:       %u", (unsigned int  )(READ_UINT32(m_WritePtr+4)));
      m_Logger->Write(Logger::DEBUG, "MulticastReceiver: Send Time:    %u", (unsigned long )(READ_UINT64(m_WritePtr+8)));
    }

    m_RawPktCirBuf->PushBack(bytes_recvd);

    m_WritePtr = m_RawPktCirBuf->GetWritingPtr();

    if (!m_ShrObj->ThreadShouldExit())
    {
      m_Socket.async_receive_from(
          boost::asio::buffer(m_WritePtr, m_MaxSize),
          m_SenderEndpoint,
          boost::bind(&McastReceiver::handle_receive_from, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
      m_Socket.close();
    }
  }
}

McastReceiver::~McastReceiver()
{
}

