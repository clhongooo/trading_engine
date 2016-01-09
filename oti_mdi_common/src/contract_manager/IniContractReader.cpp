/*
 * IniContractReader.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: jasonlin
 */

#include <iostream>
#include "IniContractReader.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using std::cout;
using std::endl;
using boost::property_tree::ptree;
using boost::property_tree::ini_parser::read_ini;

namespace atu {

IniContractReader::IniContractReader(string p_filepath) : m_filepath(p_filepath) {
	cout << "IniContractReader created..." << endl;
}

bool IniContractReader::read(unordered_map<string, AtuContract*>* p_productCode2ContractPtr, unordered_map<string, string>* p_conId2productCode) {
	if ( m_filepath.compare("") == 0 ) {
		return false;
	}

	ptree pt;
	read_ini(m_filepath, pt);

	for (ptree::const_iterator iter = pt.begin(); iter != pt.end(); iter++) {
		string productCode = iter->first;

		cout << "[" << productCode << "]" << endl;

		AtuContract* conPtr = new AtuContract();
		conPtr->m_productCode = productCode;

		for (ptree::const_iterator sub_iter = iter->second.begin(); sub_iter != iter->second.end(); sub_iter++) {
			string key = sub_iter->first;
			string val = sub_iter->second.get_value<string>();

			cout << key << " = " << val << endl;

			conPtr->set( key, val );

			if ( key.compare("conId") == 0 ) {
				(*p_conId2productCode)[val] = iter->first;
			}
		}

		(*p_productCode2ContractPtr)[productCode] = conPtr;
	}

	return true;
}

}


