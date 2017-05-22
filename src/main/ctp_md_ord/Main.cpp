#include <boost/cstdint.hpp>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "StdStreamLogger.h"
#include "CtpMd.h"
#include "CtpOrd.h"

using namespace std;
using namespace boost;

void ReadConfig(const string & sConfigPath, boost::shared_ptr<CtpMd> p_ctpmd, boost::shared_ptr<CtpOrd> p_ctpord)
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

  //--------------------------------------------------
  // CtpMd
  //--------------------------------------------------
  p_ctpmd->setDataFolder     (STool::Trim(pt.get<std::string>("MarketData.DataFolder")));
  p_ctpmd->setWriteDataToFile(STool::Trim(pt.get<std::string>("MarketData.WriteDataToFile")));
  p_ctpmd->setConnectString  (STool::Trim(pt.get<std::string>("MarketData.ConnectionString")));
  p_ctpmd->setBrokerID       (STool::Trim(pt.get<std::string>("MarketData.BrokerID")));
  p_ctpmd->setInvestorID     (STool::Trim(pt.get<std::string>("MarketData.UserID")));
  p_ctpmd->setPassword       (            pt.get<std::string>("MarketData.Password"));

  string sFlushOnEveryWrite = pt.get<std::string>("MarketData.FlushOnEveryWrite");
  boost::algorithm::to_lower(sFlushOnEveryWrite);
  if (sFlushOnEveryWrite == "true" || sFlushOnEveryWrite == "t" || sFlushOnEveryWrite == "yes" || sFlushOnEveryWrite == "y") p_ctpmd->SetFlushOnEveryWrite(true);

  //--------------------------------------------------
  // CtpOrd
  //--------------------------------------------------
  p_ctpord->setConnectString  (STool::Trim(pt.get<std::string>("OrderRouting.ConnectionString")));
  p_ctpord->setBrokerID       (STool::Trim(pt.get<std::string>("OrderRouting.BrokerID")));
  p_ctpord->setInvestorID     (STool::Trim(pt.get<std::string>("OrderRouting.UserID")));
  p_ctpord->setPassword       (            pt.get<std::string>("OrderRouting.Password"));

  //--------------------------------------------------
  vector<string> vSym;
  const string sSyms = pt.get<std::string>("General.SubscribeSymbols");
  boost::split(vSym,sSyms,boost::is_any_of(","));
  p_ctpmd->setSubscribeSymbols<vector<string> >(vSym);
}


int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " [config_file]" << endl << flush;
    return 0;
  }

  boost::shared_ptr<CtpMd>  p_ctpmd (new CtpMd());
  boost::shared_ptr<CtpOrd> p_ctpord(new CtpOrd());
  ReadConfig(argv[1], p_ctpmd, p_ctpord);

  boost::scoped_ptr<boost::thread> m_p_run_md_thread (new boost::thread(boost::bind(&CtpMd::run,p_ctpmd.get())));
  boost::scoped_ptr<boost::thread> m_p_run_ord_thread(new boost::thread(boost::bind(&CtpOrd::run,p_ctpord.get())));
  m_p_run_md_thread->join();
  m_p_run_ord_thread->join();

  return 0;
}
