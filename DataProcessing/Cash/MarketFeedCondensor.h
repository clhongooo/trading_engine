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
#include "SDateTime.h"

#ifndef MARKETFEEDCONDENSOR_H_
#define MARKETFEEDCONDENSOR_H_

class MarketFeedCondensor {
  public:
    MarketFeedCondensor() {}
    virtual ~MarketFeedCondensor() {}
    void Condense(const char *,const char *,const char *);
  private:
    map<string,string> m_Latest;
    HHMMSS m_LastOutputTime;
};

#endif
