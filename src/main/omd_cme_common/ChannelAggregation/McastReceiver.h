/*
 * McastReceiver.h
 *
 *  Created on: Apr 9, 2014
 *      Author: sunny
 */

#ifndef MCASTRECEIVER_H_
#define MCASTRECEIVER_H_

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/shared_ptr.hpp>
#include "BinaryTools.h"
#include "McastIdentifier.h"
#include "Logger.h"
#include "SharedObjects.h"
#include "SystemConfig.h"

using namespace std;

class McastReceiver
{
  public:
    McastReceiver(
        boost::asio::io_service&,
        const boost::asio::ip::address&,
        const boost::asio::ip::address&,
        const short,
        const short,
        const McastIdentifier &);
    virtual ~McastReceiver();

    void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);

  private:
    boost::asio::ip::udp::socket m_Socket;
    boost::asio::ip::udp::endpoint m_SenderEndpoint;
    const short m_MaxSize;
    //-----------------------------------
    const McastIdentifier &          m_Identifier;
    boost::shared_ptr<Logger>        m_Logger;
    boost::shared_ptr<SharedObjects> m_ShrObj;
    boost::shared_ptr<SystemConfig>  m_SysCfg;
    ExpandableCirBuffer *            m_RawPktCirBuf;
    BYTE *                           m_WritePtr;
    bool                             m_PrintPktHdr;
};

#endif /* MCASTRECEIVER_H_ */
