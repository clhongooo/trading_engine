#include "MinuteOHLCToMarketFeed.h"

MinuteOHLCToMarketFeed::MinuteOHLCToMarketFeed()
{
}

MinuteOHLCToMarketFeed::~MinuteOHLCToMarketFeed()
{
}

void MinuteOHLCToMarketFeed::Convert(const char * sInfile,const char * sOutfile,const char * sSymbolA,const char * sTradeOnlyA,const char * sDateColmA,const char * sTimeColmA,const char * sCloseColmA,const char * sCloseOnlyA,const char * sVolumeColmA)
{
  string sSymbol(sSymbolA);
  int iSymColm = -1;

  try 
  {
    iSymColm = boost::lexical_cast<int>(sSymbol);
  }
  catch(std::exception e)
  {
  }

  string sDateColm(sDateColmA);
  string sTimeColm(sTimeColmA);
  string sCloseColm(sCloseColmA);
  string sCloseOnly(sCloseOnlyA);
  string sVolumeColm(sVolumeColmA);

  int iDateColm  = boost::lexical_cast<int>(sDateColm )  ;
  int iTimeColm  = boost::lexical_cast<int>(sTimeColm )  ;
  int iCloseColm = boost::lexical_cast<int>(sCloseColm)  ;
  int iOpenColm  = boost::lexical_cast<int>(sCloseColm)-3;
  int iHighColm  = boost::lexical_cast<int>(sCloseColm)-2;
  int iLowColm   = boost::lexical_cast<int>(sCloseColm)-1;
  int iVolumeColm= boost::lexical_cast<int>(sVolumeColm);



  string sTradeOnly(sTradeOnlyA);
  bool bTradeOnly = ((sTradeOnly == "t" || sTradeOnly == "true") ? true : false);

  ofstream outFileStrm(sOutfile,std::ifstream::out);


  deque<string> dqStr;
  STool::ReadFile(sInfile,dqStr);


  long lCntVol = 1;
  YYYYMMDD ymdLastDate;
  for (deque<string>::iterator it = dqStr.begin(); it != dqStr.end(); ++it)
  {

    string & sLine = *it;
    vector<string> vs;
    boost::split(vs, sLine, boost::is_any_of(","));  

    YYYYMMDD ymdLine(vs[iDateColm]);
    if (ymdLine != ymdLastDate)
    {
      lCntVol = 1;
      ymdLastDate = ymdLine;
    }

    for (unsigned int iOHLC = 0; iOHLC < 4; ++iOHLC)
    {
      if (sCloseOnly == "t" || sCloseOnly == "T") if (iOHLC < 3) continue;

      string sPriceToUse;
      string sVolumeToUse;

      switch(iOHLC)
      {
        case 0: { sPriceToUse = vs[iOpenColm ]; break; }
        case 1: { sPriceToUse = vs[iHighColm ]; break; }
        case 2: { sPriceToUse = vs[iLowColm  ]; break; }
        case 3: { sPriceToUse = vs[iCloseColm]; break; }
      }

      if (iVolumeColm >= 0 && iVolumeColm < 100)
        sVolumeToUse = vs[iVolumeColm];
      else
        sVolumeToUse = abs(iVolumeColm);

      //--------------------------------------------------
      // time stamp and last trade price
      //--------------------------------------------------
      outFileStrm << ymdLine.ToStr() << "_" << HHMMSS(vs[iTimeColm]).ToStr() << "_000000,";

      if (iSymColm <= 0) outFileStrm << sSymbol;
      else               outFileStrm << vs[iSymColm];

      outFileStrm << "," << sPriceToUse << "," << sVolumeToUse;
      lCntVol++;

      //--------------------------------------------------
      // whether to print the order book as well
      //--------------------------------------------------
      if (bTradeOnly)
      {
        outFileStrm << endl;
      }
      else
      {
        outFileStrm << "," << "B,"<< sPriceToUse << "," << sVolumeToUse << ",999999,0,999999,0,999999,0,999999,0,A," << sPriceToUse << "," << sVolumeToUse << ",999999,0,999999,0,999999,0,999999,0" << endl;
      }
      //--------------------------------------------------
    }

  }

  outFileStrm.close();

  return;
}
