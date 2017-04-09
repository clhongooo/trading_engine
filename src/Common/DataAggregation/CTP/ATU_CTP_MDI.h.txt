/*
 * ATU_CTP_MDI.h
 *
 *  Created on: May 8, 2014
 *      Author: alex
 */

#ifndef ATU_CTP_MDI_H_
#define ATU_CTP_MDI_H_
#include "ATU_Abstract_MDI.h"
//#include "ATU_TCP_MDI_string_handler.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include "ATU_ErrorMsgStruct.h"
#include <map>
#include <dlfcn.h>
#include <boost/thread.hpp>
using namespace std;
#include "ThostFtdcMdApi.h"
#include "Toolbox.h"
//--------------------------------------------------
// Sunny added - [start]
//--------------------------------------------------
#include "GlobalVariables.h"
//--------------------------------------------------
// Sunny added - [end]
//--------------------------------------------------

namespace atu{
	class ATU_CTP_MDI: public ATU_Abstract_MDI, public CThostFtdcMdSpi{
	public:
		/** Default constructor */
		ATU_CTP_MDI();
		virtual void init();
		/** Default destructor */
		virtual ~ATU_CTP_MDI();
		virtual bool notify_marketfeed(ATU_MDI_marketfeed_struct &s);
		virtual bool on_process_subscription(ATU_MDI_subscription_struct &s);
		virtual void detach();

		virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo,int nRequestID, bool bIsLast);

		virtual void OnFrontDisconnected(int nReason);

		virtual void OnHeartBeatWarning(int nTimeLapse);

		virtual void OnFrontConnected();

		virtual void ReqUserLogin();

		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

		virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount);

		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData);

		void Md_api_release();

		virtual void setConnectString(string address);

		int m_iRequestID;

		virtual void addLog(string logSource,int logSeverity,char *argsfmt,char *argtype,...);

	protected:

	private:
		void *m_ctp_lib_handle;
		boost::thread *m_init_and_run_thread;

		CThostFtdcMdApi *pMdApi;
		char m_server_address[300];
		TThostFtdcBrokerIDType m_broker_id;
		TThostFtdcInvestorIDType m_investor_id;
		TThostFtdcPasswordType m_password;
		TThostFtdcFrontIDType m_front_id;
		TThostFtdcSessionIDType m_session_id;

		bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
		{
			bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
			if (bResult)
				addLog(__LOGSOURCE__,ATU_logfeed_struct::ERROR,"ErrorID=%s, ErrorMsg=%s","ss",pRspInfo->ErrorID,pRspInfo->ErrorMsg);
			return bResult;
		}

		set<string> m_subscribeList;

	};
}


#endif /* ATU_CTP_MDI_H_ */
