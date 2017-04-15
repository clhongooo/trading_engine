#ifndef UTIL_NYSE_H_
#define UTIL_NYSE_H_

#include "PCH.h"
#include "Constants.h"
#include "STool.h"
#include "SDateTime.h"

using namespace std;

class NYSE
{
  public:
    enum TRDGSESS {BEFORE_OPEN=7,CORE_SESS=1,AFT_CLOSE=9,TERMINALMKTSTS=10,UNDEFMKTSTS=99};

    static boost::shared_ptr<NYSE> Instance();
    virtual ~NYSE();
    bool IsAProductInThisMarket(const string &);

    static const double _TRDG_DAY_PER_YR;

  private:

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    NYSE();
    NYSE(const NYSE&);
    static boost::weak_ptr<NYSE> m_pInstance;

};

#endif
