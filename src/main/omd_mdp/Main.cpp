#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "Logger.h"
#include "ChannelController.h"
#include "DataCompletenessInspector.h"
#include "DataProcController.h"
#include "SystemConfig.h"

#include "MainThread.h"

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
      cout << "USAGE:  omd_mdp                 , use the default Config.ini" << endl;
      cout << "        omd_mdp /path/to/config , use the specified Config.ini" << endl;
      cout << "        omd_mdp STest 2.1       , use the config file for scenario test 2.1" << endl;
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
