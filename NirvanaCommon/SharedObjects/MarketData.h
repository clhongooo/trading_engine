#ifndef MARKETDATA_H_
#define MARKETDATA_H_

#include "PCH.h"
#include "SFunctional.h"
#include "Constants.h"
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include "ATU_Abstract_MDI.h"
#include "Logger.h"
#include "BarAggregator.h"
#include "BarProvider.h"
#include "SDateTime.h"
#include "SystemConfig.h"
#include "HKFE.h"
#include "HKSE.h"
#include "NYSE.h"
#include "CME.h"
#include "ContFut.h"
#include "MDI_Acknowledgement.h"
#include "BarProvider.h"
#include "Exchange.h"

using namespace std;
using namespace boost;

class MarketData
{
  public:
    virtual ~MarketData();
    static boost::shared_ptr<MarketData> Instance();
    bool UpdateMarketData(const string &);
    bool UpdateMarketData(const ATU_MDI_marketfeed_struct &);
    YYYYMMDDHHMMSS GetSystemTimeHKT();
    YYYYMMDDHHMMSS GetSystemTimeEST();
    bool GetLatestBestBidAskMid(const string &,double &,double &,double &,YYYYMMDDHHMMSS &);
    bool GetLatestMidQuote(const string &,double &);
    bool GetLatestMidQuote(const string &,double &,YYYYMMDDHHMMSS &);
    bool GetLatestNominalPrice(const string &,double &);
    bool GetLatestTradePrice(const string &,double &,YYYYMMDDHHMMSS &);
    bool GetLatestTradePrice(const string &,double &);
    bool Get1MinOHLCBar(const string &,YYYYMMDD &,HHMMSS &,double &,double &,double &,double &,long &);
    bool Get1DayOHLCBar(const string &,YYYYMMDD &,HHMMSS &,double &,double &,double &,double &,long &);
    bool Peek1MinOHLCBar(const string &,YYYYMMDD &,HHMMSS &,double &,double &,double &,double &,long &);
    bool Peek1DayOHLCBar(const string &,YYYYMMDD &,HHMMSS &,double &,double &,double &,double &,long &);
    long GetLastAccumTradeVolume(const string &);
    void CleanUpOldSnapshots();
    void DebugPrintBarAgg(const string &);
    void GetSuppD1BarOHLCVInDateRange(const string &,const YYYYMMDD &,const YYYYMMDD &,vector<YYYYMMDD> &,vector<double> &,vector<double> &,vector<double> &,vector<double> &,vector<long> &);
    void GetSuppM1BarOHLCVInDateTimeRange(const string &,const YYYYMMDD &,const HHMMSS &,const YYYYMMDD &,const HHMMSS &,vector<YYYYMMDD> &,vector<HHMMSS> &,vector<double> &,vector<double> &,vector<double> &,vector<double> &,vector<long> &);
    void GetUpdatedSymbols(set<string> &);
    void ClearUpdatedSymbols();
    bool CheckIfSymbolIsUpdated(const string &);

    //--------------------------------------------------
    // Observer
    //--------------------------------------------------
    void WaitForData();
    void NotifyConsumers();
    //--------------------------------------------------


  private:
    bool   ParseMDIString(const string &, vector<string> &, YYYYMMDD &, HHMMSS &);
    void   UpdateInternalDataWithParsedMDIString(const vector<string> &, const YYYYMMDD &, const HHMMSS &);
    void   UpdateInternalDataWithParsedMDIString(const ATU_MDI_marketfeed_struct &, const YYYYMMDD &, const HHMMSS &);
    void   UpdateSystemTime(const YYYYMMDD &,const HHMMSS &);
    void   UpdateBarAggregators(const YYYYMMDD &,const HHMMSS &,const string &,const double,const long);
    void   UpdateBarAggregatorsAndLatestSnapshotContFut(const YYYYMMDDHHMMSS &,const ATU_MDI_marketfeed_struct & sMD);
    void   UpdateLatestSnapshot(const YYYYMMDD &,const HHMMSS &,const ATU_MDI_marketfeed_struct &);
    void   UpdateLatestSnapshot(const YYYYMMDD &,const HHMMSS &,const ATU_MDI_marketfeed_struct & sMD,const double);
    void   UpdateLastAccumTradeVolume(const string &,const long);
    void   AddUpdatedSymbol(const string &);
    double ComputeTickReturn(const string &,const double);
    double GetTradePriceOrElseWeightedMidQuote(const ATU_MDI_marketfeed_struct &) const;
    double GetWeightedMidQuote(const ATU_MDI_marketfeed_struct &) const;
    boost::optional<string> GetSuppBarOHLCVPath(const string &, const string &);

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    MarketData();
    MarketData(MarketData const&);
    MarketData& operator=(MarketData const&){};
    static boost::weak_ptr<MarketData> m_pInstance;

    //--------------------------------------------------
    // System Objects
    //--------------------------------------------------
    boost::shared_ptr<Logger>       m_Logger;
    boost::shared_ptr<SystemConfig> m_SysCfg;
    boost::shared_ptr<Exchange>     m_Exchg;
    boost::shared_ptr<HKFE>         m_HKFE;
    boost::shared_ptr<HKSE>         m_HKSE;
    boost::shared_ptr<NYSE>         m_NYSE;
    boost::shared_ptr<CME>          m_CME;
    boost::shared_ptr<MDI_Acknowledgement>  m_MDIAck;

    //--------------------------------------------------
    // Mutex
    //--------------------------------------------------
    shared_mutex * Get1MinBarsMutex(const string &);
    shared_mutex * Get1DayBarsMutex(const string &);
    shared_mutex * GetLatestSnapshotsMutex(const string &);
    shared_mutex * GetLastAccumTradeVolumeMutex(const string &);

    //--------------------------------------------------
    // System Time
    //--------------------------------------------------
    YYYYMMDDHHMMSS m_MDITimeHKT_YYYYMMDDHHMMSS;
    YYYYMMDDHHMMSS m_MDITimeEST_YYYYMMDDHHMMSS;
    YYYYMMDDHHMMSS m_LastPrintedMDITime_YYYYMMDDHHMMSS;
    YYYYMMDDHHMMSS m_LastMonitorTime;
    shared_mutex   m_MDITime_Mutex;

    //--------------------------------------------------
    // Set of updated symbols
    //--------------------------------------------------
    set<string> m_UpdatedSymbols;
    shared_mutex m_UpdatedSymbolsMutex;

    typedef struct TupMDIStructTS {
      bool m_isReady;
      ATU_MDI_marketfeed_struct m_mdi_struct;
      YYYYMMDDHHMMSS m_yyyymmddhhmmss;
      TupMDIStructTS () { m_isReady = false; }
      bool IsReady() { return m_isReady; }
      void SetAsReady() { m_isReady = true; }
      void SetAsNotReady() { m_isReady = false; }
    } TupMDIStructTS;

    //--------------------------------------------------
    // Latest Snapshot
    //--------------------------------------------------
    map<string,TupMDIStructTS>   m_LatestSnapshots;
    map<string,shared_mutex *>   m_LatestSnapshotsMutex;

    //--------------------------------------------------
    // Accumulated trading volume
    //--------------------------------------------------
    map<string,long>             m_LastAccumTradeVolume;
    map<string,shared_mutex *>   m_LastAccumTradeVolumeMutex;

    //--------------------------------------------------
    // Accumulated day bars
    //--------------------------------------------------
    map<string,BarAggregator *>  m_1DayBarAgg;
    map<string,BarProvider *>    m_1DayBarProv;
    map<string,shared_mutex *>   m_1DayBarsMutex;

    //--------------------------------------------------
    // Accumulated minute bars
    //--------------------------------------------------
    map<string,BarAggregator *>  m_1MinBarAgg;
    map<string,BarProvider *>    m_1MinBarProv;
    map<string,shared_mutex *>   m_1MinBarsMutex;

    //--------------------------------------------------
    // Continuous Futures HKFE
    //--------------------------------------------------
    boost::scoped_ptr<ContFut>  m_ContFut;

    //--------------------------------------------------
    // Supplementary D1 Bar
    //--------------------------------------------------
    map<string,BarProvider*>   m_SupplementaryDayBar;
    shared_mutex               m_SupplementaryDayBarMutex;

    //--------------------------------------------------
    // Supplementary M1 Bar
    //--------------------------------------------------
    map<string,BarProvider*>   m_SupplementaryM1Bar;
    shared_mutex               m_SupplementaryM1BarMutex;

    //--------------------------------------------------
    // Observer
    //--------------------------------------------------
    boost::mutex m_ObserverMutex;
    boost::condition_variable m_cvDataAvb;
    //--------------------------------------------------

};

#endif
