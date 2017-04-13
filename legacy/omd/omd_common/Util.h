/*
 * Util.h
 *
 *  Created on: Apr 16, 2014
 *      Author: sunny
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/scoped_ptr.hpp>
#include <string>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

using namespace std;
using namespace boost;

class Util {
  public:
    enum TIMEPRECISION {SECOND, MILLISEC, MICROSEC, NANOSEC};
    enum TIMEZONE {GMT, HKT};
    Util();
    static string fromUnixTimeToString(unsigned long, TIMEPRECISION, TIMEZONE, TIMEZONE);
    static string fromUnixTimeToString(unsigned long, TIMEPRECISION, TIMEZONE, TIMEZONE, bool);
    static string fromGeniumDateToString(unsigned int);
    static unsigned long getCurrentUnixTime();
    virtual ~Util();

    static scoped_ptr<boost::posix_time::ptime> TimeSinceEpoch;
    static string  TrimLeft   (string );
    static string  TrimRight  (string );
    static string  Trim       (string );
    static string& TrimLeftR  (string&);
    static string& TrimRightR (string&);
    static string& TrimR      (string&);

};

#endif /* UTIL_H_ */

