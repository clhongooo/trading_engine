#ifndef UTIL_TRADINGHOURS_H_
#define UTIL_TRADINGHOURS_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include <iterator>
#include "SDateTime.h"
#include "STool.h"

using namespace std;

class TradingHours {

  public:
    enum DefaultBehaviour {DFT_ALLOW=0,DFT_DISALLOW};
    typedef struct
    {
      vector<HHMMSS> m_Time;
      vector<char>   m_Prop;
      HHMMSS         m_CloseTime;
    } Time_Range;

    typedef struct
    {
      vector<YYYYMMDD>    m_vYYYYMMDD;
      vector<Time_Range>  m_vTimeRange;
    } Date_TradingHours;

    TradingHours();
    TradingHours(const DefaultBehaviour);
    virtual ~TradingHours();

    void ReadConfig(const string &);
    //--------------------------------------------------
    // All convenience wrappers
    //--------------------------------------------------
    bool IsTradingHour(const string &,const YYYYMMDDHHMMSS &);
    bool IsTradingHour(const string &,const YYYYMMDD &,const HHMMSS &);
    bool IsTradingHourStartEndBuffer(const string &,const YYYYMMDDHHMMSS &,const int,const int);
    bool IsTradingHourBreakBuffer(const string &,const YYYYMMDDHHMMSS &,const int);
    bool IsTradingHourStartEndBreakBuffer(const string &,const YYYYMMDDHHMMSS &,const int,const int,const int);
    bool IsTradingHourStartEndBreakBuffer(const string &,const YYYYMMDD &, const HHMMSS &,const int,const int,const int);
    //--------------------------------------------------
    HHMMSS GetTimeNSecBefClose(const string &,const YYYYMMDD &,const int);
    //--------------------------------------------------
    bool IsCfgFileLoaded() const;

  private:
    bool IsTradingHourPrivate(const string &,const YYYYMMDDHHMMSS &,const int,const int,const int);
    bool IsTradingHourPrivate(const string &,const YYYYMMDD &,const HHMMSS &,const int,const int,const int);
    bool ReturnDefault();

    map<string,Date_TradingHours> m_map_Sym_DateTrdgHour;
    map<string,int> m_Memoized_DateIdx;
    bool m_CfgFileLoaded;
    DefaultBehaviour m_DefaultBehaviour;

};

#endif /* UTIL_TRADINGHOURS_H_ */
