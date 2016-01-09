#include "ATU_Abstract_AlgoI.h"
#include "TradingEngineAlgoI.h"

extern "C" ATU_Abstract_AlgoI *create_object() {
	ATU_Abstract_AlgoI *algoi = new TradingEngineAlgoI();
	return algoi;
}

extern "C" void destroy_object(ATU_Abstract_AlgoI *o) {
	delete o;
}
