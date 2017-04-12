#ifndef GUMBELCOPULA_H_
#define GUMBELCOPULA_H_

#include "PCH.h"
#include "Constants.h"
#include <gsl/gsl_math.h>

class GumbelCopula {
  public:
    GumbelCopula();
    virtual ~GumbelCopula();
    void SetParam(const double);
    double C(const double,const double);
    double Prob_U_smlr_u_V_eq_v(const double,const double);
  private:
    double m_theta;
    double m_delta;
};

#endif
