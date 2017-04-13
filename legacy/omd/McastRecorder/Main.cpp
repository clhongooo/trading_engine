//**************************************************
//  Author:      Sunny Yan
//  Created On:  Tue May 20 13:49:52 HKT 2014
//  Description: Minimalist Multicast Recorder
//
//**************************************************

#include "McastReceiver.h"
#include <boost/cstdint.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <iostream>
#include <memory>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

using namespace std;
using namespace boost;

void ReadConfig(const char *sConfigFile, deque<string> & dqIP, deque<unsigned short> & dqPort, deque<string> & dqLabel)
{
  ifstream file (sConfigFile);
  if (file.good())
  {
    string sValue;

    for (;;)
    {
      if (!getline(file, sValue, ',')) return;
      if (sValue.at(0) == '#')
      {
        getline(file, sValue, ',');
        getline(file, sValue, '\n');
        continue;
      }
      dqIP.push_back(string(sValue));

      if (!getline(file, sValue, ',')) return;
      dqPort.push_back(lexical_cast<unsigned short>(sValue));

      if (!getline(file, sValue, '\n')) return;
      dqLabel.push_back(string(sValue));
    }
    file.close();

  }
  else cout << "Unable to open file";
  return;
}


int main(int argc, const char *argv[])
{
  boost::posix_time::ptime ptProgramStartTime = boost::posix_time::microsec_clock::local_time();

  boost::asio::io_service ios;
  deque<string> dqIP;
  deque<unsigned short> dqPort;
  deque<string> dqLabel;
  ptr_deque<McastReceiver> dqMCR;

  if (argc != 3)
  {
    cout << "Please provide the [path to the config file] and [path to the data folder] as the parameters." <<  endl << flush;
    return 0;
  }

  ReadConfig(argv[1],dqIP,dqPort,dqLabel);

  if (dqIP.size() != dqPort.size() || dqPort.size() != dqLabel.size())
  {
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") Config file error. Please check." << endl << flush;
    return 0;
  }

  for (unsigned int i = 0; i < dqIP.size(); ++i)
  {
    cout << "Starting McastReceiver : " << dqLabel[i] << endl;

    dqMCR.push_back(
        new McastReceiver(
          ios,
          dqLabel[i],
          boost::asio::ip::address::from_string(dqIP[i]),
          dqPort[i],
          ptProgramStartTime,
          argv[2]));

  }

  ios.run();

  return 0;
}
