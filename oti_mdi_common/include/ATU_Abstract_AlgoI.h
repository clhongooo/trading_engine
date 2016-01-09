/*
 * ATU_Abstract_AlgoI.h
 *
 *  Created on: May 25, 2015
 *      Author: dt
 */

#ifndef ATU_ABSTRACT_ALGOI_H_
#define ATU_ABSTRACT_ALGOI_H_
#include "ATU_Abstract_OTI.h"
#include "ATU_Abstract_MDI.h"
#include "ATU_AccumulatedPerformance.h"
#include "ATU_AlgoStrategyPortfolioManager.h"
#include "ATU_DailyPerformance.h"
#include "ATU_SignalBasedPerformance.h"
#include "ATU_ErrorMsgStruct.h"
using namespace atu;
class ATU_Abstract_AlgoI {
public:
	ATU_Abstract_AlgoI(): m_signalfeed_call_back_func(NULL),m_portfolio_get_trade_history_call_back_func(NULL),m_portfolio_get_working_orders_call_back_func(NULL),
	m_portfolio_get_PnL_call_back_func(NULL),m_portfolio_get_daily_performance_call_back_func(NULL),m_portfolio_get_accum_performance_call_back_func(NULL),
	m_portfolio_get_pnl_performance_call_back_func(NULL),m_subscription_call_back_func(NULL),m_unsubscription_call_back_func(NULL),m_acknowledgement_call_back_func(NULL)
	{}
	virtual ~ATU_Abstract_AlgoI() {}
	vector<int> m_tradeBegin;
	vector<int> m_tradeEnd;
	string m_connectString;
	queue<void*> m_msgqueue;
	queue<int> m_msgType;
	boost::recursive_mutex m_msgqueueMutex;

	virtual void setConnectString(string connstr) {
		m_connectString=connstr;
	}
	virtual void register_signalfeed_call_back_func(ATU_OTI_signalfeed_CallBackFunc *callback) {
		if(m_signalfeed_call_back_func!=NULL) delete m_signalfeed_call_back_func;
		m_signalfeed_call_back_func=callback;
	}
	virtual void register_portfolio_get_trade_history_call_back_func(ATU_OTI_portfolio_get_trade_history_CallBackFunc *callback) {
		if(m_portfolio_get_trade_history_call_back_func!=NULL) delete m_portfolio_get_trade_history_call_back_func;
		m_portfolio_get_trade_history_call_back_func=callback;
	}
	virtual void register_portfolio_get_working_orders_call_back_func(ATU_OTI_portfolio_get_working_orders_CallBackFunc *callback) {
		if(m_portfolio_get_working_orders_call_back_func!=NULL) delete m_portfolio_get_working_orders_call_back_func;
		m_portfolio_get_working_orders_call_back_func=callback;
	}
	virtual void register_portfolio_get_PnL_call_back_func(ATU_OTI_portfolio_get_PnL_CallBackFunc *callback){
		if(m_portfolio_get_PnL_call_back_func!=NULL) delete m_portfolio_get_PnL_call_back_func;
		m_portfolio_get_PnL_call_back_func=callback;
	}
	virtual void register_portfolio_get_daily_performance_call_back_func(ATU_OTI_portfolio_get_daily_performance_CallBackFunc *callback){
		if(m_portfolio_get_daily_performance_call_back_func!=NULL) delete m_portfolio_get_daily_performance_call_back_func;
		m_portfolio_get_daily_performance_call_back_func=callback;
	}
	virtual void register_portfolio_get_accum_performance_call_back_func(ATU_OTI_portfolio_get_accum_performance_CallBackFunc *callback){
		if(m_portfolio_get_accum_performance_call_back_func!=NULL) delete m_portfolio_get_accum_performance_call_back_func;
		m_portfolio_get_accum_performance_call_back_func=callback;
	}
	virtual void register_portfolio_get_pnl_performance_call_back_func(ATU_OTI_portfolio_get_pnl_performance_CallBackFunc *callback){
		if(m_portfolio_get_pnl_performance_call_back_func!=NULL) delete m_portfolio_get_pnl_performance_call_back_func;
		m_portfolio_get_pnl_performance_call_back_func=callback;
	}
	virtual void register_process_subscription_call_back_func(
			ATU_MDI_subscription_CallBackFunc* callback) {
		if(m_subscription_call_back_func!=NULL) delete m_subscription_call_back_func;
		m_subscription_call_back_func = callback;
	}
	virtual void register_process_unsubscription_call_back_func(
			ATU_MDI_unsubscription_CallBackFunc* callback) {
		if(m_unsubscription_call_back_func!=NULL) delete m_unsubscription_call_back_func;
		m_unsubscription_call_back_func = callback;
	}

	virtual void register_process_acknowledgement_call_back_func(
			ATU_MDI_acknowledgement_CallBackFunc* callback) {
		if(m_acknowledgement_call_back_func!=NULL) delete m_acknowledgement_call_back_func;
		m_acknowledgement_call_back_func = callback;
	}

	virtual bool process_signalfeed(ATU_OTI_signalfeed_struct &s) {
		if (m_signalfeed_call_back_func!=NULL) {
			return (*m_signalfeed_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool process_quoterequestfeed(ATU_OTI_quoterequestfeed_struct &s) {
		if (m_quoterequestfeed_call_back_func!=NULL) {
			return (*m_quoterequestfeed_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool process_subscription(
			ATU_MDI_subscription_struct& s) {
		if (m_subscription_call_back_func!=NULL) {
			return (*m_subscription_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool process_unsubscription(
			ATU_MDI_unsubscription_struct& s) {
		if (m_unsubscription_call_back_func!=NULL) {
			return (*m_unsubscription_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool process_acknowledgement(
			ATU_MDI_acknowledgement_struct& s) {
		if (m_acknowledgement_call_back_func!=NULL) {
			return (*m_acknowledgement_call_back_func)(s);
		} else {
			return false;
		}
	}

	virtual bool process_marketfeed(ATU_MDI_marketfeed_struct &s){
		/*if (m_marketfeed_call_back_func!=NULL) {
			return (*m_marketfeed_call_back_func)(s);
		} else {
			return false;
		}*/
		bool tradePeriod=false;
		for (int i=0;i<m_tradeBegin.size();i++) {
			int hrmmss=atoi(s.m_timestamp.substr(9,6).c_str());
			if (hrmmss>=m_tradeBegin[i] &&  hrmmss<=m_tradeEnd[i]) {
				tradePeriod=true;
			//	cout << hrmmss << endl;
			}
		}
		if (tradePeriod) {
		for(unsigned int i = 0 ; i < m_marketfeed_call_back_func_list.size() ; i++){
			if (m_marketfeed_call_back_func_list[i]!=NULL) {
				 (*m_marketfeed_call_back_func_list[i])(s);
			} else {
				return false;
			}
		}
		}
	}
	virtual bool process_ohlcfeed(ATU_MDI_ohlcfeed_struct &s){
		/*if (m_marketfeed_call_back_func!=NULL) {
			return (*m_marketfeed_call_back_func)(s);
		} else {
			return false;
		}*/
		bool tradePeriod=false;
		for (int i=0;i<m_tradeBegin.size();i++) {
			int hrmmss=atoi(s.m_timestamp.substr(9,6).c_str());
			if (hrmmss>=m_tradeBegin[i] &&  hrmmss<=m_tradeEnd[i]) {
				tradePeriod=true;
			//	cout << hrmmss << endl;
			}
		}
		if (tradePeriod) {
		for(unsigned int i = 0 ; i < m_ohlcfeed_call_back_func_list.size() ; i++){
			if (m_ohlcfeed_call_back_func_list[i]!=NULL) {
				 (*m_ohlcfeed_call_back_func_list[i])(s);
			} else {
				return false;
			}
		}
		}
	}

	virtual bool process_ping(ATU_OTI_ping_struct &s) {
		if (m_ping_call_back_func!=NULL) {
			return (*m_ping_call_back_func)(s);
		} else {
			return false;
		}
	}
/*
	virtual bool process_reset(ATU_OTI_reset_struct &s) {
		if (m_reset_call_back_func!=NULL) {
			clearAllCallBackFuncList();
			(*m_reset_call_back_func)(s);
			return true;
		} else {
			return false;
		}
	}
*/
	virtual bool process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &s) {
		if (m_portfolio_get_trade_history_call_back_func!=NULL) {
			return (*m_portfolio_get_trade_history_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &s) {
		if (m_portfolio_get_working_orders_call_back_func!=NULL) {
			return (*m_portfolio_get_working_orders_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool process_portfolio_get_PnL(ATU_OTI_portfolio_get_PnL_struct &s) {
		if (m_portfolio_get_PnL_call_back_func!=NULL) {
			return (*m_portfolio_get_PnL_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool process_risk_setting(ATU_OTI_risk_setting_struct &s) {
		if (m_risk_setting_call_back_func!=NULL) {
			return (*m_risk_setting_call_back_func)(s);
		} else {
			return false;
		}
	}

	virtual bool process_portfolio_get_daily_performance(ATU_OTI_portfolio_get_daily_performance_struct &s) {
		if (m_portfolio_get_daily_performance_call_back_func!=NULL) {
			return (*m_portfolio_get_daily_performance_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool process_portfolio_get_accum_performance(ATU_OTI_portfolio_get_accum_performance_struct &s) {
		if (m_portfolio_get_accum_performance_call_back_func!=NULL) {
			return (*m_portfolio_get_accum_performance_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool process_portfolio_get_pnl_performance(ATU_OTI_portfolio_get_pnl_performance_struct &s) {
		if (m_portfolio_get_pnl_performance_call_back_func!=NULL) {
			return (*m_portfolio_get_pnl_performance_call_back_func)(s);
		} else {
			return false;
		}
	}
	virtual bool on_tradefeed(ATU_OTI_tradefeed_struct &s) {
		ATU_OTI_tradefeed_struct *d=new ATU_OTI_tradefeed_struct;
		*d=s;
		boost::unique_lock<boost::recursive_mutex> lock(m_msgqueueMutex);
		m_msgqueue.push(d);
		m_msgType.push(4);
	}
	virtual bool on_riskfeed(ATU_OTI_riskfeed_struct &sin) {

	}
	virtual bool on_riskstatusfeed(ATU_OTI_riskstatusfeed_struct &sin) {

	}
	virtual bool on_accumperffeed(ATU_OTI_accumperffeed_struct &s){

	}
	virtual bool on_errorfeedp(ATU_ErrorMsgStruct *s) {
		on_errorfeed(*s);
	}
	virtual bool on_errorfeed(ATU_ErrorMsgStruct &s) {
	}
	virtual bool on_dailyperffeed(ATU_OTI_dailyperffeed_struct &s){

	}
	virtual void detach() {}
	virtual bool on_marketfeed(ATU_MDI_marketfeed_struct& s) {
		int msgqueuesize=10;
		while (msgqueuesize>0) {
			{
				boost::unique_lock<boost::recursive_mutex> lock(m_msgqueueMutex);
				msgqueuesize=m_msgqueue.size();
				if (msgqueuesize==0) break;
			}
			usleep(1000);
		}
		on_marketfeed_process(s);
		/*
		ATU_MDI_marketfeed_struct *d=new ATU_MDI_marketfeed_struct;
		*d=s;
		boost::unique_lock<boost::recursive_mutex> lock(m_msgqueueMutex);
		m_msgqueue.push(d);
		m_msgType.push(0);
		*/
	}

	virtual bool on_orderfeed(ATU_OTI_orderfeed_struct &s) {
		ATU_OTI_orderfeed_struct *d=new ATU_OTI_orderfeed_struct;
		*d=s;
		boost::unique_lock<boost::recursive_mutex> lock(m_msgqueueMutex);
		m_msgqueue.push(d);
		m_msgType.push(1);
	}
	virtual bool on_pnlperffeed(ATU_OTI_pnlperffeed_struct &s){
		ATU_OTI_pnlperffeed_struct *d=new ATU_OTI_pnlperffeed_struct;
		*d=s;
		boost::unique_lock<boost::recursive_mutex> lock(m_msgqueueMutex);
		m_msgqueue.push(d);
		m_msgType.push(2);
	}
	virtual bool on_portfoliofeed(ATU_OTI_portfoliofeed_struct &s){
		ATU_OTI_portfoliofeed_struct *d=new ATU_OTI_portfoliofeed_struct;
		*d=s;
		boost::unique_lock<boost::recursive_mutex> lock(m_msgqueueMutex);
		m_msgqueue.push(d);
		m_msgType.push(3);
	}

	virtual bool on_marketfeed_process(ATU_MDI_marketfeed_struct& s) {
	}
	virtual bool on_ohlcfeed_process(ATU_MDI_ohlcfeed_struct& s) {
	}
	virtual bool on_ohlcfeed(ATU_MDI_ohlcfeed_struct& s) {
		int msgqueuesize=10;
		while (msgqueuesize>0) {
			{
				boost::unique_lock<boost::recursive_mutex> lock(m_msgqueueMutex);
				msgqueuesize=m_msgqueue.size();
				if (msgqueuesize==0) break;
			}
			usleep(1000);
		}
		on_ohlcfeed_process(s);
		/*
		ATU_MDI_marketfeed_struct *d=new ATU_MDI_marketfeed_struct;
		*d=s;
		boost::unique_lock<boost::recursive_mutex> lock(m_msgqueueMutex);
		m_msgqueue.push(d);
		m_msgType.push(0);
		*/
	}

	virtual bool on_tradefeed_process(ATU_OTI_tradefeed_struct &sin) {

	}
	virtual bool on_riskfeed_process(ATU_OTI_riskfeed_struct &sin) {

	}
	virtual bool on_riskstatusfeed_process(ATU_OTI_riskstatusfeed_struct &sin) {

	}
	virtual bool on_orderfeed_process(ATU_OTI_orderfeed_struct &sin) {
	}
	virtual bool on_portfoliofeed_process(ATU_OTI_portfoliofeed_struct &s){

	}
	virtual bool on_accumperffeed_process(ATU_OTI_accumperffeed_struct &s){

	}
	virtual bool on_pnlperffeed_process(ATU_OTI_pnlperffeed_struct &pnlfeed){

	}
	virtual bool on_errorfeedp_process(ATU_ErrorMsgStruct *s) {
	}
	virtual bool on_errorfeed_process(ATU_ErrorMsgStruct &s) {
	}
	virtual bool on_dailyperffeed_process(ATU_OTI_dailyperffeed_struct &s){

	}

	virtual bool process_msgqueue() {
		while (true) {
			ATU_MDI_marketfeed_struct *mf;
			ATU_OTI_orderfeed_struct *of;
			ATU_OTI_tradefeed_struct *tf;

			ATU_OTI_pnlperffeed_struct *pnlf;
			ATU_OTI_portfoliofeed_struct *pf;
			int msgtype=-1;
			{
			boost::unique_lock<boost::recursive_mutex> lock(m_msgqueueMutex);
			if (m_msgqueue.size()>0) {
				msgtype=m_msgType.front();
				if (msgtype==0) {
					mf=(ATU_MDI_marketfeed_struct*)m_msgqueue.front();
				}
				if (msgtype==1) {
					of=(ATU_OTI_orderfeed_struct *)m_msgqueue.front();
				}
				if (msgtype==2) {
					pnlf=(ATU_OTI_pnlperffeed_struct*)m_msgqueue.front();
				}
				if (msgtype==3) {
					pf=(ATU_OTI_portfoliofeed_struct*)m_msgqueue.front();
				}
				if (msgtype==4) {
					tf=(ATU_OTI_tradefeed_struct*)m_msgqueue.front();
				}

				m_msgqueue.pop();
				m_msgType.pop();
			}
			}
			if (msgtype>=0) {
				if (msgtype==0) {
					on_marketfeed_process(*mf);
					ATU_MDI_acknowledgement_struct sss;
					sss.m_status="0";
					process_acknowledgement(sss);

					delete mf;
				}
				if (msgtype==1) {
					on_orderfeed_process(*of);
					delete of;
				}
				if (msgtype==2) {
					on_pnlperffeed_process(*pnlf);
					delete pnlf;
				}
				if (msgtype==3) {
					on_portfoliofeed_process(*pf);
					delete pf;
				}
				if (msgtype==4) {
					on_tradefeed_process(*tf);
					delete tf;
				}

			} else {
				usleep(1000);
			}
		}
	}

	ATU_OTI_portfolio_get_trade_history_CallBackFunc *m_portfolio_get_trade_history_call_back_func;
	ATU_OTI_portfolio_get_working_orders_CallBackFunc *m_portfolio_get_working_orders_call_back_func;
	ATU_OTI_signalfeed_CallBackFunc *m_signalfeed_call_back_func;
	vector<ATU_MDI_marketfeed_CallBackFunc *> m_marketfeed_call_back_func_list;
	vector<ATU_MDI_ohlcfeed_CallBackFunc *> m_ohlcfeed_call_back_func_list;
	ATU_String_CallBackFunc *m_send_msg_call_back_func;
	ATU_String_CallBackFunc *m_broadcast_msg_call_back_func;
	ATU_OTI_portfolio_get_PnL_CallBackFunc * m_portfolio_get_PnL_call_back_func;
	ATU_OTI_portfolio_get_daily_performance_CallBackFunc * m_portfolio_get_daily_performance_call_back_func;
	ATU_OTI_portfolio_get_accum_performance_CallBackFunc * m_portfolio_get_accum_performance_call_back_func;
	ATU_OTI_portfolio_get_pnl_performance_CallBackFunc * m_portfolio_get_pnl_performance_call_back_func;
	ATU_MDI_subscription_CallBackFunc *m_subscription_call_back_func;
	ATU_MDI_unsubscription_CallBackFunc *m_unsubscription_call_back_func;
	ATU_MDI_acknowledgement_CallBackFunc *m_acknowledgement_call_back_func;

	ATU_OTI_risk_setting_CallBackFunc * m_risk_setting_call_back_func;
	ATU_OTI_quoterequestfeed_CallBackFunc * m_quoterequestfeed_call_back_func;
	ATU_OTI_ping_CallBackFunc *m_ping_call_back_func;
};
#endif /* ATU_ABSTRACT_ALGOI_H_ */
