#ifndef UTIL_PERIODICTASK_H_
#define UTIL_PERIODICTASK_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include "SDateTime.h"

#define PERIODICTASK_EXPANDSIZE 100

class PeriodicTask {
  public:
    PeriodicTask();
    virtual ~PeriodicTask();

    const int GetNewTokenAndSetIntervalInSec(const int);
    const int GetNewTokenAndSetIntervalInSec(const int,const YYYYMMDDHHMMSS &);
    bool CheckIfItIsTimeToWakeUp(const int,const YYYYMMDDHHMMSS &);

  private:
    void ExpandVectorNoLock(const int,int &);
    bool UpdateTimeIfNeededNoLock(const YYYYMMDDHHMMSS &);

    vector<YMDHMS> m_ymdhms_LastMonitorTime;
    vector<int>    m_Interval;

    int m_AllocatedSize;
    int m_LastAssignedToken;
    bool m_AllTimeIsGood;
    shared_mutex m_Mutex;
};

#endif /* UTIL_PERIODICTASK_H_ */
