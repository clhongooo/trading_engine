/*
 * ContractReaderAdaptor.h
 *
 *  Created on: Feb 2, 2015
 *      Author: jasonlin
 */

#ifndef CONTRACTREADERADAPTOR_H_
#define CONTRACTREADERADAPTOR_H_

#include "AtuContract.h"

namespace atu {

class ContractReaderAdaptor {
public:
	virtual bool read(unordered_map<string, AtuContract*>* p_productCode2ContractPtr, unordered_map<string, string>* p_conId2productCode) = 0;
};

}


#endif /* CONTRACTREADERADAPTOR_H_ */
