#ifndef _ATU_ABSTRACT_OTI_H_
#define _ATU_ABSTRACT_OTI_H_
#include "ATU_Abstract_MDI.h"
using namespace std;

typedef struct ATU_OTI_signalfeed_struct
{
  string m_timestamp;
  string m_market;
  string m_instrument;
  string m_order_id;
  double m_price;
  double m_qty;
  string m_open_or_close;
  int    m_buy_or_sell;
  string m_order_action;
  string m_order_type;
  string m_order_validity;
  string m_portfolio_name;
  string m_client_user;
  string m_order_attributes;
  ATU_OTI_signalfeed_struct():
    m_timestamp(""),
    m_market(""),
    m_instrument(""),
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
  static string ToString(const ATU_OTI_signalfeed_struct & sf)
  {
    std::ostringstream buffer; 
    buffer <<
      sf.m_timestamp        <<  ",signalfeed," <<
      sf.m_market           <<  ","            <<
      sf.m_instrument       <<  ","            <<
      sf.m_order_id         <<  ","            <<
      sf.m_price            <<  ","            <<   setprecision(5) << fixed <<
      sf.m_qty              <<  ","            <<
      sf.m_open_or_close    <<  ","            <<
      sf.m_buy_or_sell      <<  ","            <<
      sf.m_order_action     <<  ","            <<
      sf.m_order_type       <<  ","            <<
      sf.m_order_validity   <<  "\n"           <<   setprecision(0) << fixed;
    return buffer.str();
  }
  static bool ParseString(const string & csv, ATU_OTI_signalfeed_struct & sf)
  {
    vector<string> vSF;
    boost::split(vSF, csv, boost::is_any_of(","));
    if (vSF[1] != "signalfeed") return false;

    sf.m_timestamp                              =                              vSF[ 0]  ;
    sf.m_market                                 =                              vSF[ 2]  ;
    sf.m_instrument                             =                              vSF[ 3]  ;
    sf.m_order_id                               =                              vSF[ 4]  ;
    sf.m_price                                  =  boost::lexical_cast<double>(vSF[ 5]) ;
    sf.m_qty                                    =  boost::lexical_cast<double>(vSF[ 6]) ;
    sf.m_open_or_close                          =                              vSF[ 7]  ;
    sf.m_buy_or_sell                            =  boost::lexical_cast<int>   (vSF[ 8]) ;
    sf.m_order_action                           =                              vSF[ 9]  ;
    sf.m_order_type                             =                              vSF[10]  ;
    sf.m_order_validity                         =                              vSF[11]  ;

    if (vSF.size() > 12) sf.m_portfolio_name    =                              vSF[12]  ;
    if (vSF.size() > 13) sf.m_client_user       =                              vSF[13]  ;
    if (vSF.size() > 14) sf.m_order_attributes  =                              vSF[14]  ;

    return true;
  }

} ATU_OTI_signalfeed_struct;

typedef struct ATU_OTI_orderfeed_struct
{
  string m_timestamp;
  string m_market;
  string m_instrument;
  string m_order_id;
  double m_price;
  double m_qty;
  string m_open_or_close;
  int    m_buy_or_sell;
  double m_qty_filled;
  string m_order_type;
  string m_order_validity;
  int    m_deleted;
  int    m_order_status;
  string m_error_description;
  string m_portfolio_name;
  string m_created_timestamp;
  string m_changed_timestamp;
  int    m_source;
  int    m_islast;
  ATU_OTI_orderfeed_struct():
    m_timestamp(""),
    m_market(""),
    m_instrument(""),
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
  ATU_OTI_orderfeed_struct(const ATU_OTI_signalfeed_struct & sf):
    m_timestamp(sf.m_timestamp),
    m_market(sf.m_market),
    m_instrument(sf.m_instrument),
    m_order_id(sf.m_order_id),
    m_price(sf.m_price),
    m_qty(sf.m_qty),
    m_open_or_close(sf.m_open_or_close),
    m_buy_or_sell(sf.m_buy_or_sell),
    m_qty_filled(0),
    m_order_type(sf.m_order_type),
    m_order_validity(sf.m_order_validity),
    m_deleted(1),
    m_order_status(0),
    m_error_description(""),
    m_portfolio_name(""),
    m_created_timestamp(""),
    m_changed_timestamp(""),
    m_source(0),
    m_islast(0){}
  static string ToString(const ATU_OTI_orderfeed_struct & of)
  {
    std::ostringstream buffer; 
    buffer <<
      of.m_timestamp          <<  ",orderfeed,"    <<
      of.m_market             <<  ","              <<
      of.m_instrument         <<  ","              <<
      of.m_order_id           <<  ","              <<
      of.m_price              <<  ","              <<   setprecision(5) << fixed <<
      of.m_qty                <<  ","              <<
      of.m_open_or_close      <<  ","              <<
      of.m_buy_or_sell        <<  ","              <<
      of.m_qty_filled         <<  ","              <<
      of.m_deleted            <<  ","              <<
      of.m_order_status       <<  ","              <<
      of.m_error_description  <<  ","              <<
      of.m_source             <<  ","              <<
      of.m_order_type         <<  ","              <<
      of.m_order_validity     <<  ","              <<
      of.m_portfolio_name     <<  ","              <<
      of.m_created_timestamp  <<  ","              <<
      of.m_changed_timestamp  <<  ","              <<
      of.m_islast             <<  "\n"             <<   setprecision(0) << fixed;
    return buffer.str();
  }
} ATU_OTI_orderfeed_struct;

typedef struct ATU_OTI_tradefeed_struct
{
  string m_timestamp;
  string m_market;
  string m_instrument;
  string m_order_id;
  double m_price;
  double m_qty;
  string m_open_or_close;
  int    m_buy_or_sell;
  string m_trade_id;
  string m_portfolio_name;
  string m_trade_timestamp;
  int    m_source;
  int    m_islast;
  ATU_OTI_tradefeed_struct():
    m_timestamp(""),
    m_market(""),
    m_instrument(""),
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
  static string ToString(const ATU_OTI_tradefeed_struct & tf)
  {
    std::ostringstream buffer; 
    buffer <<
      tf.m_timestamp        <<  ",tradefeed,"  <<
      tf.m_market           <<  ","            <<
      tf.m_instrument       <<  ","            <<
      tf.m_order_id         <<  ","            <<
      tf.m_price            <<  ","            <<   setprecision(5) << fixed <<
      tf.m_qty              <<  ","            <<
      tf.m_buy_or_sell      <<  ","            <<
      tf.m_trade_id         <<  ","            <<
      tf.m_source           <<  "\n"           <<   setprecision(0) << fixed;
    return buffer.str();
  }
} ATU_OTI_tradefeed_struct;

typedef struct ATU_OTI_portfolio_get_trade_history_struct
{
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

typedef struct ATU_OTI_portfolio_get_working_orders_struct
{
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

#endif //_ATU_ABSTRACT_OTI_H_
