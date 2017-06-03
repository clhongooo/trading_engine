/*
 * JsonContractReader.h
 *
 *  Created on: Feb 3, 2015
 *      Author: jasonlin
 */

#ifndef JSONCONTRACTREADER_H_
#define JSONCONTRACTREADER_H_


#include "ContractReaderAdaptor.h"

namespace atu {

class JsonContractReader : public ContractReaderAdaptor {
public:
	JsonContractReader(string p_filepath);
	bool read(unordered_map<string, AtuContract*>* p_productCode2ContractPtr, unordered_map<string, string>* p_conId2productCode);
private:
	string m_filepath;
};

}


#endif /* JSONCONTRACTREADER_H_ */
