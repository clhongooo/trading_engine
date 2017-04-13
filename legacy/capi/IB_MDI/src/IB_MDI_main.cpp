/*
 * IB_MDI_main.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: jasonlin
 */

#include "ATU_IB_MDI.h"

using namespace atu;

extern "C" ATU_Abstract_MDI *create_object() {
	ATU_Abstract_MDI *mdi = new ATU_IB_MDI();
	return mdi;
}

extern "C" void destroy_object(ATU_Abstract_MDI *o) {
	delete o;
}


