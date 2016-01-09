#ifndef MATH_KOLMOGOROVSMIRNOV_H_
#define MATH_KOLMOGOROVSMIRNOV_H_

#include "PCH.h"
#include "Constants.h"
#include <numeric>

using namespace std;

class KolmogorovSmirnov {
  public:
    KolmogorovSmirnov();
    virtual ~KolmogorovSmirnov();

    static double TwoSmpKSStat(const vector<double> &,const vector<double> &,const int);
};

#endif /* MATH_KOLMOGOROVSMIRNOV_H_ */
