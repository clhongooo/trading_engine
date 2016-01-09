#ifndef VOLSURF_H
#define VOLSURF_H

#include "PCH.h"
#include "Constants.h"
#include "../Util/SDateTime.h"
#include "../Util/STool.h"
// #include "LeastSqRegression.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

using namespace std;
using namespace boost;

class VolSurf {
  public:
    VolSurf();
    virtual ~VolSurf();
    void Reset();
    void UpdateUndly(const unsigned long,const double);
    double GetUndly(const unsigned long);
    void UpdateUndlIVByStrike(const unsigned long,const double,const double,const double);
    void RemoveIVByStrike(const unsigned long,const double);
    double GetIVByStrike(const unsigned long,const double);
    double GetIVByMoneyness(const unsigned long,const double);
    void FitVolSmile(unsigned long);
    // vector<double> GetFittedVolSmileInMoneyness(unsigned long);
    // short GetOrderOfFittedPolynomial(const unsigned long);

  private:

    double m_Undly_Price;
    boost::shared_mutex m_VolSurfMutex;

    //--------------------------------------------------
    // Format:
    // YYYYMMDD     - Maturity Date
    // double       - Strike
    // double       - Implied Volatility
    //--------------------------------------------------
    map<unsigned long, map<double,double> >   m_VolSurfInStrikes;

    //--------------------------------------------------
    // LeastSqRegression approach doesn't work, polynomial of deg 20 still dont fit!?!
    //--------------------------------------------------
    // //--------------------------------------------------
    // // Format:
    // // YYYYMMDD     - Maturity Date
    // // double       - coef of polynomial. v[0] = constant term, v[1] = x's coef etc.
    // //--------------------------------------------------
    // map<unsigned long, vector<double> >       m_FittedVolSmileInMoneyness;

    //--------------------------------------------------
    // Curve-fitting using GSL
    //--------------------------------------------------
    // Format:
    // YYYYMMDD     - Maturity Date
    // gsl_interp_accel
    // gsl_spline
    //--------------------------------------------------
    map<unsigned long, gsl_interp_accel* >  m_GSL_Interp_Accel;
    map<unsigned long, gsl_spline*       >  m_GSL_Spline;

};

#endif /* VOLSURF_H */
