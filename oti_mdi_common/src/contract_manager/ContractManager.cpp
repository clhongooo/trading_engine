/*
 * ContractManager.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: jasonlin
 */

#include "ContractManager.h"

namespace atu {

ContractManager::ContractManager() {

}

ContractManager::~ContractManager() {
	for (unordered_map<string, AtuContract*>::iterator iter = m_productCode2ContractPtr.begin(); iter != m_productCode2ContractPtr.end(); iter++) {
		delete iter->second;
	}
}

bool ContractManager::extract(ContractReaderAdaptor* p_readerAdaptor) {
	return p_readerAdaptor->read(&m_productCode2ContractPtr, &m_conId2ProductCode);
}

bool ContractManager::hasProductCode(const string & p_productCode) const {
	return m_productCode2ContractPtr.find(p_productCode) != m_productCode2ContractPtr.end();
}

AtuContract* ContractManager::getContract(const string & p_productCode) {
	if (!hasProductCode(p_productCode)) {
		return NULL;
	}
	return m_productCode2ContractPtr[p_productCode];
}

string ContractManager::getProductCode(const string & p_contractId) {
	if ( m_conId2ProductCode.find(p_contractId) == m_conId2ProductCode.end() ) {
		return "";
	}
	return m_conId2ProductCode[p_contractId];
}

bool ContractManager::checkIfSecurityTypeIsCombo(const string & p_productCode)
{
  if (hasProductCode(p_productCode))
  {
    return (getContract(p_productCode))->isCombo();
  }
  else
  {
    return false;
  }
}

}


