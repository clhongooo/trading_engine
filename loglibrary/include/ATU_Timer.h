#ifndef _ATU_TIMER_H_
#define _ATU_TIMER_H_
#include <cmath>
//#include "config.h"
#include <cc++/socket.h>
#include <stdlib.h>
#include <sys/shm.h>
//#include "Constants.h"
using namespace std;
using namespace ost;
namespace atu
{
typedef struct atu_share_memory_timer
{
	double volatile m_TimeStampms;
	timespec volatile m_tS;
} atu_share_memory_timer_t;

class ATU_Timer : public Thread
{
private:
	void run(void);
	void final(void);
public:
	ATU_Timer(int key);
	virtual ~ATU_Timer();
	bool createShareMemoryTimerData(int key);
	inline double getTimeInMS() { return m_ShareMemoryTimerData->m_TimeStampms; }
	atu_share_memory_timer volatile *m_ShareMemoryTimerData;
	int m_shareMemoryKey;
	bool m_isMaster;
	bool isMasterAlive();
	bool isReady();
	void waitUntilReady();
	bool init();
	char volatile m_TimeStampStr[100];
	time_t volatile  m_lastSec;
//	long long volatile m_lastSec;
	string getTimeStamp();
	void setTimeStamp_YMDHMS();
	void setTimeStamp_YMDHMS(double curms);
	void setTimeStamp();
	string MS2STR1(double ms);
	string MS2STR2(double ms);
	string MS2STR3(double ms);
	string MS2STR4(double ms);
	string MS2STR5(double ms);
	void stop();
	bool m_isShutdown;
};

}
#endif //_ATU_TIMER_H_
