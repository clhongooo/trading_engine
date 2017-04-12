#ifndef HKSE_H
#define HKSE_H

#include "PCH.h"
#include "Constants.h"
#include <boost/filesystem/operations.hpp>
#include "STool.h"
#include "SDateTime.h"
#include "SLowerBound.h"
#include "Tokenizer.h"
#include "ValidityChk.h"
#include "CustType.h"

using namespace std;

class HKSE
{
  public:
    enum SecurityType {STOCK,ETF,WARRANT,CBBC};
    enum TRDGSESS {BEFORE_OPEN=7,MRNG_SESS=1,LUNCH_BRK=8,AFTN_SESS=2,AFT_CLOSE=9,TERMINALMKTSTS=10,UNDEFMKTSTS=99};

    bool IsAProductInThisMarket(const string &);
    static boost::shared_ptr<HKSE> Instance();
    virtual ~HKSE();

    void                 LoadHSIConstituents(const string &);
    map<string,double> * GetHSIConstituents(const YYYYMMDD &);

  private:

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    HKSE();
    HKSE(const HKSE&);
    // HKSE& operator=(const HKSE&) {};
    static boost::weak_ptr<HKSE> m_pInstance;

    //--------------------------------------------------
    // YYYYMMDD - Stock Code - Weight
    //--------------------------------------------------
    map<int,map<string,double> > m_HSI_constituents;
};

#endif
