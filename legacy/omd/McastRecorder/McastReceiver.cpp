/*
 * McastReceiver.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: sunny
 */

#include "McastReceiver.h"



McastReceiver::McastReceiver(
    boost::asio::io_service& io_service,
    const string & sName,
    const boost::asio::ip::address& multicast_address,
    const short multicast_port,
    boost::posix_time::ptime progstarttime,
    const char * sDataFolder)
: m_Socket(io_service),
  m_Name(sName),
  m_CannedFileSimFmt(NULL),
  m_CannedFileCust(NULL),
  m_ProgramStartTime(progstarttime)
{

  string sFileSimFmt = string(sDataFolder) + "/simfmt_" + m_Name + "_" + to_iso_string(m_ProgramStartTime).substr(0,15);
  string sFileCust   = string(sDataFolder) + "/cusfmt_" + m_Name + "_" + to_iso_string(m_ProgramStartTime).substr(0,15);

  m_CannedFileSimFmt  =  fopen(sFileSimFmt.c_str(),  "w");
  m_CannedFileCust   = fopen(sFileCust.c_str(),    "w");

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

    {
      //--------------------------------------------------
      // HKEx Simulator format
      //--------------------------------------------------
      unsigned short iPktLen = *((uint16_t*)(&m_Buffer[0]));
      fwrite((&m_Buffer[0])+1,1,1,m_CannedFileSimFmt);
      fwrite(m_Buffer,1,1,m_CannedFileSimFmt);
      fwrite(m_Buffer,1,iPktLen,m_CannedFileSimFmt);
      fflush(m_CannedFileSimFmt);
    }
    {
      //--------------------------------------------------
      // Custom format:
      // 8 bytes - Relative time stamp in millisec
      // n bytes - The actual packet
      //--------------------------------------------------
      uint16_t uiPktSize = *((uint16_t*)&m_Buffer[0]);
      if (uiPktSize <= BUFFER_SIZE)
      {
        //--------------------------------------------------
        // Relative time in millisec
        //--------------------------------------------------
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff = now - m_ProgramStartTime;
        uint64_t uiRelTime = diff.total_milliseconds();
        fwrite(&uiRelTime,8,1,m_CannedFileCust);
        //--------------------------------------------------
        // Msg itself
        //--------------------------------------------------
        uint16_t uiPktSize = *((uint16_t*)&m_Buffer[0]);
        fwrite(m_Buffer,1,uiPktSize,m_CannedFileCust);

        fflush(m_CannedFileCust);
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
