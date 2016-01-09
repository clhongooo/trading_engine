#ifndef UTIL_COMMISSIONCALCULATOR_H_
#define UTIL_COMMISSIONCALCULATOR_H_

#include "PCH.h"
#include "Constants.h"

using namespace std;

class CommissionCalculator {
  public:
    enum BROKER {
      IB,
      CHIEF,
      BRIGHTSMART,
      ABN
    };
    enum MARKET {
      HKSTK,
      HKIDX,
      USSTK,
      USIDX,
      CNSTK,
      CNIDX,
      JPNIDX,
      JPNSTK,
      AUSIDX,
      AUSSTK,
      FX
    };
    enum PRODUCT {FUTURES,OPTIONS,SPOT};
    CommissionCalculator();
    virtual ~CommissionCalculator();

    static double CalcCommission(const BROKER,const MARKET,const PRODUCT,const double,const double);
    static bool PaysMoreThanMinCommission(const BROKER,const MARKET,const PRODUCT,const double,const double);
    static long GetTrdgUnitConsidergMinComm(const BROKER,const MARKET,const PRODUCT);
};

#endif /* UTIL_COMMISSIONCALCULATOR_H_ */
