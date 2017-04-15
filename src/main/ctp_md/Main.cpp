#include <boost/cstdint.hpp>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "CtpMd.h"

using namespace std;
using namespace boost;

void ReadConfig(const string & sConfigPath, boost::shared_ptr<CtpMd> p_ctpmd)
{
  boost::shared_ptr<StdStreamLogger> m_Logger = StdStreamLogger::Instance();
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(sConfigPath, pt);

  string sLogLevel = STool::Trim(pt.get<std::string>("General.LogLevel"));
  if      (sLogLevel == "EMERGENCY") { m_Logger->SetLogLevel(StdStreamLogger::EMERGENCY); }
  else if (sLogLevel == "ALERT")     { m_Logger->SetLogLevel(StdStreamLogger::ALERT);     }
  else if (sLogLevel == "CRITICAL")  { m_Logger->SetLogLevel(StdStreamLogger::CRITICAL);  }
  else if (sLogLevel == "ERROR")     { m_Logger->SetLogLevel(StdStreamLogger::ERROR);     }
  else if (sLogLevel == "WARNING")   { m_Logger->SetLogLevel(StdStreamLogger::WARNING);   }
  else if (sLogLevel == "NOTICE")    { m_Logger->SetLogLevel(StdStreamLogger::NOTICE);    }
  else if (sLogLevel == "INFO")      { m_Logger->SetLogLevel(StdStreamLogger::INFO);      }
  else if (sLogLevel == "DEBUG")     { m_Logger->SetLogLevel(StdStreamLogger::DEBUG);     }

  m_Logger->Write(StdStreamLogger::INFO,"Reading Config file: %s", sConfigPath.c_str());
  m_Logger->Write(StdStreamLogger::INFO,"LogLevel: %s", sLogLevel.c_str());

  string sDataFolder = pt.get<std::string>("General.DataFolder");
  m_Logger->Write(StdStreamLogger::INFO,"DataFolder: %s", sDataFolder.c_str());
  string sThostLibFolder = pt.get<std::string>("General.ThostLibFolder");
  m_Logger->Write(StdStreamLogger::INFO,"ThostLibFolder: %s", sThostLibFolder.c_str());


    // p_ctpmd->setConnectString(const string &);
    // p_ctpmd->setBrokerID(const int);
    // p_ctpmd->setInvestorID(const string &);
    // p_ctpmd->setPassword(const string &);


}

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    cout << "Usage: " << argv[0] << " [config_file]" << endl << flush;
    return 0;
  }

  boost::shared_ptr<CtpMd> p_ctpmd;
  ReadConfig(argv[1],p_ctpmd);

  p_ctpmd->detach();

  sleep(10000);

  return 0;
}
