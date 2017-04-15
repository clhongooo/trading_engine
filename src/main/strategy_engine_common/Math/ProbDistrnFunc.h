#ifndef PROBDISTRNFUNC_H_
#define PROBDISTRNFUNC_H_

#include "PCH.h"
#include "Constants.h"
#include "SDateTime.h"
#include "STool.h"

using namespace std;
using namespace boost;

class ProbDistrnFunc {
  public:
    enum { PRECISION = 3 };
    ProbDistrnFunc();
    virtual ~ProbDistrnFunc();

    //--------------------------------------------------
    // Direct manipulation of a prob distrn function
    //--------------------------------------------------
    void   Reset               (){}
    void   Reset               (const unsigned long){}
    double GetPr               (const unsigned long,const int,const unsigned long);
    void   SetPr               (const unsigned long,const int,const unsigned long,const unsigned long){}
    double GetPr               (const unsigned long,const double,const double);
    void   SetPr               (const unsigned long,const double,const double,const double);
    bool   CheckValidity       (const unsigned long);
    void   Normalize           (const unsigned long){}
    bool   SaveDistrnFuncToFile(const char *, const unsigned long);
    bool   LoadDistrnFuncFrFile(const char *, const unsigned long);

  private:
    //--------------------------------------------------
    // Format:
    // YYYYMMDDHHMM - As at Date
    // double       - Price return
    // double       - Cum Probability
    // Reading probability should be much more frequent than updating
    //--------------------------------------------------
    map<unsigned long, map<unsigned long,unsigned long> >     m_CumProb;
    map<unsigned long, boost::shared_mutex*>                  m_CumProbMutex;

};

//--------------------------------------------------
// Prob Distribution Function Builder
//--------------------------------------------------
class ProbDistrnFuncBld {

  public:
    enum { PRECISION = 3 };
    enum Method { LINEAR, QUADRATIC, CUBICSPLINE };
    ProbDistrnFuncBld();
    virtual ~ProbDistrnFuncBld();

    //--------------------------------------------------
    // For building a new prob distrn by collecting stat
    //--------------------------------------------------
    unsigned long GetAllCounts          (const unsigned long);
    void          AddCount              (const unsigned long,const int){}
    void          AddCount              (const unsigned long,const double){}
    void          EraseAllCounts        (){}
    void          EraseCounts           (const unsigned long){}
    void          SetLowerBound         (const unsigned long,const unsigned long){}
    void          SetUpperBound         (const unsigned long,const unsigned long){}
    void          SetLowerBound         (const unsigned long,const double){}
    void          SetUpperBound         (const unsigned long,const double){}
    bool          GenerateDistrnFunc    (Method, const unsigned long);
    bool          GetProbDistrnFunc     (const unsigned long,ProbDistrnFunc &);

  private:
    //--------------------------------------------------
    // Format:
    // YYYYMMDDHHMM - As at Date
    // double       - Price return
    // double       - Cum Probability
    // Reading probability should be much more frequent than updating
    //--------------------------------------------------
    map<unsigned long, map<unsigned long,unsigned long> >      m_CumProb;
    map<unsigned long, boost::shared_mutex*>                   m_CumProbMutex;

};

#endif /* PROBDISTRNFUNC_H_ */
