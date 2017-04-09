#ifndef TECHINDICATORS_H_
#define TECHINDICATORS_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include "Logger.h"
#include "BarAggregator.h"
#include "SDateTime.h"
#include "SystemConfig.h"
#include "GKYZ.h"
#include "FSMC.h"
#include "FSMC2.h"
#include "Sma.hpp"
#include "Ema.hpp"
#include "TickReturn.h"
#include "MeanAccumulator.h"
#include "OHLC.h"
#include "HKFE.h"

using namespace std;
using namespace boost;

class TechIndicators
{
  public:
    virtual ~TechIndicators();
    static boost::shared_ptr<TechIndicators> Instance();
    void UpdateTechIndicators();
    GKYZ GetGKYZClone(const string &);
    void AddBarGKYZ(const string &,double,double,double,double);
    double GetGKYZValue(const string &);
    bool GetHistGKYZ(const string &,const int,deque<double> &);
    bool GetHistGKYZDiff(const string &,const int,deque<double> &);
    bool GetHistGKYZImpl(const string &, const int, deque<double> &, const map<string,deque<double> > &);
    double GetEstGKYZValue(const string &);
    double GetHistGKYZPctgChange(const string &);
    void AddFSMC(const string &,double,double,double,double);
    int GetFSMCState(const string &);
    void AddMinuteBarSma(const string &,const int,const double);
    double GetMinuteBarSma(const string &,const int);
    void AddlnHSIHHIRatioSma(const int,const double);
    double GetlnHSIHHIRatioSma(const int);
    void AddlnHSIHHIRatioEma(const int,const double);
    double GetlnHSIHHIRatioEma(const int);
    void AddTickReturn(const string &,const int,const YYYYMMDDHHMMSS &,const double);
    double GetLnTickReturn(const string &,const int);
    void Add1MinAvgPrice(const string &,const double);
    double Get1MinAvgPrice(const string &);
    void Reset1MinAvgPrice(const string &);
    void AddDayOHLC(const string &,const double);
    void AddDayOHLC(const string &,const double,const double,const double,const double);
    double GetDayOpen(const string &);
    double GetDayHigh(const string &);
    double GetDayLow(const string &);
    double GetDayClose(const string &);
    void ResetDayOHLC(const string &);

    //--------------------------------------------------
    // Observer
    //--------------------------------------------------
    void WaitForData();
    void NotifyConsumers();
    //--------------------------------------------------

  private:
    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    TechIndicators();
    TechIndicators(TechIndicators const&);
    TechIndicators& operator=(TechIndicators const&){};
    static boost::weak_ptr<TechIndicators> m_pInstance;


    //--------------------------------------------------
    // System Objects
    //--------------------------------------------------
    boost::shared_ptr<Logger>       m_Logger;
    boost::shared_ptr<SystemConfig> m_SysCfg;
    boost::shared_ptr<HKFE>         m_HKFE;

    //--------------------------------------------------
    // The technical indicators
    //--------------------------------------------------
    map<string,GKYZ*>           m_GKYZ;
    map<string,deque<double> >  m_HistGKYZ;
    map<string,deque<double> >  m_HistGKYZDiff;
    shared_mutex                m_GKYZMutex;

    map<string,FSMC*>           m_FSMC;
    shared_mutex                m_FSMCMutex;

    map<string,Sma<double>*>    m_MinuteBarSma;
    shared_mutex                m_MinuteBarSmaMutex;

    map<string,TickReturn*>     m_TickReturn;
    shared_mutex                m_TickReturnMutex;

    map<int,Sma<double>*>       m_lnHSIHHIRatioSma;
    shared_mutex                m_lnHSIHHIRatioSmaMutex;

    map<int,Ema<double>*>       m_lnHSIHHIRatioEma;
    shared_mutex                m_lnHSIHHIRatioEmaMutex;

    map<string,MeanAccumulator*>m_1MinAvgPrice;
    shared_mutex                m_1MinAvgPriceMutex;

    map<string,OHLC>            m_DayOHLC;
    shared_mutex                m_DayOHLCMutex;

    //--------------------------------------------------
    // Observer
    //--------------------------------------------------
    boost::mutex m_ObserverMutex;
    boost::condition_variable m_cvDataAvb;
};

#endif
