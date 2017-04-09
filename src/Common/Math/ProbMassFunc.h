#ifndef PROBMASSFUNC_H_
#define PROBMASSFUNC_H_

#include "PCH.h"
#include "Constants.h"
#include "SDateTime.h"
#include "STool.h"

using namespace std;
using namespace boost;

class ProbMassFunc {
  public:
    ProbMassFunc();
    virtual ~ProbMassFunc();

    //--------------------------------------------------
    // Direct manipulation of a prob mass function
    //--------------------------------------------------
    void   Reset             (const long);
    double GetPr             (const long,const double);
    double GetPr             (const long,const double,const double);
    void   SetPr             (const long,const double,const double);
    bool   CheckValidity     (const long);
    bool   Normalize         (const long);
    void   MergeWith         (const long,const ProbMassFunc &,const long,const double);
    void   ImportButNotNmlz  (const long,const ProbMassFunc &,const long,const double);
    void   ImportAndNmlz     (const long,const ProbMassFunc &,const long,const double);
    void   ImportButNotNmlz  (const long,const ProbMassFunc &,const long,const double,const double);
    void   ImportAndNmlz     (const long,const ProbMassFunc &,const long,const double,const double);
    double GetMean           (const long);
    double GetVariance       (const long);
    double GetStdev          (const long);
    void   AdjustToStdevNmlz (const long,const double);
    string ToString          (const long);
    //--------------------------------------------------

  private:
    bool   DeriveDensity       (const long);
    void   SetIsDensityReady   (const long,const bool);
    bool   IsDensityReady      (const long);

    map<long,map<double,double> >  m_pmf;
    map<long,map<double,double> >  m_pdf;

    //--------------------------------------------------
    // Together
    //--------------------------------------------------
    map<long,bool>   m_b_IsDensityReady;
    map<long,double> m_PMF_Mean;
    map<long,double> m_PMF_Variance;

};

#endif /* PROBMASSFUNC_H_ */
