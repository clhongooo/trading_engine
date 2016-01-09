/*
 * ATU_Feed_Logger.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: alex
 */
#include "ATU_Feed_Logger.h"
namespace atu{

ATU_Feed_Logger::ATU_Feed_Logger(string datapath,int timerShareMemoryKey, string feedtype) :m_isShutdown(false),m_Timer(NULL),m_isReady(false)
{
	m_Timer = new ATU_Timer(timerShareMemoryKey);
	m_Timer->detach();
	m_Timer->init();
	//usleep(1000 * 1000 * _ATU_Feed_Logger_INITIAL_SLEEP_SECOND_);
	//m_Timer->setTimeStamp(); // This will be called when getTimStamp
	//m_Timer->setTimeStamp_YMDHMS(); // Still need?
	string timestamp = m_Timer->getTimeStamp();
	m_StartLogTimeStamp = timestamp;
	m_Datapath = datapath + "/" +feedtype;
	char cmd[300];
	sprintf(cmd,"mkdir -p %s",m_Datapath.c_str());
	system(cmd);
	usleep(1000);
	m_FileName="";
//	usleep(1000*5);

}

ATU_Feed_Logger::~ATU_Feed_Logger() {
//	<< "Destroying ATU_Feed_Logger..." << endl;
//	.flush();
}
bool ATU_Feed_Logger::isReady() {
	return m_isReady;
}
int ATU_Feed_Logger::setFilename(string filename){
	if(filename == m_FileName) return 1;
	if(m_FileName!="") {
		m_GeneralLogFile.close();
		m_GeneralLogFile.clear();
	}
	m_FileName = filename;
	char outFileName[1000];
	sprintf(outFileName, "%s/%s.csv", m_Datapath.c_str(), m_FileName.c_str());
	m_GeneralLogFile.open(outFileName);

	m_LogSecStat=true;
	m_TimestampToLogMsgDelimiter=" : ";
	return 2;
}
void ATU_Feed_Logger::run(void) {
	if (m_Timer==NULL) {
//		<< "m_Timer is NULL" << endl;
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
void ATU_Feed_Logger::final(void) {
//	<< "finalizing Logger" << endl;
//	.flush();
	m_GeneralLogFile.close();

}
bool ATU_Feed_Logger::addLog(ATU_Feed_Logger *logger ,string msg, int writeToConsole) {
	if (logger!=NULL) {
		logger->addLog(msg, writeToConsole);
	} else {
		cout << msg << endl;
	}
	return true;
}
int ATU_Feed_Logger::addLog(string msg, int writeToConsole) {
	//	assert(msg.size()!=0);

	if (msg.length()==0) return 0;
	string msgout = "";
	m_LogMsgQueueMutex.enterMutex();

	msgout = msg;

	//adding timestamp to msg;
	m_LogMsgQueue.push(msgout);
	m_LogMsgQueueWriteToConsole.push(writeToConsole);
	m_LogMsgQueueMutex.leaveMutex();
	m_LogMsgQueueLock.signal(true);
	return true;
}
int ATU_Feed_Logger::writeLog(int &size) {
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
void ATU_Feed_Logger::backupFileCopy(string filename,string directory)
{
	char outFileName[3000];
	char srcFileName[3000];
	sprintf(outFileName,"%s/%s/%s.%s",m_Datapath.c_str(),directory.c_str(),filename.c_str(),m_Timer->getTimeStamp().c_str());
	sprintf(srcFileName,"%s/%s",m_Datapath.c_str(),filename.c_str());
	char cmd[3000];
	sprintf(cmd,"cp %s %s",srcFileName,outFileName);
	system(cmd);
}
void ATU_Feed_Logger::stop() {
//======== new method ============
	m_isShutdown2 = false;
	m_isShutdown = true;
	usleep(1000);
	m_Timer->stop();
//	<< "Assigning m_Timer=NULL" << endl;
//	.flush();
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
