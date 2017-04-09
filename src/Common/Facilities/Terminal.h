#ifndef TERMINAL_H_
#define TERMINAL_H_

#include "PCH.h"
#include "Constants.h"
#include <string>
#include <vector>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "SystemConfig.h"
#include "SystemState.h"
#include "MarketData.h"
#include "Logger.h"
#include "PortfoliosAndOrders.h"

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

//--------------------------------------------------
// Terminal commands 
//--------------------------------------------------
static const char * CMD_Q         = "q"   ;
static const char * CMD_H         = "h"   ;
static const char * CMD_L         = "l"   ;
static const char * CMD_LOGLEVEL  = "log" ;
static const char * CMD_SHUTDOWN  = "sd"  ;

static const char * CMD_PS1 = "[user@nirvana]$ ";


class TerminalSession
{
  enum { SIZE = 1024 };
  public:
    TerminalSession(boost::asio::io_service&);

    tcp::socket& socket() { return m_socket; }

    void start();
    void handle_read(const boost::system::error_code&, size_t);
    void handle_write(const boost::system::error_code&);
    void handle_write_donthg(const boost::system::error_code&);
    void output_tml_log_prompt();
    void output_tml_log();

  private:
    boost::shared_ptr<Logger>               m_Logger;
    boost::shared_ptr<SystemConfig>         m_SysCfg;
    boost::shared_ptr<SystemState>          m_SysState;
    boost::shared_ptr<MarketData>           m_MarketData;
    boost::shared_ptr<PortfoliosAndOrders>  m_PortAndOrders;

    tcp::socket                      m_socket;
    boost::asio::io_service&         m_ios;
    char                             m_Buffer[SIZE];
    char                             m_LastCmd[SIZE];
};

class TerminalServer
{
  public:
    TerminalServer(boost::asio::io_service&, short);
    ~TerminalServer() {}

  private:
    void start_accept();
    void handle_accept(TerminalSession*, const boost::system::error_code&);

    boost::shared_ptr<Logger>           m_Logger;
    boost::shared_ptr<SystemState>      m_SysState;
    boost::asio::io_service&     io_service_;
    tcp::acceptor                acceptor_;
    ptr_vector<TerminalSession>  m_vTmlSess;
};

class TerminalThread {
  public:
    TerminalThread();
    virtual ~TerminalThread();
    void Run();

  private:
    boost::shared_ptr<SystemConfig>  m_SysCfg;
    unsigned short            m_SystemServicePort;
};

#endif /* TERMINAL_H_ */
