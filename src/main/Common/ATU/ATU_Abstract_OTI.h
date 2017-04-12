#ifndef _ATU_ABSTRACT_OTI_H_
#define _ATU_ABSTRACT_OTI_H_
#include "PCH.h"
#include "Constants.h"
//#include "ParsedMarketData.h"
//#include "ATU_TCP_OTI_string_handler.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "ATU_ErrorMsgStruct.h"
#include "ATU_Abstract_MDI.h"
#include "ATU_Logger.h"
#include "ParsedMarketData.h"
//#include "ATU_AbstractOrderAlgo.h"
using namespace std;

typedef struct ATU_OTI_signalfeed_struct{
//	Timestamp of messages	Market(string)	Product Code(string)	Order ID(string)	Price (double)	Qty (double)
//			Open or Close (string)	Buy or Sell (int)	Order Action (string)	Order Type (string)	Order Validity (string)	Portfolio Name (string)
	string m_timestamp;
	string m_market;
	string m_feedcode;
	string m_order_id;
	double m_price;
	double m_qty;
	string m_open_or_close;
	int m_buy_or_sell;
	string m_order_action;
	string m_order_type;
	string m_order_validity;
	string m_portfolio_name;
	string m_client_user;
	string m_order_attributes;
	ATU_OTI_signalfeed_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_order_id(""),
		m_price(0),
		m_qty(0),
		m_open_or_close(""),
		m_buy_or_sell(0),
		m_order_action(""),
		m_order_type(""),
		m_order_validity(""),
		m_portfolio_name(""),
		m_client_user(""),
		m_order_attributes(""){}
} ATU_OTI_signalfeed_struct;

typedef struct ATU_OTI_orderfeed_struct{
//	Timestamp of messages	Market(string)	Product Code(string)	Order ID(string)	Price (double)	Qty (double)
//	Open or Close (string)	Buy or Sell (int)	Qty Filled (double)	Order Type(string)	Order Validity(string)
//Deleted (int)	Order Status(int)	Error Description(string)	Source (int)
	string m_timestamp;
	string m_market;
	string m_feedcode;
	string m_order_id;
	double m_price;
	double m_qty;
	string m_open_or_close;
	int m_buy_or_sell;
	double m_qty_filled;
	string m_order_type;
	string m_order_validity;
	int m_deleted;
	int m_order_status;
	string m_error_description;
	string m_portfolio_name;
	string m_created_timestamp;
	string m_changed_timestamp;
	int m_source;
	int m_islast;
	ATU_OTI_orderfeed_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_order_id(""),
		m_price(0),
		m_qty(0),
		m_open_or_close(""),
		m_buy_or_sell(0),
		m_qty_filled(0),
		m_order_type(""),
		m_order_validity(""),
		m_deleted(0),
		m_order_status(0),
		m_error_description(""),
		m_portfolio_name(""),
		m_created_timestamp(""),
		m_changed_timestamp(""),
		m_source(0),
		m_islast(0){}
} ATU_OTI_orderfeed_struct;

typedef struct ATU_OTI_riskfeed_struct{
	string m_timestamp;
	string m_market;
	string m_feedcode;
	string m_order_id;
	string m_msg_description;
	ATU_OTI_riskfeed_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_order_id(""),
		m_msg_description(""){}
} ATU_OTI_riskfeed_struct;

typedef struct ATU_OTI_riskstatusfeed_struct{
	string m_timestamp;
	string m_identity;
	string m_feedcode;
	string m_key;
	string m_value;
	ATU_OTI_riskstatusfeed_struct():
		m_timestamp(""),
		m_identity(""),
		m_feedcode(""),
		m_key(""),
		m_value(""){}
} ATU_OTI_riskstatusfeed_struct;

typedef struct ATU_OTI_ping_struct{
	string m_timestamp;
	ATU_OTI_ping_struct():
		m_timestamp(""){}
} ATU_OTI_ping_struct;

typedef struct ATU_OTI_tradefeed_struct{
//	Message Type	Direction	Purpose	Timestamp of messages	Market(string)	Product Code(string)	Order ID(string)
	//Price (double)	Qty (double)	Open or Close (string)	Buy or Sell (int)	Trade ID (string) Source (int)
	string m_timestamp;
	string m_market;
	string m_feedcode;
	string m_order_id;
	double m_price;
	double m_qty;
	string m_open_or_close;
	int m_buy_or_sell;
	string m_trade_id;
	string m_portfolio_name;
	string m_trade_timestamp;
	int m_source;
	int m_islast;
	ATU_OTI_tradefeed_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_order_id(""),
		m_price(0),
		m_qty(0),
		m_open_or_close(""),
		m_buy_or_sell(0),
		m_trade_id(""),
		m_portfolio_name(""),
		m_trade_timestamp(""),
		m_source(0),
		m_islast(0){}
} ATU_OTI_tradefeed_struct;

typedef struct ATU_OTI_portfoliofeed_struct{
	string m_timestamp;
	string m_market;
	string m_feedcode;
	double m_net_position;
	double m_average_open_price;
	double m_net_invested;
	double m_realized_pnl;
	double m_unrealized_pnl;
	double m_total_pnl;
	int m_islast;
	ATU_OTI_portfoliofeed_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_net_position(0),
		m_average_open_price(0),
		m_net_invested(0),
		m_realized_pnl(0),
		m_unrealized_pnl(0),
		m_total_pnl(0),
		m_islast(0){}
} ATU_OTI_portfoliofeed_struct;

typedef struct ATU_OTI_portfolio_get_trade_history_struct{
//	Message Type	Direction	Purpose	Timestamp of messages	Market(string)	Product Code(string)	Order ID(string)
	//Price (double)	Qty (double)	Open or Close (string)	Buy or Sell (int)	Trade ID (string) Source (int)
	string m_timestamp;
	string m_portfolio_name;
	string m_period;
	string m_client_user;
	ATU_OTI_portfolio_get_trade_history_struct():
		m_timestamp(""),
		m_portfolio_name(""),
		m_period(""),
		m_client_user(""){}
} ATU_OTI_portfolio_get_trade_history_struct;

typedef struct ATU_OTI_portfolio_get_working_orders_struct{
//	Message Type	Direction	Purpose	Timestamp of messages	Market(string)	Product Code(string)	Order ID(string)
	//Price (double)	Qty (double)	Open or Close (string)	Buy or Sell (int)	Trade ID (string) Source (int)
	string m_timestamp;
	string m_portfolio_name;
	string m_period;
	string m_client_user;
	ATU_OTI_portfolio_get_working_orders_struct():
		m_timestamp(""),
		m_portfolio_name(""),
		m_period(""),
		m_client_user(""){}
} ATU_OTI_portfolio_get_working_orders_struct;

typedef struct ATU_OTI_portfolio_get_PnL_struct{
//	Message Type	Direction	Purpose	Timestamp of messages	Market(string)	Product Code(string)	Order ID(string)
	//Price (double)	Qty (double)	Open or Close (string)	Buy or Sell (int)	Trade ID (string) Source (int)
	string m_timestamp;
	string m_portfolio_name;
	string m_period;
	string m_client_user;
	ATU_OTI_portfolio_get_PnL_struct():
		m_timestamp(""),
		m_portfolio_name(""),
		m_period(""),
		m_client_user(""){}
} ATU_OTI_portfolio_get_PnL_struct;

typedef struct ATU_OTI_risk_setting_struct{
//	Message Type	Direction	Purpose	Timestamp of messages	Market(string)	Product Code(string)	Order ID(string)
	//Price (double)	Qty (double)	Open or Close (string)	Buy or Sell (int)	Trade ID (string) Source (int)
	string m_timestamp;
	string m_function;
	ATU_OTI_risk_setting_struct():
		m_timestamp(""),
		m_function(""){}
} ATU_OTI_risk_setting_struct;

typedef struct ATU_OTI_insert_order_struct{
	string m_timestamp;
	string m_market;
	string m_feedcode;
	string m_order_id;
	double m_price;
	double m_qty;
	string m_open_or_close;
	int m_buy_or_sell;
	string m_order_action;
	string m_order_type;
	string m_order_validity;
	ATU_OTI_insert_order_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_order_id(""),
		m_price(0),
		m_qty(0),
		m_open_or_close(""),
		m_buy_or_sell(0),
		m_order_action(""),
		m_order_type(""),
		m_order_validity(""){}
} ATU_OTI_insert_order_struct;

typedef struct ATU_OTI_delete_order_struct{
	string m_timestamp;
	string m_market;
	string m_feedcode;
	string m_order_id;
	ATU_OTI_delete_order_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_order_id(""){}
} ATU_OTI_delete_order_struct;

typedef struct ATU_OTI_quoterequestfeed_struct{
	string m_timestamp;
	string m_market;
	string m_feedcode;
	int m_buysellboth;
	double m_volume;
	ATU_OTI_quoterequestfeed_struct():
		m_timestamp(""),
		m_market(""),
		m_feedcode(""),
		m_buysellboth(0),
		m_volume(0){}
} ATU_OTI_quoterequestfeed_struct;

typedef struct ATU_OTI_reset_struct{
	string m_timestamp;
	ATU_OTI_reset_struct():
		m_timestamp(""){}
} ATU_OTI_reset_struct;

typedef boost::function < void(string str) > ATU_String_CallBackFunc;
typedef boost::function < bool(ATU_OTI_signalfeed_struct &s) > ATU_OTI_signalfeed_CallBackFunc;
typedef boost::function < bool(ATU_OTI_portfolio_get_trade_history_struct &s) > ATU_OTI_portfolio_get_trade_history_CallBackFunc;
typedef boost::function < bool(ATU_OTI_portfolio_get_working_orders_struct &s) > ATU_OTI_portfolio_get_working_orders_CallBackFunc;
typedef boost::function < bool(ATU_OTI_ping_struct &s) > ATU_OTI_ping_CallBackFunc;
typedef boost::function < bool(ATU_OTI_portfolio_get_PnL_struct &s) > ATU_OTI_portfolio_get_PnL_CallBackFunc;
typedef boost::function < bool(ATU_OTI_tradefeed_struct &s) > ATU_OTI_tradefeed_CallBackFunc;
typedef boost::function < bool(ATU_OTI_orderfeed_struct &s) > ATU_OTI_orderfeed_CallBackFunc;
typedef boost::function < bool(ATU_OTI_riskfeed_struct &s) > ATU_OTI_riskfeed_CallBackFunc;
typedef boost::function < bool(ATU_OTI_riskstatusfeed_struct &s) > ATU_OTI_riskstatusfeed_CallBackFunc;
typedef boost::function < bool(ATU_OTI_portfoliofeed_struct &s) > ATU_OTI_portfoliofeed_CallBackFunc;
typedef boost::function < bool(ATU_OTI_quoterequestfeed_struct &s) > ATU_OTI_quoterequestfeed_CallBackFunc;
typedef boost::function < bool(ATU_OTI_risk_setting_struct &s) > ATU_OTI_risk_setting_CallBackFunc;
typedef boost::function < void(ParsedMarketData *) > ATU_OTI_Tick_CallBackFunc;
typedef boost::function < bool(ATU_OTI_reset_struct &s) > ATU_OTI_reset_CallBackFunc;
typedef boost::function < double(const string &) > ATU_OTI_get_current_position_from_portfolio_manager_CallBackFunc;
typedef boost::function < void(vector<ATU_OTI_portfoliofeed_struct> &) > ATU_OTI_get_portfoliofeeds_from_portfolio_manager_CallBackFunc;
typedef boost::function < void(const string &,ATU_OTI_portfoliofeed_struct &) > ATU_OTI_get_portfoliofeed_struct_from_portfolio_manager_CallBackFunc;
typedef boost::function < double(const string &) > ATU_OTI_get_total_pnl_from_portfolio_manager_CallBackFunc;
typedef boost::function < double() > ATU_OTI_get_allproducts_total_pnl_from_portfolio_manager_CallBackFunc;
typedef boost::function < void(vector<string> &) > ATU_OTI_get_allfeedcodes_from_portfolio_manager_CallBackFunc;

namespace atu
{

class ATU_Abstract_OTI
{

public:
	ATU_Abstract_OTI();
	virtual ~ATU_Abstract_OTI();
//would then call all the notify_orderfeed for all the order in the list
	virtual bool on_notify_orderfeed(ATU_OTI_orderfeed_struct &s);
//would then call all the notify_tradefeed for all the order in the list
	virtual bool on_notify_tradefeed(ATU_OTI_tradefeed_struct &s);
//process incoming signal feed and dispatch for all the order in the list if necessary
	virtual bool on_process_signalfeed(ATU_OTI_signalfeed_struct &s);
//process incoming quote request feed
	virtual bool on_process_quoterequestfeed(ATU_OTI_quoterequestfeed_struct &s);
//notify orderfeed to call back function
	virtual bool notify_orderfeed(ATU_OTI_orderfeed_struct &s);
//notify tradefeed to call back function
	virtual bool notify_tradefeed(ATU_OTI_tradefeed_struct &s);
//notify risk msg
	virtual bool notify_riskfeed(ATU_OTI_riskfeed_struct &s);
//notify risk status msg
	virtual bool notify_riskstatusfeed(ATU_OTI_riskstatusfeed_struct &s);
//notify error msg
	virtual bool notify_errorfeed(ATU_ErrorMsgStruct *ems);

	virtual bool on_notify_riskfeed(ATU_OTI_riskfeed_struct &s);
	virtual bool on_notify_riskstatusfeed(ATU_OTI_riskstatusfeed_struct &s);
//processing of incoming errorfeed
	virtual bool on_notify_errorfeed(ATU_ErrorMsgStruct *ems);
	virtual bool notify_portfoliofeed(ATU_OTI_portfoliofeed_struct &s);
	virtual bool on_notify_portfoliofeed(ATU_OTI_portfoliofeed_struct &s);
//register send order callback func
	virtual void register_signalfeed_call_back_func(ATU_OTI_signalfeed_CallBackFunc *callback);
	virtual void register_convert_signalfeed_call_back_func(ATU_OTI_signalfeed_CallBackFunc *callback);

	virtual void register_portfolio_get_working_orders_call_back_func(ATU_OTI_portfolio_get_working_orders_CallBackFunc *callback);
	virtual void register_portfolio_get_trade_history_call_back_func(ATU_OTI_portfolio_get_trade_history_CallBackFunc *callback);
	virtual void register_portfolio_get_PnL_call_back_func(ATU_OTI_portfolio_get_PnL_CallBackFunc *callback);

	virtual bool register_notify_orderfeed_call_back_func(ATU_OTI_orderfeed_CallBackFunc *callback);
	virtual bool register_notify_tradefeed_call_back_func(ATU_OTI_tradefeed_CallBackFunc *callback);
	virtual bool register_notify_errorfeed_call_back_func(ATU_errorfeed_CallBackFunc *callback);
	virtual bool register_notify_riskfeed_call_back_func(ATU_OTI_riskfeed_CallBackFunc *callback);
	virtual bool register_notify_riskstatusfeed_call_back_func(ATU_OTI_riskstatusfeed_CallBackFunc *callback);
	virtual bool register_notify_portfoliofeed_call_back_func(ATU_OTI_portfoliofeed_CallBackFunc *callback);

	virtual bool process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &s);
	virtual bool on_process_portfolio_get_working_orders(ATU_OTI_portfolio_get_working_orders_struct &s);
	virtual bool process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &s);
	virtual bool on_process_portfolio_get_trade_history(ATU_OTI_portfolio_get_trade_history_struct &s);
	virtual bool on_process_portfolio_get_PnL(ATU_OTI_portfolio_get_PnL_struct &s);
	virtual bool process_portfolio_get_PnL(ATU_OTI_portfolio_get_PnL_struct &s);
	virtual bool on_process_marketfeed(ATU_MDI_marketfeed_struct &s);
	virtual bool on_process_ohlcfeed(ATU_MDI_ohlcfeed_struct &s);
	virtual void register_notify_logfeed_call_back_func(ATU_logfeed_CallBackFunc *callback);
	virtual bool register_process_tick_call_back_func(ATU_OTI_Tick_CallBackFunc *);
	virtual bool notify_logfeed(ATU_logfeed_struct *s);

	virtual void setConnectString(string str) {}
	virtual void detach() {}
protected:
	ATU_logfeed_CallBackFunc *m_logfeed_call_back_func;

	//send order callback func
	ATU_OTI_signalfeed_CallBackFunc *m_signalfeed_call_back_func;
	ATU_OTI_signalfeed_CallBackFunc *m_convert_signalfeed_call_back_func;

	vector<ATU_OTI_tradefeed_CallBackFunc * >m_notify_tradefeed_call_back_func_list;
	vector<ATU_OTI_orderfeed_CallBackFunc * >m_notify_orderfeed_call_back_func_list;
	vector<ATU_OTI_riskfeed_CallBackFunc * >m_notify_riskfeed_call_back_func_list;
	vector<ATU_OTI_riskstatusfeed_CallBackFunc * >m_notify_riskstatusfeed_call_back_func_list;

	vector<ATU_errorfeed_CallBackFunc * > m_notify_errorfeed_call_back_func_list;
	vector<ATU_OTI_portfoliofeed_CallBackFunc * > m_notify_portfoliofeed_call_back_func_list;
	ATU_OTI_portfolio_get_working_orders_CallBackFunc *m_portfolio_get_working_orders_call_back_func;
	ATU_OTI_portfolio_get_trade_history_CallBackFunc *m_portfolio_get_trade_history_call_back_func;
	ATU_OTI_portfolio_get_PnL_CallBackFunc * m_portfolio_get_PnL_call_back_func;
	ATU_OTI_Tick_CallBackFunc * m_process_tick_call_back_func;
};
}
#endif //_ATU_ABSTRACT_OTI_H_
