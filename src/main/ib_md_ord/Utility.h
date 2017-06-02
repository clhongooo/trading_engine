/*
 * Utility.h
 *
 *  Created on: Feb 6, 2015
 *      Author: jasonlin
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "Contract.h"
#include "AtuContract.h"
#include "ContractManager.h"
#include <string>

using std::string;

namespace atu {

class Utility {
public:
	static string resolve2IP(string p_host);
	static void initIBContractFromAtuContract(Contract* p_contract, AtuContract* p_ppAtuContract, ContractManager* p_extractor);
};

}


#endif /* UTILITY_H_ */
