#ifndef DAILYOHLCTOMARKETFEED_H_
#define DAILYOHLCTOMARKETFEED_H_

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>
#include <set>
#include <memory>
#include <vector>
#include <boost/ptr_container/ptr_deque.hpp>
#include <string>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "BestBidAskTrade.h"
#include "BarProvider.h"

using namespace std;
using namespace boost;

class DailyOHLCToMarketFeed {
  public:
    DailyOHLCToMarketFeed();
    virtual ~DailyOHLCToMarketFeed();
    void Convert(const char *,const char *,const char *,const char *,const char *,const char *,const char *,const char *);
};

#endif
