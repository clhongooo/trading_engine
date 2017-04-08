#ifndef PROBDISTRIBUTIONS_H_
#define PROBDISTRIBUTIONS_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/thread.hpp>
#include <boost/filesystem/operations.hpp>
#include <map>
#include "DaySma.h"
#include "TickReturn.h"
#include "STool.h"
#include "ProbMassFunc.h"

class ProbDistributions {
  public:
    virtual ~ProbDistributions();
    static boost::shared_ptr<ProbDistributions> Instance();
    void SetPMF(const string &,const long,const ProbMassFunc &);
    bool GetPMF(const string &,const long,ProbMassFunc &);

  private:
    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    ProbDistributions();
    ProbDistributions(ProbDistributions const&);
    ProbDistributions& operator=(ProbDistributions const&){};
    static boost::weak_ptr<ProbDistributions> m_pInstance;

    //--------------------------------------------------
    shared_mutex * GetProbDistrnMutex(const string &);

    //--------------------------------------------------
    // Prob Mass functions
    //--------------------------------------------------
    map<string,ProbMassFunc*> m_pmf;
    map<string,shared_mutex*> m_ProbDistrnMutex;
};

#endif
