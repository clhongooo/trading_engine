/*
 * Utility.cpp
 *
 *  Created on: Feb 6, 2015
 *      Author: jasonlin
 */

#include "Utility.h"
#include "Contract.h"
#include "AtuContract.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

using std::vector;
using std::cout;
using std::endl;
using std::ostringstream;

namespace atu {

// /*
//  * If the passed p_host is an IP, return the IP. Otherwise, resolve the hostname to IP and return
//  */
// string Utility::resolve2IP(string p_host) {
// 	string resolvedIP = p_host;
//
// 	string regexpIPSegment = "(\\d|[1-9]\\d|1\\d\\d?|2[0-4]\\d|25[0-5])";
// 	ostringstream oss;
// 	oss << "^";
// 	for (int idx = 0; idx < 4; idx++) {
// 			oss << regexpIPSegment;
// 			if (idx < 3) oss << "\\.";
// 	}
// 	oss << "$";
// 	boost::regex regexIP(oss.str().c_str());
//
// 	if (!boost::regex_match( p_host.c_str(), regexIP )) {
// 		// resolve hostname to IP
// 		typedef boost::asio::ip::tcp::resolver boost_resolver;
// 		boost::asio::io_service ioService;
// 		boost_resolver resolver(ioService);
// 		boost_resolver::query query(p_host, "");
// 		for (boost_resolver::iterator iter = resolver.resolve(query); iter != boost_resolver::iterator(); ++iter) {
// 				boost::asio::ip::tcp::endpoint end = *iter;
// 				resolvedIP = end.address().to_string();
// 				cout << "--->>> Host IP: " << resolvedIP << endl;
// 				break;
// 		}
// 	}
//
// 	return resolvedIP;
// }

void Utility::initIBContractFromAtuContract(Contract* p_contract, AtuContract* p_atuContract, ContractManager* p_extractor) {
	p_contract->symbol = p_atuContract->get("symbol");
	p_contract->localSymbol = p_atuContract->get("localSymbol");
	p_contract->secType = p_atuContract->get("secType");
	p_contract->currency = p_atuContract->get("currency");
	p_contract->right = p_atuContract->get("right");
	p_contract->strike = boost::lexical_cast<double>(p_atuContract->get("strike"));
	string strConId = p_atuContract->get("conId");
	if (strConId != "") {
		p_contract->conId = boost::lexical_cast<long>(strConId);
	}
	p_contract->exchange = p_atuContract->get("exchange");

	// ---- combo setting start ----
	if (p_contract->secType == "COMBO") {

			Contract::ComboLegListSPtr cll(new Contract::ComboLegList());

			vector<AtuComboLeg> legList = p_atuContract->getComboLegs();
			for (vector<AtuComboLeg>::const_iterator iter = legList.begin(); iter != legList.end(); iter++) {
				AtuContract* con = p_extractor->getContract( iter->m_productCode );

				ComboLegSPtr leg(new ComboLeg());
				leg->conId = boost::lexical_cast<long>(con->get("conId"));
				leg->ratio = iter->m_ratio;
				leg->action = (iter->m_buyOrSell == 1 ? "BUY" : "SELL");
				leg->exchange = iter->m_exchange;
				leg->openClose = 0;
				leg->shortSaleSlot = 0;
				leg->designatedLocation = "";

				cll->push_back(leg);
			}

			p_contract->secType = "BAG";
			p_contract->currency = p_atuContract->get("currency");
			p_contract->comboLegs = cll;

	}

}

}


