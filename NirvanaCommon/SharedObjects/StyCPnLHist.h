#ifndef SHAREDOBJECTS_STYCPNLHIST_H_
#define SHAREDOBJECTS_STYCPNLHIST_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <map>
#include <ctime>
#include "SystemConfig.h"
#include "STool.h"
#include "CPnLHist.h"

class StyCPnLHist {
  public:
    static boost::shared_ptr<StyCPnLHist> Instance();
    virtual ~StyCPnLHist();

    void   LoadPersistedCPnLHistFromFile();
    void   CPnLHistWriteToFile(const int);
    void   CPnLHistAddAndTruncate(const int,const YYYYMMDD &,const double);
    double CPnLHistGetPnL(const int,const YYYYMMDD &,const YYYYMMDD &);
    double CPnLHistGetPnL(const int,const YYYYMMDD &,const int,const bool);
    double CPnLHistGetPnL(const int,const int,const bool);
    bool   CPnLHistIsDateAvb(const int,const YYYYMMDD &);
    double CPnLHistGetEstWinningProb(const int,const int);
    double CPnLHistGetEstNetOdds(const int,const int);

  private:
    void OutputCPnLHistNoLock(const int, const char);

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    StyCPnLHist();
    StyCPnLHist(StyCPnLHist const&);
    StyCPnLHist& operator=(StyCPnLHist const&){};
    static boost::weak_ptr<StyCPnLHist> m_pInstance;

    //--------------------------------------------------
    // System Objects
    //--------------------------------------------------
    boost::shared_ptr<Logger>         m_Logger;
    boost::shared_ptr<SystemConfig>   m_SysCfg;

    //--------------------------------------------------
    map<int,CPnLHist> m_map_cpnl_hist;
    shared_mutex      m_cpnl_hist_mutex;
};

#endif /* SHAREDOBJECTS_STYCPNLHIST_H_ */
