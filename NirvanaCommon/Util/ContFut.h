#ifndef CONTFUTHKFE_H_
#define CONTFUTHKFE_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/thread.hpp>
#include "ATU_Abstract_MDI.h"
#include <set>
#include <vector>
#include "Exchange.h"
#include "HKFE.h"
#include "CME.h"
#include "SystemConfig.h"

class ContFut {
  public:
    ContFut();
    virtual ~ContFut();

    static bool IsSymContFut(const string &);
    static string GetContFutUndly(const string &);
    static string GetContFutFrRglrSym(const string &,const int);
    static string GetContFutFrUndly(const string &,const int);

    void Add(const YYYYMMDDHHMMSS &, const ATU_MDI_marketfeed_struct &);
    bool GetContFutLatestSnapshot(const string &,ATU_MDI_marketfeed_struct &, YYYYMMDDHHMMSS &);
    bool GetContFutLatestSnapshot(const string &,ATU_MDI_marketfeed_struct &);
    bool DoesSymHaveContFut(const string &);
    set<string> GetAvbContFut();

  private:
    void UpdateContFutLatestSnapshot(const YYYYMMDDHHMMSS &, const string &,const ATU_MDI_marketfeed_struct &,const double);
    bool CheckIfContFutAlrRolled(const string &,const YYYYMMDD &);
    double GetContFutAdjustment(const string &);

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<Exchange>      m_Exchg;
    boost::shared_ptr<HKFE>          m_HKFE;
    boost::shared_ptr<CME>           m_CME;
    boost::shared_ptr<SystemConfig>  m_SysCfg;
    //--------------------------------------------------

    shared_mutex                             m_ContFutMutex;

    set<string>                              m_SymList;
    map<string,set<YYYYMMDD> >               m_ContFutAlrRolled;
    map<string,double>                       m_ContFutAdjustment;
    map<string,ATU_MDI_marketfeed_struct>    m_LatestSnapshot_Sym_1FM;
    map<string,YYYYMMDDHHMMSS>               m_LatestSnapshotUpdateTime_Sym_1FM;
    map<string,ATU_MDI_marketfeed_struct>    m_LatestSnapshot_Sym_2FM;
    map<string,YYYYMMDDHHMMSS>               m_LatestSnapshotUpdateTime_Sym_2FM;
    map<string,ATU_MDI_marketfeed_struct>    m_LatestSnapshot_Sym_c1;
    map<string,YYYYMMDDHHMMSS>               m_LatestSnapshotUpdateTime_Sym_c1;
    map<string,bool>                         m_Sym_c1Ready;

};

#endif
