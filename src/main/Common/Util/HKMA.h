#ifndef HKMA_H
#define HKMA_H

#include "PCH.h"
#include "Constants.h"
#include "SDateTime.h"

using namespace std;

class HKMA
{
  public:
    static boost::shared_ptr<HKMA> Instance();
    virtual ~HKMA();

    void LoadExchgFundBill(const string &);
    bool GetExchgFundBillAnnlzd(const YYYYMMDD &,double &);

  private:

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    HKMA();
    HKMA(HKMA const&);
    HKMA& operator=(HKMA const&){ return *this; };
    static boost::weak_ptr<HKMA> m_pInstance;

    map<int,double> m_ExchgFundBill;

};

#endif
