#ifndef VOLSURFACES_H_
#define VOLSURFACES_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/thread.hpp>
#include <boost/filesystem/operations.hpp>
#include <map>
#include "VolSurf.h"
#include "DaySma.h"
#include "TickReturn.h"
#include "SystemConfig.h"
#include "STool.h"
#include "BarProvider.h"
#include "MarketData.h"

class VolSurfaces {

  public:
    virtual ~VolSurfaces();
    static boost::shared_ptr<VolSurfaces> Instance();

    void UpdateUndly(const string &,const unsigned long,const double);
    void UpdateUndlIVByStrike(const string &,const unsigned long,const double,const double,const double);
    double GetIVByStrike(const string &,const unsigned long,const double);
    double GetIVByMoneyness(const string &,const unsigned long,const double);
    void FitVolSmile(const string &,unsigned long);
    // short GetOrderOfFittedPolynomial(const string &,const unsigned long);
    void ResetSurfaces(const string &);
    void UpdateNirVHSI(const YYYYMMDDHHMMSS &,const double);
    double GetNirVHSI() const;
    double GetOfficialVHSIPrevEOD() const;
    double GetOfficialVHSI() const;
    double GetOfficialVHSI(const YYYYMMDDHHMMSS &) const;
    double GetNirVHSI3DaySma();
    double GetOfficialVHSI3DaySma();
    double GetNirVHSI1DayHistReturn();
    double GetOfficialVHSI1DayHistReturn();

    //--------------------------------------------------
    // IV prediction model
    //--------------------------------------------------
    enum HSIVolSurfModelParam {theta=0,phi,psi,h,a,b1,b2,c};
    double Predict_HSI_IV();
    void LoadHSIVolSurfModelParam(const string &);
    long HashKey(const int, const double, const HSIVolSurfModelParam);

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
    VolSurfaces();
    VolSurfaces(VolSurfaces const&);
    VolSurfaces& operator=(VolSurfaces const&){};
    static boost::weak_ptr<VolSurfaces> m_pInstance;

    shared_mutex * GetVolSurfMutex(const string &);

    //--------------------------------------------------
    // VolSurf (Current)
    // Key=HSI/HHI
    //--------------------------------------------------
    map<string,VolSurf*>  m_VolSurf;
    map<string,shared_mutex*> m_VolSurfMutex;

    //--------------------------------------------------
    // Other volatility related indicators
    //--------------------------------------------------
    double m_NirVHSI;
    DaySma m_3DaySma_NirVHSI;
    DaySma m_3DaySma_OfficialVHSI;
    TickReturn m_1DayHistReturn_NirVHSI;
    TickReturn m_1DayHistReturn_OfficialVHSI;
    BarProvider m_bp_OfficialVHSI;

    //--------------------------------------------------
    // IV prediction model
    // Key   : Hash of <YYYYMM - Moneyness - param>
    // Value : Coef
    //--------------------------------------------------
    map<long,double> m_HSIVolSurfModelParam;

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<MarketData>  m_MarketData;

    //--------------------------------------------------
    // Observer
    //--------------------------------------------------
    boost::mutex m_ObserverMutex;
    boost::condition_variable m_cvDataAvb;
};

#endif
