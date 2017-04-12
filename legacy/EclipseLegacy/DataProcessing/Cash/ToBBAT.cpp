#include "ToBBAT.h"

void ToBBAT::Convert(const char * sInfile,const char * sOutfile,const char * sSymbol)
{
  double dPrice;
  double dVolume;
  vector<unsigned long> hhmmss;

  map<string,BestBidAskTrade> bbat;
  map<string,double> PrevLastTradeVolume;

  ifstream inFileStrm (sInfile,std::ifstream::in);
  ofstream outFileStrm(sOutfile,std::ifstream::out);

  if (inFileStrm.good())
  {
    string sValue;

    for (;;)
    {
      //--------------------------------------------------
      // 26 columns
      //--------------------------------------------------
      if (!getline(inFileStrm, sValue, ',')) return;
      string sub = (sValue.at(0) == '0' ? sValue.substr(1,5) : sValue);
      unsigned long ulHHMMSS = lexical_cast<unsigned long>(sub);

      if (!getline(inFileStrm, sValue, ',')) return;
      string sSym = sValue;

      if (bbat.find(sSym) == bbat.end()) bbat[sSym] = BestBidAskTrade();

      map<string,BestBidAskTrade>::iterator it;
      for (it = bbat.begin(); it != bbat.end(); ++it) it->second.ChangeTime(ulHHMMSS);

      {
        if (!getline(inFileStrm, sValue, ',')) return; //Price
        dPrice = lexical_cast<double>(sValue);
        if (!getline(inFileStrm, sValue, ',')) return; //Volume
        dVolume = lexical_cast<size_t>(sValue);

        if (PrevLastTradeVolume.find(sSym) == PrevLastTradeVolume.end())
          PrevLastTradeVolume[sSym] = 0;

        dVolume -= PrevLastTradeVolume[sSym];

        bbat[sSym].ChangeLastTrade(dPrice,dVolume);

        PrevLastTradeVolume[sSym] = PrevLastTradeVolume[sSym] + dVolume;

        if (!getline(inFileStrm, sValue, ',')) return; //B

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        dPrice = lexical_cast<double>(sValue);
        if (!getline(inFileStrm, sValue, ',')) return; //Volume
        dVolume = lexical_cast<size_t>(sValue);
        bbat[sSym].ChangeBestBid(dPrice,dVolume);

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        if (!getline(inFileStrm, sValue, ',')) return; //Volume

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        if (!getline(inFileStrm, sValue, ',')) return; //Volume

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        if (!getline(inFileStrm, sValue, ',')) return; //Volume

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        if (!getline(inFileStrm, sValue, ',')) return; //Volume

        if (!getline(inFileStrm, sValue, ',')) return; //A

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        dPrice = lexical_cast<double>(sValue);
        if (!getline(inFileStrm, sValue, ',')) return; //Volume
        dVolume = lexical_cast<size_t>(sValue);
        bbat[sSym].ChangeBestAsk(dPrice,dVolume);

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        if (!getline(inFileStrm, sValue, ',')) return; //Volume

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        if (!getline(inFileStrm, sValue, ',')) return; //Volume

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        if (!getline(inFileStrm, sValue, ',')) return; //Volume

        if (!getline(inFileStrm, sValue, ',')) return; //Price
        if (!getline(inFileStrm, sValue, '\n')) return; //Volume

      }

      unsigned long  ulTime;
      double  dbidprice;
      size_t  dbidvolume;
      double  daskprice;
      size_t  daskvolume;
      double  dtradeprice;
      size_t  dtradevolume;

      for (it = bbat.begin(); it != bbat.end(); ++it)
      {
        if (sSym == sSymbol && it->first == sSymbol && bbat[it->first].GetBestBidAskTrade(ulTime,dbidprice,dbidvolume,daskprice,daskvolume,dtradeprice,dtradevolume))
        {
          outFileStrm <<
            it->first    << "\t" <<
            ulTime       << "\tB: " <<
            dbidprice    << " (" <<
            dbidvolume   << ")\tA: " <<
            daskprice    << " (" <<
            daskvolume   << ")\tT: " <<
            dtradeprice  << " (" <<
            dtradevolume << ")" <<
            endl;
        }
      }


    }
    inFileStrm.close();

  }
  else cerr << "Unable to open " << string(sInfile);

  return;
}

