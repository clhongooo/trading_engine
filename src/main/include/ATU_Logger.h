#ifndef _ATU_LOGGER_H_
#define _ATU_LOGGER_H_
//#include "config.h"
#include "ATU_Timer.h"
#include "LogStream.h"
#include <queue>
#include <boost/lockfree/queue.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <cstdarg>
using namespace std;
using namespace ost;
#define __LOGSOURCE__ string(__FILE__)+","+string(__FUNCTION__)+","+boost::lexical_cast<string>(__LINE__) //+",VER"+__SVN_REV__
#define LOG_MAX_LENGTH 10000
typedef struct {
	enum LogLevel {EMERGENCY, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG};
	enum LogFormat { PLAINTEXT, JSON };
//	string m_logMessage;
	int m_logSeverity;
	int m_logFormat;
	string m_logSource;
//	string m_timeStamp;
	double m_timeInMS;

	char *m_logData;
	char *m_logFmt;
	char *m_logArgType;
} ATU_logfeed_struct;

typedef boost::function < void(ATU_logfeed_struct *s) > ATU_logfeed_CallBackFunc;

inline char getDatatypeStringAndSize(char datatype, int &size) {
	switch( datatype )
	{
		case 'd':
			size = sizeof( int );
			return datatype;
			break;
		case 'l':
			size = sizeof( long );
			return datatype;
			break;
		case 'f':
			size = sizeof ( double );
			return datatype;
			break;
		case 's':
			size = sizeof ( char * );
			return datatype;
			break;
		default:
			printf("getDatatypeStringAndSize didn't know the type %c\n",datatype);
			size = 0;
			return 0;
			break;
	}
	return 0;
}
inline ATU_logfeed_struct *copy2logfeed_struct(string &logSource,
		int logSeverity,char * argsfmt,char *argtype, va_list listPointer){

	char *buffer = (char *)malloc(LOG_MAX_LENGTH);
	int logptr = 0 , curLengthLimit = LOG_MAX_LENGTH;
	// fetch type from fmt string
	ATU_logfeed_struct *logfeed=new ATU_logfeed_struct();
	bool isOverflow = false;
	for (unsigned int i=0;i<strlen(argtype);i++) {
//		// get the next char, which tells
//		// what kind of data it is.
		int datalength=0;
		char dtype = getDatatypeStringAndSize(argtype[i],datalength);
		if( datalength == 0 ) {
			printf("FATAL ERROR: I didn't know datatype %c\n", dtype);
			logfeed->m_logFmt=NULL;
			logfeed->m_logArgType=NULL;
			logfeed->m_logData=NULL;
			return logfeed;
		}

//		// retrieve and copy the data.
		switch (dtype) {
			case 'd': {
				int data = va_arg( listPointer, int );
//				while(logptr+datalength> curLengthLimit) {
//					char * buffer2 = (char *)malloc(curLengthLimit * 2);
//					memcpy(buffer2,buffer,logptr);
//					free(buffer);
//					buffer = buffer2;
//					curLengthLimit = curLengthLimit * 2;
//				}
				if(logptr+datalength> curLengthLimit) {
					isOverflow = true;
				}
				else{
					memcpy( buffer+logptr, &data, datalength );
					logptr += datalength;
				}
				break;
			}
			case 'f': {
				double data = va_arg( listPointer, double );
//				while(logptr+datalength> curLengthLimit) {
//					char * buffer2 = (char *)malloc(curLengthLimit * 2);
//					memcpy(buffer2,buffer,logptr);
//					free(buffer);
//					buffer = buffer2;
//					curLengthLimit = curLengthLimit * 2;
//				}
				if(logptr+datalength> curLengthLimit) {
					isOverflow = true;
				}
				else{
					memcpy( buffer+logptr, &data, datalength );
					logptr += datalength;
				}
				break;
			}
			case 'l': {
				double data = va_arg( listPointer, long );
//				while(logptr+datalength> curLengthLimit) {
//					char * buffer2 = (char *)malloc(curLengthLimit * 2);
//					memcpy(buffer2,buffer,logptr);
//					free(buffer);
//					buffer = buffer2;
//					curLengthLimit = curLengthLimit * 2;
//				}
				if(logptr+datalength> curLengthLimit) {
					isOverflow = true;
				}
				else{
					memcpy( buffer+logptr, &data, datalength );
					logptr += datalength;
				}
				break;
			}
			case 's': {
				char * data = 0;
				data = va_arg( listPointer, char * );
				/*
				if(data < (char *)0x00010000 || data >(char *)0x7FFEFFFF){
					data = "FATAL ERROR: LOGFEED FORMAT ERROR! STRING WITHOUT NULL TERMINATOR!";
				}
				*/
				datalength = strlen(data)+1;
//				while(logptr+datalength> curLengthLimit) {
//					char * buffer2 = (char *)malloc(curLengthLimit * 2);
//					memcpy(buffer2,buffer,logptr);
//					free(buffer);
//					buffer = buffer2;
//					curLengthLimit = curLengthLimit * 2;
////					printf("BUFFER RESIZE to %d\n",curLengthLimit);
//				}
				if(logptr+datalength> curLengthLimit) {
					isOverflow = true;
				}
				else{
					memcpy( buffer+logptr, data, datalength );
					logptr += datalength;
				}
				break;
			}
			default: {
				printf("log_copy2logfeed_struct didn't know the type %c.\n",dtype );
				return logfeed; // type not known, so . . .
			}
		} //switch
		if (isOverflow) {
//			printf("FATAL ERROR: Buffer is OVERFLOW\n");
			break;
		}
	}
//	logfeed->m_logMessage="";
	logfeed->m_logSeverity=logSeverity;
	logfeed->m_logFormat=ATU_logfeed_struct::PLAINTEXT;
	logfeed->m_logSource=logSource;
	if(isOverflow){
		logfeed->m_logSeverity=ATU_logfeed_struct::ERROR;
		const char *errStr = "FATAL ERROR: Buffer is OVERFLOW";
		memcpy( buffer, errStr , strlen(errStr)+1 );
		buffer[strlen(errStr)+1] = 0 ;
		logfeed->m_logData = buffer;
		int fmtlen=3;
		logfeed->m_logFmt = (char*)malloc(fmtlen);
		memcpy(logfeed->m_logFmt,"%s",fmtlen);
		logfeed->m_logFmt[fmtlen] = 0;
		int argtypelen=2;
		logfeed->m_logArgType=(char*)malloc(argtypelen);
		memcpy(logfeed->m_logArgType,"s",argtypelen);
		logfeed->m_logArgType[argtypelen] = 0;
	}
	else{
		logfeed->m_logData = buffer;
		int fmtlen=strlen(argsfmt)+1;
		if(fmtlen>10000000) {
			printf("fmtlen is huge: %d\n",fmtlen );
			memcpy(logfeed->m_logFmt,"",0);
		}
		else {
			logfeed->m_logFmt = (char*)malloc(fmtlen);
			memcpy(logfeed->m_logFmt,argsfmt,fmtlen);
		}
		int argtypelen=strlen(argtype)+1;
		if(argtypelen>10000000) {
			printf("argtypelen is huge: %d\n",argtypelen );
			memcpy(logfeed->m_logArgType,"",0);
		}
		else {
			logfeed->m_logArgType=(char*)malloc(argtypelen);
			memcpy(logfeed->m_logArgType,argtype,argtypelen);
		}
	}
	va_end( listPointer );
	return logfeed;
}
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
	inline void on_notify_logfeed(ATU_logfeed_struct *s) {
		s->m_timeInMS=m_Timer->getTimeInMS();
//		s->m_timeInMS=m_Timer->getTimeInMS();
			try {
				if (m_logfeed_queue!=NULL) {
					m_logfeed_queue->push(s);
				}
			}
			catch(std::exception &e)
			{
				cout << "ATU_Logger on_notify_logfeed push error" << std::endl;
			}
		//adding timestamp to msg;
		//	m_LogMsgQueue.push(msgout);
		//	m_LogMsgQueueWriteToConsole.push(writeToConsole);
		//	m_LogMsgQueueMutex.leaveMutex();
	//	m_LogMsgQueueLock.signal(true);
	}
//	void on_notify_logfeed_(ATU_logfeed_struct &s);

	string m_Datapath;
	string m_FileName;
	int writeLog(int &size);
	int writeLogFeed(int &size);
	int addLog(string msg,int writeToConsole=0);
	static bool addLog(ATU_Logger*, string msg, int writeToConsole=0);
	inline int addLog(string logSource,int logSeverity,char *argsfmt,char *argtype,...) {
		va_list listPointer;
		va_start(listPointer,argtype);

		ATU_logfeed_struct *logfeed=copy2logfeed_struct(logSource,
				logSeverity,argsfmt,argtype, listPointer);
		on_notify_logfeed(logfeed);
	}
	void backupFileCopy(string fielanme,string directory);
	void FindAndReplace( std::string& tInput, std::string tFind, std::string tReplace );

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


	boost::lockfree::queue< ATU_logfeed_struct* >* m_logfeed_queue;
//	queue< ATU_logfeed_struct* > m_logfeed_queue;
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
