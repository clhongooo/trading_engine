#ifndef THREADHEALTHMONITOR_H_
#define THREADHEALTHMONITOR_H_

#include "PCH.h"
#include "Constants.h"
#include <map>
#include <cmath>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/utility.hpp>
#include <boost/thread.hpp>
#include <cstdio>
#include <cstdlib>
#include "SystemState.h"
#include "Logger.h"
#include "STool.h"

#define UNHEALTHY_THRESHOLD_IN_SEC 120

using namespace std;
using namespace boost;

class ThreadHealthMonitor {
  public:
    ThreadHealthMonitor();
    virtual ~ThreadHealthMonitor();
    void ReportHealthy(const char *);
    bool IsSystemHealthy();
    static boost::shared_ptr<ThreadHealthMonitor> Instance();
    void Run();

  private:
    //--------------------------------------------------
    // System Objects
    //--------------------------------------------------
    boost::shared_ptr<Logger>      m_Logger;
    boost::shared_ptr<SystemState> m_SystemState;

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    static boost::weak_ptr <ThreadHealthMonitor> m_pInstance;

    //--------------------------------------------------
    map<const char *, boost::posix_time::ptime> m_LastReportedHealth;
    boost::posix_time::ptime m_StartRefTime;
    boost::shared_mutex m_Mutex;

    bool m_SysIsHealthy;

};

#endif /* THREADHEALTHMONITOR_H_ */
