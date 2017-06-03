/*
 * ATU_IB_Client_MDI_MDI.cpp
 *
 *  Created on: Oct 8, 2014
 *      Author: jasonlin
 */

#include "ATU_IB_Client_MDI.h"

ATU_IB_Client_MDI::ATU_IB_Client_MDI() {}
ATU_IB_Client_MDI::~ATU_IB_Client_MDI() {}
void ATU_IB_Client_MDI::orderStatus(OrderId p_orderId, const IBString &p_status, int p_filled, int p_remaining, double p_avgFillPrice, int p_permId,
                                    int p_parentId, double p_lastFillPrice, int p_clientId, const IBString& p_whyHeld) {}
void ATU_IB_Client_MDI::openOrder(OrderId orderId, const Contract&, const Order&, const OrderState&) {}
void ATU_IB_Client_MDI::openOrderEnd() {}
void ATU_IB_Client_MDI::winError(const IBString& p_str, int p_lastError) {}
void ATU_IB_Client_MDI::connectionClosed() {}
void ATU_IB_Client_MDI::updateAccountValue(const IBString& p_key, const IBString& p_val, const IBString& p_currency, const IBString& p_accountName) {}
void ATU_IB_Client_MDI::updatePortfolio(const Contract& p_contract, int p_position, double p_marketPrice, double p_marketValue, double p_averageCost,
                                        double p_unrealizedPNL, double p_realizedPNL, const IBString& p_accountName) {}
void ATU_IB_Client_MDI::updateAccountTime(const IBString &p_timeStamp) {}
void ATU_IB_Client_MDI::accountDownloadEnd(const IBString &p_accountName) {}
void ATU_IB_Client_MDI::nextValidId(OrderId p_orderId) {}
void ATU_IB_Client_MDI::contractDetails(int p_reqId, const ContractDetails& p_contractDetails) {}
void ATU_IB_Client_MDI::bondContractDetails(int p_reqId, const ContractDetails& p_contractDetails) {}
void ATU_IB_Client_MDI::contractDetailsEnd(int p_reqId) {}
void ATU_IB_Client_MDI::execDetails(int p_reqId, const Contract& p_contract, const Execution& p_execution) {}
void ATU_IB_Client_MDI::execDetailsEnd(int p_reqId) {}
void ATU_IB_Client_MDI::error(const int p_id, const int p_errorCode, const IBString p_errorString) { }
void ATU_IB_Client_MDI::updateMktDepthL2(TickerId p_id, int p_position, IBString p_marketMaker, int p_operation, int p_side, double p_price, int p_size) {}
void ATU_IB_Client_MDI::updateNewsBulletin(int p_msgId, int p_msgType, const IBString& p_newsMessage, const IBString& p_originExch) {}
void ATU_IB_Client_MDI::managedAccounts(const IBString& p_accountsList) {}
void ATU_IB_Client_MDI::receiveFA(faDataType p_pFaDataType, const IBString& p_cxml) {}
void ATU_IB_Client_MDI::historicalData(TickerId p_reqId, const IBString& p_date, double p_open, double p_high, double p_low, double p_close, int p_volume,
                                       int p_barCount, double p_WAP, int p_hasGaps) {}
void ATU_IB_Client_MDI::scannerParameters(const IBString& p_xml) {}
void ATU_IB_Client_MDI::scannerData(int p_reqId, int p_rank, const ContractDetails& p_contractDetails, const IBString& p_distance, const IBString& p_benchmark,
                                    const IBString& p_rojection, const IBString& p_legsStr) {}
void ATU_IB_Client_MDI::scannerDataEnd(int p_reqId) {}
void ATU_IB_Client_MDI::realtimeBar(TickerId p_reqId, long p_time, double p_open, double p_high, double p_low, double p_close, long p_volume, double p_wap, int p_count) {}
void ATU_IB_Client_MDI::currentTime(long p_time) { }
void ATU_IB_Client_MDI::fundamentalData(TickerId p_reqid, const IBString& p_data) {}
void ATU_IB_Client_MDI::deltaNeutralValidation(int p_reqId, const UnderComp& p_underComp) {}
void ATU_IB_Client_MDI::commissionReport(const CommissionReport& p_commissionReport) {}
void ATU_IB_Client_MDI::position(const IBString& p_account, const Contract& p_contract, int p_position, double p_avgCost) {}
void ATU_IB_Client_MDI::positionEnd() {}
void ATU_IB_Client_MDI::accountSummary(int p_reqId, const IBString& p_account, const IBString& p_tag, const IBString& p_value, const IBString& p_currency) {}
void ATU_IB_Client_MDI::accountSummaryEnd(int p_reqId) {}
void ATU_IB_Client_MDI::verifyMessageAPI(const IBString& p_apiData) {}
void ATU_IB_Client_MDI::verifyCompleted(bool p_isSuccessful, const IBString& p_errorText) {}
void ATU_IB_Client_MDI::displayGroupList(int p_reqId, const IBString& p_groups) {}
void ATU_IB_Client_MDI::displayGroupUpdated(int p_reqId, const IBString& p_contractInfo) {}
