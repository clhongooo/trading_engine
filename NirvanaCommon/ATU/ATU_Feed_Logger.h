/*
 * ATU_Feed_Logger.h
 *
 *  Created on: Jun 5, 2014
 *      Author: alex
 */

#ifndef ATU_FEED_LOGGER_H_
#define ATU_FEED_LOGGER_H_
#include "PCH.h"
#include "Constants.h"
#include "ATU_Timer.h"
#include "LogStream.h"
#include <queue>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
using namespace std;
using namespace ost;
namespace atu{
	class ATU_Feed_Logger : public Thread{
		private:
		void run(void);
		void final(void);
		public:
		ATU_Feed_Logger(string datapath,int timershareMemoryKey, string feedtype);
		virtual ~ATU_Feed_Logger();

		//1: old file 2: new file
		int setFilename(string filename);
		string m_Datapath;
		string m_FileName;
		int writeLog(int &size);
		int addLog(string msg,int writeToConsole=0);
		static bool addLog(ATU_Feed_Logger*, string msg, int writeToConsole=0);
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


#endif /* ATU_FEED_LOGGER_H_ */
