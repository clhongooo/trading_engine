/*
 * Terminal.h
 *
 *  Created on: May 16, 2014
 *      Author: sunny
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include "SystemConfig.h"
#include "SharedObjects.h"
#include "STool.h"
#include "ChannelController.h"
#include "Logger.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>


using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

//--------------------------------------------------
// Terminal commands 
//--------------------------------------------------
static const char * CMD_Q         = "q"   ;
static const char * CMD_H         = "h"   ;
static const char * CMD_L         = "l"   ;
static const char * CMD_CAPTEST   = "ct"  ;
static const char * CMD_HELP      = "help";
static const char * CMD_LOGLEVEL  = "log" ;
static const char * CMD_RESET     = "rs"  ;
static const char * CMD_REFRESH   = "rf"  ;
static const char * CMD_SHUTDOWN  = "sd"  ;
static const char * CMD_LASTSEQNO = "lsn"  ;
static const char * CMD_CIRCUFDTL = "cbd"  ;

static const char * CMD_PS1 = "[user@omd]$ ";


class TerminalSession
{
  enum { SIZE = 1024 };
  public:
    TerminalSession(boost::asio::io_context&,ChannelController*);

    tcp::socket& socket() { return m_socket; }

    void start();
    void handle_read(const boost::system::error_code&, size_t);
    void handle_write(const boost::system::error_code&);
    void handle_write_donthg(const boost::system::error_code&);
    void output_tml_log_prompt();
    void output_tml_log();

  private:
    boost::shared_ptr<Logger>         m_Logger;
    boost::shared_ptr<SystemConfig>   m_SysCfg;
    boost::shared_ptr<SharedObjects>  m_ShrObj;
    tcp::socket                m_socket;
    boost::asio::io_context&   m_ios;
    char                       m_Buffer[SIZE];
    char                       m_LastCmd[SIZE];
    ChannelController *       m_pChnlCtrlr;
};

class TerminalServer
{
  public:
    TerminalServer(boost::asio::io_context&, short, ChannelController *);
    ~TerminalServer() {}

  private:
    void start_accept();
    void handle_accept(TerminalSession*, const boost::system::error_code&);

    boost::shared_ptr<Logger>           m_Logger;
    boost::shared_ptr<SharedObjects>    m_ShrObj;
    boost::asio::io_context&     io_context_;
    tcp::acceptor                acceptor_;
    ChannelController *          m_pChnlCtrlr;
    ptr_vector<TerminalSession>  m_vTmlSess;
};

class TerminalThread {
  public:
    TerminalThread(ChannelController *);
    virtual ~TerminalThread();
    void Run();

  private:
    boost::shared_ptr<SystemConfig>  m_SysCfg;
    unsigned short            m_SystemServicePort;
    ChannelController *       m_pChnlCtrlr;
};

#endif /* TERMINAL_H_ */
