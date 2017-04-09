#include "ATU_Timer.h"
#include "Constants.h"
using namespace std;
namespace atu {
// TODO: There is error when stress testing the code
// e.g. while true; do ./atu_timer_main 12766; done
// "pure virtual method called" error will pop out after few seconds and evenually will core dump
ATU_Timer::ATU_Timer(int key) :	m_ShareMemoryTimerData(NULL), m_shareMemoryKey(key), m_lastSec(0), m_isShutdown(false) {
	if (key != 0) {
		createShareMemoryTimerData(key);
	} else {
		m_ShareMemoryTimerData = new atu_share_memory_timer_t;
	}
	//check if a master is present
	m_isMaster = !isMasterAlive();
}
ATU_Timer::~ATU_Timer() {
#ifdef _ATUDEBUG_
	cout << "Destroying ATU_Timer" << endl;
#endif //_ATUDEBUG_
}
bool ATU_Timer::isReady() {
	return isMasterAlive();
}
void ATU_Timer::waitUntilReady() {
	while(!isReady());
	return;
}
bool ATU_Timer::init() {
	// Should be called after the thread is running (i.e. detached)
	waitUntilReady();
	setTimeStamp();
	return true;
}

bool ATU_Timer::isMasterAlive() {
	int max_retry_count = _ATU_TIMER_CHECK_MASTER_SLAVE_DELAY_RETRY_;
	int i = 0;
	double lastMS,curMS;
	lastMS = m_ShareMemoryTimerData->m_TimeStampms;
	do {
		usleep(_ATU_TIMER_CHECK_MASTER_SLAVE_DELAY_USEC_);
		curMS = m_ShareMemoryTimerData->m_TimeStampms;
	} while ((max_retry_count > i++) && (lastMS == curMS));
	return (lastMS != curMS);
}
void ATU_Timer::run() {
	if (m_isMaster) {
		while (!m_isShutdown) {
			clock_gettime(CLOCK_REALTIME, (timespec*) (&(m_ShareMemoryTimerData->m_tS)));
			long long cur = (long long) m_ShareMemoryTimerData->m_tS.tv_nsec;
			double curmsec = ((double) cur / double(1000 * 1000)) + (double) m_ShareMemoryTimerData->m_tS.tv_sec * (double) 1000;
			m_ShareMemoryTimerData->m_TimeStampms = curmsec;
		}
	} else {
		while (!m_isShutdown) {
			usleep(1000 * 1000 *  _ATU_TIMER_SLAVE_SLEEP_SECOND_);
		}
	}

}
void ATU_Timer::final(void) {
#ifdef _ATUDEBUG_
	cout << "finalizing ATU_Timer" << endl;
	cout.flush();
#endif //_ATUDEBUG_
}
bool ATU_Timer::createShareMemoryTimerData(int key) {
	int shmid;
	if ((shmid = shmget(key, sizeof(atu_share_memory_timer_t), IPC_CREAT | 0777)) < 0) {

		m_ShareMemoryTimerData = NULL;
		return false;
	} else {
		m_ShareMemoryTimerData = (atu_share_memory_timer_t*) shmat(shmid, 0, 0);
		if ((int*) (m_ShareMemoryTimerData) == (int*) -1) {

			m_ShareMemoryTimerData = NULL;
			return false;
		}
	}
	return true;
}
/*
double ATU_Timer::getTimeInMS() {
	return m_ShareMemoryTimerData->m_TimeStampms;
}
*/
void ATU_Timer::setTimeStamp_YMDHMS() {
	//time_t rawtime;
	struct tm * timeinfo;
	struct tm timeinfo2;
	char buffer[200];
	//struct timeb tp;
	timeinfo = localtime_r((time_t*) (&(m_ShareMemoryTimerData->m_tS.tv_sec)),
			&timeinfo2);
	strftime(buffer, 200, "%Y%m%d_%H%M%S", &timeinfo2);
	sprintf((char*) (m_TimeStampStr), "%s_%06d", buffer,
			(int)(m_ShareMemoryTimerData->m_tS.tv_nsec / 1000));
}
void ATU_Timer::setTimeStamp_YMDHMS(double curms) {
	//time_t rawtime;
	struct tm * timeinfo;
	struct tm timeinfo2;
	char buffer[200];
	//struct timeb tp;
	time_t tvsec=(time_t)(curms/1000);
	timeinfo = localtime_r((time_t*) (&(tvsec)), &timeinfo2);
	strftime(buffer, 200, "%Y%m%d_%H%M%S", &timeinfo2);
	double ms=curms*1000-tvsec*1000*1000;
	sprintf((char*) (m_TimeStampStr), "%s_%06d", buffer, (int)(ms));
}

void ATU_Timer::setTimeStamp() {
	double curms=m_ShareMemoryTimerData->m_TimeStampms;
	time_t curs=(time_t)(curms/1000);
	if (m_lastSec != curs) {
		setTimeStamp_YMDHMS(curms);
	} else {
		double ms=curms*1000-curs*1000*1000;
		sprintf((char*) (&m_TimeStampStr[16]), "%06d",
				(int)(ms));
	}
	m_lastSec = curs;
/*
	if (m_lastSec != m_ShareMemoryTimerData->m_tS.tv_sec
			|| (m_ShareMemoryTimerData->m_tS.tv_nsec / 1000) < 100) {
		setTimeStamp_YMDHMS();
	} else {
		sprintf((char*) (&m_TimeStampStr[16]), "%06d",
				(int)(m_ShareMemoryTimerData->m_tS.tv_nsec / 1000));
	}
	m_lastSec = m_ShareMemoryTimerData->m_tS.tv_sec;
*/
}
string ATU_Timer::getTimeStamp() {
	setTimeStamp();
	return string((char*) (m_TimeStampStr));
}
string ATU_Timer::MS2STR1(double ms)
{
	//this timestamp format match ORC
  time_t rawtime;
  struct tm * timeinfo;
  struct tm timeinfo2;
  char buffer [200];
  double curmsec=ms;
  rawtime=(long long)(curmsec/1000.0);

  timeinfo = localtime_r( (time_t*)(&(rawtime)),&timeinfo2 );
  timeinfo=&timeinfo2;
  strftime (buffer,200,"%H:%M:%S",timeinfo);
  char outtime[1000];
  double justms=(curmsec-floor(curmsec/1000.0)*1000.0)*100;

  sprintf(outtime,"%s.%05ld",buffer,(long)justms);
  return string(outtime);

}
string ATU_Timer::MS2STR3(double ms)
{
	//this timestamp format match omex exchange
  time_t rawtime;
  struct tm * timeinfo;
  struct tm timeinfo2;
  char buffer [200];
  double curmsec=ms;
  rawtime=(long long)(curmsec/1000.0);

  timeinfo = localtime_r( (time_t*)(&(rawtime)),&timeinfo2 );
  timeinfo=&timeinfo2;
  strftime (buffer,200,"%Y%m%d%H%M%S",timeinfo);
 // char outtime[1000];
//  double justms=(curmsec-floor(curmsec/1000.0)*1000.0)*100;

 // sprintf(outtime,"%s.%05d",buffer,(long)justms);
  return string(buffer);

}
string ATU_Timer::MS2STR4(double ms)
{
	//this timestamp format match omex exchange
  time_t rawtime;
  struct tm * timeinfo;
  struct tm timeinfo2;
  char buffer [200];
  double curmsec=ms;
  rawtime=(long long)(curmsec/1000.0);

  timeinfo = localtime_r( (time_t*)(&(rawtime)),&timeinfo2 );
  timeinfo=&timeinfo2;
  strftime (buffer,200,"%H%M%S",timeinfo);
 // char outtime[1000];
//  double justms=(curmsec-floor(curmsec/1000.0)*1000.0)*100;

 // sprintf(outtime,"%s.%05d",buffer,(long)justms);
  return string(buffer);

}

string ATU_Timer::MS2STR2(double ms)
{
	//this timestamp format match ORC
  time_t rawtime;
  struct tm * timeinfo;
  struct tm timeinfo2;
  char buffer [200];
  double curmsec=ms;
  rawtime=(long long)(curmsec/1000.0);

  timeinfo = localtime_r( (time_t*)(&(rawtime)),&timeinfo2 );
  timeinfo=&timeinfo2;
  strftime (buffer,200,"%Y%m%d",timeinfo);
 // char outtime[1000];
//  double justms=(curmsec-floor(curmsec/1000.0)*1000.0)*100;

 // sprintf(outtime,"%s.%05d",buffer,(long)justms);
  return string(buffer);

}
string ATU_Timer::MS2STR5(double curms) {
	char TimeStampStr[200];
	struct tm * timeinfo;
	struct tm timeinfo2;
	char buffer[200];
	//struct timeb tp;
	time_t tvsec=(time_t)(curms/1000);
	timeinfo = localtime_r((time_t*) (&(tvsec)), &timeinfo2);
	strftime(buffer, 200, "%Y%m%d_%H%M%S", &timeinfo2);
	double ms=curms*1000-tvsec*1000*1000;
	sprintf((char*) (TimeStampStr), "%s_%06d", buffer, (int)(ms));
	return TimeStampStr;
}
void ATU_Timer::stop(){
	m_isShutdown = true;

}

}
