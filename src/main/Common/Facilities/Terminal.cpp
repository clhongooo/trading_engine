#include "Terminal.h"


  TerminalSession::TerminalSession(boost::asio::io_service& io_service)
: m_socket(io_service),
  m_ios(io_service)
{
  m_Logger        = Logger::Instance();
  m_SysCfg        = SystemConfig::Instance();
  m_SysState      = SystemState::Instance();
  m_MarketData    = MarketData::Instance();
  m_PortAndOrders = PortfoliosAndOrders::Instance();

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
    if (strncmp(m_Buffer,CMD_L,strlen(CMD_L))==0 && strncmp(m_Buffer,CMD_LOGLEVEL,strlen(CMD_LOGLEVEL))!=0)
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
    if (strncmp(m_Buffer,CMD_H,strlen(CMD_H))==0)
    {

      sprintf(m_Buffer,"0       Emergency       System is unusable               "); output_tml_log();
      sprintf(m_Buffer,"1       Alert           Action must be taken immediately "); output_tml_log();
      sprintf(m_Buffer,"2       Critical        Critical conditions              "); output_tml_log();
      sprintf(m_Buffer,"3       Error           Error conditions                 "); output_tml_log();
      sprintf(m_Buffer,"4       Warning         Warning conditions               "); output_tml_log();
      sprintf(m_Buffer,"5       Notice          Normal but significant condition "); output_tml_log();
      sprintf(m_Buffer,"6       Informational   Informational messages           "); output_tml_log();
      sprintf(m_Buffer,"7       Debug           Debug-level messages             "); output_tml_log();

      sprintf(m_Buffer,"Supported commands:\nl (repeat last command)\nh (help)\nq (quit)\nlog[0-7]\nsd (shutdown)\n");
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
    // Shutdown
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_SHUTDOWN,strlen(CMD_SHUTDOWN))==0)
    {
      sprintf(m_Buffer, "TerminalSession: Nirvana will now shutdown. Stopping all threads.");
      output_tml_log_prompt();
      m_SysState->SetSystemState(SystemState::STATE_OFF);
      for (unsigned int i = 0; i < 3; ++i)
      {
        m_MarketData->NotifyConsumers(); // wake up other threads
        m_MarketData->NotifyConsumers(); // wake up other threads
        m_PortAndOrders->NotifyConsumers(); // wake up other threads
        sleep(1);
      }
      m_ios.stop();
      return;
    }

    //--------------------------------------------------
    // Log Level
    //--------------------------------------------------
    else if (strncmp(m_Buffer,CMD_LOGLEVEL,strlen(CMD_LOGLEVEL))==0 )
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

        default    :  { sprintf(m_Buffer, "TerminalSession: Unrecognized log level. The log level must range from 0 to 7.");                        break; }
      }

      output_tml_log_prompt();

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


  TerminalServer::TerminalServer(boost::asio::io_service& io_service, short port)
: io_service_(io_service),
  acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
  m_Logger   = Logger::Instance();
  m_SysState = SystemState::Instance();
  start_accept();
}

void TerminalServer::start_accept()
{
  TerminalSession* newTmlSess = new TerminalSession(io_service_);
  m_vTmlSess.push_back(newTmlSess);

  if (!m_SysState->ChkIfThreadShouldStop())
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
    newTmlSess = new TerminalSession(io_service_);
    m_vTmlSess.push_back(newTmlSess);
    if (!m_SysState->ChkIfThreadShouldStop())
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


TerminalThread::TerminalThread() :
  m_SystemServicePort(8000) //default
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
    TerminalServer s(io_service,m_SystemServicePort);
    io_service.run(); // Blocks here
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

}
