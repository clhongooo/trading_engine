#ifndef ATU_ABSTRACT_MDI_H_
#define ATU_ABSTRACT_MDI_H_

#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "SDateTime.h"

using namespace std;
using namespace boost;

typedef struct ATU_MDI_subscription_struct {
  string m_timestamp;
  string m_market;
  string m_instrument;
  string m_begindate;
  string m_enddate;
  string m_subscription_attribute;
  ATU_MDI_subscription_struct():
    m_timestamp(""),
    m_market(""),
    m_instrument(""),
    m_begindate(""),
    m_enddate(""),
    m_subscription_attribute(""){}
} ATU_MDI_subscription_struct;

typedef struct ATU_MDI_unsubscription_struct {
  string m_timestamp;
  string m_market;
  string m_instrument;
  ATU_MDI_unsubscription_struct():
    m_timestamp(""),
    m_market(""),
    m_instrument(""){}
} ATU_MDI_unsubscription_struct;

typedef struct ATU_MDI_acknowledgement_struct {
  string m_timestamp;
  string m_status;
  string m_errormsg;
  ATU_MDI_acknowledgement_struct():
    m_timestamp(""),
    m_status(""),
    m_errormsg(""){}
} ATU_MDI_acknowledgement_struct;

typedef struct ATU_MDI_marketfeed_struct {
  uint64_t m_microsec_since_epoch_gmt;
  char     m_instrument[50];
  double   m_traded_price;
  long     m_traded_volume;
  double   m_bid_price_1;
  long     m_bid_volume_1;
  double   m_bid_price_2;
  long     m_bid_volume_2;
  double   m_bid_price_3;
  long     m_bid_volume_3;
  double   m_bid_price_4;
  long     m_bid_volume_4;
  double   m_bid_price_5;
  long     m_bid_volume_5;
  double   m_ask_price_1;
  long     m_ask_volume_1;
  double   m_ask_price_2;
  long     m_ask_volume_2;
  double   m_ask_price_3;
  long     m_ask_volume_3;
  double   m_ask_price_4;
  long     m_ask_volume_4;
  double   m_ask_price_5;
  long     m_ask_volume_5;
  ATU_MDI_marketfeed_struct():
    m_microsec_since_epoch_gmt(0),
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
    m_ask_volume_5(0){
      strcpy(m_instrument,"");
    }
  void SetMicrosecSinceEpoch(uint64_t u)
  {
    m_microsec_since_epoch_gmt = u;
  }
  void SetSymbol(const string & s)
  {
    strcpy(m_instrument,s.c_str());
  }
  static string ToString(const ATU_MDI_marketfeed_struct & mfs)
  {
    std::ostringstream buffer; 
    buffer << SDateTime::fromUnixTimeToString(mfs.m_microsec_since_epoch_gmt, SDateTime::MICROSEC, SDateTime::GMT, SDateTime::HKT) << ",";
    buffer << mfs.m_instrument    << ",";
    buffer << mfs.m_traded_price  << ",";
    buffer << mfs.m_traded_volume << ",B,";
    buffer << mfs.m_bid_price_1   << ",";
    buffer << mfs.m_bid_volume_1  << ",";
    buffer << mfs.m_bid_price_2   << ",";
    buffer << mfs.m_bid_volume_2  << ",";
    buffer << mfs.m_bid_price_3   << ",";
    buffer << mfs.m_bid_volume_3  << ",";
    buffer << mfs.m_bid_price_4   << ",";
    buffer << mfs.m_bid_volume_4  << ",";
    buffer << mfs.m_bid_price_5   << ",";
    buffer << mfs.m_bid_volume_5  << ",A,";
    buffer << mfs.m_ask_price_1   << ",";
    buffer << mfs.m_ask_volume_1  << ",";
    buffer << mfs.m_ask_price_2   << ",";
    buffer << mfs.m_ask_volume_2  << ",";
    buffer << mfs.m_ask_price_3   << ",";
    buffer << mfs.m_ask_volume_3  << ",";
    buffer << mfs.m_ask_price_4   << ",";
    buffer << mfs.m_ask_volume_4  << ",";
    buffer << mfs.m_ask_price_5   << ",";
    buffer << mfs.m_ask_volume_5        ;
    return buffer.str();
  }
  static bool ParseMDIString(const string & csv, ATU_MDI_marketfeed_struct & mfs)
  {
    vector<string> vMDI;
    boost::split(vMDI, csv, boost::is_any_of(","));
    if (vMDI.size() != 26) return false;

    mfs.m_microsec_since_epoch_gmt = SDateTime::fromStringToUnixTime(vMDI[0],SDateTime::MICROSEC);
    strcpy(mfs.m_instrument,vMDI[1].c_str());

    mfs.m_traded_price  = boost::lexical_cast<double>    (vMDI[ 2]);
    mfs.m_traded_volume = boost::lexical_cast<long  >    (vMDI[ 3]);
    mfs.m_bid_price_1   = boost::lexical_cast<double>    (vMDI[ 5]);
    mfs.m_bid_volume_1  = boost::lexical_cast<long  >    (vMDI[ 6]);
    mfs.m_bid_price_2   = boost::lexical_cast<double>    (vMDI[ 7]);
    mfs.m_bid_volume_2  = boost::lexical_cast<long  >    (vMDI[ 8]);
    mfs.m_bid_price_3   = boost::lexical_cast<double>    (vMDI[ 9]);
    mfs.m_bid_volume_3  = boost::lexical_cast<long  >    (vMDI[10]);
    mfs.m_bid_price_4   = boost::lexical_cast<double>    (vMDI[11]);
    mfs.m_bid_volume_4  = boost::lexical_cast<long  >    (vMDI[12]);
    mfs.m_bid_price_5   = boost::lexical_cast<double>    (vMDI[13]);
    mfs.m_bid_volume_5  = boost::lexical_cast<long  >    (vMDI[14]);
    mfs.m_ask_price_1   = boost::lexical_cast<double>    (vMDI[16]);
    mfs.m_ask_volume_1  = boost::lexical_cast<long  >    (vMDI[17]);
    mfs.m_ask_price_2   = boost::lexical_cast<double>    (vMDI[18]);
    mfs.m_ask_volume_2  = boost::lexical_cast<long  >    (vMDI[19]);
    mfs.m_ask_price_3   = boost::lexical_cast<double>    (vMDI[20]);
    mfs.m_ask_volume_3  = boost::lexical_cast<long  >    (vMDI[21]);
    mfs.m_ask_price_4   = boost::lexical_cast<double>    (vMDI[22]);
    mfs.m_ask_volume_4  = boost::lexical_cast<long  >    (vMDI[23]);
    mfs.m_ask_price_5   = boost::lexical_cast<double>    (vMDI[24]);
    mfs.m_ask_volume_5  = boost::lexical_cast<long  >    (vMDI[25]);

    return true;
  }
} ATU_MDI_marketfeed_struct;

typedef struct ATU_MDI_ping_struct {
  string m_timestamp;
  ATU_MDI_ping_struct():
    m_timestamp(""){}
} ATU_MDI_ping_struct;

typedef struct ATU_MDI_reset_struct {
  string m_timestamp;
  ATU_MDI_reset_struct():
    m_timestamp(""){}
} ATU_MDI_reset_struct;

typedef struct ATU_MDI_ohlcfeed_struct {
  string m_timestamp;
  string m_market;
  string m_instrument;
  double m_open;
  double m_high;
  double m_low;
  double m_close;
  double m_traded_volume;
  ATU_MDI_ohlcfeed_struct():
    m_timestamp(""),
    m_market(""),
    m_instrument(""),
    m_open(0),
    m_high(0),
    m_low(0),
    m_close(0),
    m_traded_volume(0){}
} ATU_MDI_ohlcfeed_struct;

#endif /* ATU_ABSTRACT_MDI_H_ */
