//**************************************************
//  Author:      Sunny Yan
//  Created On:  Tue May 20 13:49:52 HKT 2014
//  Description: Minimalist Multicast Recorder
//
//**************************************************

#include "McastReceiver.h"
#include <boost/cstdint.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

void ReadConfig(const char *sConfigFile, deque<string> & dqIP, deque<unsigned short> & dqPort, deque<string> & dqLabel)
{
  ifstream file (sConfigFile);
  if (file.good())
  {
    string sLine;

    for (;;)
    {
      if (!getline(file, sLine, '\n')) return;
      if (sLine.at(0) == '#') continue;

      vector<string> vFields;
      boost::split(vFields,sLine,boost::is_any_of(","));
      if (vFields.size() != 3) continue;

      dqIP.push_back(vFields[0]);
      dqPort.push_back(lexical_cast<unsigned short>(vFields[1]));
      dqLabel.push_back(vFields[2]);
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

  if (argc != 4)
  {
    cout << "Usage: " << argv[0] << " [config_file] [dest_data_folder] [output_format]" << endl << flush;
    cout << "       output_format:" << endl << flush;
    cout << "       1: HKEx simulator." << endl << flush;
    cout << "       2: 8 byte milliseconds since program start + original UDP packet." << endl << flush;
    cout << "       3: 8 byte unix time + original UDP packet." << endl << flush;
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
    short iMode = boost::lexical_cast<int>(argv[3]);
    if (iMode == 1)
    {
      //--------------------------------------------------
      // HKEx Simulator format
      //--------------------------------------------------
      dqMCR.push_back(new McastReceiverHKExSim(ios,
                                               dqLabel[i],
                                               boost::asio::ip::address::from_string(dqIP[i]),
                                               dqPort[i],
                                               ptProgramStartTime,
                                               argv[2],
                                               iMode));
    }
    else if (iMode == 2)
    {
      //--------------------------------------------------
      // Relative time in millisec
      //--------------------------------------------------
      dqMCR.push_back(new McastReceiverRelTime(ios,
                                               dqLabel[i],
                                               boost::asio::ip::address::from_string(dqIP[i]),
                                               dqPort[i],
                                               ptProgramStartTime,
                                               argv[2],
                                               iMode));
    }
    else if (iMode == 3)
    {
      //--------------------------------------------------
      // Unix time in millisec
      //--------------------------------------------------
      dqMCR.push_back(new McastReceiverUnixTime(ios,
                                                dqLabel[i],
                                                boost::asio::ip::address::from_string(dqIP[i]),
                                                dqPort[i],
                                                ptProgramStartTime,
                                                argv[2],
                                                iMode));
    }
  }

  ios.run();

  cout << argv[0] << " exits." << endl << flush;
  return 0;
}
