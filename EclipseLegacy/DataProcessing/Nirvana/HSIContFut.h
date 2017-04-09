#ifndef HSICONTFUT_H_
#define HSICONTFUT_H_

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
#include "Exchange.h"
#include "HKFE.h"
#include "SDateTime.h"
#include "BarProvider.h"

using namespace std;
using namespace boost;

class HSIContFut {
  public:
    HSIContFut() {}
    virtual ~HSIContFut() {}
    void Create(const char *,const char *,const char *,const char *,const char *);
};

#endif /* HSICONTFUT_H_ */
