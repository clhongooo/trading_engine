#include "MarketFeedMerger.h"

void MarketFeedMerger::Merge(const char * sInfile1,const char * sInfile2,const char * sOutfile)
{
  //
  // ifstream inFileStrm1(sInfile1,std::ifstream::in);
  // ifstream inFileStrm2(sInfile2,std::ifstream::in);
  // ofstream outFileStrm(sOutfile,std::ifstream::out);
  //
  // if (inFileStrm1.good() && inFileStrm2.good())
  // {
  //
  //   string sLine1;
  //   string sLine2;
  //
  //   vector<string> vCsvFields1;
  //   vector<string> vCsvFields2;
  //
  //   bool bRtn1 = getline(inFileStrm1, sLine1, '\n');
  //   bool bRtn2 = getline(inFileStrm2, sLine2, '\n');
  //   for (;;)
  //   {
  //     //--------------------------------------------------
  //     // 26 columns
  //     //--------------------------------------------------
  //     while (bRtn1 && !bRtn2)
  //     {
  //       outFileStrm << sLine1 << endl;
  //       bRtn1 = getline(inFileStrm1, sLine1, '\n');
  //     }
  //
  //     while (!bRtn1 && bRtn2)
  //     {
  //       outFileStrm << sLine2 << endl;
  //       bRtn2 = getline(inFileStrm2, sLine2, '\n');
  //     }
  //
  //     if (!bRtn1 && !bRtn2) return;
  //
  //     vCsvFields1.clear();
  //     vCsvFields2.clear();
  //     boost::split(vCsvFields1, sLine1, boost::is_any_of(","));
  //     boost::split(vCsvFields2, sLine2, boost::is_any_of(","));
  //
  //     if (vCsvFields1.empty() || vCsvFields2.empty()) return;
  //
  //     if (vCsvFields1[0].length() < 6) return;
  //     if (vCsvFields1[0].length() != vCsvFields2[0].length()) return;
  //     if (vCsvFields1[0].length() == 6)
  //     {
  //       //--------------------------------------------------
  //       // HHMMSS
  //       //--------------------------------------------------
  //       int iHHMMSS1 = boost::lexical_cast<int>(vCsvFields1[0]);
  //       int iHHMMSS2 = boost::lexical_cast<int>(vCsvFields2[0]);
  //       if (iHHMMSS1 <= iHHMMSS2)
  //       {
  //         outFileStrm << sLine1 << endl;
  //         bRtn1 = getline(inFileStrm1, sLine1, '\n');
  //       }
  //       else
  //       {
  //         outFileStrm << sLine2 << endl;
  //         bRtn2 = getline(inFileStrm2, sLine2, '\n');
  //       }
  //     }
  //   }
  //   inFileStrm1.close();
  //   inFileStrm2.close();
  //   outFileStrm.close();
  //
  // }
  // else
  // {
  //   cerr << "Unable to open " << string(sInfile1);
  //   cerr << "Unable to open " << string(sInfile2);
  // }
  //
  // return;
}

