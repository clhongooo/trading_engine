////**************************************************
//  Author:      Sunny Yan
//  Created On:  Wed Apr 2 13:41:14 HKT 2014
//  Description:
//
//**************************************************

#ifndef OMDMESSAGEHEADER_H_
#define OMDMESSAGEHEADER_H_

#ifndef NDEBUG
#define NDEBUG

#include "BinaryTools.h"
// #include "jsoncpp/json/json.h"
#include <cstring>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;
//----------------------------------------------------------------------------------------------------


//OMD COMMON MESSAGE TYPE
#define      OMD_SEQUENCE_RESET                    100
#define      OMD_LOGON                             101
#define      OMD_LOGON_RESPONSE                    102
#define      OMD_RETRANSMISSION_REQUEST            201
#define      OMD_RETRANSMISSION_RESPONSE           202
#define      OMD_REFRESH_COMPLETE                  203
#define      OMD_MAXIMUM_MESSAGE_TYPE              1000

//OMD-C MESSAGE TYPE
#define      OMDC_MARKET_DEFINITION                10
#define      OMDC_SECURITY_DEFINITION              11
#define      OMDC_LIQUIDITY_PROVIDER               13
#define      OMDC_CURRENCY_RATE                    14
#define      OMDC_TRADING_SESSION_STATUS           20
#define      OMDC_SECURITY_STATUS                  21
#define      OMDC_NEWS                             22
#define      OMDC_ADD_ORDER                        30
#define      OMDC_MODIFY_ORDER                     31
#define      OMDC_DELETE_ORDER                     32
#define      OMDC_ADD_ODD_LOT_ORDER                33
#define      OMDC_DELETE_ODD_LOT_ORDER             34
#define      OMDC_NOMINAL_PRICE                    40
#define      OMDC_INDICATIVE_EQUILIBRIUM_PRICE     41
#define      OMDC_YIELD                            44
#define      OMDC_TRADE                            50
#define      OMDC_TRADE_CANCEL                     51
#define      OMDC_TRADE_TICKER                     52
#define      OMDC_AGGREGATE_ORDER_BOOK_UPDATE      53
#define      OMDC_BROKER_QUEUE                     54
#define      OMDC_STATISTICS                       60
#define      OMDC_MARKET_TURNOVER                  61
#define      OMDC_CLOSING_PRICE                    62
#define      OMDC_INDEX_DEFINITION                 70
#define      OMDC_INDEX_DATA                       71


//OMD-D MESSAGE TYPE

#define      OMDD_COMMODITY_DEFINITION                 301
#define      OMDD_CLASS_DEFINITION                     302
#define      OMDD_SERIES_DEFINITION_BASE               303
#define      OMDD_SERIES_DEFINITION_EXTENDED           304
#define      OMDD_COMBINATION_DEFINITION               305
#define      OMDD_MARKET_STATUS                        320
#define      OMDD_SERIES_STATUS                        321
#define      OMDD_COMMODITY_STATUS                     322
#define      OMDD_ADD_ORDER                            330
#define      OMDD_MODIFY_ORDER                         331
#define      OMDD_DELETE_ORDER                         332
#define      OMDD_AGGREGATE_ORDER_BOOK_UPDATE          353
#define      OMDD_ORDER_BOOK_CLEAR                     335
#define      OMDD_QUOTE_REQUEST                        336
#define      OMDD_TRADE                                350
#define      OMDD_TRADE_AMENDMENT                      356
#define      OMDD_TRADE_STATISTICS                     360
#define      OMDD_SERIES_STATISTICS                    363
#define      OMDD_CALCULATED_OPENING_PRICE             364
#define      OMDD_ESTIMATED_AVERAGE_SETTLEMENT_PRICE   365
#define      OMDD_MARKET_ALERT                         323
#define      OMDD_OPEN_INTEREST                        366
#define      OMDD_IMPLIED_VOLATILITY                   367



#pragma pack(1)


typedef struct {
  uint16_t  PktSize;
  uint8_t   MsgCount;
  char      Filler;
  uint32_t  SeqNum;
  uint64_t  SendTime;
} OMD_Packet_Header;

//----------------------------------------------------------------------------------------------------
//3.4 Control Message
//3.41

//3.42
typedef struct {
  Uint16 MsgSize;
  Uint16 MsgType;
  Uint32 NewSeqNo;
} OMD_Sequence_Reset;

//----------------------------------------------------------------------------------------------------
//3.5 Retransmission

typedef struct {
  Uint16  PktSize;
  Uint8   MsgCount;
  char    Filler;
  Uint32  SeqNum;
  Uint64  SendTime;
} OMD_Retransmission_Header;

//3.5.1
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  char    Username[12];
} OMD_Logon;


//3.5.2
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint8   SessionStatus;
  char    Filler[3];
} OMD_Logon_Response;


//3.5.3
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint16  ChannelID;
  char    Filler[2];
  Uint32  BeginSeqNum;
  Uint32  EndSeqNum;
} OMD_Retransmission_Request;

//3.5.4
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint16  ChannelID;
  Uint8   RetransStatus;
  char    Filler;
  Uint32  BeginSeqNum;
  Uint32  EndSeqNum;
} OMD_Retransmission_Response;

//----------------------------------------------------------------------------------------------------
// 3.6  Refresh
//3.6.1
typedef struct {
  Uint16 MsgSize;
  Uint16 MsgType;
  Uint32 LastSeqNum;
} OMD_Refresh_Complete;

//----------------------------------------------------------------------------------------------------
//3.7 Reference Data
//3.7.1
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  char    MarketCode[4];
  char    MarketName[25];
  char    CurrencyCode[3];
  Uint32  NumberOfSecurities;
} OMDC_Market_Definition;

//3.7.2
typedef struct {
  Uint32 UnderlyingSecurityCode;
  Uint32 UnderlyingSecurityWeight;
} OMDC_UnderlyingSecurity;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  char    MarketCode[4];
  char    ISINCode[12];
  char    InstrumentType[4];
  char    SpreadTableCode[2];
  char    SecurityShortName[40];
  char    CurrencyCode[3];
  BYTE    SecurityNameGCCS[60];
  BYTE    SecurityNameGB[60];
  Uint32  LotSize;
  Int32   PreviousClosingPrice;
  char    Filler;
  char    ShortSellFlag;
  char    Filler_;
  char    CCASSFlag;
  char    DummySecurityFlag;
  char    TestSecurityFlag;
  char    StampDutyFlag;
  char    Filler__;
  Uint32  ListingDate;
  Uint32  DelistingDate;
  char    FreeText[38];
  char    EFNFlag;
  Uint32  AccruedInterest;
  Uint32  CouponRate;
  Uint32  ConversionRatio;
  Int32   StrikePrice;
  Uint32  MaturityDate;
  char    CallPutFlag;
  char    Style;
  Uint16  NoUnderlyingSecurities;
} OMDC_Security_Definition;

//3.7.3
typedef struct {
  Uint16 MsgSize;
  Uint16 MsgType;
  Uint32 SecurityCode;
  Uint16 NoLiquidityProviders;
} OMDC_Liquidity_Provider;

//3.7.4
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  char    CurrencyCode[3];
  char    Filler;
  Uint16  CurrencyFactor;
  char    Filler_[2];
  Uint32  CurrencyRate;
} OMDC_Currency_Rate;

//----------------------------------------------------------------------------------------------------
//3.8 Status Data
//3.8.1
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  char    MarketCode[4];
  Uint8   TradingSessionID;
  Uint8   TradingSessionSubID;
  Uint8   TradingSesStatus;
  char    TradingSesControlFlag;
  char    Filler[4];
  Uint64  StartDateTime;
  Uint64  EndDateTime;
} OMDC_Trading_Session_Status;

//3.8.2
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Uint8   SecurityTradingStatus;
  char    Filler[3];
} OMDC_Security_Status;

//----------------------------------------------------------------------------------------------------
//3.9 Order Book Data
//3.9.1
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Uint64  OrderID;
  Int32   Price;
  Uint32  Quantity;
  Uint16  Side;
  char    OrderType;
  char    Filler;
  Int32   OrderBookPosition;
} OMDC_Add_Order;

//3.9.2
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Uint64  OrderID;
  Uint32  Quantity;
  Uint16  Side;
  char    Filler[2];
  Int32   OrderBookPosition;
} OMDC_Modify_Order;


//3.9.3
typedef struct {
  Uint16 MsgSize;
  Uint16 MsgType;
  Uint32 SecurityCode;
  Uint64 OrderID;
  Uint16 Side;
  char Filler[2];
} OMDC_Delete_Order;

//3.9.4
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Uint64  OrderID;
  Int32   Price;
  Uint32  Quantity;
  Uint16  BrokerID;
  Uint16  Side;
} OMDC_Add_Odd_Lot_Order;

//3.9.5
typedef struct {
  Uint16 MsgSize;
  Uint16 MsgType;
  Uint32 SecurityCode;
  Uint64 OrderID;
  Uint16 BrokerID;
  Uint16 Side;
} OMDC_Delete_Odd_Lot_Order;

//3.9.6
typedef struct {
  Uint64  AggregateQuantity;
  Int32   Price;
  Uint32  NumberOfOrders;
  Uint16  Side;
  Uint8   PriceLevel;
  Uint8   UpdateAction;
  char    Filler[4];
} OMDC_Aggregate_Order_Book_Update_Details;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  char    Filler[3];
  Uint8   NoEntries;
} OMDC_Aggregate_Order_Book_Update;

//3.9.7
typedef struct {
  Uint16  Item;
  char    Type;
  char    Filler;
} OMDC_Broker_Queue_Item;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Uint8   ItemCount;
  Uint16  Side;
  char    BQMoreFlag;
} OMDC_Broker_Queue;

//----------------------------------------------------------------------------------------------------
//3.10 Trade and Price Data
//3.10.1
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Uint32  TradeID;
  Int32   Price;
  Uint32  Quantity;
  Int16   TrdType;
  char    Filler[2];
  Uint64  TradeTime;
} OMDC_Trade;

//3.10.2
typedef struct {
  Uint16 MsgSize;
  Uint16 MsgType;
  Uint32 SecurityCode;
  Uint32 TradeID;
} OMDC_Trade_Cancel;

//3.10.3
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Uint32  TickerID;
  Int32   Price;
  Uint64  AggregateQuantity;
  Uint64  TradeTime;
  Int16   TrdType;
  char    TrdCancelFlag;
  char    Filler;
} OMDC_Trade_Ticker;

//3.10.4
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Int32   ClosingPrice;
  Uint32  NumberOfTrades;
} OMDC_Closing_Price;

//3.10.5
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Int32   NominalPrice;
} OMDC_Nominal_Price;

//3.10.6
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Int32   Price;
  Uint64  AggregateQuantity;
} OMDC_Indicative_Equilibrium_Price;

//----------------------------------------------------------------------------------------------------
//3.11 Value Added Data
//3.11.1
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Uint64  SharesTraded;
  Int64   Turnover;
  Int32   HighPrice;
  Int32   LowPrice;
  Int32   LastPrice;
  Int32   VWAP;
  Uint32  ShortSellSharesTraded;
  Int64   ShortSellTurnover;
} OMDC_Statistics;

//3.11.2
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  char    MarketCode[4];
  char    CurrencyCode[3];
  char    Filler;
  Int64   Turnover;
} OMDC_Market_Turnover;

//3.11.3
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  SecurityCode;
  Int32   Yield;
} OMDC_Yield;

//----------------------------------------------------------------------------------------------------
//3.12 News
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  char    NewsType[3];
  char    NewsID[3];
  BYTE    Headline[320];
  char    CancelFlag;
  char    LastFragment;
  char    Filler[4];
  Uint64  ReleaseTime;
  char    Filler_[2];
  Uint16  NoMarketCodes;
} OMDC_News;

//----------------------------------------------------------------------------------------------------
//3.13 Index DAta
//3.13.1
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  char    IndexCode[11];
  char    IndexSource;
  char    CurrencyCode[3];
  char    Filler;
} OMDC_Index_Definition;

//3.13.2
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  char    IndexCode[11];
  char    IndexStatus;
  Int64   IndexTime;
  Int64   IndexValue;
  Int64   NetChgPrevDay;
  Int64   HighValue;
  Int64   LowValue;
  Int64   EASValue;
  Int64   IndexTurnover;
  Int64   OpeningValue;
  Int64   ClosingValue;
  Int64   PreviousSesClose;
  Int64   IndexVolume;
  Int32   NetChgPrevDayPct;
  char    Exception;
  char    Filler[3];
} OMDC_Index_Data;

//----------------------------------------------------------------------------------------------------
//OMD-D
typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint16  CommodityCode;
  Uint16  DecimallnUnderlyingPrice;
  char    ISINCode[12];
  char    BaseCurrency[3];
  Uint8   UnderlyingPriceUnit;
  char    CommodityName[32];
  Int64   NominalValue;
  char    UnderlyingCode[20];
  Uint8   UnderlyingType;
  Uint8   EffectiveTomorrow;
  char    Filler[2];
} OMDD_Commodity_Definition;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint8   Country;
  Uint8   Market;
  Uint8   InstrumentGroup;
  Uint8   Modifier;
  Uint16  CommodityCode;
  char    Filler1[2];
  Int32   PriceQuotationFactor;
  Uint32  ContractSize;
  Uint16  DecimallnStrikePrice;
  Uint16  DecimallnContractSize;
  Uint16  DecimallnPremium;
  Uint16  RankingType;
  Uint8   Tradable;
  Uint8   PremiumUnit4Price;
  char    BaseCurrency[3];
  char    InstrumentClassID[14];
  char    InstrumentClassName[32];
  char    IsFractions;
  char    SettlementCurrencyID[32];
  Uint8   EffectiveTomorrow;
  char    Filler2;
} OMDD_Class_Definition;



typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  char    Symbol[32];
  Uint8   FinancialProduct;
  Uint16  NumberOfDecimalsPrice;
  Uint8   NumberOfLegs;
  Int32   StrikePrice;
  char    ExpirationDate[8];
  Uint16  Filler;
  Uint8   PutOrCall;
  char    Filler2;
} OMDD_Series_Definition_Base;



typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  char    Symbol[32];
  Uint8   Country;
  Uint8   Market;
  Uint8   InstrumentGroup;
  Uint8   Modifier;
  Uint16  CommodityCode;
  Uint16  ExpirationDate;
  Int32   StrikePrice;
  Int64   ContractSize;
  char    ISINCode[12];
  Uint8   SeriesStatus;
  Uint8   EffectiveTomorrow;
  char    Filler[6];
  char    EffectiveExpDate[8];
  Int64   DateTimeLastTrading;
} OMDD_Series_Definition_Extended;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  ComboOrderbookID;
  Uint32  LegOrderbookID;
  char    Filler[3];
  char    LegSide;
  Int32   LegRatio;
} OMDD_Combination_Definition;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint16  StateLevel;
  Uint8   Market;
  Uint8   Instrument;
  Uint32  OrderbookID;
  Uint16  CommodityCode;
  char    Filler1[2];
  char    ActualStartDate[8];
  char    ActualStartTime[6];
  char    PlannedStartDate[8];
  char    PlannedStartTime[6];
  Uint16  SecondsToStateChange;
  Uint16  State;
  Uint8   Priority;
  char    Filler2[3];
} OMDD_Market_Status;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  char    Suspended;
  char    Filler[3];
} OMDD_Series_Status;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint16  CommodityCode;
  char    Suspended;
  char    Filler;
} OMDD_Commodity_Status;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  Uint64  OrderID;
  Int32   Price;
  Uint32  Quantity;
  Uint8   Side;
  Uint8   LotType;
  Uint16  OrderType;
  Uint32  OrderBookPosition;
} OMDD_Add_Order;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  Uint64  OrderID;
  Int32   Price;
  Uint32  Quantity;
  Uint8   Side;
  char    Filler;
  Uint16  OrderType;
  Uint32  OrderBookPosition;
} OMDD_Modify_Order;

typedef struct {
  Uint16 MsgSize;
  Uint16 MsgType;
  Uint32 OrderbookID;
  Uint64 OrderID;
  Uint8  Side;
  char Filler;
} OMDD_Delete_Order;

//3.9.6
typedef struct {
  Uint64  AggregateQuantity;
  Int32   Price;
  Uint32  NumberOfOrders;
  Uint8   Side;
  char    Filler;
  Uint8   PriceLevel;
  Uint8   UpdateAction;
  char    Filler2[4];
} OMDD_Aggregate_Order_Book_Update_Details;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  char    Filler[3];
  Uint8   NoEntries;
} OMDD_Aggregate_Order_Book_Update;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
} OMDD_Order_Book_Clear;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  Int32   NumberOfLots;
  Uint8   BidAskFlag;
  char    Filler[3];
} OMDD_Quote_Request;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  Uint64  OrderID;
  Int32   Price;
  Uint64  TradeID;
  Uint32  ComboGroupID;
  Uint8   Side;
  Uint8   DealType;
  Uint16  TradeCondition;
  Uint16  DealInfo;
  char    Filler[2];
  Uint64  Quantity;
  Uint64  TradeTime;
} OMDD_Trade;

typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint64  TradeID;
  Uint32  ComboGroupID;
  Int32   Price;
  Uint64  Quantity;
  Uint64  TradeTime;
  Uint8   TradeState;
  char    Filler[3];
} OMDD_Trade_Amendment;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  Int32   Price;
  Uint8   DealSource;
  Uint8   Session;
  char    Filler1[2];
  Int64   AggregrateQuantity;
  Int32   Open;
  Int32   High;
  Int32   Low;
  char    Filler2[4];
  Uint64  TradeReportVolume;
  Uint32  DealCount;
  Uint64  Turnover;
} OMDD_Trade_Statistics;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  Uint8   Session;
  char    Filler[3];
  Int32   Open;
  Int32   High;
  Int32   Low;
  Uint64  TradeReportVolume;
  Uint32  DealCount;
  Int32   Price;
  Uint64  Turnover;
} OMDD_Series_Statistics;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  Int32   CalculatedOpeningPrice;
  char    Filler[4];
  Uint64  Quantity;
} OMDD_Calculated_Opening_Price;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  char    EASType;
  char    InstrumentCode[20];
  Int64   EAS;
  char    Filler[3];
} OMDD_Estimated_Average_Settlement_Price;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint16  AlertID;
  char    Source;
  char    Filler;
  BYTE    Header[320];
  char    LastFragment;
  Uint8   InfoType;
  Uint8   Priority;
  Uint8   NoLines;
} OMDD_Market_Alert;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint16  DayIndicator;
  char    Filler[6];
  Uint32  OrderbookID;
  Uint32  Settlement;
  Uint32  DealCount;
  Uint32  GrossOI;
  Uint32  NetOI;
  Uint64  Turnover;
} OMDD_Open_Interest;


typedef struct {
  Uint16  MsgSize;
  Uint16  MsgType;
  Uint32  OrderbookID;
  Uint32  ImpliedVolatility;
} OMDD_Implied_Volatility;


//----------------------------------------------------------------------------------------------------

#pragma pack(8)

// //--------------------------------------------------
// // OMD
// //--------------------------------------------------
// JSONNode genNode    (OMD_Sequence_Reset &s                        , string name, char *buffer, int bufferLength);
// string   printStruct(OMD_Sequence_Reset &s                        , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMD_Retransmission_Header &s                 , string name, char *buffer, int bufferLength);
// string   printStruct(OMD_Retransmission_Header &s                 , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMD_Logon &s                                 , string name, char *buffer, int bufferLength);
// string   printStruct(OMD_Logon &s                                 , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMD_Logon_Response &s                        , string name, char *buffer, int bufferLength);
// string   printStruct(OMD_Logon_Response &s                        , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMD_Retransmission_Request &s                , string name, char *buffer, int bufferLength);
// string   printStruct(OMD_Retransmission_Request &s                , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMD_Retransmission_Response &s               , string name, char *buffer, int bufferLength);
// string   printStruct(OMD_Retransmission_Response &s               , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMD_Refresh_Complete &s                      , string name, char *buffer, int bufferLength);
// string   printStruct(OMD_Refresh_Complete &s                      , string varName, char *buffer, int bufferLength);

// //--------------------------------------------------
// // OMD-C
// //--------------------------------------------------
// JSONNode genNode    (OMDC_Market_Definition &s                    , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Market_Definition &s                    , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_UnderlyingSecurity &s                   , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_UnderlyingSecurity &s                   , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Security_Definition &s                  , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Security_Definition &s                  , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Liquidity_Provider &s                   , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Liquidity_Provider &s                   , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Currency_Rate &s                        , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Currency_Rate &s                        , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Trading_Session_Status &s               , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Trading_Session_Status &s               , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Security_Status &s                      , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Security_Status &s                      , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Add_Order &s                            , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Add_Order &s                            , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Modify_Order &s                         , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Modify_Order &s                         , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Delete_Order &s                         , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Delete_Order &s                         , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Add_Odd_Lot_Order &s                    , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Add_Odd_Lot_Order &s                    , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Delete_Odd_Lot_Order &s                 , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Delete_Odd_Lot_Order &s                 , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Aggregate_Order_Book_Update_Details &s  , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Aggregate_Order_Book_Update_Details &s  , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Aggregate_Order_Book_Update &s          , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Aggregate_Order_Book_Update &s          , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Broker_Queue_Item &s                    , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Broker_Queue_Item &s                    , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Broker_Queue &s                         , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Broker_Queue &s                         , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Trade &s                                , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Trade &s                                , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Trade_Cancel &s                         , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Trade_Cancel &s                         , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Trade_Ticker &s                         , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Trade_Ticker &s                         , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Closing_Price &s                        , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Closing_Price &s                        , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Nominal_Price &s                        , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Nominal_Price &s                        , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Indicative_Equilibrium_Price &s         , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Indicative_Equilibrium_Price &s         , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Statistics &s                           , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Statistics &s                           , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Market_Turnover &s                      , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Market_Turnover &s                      , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Yield &s                                , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Yield &s                                , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_News &s                                 , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_News &s                                 , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Index_Definition &s                     , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Index_Definition &s                     , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDC_Index_Data &s                           , string name, char *buffer, int bufferLength);
// string   printStruct(OMDC_Index_Data &s                           , string varName, char *buffer, int bufferLength);

// //--------------------------------------------------
// // OMD-D
// //--------------------------------------------------
// JSONNode genNode    (OMDD_Commodity_Definition &s                 , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Commodity_Definition &s                 , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Class_Definition &s                     , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Class_Definition &s                     , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Series_Definition_Base &s               , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Series_Definition_Base &s               , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Series_Definition_Extended &s           , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Series_Definition_Extended &s           , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Combination_Definition &s               , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Combination_Definition &s               , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Market_Status &s                        , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Market_Status &s                        , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Series_Status &s                        , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Series_Status &s                        , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Commodity_Status &s                     , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Commodity_Status &s                     , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Add_Order &s                            , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Add_Order &s                            , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Modify_Order &s                         , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Modify_Order &s                         , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Delete_Order &s                         , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Delete_Order &s                         , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Aggregate_Order_Book_Update_Details &s  , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Aggregate_Order_Book_Update_Details &s  , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Aggregate_Order_Book_Update &s          , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Aggregate_Order_Book_Update &s          , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Order_Book_Clear &s                     , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Order_Book_Clear &s                     , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Quote_Request &s                        , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Quote_Request &s                        , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Trade &s                                , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Trade &s                                , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Trade_Amendment &s                      , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Trade_Amendment &s                      , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Trade_Statistics &s                     , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Trade_Statistics &s                     , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Series_Statistics &s                    , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Series_Statistics &s                    , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Calculated_Opening_Price &s             , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Calculated_Opening_Price &s             , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Estimated_Average_Settlement_Price &s   , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Estimated_Average_Settlement_Price &s   , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Market_Alert &s                         , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Market_Alert &s                         , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Open_Interest &s                        , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Open_Interest &s                        , string varName, char *buffer, int bufferLength);
// JSONNode genNode    (OMDD_Implied_Volatility &s                   , string name, char *buffer, int bufferLength);
// string   printStruct(OMDD_Implied_Volatility &s                   , string varName, char *buffer, int bufferLength);



#endif //NDEBUG
#endif //OMDMESSAGEHEADER_H_
