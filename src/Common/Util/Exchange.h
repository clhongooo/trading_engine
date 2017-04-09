#ifndef UTIL_EXCHANGE_H_
#define UTIL_EXCHANGE_H_

#include "PCH.h"
#include "Constants.h"
#include "HKFE.h"
#include "HKSE.h"
#include "NYSE.h"
#include "CME.h"
#include "SDateTime.h"
#include "TradingHours.h"

using namespace std;
using namespace boost;

class Exchange {
  public:
    EExchange GetPrimaryExchange(const string &);
    void LoadTradingHours(const string &);
    bool IsTradingHour(const string &, const YYYYMMDDHHMMSS &);
    bool IsTradingHour(const string &, const YYYYMMDDHHMMSS &, const int, const int, const int);
    HHMMSS GetTimeNSecBefClose(const string &, const YYYYMMDD &, const int);
    HHMMSS GetTimeNSecAftOpen(const string &, const YYYYMMDD &, const int);
    bool IsNewDay(const string &, const YYYYMMDDHHMMSS &, const YYYYMMDDHHMMSS &);
    bool IsRegularFutCtrt(const string &);
    bool IsMiniFutCtrt(const string &);
    string GetMiniFrRglrFutCtrt(const string &);
    double GetContractMultiplier(const string &);

    static boost::shared_ptr<Exchange> Instance();
    virtual ~Exchange();

  private:
    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    Exchange();
    Exchange(const Exchange&);
    // Exchange& operator=(const Exchange&) {};
    static boost::weak_ptr<Exchange> m_pInstance;

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<HKFE>  m_HKFE;
    boost::shared_ptr<HKSE>  m_HKSE;
    boost::shared_ptr<NYSE>  m_NYSE;
    boost::shared_ptr<CME>   m_CME;
    //--------------------------------------------------
    map<string,EExchange> m_CachedPrimaryExchg;
    TradingHours m_TrdgHr;

    //--------------------------------------------------
    // NewDay
    //--------------------------------------------------
    HHMMSS m_HardDayClose;
    SDateTime::TIMEZONE m_DefaultTimeZone;
};

#endif /* UTIL_EXCHANGE_H_ */
