#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "Logger/Logger.h"
#include "ChannelAggregation/ChannelController.h"
#include "ChannelAggregation/DataCompletenessInspector.h"
#include "DataProcessing/DataProcController.h"
#include "SystemConfig.h"

#include "MainThread.h"

using namespace std;

int main(int argc, const char* argv[])
{
  //--------------------------------------------------
  // Init the singleton instances first
  //--------------------------------------------------
  shared_ptr<Logger>          pLogger   =  Logger::Instance();
  shared_ptr<SystemConfig>    pSysCfg   =  SystemConfig::Instance();
  shared_ptr<SharedObjects>   pShrObj   =  SharedObjects::Instance();
  //--------------------------------------------------

  if (argc > 1)
  {
    //--------------------------------------------------
    // Print help
    //--------------------------------------------------
    if (argv[1][0]=='h')
    {
      cout << "NAME:   Cash AT OMD-C" << endl;
      cout << "USAGE:  omdc                 , use the default Config.ini" << endl;
      cout << "        omdc /path/to/config , use the specified Config.ini" << endl;
      cout << "        omdc STest 2.1       , use the config file for scenario test 2.1" << endl;
    }
    //--------------------------------------------------
    // Scenario test
    //--------------------------------------------------
    else if (strncmp(argv[1],"STest",5)==0)
    {
      //--------------------------------------------------
      // Load the default config file first
      //--------------------------------------------------
      string sConfigPath("Config.ini");
      pSysCfg->ReadConfig(sConfigPath);

      if (strncmp(argv[2],"1.",2) == 0)
      {
        ostringstream ossConfigFile;
        ossConfigFile << "Testing/Config-HKExRTS" << string(argv[2]) << ".ini";
        pSysCfg->ReadConfigOptional(ossConfigFile.str());
      }
      else if (strncmp(argv[2],"2.",2) == 0)
      {
        ostringstream ossConfigFile;
        ossConfigFile << "Testing/Config-HKExArb" << string(argv[2]) << ".ini";
        pSysCfg->ReadConfigOptional(ossConfigFile.str());
      }
      else if (strncmp(argv[2],"5.",2) == 0)
      {
        ostringstream ossConfigFile;
        ossConfigFile << "Testing/Config-Readiness" << string(argv[2]) << ".ini";
        pSysCfg->ReadConfigOptional(ossConfigFile.str());
      }
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
  else
  {
    string sConfigPath("Config.ini"); // using default Config path
    pSysCfg->ReadConfig(sConfigPath);

  }

  //--------------------------------------------------
  // Start threads through MainThread
  //--------------------------------------------------
  MainThread mainthd;
  mainthd.RunMainThread();
  return 0;

}
