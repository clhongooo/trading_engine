#ifndef MARKETFEEDTOOHLC_H_
#define MARKETFEEDTOOHLC_H_

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
#include <set>
#include <memory>
#include <vector>
#include <boost/ptr_container/ptr_deque.hpp>
#include <string>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

class MarketFeedToOHLC {
  public:
    MarketFeedToOHLC();
    virtual ~MarketFeedToOHLC();
    void Convert(const char *,const char *,const char *);
};

#endif
