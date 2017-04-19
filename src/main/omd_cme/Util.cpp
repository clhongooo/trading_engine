/*
 * Util.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: sunny
 */

#include "Util.h"

scoped_ptr<boost::posix_time::ptime> Util::TimeSinceEpoch;

Util::Util()
{
}

Util::~Util()
{
}

string Util::fromUnixTimeToString(unsigned long ulUnixTime, TIMEPRECISION timePrecision, TIMEZONE tzSrc, TIMEZONE tzDest)
{
  return fromUnixTimeToString(ulUnixTime, timePrecision, tzSrc, tzDest, false);
}

string Util::fromUnixTimeToString(unsigned long ulUnixTime, TIMEPRECISION timePrecision, TIMEZONE tzSrc, TIMEZONE tzDest, bool bMicrosec)
{
  unsigned long ulMicrosec = 0;
  if (bMicrosec)
  {
    switch(timePrecision)
    {
      case NANOSEC:  { ulMicrosec = (ulUnixTime / 1000) % 1000000; break; }
      case MICROSEC: { ulMicrosec = (ulUnixTime       ) % 1000000; break; }
      case MILLISEC: { ulMicrosec = (ulUnixTime * 1000) % 1000000; break; }
      default:       {                                             break; }
    }
  }

  switch(timePrecision)
  {
    case NANOSEC:
      {
        ulUnixTime = (ulUnixTime/1000000000);
        break;
      }
    case MICROSEC:
      {
        ulUnixTime = (ulUnixTime/1000000);

        break;
      }
    case MILLISEC:
      {
        ulUnixTime = (ulUnixTime/1000);
        break;
      }
    default:
      {
        break;
      }
  }

  if (tzSrc == HKT && tzDest == GMT)
  {
    ulUnixTime -= 8*60*60;
  }
  else if (tzSrc == GMT && tzDest == HKT)
  {
    ulUnixTime += 8*60*60;
  }

  boost::gregorian::date d(1970,1,1);
  boost::posix_time::ptime t1(d, boost::posix_time::seconds(ulUnixTime));
  ostringstream oo;
  oo << to_simple_string(t1);
  if (bMicrosec) oo << ". Microsec: " << std::setw(6) << std::setfill('0') << ulMicrosec;
  return oo.str();
}


//--------------------------------------------------
// For MsgType 304 ExpirationDate
//--------------------------------------------------
string Util::fromGeniumDateToString(unsigned int expdate)
{
  unsigned int a=expdate;
  unsigned int siza=(sizeof(a)-2)*8;
  unsigned int month=(a<<(7+siza))>>(7+siza+5) ;
  unsigned int day=(a<<(7+siza+4))>>(7+siza+4) ;
  unsigned int year= (a>>9)-1+1990;
  char tmp[100];
  sprintf(tmp,"%d-%02d-%02d",year,month,day);
  return string(tmp);
}

//--------------------------------------------------
// Get current UNIX time in milliseconds
//--------------------------------------------------
unsigned long Util::getCurrentUnixTime()
{
  if (!Util::TimeSinceEpoch)
  {
    Util::TimeSinceEpoch.reset(new boost::posix_time::ptime(gregorian::date(1970,1,1)));
  }

  boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
  boost::posix_time::time_duration diff = now - (*Util::TimeSinceEpoch);
  return diff.total_milliseconds();
}

string Util::TrimLeft(string s)
{
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
  return s;
}

string Util::TrimRight(string s)
{
  s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
  return s;
}

string Util::Trim(string s)
{
  return TrimLeft(TrimRight(s));
}

string& Util::TrimLeftR(string& s)
{
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
  return s;
}

string& Util::TrimRightR(string& s)
{
  s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
  return s;
}

string& Util::TrimR(string& s)
{
  return TrimLeftR(TrimRightR(s));
}
