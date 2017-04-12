#ifndef UTIL_HKINTRADAYSEASONALITY_H_
#define UTIL_HKINTRADAYSEASONALITY_H_

#define TOTNUMSEASONALITYBUCKET 24

#include "PCH.h"
#include "Constants.h"
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <SDateTime.h>
#include <set>
#include "HighLow.hpp"
#include "VolSurfaces.h"
#include "MarketData.h"
#include "ContFut.h"
#include "STool.h"

using namespace std;
using namespace boost;

class HKIntradaySeasonality {
  public:
    enum HKC1INDEX {HKC1INDEX_HSI=0,HKC1INDEX_HHI=1,HKC1INDEX_TOT=2};
    virtual ~HKIntradaySeasonality();
    static boost::shared_ptr<HKIntradaySeasonality> Instance();

    int ClassifySeasonalityBucket(const HHMMSS &);
    int GetTotNumSeasonalityBucket();
    int GetNumOfMinutesInSeasonalityBucket();
    void InitAndResetHistHKItrdSeasonalBucketData();
    const map<HHMMSS,double> GetHistHKItrdSeasonalBucketData(const string &,const int);
    void UpdateHistHKItrdSeasonalBucketData(const string &,const HHMMSS &,const double);
    void AnalyzeIntrdVolatility();
    double GetExpectedHLRange(const string &,const YYYYMMDDHHMMSS &);

  private:
    void LoadTrainingResultsFromFile();
    void OutputTrainingResultsToFile();
    string GetSymbolName(const HKC1INDEX);
    string GetSymbolName(const int);
    void UpdateNmlzdHLRangePctg(const string &,const YYYYMMDD &,const int,const double);
    double GetNmlzdHLRangePctg(const string &,const YYYYMMDD &,const int);
    void CalcAvgInRollingWin(const YYYYMMDD &);
    void ResetTrainingResults();

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    HKIntradaySeasonality();
    HKIntradaySeasonality(HKIntradaySeasonality const&);
    HKIntradaySeasonality& operator=(HKIntradaySeasonality const&){};
    static boost::weak_ptr<HKIntradaySeasonality> m_pInstance;

    //--------------------------------------------------
    // System Objects
    //--------------------------------------------------
    boost::shared_ptr<Logger>           m_Logger;
    boost::shared_ptr<MarketData>       m_MarketData;
    boost::shared_ptr<VolSurfaces>      m_VolSurfaces;
    boost::shared_ptr<SystemConfig>     m_SysCfg;

    //--------------------------------------------------
    bool        m_OnOff;
    string      m_TrainingResultPath;
    int         m_WindowLength;
    set<string> m_sAllSym;
    //--------------------------------------------------
    // Data divided into intraday seasonal bucket for strategy training
    // <Time interval> <HHMMSS> <price>
    //--------------------------------------------------
    map<string,vector<map<HHMMSS,double> > > m_HistHKItrdSeasonalBucketData;
    shared_mutex                             m_HistHKItrdSeasonalBucketDataMutex;

    vector<map<YYYYMMDD,vector<double> *> > m_TrainingResults;

    //--------------------------------------------------
    // For each of the HSI and HHI, we keep the running sum of normalized volatility of each time bucket
    //--------------------------------------------------
    vector<map<YYYYMMDD,vector<double> > > m_SumInRollingWin;
    vector<map<YYYYMMDD,vector<double> > > m_AvgInRollingWin;


};

#endif /* UTIL_HKINTRADAYSEASONALITY_H_ */
