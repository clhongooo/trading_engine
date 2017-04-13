/*
 * ATU_IB_Client_OTI.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: jasonlin
 */

#include "ATU_IB_Client_OTI.h"

namespace atu {

ATU_IB_Client_OTI::ATU_IB_Client_OTI() {}

ATU_IB_Client_OTI::~ATU_IB_Client_OTI() {}

void ATU_IB_Client_OTI::tickPrice(TickerId p_tickerId, TickType p_field, double p_price, int p_canAutoExecute) {}
void ATU_IB_Client_OTI::tickSize(TickerId p_tickerId, TickType p_field, int p_size) {}
void ATU_IB_Client_OTI::tickOptionComputation(TickerId p_tickerId, TickType p_tickType, double p_impliedVok, double p_delta, double p_optPrice, double p_pvDividend, double p_gamma, double p_vega, double p_theta, double p_undPrice) {}
void ATU_IB_Client_OTI::tickGeneric(TickerId p_tickerId, TickType p_tickType, double p_value) {}
void ATU_IB_Client_OTI::tickString(TickerId p_tickerId, TickType p_tickType, const IBString &p_value) {}
void ATU_IB_Client_OTI::tickEFP(TickerId p_tickerId, TickType p_tickType, double p_basisPoints, const IBString &p_formattedBasisPoints,
	double p_totalDividends, int p_holdDays, const IBString &p_futureExpiry, double p_dividendImpact, double p_dividendsToExpiry) {}
void ATU_IB_Client_OTI::tickSnapshotEnd(int p_reqId) {}
void ATU_IB_Client_OTI::marketDataType(TickerId p_reqId, int p_marketDataType) {}
void ATU_IB_Client_OTI::winError(const IBString &p_str, int p_lastError) {}
void ATU_IB_Client_OTI::connectionClosed() {}
void ATU_IB_Client_OTI::updateAccountValue(const IBString &p_key, const IBString &p_val, const IBString &p_currency, const IBString &p_accountName) {}
void ATU_IB_Client_OTI::updatePortfolio(const Contract &p_contract, int p_position, double p_marketPrice, double p_marketValue, double p_averageCost,double p_unrealizedPNL, double p_realizedPNL, const IBString &p_accountName) {}
void ATU_IB_Client_OTI::updateAccountTime(const IBString &p_timeStamp) {}
void ATU_IB_Client_OTI::accountDownloadEnd(const IBString &p_accountName) {}
void ATU_IB_Client_OTI::contractDetails(int p_reqId, const ContractDetails &p_contractDetails) {}
void ATU_IB_Client_OTI::bondContractDetails(int p_reqId, const ContractDetails &p_contractDetails) {}
void ATU_IB_Client_OTI::contractDetailsEnd(int p_reqId) {}
void ATU_IB_Client_OTI::updateMktDepth(TickerId p_id, int p_position, int p_operation, int p_side, double p_price, int p_size) {}
void ATU_IB_Client_OTI::updateMktDepthL2(TickerId p_id, int p_position, IBString p_marketMaker, int p_operation, int p_side, double p_price, int p_size) {}
void ATU_IB_Client_OTI::updateNewsBulletin(int p_msgId, int p_msgType, const IBString &p_newsMessage, const IBString &p_originExch) {}
void ATU_IB_Client_OTI::managedAccounts(const IBString &p_accountsList) {}
void ATU_IB_Client_OTI::receiveFA(faDataType p_pFaDataType, const IBString &p_cxml) {}
void ATU_IB_Client_OTI::historicalData(TickerId p_reqId, const IBString &p_date, double p_open, double p_high, double p_low, double p_close, int p_volume,
	int p_barCount, double p_WAP, int p_hasGaps) {}
void ATU_IB_Client_OTI::scannerParameters(const IBString &p_xml) {}
void ATU_IB_Client_OTI::scannerData(int p_reqId, int p_rank, const ContractDetails &p_contractDetails, const IBString &p_distance, const IBString &p_benchmark,
	const IBString &p_rojection, const IBString &p_legsStr) {}
void ATU_IB_Client_OTI::scannerDataEnd(int p_reqId) {}
void ATU_IB_Client_OTI::realtimeBar(TickerId p_reqId, long p_time, double p_open, double p_high, double p_low, double p_close, long p_volume, double p_wap, int p_count) {}
void ATU_IB_Client_OTI::currentTime(long p_time) {}
void ATU_IB_Client_OTI::fundamentalData(TickerId p_reqid, const IBString &p_data) {}
void ATU_IB_Client_OTI::commissionReport(const CommissionReport &p_commissionReport) {  }
void ATU_IB_Client_OTI::position(const IBString &p_account, const Contract &p_contract, int p_position, double p_avgCost) {}
void ATU_IB_Client_OTI::positionEnd() {}
void ATU_IB_Client_OTI::accountSummary(int p_reqId, const IBString &p_account, const IBString &p_tag, const IBString &p_value, const IBString &p_currency) {}
void ATU_IB_Client_OTI::accountSummaryEnd(int p_reqId) {}
void ATU_IB_Client_OTI::verifyMessageAPI(const IBString &p_apiData) {}
void ATU_IB_Client_OTI::verifyCompleted(bool p_isSuccessful, const IBString &p_errorText) {}
void ATU_IB_Client_OTI::displayGroupList(int p_reqId, const IBString &p_groups) {}
void ATU_IB_Client_OTI::displayGroupUpdated(int p_reqId, const IBString &p_contractInfo) {}

}


