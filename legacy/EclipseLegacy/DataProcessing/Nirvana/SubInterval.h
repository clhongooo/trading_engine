#ifndef SUBINTERVAL_H_
#define SUBINTERVAL_H_

#include <boost/cstdint.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace boost;

class SubInterval {
  public:
    SubInterval();
    virtual ~SubInterval();
    void Convert(const char *,const char *,const char *);
};

#endif
