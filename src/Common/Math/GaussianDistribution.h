#ifndef GAUSSIANDISTRIBUTION_H_
#define GAUSSIANDISTRIBUTION_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/math/distributions/normal.hpp>

using namespace boost;

class GaussianDistribution {
  public:
    GaussianDistribution();
    virtual ~GaussianDistribution();
    void SetParam(const double,const double);
    double F(const double);
    double F_inv(const double);
  private:
    boost::scoped_ptr<boost::math::normal> dist;
    double m_mean;
    double m_sd;
};

#endif
