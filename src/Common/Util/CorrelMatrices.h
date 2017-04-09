#ifndef CORRELMATRICES_H_
#define CORRELMATRICES_H_

#include "PCH.h"
#include "Constants.h"
#include <map>
#include <boost/filesystem/operations.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include "ATU_Abstract_MDI.h"
#include "Logger.h"
#include "SDateTime.h"

using namespace std;
using namespace boost;

class CorrelMatrices
{
  public:
    virtual ~CorrelMatrices();
    static boost::shared_ptr<CorrelMatrices> Instance();
    void LoadCorrelMatrices(const string &);
    double GetCorrel(const YYYYMMDD &,const string &,const string &);
    double GetCorrel(const int,const string &,const string &);

  private:
    string GenerateCombinedKey(const string &, const string &);

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    CorrelMatrices();
    CorrelMatrices(CorrelMatrices const&);
    CorrelMatrices& operator=(CorrelMatrices const&){};
    static boost::weak_ptr<CorrelMatrices> m_pInstance;

    //--------------------------------------------------
    // System Objects
    //--------------------------------------------------
    boost::shared_ptr<Logger>       m_Logger;

    //--------------------------------------------------
    // Correl Matrices
    // YYYYMMDD - SymA_SymB - Correl
    //--------------------------------------------------
    map<int,map<string,double> > m_CorrelMatrices;
    shared_mutex                 m_Mutex;

};

#endif
