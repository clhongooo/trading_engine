#ifndef _ATU_LOGGER_H_
#define _ATU_LOGGER_H_
//#include "config.h"
#include "ATU_Timer.h"
#include "Constants.h"
#include "LogStream.h"
#include <queue>
using namespace std;
using namespace ost;
namespace atu
{
class ATU_Logger;
class ATU_Logger : public Thread
{
private: 
	void run(void);
	void final(void);
public: 
	ATU_Logger();
	ATU_Logger(string datapath,string filename,int timershareMemoryKey);
	virtual ~ATU_Logger();

	string m_Datapath;
	string m_FileName;
	int writeLog(int &size);
	int addLog(string msg,int writeToConsole=0);
	static bool addLog(ATU_Logger*, string msg, int writeToConsole=0);
	void backupFileCopy(string fielanme,string directory);
	void stop();
	bool isReady();
	queue<string> m_LogMsgQueue;
	queue<int> m_LogMsgQueueWriteToConsole;
	ofstream m_GeneralLogFile;
	Conditional m_LogMsgQueueLock;
	Mutex m_LogMsgQueueMutex;
	ATU_Timer *m_Timer;
	bool m_LogSecStat;
	string m_TimestampToLogMsgDelimiter;
private:
	bool m_isShutdown;
	bool m_isShutdown2;
	bool m_isReady;
public:

	time_t volatile  m_lastSec;
	bool m_isMaster;
	string m_StartLogTimeStamp;
};


}
#endif //_ATU_LOGGER_H_
