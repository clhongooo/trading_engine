#ifndef STIMER_HPP
#define STIMER_HPP

#include "PCH.h"
#include "Constants.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

class STimer
{
	public:
		static string CurrentTimeHMS()
		{
			std::ostringstream msg;
			const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
			boost::posix_time::time_facet*const f = new boost::posix_time::time_facet("%H:%M:%S");
			msg.imbue(std::locale(msg.getloc(),f));
			msg << now;
			return msg.str();
		}
};

#endif
