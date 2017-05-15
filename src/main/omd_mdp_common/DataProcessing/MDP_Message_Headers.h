//**************************************************
//  Author:      Sunny Yan
//  Created On:  Sun Apr 23 22:23:29 HKT 2017
//**************************************************

#ifndef MDPMESSAGEHEADER_H_
#define MDPMESSAGEHEADER_H_

#include "sbe/sbe.h"
#include "MessageHeader.h"
#include "MDIncrementalRefreshBook32.h"
#include "MDIncrementalRefreshDailyStatistics33.h"
#include "MDIncrementalRefreshLimitsBanding34.h"
#include "MDIncrementalRefreshSessionStatistics35.h"
#include "MDIncrementalRefreshTrade36.h"
#include "MDIncrementalRefreshVolume37.h"
#include "MDInstrumentDefinitionOption41.h"
#include "MDIncrementalRefreshTradeSummary42.h"
#include "MDInstrumentDefinitionFuture27.h"
#include "MDInstrumentDefinitionSpread29.h"
#include <QuoteRequest39.h>
#include <SnapshotFullRefresh38.h>
#include <SecurityStatus30.h>
#include <ChannelReset4.h>

#include <cstring>

using namespace std;
using namespace mktdata;

//----------------------------------------------------------------------------------------------------
#define  MDP_VERSION 6

//----------------------------------------------------------------------------------------------------
#define  MDP_CHANNEL_RESET                        4
#define  MDP_HEARTBEAT                           12
#define  MDP_REFRESH_SECURITY_DEFINITION_FUTURE  27
#define  MDP_REFRESH_SECURITY_DEFINITION_SPREAD  29
#define  MDP_SECURITY_STATUS                     30
#define  MDP_REFRESH_BOOK                        32
#define  MDP_REFRESH_DAILY_STATISTICS            33
#define  MDP_REFRESH_LIMITS_BANDING              34
#define  MDP_REFRESH_SESSION_STATISTICS          35
#define  MDP_REFRESH_TRADE                       36
#define  MDP_REFRESH_VOLUME                      37
#define  MDP_SNAPSHOT_FULL_REFRESH               38
#define  MDP_QUOTE_REQUEST                       39
#define  MDP_INSTRUMENT_DEFINITION_OPTION        41
#define  MDP_REFRESH_TRADE_SUMMARY               42
#define  MDP_CYCLE_COMPLETE                      4294967000 // magic num

#pragma pack(1)
typedef struct {
  uint32_t PktSeqNum;
  uint64_t SendingTime;
} MDP_Packet_Header;

#pragma pack(8)

#endif
