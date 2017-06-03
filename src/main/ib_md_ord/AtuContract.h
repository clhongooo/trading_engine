/*
 * AtuContract.h
 *
 *  Created on: Feb 2, 2015
 *      Author: jasonlin
 */

#ifndef ATUCONTRACT_H_
#define ATUCONTRACT_H_

#include <tr1/unordered_map>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

using std::vector;
using std::string;
using std::tr1::unordered_map;

namespace atu {

typedef struct {
	string m_exchange;
	int m_buyOrSell;
	int m_ratio;
	string m_productCode;
} AtuComboLeg;

class AtuContract {
public:
	string m_productCode;
	string get(string p_key);
	bool isCombo();
	vector<AtuComboLeg> getComboLegs();
	string set(string p_key, string p_value);
private:
	unordered_map<string, string> m_keyValContractDetails;
};

}


#endif /* CONTRACT_H_ */
