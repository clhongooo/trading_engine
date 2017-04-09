#include "MarketFeedToOHLC.h"

MarketFeedToOHLC::MarketFeedToOHLC() {
}

MarketFeedToOHLC::~MarketFeedToOHLC() {
}

void MarketFeedToOHLC::Convert(const char * sInfile,const char * sOutfile,const char * sVolMode)
{
  int iLastHour = 0;
  int iLastMinute = 0;
  double dOpen = 0;
  double dClose = 0;
  double dVol1 = 0;
  double dVol2 = 0;
  set<double> setTradeTick;

  bool bCumVolMode = true;

  if (sVolMode[0] == 'C') bCumVolMode = true;
  if (sVolMode[0] == 'D') bCumVolMode = false;

  ifstream inFileStrm (sInfile,std::ifstream::in);
  ofstream outFileStrm(sOutfile,std::ifstream::out);

  string strInfile(sInfile);

  if (inFileStrm.good())
  {
    string sValue;

    for (;;)
    {
      //--------------------------------------------------
      // Parse data
      // 094504,HSIJ0,21306,267
      //--------------------------------------------------
      if (!getline(inFileStrm, sValue, ',')) return; //Time

      unsigned long ulYYYYMMDD = 0;
      unsigned long ulHHMMSS = 0;
      if (sValue.length() > 6 && sValue.at(0) == '2') //e.g. 20111231_093000 or 20111231_093000_000000
      {
        ulYYYYMMDD = lexical_cast<unsigned long>(sValue.substr(0,8));
        string sub = (sValue.at(9) == '0' ? sValue.substr(10,5) : sValue.substr(9,6));
        ulHHMMSS = lexical_cast<unsigned long>(sub);
      }
      else if (sValue.length() == 6) //e.g. 093000
      {
        string sub = (sValue.at(0) == '0' ? sValue.substr(1,5) : sValue);
        ulHHMMSS = lexical_cast<unsigned long>(sub);
      }

      int iThisHour = ulHHMMSS / 10000;
      int iThisMinute = (ulHHMMSS % 10000) / 100;

      if (!getline(inFileStrm, sValue, ',')) return; //Symbol
      string sSym = sValue;
      if (!getline(inFileStrm, sValue, ',')) return; //Price
      double dPrice = lexical_cast<double>(sValue);

      if (!getline(inFileStrm, sValue, '\n')) return; //Volume
      vector<string> vs;
      boost::split(vs, sValue, boost::is_any_of(","));
      double dVolume = lexical_cast<double>(vs[0]);

      if (iLastHour == 0 && iLastMinute == 0)
      {
        //--------------------------------------------------
        // Init
        //--------------------------------------------------
        iLastHour = iThisHour;
        iLastMinute = iThisMinute;
        dOpen = dClose = dPrice;
        if (bCumVolMode)
        {
          dVol2 = dVolume; //no need dVol1
        }
        else
        {
          dVol2 += dVolume;
        }
        setTradeTick.clear();
        setTradeTick.insert(dPrice);
      }
      else if (iLastHour != iThisHour || iLastMinute != iThisMinute)
      {
        //--------------------------------------------------
        // Process batch
        //--------------------------------------------------
        if (!setTradeTick.empty())
        {
          outFileStrm <<
            // setprecision(0) << fixed << sSym << "," <<
            ulYYYYMMDD << "," << iLastHour << ":" << iLastMinute << "," << dOpen << "," << *(--setTradeTick.end()) << "," << *(setTradeTick.begin()) << "," << dClose << "," <<
            (bCumVolMode ? dVol2 - dVol1 : dVol2)
            << endl;
        }

        //--------------------------------------------------
        // Re-init
        //--------------------------------------------------
        iLastHour = iThisHour;
        iLastMinute = iThisMinute;
        dOpen = dClose = dPrice;
        if (bCumVolMode)
        {
          dVol1 = dVol2; //dVol1 special
          dVol2 = dVolume;
        }
        else
        {
          dVol2 = 0;
        }
        setTradeTick.clear();
        setTradeTick.insert(dPrice);
      }
      else
      {
        dClose = dPrice;
        dVol2 = dVolume;
        setTradeTick.insert(dPrice);
      }
    }
    inFileStrm.close();

  }
  else cerr << "Unable to open " << string(sInfile);

  return;
}
