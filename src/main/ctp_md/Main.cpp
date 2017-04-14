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

using namespace std;
using namespace boost;

void ReadConfig(const string & sConfigPath)
{
  cout << "Reading Config file: " << sConfigPath << endl;

  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(sConfigPath, pt);

  string sDataFolder = pt.get<std::string>("General.DataFolder");
  cout << "DataFolder: " << sDataFolder << endl;
  string sThostLibFolder = pt.get<std::string>("General.ThostLibFolder");
  cout << "ThostLibFolder: " << sThostLibFolder << endl;
}

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    cout << "Usage: " << argv[0] << " [config_file]" << endl << flush;
    return 0;
  }

  ReadConfig(argv[1]);

  return 0;
}
