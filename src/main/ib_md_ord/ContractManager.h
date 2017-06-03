/*
 * ContractManager.h
 *
 *  Created on: Feb 2, 2015
 *      Author: jasonlin
 */

#ifndef CONTRACTMANAGER_H_
#define CONTRACTMANAGER_H_

#include "AtuContract.h"
#include "ContractReaderAdaptor.h"

namespace atu {

class ContractManager {
public:
	ContractManager();
	~ContractManager();
	bool extract(ContractReaderAdaptor* p_readerAdaptor);
	bool hasProductCode(const string & p_productCode) const;
  bool checkIfSecurityTypeIsCombo(const string & p_productCode);
	AtuContract* getContract(const string & p_productCode);
	string getProductCode(const string & p_contractId);
private:
	unordered_map<string, string> m_conId2ProductCode;
	unordered_map<string, AtuContract*> m_productCode2ContractPtr;
};

}
#endif /* CONTRACTMANAGER_H_ */
