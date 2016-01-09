#include "Util/BestBidAskTrade.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <memory>
#include <vector>
#include <boost/ptr_container/ptr_deque.hpp>
#include <string>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>

#ifndef MARKETFEEDMERGER_H_
#define MARKETFEEDMERGER_H_

class MarketFeedMerger {
public:
	MarketFeedMerger() {}
	virtual ~MarketFeedMerger() {}
    void Merge(const char *,const char *,const char *);
};

#endif /* MARKETFEEDMERGER_H_ */
