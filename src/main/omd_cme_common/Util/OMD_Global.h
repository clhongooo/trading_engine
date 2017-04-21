//**************************************************
//  Author:      Sunny Yan
//  Created On:  Tue Apr 1 18:19:56 HKT 2014
//  Description: Global definition:
//               - Data types
//               - HKEx OMD-C and OMD-D  constants
//
//**************************************************

#ifndef OMD_GLOBAL_H_
#define OMD_GLOBAL_H_

#include <boost/cstdint.hpp>

typedef      uint8_t                               BYTE;
#define      Uint8                                 uint8_t
#define      Uint16                                uint16_t
#define      Uint32                                uint32_t
#define      Uint64                                uint64_t
#define      Int8                                  int8_t
#define      Int16                                 int16_t
#define      Int32                                 int32_t
#define      Int64                                 int64_t


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


//Cash Custom Messages
#define      OMDC_AGGREGATE_ORDER_BOOK_SNAPSHOT    501
#define      OMDC_TRADE_TICKER_LIST                502
#define      OMDC_BROKER_QUEUE_BID                 503
#define      OMDC_BROKER_QUEUE_ASK                 504

#define      MAX_OMD_PACKET_SIZE                   1500
#define      MAX_MSG_RTS_SUPT                      10000
#define      MAX_MESSAGE_SIZE                      MAX_OMD_PACKET_SIZE

#define      JSON_BUFFER_SIZE                      65536

#define      READ_UINT8(BYTE_PTR)                  (*((uint8_t*)(BYTE_PTR)))
#define      READ_UINT16(BYTE_PTR)                 (*((uint16_t*)(BYTE_PTR)))
#define      READ_UINT32(BYTE_PTR)                 (*((uint32_t*)(BYTE_PTR)))
#define      READ_UINT64(BYTE_PTR)                 (*((uint64_t*)(BYTE_PTR)))

#define      READ_INT8(BYTE_PTR)                   (*((int8_t*)(BYTE_PTR)))
#define      READ_INT16(BYTE_PTR)                  (*((int16_t*)(BYTE_PTR)))
#define      READ_INT32(BYTE_PTR)                  (*((int32_t*)(BYTE_PTR)))
#define      READ_INT64(BYTE_PTR)                  (*((int64_t*)(BYTE_PTR)))

#define      WRITE_UINT8(BYTE_PTR, val)            ((*((uint8_t*)(BYTE_PTR)))=(val))
#define      WRITE_UINT16(BYTE_PTR,val)            ((*((uint16_t*)(BYTE_PTR)))=(val))
#define      WRITE_UINT32(BYTE_PTR,val)            ((*((uint32_t*)(BYTE_PTR)))=(val))
#define      WRITE_UINT64(BYTE_PTR,val)            ((*((uint64_t*)(BYTE_PTR)))=(val))

#define      WRITE_INT8(BYTE_PTR, val)             ((*((int8_t*)(BYTE_PTR)))=(val))
#define      WRITE_INT16(BYTE_PTR,val)             ((*((int16_t*)(BYTE_PTR)))=(val))
#define      WRITE_INT32(BYTE_PTR,val)             ((*((int32_t*)(BYTE_PTR)))=(val))
#define      WRITE_INT64(BYTE_PTR,val)             ((*((int64_t*)(BYTE_PTR)))=(val))



#endif
