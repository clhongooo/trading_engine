/*
 * ThreadHealthMonitor.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: sunny
 */

#include "ThreadHealthMonitor.h"

boost::weak_ptr<ThreadHealthMonitor> ThreadHealthMonitor::m_pInstance;


ThreadHealthMonitor::ThreadHealthMonitor() :
  m_StartRefTime(boost::posix_time::microsec_clock::local_time())
{
  m_Logger = Logger::Instance();
  m_ShrObj = SharedObjects::Instance();
  m_LastReportedHealth.clear();
}

ThreadHealthMonitor::~ThreadHealthMonitor()
{
}

boost::shared_ptr <ThreadHealthMonitor> ThreadHealthMonitor::Instance() {
  boost::shared_ptr <ThreadHealthMonitor> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new ThreadHealthMonitor());
    m_pInstance = instance;
  }
  return instance;
}

void ThreadHealthMonitor::ReportHealthy(ThreadIdentity id, unsigned short channelid)
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);
  m_LastReportedHealth[ToHash(id,channelid)] = boost::posix_time::microsec_clock::local_time();

  return;
}


unsigned long ThreadHealthMonitor::ToHash(ThreadIdentity id, unsigned short channelid)
{
  return id*1000+channelid;
}


void ThreadHealthMonitor::FromHash(unsigned long hash, ThreadIdentity &id, unsigned short &channelid)
{
  id = NumToEnum(hash/1000);
  channelid = hash%1000;
  return;
}


ThreadHealthMonitor::ThreadIdentity ThreadHealthMonitor::NumToEnum(unsigned short id)
{
  switch(id)
  {
    case 1:
      {
        return REALTIMEPROCESSOR;
        break;
      }
    case 2:
      {
        return REFRESHPROCESSOR;
        break;
      }
    case 3:
      {
        return DATACOMPLETENESSINSPECTOR;
        break;
      }
    case 4:
      {
        return RTSCLIENT;
        break;
      }
    case 5:
      {
        return PREPROCESSOR;
        break;
      }
    default:
      {
        return REALTIMEPROCESSOR;
        break;
      }
  }
}
void ThreadHealthMonitor::EnumToString(ThreadIdentity id, char * sTmp)
{
  switch (id)
  {
    case 1:
      {
        strcpy(sTmp,"RealTimeProcessor");
        break;
      }
    case 2:
      {
        strcpy(sTmp,"RefreshProcessor");
        break;
      }
    case 3:
      {
        strcpy(sTmp,"DataCompletenessInspector");
        break;
      }
    case 4:
      {
        strcpy(sTmp,"RTSClient");
        break;
      }
    case 5:
      {
        strcpy(sTmp,"PreProcessor");
        break;
      }
    default:
      {
        strcpy(sTmp,"");
        break;
      }
  }
  return;
}


void ThreadHealthMonitor::Run()
{
  char sHealthMsg[LEN];
  char sTmp[LEN];
  map<unsigned long, boost::posix_time::ptime>::iterator it;

  for (;;)
  {
    if (m_Logger)
    {
      boost::shared_lock<boost::shared_mutex> lock(m_Mutex);

      boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

      for (it = m_LastReportedHealth.begin(); it != m_LastReportedHealth.end(); ++it)
      {
        memset(sHealthMsg,'\0',LEN);
        ThreadIdentity id;
        unsigned short usChannelID;

        boost::posix_time::time_duration diff = now - it->second;
        unsigned long ulTDiff = diff.total_milliseconds();

        FromHash(it->first,id,usChannelID);

        strcat(sHealthMsg,"ThreadHealthMonitor: ");
        memset(sTmp,'\0',LEN); EnumToString(id,sTmp);
        strcat(sHealthMsg,sTmp);
        strcat(sHealthMsg,": ChannelID:");
        memset(sTmp,'\0',LEN); sprintf(sTmp, "%d", usChannelID);
        strcat(sHealthMsg,sTmp);
        strcat(sHealthMsg,". Last reported health ");
        memset(sTmp,'\0',LEN); sprintf(sTmp, "%d", ulTDiff);
        strcat(sHealthMsg,sTmp);
        strcat(sHealthMsg," millisec / ");
        memset(sTmp,'\0',LEN); sprintf(sTmp, "%d", ulTDiff / 1000);
        strcat(sHealthMsg,sTmp);
        strcat(sHealthMsg," sec / " );
        memset(sTmp,'\0',LEN); sprintf(sTmp, "%d", ulTDiff / 60000);
        strcat(sHealthMsg,sTmp);
        strcat(sHealthMsg," min ago." );
        strcat(sHealthMsg,"\0" );
        m_Logger->Write(Logger::NOTICE, "ThreadHealthMonitor: %s.", sHealthMsg);
      }

      lock.unlock();
    }

    for (unsigned int i = 0; i < 30; ++i)
    {
      boost::this_thread::sleep(boost::posix_time::seconds(1));

      //--------------------------------------------------
      // System shutdown
      //--------------------------------------------------
      if (m_ShrObj->ThreadShouldExit())
      {
        m_Logger->Write(Logger::NOTICE,"ThreadHealthMonitor: Stopping thread.");
        return;
      }
    }

  }
  return;
}

