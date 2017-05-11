/*
 * ThreadHealthMonitor.h
 *
 *  Created on: Aug 6, 2014
 *      Author: sunny
 */

#ifndef THREADHEALTHMONITOR_H_
#define THREADHEALTHMONITOR_H_

#include <map>
#include <cmath>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "Logger.h"
#include "SharedObjects.h"
#include <boost/thread.hpp>
#include <cstdio>
#include <cstdlib>

using namespace std;
using namespace boost;

class ThreadHealthMonitor {
  public:
    enum { LEN = 4096 };
    enum ThreadIdentity {REALTIMEPROCESSOR=1, REFRESHPROCESSOR=2, DATACOMPLETENESSINSPECTOR=3, RTSCLIENT=4, PREPROCESSOR=5, INSTRUMENTREPLAYPROCESSOR=6};
    ThreadHealthMonitor();
    virtual ~ThreadHealthMonitor();
    void ReportThatIAmHealthy(ThreadIdentity, unsigned short);
    static boost::shared_ptr<ThreadHealthMonitor> Instance();
    void Run();

  private:
    boost::shared_ptr<Logger>          m_Logger;
    boost::shared_ptr<SharedObjects>   m_ShrObj;

    static boost::weak_ptr <ThreadHealthMonitor> m_pInstance;

    void EnumToString(ThreadIdentity, char *);
    ThreadIdentity NumToEnum(unsigned short);

    unsigned long ToHash(ThreadIdentity,unsigned short);
    void FromHash(unsigned long, ThreadIdentity &, unsigned short &);

    map<unsigned long, boost::posix_time::ptime> m_LastReportedHealth;
    boost::posix_time::ptime m_StartRefTime;
    boost::shared_mutex m_Mutex;

};

#endif /* THREADHEALTHMONITOR_H_ */
