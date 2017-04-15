#ifndef _ATU_ABSTRACT_OTI_H_
#define _ATU_ABSTRACT_OTI_H_
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "ATU_ErrorMsgStruct.h"
#include "ATU_Abstract_MDI.h"
#include "ATU_Logger.h"
using namespace std;

typedef struct ATU_OTI_signalfeed_struct{
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

#endif //_ATU_ABSTRACT_OTI_H_
