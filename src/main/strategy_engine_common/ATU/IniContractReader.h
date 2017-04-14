/*
 * IniContractReader.h
 *
 *  Created on: Feb 2, 2015
 *      Author: jasonlin
 */

#ifndef INICONTRACTREADER_H_
#define INICONTRACTREADER_H_


#include "PCH.h"
#include "Constants.h"
#include "ContractReaderAdaptor.h"

namespace atu {

class IniContractReader : public ContractReaderAdaptor {
public:
	IniContractReader(string p_filepath);
	bool read(unordered_map<string, AtuContract*>* p_productCode2ContractPtr, unordered_map<string, string>* p_conId2productCode);
private:
	string m_filepath;
};

}


#endif /* INICONTRACTREADER_H_ */
