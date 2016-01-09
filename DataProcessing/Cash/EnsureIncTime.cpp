#include "EnsureIncTime.h"

void EnsureIncTime::Convert(const char * sInfile,const char * sOutfile)
{
  unsigned long ulLastHHMMSS=0;

  ifstream inFileStrm (sInfile,std::ifstream::in);
  ofstream outFileStrm(sOutfile,std::ifstream::out);

  if (inFileStrm.good())
  {
    string sValue;
    string sHHMMSS;
    unsigned long ulHHMMSS=0;

    for (;;)
    {
      //--------------------------------------------------
      // 26 columns
      //--------------------------------------------------
      if (!getline(inFileStrm, sValue, '\n')) return;
      sHHMMSS = sValue.substr(0,6);
      ulHHMMSS = lexical_cast<unsigned long>(sHHMMSS);

      if (ulLastHHMMSS==0 || (ulLastHHMMSS!=0 && ulHHMMSS >= ulLastHHMMSS))
      {
        outFileStrm << sValue << endl;
      }
      if (ulHHMMSS > ulLastHHMMSS) ulLastHHMMSS = ulHHMMSS;

    }
    inFileStrm.close();

  }
  else cerr << "Unable to open " << string(sInfile);

  return;
}

