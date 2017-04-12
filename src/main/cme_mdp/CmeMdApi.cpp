#include <string>
#include <iostream>

#include <time.h>
#include <sys/time.h>

#include <MDIncrementalRefreshBook32.h>
#include <MDIncrementalRefreshDailyStatistics33.h>
#include <MDIncrementalRefreshLimitsBanding34.h>
#include <MDIncrementalRefreshSessionStatistics35.h>
#include <MDIncrementalRefreshTrade36.h>
#include <MDIncrementalRefreshVolume37.h>
#include <MDInstrumentDefinitionOption41.h>
#include <MDIncrementalRefreshTradeSummary42.h>
#include <MDInstrumentDefinitionFuture27.h>
#include <MDInstrumentDefinitionSpread29.h>
#include <QuoteRequest39.h>
#include <SnapshotFullRefresh38.h>
#include <SecurityStatus30.h>
#include <ChannelReset4.h>

#include "error.h"
#include "CmeMdApi.h"

using namespace mktdata;
using namespace std;

void
CmeMdApi::SubscribeMarketData(const vector<string>& instruments)
{
	for (int i = 0; i < instruments.size(); ++i) {
		subscribe.insert(instruments[i]);
		obmap[instruments[i]];
	}
}

void
CmeMdApi::LoadInstrumentsConfig(const map<int, string>& config_map)
{
	instmap = config_map;
}

void
CmeMdApi::OnRefreshLimitsBanding(MessageHeader& hdr, char *buf, int len)
{
	return;

	MDIncrementalRefreshLimitsBanding34 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noMDEntries();

	while (ent.hasNext()) {
		ent.next();

		const int secid = ent.securityID();

		//if (instmap.find(secid) == instmap.end())
		//	continue;
		string instid = instmap[secid];
		/*if (subscribe.find(instid) == subscribe.end())
			continue;*/

		auto& ob = obmap[instid];
		snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

		ob.secid = secid;
		ob.seqno = ent.rptSeq();
		ob.transact_time = msg.transactTime();

		int64_t high_limit = ent.highLimitPrice().mantissa();
		int64_t low_limit = ent.lowLimitPrice().mantissa();
		int64_t max_price_variation = ent.maxPriceVariation().mantissa();
		int rpt_seq = ent.rptSeq();
		int md_update_action = ent.mDUpdateAction();
		string md_entry_type = ent.mDEntryType();
		cout << "Limits Banding: SecurityID:" << secid
				 << " RptSeq:" << rpt_seq
				 << " HighLimit:" << high_limit
				 << " LowLimit:" << low_limit
				 << " MaxPriceVariation:" << max_price_variation
				 << " UpdateAction:" << md_update_action
				 << " EntryType:" << md_entry_type << endl;
		
		OnRefreshBook(&ob);
	}

}

void
CmeMdApi::Disseminate(char *buf, int len)
{
	struct binhdr *binhdr = (struct binhdr *)buf;
	int offset = sizeof(binhdr->msg_seqno) + sizeof(binhdr->sending_time);

	MessageHeader hdr;

	while (offset < len) {
		int16_t msg_size = *(int16_t *)&buf[offset];
		hdr.wrap(buf, offset + sizeof(msg_size), ver, offset + msg_size);

		switch (hdr.templateId()) {
		case 4:
			OnChannelReset(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 12:
			// OnHeartBeat();
			break;
		case 27:
			OnRefreshSecurityDefinitionFuture(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 29:
			OnRefreshSecurityDefinitionSpread(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 30:
			OnSecurityStatus(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 38:
			OnSnapshotFullRefresh(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 39:
			// printf("unknown %d\n", hdr.templateId());
			OnQuoteRequest(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 32:
			OnRefreshBook(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 33:
			OnRefreshDailyStatistics(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 34:
			OnRefreshLimitsBanding(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 35:
			OnRefreshSessionStatistics(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 36:
			OnRefreshTrade(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 37:
			OnRefreshVolume(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		case 41:
      OnInstrumentDefinitionOption(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
      break;
		case 42:
			OnRefreshTradeSummary(hdr, buf + offset + sizeof(msg_size), msg_size - sizeof(msg_size));
			break;
		default:
			printf("unknown templateId %d\n", hdr.templateId());
			break;
		}

		offset += msg_size;
	}
}

void
CmeMdApi::OnRefreshSessionStatistics(MessageHeader& hdr, char *buf, int len)
{
	MDIncrementalRefreshSessionStatistics35 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noMDEntries();

	while (ent.hasNext()) {
		ent.next();

		const int secid = ent.securityID();

		//if (instmap.find(secid) == instmap.end())
		//	continue;
		string instid = instmap[secid];
		/*if (subscribe.find(instid) == subscribe.end())
			continue;*/

		int64_t md_entry_px = ent.mDEntryPx().mantissa();
		int rpt_seq = ent.rptSeq();
		int open_close_settl_flag = ent.openCloseSettlFlag();
		int md_update_action = ent.mDUpdateAction();
		auto md_entry_type = ent.mDEntryType();
		cout << "Session Statistics: SecurityID:" << secid
				 << " RptSeq:" << rpt_seq
				 << " EntryPx:" << md_entry_px
				 << " EntryType:" << md_entry_type
				 << " OpenCloseSettlFlag:" << open_close_settl_flag << endl;
		

		auto& ob = obmap[instid];
		snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

		ob.secid = secid;
		ob.seqno = ent.rptSeq();
		ob.transact_time = msg.transactTime();

		OnRefreshBook(&ob);
	}
}

void
CmeMdApi::OnRefreshDailyStatistics(MessageHeader& hdr, char *buf, int len)
{
	MDIncrementalRefreshDailyStatistics33 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noMDEntries();

	while (ent.hasNext()) {
		ent.next();

		const int secid = ent.securityID();

		//if (instmap.find(secid) == instmap.end())
		//	continue;
		string instid = instmap[secid];
		/*if (subscribe.find(instid) == subscribe.end())
			continue;*/

		auto& ob = obmap[instid];
		snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

		ob.secid = secid;
		ob.seqno = ent.rptSeq();
		ob.transact_time = msg.transactTime();

		int64_t md_entry_px = ent.mDEntryPx().mantissa();
		int rpt_seq = ent.rptSeq();
		auto md_entry_type = ent.mDEntryType();
		// int md_entry_size = ent.mDEntrySize();
		auto trading_date = ent.tradingReferenceDate();
		auto settl_price_type = ent.settlPriceType();

		switch(md_entry_type) {
			case MDEntryTypeDailyStatistics::ClearedVolume: {
				printf("Cleared Volume:\n");
			}
			break;
			case MDEntryTypeDailyStatistics::SettlementPrice: {
				printf("Settlement Price:\n");
				if (settl_price_type.finalfinal()) 
					cout << "finalfinal " << endl;
				if (settl_price_type.actual()) 
					cout << "actual " << endl;
				if (settl_price_type.rounded()) 
					cout << "rounded " << endl;
				if (settl_price_type.intraday()) 
					cout << "intraday " << endl;
				if (settl_price_type.reservedBits()) 
					cout << "reservedBits" << endl;
				if (settl_price_type.nullValue()) 
					cout << "nullValue" << endl;
			}
			break;
			case MDEntryTypeDailyStatistics::OpenInterest: {
				printf("Open Interest:\n");
			}
			break;
			case MDEntryTypeDailyStatistics::FixingPrice: {
				printf("Fixing Price:\n");
			}
			break;
			case MDEntryTypeStatistics::OpenPrice: {
				printf("Open Price:\n");
			}
			break;
			case MDEntryTypeStatistics::HighTrade: {
				printf("High Trade:\n");
			}
			break;
			case MDEntryTypeStatistics::LowTrade: {
				printf("Low Trade:\n");
			}
			break;
			case MDEntryTypeStatistics::HighestBid: {
				printf("Highest Bid:\n");
			}
			break;
			case MDEntryTypeStatistics::LowestOffer: {
				printf("Lowest Offer:\n");
			}
			break;
			default:
			break;
		}
		cout << "Session Statistics: SecurityID:" << secid
				 << " RptSeq:" << rpt_seq
				 << " EntryPx:" << md_entry_px
				 // << " EntrySize:" << md_entry_size
				 << " EntryType:" << md_entry_type
				 << " TradingDate:" << trading_date << endl;

		// auto md_entry_type = ent.mDEntryType();

		OnRefreshBook(&ob);
	}
}

void
CmeMdApi::OnInstrumentDefinitionOption(MessageHeader& hdr, char *buf, int len)
{
	MDInstrumentDefinitionOption41 msg;

	// msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	// auto& ent = msg.noMDEntries();

	// while (ent.hasNext()) {
	// 	ent.next();
  //
	// 	const int secid = ent.securityID();
  //
	// 	int rpt_seq = ent.rptSeq();
	// 	int md_entry_size = ent.mDEntrySize();
	// 	int md_update_action = ent.mDUpdateAction();
	// 	string md_entry_type = ent.mDEntryType();
  //
	// 	cout << "RefreshVolume: SecurityID:" << secid
	// 			 << " RptSeq:" << rpt_seq
	// 			 << " EntrySize:" << md_entry_size
	// 			 << " UpdateAction:" << md_update_action
	// 			 << " EntryType:" << md_entry_type << endl;
	// 	//if (instmap.find(secid) == instmap.end())
	// 	//	continue;
	// 	string instid = instmap[secid];
	// 	// if (subscribe.find(instid) == subscribe.end())
	// 	//	continue;
  //
	// 	auto& ob = obmap[instid];
	// 	snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());
  //
	// 	ob.secid = secid;
	// 	ob.seqno = ent.rptSeq();
	// 	ob.transact_time = msg.transactTime();
  //
	// 	OnRefreshBook(&ob);
	// }
}


void
CmeMdApi::OnRefreshVolume(MessageHeader& hdr, char *buf, int len)
{
	MDIncrementalRefreshVolume37 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noMDEntries();

	while (ent.hasNext()) {
		ent.next();

		const int secid = ent.securityID();
		
		int rpt_seq = ent.rptSeq();
		int md_entry_size = ent.mDEntrySize();
		int md_update_action = ent.mDUpdateAction();
		string md_entry_type = ent.mDEntryType();
		
		cout << "RefreshVolume: SecurityID:" << secid
				 << " RptSeq:" << rpt_seq
				 << " EntrySize:" << md_entry_size
				 << " UpdateAction:" << md_update_action
				 << " EntryType:" << md_entry_type << endl;
		//if (instmap.find(secid) == instmap.end())
		//	continue;
		string instid = instmap[secid];
		// if (subscribe.find(instid) == subscribe.end())
		//	continue;

		auto& ob = obmap[instid];
		snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

		ob.secid = secid;
		ob.seqno = ent.rptSeq();
		ob.transact_time = msg.transactTime();

		OnRefreshBook(&ob);
	}
}

void
CmeMdApi::OnRefreshBook(MessageHeader& hdr, char *buf, int len)
{
	MDIncrementalRefreshBook32 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noMDEntries();

	while (ent.hasNext()) {
		ent.next();

		const int secid = ent.securityID();

		//if (instmap.find(secid) == instmap.end())
		//	continue;
		string instid = instmap[secid];
		//if (subscribe.find(instid) == subscribe.end())
		//	continue;

		auto& ob = obmap[instid];
		snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

		if (ent.mDEntryType() == MDEntryTypeBook::Bid || ent.mDEntryType() == MDEntryTypeBook::Offer) {
			struct level *level = ent.mDEntryType() == MDEntryTypeBook::Bid ? ob.bid : ob.ask;
			const int nlevel = ent.mDPriceLevel() - 1;

			switch (ent.mDUpdateAction()) {
			case MDUpdateAction::New:
				for (int i = 9; i > nlevel; --i)
				// for (int i = 4; i > nlevel; --i)
					level[i] = level[i - 1];
				// fall through
			case MDUpdateAction::Change:
				// if (ent.mDUpdateAction() == MDUpdateAction::Change) {
				// 	printf("Change order: level-%d price-%ld size-%d\n", nlevel,
				// 			 ent.mDEntryPx().mantissa(), ent.mDEntrySize());
				// }
				// if (ent.mDUpdateAction() == MDUpdateAction::Change &&
				// 		level[nlevel].price != ent.mDEntryPx().mantissa()) {
				// 	printf("Warning: Change level-%d prices[%ld-%ld] not matched\n",
				// 				 nlevel + 1, level[nlevel].price, ent.mDEntryPx().mantissa());
				// }
				level[nlevel].price = ent.mDEntryPx().mantissa();
				level[nlevel].size = ent.mDEntrySize();
				level[nlevel].norder = ent.numberOfOrders();

				ob.secid = secid;
				ob.seqno = ent.rptSeq();
				ob.transact_time = msg.transactTime();

				if (ob.bid[0].price < ob.ask[0].price)
					OnRefreshBook(&ob);

				break;
			case MDUpdateAction::Delete: {
				// printf("Delete order: level-%d price-%ld size-%d\n", nlevel,
				// 			 ent.mDEntryPx().mantissa(), ent.mDEntrySize());
				for (int i = nlevel; i < 9; ++i)
				// for (int i = nlevel; i < 4; ++i)
					level[i] = level[i + 1];

				break;
			}
			case MDUpdateAction::DeleteThru:
				// printf("Delete thru: delete all book on one side\n");
				for (int i = 0; i < 10; i++)
					level[i].price = level[i].size = level[i].norder = 0;
				OnRefreshBook(&ob);
				break;
			case MDUpdateAction::DeleteFrom:
				// printf("Delete from: delete top %d levels of book on one side\n",
				// 			 nlevel + 1);
				for (int i = 0; i < nlevel + 1; i++)
					level[i].price = level[i].size = level[i].norder = 0;
				OnRefreshBook(&ob);
				break;
			
			default:
				printf(",unknown");
				break;
			}
		} else if (ent.mDEntryType() == MDEntryTypeBook::ImpliedBid || ent.mDEntryType() == MDEntryTypeBook::ImpliedOffer) {
			ob.secid = secid;
			ob.seqno = ent.rptSeq();
			ob.transact_time = msg.transactTime();

			OnRefreshBook(&ob);
		}
	}
}

void
CmeMdApi::OnRefreshTradeSummary(MessageHeader& hdr, char *buf, int len)
{
	MDIncrementalRefreshTradeSummary42 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noMDEntries();
	auto& order_id_ent = msg.noOrderIDEntries();

	while(order_id_ent.hasNext()) {
		order_id_ent.next();
		int order_id = order_id_ent.orderID();
		int last_qty = order_id_ent.lastQty();
		cout << "NoOrderIDEntries: OrderID:" << order_id 
				 << "	Qty:" << last_qty << endl;
	}

	while (ent.hasNext()) {
		ent.next();

		const int secid = ent.securityID();

		int rpt_seq = ent.rptSeq();
		int64_t md_entry_px = ent.mDEntryPx().mantissa();
		int md_entry_size = ent.mDEntrySize();
		int no_orders = ent.numberOfOrders();
		int aggressor_side = ent.aggressorSide();
		string md_entry_type = ent.mDEntryType();
		// int md_trade_entry_id = ent.mDTradeEntryID();
		cout << "TradeSummary: SecurityID:" << secid
				 << " RptSeq:" << rpt_seq
				 << " Price:" << md_entry_px
				 << " EntrySize:" << md_entry_size
				 << " OrderNo:" << no_orders
				 << " AggressorSide:" << aggressor_side
				 << " EntryType:" << md_entry_type << endl;
				 // << " TradeEntryID:" << md_trade_entry_id << endl;
		//if (instmap.find(secid) == instmap.end())
		//	continue;
		string instid = instmap[secid];
		//if (subscribe.find(instid) == subscribe.end())
		//	continue;

		auto& ob = obmap[instid];
		snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

		ob.secid = secid;
		ob.seqno = ent.rptSeq();
		ob.transact_time = msg.transactTime();

		OnRefreshBook(&ob);
	}
}

void
CmeMdApi::OnRefreshTrade(MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
	return;
#endif
	MDIncrementalRefreshTrade36 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noMDEntries();

	while (ent.hasNext()) {
		ent.next();

		const int secid = ent.securityID();

		int rpt_seq = ent.rptSeq();
		int64_t md_entry_px = ent.mDEntryPx().mantissa();
		int md_entry_size = ent.mDEntrySize();
		int no_orders = ent.numberOfOrders();
		int aggressor_side = ent.aggressorSide();
		int trade_id = ent.tradeID();
		string md_entry_type = ent.mDEntryType();
		int md_update_action = ent.mDUpdateAction();
		// int md_trade_entry_id = ent.mDTradeEntryID();
		cout << "TradeSummary: SecurityID:" << secid
				 << " RptSeq:" << rpt_seq
				 << " Price:" << md_entry_px
				 << " EntrySize:" << md_entry_size
				 << " OrderNo:" << no_orders
				 << " AggressorSide:" << aggressor_side
				 << " TradeID:" << trade_id
				 << " UpdateAction:" << md_update_action
				 << " EntryType:" << md_entry_type << endl;
	}
}

void
CmeMdApi::OnRefreshSecurityDefinitionFuture(MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
	return;
#endif
	MDInstrumentDefinitionFuture27 msg;
	
	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noEvents();
	auto security_id = msg.securityID();
	int tot_num_reports = msg.totNumReports();
	while(ent.hasNext()) {
		ent.next();
		const auto event_time = ent.eventTime();
		auto event_type = ent.eventType();
		string security_group = msg.getSecurityGroupAsString();
		auto market_segment_id = msg.marketSegmentID();
		auto symbol = msg.symbol();
		cout << "Instrument Definition : " \
				 << " event time:" << TransferTimestamp(event_time) \
				 << " event type:" << event_type \
				 << " security ID:" << security_id \
				 << " symbol:" << symbol \
				 << " security group:" << security_group 
				 << " total number reports:" << tot_num_reports 
				 << " market segment ID:" << market_segment_id << endl;
	}
	
	auto& feed_types = msg.noMDFeedTypes();
	while(feed_types.hasNext()) {
		feed_types.next();
		int market_depth = feed_types.marketDepth();
		cout << "NoMDFeedTypes : market depth - " << market_depth << endl;
	}
}


void
CmeMdApi::OnRefreshSecurityDefinitionSpread(MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
	return;
#endif
	MDInstrumentDefinitionSpread29 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto security_id = msg.securityID();
	auto& ent = msg.noEvents();
	while(ent.hasNext()) {
		ent.next();
	const auto event_time = ent.eventTime();
	auto event_type = ent.eventType();
	string security_group = msg.getSecurityGroupAsString();
	auto market_segment_id = msg.marketSegmentID();
	auto symbol = msg.symbol();
	
	cout << "Instrument Definition : "
			 << " event time:" << TransferTimestamp(event_time)
			 << " event type:" << event_type
			 << " security ID:" << security_id
			 << " symbol:" << symbol
			 << " security group:" << security_group
			 << " market segment ID:" << market_segment_id << endl;
	}
}

void
CmeMdApi::OnQuoteRequest(MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
	return;
#endif
	QuoteRequest39 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);
	
	auto& sym = msg.noRelatedSym();
	while(sym.hasNext()) {
		sym.next();
	string symbol = sym.symbol();
	int security_id = sym.securityID();
	int order_qty = sym.orderQty();
	cout << "Quote Request : " \
			 << " symbol:" << symbol << " securityID:" << security_id \
			 << " OrderQty:" << order_qty << endl;
	}
}

void
CmeMdApi::OnSecurityStatus(MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
	return;
#endif
	SecurityStatus30 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);
	
	auto security_status = msg.securityTradingStatus();
		
	switch(security_status) {
		case SecurityTradingStatus::TradingHalt: {
			cout << "Status: Trading Halt:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::Close: {
			cout << "Status: Close:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::NewPriceIndication: {
			cout << "Status: New Price Indication:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::ReadyToTrade: {
			cout << "Status: Ready To Trade:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::NotAvailableForTrading: {
			cout << "Status: Not Available For Trading:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::UnknownorInvalid: {
			cout << "Status: Unknown or Invalid:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::PreOpen: {
			cout << "Status: Pre-Open:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::PreCross: {
			cout << "Status: Pre-Cross:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::Cross: {
			cout << "Status: Cross:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::PostClose: {
			cout << "Status: Post-Close:" << security_status << endl;
		}
		break;
		case SecurityTradingStatus::NoChange: {
			cout << "Status: No Change:" << security_status << endl;
		}
		case SecurityTradingStatus::NULL_VALUE: {
			cout << "Status: NULL VALUE:" << security_status << endl;
		}
		break;
	}
	
	string security_group = msg.securityGroup();
	string asset = msg.asset();
	int secid = msg.securityID();
	// int security_trading_status = msg.securityTradingStatus();
	auto trade_date = msg.tradeDate();
	int halt_reason = msg.haltReason();
	int security_trading_event = msg.securityTradingEvent();

	cout << "SecurityStatus: SecurityID:" << secid
			 << " SecurityGroup:" << security_group
			 << " Asset:" << asset
			 << " TradeDate:" << trade_date
			 << " SecurityTradingStatus:" << security_status
			 << " HaltReason:" << halt_reason
			 << " SecurityTradingEvent:" << security_trading_event << endl;
}

void
CmeMdApi::OnChannelReset(MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
	return;
#endif
	ChannelReset4 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noMDEntries();

	while(ent.hasNext()) {
		ent.next();
		int md_update_action = ent.mDUpdateAction();
		string md_entry_type = ent.mDEntryType();
		cout << "Channel Reset: MDUpdateAction:" << md_update_action
				 << " MDEntryType:" << md_entry_type << endl;
	}
	ResetBooks();
}

void
CmeMdApi::OnSnapshotFullRefresh(MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
	return;
#endif
	SnapshotFullRefresh38 msg;

	msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), ver, len);

	auto& ent = msg.noMDEntries();

	int last_seq_no = msg.lastMsgSeqNumProcessed();
	int tot_num_reports = msg.totNumReports();
	int secid = msg.securityID();
	int rpt_seq = msg.rptSeq();
	auto trade_date = msg.tradeDate();
	int security_trading_status = msg.mDSecurityTradingStatus();
	int64_t high_limit = msg.highLimitPrice().mantissa();
	int64_t low_limit = msg.lowLimitPrice().mantissa();
	int64_t max_price_variation = msg.maxPriceVariation().mantissa();

	cout << "SnapshotFullRefresh: SecurityID:" << secid
			 << " TotNumReport:" << tot_num_reports
			 << " LastMsgSeqNumProcessed:" << last_seq_no
			 << " RptSeq:" << rpt_seq
			 << " TradeDate:" << trade_date
			 << " SecurityTradingStatus:" << security_trading_status
			 << " HighLimit:" << high_limit
			 << " LowLimit:" << low_limit
			 << " MaxPriceVariation:" << max_price_variation << endl;
	while(ent.hasNext()) {
		ent.next();
		int64_t md_entry_px = ent.mDEntryPx().mantissa();
		int md_entry_size = ent.mDEntrySize();
		int no_orders = ent.numberOfOrders();
		int md_price_level = ent.mDPriceLevel();
		auto trading_date = ent.tradingReferenceDate();
		int open_close_settl_flag = ent.openCloseSettlFlag();
		auto settl_price_type = ent.settlPriceType();
		char md_entry_type = ent.mDEntryType();

		cout << "noMDEntries: SecurityID:" << secid
				 << " Price:" << md_entry_px
				 << " EntrySize:" << md_entry_size
				 << " OrderNo:" << no_orders
				 << " MDPriceLevel:" << md_price_level
				 << " tradingReferenceDate:" << trading_date
				 << " OpenCloseSettlFlag:" << open_close_settl_flag
				 << " EntryType:" << md_entry_type << endl;
		printf("Settlement Price Type: ");
		if (settl_price_type.finalfinal()) 
			cout << "finalfinal ";
		if (settl_price_type.actual()) 
			cout << "actual ";
		if (settl_price_type.rounded()) 
			cout << "rounded ";
		if (settl_price_type.intraday()) 
			cout << "intraday ";
		if (settl_price_type.reservedBits()) 
			cout << "reservedBits ";
		if (settl_price_type.nullValue()) 
			cout << "nullValue ";
		cout << endl;		
	}
	// ResetBooks();
}


void CmeMdApi::ResetBooks() {
	for (auto iter = obmap.begin(); iter != obmap.end(); iter++) {
		OrderBook& ob = iter->second;
		for (int i = 0; i < 10; i++) {
			ob.bid[i].price = 0;
			ob.bid[i].size = 0;
			ob.bid[i].norder = 0;
			ob.ask[i].price = 0;
			ob.ask[i].size = 0;
			ob.ask[i].norder = 0;
		}
		OnRefreshBook(&ob);
	}
}

string CmeMdApi::TransferTimestamp(uint64_t micro_sec) {
  uint64_t seconds = micro_sec / 1000000000;
  long int msec = (long int)seconds;
  struct tm *timeinfo = localtime(&msec);
  struct timeval tv;
  gettimeofday(&tv, NULL);
  char str[32];
  sprintf(str, "%d%02d%02d-%02d%02d", 1900 + timeinfo->tm_year,
          1 + timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour,
          timeinfo->tm_min);
  return string(str);
}
