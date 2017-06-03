#include <boost/cstdint.hpp>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "ExpandableCirBuffer.h"
#include "ZMQServerClient.h"
#include "StdStreamLogger.h"
#include "ATU_IB_MDI.h"
#include "ATU_IB_OTI.h"
#include "BinaryTools.h"

using namespace std;
using namespace boost;

void ReadConfig(const string & sConfigPath, boost::shared_ptr<ATU_IB_MDI> p_ibmd, boost::shared_ptr<ATU_IB_OTI> p_ibord, boost::shared_ptr<ZMQServer> p_zmq_server_md, boost::shared_ptr<ZMQServer> p_zmq_server_ord, boost::shared_ptr<BinaryRecorder> p_BinaryRecorder)
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
  // ATU_IB_MDI
  //--------------------------------------------------
  p_ibmd->setPort     (pt.get<int>("MarketData.Port"));
  p_ibmd->setClientID (pt.get<int>("MarketData.ClientID"));
  p_ibmd->setIP               (STool::Trim(pt.get<std::string>("MarketData.IP")));
  p_ibmd->setAccount          (STool::Trim(pt.get<std::string>("MarketData.Account")));
  p_zmq_server_md->Set        (STool::Trim(pt.get<std::string>("MarketData.ZmqMdPort")));

  //--------------------------------------------------
  // ATU_IB_OTI
  //--------------------------------------------------
  p_ibord->setPort      (pt.get<int>("OrderRouting.Port"));
  p_ibord->setClientID  (pt.get<int>("OrderRouting.ClientID"));
  p_ibord->setOrderMappingFile(STool::Trim(pt.get<std::string>("OrderRouting.OrderMappingFile")));
  p_ibord->setIP              (STool::Trim(pt.get<std::string>("OrderRouting.IP")));
  p_ibord->setAccount         (STool::Trim(pt.get<std::string>("OrderRouting.Account")));
  p_zmq_server_ord->Set       (STool::Trim(pt.get<std::string>("OrderRouting.ZmqOrdPort")));

  //--------------------------------------------------
  // to ruin things
  //--------------------------------------------------
  if (STool::ToLowerCase(pt.get<std::string>("MarketData.IsON"))   != "true") p_ibmd->setIP  ("0.0.0.1");
  if (STool::ToLowerCase(pt.get<std::string>("OrderRouting.IsON")) != "true") p_ibord->setIP ("0.0.0.1");

  //--------------------------------------------------
  p_ibmd->readContracts(STool::Trim(pt.get<std::string>("General.ContractFile")));
  p_ibord->readContracts(STool::Trim(pt.get<std::string>("General.ContractFile")));
  p_ibord->readOrderMapping();

  //--------------------------------------------------
  vector<string> vSym;
  const string sSyms = pt.get<std::string>("General.SubscribeSymbols");
  boost::split(vSym,sSyms,boost::is_any_of(","));
  for (int i = 0; i < vSym.size(); i++)
  p_ibmd->on_process_subscription(vSym[i]);

  //--------------------------------------------------
  p_BinaryRecorder->SetOutFilePath(STool::Trim(pt.get<std::string>("MarketData.DataFolder"))+"/"+SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff()+".csv","w+");

  string sFlushOnEveryWrite = pt.get<std::string>("MarketData.FlushOnEveryWrite");
  boost::algorithm::to_lower(sFlushOnEveryWrite);
  if (sFlushOnEveryWrite == "true" || sFlushOnEveryWrite == "t" || sFlushOnEveryWrite == "yes" || sFlushOnEveryWrite == "y") p_BinaryRecorder->SetFlushOnEveryWrite(true);

  //--------------------------------------------------
  string wdtf2 = STool::Trim(pt.get<std::string>("MarketData.WriteDataToFile"));
  boost::algorithm::to_lower(wdtf2);
  bool b_WriteDataToFile = false;
  if (wdtf2 == "true" || wdtf2 == "t" || wdtf2 == "yes" || wdtf2 == "y") b_WriteDataToFile = true;
  if (b_WriteDataToFile && !p_BinaryRecorder->EnableWriter())
  {
    m_Logger->Write(StdStreamLogger::ERROR,"File cannot be opened. Exiting.");
    exit(1);
  }
  if (b_WriteDataToFile) m_Logger->Write(StdStreamLogger::INFO,"Write data to file = true");
  else                   m_Logger->Write(StdStreamLogger::INFO,"Write data to file = false");
  //--------------------------------------------------


}

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " [config_file]" << endl << flush;
    return 0;
  }

  boost::shared_ptr<StdStreamLogger> m_Logger = StdStreamLogger::Instance();
  //--------------------------------------------------
  boost::shared_ptr<ZMQServer> p_zmq_server_md (new ZMQServer(ZMQ_PUB));
  boost::shared_ptr<ZMQServer> p_zmq_server_ord(new ZMQServer(ZMQ_PAIR));
  //--------------------------------------------------
  boost::shared_ptr<CentralMemMgr> cmm = CentralMemMgr::Instance();
  boost::shared_ptr<ExpandableCirBuffer> p_ecbMD (new ExpandableCirBuffer(1,4096,sizeof(ATU_MDI_marketfeed_struct)+3,cmm));
  boost::shared_ptr<ExpandableCirBuffer> p_ecbOrd(new ExpandableCirBuffer(1,4096,4096,cmm));
  //--------------------------------------------------
  boost::shared_ptr<ATU_IB_MDI>  p_ibmd (new ATU_IB_MDI(p_ecbMD));
  boost::shared_ptr<ATU_IB_OTI> p_ibord(new ATU_IB_OTI(p_ecbOrd));
  //--------------------------------------------------
  boost::shared_ptr<BinaryRecorder> p_BinaryRecorder(new BinaryRecorder());
  //--------------------------------------------------
  ReadConfig(argv[1], p_ibmd, p_ibord, p_zmq_server_md, p_zmq_server_ord, p_BinaryRecorder);
  //--------------------------------------------------

  boost::scoped_ptr<boost::thread> m_p_run_md_thread (new boost::thread(boost::bind(&ATU_IB_MDI::run,p_ibmd.get())));
  boost::scoped_ptr<boost::thread> m_p_run_ord_thread(new boost::thread(boost::bind(&ATU_IB_OTI::run,p_ibord.get())));

  bool bBusyLoop = true;
  BYTE * pb = NULL;
  unsigned long ulTStamp;
  for (;;)
  {
    //--------------------------------------------------
    // always give priority to order routing
    //--------------------------------------------------
    if (!p_ecbOrd->EmptyNoLock())
    {
      while (p_ecbOrd->GetReadingPtrTStamp(pb,&ulTStamp))
      {
        const string sFeed((char*)pb);
        p_ecbOrd->PopFront();
        p_zmq_server_ord->Send(sFeed);
        m_Logger->Write(StdStreamLogger::INFO,"Sent to zmq: %s", sFeed.c_str());
        bBusyLoop = false;
      }
    }
    //--------------------------------------------------
    Tuple2<bool,string> tup = p_zmq_server_ord->GetStr();
    while (tup._1())
    {
      m_Logger->Write(StdStreamLogger::INFO,"Received from zmq: %s", tup._2().c_str());
      ATU_OTI_signalfeed_struct sfs;
      if (ATU_OTI_signalfeed_struct::ParseString(tup._2(),sfs))
        p_ibord->on_process_signalfeed(sfs);
      tup = p_zmq_server_ord->GetStr();
      bBusyLoop = false;
    }

    //--------------------------------------------------
    // process market data with spare time
    //--------------------------------------------------
    if (!p_ecbMD->EmptyNoLock())
    {
      while (p_ecbMD->GetReadingPtrTStamp(pb,&ulTStamp))
      {
        const ATU_MDI_marketfeed_struct* mfs = (ATU_MDI_marketfeed_struct*)pb;
        p_BinaryRecorder->WriteATUMDIStruct(*mfs);
        p_zmq_server_md->Send(ATU_MDI_marketfeed_struct::ToString(*mfs));
        p_ecbMD->PopFront();
        bBusyLoop = false;

        if (!p_ecbOrd->Empty() || max(p_zmq_server_ord->GetSendQueueSize(),p_zmq_server_ord->GetRecvQueueSize()) > 0) break;
      }
    }

    //--------------------------------------------------
    // prevent busy loop
    //--------------------------------------------------
    if (bBusyLoop) boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    else           bBusyLoop = true;
  }

  //--------------------------------------------------
  // Won't reach here anyway
  //--------------------------------------------------
  m_p_run_md_thread->join();
  m_p_run_ord_thread->join();

  return 0;
}
