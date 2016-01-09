
/*
 * ATU_Abstract_MDI.cpp
 *
 *  Created on: May 26, 2014
 *      Author: alex
 */
#include "ATU_Abstract_MDI.h"

namespace atu{
	ATU_Abstract_MDI::ATU_Abstract_MDI() : m_logfeed_call_back_func(NULL) {
		sessionDestroy = true;
	}
	ATU_Abstract_MDI::~ATU_Abstract_MDI() {
		if(m_marketfeed_call_back_func!=NULL) delete m_marketfeed_call_back_func;
		if(m_ohlcfeed_call_back_func!=NULL) delete m_ohlcfeed_call_back_func;
		if(m_logfeed_call_back_func!=NULL) delete m_logfeed_call_back_func;
	}

	bool ATU_Abstract_MDI::notify_marketfeed(ATU_MDI_marketfeed_struct& s) {
		return true;
	}
	bool ATU_Abstract_MDI::notify_ohlcfeed(ATU_MDI_ohlcfeed_struct& s) {
			return true;
	}
	bool ATU_Abstract_MDI::notify_logfeed(ATU_logfeed_struct* s) {
		if (m_logfeed_call_back_func!=NULL) {
			(*m_logfeed_call_back_func)(s);
		}
		return true;
	}

	void ATU_Abstract_MDI::register_notify_logfeed_call_back_func(
			ATU_logfeed_CallBackFunc* callback) {
		m_logfeed_call_back_func = callback;
	}

	void ATU_Abstract_MDI::register_notify_marketfeed_call_back_func(
			ATU_MDI_marketfeed_CallBackFunc* callback) {
		m_marketfeed_call_back_func = callback;
	}
	void ATU_Abstract_MDI::register_notify_ohlcfeed_call_back_func(
				ATU_MDI_ohlcfeed_CallBackFunc* callback) {
			m_ohlcfeed_call_back_func = callback;
	}

	bool ATU_Abstract_MDI::on_process_subscription(ATU_MDI_subscription_struct &s){
		return true;
	}
	bool ATU_Abstract_MDI::on_process_unsubscription(ATU_MDI_unsubscription_struct &s){
		return true;
	}
	bool ATU_Abstract_MDI::on_process_acknowledgement(ATU_MDI_acknowledgement_struct &s){
		return true;
	}
	bool ATU_Abstract_MDI::process_unsubscription(ATU_MDI_unsubscription_struct &s){
		return true;
	}
	bool ATU_Abstract_MDI::process_subscription(ATU_MDI_subscription_struct &s){
			return true;
	}
}


