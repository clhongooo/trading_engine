/*
 * ATU_IB_MDI.cpp
 *
 *  Created on: Oct 8, 2014
 *      Author: jasonlin
 */

#include "ATU_IB_MDI.h"
#include "Utility.h"
#include "Toolbox.h"
#include "ConfigMgr.h"
#include <string>
#include <boost/lexical_cast.hpp>
#include "IniContractReader.h"

namespace atu {

ATU_IB_MDI::ATU_IB_MDI(const string &p_host, unsigned int p_port, int p_clientId) :
		 m_host(p_host)
		,m_port(p_port)
		,m_clientId(p_clientId)
		,m_account(IBString(""))
		,m_isReady(false)
		,m_clientSocket(this)
		,m_nextTickerId(0)
{
    //--------------------------------------------------
    // Sunny added - [start]
    //--------------------------------------------------
    cout << "IB MDI: SVN revision no. (IB MDI): "           << string(__SVN_REV__)       << endl;
    cout << "IB MDI: SVN revision no. (Entire workspace): " << string(__SVN_WKSPC_REV__) << endl;
    //--------------------------------------------------
    // Sunny added - [end]
    //--------------------------------------------------
	m_subscribeList.clear();
}

ATU_IB_MDI::ATU_IB_MDI() :
		 m_port(-1)
		,m_clientId(-1)
		,m_isReady(false)
		,m_clientSocket(this)
		,m_nextTickerId(0)
{

}

ATU_IB_MDI::~ATU_IB_MDI()
{
	if (m_contractExtractor != NULL) {
		delete m_contractExtractor;
	}
}

// @param address : example "192.168.168.110@7496@1@accountName" (IP@port@clientId@accountName)
void ATU_IB_MDI::setConnectString(string p_connStr)
{
	// If the passed parameter ends with ".ini", read config from a file
	// Otherwise, it is a plain text string
	string ext = ".ini";
	// If p_connStr ends with ".ini"
	if ( p_connStr.length() > 4 && p_connStr.compare(p_connStr.length() - ext.length(), ext.length(), ext) == 0 ) {

		ConfigMgr *configMgr = new ConfigMgr();
		Config *config = configMgr->getConfig(p_connStr);
		for (map<string, string>::iterator iter = config->m_keyValuePairMap.begin(); iter != config->m_keyValuePairMap.end(); iter++) {
			cout << iter->first << " = " << iter->second << endl;
		}
		m_host = Utility::resolve2IP( config->getValue("MDI.host") );

		m_port = atoi(config->getValue("MDI.port").c_str());
		m_clientId = atoi(config->getValue("MDI.client_id").c_str());
		m_account = IBString(config->getValue("MDI.account"));

		readContracts(config->getValue("MDI.contract_file"));

	} else {
		throw "ConnectString should be a .ini file path.";
	}
//	vector<string> segments;
//	Toolbox::split(segments, address, "@");
//	if (segments.size() == 4) {
//		m_host = segments[0];
//		m_port = atoi(segments[1].c_str());
//		m_clientId = atoi(segments[2].c_str());
//		m_account = IBString(segments[3]);
//	}
}

void ATU_IB_MDI::readContracts(string p_filepath) {
	m_contractExtractor = new ContractManager();
	cout << "Reading contracts..." << endl;
	IniContractReader* iniReader = new IniContractReader(p_filepath);
	m_contractExtractor->extract(iniReader);
	delete iniReader;
}

void ATU_IB_MDI::init()
{

}

bool ATU_IB_MDI::isReady() const
{
	return m_isReady;
}

void ATU_IB_MDI::detach()
{
	if (m_host.size() == 0 || m_port == -1 || m_clientId == -1) {
		addLog(__LOGSOURCE__, ATU_logfeed_struct::INFO, "%s", "s", "--->>> IP, port, and client ID are all necessary");
		return;
	}

	if (m_isReady)
		return;

	// All connection stuff asigned to socket thread
	// Connect to server
	addLog(__LOGSOURCE__, ATU_logfeed_struct::INFO, "%s", "s", "--->>> Initiating MDI connection to TWS...");
	bool bConn = m_clientSocket.eConnect(m_host.c_str(), m_port, m_clientId);
	if (bConn) {
		addLog(__LOGSOURCE__, ATU_logfeed_struct::INFO, "%s", "s", "--->>> MDI connected to TWS!");
		m_isReady = true;
		m_clientSocket.startHandlingSendRecvMsg();
	} else {
		addLog(__LOGSOURCE__, ATU_logfeed_struct::INFO, "%s", "s", "--->>> MDI failed to connect to TWS.");
	}
}

// Interfaces in ATU_IB_Client_MDI
void ATU_IB_MDI::tickPrice(TickerId p_tickerId, TickType p_field, double p_price, int p_canAutoExecute)
{
//	cout << "*********************[tickPrice]" << p_field << " | " << p_price << endl;
	bool usingLevel2 = m_usingLevel2[p_tickerId];
	if (!usingLevel2) {
//		cout << "Using level 1 " << this->m_marketDepthData[p_tickerId].m_feedcode << endl;
		// TODO:
		// DON'T implement this. Data is incorrect.
		MarketDepthData &depthData = m_marketDepthData[p_tickerId];
		IBString secType = m_subscribeList[p_tickerId].secType;
		// Best bid
		if (p_field == BID && secType != "IND") {
//			cout << "Best bid price: " << p_price << endl;
			double *price = &(depthData.m_bidsPriceVol[0].first);
			*price = p_price;
			ATU_MDI_marketfeed_struct mf = depthData.getMarketfeed();
			notify_marketfeed(mf);
		}
		// Best ask
		else if (p_field == ASK && secType != "IND") {
//			cout << "Best ask price: " << p_price << endl;
			double *price = &(depthData.m_asksPriceVol[0].first);
			*price = p_price;
			ATU_MDI_marketfeed_struct mf = depthData.getMarketfeed();
			notify_marketfeed(mf);
		}
		// last price
		else if (p_field == LAST && secType == "IND") {
			depthData.m_traded_price = p_price;
			ATU_MDI_marketfeed_struct mf = depthData.getMarketfeed();
			notify_marketfeed(mf);
		}
		// Mark price (just for IB internal usage)
//		else if (p_field == 37 && secType == "IND") {
//			depthData.m_traded_price = p_price;
//			ATU_MDI_marketfeed_struct mf = depthData.getMarketfeed();
//			notify_marketfeed(mf);
//		}
	}
}

void ATU_IB_MDI::tickSize(TickerId p_tickerId, TickType p_field, int p_size)
{
//	cout << "------------------[tickSize]" << p_field << " | " << p_size << endl;
	bool usingLevel2 = m_usingLevel2[p_tickerId];
	if (!usingLevel2) {
//		cout << "Using level 1 " << this->m_marketDepthData[p_tickerId].m_feedcode << endl;
		// TODO:
		// Don't implement this. Data is incorrect.
		MarketDepthData &depthData = m_marketDepthData[p_tickerId];
		IBString secType = m_subscribeList[p_tickerId].secType;
		// Best bid
		if (p_field == BID_SIZE && secType != "IND") {
//			cout << "Best bid volume: " << p_size << endl;
			depthData.m_bidsPriceVol[0].second = p_size;
			ATU_MDI_marketfeed_struct mf = depthData.getMarketfeed();
			notify_marketfeed(mf);
		}
		// Best ask
		else if (p_field == ASK_SIZE && secType != "IND") {
//			cout << "Best ask volume: " << p_size << endl;
			depthData.m_asksPriceVol[0].second = p_size;
			ATU_MDI_marketfeed_struct mf = depthData.getMarketfeed();
			notify_marketfeed(mf);
		}
		// Last size
		else if (p_field == LAST_SIZE && secType == "IND") {
			depthData.m_traded_volume = p_size;
			ATU_MDI_marketfeed_struct mf = depthData.getMarketfeed();
			notify_marketfeed(mf);
		}
	}
}

void ATU_IB_MDI::tickOptionComputation(TickerId p_tickerId, TickType p_tickType, double p_impliedVok, double p_delta,
		double p_optPrice, double p_pvDividend, double p_gamma, double p_vega, double p_theta, double p_undPrice)
{

}

void ATU_IB_MDI::tickGeneric(TickerId p_tickerId, TickType p_tickType, double p_value)
{

}

/*
 * tickString() is for updating last traded price and volume of non-index products
 */
void ATU_IB_MDI::tickString(TickerId p_tickerId, TickType p_tickType, const IBString& p_value)
{
	if (p_tickType == RT_VOLUME) {
//		cout << ">>>>>>>>>>>>>>>>>>>>>[tickString]" << p_value << endl;
		vector<string> segments;
		string s = p_value;
		Toolbox::split(segments, s, ";");
		if (segments.size() > 0) {
			MarketDepthData &depthData = m_marketDepthData[p_tickerId];
			double price = segments[0].size() > 0 ? boost::lexical_cast<double>(segments[0]) : depthData.m_traded_price;
			double volume = segments[3].size() > 0 ? boost::lexical_cast<double>(segments[3]) : depthData.m_traded_volume;

			depthData.m_traded_price = price;
			depthData.m_traded_volume = volume;

			ATU_MDI_marketfeed_struct mf = depthData.getMarketfeed();
			notify_marketfeed(mf);
		}
	}
}

void ATU_IB_MDI::tickEFP(TickerId p_tickerId, TickType p_tickType, double p_basisPoints, const IBString& p_formattedBasisPoints,
		double p_totalDividends, int p_holdDays, const IBString& p_futureExpiry, double p_dividendImpact, double p_dividendsToExpiry)
{

}

void ATU_IB_MDI::tickSnapshotEnd(int p_reqId)
{

}

void ATU_IB_MDI::marketDataType(TickerId p_reqId, int p_marketDataType)
{

}

void ATU_IB_MDI::updateMktDepth(TickerId p_id, int p_position, int p_operation, int p_side, double p_price, int p_size) {
	m_usingLevel2[p_id] = true;
//	cout << "Using level 2 " << this->m_marketDepthData[p_id].m_feedcode << endl;
//	cout << (p_operation == 0 ? "Insert" : (p_operation == 1 ? "Update" : "Delete")) << " [" << (p_position + 1) << "] " << (p_side == 0 ? "Ask" : "Bid") << ": price " << p_price << " size " << p_size << endl;
	bool valid = m_marketDepthData[p_id].update(p_position, p_operation, p_side, p_price, p_size);
	if (valid) {

		ATU_MDI_marketfeed_struct mf = m_marketDepthData[p_id].getMarketfeed();

IBString secType = m_subscribeList[p_id].secType;
if (secType == "CASH") {
	mf.m_traded_price = ( mf.m_ask_price_1 + mf.m_bid_price_1 ) / 2;
}
		notify_marketfeed(mf);
	}
}

// Interface in ATU_Abstract_MDI
bool ATU_IB_MDI::on_process_subscription(ATU_MDI_subscription_struct &s){

	m_updateTickerIdMutex.lock();

	if (m_feedcodeSet.find(s.m_feedcode) != m_feedcodeSet.end()) {
//		cout << "The feedcode: " << s.m_feedcode << " has been subscribed." << endl;
		m_updateTickerIdMutex.unlock();
		return false;
	}

	Contract contract;
	AtuContract* atuContract = NULL;
	bool wrongFeedcode = false;

	if ( m_contractExtractor->hasProductCode(s.m_feedcode) ) {
		atuContract = m_contractExtractor->getContract(s.m_feedcode);
		Utility::initIBContractFromAtuContract(&contract, atuContract, m_contractExtractor);
	}
	else {
		// The content of feedcode follows the pattern: {symbol}-{localSymbol}-{secType}
		vector<string> parts;
		Toolbox::split(parts, s.m_feedcode, "-");
		if (parts.size() >= 3) {
			contract.symbol = parts[0];
			contract.localSymbol = parts[1];
			contract.secType = parts[2];
			if (parts.size() > 3) {
				contract.currency = parts[3];
			}
			contract.exchange = s.m_market;
		}
		//	else if (s.m_feedcode.find('_') != string::npos) {
		//		Toolbox::split(parts, s.m_feedcode, "_");
		//		if (parts.size() == 2) {
		//			contract.conId = boost::lexical_cast<long>(parts[1]);
		//		} else {
		//			wrongFeedcode = true;
		//		}
		//	}
		else {
			wrongFeedcode = true;
		}
	}

	if (wrongFeedcode) {
		addLog(__LOGSOURCE__, ATU_logfeed_struct::INFO, "%s", "s", "Feedcode is incorrect. it should include symbol-localSymbol-secType.");
		m_updateTickerIdMutex.unlock();
		return false;
	}

	IBString geneticTicks("100,101,104,105,106,107,165,221,225,233,236,258,293,294,295,318");
	bool snapShot = false;
	TagValueListSPtr valList;

	m_feedcodeSet.insert(s.m_feedcode);
	// By default, usingLevel2 is set to false. When level2 data are returned, the value will be changed to true
	m_usingLevel2[m_nextTickerId] = false;
	m_subscribeList[m_nextTickerId] = contract;
	m_marketDepthData[m_nextTickerId] = MarketDepthData(s.m_feedcode);
	// snapshot should be false here!
	m_clientSocket.reqMktDataSync(m_nextTickerId, contract, geneticTicks, snapShot, valList);
	m_clientSocket.reqMktDepthSync(m_nextTickerId, contract, 20, valList);
	m_nextTickerId++;

	m_updateTickerIdMutex.unlock();

	return true;
}

bool ATU_IB_MDI::notify_marketfeed(ATU_MDI_marketfeed_struct &s)
{
	if (m_marketfeed_call_back_func != NULL) {
		return (*m_marketfeed_call_back_func)(s);
	} else {
		return false;
	}
}

void ATU_IB_MDI::error(const int p_id, const int p_errorCode, const IBString p_errorString)
{
	string errMsg = boost::lexical_cast<string>(p_errorCode) + " | " + p_errorString;
	addLog(__LOGSOURCE__, ATU_logfeed_struct::INFO, "%s", "s", errMsg.c_str());
}

void ATU_IB_MDI::addLog(string logSource, int logSeverity, char *argsfmt, char *argtype, ...) {
	va_list listPointer;
	va_start(listPointer, argtype);

	ATU_logfeed_struct *logfeed = copy2logfeed_struct(logSource, logSeverity, argsfmt, argtype, listPointer);
	notify_logfeed(logfeed);
}

}
