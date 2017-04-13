#ifndef ATU_ABSTRACT_MDI_H_
#define ATU_ABSTRACT_MDI_H_

#include "PCH.h"
#include "Constants.h"
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

#endif /* ATU_ABSTRACT_MDI_H_ */
