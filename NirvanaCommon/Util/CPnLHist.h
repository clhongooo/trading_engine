#ifndef UTIL_CPNLHIST_H_
#define UTIL_CPNLHIST_H_

#include "PCH.h"
#include "Constants.h"
#include "SDateTime.h"
#include "STool.h"

class CPnLHist {
  public:
    CPnLHist();
    void   Reset();
    virtual ~CPnLHist();
    //--------------------------------------------------
    // File I/O
    //--------------------------------------------------
    bool   LoadFromFile(const string &);
    void   WriteToFile(const string &);

    //--------------------------------------------------
    // Set
    //--------------------------------------------------
    void   AddAndTruncate(const YYYYMMDD &,const double);

    //--------------------------------------------------
    // Get
    //--------------------------------------------------
    void   GetInternalData(map<string,double> &);
    bool   IsDateAvailable(const YYYYMMDD &);
    double GetPnL(const YYYYMMDD &,const YYYYMMDD &);
    double GetPnL(const YYYYMMDD &,const int,const bool);
    double GetPnL(const int,const bool);
    double GetEstWinningProb(const int);
    double GetEstNetOdds(const int);
    //--------------------------------------------------

  private:
    typedef struct {
      double m_cpnl;
      long   m_idx;
    } cpnl_idx_tuple;

    bool ProduceWinLossSeries(const int,vector<double> &,vector<double> &);

    map<YYYYMMDD,cpnl_idx_tuple> m_map_hist_cpnl;
};

#endif /* UTIL_CPNLHIST_H_ */
