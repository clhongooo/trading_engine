#include "StdStreamLogger.h"

boost::weak_ptr<StdStreamLogger> StdStreamLogger::m_pInstance;
StdStreamLogger::LogLevel StdStreamLogger::m_LogLevel = StdStreamLogger::EMERGENCY;

boost::shared_ptr<StdStreamLogger> StdStreamLogger::Instance()
{
  boost::shared_ptr<StdStreamLogger> instance = m_pInstance.lock();

  if (!instance) {
    instance.reset(new StdStreamLogger());
    m_pInstance = instance;
  }
  return instance;
}

void StdStreamLogger::SetLogLevel(LogLevel lev)
{
  m_LogLevel = lev;
}

StdStreamLogger::LogLevel StdStreamLogger::GetLogLevel() const
{
  return m_LogLevel;
}

void StdStreamLogger::Write(const LogLevel loglev, const char *fmt, ...)
{
  if (loglev > m_LogLevel) return;

  va_list ap;

  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  va_end(ap);

  fprintf(stdout, "\n");
}

bool StdStreamLogger::NeedToPrint(const LogLevel lev) const
{
  if (lev <= m_LogLevel) return true;
  else return false;
}

