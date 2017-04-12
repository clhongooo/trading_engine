#ifndef ENSUREINCTIME_H_
#define ENSUREINCTIME_H_

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

using namespace std;
using namespace boost;

class EnsureIncTime {
  public:
    EnsureIncTime() {}
    virtual ~EnsureIncTime() {}
    void Convert(const char *,const char *);
};

#endif /* ENSUREINCTIME_H_ */
