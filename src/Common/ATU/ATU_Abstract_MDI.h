/*
 * ATU_Abstract_MDI.h
 *
 *  Created on: May 26, 2014
 *      Author: alex
 */

#ifndef ATU_ABSTRACT_MDI_H_
#define ATU_ABSTRACT_MDI_H_

#include "PCH.h"
#include "Constants.h"
//#include "ParsedMarketData.h"
//#include "ATU_TCP_MDI_string_handler.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "ATU_ErrorMsgStruct.h"
#include "ATU_Logger.h"

using namespace std;
typedef struct ATU_MDI_subscription_struct{
	string m_timestamp;
	string m_market;
	string m_feedcode;
	string m_begindate;
	string m_enddate;
	string m_subscription_attribute;
	ATU_MDI_subscription_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_begindate(""),
		m_enddate(""),
		m_subscription_attribute(""){}
}ATU_MDI_subscription_struct;
typedef struct ATU_MDI_unsubscription_struct{
	string m_timestamp;
	string m_market;
	string m_feedcode;
	ATU_MDI_unsubscription_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""){}
}ATU_MDI_unsubscription_struct;
typedef struct ATU_MDI_acknowledgement_struct{
	string m_timestamp;
	string m_status;
	string m_errormsg;
	ATU_MDI_acknowledgement_struct():
		m_timestamp(""),
		m_status(""),
		m_errormsg(""){}
}ATU_MDI_acknowledgement_struct;
typedef struct ATU_MDI_marketfeed_struct{
	string m_timestamp;
	string m_feedcode;
	double m_traded_price;
	double m_traded_volume;
	double m_bid_price_1;
	double m_bid_volume_1;
	double m_bid_price_2;
	double m_bid_volume_2;
	double m_bid_price_3;
	double m_bid_volume_3;
	double m_bid_price_4;
	double m_bid_volume_4;
	double m_bid_price_5;
	double m_bid_volume_5;
	double m_ask_price_1;
	double m_ask_volume_1;
	double m_ask_price_2;
	double m_ask_volume_2;
	double m_ask_price_3;
	double m_ask_volume_3;
	double m_ask_price_4;
	double m_ask_volume_4;
	double m_ask_price_5;
	double m_ask_volume_5;
	ATU_MDI_marketfeed_struct():
		m_timestamp(""),
		m_feedcode(""),
		m_traded_price(0),
		m_traded_volume(0),
		m_bid_price_1(0),
		m_bid_volume_1(0),
		m_bid_price_2(0),
		m_bid_volume_2(0),
		m_bid_price_3(0),
		m_bid_volume_3(0),
		m_bid_price_4(0),
		m_bid_volume_4(0),
		m_bid_price_5(0),
		m_bid_volume_5(0),
		m_ask_price_1(0),
		m_ask_volume_1(0),
		m_ask_price_2(0),
		m_ask_volume_2(0),
		m_ask_price_3(0),
		m_ask_volume_3(0),
		m_ask_price_4(0),
		m_ask_volume_4(0),
		m_ask_price_5(0),
		m_ask_volume_5(0){}
} ATU_MDI_marketfeed_struct;

typedef struct ATU_MDI_ping_struct{
	string m_timestamp;
	ATU_MDI_ping_struct():
		m_timestamp(""){}
} ATU_MDI_ping_struct;

typedef struct ATU_MDI_reset_struct{
	string m_timestamp;
	ATU_MDI_reset_struct():
		m_timestamp(""){}
} ATU_MDI_reset_struct;

typedef struct ATU_MDI_ohlcfeed_struct{
	string m_timestamp;
	string m_market;
	string m_feedcode;
	double m_open;
	double m_high;
	double m_low;
	double m_close;
	double m_traded_volume;
	ATU_MDI_ohlcfeed_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_open(0),
		m_high(0),
		m_low(0),
		m_close(0),
		m_traded_volume(0){}
} ATU_MDI_ohlcfeed_struct;


//typedef struct {
//	string m_logMessage;
//	int m_logSeverity;
//} ATU_OTI_MDI_logfeed_struct;

typedef boost::function < void(string str) > ATU_String_CallBackFunc;
typedef boost::function < bool(ATU_MDI_subscription_struct &s) > ATU_MDI_subscription_CallBackFunc;
typedef boost::function < bool(ATU_MDI_unsubscription_struct &s) > ATU_MDI_unsubscription_CallBackFunc;
typedef boost::function < bool(ATU_MDI_marketfeed_struct &s) > ATU_MDI_marketfeed_CallBackFunc;
typedef boost::function < bool(ATU_MDI_ohlcfeed_struct &s) > ATU_MDI_ohlcfeed_CallBackFunc;
typedef boost::function < bool(ATU_MDI_ping_struct &s) > ATU_MDI_ping_CallBackFunc;
typedef boost::function < bool(ATU_MDI_reset_struct &s) > ATU_MDI_reset_CallBackFunc;
typedef boost::function < bool(ATU_MDI_acknowledgement_struct &s) > ATU_MDI_acknowledgement_CallBackFunc;
//typedef boost::function < void(ATU_OTI_MDI_logfeed_struct &s) > ATU_OTI_MDI_logfeed_CallBackFunc;

namespace atu {
	class ATU_Abstract_MDI {
	public:
		ATU_Abstract_MDI();
		virtual ~ATU_Abstract_MDI();

		virtual bool on_process_subscription(ATU_MDI_subscription_struct &s);
		virtual bool on_process_unsubscription(ATU_MDI_unsubscription_struct &s);
		virtual bool on_process_acknowledgement(ATU_MDI_acknowledgement_struct &s);
		virtual bool process_subscription(ATU_MDI_subscription_struct &s);
		virtual bool process_unsubscription(ATU_MDI_unsubscription_struct &s);
		virtual bool notify_marketfeed(ATU_MDI_marketfeed_struct &s);
		virtual bool notify_ohlcfeed(ATU_MDI_ohlcfeed_struct &s);
		virtual void register_notify_marketfeed_call_back_func(ATU_MDI_marketfeed_CallBackFunc *callback);
		virtual void register_notify_ohlcfeed_call_back_func(ATU_MDI_ohlcfeed_CallBackFunc *callback);


		virtual void register_notify_logfeed_call_back_func(ATU_logfeed_CallBackFunc *callback);
		virtual bool notify_logfeed(ATU_logfeed_struct *s);
		virtual void setConnectString(string address) {}
		virtual void detach() {}
		bool sessionDestroy;
	protected:
		ATU_MDI_marketfeed_CallBackFunc *m_marketfeed_call_back_func;
		ATU_MDI_ohlcfeed_CallBackFunc *m_ohlcfeed_call_back_func;
		ATU_logfeed_CallBackFunc *m_logfeed_call_back_func;

	};

}


#endif /* ATU_ABSTRACT_MDI_H_ */
