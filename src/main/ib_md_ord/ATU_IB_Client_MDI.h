/*
 * ATU_IB_Client_MDI.h
 *
 *  Created on: Oct 8, 2014
 *      Author: jasonlin
 */

#ifndef ATU_IB_CLIENT_MDI_H_
#define ATU_IB_CLIENT_MDI_H_


#include "EWrapper.h"

namespace atu {

class ATU_IB_Client_MDI : public EWrapper
{
public:
	ATU_IB_Client_MDI();
	virtual ~ATU_IB_Client_MDI();

	// Market Data
	virtual void tickPrice(TickerId p_tickerId, TickType p_field, double p_price, int p_canAutoExecute) = 0;
	virtual void tickSize(TickerId p_tickerId, TickType p_field, int p_size) = 0;
	virtual void tickOptionComputation(TickerId p_tickerId, TickType p_tickType, double p_impliedVok, double p_delta,
			double p_optPrice, double p_pvDividend, double p_gamma, double p_vega, double p_theta, double p_undPrice) = 0;
	virtual void tickGeneric(TickerId p_tickerId, TickType p_tickType, double p_value) = 0;
	virtual void tickString(TickerId p_tickerId, TickType p_tickType, const IBString &p_value) = 0;
	virtual void tickEFP(TickerId p_tickerId, TickType p_tickType, double p_basisPoints, const IBString &p_formattedBasisPoints,
			double p_totalDividends, int p_holdDays, const IBString &p_futureExpiry, double p_dividendImpact, double p_dividendsToExpiry) = 0;
	virtual void tickSnapshotEnd(int p_reqId) = 0;
	virtual void marketDataType(TickerId p_reqId, int p_marketDataType) = 0;
	virtual void updateMktDepth(TickerId p_id, int p_position, int p_operation, int p_side, double p_price, int p_size) = 0;
	void error(const int p_id, const int p_errorCode, const IBString p_errorString) = 0;

	void orderStatus(OrderId p_orderId, const IBString &p_status, int p_filled, int p_remaining, double p_avgFillPrice, int p_permId,
			int p_parentId, double p_lastFillPrice, int p_clientId, const IBString &p_whyHeld);
	void openOrder(OrderId p_orderId, const Contract &p_contract, const Order &p_order, const OrderState &p_orderState);
	void openOrderEnd();
	void winError(const IBString &p_str, int p_lastError);
	void connectionClosed();
	void updateAccountValue(const IBString &p_key, const IBString &p_val, const IBString &p_currency, const IBString &p_accountName);
	void updatePortfolio(const Contract &p_contract, int p_position, double p_marketPrice, double p_marketValue, double p_averageCost,
			double p_unrealizedPNL, double p_realizedPNL, const IBString &p_accountName);
	void updateAccountTime(const IBString &p_timeStamp);
	void accountDownloadEnd(const IBString &p_accountName);
	void nextValidId(OrderId p_orderId);
	void contractDetails(int p_reqId, const ContractDetails &p_contractDetails);
	void bondContractDetails(int p_reqId, const ContractDetails &p_contractDetails);
	void contractDetailsEnd(int p_reqId);
	void execDetails(int p_reqId, const Contract &p_contract, const Execution& p_execution);
	void execDetailsEnd(int p_reqId);
	void updateMktDepthL2(TickerId p_id, int p_position, IBString p_marketMaker, int p_operation, int p_side, double p_price, int p_size);
	void updateNewsBulletin(int p_msgId, int p_msgType, const IBString &p_newsMessage, const IBString &p_originExch);
	void managedAccounts(const IBString &p_accountsList);
	void receiveFA(faDataType p_pFaDataType, const IBString &p_cxml);
	void historicalData(TickerId p_reqId, const IBString &p_date, double p_open, double p_high, double p_low, double p_close, int p_volume,
			int p_barCount, double p_WAP, int p_hasGaps);
	void scannerParameters(const IBString &p_xml);
	void scannerData(int p_reqId, int p_rank, const ContractDetails &p_contractDetails, const IBString &p_distance, const IBString &p_benchmark,
			const IBString &p_rojection, const IBString &p_legsStr);
	void scannerDataEnd(int p_reqId);
	void realtimeBar(TickerId p_reqId, long p_time, double p_open, double p_high, double p_low, double p_close, long p_volume, double p_wap, int p_count);
	void currentTime(long p_time);
	void fundamentalData(TickerId p_reqid, const IBString &p_data);
	void deltaNeutralValidation(int p_reqId, const UnderComp &p_underComp);
	void commissionReport(const CommissionReport &p_commissionReport);
	void position(const IBString &p_account, const Contract &p_contract, int p_position, double p_avgCost);
	void positionEnd();
	void accountSummary(int p_reqId, const IBString &p_account, const IBString &p_tag, const IBString &p_value, const IBString &p_currency);
	void accountSummaryEnd(int p_reqId);
	void verifyMessageAPI(const IBString &p_apiData);
	void verifyCompleted(bool p_isSuccessful, const IBString &p_errorText);
	void displayGroupList(int p_reqId, const IBString &p_groups);
	void displayGroupUpdated(int p_reqId, const IBString &p_contractInfo);

};

}

#endif /* ATU_IB_CLIENT_MDI_H_ */
