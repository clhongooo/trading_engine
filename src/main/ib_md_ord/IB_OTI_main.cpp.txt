/*
 * IB_OTI_main.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: jasonlin
 */

#include "ATU_IB_OTI.h"

using namespace atu;

extern "C" ATU_Abstract_OTI *create_object() {
	ATU_Abstract_OTI *oti = new ATU_IB_OTI();
	return oti;
}

extern "C" void destroy_object(ATU_Abstract_OTI *o) {
	delete o;
}


