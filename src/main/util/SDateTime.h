//--------------------------------------------------
// Sunny DateTime
// Simple DateTime
// Date / time is deliberately not checked for correctness to achieve max speed.
//--------------------------------------------------

#ifndef SDATETIME_H
#define SDATETIME_H

#include "PCH.h"
#include "Constants.h"
#include "STool.h"
#include "Tokenizer.h"
#include "sys/time.h"
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/scoped_ptr.hpp>
#include <stdexcept>
#include <sstream>

using namespace std;
using namespace boost;

typedef double GregorianDayNum;
typedef double JulianDayNum;


class YYYYMMDD;
class YMD;
class HHMM;
class HHMMSS;
class HMS;
class YMDHMS;
class YYYYMMDDHHMMSS;


class SDateTime
{
  public:
    static int DAYSINMTH[12];
    enum TIMEPRECISION {SECOND, MILLISEC, MICROSEC, NANOSEC};
    enum TIMEZONE {GMT, HKT, CST, EST};
    //static void JulianDayNum2YMD(JulianDayNum jdn, YMD & ymd);
    static JulianDayNum CalcJulianDayNum(int y, int m, int d);
    static GregorianDayNum CalcGregorianDayNum(int y, int m, int d);


    // 20130916 - 20130917 = 1 day of difference
    // 20130917 - 20130917 = 0 day of difference
    template <class T1, class T2>
      static int CalendarDayDiff(const T1 & ymd1, const T2 & ymd2)
      {
        if (ymd1 == ymd2) return 0;
        else
        {
          return CalcGregorianDayNum(ymd2.Year(),ymd2.Month(),ymd2.Day()) - CalcGregorianDayNum(ymd1.Year(),ymd1.Month(),ymd1.Day());
        }
      }

    template <class T>
      static bool IsEndOfMonth(const T & ymd)
      {
        if (ymd.Month() == 2 && IsLeapYear(ymd.Year()))
          return (ymd.Day() == 29) ? true : false;
        else
          return (ymd.Day() == DAYSINMTH[ymd.Month()-1]) ? true : false;
      }

    template <class T>
      // Weekday means Mon - Fri
      static bool IsWeekDay(const T & ymd)
      {
        switch (DayOfWeek(ymd))
        {
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
            return true;
          default:
            return false;
        }
      }

    // Sunday = 0, Monday = 1, ... Saturday = 6
    template <class T>
      static int DayOfWeek(const T & ymd)
      {
        JulianDayNum jdn = CalcJulianDayNum(ymd.Year(),ymd.Month(),ymd.Day());

        int wkday = ((int)jdn+1)%7;
        return wkday;
      }

    static bool IsLeapYear  (int y);
    static bool IsValidYMD  (int year, int month, int day);
    static int  DaysInMonth (int y,int m);
    static int  DaysInYear  (int year);
    static std::string FormatPTime(boost::posix_time::ptime);
    static string fromUnixTimeToString(unsigned long, TIMEPRECISION, TIMEZONE, TIMEZONE);
    static string GetCurrentTimeYYYYMMDD_HHMMSS_000000();
    static string GetCurrentTimeYYYYMMDD_HHMMSS();
    static unsigned long GetCurrentTimeInMillsecSinceEpochGMT();
    static unsigned long GetCurrentTimeInMicrosecSinceEpochGMT();

    template <class T1, class T2>
      static bool FromCashTSToYMDHMS(const string & ts, T1 & yyyymmdd, T2 & hhmmss)
    {
      try
      {
        //--------------------------------------------------
        // Determine the format of time stamp
        //--------------------------------------------------
        vector<string> vTS;
        boost::split(vTS, ts, boost::is_any_of("_"));

        if (vTS.size() >= 4)
        {
          return false;
        }
        else if (vTS.size() == 3)
        {
          yyyymmdd.Set(boost::lexical_cast<int>(vTS[0]));
          hhmmss.Set(boost::lexical_cast<int>(vTS[1]));
        }
        else if (vTS.size() == 2)
        {
          yyyymmdd.Set(boost::lexical_cast<int>(vTS[0]));
          hhmmss.Set(boost::lexical_cast<int>(vTS[1]));
        }
        else // size must be 1
        {
          time_t t = time(0);
          struct tm * now = localtime(&t);
          yyyymmdd.Set((now->tm_year+1900)*10000 + (now->tm_mon+1)*100 + (now->tm_mday));
          hhmmss.Set(boost::lexical_cast<int>(vTS[0]));
        }
      }
      catch (std::exception e)
      {
        return false;
      }

      return true;
    }

    static YYYYMMDDHHMMSS ChangeTimeZone(const YYYYMMDDHHMMSS &, const TIMEZONE, const TIMEZONE);

};

class YYYYMMDD
{
  friend class YMD;

  private:
  int _iYMD;

  public:
  static YYYYMMDD BAD_DATE;

  YYYYMMDD();
  YYYYMMDD(const YYYYMMDD &ymd);
  YYYYMMDD(int iA);
  YYYYMMDD(const string &s);
  YYYYMMDD(const YMD &ymd);
  YYYYMMDD(const int,const int,const int);

  // setter and getter
  void Set(const string &s);
  void Set(int iYMD);
  void Set(int iY,int iM,int iD);
  void Set(const YYYYMMDD &ymd);
  int Value() const;
  void Invalidate();
  const bool IsValid() const;
  const int Year() const;
  const int Month() const;
  const int Day() const;

  // supported functions
  // comparators 1
  bool operator==(const YYYYMMDD& ymd) const;
  bool operator!=(const YYYYMMDD& ymd) const;
  bool operator> (const YYYYMMDD& ymd) const;
  bool operator>=(const YYYYMMDD& ymd) const;
  bool operator< (const YYYYMMDD& ymd) const;
  bool operator<=(const YYYYMMDD& ymd) const;
  long operator- (const YYYYMMDD& ymd) const;
  // comparators 2
  bool operator==(const YMD& ymd) const;
  bool operator!=(const YMD& ymd) const;
  bool operator> (const YMD& ymd) const;
  bool operator>=(const YMD& ymd) const;
  bool operator< (const YMD& ymd) const;
  bool operator<=(const YMD& ymd) const;

  // export
  const string ToStrYM() const;
  const string ToStr() const;
  const string ToStr_() const;
  int   ToInt() const;
  int   ToIntYM() const;
  int   GetDecade() const;
};

class YMD
{
  friend class YYYYMMDD;

  private:
  int _iY;
  int _iM;
  int _iD;

  public:
  static YMD BAD_DATE;
  YMD();
  YMD(int iA);
  YMD(const YMD & ymd);
  YMD(int iY,int iM,int iD);
  YMD(const YYYYMMDD & ymd);

  // setters and getters
 void Set(const int iY, const int iM, const int iD);
 void Set(const YYYYMMDD &);
 void SetYear(int iY);
 void SetMonth(int iM);
 void SetDay(int iD);
 const int Year() const;
 const int Month() const;
 const int Day() const;
 const void Invalidate();
 const bool IsValid() const;


  // supported functions
  // comparators 1
  bool operator==(const YMD& ymd) const;
  bool operator!=(const YMD& ymd) const;
  bool operator> (const YMD& ymd) const;
  bool operator>=(const YMD& ymd) const;
  bool operator< (const YMD& ymd) const;
  bool operator<=(const YMD& ymd) const;
  // comparators 2
  bool operator==(const YYYYMMDD& ymd) const;
  bool operator!=(const YYYYMMDD& ymd) const;
  bool operator> (const YYYYMMDD& ymd) const;
  bool operator>=(const YYYYMMDD& ymd) const;
  bool operator< (const YYYYMMDD& ymd) const;
  bool operator<=(const YYYYMMDD& ymd) const;

  // changing date
  void AddYear(int iY);
  void SubtractYear(int iY);
  void AddMonth(int iM);
  void SubtractMonth(int iM);
  // definitely favouring small increments / decrements in days; avoid conversion to Julian date for efficiency's sake
  void AddDay(int iD);
  void SubtractDay(int iD);

  // export
  void ToYYYYMMDD(YYYYMMDD & yyyymmdd) const;
  const string ToStrYM()    const;
  const string ToStrYM_()   const;
  const string ToStr()  const;
  const string ToStr_() const;
  int ToIntYM() const;
  int ToInt() const;

  // import
  void FromYYYYMMDD(const YYYYMMDD & yyyymmdd);
};

class HHMMSS
{
  friend class HMS;
  friend class HHMM;

  private:
  int _iHMS;

  public:
  static HHMMSS BAD_TIME;
  HHMMSS();
  HHMMSS(const HHMMSS & hms);
  HHMMSS(int iA);
  HHMMSS(const string &s);
  HHMMSS(const HMS &hms);
  HHMMSS(const int,const int,const int);

  // setter
  void Set(const string &s);
  void Set(int iHMS);
  void Set(int iH,int iM,int iS);
  void Set(int iH,int iM);
  void Set(const HHMMSS &);
  const int Hour() const;
  const int Minute() const;
  const int Second() const;

  void Invalidate();
  const int Value() const;
  const bool IsValid() const;


  // supported functions
  // comparators 1
  bool operator==(const HHMMSS& hms) const;
  bool operator!=(const HHMMSS& hms) const;
  bool operator> (const HHMMSS& hms) const;
  bool operator>=(const HHMMSS& hms) const;
  bool operator< (const HHMMSS& hms) const;
  bool operator<=(const HHMMSS& hms) const;
  // comparators 2
  bool operator==(const HMS& hms) const;
  bool operator!=(const HMS& hms) const;
  bool operator> (const HMS& hms) const;
  bool operator>=(const HMS& hms) const;
  bool operator< (const HMS& hms) const;
  bool operator<=(const HMS& hms) const;

  long operator- (const HHMMSS & hms2) const;

  // export
  int ToInt() const;
  const string ToStr()    const;
  const string ToStr_()   const;
  const string ToStrHM()  const;
  const string ToStrHM_() const;
};

class HHMM
{
  friend class HMS;
  friend class HHMMSS;

  private:
  int _iHM;

  public:
  static HHMM BAD_TIME;
  HHMM();
  HHMM(const HHMM & hm);
  HHMM(const HHMMSS & hms);
  HHMM(int iA);
  HHMM(const string &s);
  HHMM(const HMS &hms);

  // setter
  void Set(const string &s);
  void Set(int iHM);
  void Set(int iH,int iM);
  void Set(const HHMM &);
  const int Hour() const;
  const int Minute() const;

  void Invalidate();
  const int Value() const;
  const bool IsValid() const;


  // supported functions
  // comparators 1
  bool operator==(const HHMM& hm) const;
  bool operator!=(const HHMM& hm) const;
  bool operator> (const HHMM& hm) const;
  bool operator>=(const HHMM& hm) const;
  bool operator< (const HHMM& hm) const;
  bool operator<=(const HHMM& hm) const;
  // comparators 2
  bool operator==(const HMS& hm) const;
  bool operator!=(const HMS& hm) const;
  bool operator> (const HMS& hm) const;
  bool operator>=(const HMS& hm) const;
  bool operator< (const HMS& hm) const;
  bool operator<=(const HMS& hm) const;

  // export
  int ToInt() const;
  const string ToStr()    const;
  const string ToStr_()   const;
};


class HMS
{
  friend class HHMMSS;
  friend class HHMM;

  private:
  int _iH;
  int _iM;
  int _iS;
  // export
  int ToInt() const;

  public:
  static HMS BAD_TIME;
  HMS();
  HMS(const int hms);
  HMS(const HMS & hms);
  HMS(const HHMMSS & hms);
  HMS(int iH,int iM,int iS);

  // setters and getters
 void SetHour(int iH);
 void SetMinute(int iM);
 void SetSecond(int iS);
 void Set(const HHMMSS &);
 void Set(int iH,int iM,int iS);
 int Hour() const;
 int Minute() const;
 int Second() const;
 void Invalidate();
 const bool IsValid() const;


  // supported functions
  // comparators 1
  bool operator==(const HMS& hms) const;
  bool operator!=(const HMS& hms) const;
  bool operator> (const HMS& hms) const;
  bool operator>=(const HMS& hms) const;
  bool operator< (const HMS& hms) const;
  bool operator<=(const HMS& hms) const;
  // comparators 2
  bool operator==(const HHMMSS& hms) const;
  bool operator!=(const HHMMSS& hms) const;
  bool operator> (const HHMMSS& hms) const;
  bool operator>=(const HHMMSS& hms) const;
  bool operator< (const HHMMSS& hms) const;
  bool operator<=(const HHMMSS& hms) const;

  // changing time
  int AddMinute(int iM);
  int SubtractMinute(int iM);
  int AddSecond(int iS);
  int SubtractSecond(int iS);

  // export
  void ToHHMMSS(HHMMSS & hhmmss) const;
  HHMMSS ToHHMMSS()      const;
  const string ToStrHM() const;
  const string ToStr()   const;
  const string ToStrHM_()const;
  const string ToStr_()  const;
};

class YMDHMS
{
  friend class YYYYMMDDHHMMSS;
  friend class YYYYMMDD;
  friend class HHMMSS;
  friend class YMD;
  friend class HMS;

  private:
  YMD _ymd;
  HMS _hms;
  public:
  YMDHMS();
  YMDHMS(const int iY,const int iM,const int iD, const int iH,const int iMin,const int iS);
  YMDHMS(const YYYYMMDDHHMMSS &);
  void SetYMDHMS(const int iY,const int iM,const int iD, const int iH,const int iMin,const int iS);
  void Set(const YMD&,const HMS&);
  void Set(const YYYYMMDDHHMMSS &);
  void SetYMD(const int,const int,const int);
  void Set(const YMD&);
  void SetHMS(const int,const int,const int);
  void Set(const HMS&);
  const YMD GetYMD() const;
  const HMS GetHMS() const;
  bool operator==(const YMDHMS& ymdhms) const;
  const bool IsValid() const;

  // changing time
  void AddMinute(int iM);
  void AddSecond(int iS);
};

class YYYYMMDDHHMMSS
{
  friend class YYYYMMDD;
  friend class HHMMSS;
  friend class YMDHMS;
  friend class YMD;
  friend class HMS;

  private:
  YYYYMMDD _ymd;
  HHMMSS _hms;
  public:
  YYYYMMDDHHMMSS();
  YYYYMMDDHHMMSS(const YYYYMMDDHHMMSS&);
  YYYYMMDDHHMMSS(const int iYYYYMMDD, const int iHHMMSS);
  YYYYMMDDHHMMSS(const YMD &,const HMS &);
  YYYYMMDDHHMMSS(const YYYYMMDD &,const HHMMSS &);
  YYYYMMDDHHMMSS(const string &,const string &);
  YYYYMMDDHHMMSS(const int, const int, const int, const int, const int, const int);
  void Set(const YYYYMMDD &,const HHMMSS &);
  void Set(const YYYYMMDDHHMMSS &);
  void Set(const YMDHMS &);
  void Set(const string &,const string &);
  void Invalidate();
  const bool IsValid() const;
  YYYYMMDDHHMMSS& operator=(const YYYYMMDDHHMMSS&);

  const YYYYMMDD GetYYYYMMDD() const;
  void GetYMD(YMD &) const;
  const HHMMSS GetHHMMSS() const;

  bool operator==(const YYYYMMDDHHMMSS & ymdhms) const;
  bool operator!=(const YYYYMMDDHHMMSS & ymdhms) const;
  bool operator> (const YYYYMMDDHHMMSS & ymdhms) const;
  bool operator>=(const YYYYMMDDHHMMSS & ymdhms) const;
  bool operator< (const YYYYMMDDHHMMSS & ymdhms) const;
  bool operator<=(const YYYYMMDDHHMMSS & ymdhms) const;
  long operator- (const YYYYMMDDHHMMSS & ymdhms) const;

  bool operator==(const YMDHMS & ymdhms) const;
  bool operator!=(const YMDHMS & ymdhms) const;
  bool operator> (const YMDHMS & ymdhms) const;
  bool operator>=(const YMDHMS & ymdhms) const;
  bool operator< (const YMDHMS & ymdhms) const;
  bool operator<=(const YMDHMS & ymdhms) const;
  long operator- (const YMDHMS & ymdhms) const;

  const string ToStr() const;
  const string ToCashTimestampStr() const;
  const string ToCashTimestampStrNoMillisec() const;
  const long ToInt() const;
};

#endif

