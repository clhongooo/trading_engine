#include "DailyOHLCToMarketFeed.h"

DailyOHLCToMarketFeed::DailyOHLCToMarketFeed()
{
}

DailyOHLCToMarketFeed::~DailyOHLCToMarketFeed()
{
}

void DailyOHLCToMarketFeed::Convert(const char * sInfile,const char * sOutfile,const char * sSymbolA,
    const char * s_O_HHMMSS_A,
    const char * s_H_HHMMSS_A,
    const char * s_L_HHMMSS_A,
    const char * s_C_HHMMSS_A,
    const char * s_ColumnFormat_A
    )
{
  string sSymbol(sSymbolA);
  string s_O_HHMMSS(s_O_HHMMSS_A);
  string s_H_HHMMSS(s_H_HHMMSS_A);
  string s_L_HHMMSS(s_L_HHMMSS_A);
  string s_C_HHMMSS(s_C_HHMMSS_A);
  string s_ColumnFormat(s_ColumnFormat_A);

  ofstream outFileStrm(sOutfile,std::ifstream::out);

  BarProvider bp(sInfile, s_ColumnFormat, 5, 'F', 'N', 'P');

  bp.ResetPsn();
  while (bp.HasNextBar())
  {
    bp.MoveNext();

    outFileStrm <<
      bp.GetYYYYMMDD().ToStr() << "_" << s_O_HHMMSS << "_000000," <<
      sSymbol << "," <<
      bp.Open() << "," <<
      "10000" << "," <<
      "B,"<< bp.Open() <<",10000000,999999,0,999999,0,999999,0,999999,0,A,"<< bp.Open() <<",10000000,999999,0,999999,0,999999,0,999999,0" << endl;

    outFileStrm <<
      bp.GetYYYYMMDD().ToStr() << "_" << s_H_HHMMSS << "_000000," <<
      sSymbol << "," <<
      bp.High() << "," <<
      "20000" << "," <<
      "B,"<< bp.High() <<",10000000,999999,0,999999,0,999999,0,999999,0,A,"<< bp.High() <<",10000000,999999,0,999999,0,999999,0,999999,0" << endl;

    outFileStrm <<
      bp.GetYYYYMMDD().ToStr() << "_" << s_L_HHMMSS << "_000000," <<
      sSymbol << "," <<
      bp.Low() << "," <<
      "30000" << "," <<
      "B,"<< bp.Low() <<",10000000,999999,0,999999,0,999999,0,999999,0,A,"<< bp.Low() <<",10000000,999999,0,999999,0,999999,0,999999,0" << endl;

    outFileStrm <<
      bp.GetYYYYMMDD().ToStr() << "_" << s_C_HHMMSS << "_000000," <<
      sSymbol << "," <<
      bp.Close() << "," <<
      "40000" << "," <<
      "B,"<< bp.Close() <<",10000000,999999,0,999999,0,999999,0,999999,0,A,"<< bp.Close() <<",10000000,999999,0,999999,0,999999,0,999999,0" << endl;

  }

  outFileStrm.close();

  return;
}
