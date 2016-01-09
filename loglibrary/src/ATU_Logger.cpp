#include "ATU_Logger.h"
#include "Constants.h"
#include <boost/format.hpp>
using namespace ost;
using namespace std;
namespace atu {
#define LOGGER_LOCKFREE_QUEUE_SIZE 10000
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
	m_logfeed_queue=new boost::lockfree::queue< ATU_logfeed_struct* >(LOGGER_LOCKFREE_QUEUE_SIZE);
//	m_logfeed_queue=new queue< ATU_logfeed_struct* >();
//	usleep(1000*5);

}
ATU_Logger::~ATU_Logger() {
//#ifdef _ATUDEBUG_
//	<< "Destroying ATU_Logger..." << endl;
//	.flush();
	//delete m_logfeed_queue;
//#endif // _ATUDEBUG_
}
bool ATU_Logger::isReady() {
	return m_isReady;
}
void ATU_Logger::run(void) {
	if (m_Timer==NULL) {
//		<< "m_Timer is NULL" << endl;
	}
	if (m_Timer->m_isMaster) {
		if (m_LogSecStat) {
			addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Start running Logger with Timer in Master mode","");
			// TODO:
			// If Timer service is not started, the whole application (and their children) should exit (CCF 20130520)
			::exit(1);
		}
	} else {
		if (m_LogSecStat) {
			addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Start running Logger with Timer in Slave mode","");
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
				cooo << (double) writesize / (double) 1024 << "kb ";
//					<< " pending queue size=" << m_LogMsgQueue.size()
//					<< " pending queue size=" << m_logfeed_queue->size();
//				addLog(__LOGSOURCE__,ATU_logfeed_struct::DEBUG,(char*)(cooo.str().c_str()), "");
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
		writeLogFeed(size);
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
//	<< "finalizing Logger" << endl;
//	.flush();
	m_GeneralLogFile.close();

}
/*
int ATU_Logger::addLog(string logSource,int logSeverity,char *argsfmt,char *argtype,...) {
	va_list listPointer;
	va_start(listPointer,argtype);

	ATU_logfeed_struct *logfeed=copy2logfeed_struct(logSource,
			logSeverity,argsfmt,argtype, listPointer);
	on_notify_logfeed(logfeed);

}
*/
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
//	m_LogMsgQueueLock.signal(true);
	return true;
}

int ATU_Logger::writeLogFeed(int &size) {
//	size=0;
//	return 1;

	try {
		ATU_logfeed_struct *s;
		if (m_logfeed_queue!=NULL && !m_logfeed_queue->empty()) {

			m_logfeed_queue->pop(s);
			boost::format msg(s->m_logFmt);
			int output=0;
			string logSeverity="";
			string logFormat="";
			switch (s->m_logSeverity) {
				case ATU_logfeed_struct::EMERGENCY: logSeverity="EMERGENCY"; output=2; break;
				case ATU_logfeed_struct::ALERT: logSeverity="ALERT"; output=2; break;
				case ATU_logfeed_struct::CRITICAL: logSeverity="CRITICAL"; output=2; break;
				case ATU_logfeed_struct::ERROR: logSeverity="ERROR"; output=2; break;
				case ATU_logfeed_struct::WARNING: logSeverity="WARNING"; output=2; break;
				case ATU_logfeed_struct::NOTICE:  logSeverity="NOTICE"; output=2; break;
				case ATU_logfeed_struct::INFO: logSeverity="INFO"; output=2; break;
				case ATU_logfeed_struct::DEBUG: logSeverity="DEBUG"; output=2; break;
			}
			switch (s->m_logFormat) {
				case ATU_logfeed_struct::PLAINTEXT: logFormat="PLAINTEXT"; break;
				case ATU_logfeed_struct::JSON: logFormat="JSON"; break;
			}

			int ptrloc = 0;
			for (unsigned int i=0;i<strlen(s->m_logArgType);i++) {
				int datalength;
				char dtype=getDatatypeStringAndSize(s->m_logArgType[i],datalength);
				switch (dtype) {
					case 'd': {
						int data = *(reinterpret_cast<int *>(s->m_logData+ptrloc));
						ptrloc += datalength;
						msg % data;
						break;
					}
					case 'f': {
						double data = *(reinterpret_cast<double *>(s->m_logData+ptrloc));
						ptrloc += datalength;
						msg % data;
						break;
					}
					case 'l': {
						long data = *(reinterpret_cast<long *>(s->m_logData+ptrloc));
						ptrloc += datalength;
						msg % data;
						break;
					}
					case 's': {
						char * data = (reinterpret_cast<char *>(s->m_logData+ptrloc));
						datalength = strlen(data) + 1;
						ptrloc += datalength;
						msg % data;
						break;
					}
				}
			}
			string timeStamp=m_Timer->MS2STR5(s->m_timeInMS);
			string tmpstr=msg.str();
			//tmpstr.erase(tmpstr.find_last_not_of("\n")+1);
			FindAndReplace(tmpstr,"\n","");
			string logmsg=timeStamp+","+logFormat+","+logSeverity+","+tmpstr+","+s->m_logSource;
			size=logmsg.length();
			m_GeneralLogFile << logmsg << endl;
			m_GeneralLogFile.flush();

			if (output != 0) {
				cout << logmsg << endl;
				cout.flush();
			}
			if (s->m_logArgType!=NULL) free(s->m_logArgType);
			if (s->m_logFmt!=NULL) free(s->m_logFmt);
			if (s->m_logData!=NULL) free(s->m_logData);
			delete s;
		}
	}
	catch(std::exception &e)
	{
		cout << "ATU_Logger writeLogFeed pop error" << std::endl;
	}

}
void ATU_Logger::FindAndReplace( std::string& tInput, std::string tFind, std::string tReplace )
{
        size_t uPos = 0;
        size_t uFindLen = tFind.length();
        size_t uReplaceLen = tReplace.length();
        if( uFindLen == 0 )
        {
                return;
        }

	for( ;(uPos = tInput.find( tFind, uPos )) != std::string::npos; )
        {
                tInput.replace( uPos, uFindLen, tReplace );
                uPos += uReplaceLen;
        }
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
