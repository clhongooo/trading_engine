/*
 * ATU_CTP_MDI.cpp
 *
 *  Created on: May 8, 2014
 *      Author: alex
 */

#include "ATU_CTP_MDI.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
using namespace boost::filesystem;
using namespace boost::interprocess;

namespace atu{

	ATU_CTP_MDI::ATU_CTP_MDI() : ATU_Abstract_MDI() {
    //--------------------------------------------------
    // Sunny added - [start]
    //--------------------------------------------------
    cout << "CTP MDI: SVN revision no. (CTP MDI): "          << string(__SVN_REV__)       << endl;
    cout << "CTP MDI: SVN revision no. (Entire workspace): " << string(__SVN_WKSPC_REV__) << endl;
    //--------------------------------------------------
    // Sunny added - [end]
    //--------------------------------------------------

		//ATU_Abstract_MDI::ATU_Abstract_MDI();
		strcpy(m_server_address,"tcp://222.66.166.147:41214");
		m_subscribeList.clear();
	}
	void ATU_CTP_MDI::detach(){
		m_init_and_run_thread=new boost::thread(boost::bind(&ATU_CTP_MDI::init,this));
	}
	void ATU_CTP_MDI::setConnectString(string address) {
		strcpy(m_server_address,address.c_str());
	}
	void ATU_CTP_MDI::init(){
		m_ctp_lib_handle=dlopen("libthostmduserapi.so",RTLD_NOW);
		if (m_ctp_lib_handle!=NULL) {
			addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"CTP mdapi loaded","");
		} else {
			addLog(__LOGSOURCE__,ATU_logfeed_struct::ERROR,"CTP mdapi NOT loaded","");
		}
		typedef CThostFtdcMdApi* (*CreateFtdcMdApiPtr)(const char *, bool, bool);
		CreateFtdcMdApiPtr CreateFtdcMdApi= (CreateFtdcMdApiPtr)dlsym(m_ctp_lib_handle,"_ZN15CThostFtdcMdApi15CreateFtdcMdApiEPKcbb");

		pMdApi=CreateFtdcMdApi("",true,true);

//	    strcpy(m_server_address,"tcp://180.166.165.179:41213");
//		strcpy(m_broker_id,"1013");
//		strcpy(m_investor_id,"00000062");
//		strcpy(m_password,"834591");
		pMdApi->RegisterSpi((CThostFtdcMdSpi*) this);
		pMdApi->RegisterFront(m_server_address);
		pMdApi->Init();
		if (pMdApi!=NULL) {
			pMdApi->Join();
		}
	}

	ATU_CTP_MDI::~ATU_CTP_MDI(){

	}

	void ATU_CTP_MDI::OnFrontConnected() {
		addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Front is connected","");
		ReqUserLogin();
	}

	void ATU_CTP_MDI::ReqUserLogin(){
		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_broker_id);
		strcpy(req.UserID, m_investor_id);
		strcpy(req.Password, m_password);
		int iResult = pMdApi->ReqUserLogin(&req, ++m_iRequestID);
		addLog(__LOGSOURCE__,ATU_logfeed_struct::CRITICAL,"User Login is %s","s",(iResult == 0) ? "OK" : "Fail");

		for(set<string>::iterator iter = m_subscribeList.begin(); iter != m_subscribeList.end(); iter++){
			char * _ppInstrumentID[2000]; int nCount = 0;
			char _pInst[100];
			strcpy(_pInst, (*iter).c_str());
			_ppInstrumentID[nCount++] = _pInst;
			SubscribeMarketData(_ppInstrumentID,nCount);
		}

	}

	void ATU_CTP_MDI::OnRspError(CThostFtdcRspInfoField* pRspInfo,
	                        int nRequestID, bool bIsLast){
		addLog(__LOGSOURCE__,ATU_logfeed_struct::ALERT,"Error is Found!","");
		IsErrorRspInfo(pRspInfo);
	}

	void ATU_CTP_MDI::OnFrontDisconnected(int nReason){
		addLog(__LOGSOURCE__,ATU_logfeed_struct::EMERGENCY,"Front is disconnected!","");
	}

	void ATU_CTP_MDI::OnHeartBeatWarning(int nTimeLapse){

	}

	void ATU_CTP_MDI::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast){
		if (pRspInfo->ErrorID != 0) {
			char _errorID[20];
			sprintf(_errorID,"%d",pRspInfo->ErrorID);
		}

	}

	bool ATU_CTP_MDI::on_process_subscription(ATU_MDI_subscription_struct &s) {
		char * _ppInstrumentID[2000]; int nCount = 0;
		memset(_ppInstrumentID, 0 , sizeof(_ppInstrumentID));
		char _pInst[100];
		strcpy(_pInst, s.m_feedcode.c_str());
		m_subscribeList.insert(s.m_feedcode);
		_ppInstrumentID[nCount++] = _pInst;
		SubscribeMarketData(_ppInstrumentID,nCount);
		return true;
	}

	int ATU_CTP_MDI::SubscribeMarketData(char *ppInstrumentID[], int nCount){
		for(int i = 0 ; i < nCount; i++) {
			addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Subscribe %s.","s",ppInstrumentID[i]);
		}
		int ret = pMdApi->SubscribeMarketData(ppInstrumentID, nCount);
		return ret;
	}

	void ATU_CTP_MDI::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast){

	}

	void ATU_CTP_MDI::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
	{
		ATU_MDI_marketfeed_struct mf;
		string tradingDay = pDepthMarketData->TradingDay;
		string updateTime = pDepthMarketData->UpdateTime;
		string updateMillisec = boost::lexical_cast<string>(pDepthMarketData->UpdateMillisec);
		Toolbox::FindAndReplace(updateTime,":","");
		char tmpstr[10];
		sprintf(tmpstr,"%06d",pDepthMarketData->UpdateMillisec*1000);
		mf.m_timestamp = tradingDay+"_"+updateTime+"_"+string(tmpstr);
		mf.m_feedcode = pDepthMarketData->InstrumentID;
		mf.m_traded_price = (pDepthMarketData->LastPrice == DBL_MAX) ? 999999 : pDepthMarketData->LastPrice;
		mf.m_traded_volume = (pDepthMarketData->Volume == DBL_MAX) ? 999999 : pDepthMarketData->Volume;
		mf.m_ask_price_1 = (pDepthMarketData->AskPrice1 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice1;
		mf.m_ask_volume_1 = (pDepthMarketData->AskVolume1 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume1;
		mf.m_ask_price_2 = (pDepthMarketData->AskPrice2 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice2;
		mf.m_ask_volume_2 = (pDepthMarketData->AskVolume2 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume2;
		mf.m_ask_price_3 = (pDepthMarketData->AskPrice3 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice3;
		mf.m_ask_volume_3 = (pDepthMarketData->AskVolume3 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume3;
		mf.m_ask_price_4 = (pDepthMarketData->AskPrice4 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice4;
		mf.m_ask_volume_4 = (pDepthMarketData->AskVolume4 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume4;
		mf.m_ask_price_5 = (pDepthMarketData->AskPrice5 == DBL_MAX) ? 999999 : pDepthMarketData->AskPrice5;
		mf.m_ask_volume_5 = (pDepthMarketData->AskVolume5 == DBL_MAX) ? 999999 : pDepthMarketData->AskVolume5;

		mf.m_bid_price_1 = (pDepthMarketData->BidPrice1 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice1;
		mf.m_bid_volume_1 = (pDepthMarketData->BidVolume1 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume1;
		mf.m_bid_price_2 = (pDepthMarketData->BidPrice2 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice2;
		mf.m_bid_volume_2 = (pDepthMarketData->BidVolume2 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume2;
		mf.m_bid_price_3 = (pDepthMarketData->BidPrice3 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice3;
		mf.m_bid_volume_3 = (pDepthMarketData->BidVolume3 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume3;
		mf.m_bid_price_4 = (pDepthMarketData->BidPrice4 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice4;
		mf.m_bid_volume_4 = (pDepthMarketData->BidVolume4 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume4;
		mf.m_bid_price_5 = (pDepthMarketData->BidPrice5 == DBL_MAX) ? 999999 : pDepthMarketData->BidPrice5;
		mf.m_bid_volume_5 = (pDepthMarketData->BidVolume5 == DBL_MAX) ? 999999 : pDepthMarketData->BidVolume5;
		//--------------------------------------------------
		// Sunny added - [start]
		//--------------------------------------------------
		boost::lock_guard<boost::recursive_mutex> lock(g_Mutex);
		//--------------------------------------------------
		// Sunny added - [end]
		//--------------------------------------------------
		notify_marketfeed(mf);
	}

	bool ATU_CTP_MDI::notify_marketfeed(ATU_MDI_marketfeed_struct &s){
		if (m_marketfeed_call_back_func!=NULL) {
			return (*m_marketfeed_call_back_func)(s);
		} else {
			return false;
		}
	}

	void ATU_CTP_MDI::Md_api_release(){
		if (pMdApi != NULL) {
			pMdApi->RegisterSpi(NULL);
			pMdApi->Release();
			pMdApi = NULL;
		}
	}

	void ATU_CTP_MDI::addLog(string logSource,int logSeverity,char *argsfmt,char *argtype,...) {
			va_list listPointer;
			va_start(listPointer,argtype);

			ATU_logfeed_struct *logfeed=copy2logfeed_struct(logSource,
					logSeverity,argsfmt,argtype, listPointer);
			notify_logfeed(logfeed);

		}
}



