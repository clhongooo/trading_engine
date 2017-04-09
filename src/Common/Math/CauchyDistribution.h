#ifndef CAUCHYDISTRIBUTION_H_
#define CAUCHYDISTRIBUTION_H_

#include "PCH.h"
#include "Constants.h"
#include <gsl/gsl_math.h>

class CauchyDistribution {
  public:
    CauchyDistribution();
    virtual ~CauchyDistribution();
    void SetParam(const double,const double);
    double F(const double);
    double F_inv(const double);
  private:
    double m_location;
    double m_scale;
};

#endif
