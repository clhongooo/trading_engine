#ifndef TOBBAT_H_
#define TOBBAT_H_

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

class ToBBAT {
  public:
    ToBBAT() {}
    virtual ~ToBBAT() {}
    void Convert(const char *,const char *,const char *);
};

#endif /* TOBBAT_H_ */
