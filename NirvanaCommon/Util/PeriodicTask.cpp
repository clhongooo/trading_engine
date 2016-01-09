#include <PeriodicTask.h>

PeriodicTask::PeriodicTask() :
  m_AllocatedSize(0),
  m_LastAssignedToken(0),
  m_AllTimeIsGood(false)
{
  ExpandVectorNoLock(PERIODICTASK_EXPANDSIZE, m_AllocatedSize);
}

PeriodicTask::~PeriodicTask()
{
}

void PeriodicTask::ExpandVectorNoLock(const int sizetoexpand, int & allocdsize)
{
  for (unsigned int i = 0; i < sizetoexpand; ++i)
  {
    m_ymdhms_LastMonitorTime.push_back(YMDHMS()); 
    m_Interval              .push_back(-1      );
  }

  allocdsize += sizetoexpand;
}

const int PeriodicTask::GetNewTokenAndSetIntervalInSec(const int interval)
{
  return GetNewTokenAndSetIntervalInSec(interval, YYYYMMDDHHMMSS());
}

const int PeriodicTask::GetNewTokenAndSetIntervalInSec(const int interval, const YYYYMMDDHHMMSS & ymdhms)
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);

  //--------------------------------------------------
  UpdateTimeIfNeededNoLock(ymdhms);
  //--------------------------------------------------

  if (m_LastAssignedToken >= m_AllocatedSize - 1) ExpandVectorNoLock(PERIODICTASK_EXPANDSIZE,m_AllocatedSize);

  m_ymdhms_LastMonitorTime[m_LastAssignedToken] = ymdhms;
  m_Interval              [m_LastAssignedToken] = interval;

  return (m_LastAssignedToken++);
}

bool PeriodicTask::CheckIfItIsTimeToWakeUp(const int token, const YYYYMMDDHHMMSS & ymdhms)
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);

  //--------------------------------------------------
  // return false if we have just init it
  //--------------------------------------------------
  if (UpdateTimeIfNeededNoLock(ymdhms)) return false;
  //--------------------------------------------------

  //--------------------------------------------------
  // we don't recognize this token
  //--------------------------------------------------
  if (token > m_LastAssignedToken) return false;
  //--------------------------------------------------

  YMDHMS & ymdhmsLMT = m_ymdhms_LastMonitorTime[token];

  if (!ymdhmsLMT.IsValid()) return false;
  if (ymdhms <= ymdhmsLMT) return false;

  long lDiffInSec = ymdhms - ymdhmsLMT;

  if (lDiffInSec >= m_Interval[token])
  {
    ymdhmsLMT.AddSecond((lDiffInSec / m_Interval[token]) * m_Interval[token]);
    return true;
  }
  else return false;
}

bool PeriodicTask::UpdateTimeIfNeededNoLock(const YYYYMMDDHHMMSS & ymdhms)
{
  if (!m_AllTimeIsGood && ymdhms.IsValid())
  {
    std::for_each(m_ymdhms_LastMonitorTime.begin(), m_ymdhms_LastMonitorTime.end(), [&](YMDHMS & a){ a.Set(ymdhms); });
    m_AllTimeIsGood = true;
    return true;
  }
  return false;
}

