#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "Logger.h"
#include "ChannelController.h"
#include "DataCompletenessInspector.h"
#include "DataProcController.h"
#include "SystemConfig.h"

#include "OMD_MainThread.h"

using namespace std;

int main(int argc, const char* argv[])
{
  //--------------------------------------------------
  // Init the singleton instances first
  //--------------------------------------------------
  boost::shared_ptr<Logger>          pLogger   =  Logger::Instance();
  boost::shared_ptr<SystemConfig>    pSysCfg   =  SystemConfig::Instance();
  boost::shared_ptr<SharedObjects>   pShrObj   =  SharedObjects::Instance();
  //--------------------------------------------------

  if (argc > 1)
  {
    //--------------------------------------------------
    // Print help
    //--------------------------------------------------
    if (argv[1][0]=='h')
    {
      cout << "NAME:   CME MDP 3.0 client" << endl;
      cout << "USAGE:" << endl;
      cout << "        cme_mdp /path/to/config" << endl;
    }
    //--------------------------------------------------
    // Using the specified Config.ini
    //--------------------------------------------------
    else
    {
      string sConfigPath(argv[1]);
      pSysCfg->ReadConfig(sConfigPath);
    }
  }

  //--------------------------------------------------
  // Start threads through MainThread
  //--------------------------------------------------
  MainThread mainthd;
  mainthd.RunMainThread();
  return 0;

}
