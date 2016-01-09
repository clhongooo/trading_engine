#include "MarketFeedCondensor.h"

void MarketFeedCondensor::Condense(const char * sInfile,const char * sOutfile,const char *sSec)
{
  //
  // int iSnapshotSec = boost::lexical_cast<int>(string(sSec));
  //
  // ifstream inFileStrm(sInfile,std::ifstream::in);
  // ofstream outFileStrm(sOutfile,std::ifstream::out);
  //
  // if (inFileStrm.good())
  // {
  //
  //   string sLine;
  //   int iDateTimeLen = 0;
  //
  //   for (;;)
  //   {
  //
  //     //--------------------------------------------------
  //     // 26 columns
  //     //--------------------------------------------------
  //
  //     bool bRtn = getline(inFileStrm, sLine, '\n');
  //     if (!bRtn) break;
  //
  //     boost::char_separator<char> sep(",");
  //     boost::tokenizer<boost::char_separator<char> > tokens(sLine, sep);
  //
  //     tokenizer<char_separator<char> >::iterator it = tokens.begin();
  //
  //     int iHHMMSS = 0;
  //     iDateTimeLen = (*it).length();
  //     if      (iDateTimeLen == 6 ) iHHMMSS = boost::lexical_cast<int>(*it);
  //     else if (iDateTimeLen == 15) iHHMMSS = boost::lexical_cast<int>((*it).substr(9,6));
  //     else if (iDateTimeLen == 22) iHHMMSS = boost::lexical_cast<int>((*it).substr(9,6));
  //
  //     HHMMSS hmsTimeNow(iHHMMSS);
  //     it++;
  //     m_Latest[*it] = sLine;
  //
  //     if (m_LastOutputTime == HHMMSS::BAD_TIME) m_LastOutputTime.Set(iHHMMSS);
  //
  //     if (abs(hmsTimeNow - m_LastOutputTime) >= iSnapshotSec)
  //     {
  //       for (map<string,string>::iterator it = m_Latest.begin(); it != m_Latest.end(); ++it)
  //       {
  //         if (iDateTimeLen == 6)
  //         {
  //           outFileStrm << hmsTimeNow.ToStr();
  //         }
  //         else if (iDateTimeLen == 15)
  //         {
  //           outFileStrm << sLine.substr(0,9) << hmsTimeNow.ToStr();
  //         }
  //         else if (iDateTimeLen == 22)
  //         {
  //           outFileStrm << sLine.substr(0,9) << hmsTimeNow.ToStr() << "_000000";
  //         }
  //
  //         outFileStrm << (it->second).substr(iDateTimeLen,(it->second).length()-iDateTimeLen);
  //
  //         outFileStrm << endl;
  //       }
  //       m_Latest.clear();
  //       m_LastOutputTime.Set(hmsTimeNow);
  //     }
  //
  //   }
  //
  //   inFileStrm.close();
  //   outFileStrm.close();
  //
  // }
  // else
  // {
  //   cerr << "Unable to open " << string(sInfile);
  // }
  //
  // return;
}

