#ifndef UTIL_CME_H_
#define UTIL_CME_H_

#include "PCH.h"
#include "Constants.h"
#include "STool.h"
#include "SDateTime.h"

class CME
{
  public:
    enum ContractType {FUTURES,CALL,PUT};
    enum TRDGSESS {BEFORE_OPEN=7,CORE_SESS=1,AFT_CLOSE=9,TERMINALMKTSTS=10,UNDEFMKTSTS=99};
    static const double _TRDG_DAY_PER_YR;

    static boost::shared_ptr<CME> Instance();
    virtual ~CME();
    bool IsAProductInThisMarket(const string &);

    double GetContractMultiplier(const string &);
    string GetCtrtCode1(const ContractType, const YYYYMMDD & ymd, const int);
    string GetCtrtCode2(const ContractType, const YYYYMMDD & ymd, const int);
    bool IsRollDate(const YYYYMMDD &);

  private:

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    CME();
    CME(const CME&);
    static boost::weak_ptr<CME> m_pInstance;

    int CalcRollDay(const YYYYMMDD &);

};

#endif /* UTIL_CME_H_ */
