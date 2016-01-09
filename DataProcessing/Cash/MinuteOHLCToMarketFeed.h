#ifndef MINUTEOHLCTOMARKETFEED_H_
#define MINUTEOHLCTOMARKETFEED_H_

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
#include <deque>
#include <string>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "BestBidAskTrade.h"
#include "SDateTime.h"
#include "STool.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

class MinuteOHLCToMarketFeed {
  public:
    MinuteOHLCToMarketFeed();
    virtual ~MinuteOHLCToMarketFeed();
    void Convert(const char *,const char *,const char *,const char *,const char *,const char *,const char *,const char *,const char *);
};

#endif
