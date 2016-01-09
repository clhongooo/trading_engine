#include "ATU_Logger.h"
using namespace ost;
using namespace std;
namespace atu {
ATU_Logger::ATU_Logger(string datapath, string filename,int timerShareMemoryKey) :m_isShutdown(false),m_Timer(NULL),m_isReady(false)
{

	m_Timer = new ATU_Timer(timerShareMemoryKey);
	m_Timer->detach();
	m_Timer->init();
	//usleep(1000 * 1000 * _ATU_LOGGER_INITIAL_SLEEP_SECOND_);
	//m_Timer->setTimeStamp(); // This will be called when getTimStamp
	//m_Timer->setTimeStamp_YMDHMS(); // Still need?

	m_Datapath = datapath;
	m_FileName = filename;
	char outFileName[1000];
	char outFileName2[1000];
	char outFileName3[1000];
	string timestamp = m_Timer->getTimeStamp();
	m_StartLogTimeStamp = timestamp;
	sprintf(outFileName, "%s/%s/%s.%s.%s", m_Datapath.c_str(), "log", filename.c_str(), timestamp.c_str(), "log");
	sprintf(outFileName2, "%s/%s/%s.%s", m_Datapath.c_str(), "log", filename.c_str(), "log");
	sprintf(outFileName3, "%s.%s.%s", filename.c_str(), timestamp.c_str(), "log");
	m_GeneralLogFile.open(outFileName);
	char cmd[3000];
	sprintf(cmd, "rm %s", outFileName2);
	system(cmd);

	sprintf(cmd, "ln -s %s %s", outFileName3, outFileName2);
	system(cmd);
	m_LogSecStat=true;
	m_TimestampToLogMsgDelimiter=" : ";
//	usleep(1000*5);

}
ATU_Logger::~ATU_Logger() {
//#ifdef _ATUDEBUG_
	cout << "Destroying ATU_Logger..." << endl;
	cout.flush();
//#endif // _ATUDEBUG_
}
bool ATU_Logger::isReady() {
	return m_isReady;
}
void ATU_Logger::run(void) {
	if (m_Timer==NULL) {
		cout << "m_Timer is NULL" << endl;
	}
	if (m_Timer->m_isMaster) {
		if (m_LogSecStat) {
			addLog("Start running Logger with Timer in Master mode",2);
			// TODO:
			// If Timer service is not started, the whole application (and their children) should exit (CCF 20130520)
			::exit(1);
		}
	} else {
		if (m_LogSecStat) {
			addLog("Start running Logger with Timer in Slave mode",2);
		}
	}
	int count = 0;
	time_t lastSec=0;
	int writesize = 0;
	while (!m_isShutdown && m_Timer!=NULL) {
		m_isReady=true;
		if (m_Timer->m_ShareMemoryTimerData->m_tS.tv_sec != lastSec) {
			if (m_LogSecStat) {
				LogStream cooo(5);
				cooo << "Logger:Writing " << count << " log msg per sec with ";
				cooo << (double) writesize / (double) 1024 << "kb "
					<< " pending queue=size=" << m_LogMsgQueue.size();
				addLog(cooo.str(), 0);
			}
			count = 0;
			lastSec = m_Timer->m_ShareMemoryTimerData->m_tS.tv_sec;
			writesize = 0;
		}
		lastSec = m_Timer->m_ShareMemoryTimerData->m_tS.tv_sec;
		int size = 0;
		//int ret = writeLog(size);
		writeLog(size);
		writesize += size;
		if (size > 0) {
			count++;
		} else {
			m_LogMsgQueueLock.wait(1, true);
		}
	}

	while (!m_isShutdown2) {
		usleep(1000);
	}

}
void ATU_Logger::final(void) {
	cout << "finalizing Logger" << endl;
	cout.flush();
	m_GeneralLogFile.close();

}
bool ATU_Logger::addLog(ATU_Logger *logger ,string msg, int writeToConsole) {
	if (logger!=NULL) {
		logger->addLog(msg, writeToConsole);
	} else {
		cout << msg << endl;
	}
	return true;
}
int ATU_Logger::addLog(string msg, int writeToConsole) {
	//	assert(msg.size()!=0);

	if (msg.length()==0) return 0;
	string msgout = "";
	m_LogMsgQueueMutex.enterMutex();

	msgout = m_Timer->getTimeStamp() + m_TimestampToLogMsgDelimiter + msg;

	//adding timestamp to msg;
	m_LogMsgQueue.push(msgout);
	m_LogMsgQueueWriteToConsole.push(writeToConsole);
	m_LogMsgQueueMutex.leaveMutex();
	m_LogMsgQueueLock.signal(true);
	return true;
}
int ATU_Logger::writeLog(int &size) {
	size = 0;
	string msg;
	int writeToConsole = 0;
	m_LogMsgQueueMutex.enterMutex();
	if (!m_LogMsgQueue.empty()) {
		msg = m_LogMsgQueue.front();
		writeToConsole = m_LogMsgQueueWriteToConsole.front();
		m_LogMsgQueue.pop();
		m_LogMsgQueueWriteToConsole.pop();
		m_LogMsgQueueMutex.leaveMutex();
		size = msg.length();
		m_GeneralLogFile << msg << endl;
		m_GeneralLogFile.flush();
		if (writeToConsole != 0) {
			cout << msg << endl;
		}
	} else {
		m_LogMsgQueueMutex.leaveMutex();
	}
	return true;
}
void ATU_Logger::backupFileCopy(string filename,string directory)
{
	char outFileName[3000];
	char srcFileName[3000];
	sprintf(outFileName,"%s/%s/%s.%s",m_Datapath.c_str(),directory.c_str(),filename.c_str(),m_Timer->getTimeStamp().c_str());
	sprintf(srcFileName,"%s/%s",m_Datapath.c_str(),filename.c_str());
	char cmd[3000];
	sprintf(cmd,"cp %s %s",srcFileName,outFileName);
	system(cmd);
}
void ATU_Logger::stop() {
//======== new method ============
	m_isShutdown2 = false;
	m_isShutdown = true;
	usleep(1000);
	m_Timer->stop();
	cout << "Assigning m_Timer=NULL" << endl;
	cout.flush();
	m_Timer=NULL;
	usleep(1000);
	m_isShutdown2 = true;
//================================
/*
	m_Timer->stop();
	while(m_Timer->isRunning()) {
		usleep(1000);
	}
	m_isShutdown = true;
*/
}

}
