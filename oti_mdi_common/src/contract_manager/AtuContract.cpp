/*
 * AtuContract.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: jasonlin
 */

#include <boost/lexical_cast.hpp>
#include "Toolbox.h"
#include "AtuContract.h"

namespace atu {

string AtuContract::get(string p_key) {
	if ( m_keyValContractDetails.find(p_key) == m_keyValContractDetails.end() ) {
		return "";
	}
	return m_keyValContractDetails[p_key];
}

vector<AtuComboLeg> AtuContract::getComboLegs() {
	vector<AtuComboLeg> vLegs;

	string strLegs = get("comboLegs");
	if ( strLegs.compare("") == 0 ) {
		return vLegs;
	}

	vector<string> legStrList;
	Toolbox::split(legStrList, strLegs, ",");

	for (vector<string>::const_iterator iter = legStrList.begin(); iter != legStrList.end(); iter++) {
		string str = *iter;     // exchange;action;ratio;product_code
		vector<string> attrs;
		Toolbox::split(attrs, str, ";");

		AtuComboLeg comboLeg;
		comboLeg.m_exchange = attrs[0];
		comboLeg.m_buyOrSell = (attrs[1] == "BUY" ? 1 : 2);
		comboLeg.m_ratio = boost::lexical_cast<int>( attrs[2].c_str() );
		comboLeg.m_productCode = attrs[3];

		vLegs.push_back(comboLeg);
	}

	return vLegs;
}

string AtuContract::set(string p_key, string p_value) {
	m_keyValContractDetails[p_key] = p_value;
	return p_value;
}

bool AtuContract::isCombo()
{
  if (get("secType") == "COMBO")
  {
    return true;
  }
  else
  {
    return false;
  }
}


}


