#include <ThreadHealthMonitor.h>

boost::weak_ptr<ThreadHealthMonitor> ThreadHealthMonitor::m_pInstance;


ThreadHealthMonitor::ThreadHealthMonitor() :
  m_StartRefTime(boost::posix_time::microsec_clock::local_time()),
  m_SysIsHealthy(true)
{
  m_Logger       = Logger::Instance();
  m_SystemState  = SystemState::Instance();
  m_LastReportedHealth.clear();
}

ThreadHealthMonitor::~ThreadHealthMonitor()
{
}

boost::shared_ptr<ThreadHealthMonitor> ThreadHealthMonitor::Instance() {
  boost::shared_ptr<ThreadHealthMonitor> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new ThreadHealthMonitor());
    m_pInstance = instance;
  }
  return instance;
}

void ThreadHealthMonitor::ReportHealthy(const char * sIdentity)
{
  // char sTmp[HEALTHKEYLEN+1];
  // memset(sTmp,'\0',HEALTHKEYLEN+1);
  // strncpy(sTmp,sIdentity,STool::Min(strlen(sIdentity),HEALTHKEYLEN));

  if (strlen(sIdentity) > HEALTHKEYLEN) m_Logger->Write(Logger::ERROR, "ThreadHealthMonitor: ReportHealthy: sIdentity too long. Exiting.");

  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);
  m_LastReportedHealth[sIdentity] = boost::posix_time::microsec_clock::local_time();
}

void ThreadHealthMonitor::Run()
{

  for (;;)
  {
    if (m_Logger)
    {
      boost::shared_lock<boost::shared_mutex> lock(m_Mutex);

      boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

      map<const char *, boost::posix_time::ptime>::iterator it;

      bool bAllReportedHealthy = true;
      for (it = m_LastReportedHealth.begin(); it != m_LastReportedHealth.end(); ++it)
      {
        boost::posix_time::time_duration diff = now - it->second;
        unsigned long ulTDiff = diff.total_milliseconds();

        m_Logger->Write(Logger::NOTICE, "ThreadHealthMonitor: %s reported health %d millisec / %d sec / %d min ago ",
            it->first,
            ulTDiff,
            ulTDiff / 1000,
            ulTDiff / 60000
            );

        if (ulTDiff / 1000 > UNHEALTHY_THRESHOLD_IN_SEC) bAllReportedHealthy = false;
      }

      m_SysIsHealthy = bAllReportedHealthy;

      if (m_LastReportedHealth.empty()) m_Logger->Write(Logger::NOTICE, "ThreadHealthMonitor: Nobody reported their health ever.");

      if (!m_SysIsHealthy) m_Logger->Write(Logger::NOTICE, "ThreadHealthMonitor: Some threads have not reported health for more than %d seconds. Please check.", UNHEALTHY_THRESHOLD_IN_SEC);
    }

    for (unsigned int i = 0; i < 5; ++i)
    {
      if (m_SystemState->ChkIfThreadShouldStop()) break;
      sleep(1);
    }
    if (m_SystemState->ChkIfThreadShouldStop()) break;

  }
  return;
}

bool ThreadHealthMonitor::IsSystemHealthy()
{
  return m_SysIsHealthy;
}

